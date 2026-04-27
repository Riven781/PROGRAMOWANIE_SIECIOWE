import urllib.request
import sys

#f = urllib.request.urlopen('http://www.python.org/')

#print(f.read(100))  #czyta 100 bajtow
#print(f.read(100).decode('utf-8'))
"""
req = urllib.request.Request('http://www.example.com/')
req.add_header('Referer', 'http://www.python.org/')
"""
#print(sys.argv)

if not len(sys.argv) > 1:
    raise ValueError("Nie podano argumentu")

def get_server(host, port, use_https):
    schema = 'https' if use_https else 'http'
    try:
        full_url = f"{schema}://{host}:{port}/"

        r = urllib.request.urlopen(full_url)
        #r.getheaders()  #zwraca liste krotek
        #print(r.read())
        server = dict(r.getheaders()).get('Server', '')

        print(f"port {port}: {server}")
    except:
        print(f'port: {port} ERROR...')
    

get_server(sys.argv[1], 80, use_https=False)
get_server(sys.argv[1], 443, use_https=True)