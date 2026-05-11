import sys
import urllib.request
from bs4 import BeautifulSoup




if __name__ == '__main__':
    url ='https://pl.investing.com/currencies/usd-pln'
    try:
        req = urllib.request.Request(
            url,
            headers={'User-Agent': 'Mozilla/5.0'}
        )
        r = urllib.request.urlopen(req)

        if r.status != 200:
            sys.exit(1)

        headers_dict = dict(r.getheaders())

        #print(headers_dict)

        if 'text/html' not in headers_dict.get('Content-Type', ''):
            sys.exit(1)

        html_doc = r.read().decode()
        soup = BeautifulSoup(html_doc, 'html.parser')

        #print(soup.prettify())

        #print(soup.p['text-5xl/9'])
        matching_element = soup.find_all('div', class_='text-5xl/9')
   
        if len(matching_element) > 1 or len(matching_element) == 0:
            sys.exit(1)

        usd_price = matching_element[0].get_text(strip=True)

        usd_price_converted = float(usd_price.replace(',', '.'))

        print(usd_price_converted)

    except Exception as e:
        print(e)
        sys.exit(1)
    sys.exit(0)