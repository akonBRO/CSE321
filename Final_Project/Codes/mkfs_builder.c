
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <time.h>
#include <assert.h>


#define BS 4096u                        
#define INODE_SIZE 128u
#define ROOT_INO 1u
#define DIRECT_MAX 12


#pragma pack(push, 1)
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t block_size;
    uint64_t total_blocks;
    uint64_t inode_count;
    uint64_t inode_bitmap_start;
    uint64_t inode_bitmap_blocks;
    uint64_t data_bitmap_start;
    uint64_t data_bitmap_blocks;
    uint64_t inode_table_start;
    uint64_t inode_table_blocks;
    uint64_t data_region_start;
    uint64_t data_region_blocks;
    uint64_t root_inode;
    uint64_t mtime_epoch;
    uint32_t flags;
    uint32_t checksum;
} superblock_t;
#pragma pack(pop)
_Static_assert(sizeof(superblock_t) == 116, "superblock must fit in one block");


#pragma pack(push,1)
typedef struct {
    uint16_t mode;
    uint16_t links;
    uint32_t uid;
    uint32_t gid;
    uint64_t size_bytes;
    uint64_t atime;
    uint64_t mtime;
    uint64_t ctime;
    uint32_t direct[DIRECT_MAX];
    uint32_t reserved_0;
    uint32_t reserved_1;
    uint32_t reserved_2;
    uint32_t proj_id;
    uint32_t uid16_gid16;
    uint64_t xattr_ptr;
    uint64_t inode_crc;
} inode_t;
#pragma pack(pop)
_Static_assert(sizeof(inode_t)==INODE_SIZE, "inode size mismatch");


#pragma pack(push,1)
typedef struct {
    uint32_t inode_no;
    uint8_t type;
    char name[58];
    uint8_t checksum;
} dirent64_t;
#pragma pack(pop)
_Static_assert(sizeof(dirent64_t)==64, "dirent size mismatch");


uint32_t CRC32_TAB[256];
void crc32_init(void){
    for (uint32_t i=0;i<256;i++){
        uint32_t c=i;
        for(int j=0;j<8;j++) c = (c&1)?(0xEDB88320u^(c>>1)):(c>>1);
        CRC32_TAB[i]=c;
    }
}
uint32_t crc32(const void* data, size_t n){
    const uint8_t* p=(const uint8_t*)data; uint32_t c=0xFFFFFFFFu;
    for(size_t i=0;i<n;i++) c = CRC32_TAB[(c^p[i])&0xFF] ^ (c>>8);
    return c ^ 0xFFFFFFFFu;
}


static uint32_t superblock_crc_finalize(superblock_t *sb) {
    sb->checksum = 0;
    uint32_t s = crc32((void *) sb, BS - 4);
    sb->checksum = s;
    return s;
}
void inode_crc_finalize(inode_t* ino){
    uint8_t tmp[INODE_SIZE]; memcpy(tmp, ino, INODE_SIZE);
    memset(&tmp[120], 0, 8);
    uint32_t c = crc32(tmp, 120);
    ino->inode_crc = (uint64_t)c;
}
void dirent_checksum_finalize(dirent64_t* de) {
    const uint8_t* p = (const uint8_t*)de;
    uint8_t x = 0;
    for (int i = 0; i < 63; i++) x ^= p[i];
    de->checksum = x;
}


void print_usage() {
    fprintf(stderr, "Usage: mkfs_builder --image <out.img> --size-kib <180..4096> --inodes <128..512>\n");
}


