#!/usr/bin/python3
# -*- coding: UTF-8 -*-

'''
Aplikacja WSGI implementująca najważniejsze części opisywanej na wykładzie
usługi REST dającej dostęp do bazy z danymi osób.

Uwaga: kod dydaktyczny bez pełnej obsługi błędów i sytuacji nadzwyczajnych.

Aplikacja nie potrafi sama stworzyć swojej bazy danych, trzeba to zrobić
przed jej uruchomieniem. Skrypt rest_webapp.sh pokazuje jak.
'''

plik_bazy = './osoby.sqlite'

import re, sqlite3, urllib.parse
import inspect

class HttpException(Exception):
    def __init__(self, status_code, detail = None):
        self.status_code = status_code
        self.detail = detail

class App:
    def __init__(self):
        self.status = '200 OK'
        self.headers = [ ('Content-Type', 'text/html; charset=UTF-8') ]
        self.content = b''
        self.routes = {}

    def _init_by_server(self, environment, start_response):
        self.env = environment
        self.start_response = start_response
        return self

    def start(self):
        from wsgiref.simple_server import make_server
        port = 8000
        httpd = make_server('', port, self._init_by_server)
        print('Listening on port %i, press ^C to stop.' % port)
        httpd.serve_forever()
    
    def __iter__(self):
        try:
            self.route()
        except sqlite3.Error as e:
            s = 'SQLite error: ' + str(e)
            self.failure('500 Internal Server Error', s)
        except HttpException as e:
            self.failure(e.status_code, e.detail)
        n = len(self.content)
        self.headers.append( ('Content-Length', str(n)) )
        self.start_response(self.status, self.headers)
        yield self.content

    def failure(self, status, detail = None):
        self.status = status
        s = '<html>\n<head>\n<title>' + status + '</title>\n</head>\n'
        s += '<body>\n<h1>' + status + '</h1>\n'
        if detail is not None:
            s += '<p>' + detail + '</p>\n'
        s += '</body>\n</html>\n'
        self.content = s.encode('UTF-8')

    def route(self):
        method = self.env['REQUEST_METHOD']
        qs_raw = self.env['QUERY_STRING']
        qs_params = urllib.parse.parse_qs(qs_raw)
        is_path = False

        paths_dict = self.routes[method]
        for path in paths_dict:  #wazna bedzie kolejnosc definiownia endpointow
            if m := re.search(path, self.env['PATH_INFO']):
                is_path = True
                args = m.groups()
                kwargs = {}
                func = paths_dict[path]
                #jesli sa quesry_stringi i jesli programista chce je dostc w endpoincie (bo zadeklrowal **kwargs) to je dosatnie
                if len(qs_params) > 0:
                    sig = inspect.signature(func)
                    for _, param in sig.parameters.items():
                        if param.kind == inspect.Parameter.VAR_KEYWORD:
                            kwargs = qs_params


                if method in ['POST', 'PUT', 'PATCH']: #w aplikcji przyjmiemy ze te metody wymagaja body 
                    f = self.env['wsgi.input']
                    n = int(self.env['CONTENT_LENGTH'])
                    raw_bytes = f.read(n)
                    body = raw_bytes.decode('UTF-8')
                    response = func(*args, body, **kwargs)
                else:
                    response = func(*args, **kwargs)
                
                if  isinstance(response ,Response):
                    self.content = response.content
                    self.headers = response.headers
                    self.status = response.status_code
                elif isinstance(response, bytes):
                    self.content = response
                elif isinstance(response, str):
                    self.content = response.encode('utf-8')
                elif response is None:
                    self.content = b''
                else:
                    raise HttpException('500 Internal Server Error' ,"Endpoint returned unsupported response type")
                return
        if is_path:
            raise HttpException("405 Method Not Allowed")
        else:
            raise HttpException("404 Not Found")
                
    def get(self, path_or_regex):
        def register_decortor(func):
            if self.routes.get('GET') is None:
                self.routes['GET'] = {}
            self.routes['GET'].update({path_or_regex : func})
            return func
        return register_decortor
    
    def post(self, path_or_regex):
        def register_decortor(func):
            if self.routes.get('POST') is None:
                self.routes['POST'] = {}
            self.routes['POST'].update({path_or_regex : func})
            return func
        return register_decortor
    
    def put(self, path_or_regex):
        def register_decortor(func):
            if self.routes.get('PUT') is None:
                self.routes['PUT'] = {}
            self.routes['PUT'].update({path_or_regex : func})
            return func
        return register_decortor
    

class Response:
    def __init__(self, content = '', headers={}, status_code="200 OK"):
        self.status_code = status_code
        self.content = content
        self.headers = headers


