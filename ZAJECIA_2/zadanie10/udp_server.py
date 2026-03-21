import socket

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    s.bind(('0.0.0.0', 20123))

    while True:
        data, addr = s.recvfrom(1024)
        print(f"Odebrano {data} od {addr}")

        s.sendto(b"Hello, world!\r\n", addr)