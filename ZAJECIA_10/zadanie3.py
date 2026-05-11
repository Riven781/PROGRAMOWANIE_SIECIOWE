import urllib.request
import sys
import json

def fetch_name_and_bands(id):
    url = f'https://api.discogs.com/artists/{id}'
    headers = {'User-Agent' : 'shared_bands_app (student - project)'}
    req = urllib.request.Request(url, headers=headers)

    try:
        with urllib.request.urlopen(req, timeout=3) as r:
            headers_dict = dict(r.getheaders())
            if 'application/json' not in headers_dict.get('Content-Type', ''):
                raise ValueError('Non-JSON data')
            raw_data = r.read().decode('utf-8')
            json_data = json.loads(raw_data)
    except Exception as e:
        print(e)
        sys.exit(1)
    bands = json_data.get('groups', []) #lista slownikow [{'id': 359282, 'name': 'Budka Suflera', 'resource_url': '..', 'active': True}, {'id': 4751291, 'name': 'Wieko', 'resource_url': '..', 'active': False}]
    band_names = [band.get('name') for band in bands]
    artist_name = json_data.get('name', None)
    if artist_name is None:
        sys.exit(1)
    return artist_name, band_names

def get_band_artists_dict(artist_bands_dict):
    band_artists_dict = {}
    for artist in artist_bands_dict:
        for band in artist_bands_dict[artist]:
            try:
                band_artists_dict[band].append(artist)
            except KeyError as e:
                band_artists_dict[band] = [artist]
    return band_artists_dict

def print_band_artists(band_artists_dict):
    for band in band_artists_dict:
        print(f'{band} : {band_artists_dict[band]}')

if __name__ == '__main__':
    if len(sys.argv) == 1:
        sys.exit(0)
    artists_id = sys.argv[1:]
    artist_bands = dict([fetch_name_and_bands(artist_id) for artist_id in artists_id])
    #print(artist_bands)
    band_artists = {band : artists for band, artists in get_band_artists_dict(artist_bands).items() if len(artists) > 1}
    band_artists_sorted = dict(sorted(band_artists.items()))
    print_band_artists(band_artists_sorted)
    sys.exit(0)
   