class DbRepository:
    def __init__(self, file):
        self.file = file

    def select(self, table_name, filters_dict = {}):
        conn = sqlite3.connect(self.file)
        crsr = conn.cursor()
        query = f'SELECT * FROM {table_name} '
        conditions = []
        values = []
        for filter_key, filter_value in filters_dict.items():
            if filter_value is None:
                break
            conditions.append(f'{filter_key} = ?')
            values.append(filter_value)
        if len(conditions) > 0:
            query += 'WHERE '
            query += ' and '.join(conditions) + ';'

        crsr.execute(query, values)
        colnames = [ d[0] for d in crsr.description ]
        rows = crsr.fetchall()
        crsr.close()
        conn.close()
        return colnames, rows
    
    def insert(self, table_name, colnames, vals):
        q = f'INSERT INTO {table_name} (' + ', '.join(colnames) + ') VALUES ('
        q += ', '.join(['?' for v in vals]) + ')'
        id = self.sql_modify(q, vals)
        colnames, rows = self.select(table_name, {"id" : id})
        return colnames, rows
    
    def update(self, table_name, id, colnames, vals):
        q = 'UPDATE osoby SET '
        q += ', '.join([c + ' = ?' for c in colnames]) 
        q += ' WHERE id = ' + str(id)
        self.sql_modify(q, vals)
        colnames, rows = self.select(table_name ,{"id" : id})
        return colnames, rows

    def sql_modify(self, query, params = None):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        if params is None:
            crsr.execute(query)
        else:
            crsr.execute(query, params)
        rowid = crsr.lastrowid   # id wiersza wstawionego przez INSERT
        crsr.close()
        conn.commit()
        conn.close()
        return rowid
    
        

def get_rows_formatted(colnames, rows):
    s = '\t'.join(colnames) + '\n'
    for row in rows:
        s += '\t'.join([str(val) for val in row]) + '\n'
    return s.encode('UTF-8')
  
def get_tsv(body):
    lines = body.splitlines()
    colnames = lines[0].split('\t')
    vals = lines[1].split('\t')
    return colnames, vals

app = App()
repository = DbRepository('./osoby.sqlite')

@app.get('^/osoby/(?P<id>[0-9]+)$')
def get_osoby_by_id(id, **kwargs): #kolejnosc argumentow jest wazna path args, body, kwargs z query string
    colnames, rows = repository.select('osoby', {"id" : id})
    if len(rows) == 0:
        raise HttpException("404 Not Found")
    rows_formatted = get_rows_formatted(colnames, rows)
    headers = [ ('Content-Type', 'text/tab-separated-values; charset=UTF-8') ]
    return Response(rows_formatted, headers)


@app.get('^/osoby$')
def get_osoby():
    colnames, rows = repository.select('osoby')
    rows_formatted = get_rows_formatted(colnames, rows)
    headers = [ ('Content-Type', 'text/tab-separated-values; charset=UTF-8') ]
    return Response(rows_formatted, headers)


@app.post('^/osoby$')
def post_osoby(body):
    colnames, vals = get_tsv(body)
    colnames_out, vals_out = repository.insert('osoby', colnames, vals)
    rows_formatted = get_rows_formatted(colnames_out, vals_out)
    headers = [ ('Content-Type', 'text/tab-separated-values; charset=UTF-8') ]
    return Response(rows_formatted, headers)

@app.put('^/osoby/(?P<id>[0-9]+)$')
def update_osoby(id, body):
    print('ppp')
    colnames, vals = get_tsv(body)
    colnames_out, vals_out = repository.update('osoby', id, colnames, vals)
    rows_formatted = get_rows_formatted(colnames_out, vals_out)
    headers = [ ('Content-Type', 'text/tab-separated-values; charset=UTF-8') ]
    return Response(rows_formatted, headers)

@app.get('^/osoby/search$')
def get_osoby_by_params(**params):
    imie = params.get('imie', [None])[0]
    nazwisko = params.get('nazwisko', [None])[0]
    filters_dict = {
        "imie" : imie,
        "nazwisko" : nazwisko
    }
    colnames, rows = repository.select('osoby', filters_dict)
    rows_formatted = get_rows_formatted(colnames, rows)
    headers = [ ('Content-Type', 'text/tab-separated-values; charset=UTF-8') ]
    return Response(rows_formatted, headers)

    

app.start()

