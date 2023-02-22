# Synchronization and interaction of processes
## Description
The program simulates the operation of a primitive DBMS that stores a single table in RAM. While performing some work cycles, K spawned processes, using a message queue, transmit to the parent process the row number and the content with which the data stored in it should be replaced. The parent process performs the specified operation and returns the previous content of the row that was changed.

### Sample of work

```bash
kir@ubuntu:~/lab_9$ ./lab9
 Enter:2 Kirill
Egor
```