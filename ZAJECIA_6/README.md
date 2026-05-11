Zadania:

    1. Przeczytaj poniższą specyfikację strumieniowego protokołu zliczania palindromów. Porównaj ją ze specyfikacją, którą samodzielnie napisałeś(-aś) w poprzednim tygodniu. Przeanalizuj, które części obu specyfikacji są ze sobą zgodne, a które są sprzeczne. Całkiem możliwe, że części sprzecznych nie będzie — przedstawione tydzień temu wytyczne prawdopodobnie doprowadziły większość z Was do specyfikacji takiej jak ta tutaj.

    Komunikacja pomiędzy klientem a serwerem odbywa się przy pomocy połączenia strumieniowego. Klient wysyła jedną lub więcej linii zawierających wyrazy. Dla każdej odebranej linii serwer zwraca linię zawierającą albo obliczony wynik, albo komunikat o błędzie.

    Ogólna definicja linii jest zapożyczona z innych protokołów tekstowych: ciąg drukowalnych znaków ASCII (być może pusty) zakończony dwuznakiem \r\n pełniącym rolę terminatora linii.

    Linia z zapytaniem klienta może zawierać tylko litery oraz spacje pełniące rolę separatorów słów. Obowiązują te same wymagania i interpretacje co w uprzednio rozważanym protokole datagramowym (puste zapytanie z zero słów jest uznawane za poprawne, utożsamiamy wielkie i małe litery, itd.).

    Linia z odpowiedzią serwera może zawierać albo dwa niepuste ciągi cyfr rozdzielone znakiem /, albo pięć liter składających się na słowo „ERROR”.

    (Uwaga na marginesie: wszystkie linie, i te wysyłane przez klientów, i przez serwer, mają oczywiście do opisanej powyżej zawartości dołączony terminator linii, czyli \r\n.)

    Serwer może, ale nie musi, zamykać połączenie w reakcji na nienaturalne zachowanie klienta. Obejmuje to wysyłanie danych binarnych zamiast znaków ASCII, wysyłanie linii o długości przekraczającej przyjęty w kodzie źródłowym serwera limit, długi okres nieaktywności klienta itd. Jeśli serwer narzuca maksymalną długość linii, to limit ten powinien wynosić co najmniej 1024 bajty (1022 drukowalne znaki i dwubajtowy terminator linii).

    Serwer nie powinien zamykać połączenia gdy udało mu się odebrać poprawną linię w sensie ogólnej definicji, ale dane w niej zawarte są niepoprawne (np. oprócz liter i spacji są przecinki). Powinien wtedy zwracać komunikat błędu i przechodzić do przetwarzania następnej linii przesłanej przez klienta.

    2. Napisz serwer TCP/IPv4 nasłuchujący na porcie nr 2020 i implementujący powyższy protokół. Serwer musi być w stanie równocześnie obsługiwać co najmniej 100 połączeń. Użytym językiem może być C albo C++. Proszę pisać kod tak, aby się kompilował bez ostrzeżeń odpowiednio pod gcc -std=c99 -pedantic -Wall bądź g++ -std=c++17 -pedantic -Wall.