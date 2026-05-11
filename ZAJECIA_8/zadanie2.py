import sys
import urllib.request


if __name__ == '__main__':
    url ='http://th.if.uj.edu.pl/'
    try:
        r = urllib.request.urlopen(url)

        if r.status != 200:
            sys.exit(1)

        headers_dict = dict(r.getheaders())

        content_type = headers_dict.get('Content-Type', '')
        if 'text/html' not in content_type:
            sys.exit(1)

        if r.read().decode().find('Institute of Theoretical Physics') == -1:
            sys.exit(1)

    except Exception:
        sys.exit(1)
    sys.exit(0)