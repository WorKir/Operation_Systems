# Work with threads (WinAPI)

## Topic
Organization of multitasking in the Win32 environment using processes and threads

## Goal
Familiarization with Win32 multitasking capabilities and working with processes and threads.

## Description
1. The main process is created
2. 2 child processes are created
3. The first child creates a named channel with the master and the second child. Also, the first child process creates a file for processing.

4. The second child process creates threads to work with the file

5. The processing of the file is implemented by the threads of the second child process, which are synchronized through Events

### The essence of the work of the first child process and the content of transmission over the channel
The first child process receives a message from the master process about the start of processing.
After a certain delay, it creates file №1 and passes processing parameters, including the number of threads, to the second process.
then receives intermediate data from the second child process about the processing time and transmits the results to the master after processing.
One of the threads reads file №1 block by block and forms a new file №2 from the data of the selected blocks and transfers control to the second thread. The second thread processes the read block and, after processing, transfers control to the first thread. After processing all the data of file No. 1, the result is transmitted through the channel to the first process.

### The essence of work and features of the second child process
####The essence of processing
Transcode an input text file containing block-by-block English, Russian and Ukrainian letters in Windows-1251 encoding into a file containing a textual representation of UNICODE character encoding. For example, the apostrophe symbol 'should be output to a new file as' because its UNICODE value is 39, and the Russian letter a will be represented &#39 as a.

####Processing features
Recode all characters, and then leave only headers in another file