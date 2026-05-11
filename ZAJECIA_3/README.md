
Zadania:
    1. Dokończ pisanie par klient-serwer dla TCP/IPv4 oraz UDP/IPv4 (co razem daje cztery programy). Przetestuj czy działają poprawnie gdy klient i serwer są uruchomione na dwóch różnych komputerach w SPK. Wymaga to znajomości adresu IP przydzielonego komputerowi, na którym uruchamiany jest serwer — można go znaleźć w wynikach polecenia ip address show.

    2. Sprawdź co się dzieje, gdy podasz zły adres IP albo zły numer portu serwera. Czy jądro systemu operacyjnego daje nam w jakiś sposób o tym znać? Jeśli tak, to jak długo trzeba czekać, aż jądro poinformuje nasz proces o wystąpieniu błędu?

    Pamiętaj, że protokoły sieciowe z korekcją błędów wykonują wielokrotne retransmisje pakietów w zwiększających się odstępach czasu. Może to zająć nawet kilkadziesiąt minut. Nie pomyl sytuacji „proces zawiesza się na pięć minut zanim jądro zwróci -1” z sytuacją „zawiesza się na stałe”.

    Jeśli któryś z klientów może się zawiesić czekając w nieskończoność na odpowiedź z nieistniejącego serwera, to popraw jego kod aby tego nie robił. W slajdach z wykładu są pokazane funkcje, które pozwalają na wykonywanie operacji we-wy z timeoutem (można go ustawić np. na 10 sekund).

    3. Przeanalizuj niniejszą specyfikację protokołu sprawdzania, czy wyrazy są palindromami. Czy jest ona jednoznaczna, czy też może zostawia pewne rzeczy niedopowiedziane?

    Komunikacja pomiędzy klientem a serwerem odbywa się przy pomocy datagramów. Klient wysyła datagram zawierający wyrazy do sprawdzenia. Serwer odpowiada datagramem zawierającym albo ułamek z liczbą otrzymanych wyrazów w mianowniku i liczbą wyrazów-palindromów w liczniku, albo komunikat o błędzie.

    Zawartość datagramów interpretujemy jako tekst w ASCII. Datagramy wysyłane przez klienta mogą zawierać litery i spacje. Datagramy wysyłane przez serwer mogą zawierać albo cyfry i znak /, albo pięć liter składających się na słowo „ERROR”. Żadne inne znaki nie są dozwolone (ale patrz następny akapit).

    Aby ułatwić ręczne testowanie serwera przy pomocy ncat, serwer może również akceptować datagramy mające na końcu dodatkowy znak \n (czyli bajt o wartości 10) albo dwa znaki \r\n (bajty 13, 10). Serwer może wtedy, ale nie musi, dodać \r\n do zwracanej odpowiedzi.

    4. Napisz serwer UDP/IPv4 nasłuchujący na porcie nr 2020 i implementujący powyższy protokół.

    Serwer musi weryfikować odebrane dane i zwracać komunikat o błędzie jeśli są one nieprawidłowe w sensie zgodności ze specyfikacją protokołu.

    To zadanie jest ważne — zaimplementowane rozwiązanie trzeba oddać najpóźniej we wtorek 24 marca, nawet jeśli nie jest jeszcze w pełni gotowe.

Podsumowanie analizy specyfikacji z zajęć nr 3

Proszę uwzględnić poniższe decyzje przy implementowaniu serwera.

Czy między wyrazami może być więcej niż jedna spacja?

    Nie. Nie pozwalamy na używanie ciągu dwóch lub więcej spacji jako separatora.

    Spacje przed pierwszym wyrazem bądź za ostatnim też są zabronione.
Czy utożsamiamy wielkie i małe litery?

    Tak. Jeśli klient jako jeden z wyrazów prześle „Ala”, to serwer powinien ten wyraz uznać za palindrom.
Czy jednoliterowe wyrazy są palindromami?

    Tak.
Jak serwer odpowiada na puste zapytania? Zwraca „0/0” czy „ERROR”?

    „0/0”. Zapytania zawierające pusty zbiór wyrazów uznajemy za poprawne.
Czy jest jakiś limit na długość zapytań-datagramów wysyłanych przez klienta?

    Na poziomie naszego protokołu aplikacyjnego klientowi nie stawiamy żadnych wymagań. Jedynym ograniczeniem rozmiaru wysyłanych przez niego zapytań jest więc limit narzucany przez używany protokół transportowy. W szczególności, UDP/IPv4 ma limit równy 65507 bajtom (ciut poniżej 64 KiB).

    Serwer musi być w stanie przetwarzać zapytania mające 1024 bajty lub mniej, na większe może odpowiadać „ERROR”. Zalecane jest, aby podczas implementowania serwera przyjąć wyższe ograniczenie, jeśli jest to możliwe. Najlepiej, gdy serwer może przetwarzać zapytania maksymalnego rozmiaru dopuszczanego przez używany protokół transportowy.

