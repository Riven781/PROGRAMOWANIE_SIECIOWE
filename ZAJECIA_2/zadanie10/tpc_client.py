import socket

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect(('127.0.0.1', 20123))
    data = s.recv(1024)
    print(data.decode())  #zamienić te bajty na czytelny string w Pythonie (aby ladnie sie wyswietlilo na wyjsciu) -- data.decode('utf-8')