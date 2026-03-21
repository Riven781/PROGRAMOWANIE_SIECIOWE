import socket

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind(('0.0.0.0', 20123))
    s.listen(5)

    while True:
        conn, addr = s.accept()
        with conn:
            print(f"Polaczono z {addr}")
            conn.sendall(b"Hello, world!\r\n")