Zadania:



    1. Przetestuj ręcznie znajdujący palindromy serwer UDP. Jeśli akceptuje końcowe \r\n, to możesz to zrobić uruchamiając

    ncat --udp --crlf 127.0.0.1 2020

    albo

    socat stdio udp4:127.0.0.1:2020,crlf

    i wpisując kolejne zapytania z klawiatury.

    Zapytanie bez końcowego \r\n czy też \n można wygenerować poleceniem printf podłączonym do wejścia socata:

    printf "Ala i kot" | socat -t 5.0 stdio udp4:127.0.0.1:2020

    Przełącznik -t 5.0 nakazuje socatowi odczekać pięć sekund po zakończeniu wysyłania danych do serwera i zakończyć działanie. Jakiś timeout jest niezbędny, bo socat nie wie, że serwer zwróci dokładnie jeden datagram. W ogólnym przypadku odpowiedź z serwera UDP może się przecież składać z wielu datagramów, a na poziomie bezpołączeniowego protokołu transportowego, jakim jest UDP, nie ma po czym poznać że już je wszystkie odebrano.

    2. Często spotykanym w poprzednich latach błędem było zwracanie przez serwer dodatkowych bajtów o wartości zero, bo np. ktoś w kodzie zadeklarował sobie char wynik[20], użył sprintf aby „wydrukować” do tej tablicy tekstową reprezentację wyniku (która zajęła tylko kilka początkowych elementów tablicy), a potem przez pomyłkę wysłał klientowi całą 20-bajtową tablicę. Ten błąd łatwo przegapić, bo bajty o wartości zero są niewidoczne gdy się je wyświetla na ekranie.

    Aby sprawdzić jakie dokładnie bajty są w strumieniu danych trzeba ten strumień wysłać nie wprost na ekran, lecz np. na wejście programu od. Proszę porównać to, co wypisują dwa poniższe polecenia:

    printf "abc ijk\0xyz\n"

    printf "abc ijk\0xyz\n" | od -A d -t u1 -t c

    Użyte przełączniki nakazują wyświetlić kolejne bajty w postaci dziesiętnej oraz jako znaki ASCII (bajty odpowiadające niedrukowalnym znakom kontrolnym są wyświetlane jako sekwencje z backslashem na początku).

    Proszę spróbować zapisać zwrócone przez serwer dane do pliku, a potem ten plik wyświetlić za pomocą od:

    ncat --udp --crlf 127.0.0.1 2020 > wynik-z-serwera.txt

    printf "xyz" | socat -t 5.0 stdio udp4:127.0.0.1:2020 > wynik-z-serwera.txt

    od -A d -t u1 -t c < wynik-z-serwera.txt

    3. Możliwość przekierowania równocześnie wejścia i wyjścia socata można wykorzystać do stworzenia powtarzalnych testów. Załóżmy, że w pliku test-dane.txt jest ciąg bajtów składający się na zapytanie testowe. Uruchamiamy

    socat -t 5.0 stdio udp4:127.0.0.1:2020 < test-dane.txt > wynik-z-serwera.txt

    Jeśli przygotowaliśmy również plik test-wynik.txt, to przy pomocy poleceń cmp albo diff można łatwo porównać zawartość pliku wynik-z-serwera.txt ze wzorcowym wynikiem.

    socat nie radzi sobie z datagramami mającymi długość zero bajtów, nie da się więc przy jego pomocy wysłać pustego zapytania. Nie ma też jak odróżnić sytuacji, gdy serwer zwrócił pustą odpowiedź, od sytuacji gdy w ogóle żadna odpowiedź nie została zwrócona. Z tych powodów zamiast socata możesz chcieć użyć programu mini-udpcat.py, który został napisany specjalnie na potrzeby tych zajęć.

    4. Przygotuj kilka par plików z przykładowymi zapytaniami i oczekiwanymi wynikami. Uwzględnij także błędne zapytania, na które odpowiedzią powinno być „ERROR”.

    Wymień się tymi plikami z dwiema-trzema innymi osobami z grupy. Sprawdź, czy Twój serwer poprawnie obsługuje zapytania przygotowane przez inne osoby. Jeśli nie, to spróbujcie wspólnie ustalić przyczynę: różnice w rozumieniu specyfikacji protokołu, korzystanie w teście z opcjonalnej funkcjonalności, którą nie wszystkie serwery muszą implementować (u nas: \r\n na końcu datagramu), błędy w kodzie serwera, coś innego?

    5. (nieobowiązkowe, ale przydatne) Przygotuj sobie narzędzie automatycznie testujące sumator w oparciu o powyższe pliki z zapytaniami i odpowiedziami. Na przykład skrypt dla uniksowej powłoki, wywołujący polecenia używane w poprzednich punktach. Możesz też napisać program w C albo innym języku, wczytujący te pliki oraz komunikujący się przez gniazdko z serwerem. Zanim jednak się zabierzesz za jego pisanie, to lepiej sprawdź czy w sieci nie da się znaleźć gotowego narzędzia do testowania usług UDP — wielce możliwe, że ktoś już coś takiego zaimplementował.
