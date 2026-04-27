import sys
import requests

if not len(sys.argv) > 1:
    raise ValueError("Nie podano argmentu")

domena = sys.argv[1]


def get_server(url, port, use_https=False):
    try:
        scheme = "https" if use_https else "http"
        full_url = f"{scheme}://{url}/"

        r = requests.get(full_url)

        print(r.status_code)

        server = r.headers.get("Server", "brak serwera")
        #print(r.headers)

        print(f"port {port}: {server}")
    except requests.exceptions.RequestException:
        print(f"port {port}: bład z połaczeniem")


#python3 zadanie1.py www.uj.edu.pl

get_server(domena, 80, use_https=False)
get_server(domena, 443, use_https=True)