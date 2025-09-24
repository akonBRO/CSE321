---> gcc -O2 -std=c17 -Wall -Wextra mkfs_builder.c -o mkfs_builder

---> gcc -O2 -std=c17 -Wall -Wextra mkfs_adder.c -o mkfs_adder

(Testing the given text file)

./mkfs_builder --image out.img --size-kib 512 --inodes 256
Output - Successfully created MiniVSFS image 'out.img' with 128 blocks and 256 inodes

./mkfs_adder --input out.img --output output_file2.img --file file_2.txt
Output: Successfully added file 'file_2.txt' to 'out.img'. Output saved as 'output_file2.img'.

./mkfs_adder --input output_file2.img --output output_file6.img --file file_6.txt
Output: Successfully added file 'file_6.txt' to 'output_file2.img'. Output saved as 'output_file6.img'.

./mkfs_adder --input output_file6.img --output output_file14.img --file file_14.txt
Output: Successfully added file 'file_14.txt' to 'output_file6.img'. Output saved as 'output_file14.img'.

./mkfs_adder --input output_file14.img --output output_file32.img --file file_32.txt
Output: Successfully added file 'file_14.txt' to 'output_file6.img'. Output saved as 'output_file14.img'.

./mkfs_adder --input output_file32.img --output output_file32.img --file file_32.txt