class OsobyApp:
    def __init__(self, environment, start_response):
        '''
Konstruktor wywoływany przez serwer WSGI. Jak każdy konstruktor tworzy nowy
obiekt, następnie zapamiętuje w jego polach przekazane przez serwer argumenty
i inicjuje pola na odpowiedź.
'''
        self.env = environment
        self.start_response = start_response
        self.status = '200 OK'
        self.headers = [ ('Content-Type', 'text/html; charset=UTF-8') ]
        self.content = b''

    def __iter__(self):
        '''
Metoda obsługująca proces iterowania po stworzonym obiekcie. Serwer WSGI
wymaga aby w środku była co najmniej jedna instrukcja "yield" zwracająca
ciąg bajtów do odesłania klientowi HTTP.
'''
        try:
            self.route()
        except sqlite3.Error as e:
            s = 'SQLite error: ' + str(e)
            self.failure('500 Internal Server Error', s)
        n = len(self.content)
        self.headers.append( ('Content-Length', str(n)) )
        self.start_response(self.status, self.headers)
        yield self.content

    def failure(self, status, detail = None):
        '''
Metoda wstawiająca do pól obiektu status błędu oraz dokument HTML
z komunikatem o jego wystąpieniu.
'''
        self.status = status
        s = '<html>\n<head>\n<title>' + status + '</title>\n</head>\n'
        s += '<body>\n<h1>' + status + '</h1>\n'
        if detail is not None:
            s += '<p>' + detail + '</p>\n'
        s += '</body>\n</html>\n'
        self.content = s.encode('UTF-8')

    def route(self):
        '''
Pierwszą rzeczą, którą aplikacja musi zrobić po odebraniu zapytania, jest
sprawdzenie nazwy metody HTTP oraz nazwy zasobu. Jest to konieczne aby się
zorientować o co klient prosi i wywołać odpowiedni fragment kodu realizujący
jego zlecenie. Jest to tzw. routing zapytania.

W niniejszej aplikacji routing jest realizowany częściowo w tej metodzie,
a częściowo w metodach handle_table() i handle_item().
'''
        if self.env['PATH_INFO'] == '/osoby':
            self.handle_table()
            return
        if self.env['PATH_INFO'] == '/osoby/search':
            params = urllib.parse.parse_qs(self.env['QUERY_STRING'])
            print(self.env['QUERY_STRING'])

            imie = params.get('imie', [None])[0]
            nazwisko = params.get('nazwisko', [None])[0]

            print(imie)
            print(nazwisko)

            colnames, rows = self.sql_select(imie=imie, nazwisko=nazwisko)
            if len(rows) == 0:
                self.failure('404 Not Found')
            else:
                self.send_rows(colnames, rows)
            return
        
        m = re.search('^/osoby/(?P<id>[0-9]+)$', self.env['PATH_INFO'])
        if m is not None:
            self.handle_item(m.group('id'))
            return
        self.failure('404 Not Found')

    def handle_table(self):
        '''
Obsługa zapytań odnoszących się do tabeli "osoby" traktowanej jako całość.
Można ją pobrać, albo można dodać do niej nowy wiersz.
'''
        if self.env['REQUEST_METHOD'] == 'GET':
            colnames, rows = self.sql_select()
            self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'POST':
            colnames, vals = self.read_tsv()
            q = 'INSERT INTO osoby (' + ', '.join(colnames) + ') VALUES ('
            q += ', '.join(['?' for v in vals]) + ')'
            id = self.sql_modify(q, vals)
            colnames, rows = self.sql_select(id)
            self.send_rows(colnames, rows)
        else:
            self.failure('501 Not Implemented')

    def handle_item(self, id):
        '''
Obsługa zapytań odnoszących się do konkretnego wiersza w tabeli "osoby".
Można go pobrać, zmodyfikować, albo usunąć.
'''
        if self.env['REQUEST_METHOD'] == 'GET':
            colnames, rows = self.sql_select(id)
            if len(rows) == 0:
                self.failure('404 Not Found')
            else:
                self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'PUT':
            colnames, vals = self.read_tsv()
            q = 'UPDATE osoby SET '
            q += ', '.join([c + ' = ?' for c in colnames])
            q += ' WHERE id = ' + str(id)
            self.sql_modify(q, vals)
            colnames, rows = self.sql_select(id)
            self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'DELETE':
            q = 'DELETE FROM osoby WHERE id = ' + str(id)
            self.sql_modify(q)
        else:
            self.failure('501 Not Implemented')

    def read_tsv(self):
        f = self.env['wsgi.input']
        n = int(self.env['CONTENT_LENGTH'])
        raw_bytes = f.read(n)
        lines = raw_bytes.decode('UTF-8').splitlines()
        colnames = lines[0].split('\t')
        vals = lines[1].split('\t')
        return colnames, vals

    def send_rows(self, colnames, rows):
        s = '\t'.join(colnames) + '\n'
        for row in rows:
            s += '\t'.join([str(val) for val in row]) + '\n'
        self.content = s.encode('UTF-8')
        self.headers = [ ('Content-Type',
                'text/tab-separated-values; charset=UTF-8') ]

    def sql_select(self, id = None, imie=None, nazwisko=None):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        query = 'SELECT * FROM osoby'
        if id or imie or nazwisko:
            query += ' WHERE '
        conditions = []
        values = []
        if id:
            conditions.append('id = ?')
            values.append(id)
        if imie:
            conditions.append("imie = ?")
            values.append(imie)
        if nazwisko:
            conditions.append("nazwisko = ?")
            values.append(nazwisko)
        if len(conditions) >= 1:
            query += ' and '.join(conditions) + ';'
        

        crsr.execute(query, values)
        colnames = [ d[0] for d in crsr.description ]
        rows = crsr.fetchall()
        crsr.close()
        conn.close()
        return colnames, rows

    def sql_modify(self, query, params = None):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        if params is None:
            crsr.execute(query)
        else:
            crsr.execute(query, params)
        rowid = crsr.lastrowid   # id wiersza wstawionego przez INSERT
        crsr.close()
        conn.commit()
        conn.close()
        return rowid



