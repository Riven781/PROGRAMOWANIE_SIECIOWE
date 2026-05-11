Zadania:

    1. Zapoznaj się z witryną Discogs oraz ze specyfikacją udostępnianego przez nią REST API.

    2. Spróbuj napisać program wyszukujący w bazie Discogs wszystkie albumy danego wykonawcy (np. Budki Suflera, czyli zespołu nr 359282). Sprawdź czy to, co drukuje program zgadza się z tym, co można ręcznie wyszukać na witrynie.

    Może się okazać, że Discogs odrzuca zapytania, jeśli są one całkowicie anonimowe. Sprawdź wtedy w dokumentacji kim / czym trzeba być, aby uzyskać odpowiedź — zalogowanym użytkownikiem witryny, aplikacją napisaną przez zarejestrowanego dewelopera?

    3. Napisz program, który w oparciu o informacje z Discogs sprawdza, czy muzycy z zadanego zbioru grali razem w jakichś zespołach. Wyświetl nazwy tych zespołów oraz imiona i nazwiska tych muzyków ze zbioru, którzy do nich należeli. Załóż, że numeryczne identyfikatory muzyków podawane są w argv.

    Dla przykładu: 516820 to Tomasz Zeliszewski, 532854 Marek Stefankiewicz, a 702387 Mieczysław Jurecki. Wszyscy trzej grali w Budce Suflera (id 359282), do tego Zeliszewski i Jurecki występowali w zespole Wieko (id 4751291), a Stefankiewicz i Jurecki — w Perfekcie (id 669348).

    Zadbaj, aby drukowane wyniki były posortowane po nazwie zespołu. Nie zapomnij o weryfikowaniu poprawności odpowiedzi zwracanych przez serwer. Dobrze by było, gdyby w razie przekroczenia limitu zapytań na minutę program chwilę czekał i ponawiał zapytanie, ale nie jest to obowiązkowe (program może taką sytuację traktować jak każdy inny błąd i kończyć działanie).

    To zadanie jest ważne — zaimplementowany program oddaj najpóźniej we wtorek 19 maja. Możesz użyć Pythona, Javy lub C/C++. Nie będzie możliwości oddawania poprawionych wersji w późniejszym terminie. Przy wystawianiu zaliczeń będę to zadanie brał z wagą 50% w porównaniu do zadań z palindromami.

    Uwaga: nie wolno Państwu używać bibliotek dedykowanych bazie Discogs, które są wymienione na początku strony https://www.discogs.com/developers (pod nagłówkiem „Quickstart”). To podkopałoby cel dydaktyczny, którym jest sprawdzenie, czy za pomocą zwykłej biblioteki klienckiej HTTP potraficie pobrać dane z serwera REST. Innymi słowy, zadanie sprawdza, czy potraficie przeczytać ze zrozumieniem dokumentację serwera REST i napisać kod konstruujący odpowiednie zapytania HTTP.

    4. (nieobowiązkowe) Rozszerz powyższy program tak, aby jako argument można było podawać nie tylko identyfikator, lecz również nazwisko (albo imię i nazwisko) muzyka. Najprawdopodobniej będzie wymagać to zarejestrowania się na witrynie Discogs w celu uzyskania tokena dla aplikacji, bo operacji wyszukiwania po nazwie zdaje się nie można wywoływać anonimowo.
