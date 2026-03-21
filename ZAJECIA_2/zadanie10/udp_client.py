import socket

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    s.sendto(b"", ('127.0.0.1', 20123))

    data, addr = s.recvfrom(1024)

    print(data.decode())