int main(int argc, char *argv[]) {
    crc32_init();


    char* image_path = NULL;
    uint64_t size_kib = 0;
    uint64_t inode_count = 0;


    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0) {
            if (++i < argc) image_path = argv[i];
            else { fprintf(stderr, "Error: --image requires a value.\n"); print_usage(); return EXIT_FAILURE; }
        } else if (strcmp(argv[i], "--size-kib") == 0) {
            if (++i < argc) size_kib = strtoull(argv[i], NULL, 10);
            else { fprintf(stderr, "Error: --size-kib requires a value.\n"); print_usage(); return EXIT_FAILURE; }
        } else if (strcmp(argv[i], "--inodes") == 0) {
            if (++i < argc) inode_count = strtoull(argv[i], NULL, 10);
            else { fprintf(stderr, "Error: --inodes requires a value.\n"); print_usage(); return EXIT_FAILURE; }
        } else {
            fprintf(stderr, "Error: Unknown argument '%s'.\n", argv[i]);
            print_usage(); return EXIT_FAILURE;
        }
    }


    if (!image_path || size_kib == 0 || inode_count == 0) {
        fprintf(stderr, "Error: All required arguments (--image, --size-kib, --inodes) must be provided.\n");
        print_usage(); return EXIT_FAILURE;
    }
    if (size_kib < 180 || size_kib > 4096 || size_kib % 4 != 0) {
        fprintf(stderr, "Error: --size-kib must be a multiple of 4 between 180 and 4096.\n");
        return EXIT_FAILURE;
    }
    if (inode_count < 128 || inode_count > 512) {
        fprintf(stderr, "Error: --inodes must be between 128 and 512.\n");
        return EXIT_FAILURE;
    }


    uint64_t total_blocks = (size_kib * 1024) / BS;
    uint64_t inode_table_blocks = (inode_count * INODE_SIZE + BS - 1) / BS;
    uint64_t data_region_start = 1 + 1 + 1 + inode_table_blocks;
    uint64_t data_region_blocks = total_blocks - data_region_start;


    if (data_region_blocks <= 0) {
        fprintf(stderr, "Error: Disk size is too small to fit file system metadata.\n");
        return EXIT_FAILURE;
    }


    uint8_t *disk_image = (uint8_t *)calloc(total_blocks, BS);
    if (!disk_image) {
        fprintf(stderr, "Error: Failed to allocate memory for disk image.\n");
        return EXIT_FAILURE;
    }


    time_t now = time(NULL);


    superblock_t* sb = (superblock_t*)disk_image;
    sb->magic = 0x4D565346u;
    sb->version = 1;
    sb->block_size = BS;
    sb->total_blocks = total_blocks;
    sb->inode_count = inode_count;
    sb->inode_bitmap_start = 1;
    sb->inode_bitmap_blocks = 1;
    sb->data_bitmap_start = 2;
    sb->data_bitmap_blocks = 1;
    sb->inode_table_start = 3;
    sb->inode_table_blocks = inode_table_blocks;
    sb->data_region_start = data_region_start;
    sb->data_region_blocks = data_region_blocks;
    sb->root_inode = ROOT_INO;
    sb->mtime_epoch = now;
    sb->flags = 0;


    uint8_t *inode_bitmap = disk_image + (sb->inode_bitmap_start * BS);
    uint8_t *data_bitmap = disk_image + (sb->data_bitmap_start * BS);


    inode_bitmap[0] |= (1u << (ROOT_INO - 1));
    data_bitmap[0] |= (1u << 0);


    inode_t* root_ino = (inode_t*)(disk_image + (sb->inode_table_start * BS) + (ROOT_INO - 1) * INODE_SIZE);
    memset(root_ino, 0, INODE_SIZE);
    root_ino->mode = 0040000u;
    root_ino->links = 2;
    root_ino->uid = 0;
    root_ino->gid = 0;
    root_ino->size_bytes = 2 * sizeof(dirent64_t);
    root_ino->atime = now;
    root_ino->mtime = now;
    root_ino->ctime = now;
    root_ino->proj_id = 6;
    root_ino->direct[0] = sb->data_region_start;


    inode_crc_finalize(root_ino);


    dirent64_t* root_dir_data = (dirent64_t*)(disk_image + root_ino->direct[0] * BS);


    dirent64_t* dot_entry = &root_dir_data[0];
    dot_entry->inode_no = ROOT_INO;
    dot_entry->type = 2;
    strncpy(dot_entry->name, ".", 58);
    dirent_checksum_finalize(dot_entry);


    dirent64_t* dotdot_entry = &root_dir_data[1];
    dotdot_entry->inode_no = ROOT_INO;
    dotdot_entry->type = 2;
    strncpy(dotdot_entry->name, "..", 58);
    dirent_checksum_finalize(dotdot_entry);


    superblock_crc_finalize(sb);


    FILE* fp = fopen(image_path, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Could not open output file '%s' for writing: %s\n", image_path, strerror(errno));
        free(disk_image); return EXIT_FAILURE;
    }
    if (fwrite(disk_image, BS, total_blocks, fp) != total_blocks) {
        fprintf(stderr, "Error: Failed to write the complete disk image to file.\n");
        fclose(fp); free(disk_image); return EXIT_FAILURE;
    }
    fclose(fp);
    free(disk_image);


    printf("Successfully created MiniVSFS image '%s' with %" PRIu64 " blocks and %" PRIu64 " inodes.\n", image_path, total_blocks, inode_count);
    return EXIT_SUCCESS;
}
