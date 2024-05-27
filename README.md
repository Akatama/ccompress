# ccompress 
An implementation of Build Your Own Compression Tool from Coding Challenges

## Dependencies
This project only has three depenencies: make, gcc and uthash. uthash is a library which contains C macros for hash tables and more. This project in particular needs uthash.h, the hash table library.

Install on Ubuntu
`sudo apt install make gcc uthash-dev`

Install on openSUSE Tumbleweed
`sudo zypper install make gcc uthash-devel`

If uthash is not in your distro's package manager, then you will want to either to clone the git repo, or download a .zip file from there. 
Once that is done, you will want to move uthash.h into the /usr/include directory. You can find uthash's github repo [here](https://github.com/troydhanson/uthash).

## Building
Assuming you have all of the required depencies, all you should need to do is enter `cd ccompress` then run `make`. If make does not work, make sure you are in the same directory as the Makefile.

## Usage
Once ccompress is built, you can always call `./ccompress -h` to see help. Here it is on this page just for completeness.

Usage: ccompress [INPUT_FILE] [OPTION] ... [OUTPUT_FILE]

Compress or Decompress INPUT_FILE into OUTPUT_FILE

-c  compresses INPUT_FILE and stores the result in OUTPUT_FILE

-d  decompresses INPUT_FILE and stores the result in OUTPUT_FILE

-q  "quiet mode" supresses the output of info used during the compression or decompression process

Example call to compress a file: `./ccompress file1.txt -c file1.bin`

Example call to decompress a file: `./ccompress file1.bin -d file1_decompress.txt`
