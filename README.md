# Dir Command
# Author: Wenda Zhao

#find

find is a C file for the File and dir in the <dir>.

## Installation

Downlod Project1_Wenda.tar

tar -cvf Project1_Wenda.tar
make

## Usage

./find

If you simply imput ./find, the program will use current path to output.

./find <dir>

<dir> could be absolute path and relative path.

## Contributing
Textbook p.132 Figure 4.22

## Author's thoughts
The author spend bunch of time to read all content of Ch.4 and understand each line of the 
code from the textbook. However, limited by time, some function could be more developed 
in the future:
	First, for the symbolic link, this program could detect the symbolic link file. Next,
the readlink() function could read it.
	Second, for the format of output, we could defined a flag to show the times dir 
was opened. Everytime the opendir() opened a dir, the flag++, and everytime use the 
chdir("..") back to parent dir, the flag--. When printing the filename, use flag*"\t" for the start of each line, 
to make it like a formating output.
	Third, for the -s <file size in bytes>, do the if (argc == 3) in main(). When doing 
printing file's names, use the st_size from lstat() to pick the filesize larger than the number 
input.
	Fourth, for the -f <string pattern>, do the if(argc == 3) in main() also. When doing 
printing file's names, use the dirp->d_name when use opendir(), to get the name of file. Then
do the strings selection to print the name containing the string operator input.
