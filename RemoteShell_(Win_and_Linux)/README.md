# Remote Shell
Universal for Win and Linux.

## Description
The program was developed for educational purposes and tested only on personal devices.
There are two files in the repository -- client.py and server.py. Accordingly, the client must be launched on the computer over which it is necessary to perform control, and the seerver on the computer from which it will take place.
For the program to work, you need to specify the address and port of the server in the client.py file, and specify the address (usually localhost) and the listening port in the server.py file.
```python
host = <Listening (or server) IP>
port = <Listening (or server) port>
```
Next, you run client.py on the client's computer and server.py on the server's computer, and a connection occurs, which creates remote control.
