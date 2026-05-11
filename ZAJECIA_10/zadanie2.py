import urllib.request
import sys
import json
import time

artist_id = 359282
base_url = f'https://api.discogs.com/artists/{artist_id}/releases'

def fetch_page(page):
    params = {
        "page" : page,
        "per_page" : 100

    }
    url = base_url + "?" + urllib.parse.urlencode(params)
    req = urllib.request.Request(url)

    with urllib.request.urlopen(req) as r:
        rest_response = r.read().decode("utf-8")
        rest_r_json = json.loads(rest_response)

    return rest_r_json

def fetch_all_release():
    page = 1
    releases = []

    while True:
        data = fetch_page(page)
        releases.extend(data["releases"])

        page += 1
        if page >= data["pagination"]["pages"]:
            break
        time.sleep(3)
    return releases


if __name__ == '__main__':
    
    releases = fetch_all_release()
    albums = []
    for r in releases:
        if r.get('type') == 'master' and r.get('role') == 'Main':
            albums.append(r.get('title'))
    print(albums)


