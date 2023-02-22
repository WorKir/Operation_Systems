# Working with WIN32 processes (WinAPI)
## Description
The program creates a directory system of the type FILE11/ FILE12/ and FILE21/ FILE22/ FILE23/. Next, the program copies a group of files (both binary and text) in the specified directory from the existing directory, and then creates its own binary file with a set of integers in the selected directories.
Processes search for files where they find a given pattern until they are all finished. The main process outputs a list of sample files to a standard stream.
The child processes pass the names of the files where the sample is found to the parent.
Monopoly blocking of the last line of the first and second files is also established.

## Structure
Main_process.cpp -- is main process
Child_*.cpp -- is child process