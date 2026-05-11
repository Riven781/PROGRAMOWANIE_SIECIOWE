Zadania:

    1. Serwery HTTP zazwyczaj podają swoją nazwę i czasem również wersję, np. Apache/2.2.10 (Unix), w polu Server w nagłówku odpowiedzi.

    Napisz program, który w argv dostaje nazwy witryn, takie jak google.com, www.uj.edu.pl itp. Do każdej z nich wysyła zapytanie GET / przez HTTP i HTTPS (na domyślne numery portów 80 i 443) i drukuje zwrócone wartości pól Server.

    Przykładowo, dla www.uj.edu.pl należy wypisać:

    port 80: nginx
    port 443: nginx

    2. Napisz program sprawdzający, czy pewna określona witryna działa poprawnie. Sprawdzenie ma polegać na pobraniu strony spod ustalonego adresu (np. spod http://th.if.uj.edu.pl/). Proszę nie zapomnieć o zweryfikowaniu, czy na pewno udało się ją poprawnie pobrać (status 200) i czy to jest strona HTML (typ text/html). Następnie należy sprawdzić, czy rzeczywiście jest to spodziewana strona, a nie np. komunikat o wewnętrznym błędzie serwera WWW — to można zweryfikować sprawdzając czy w pobranej treści znajduje się pewien zadany z góry ciąg znaków (np. „Institute of Theoretical Physics”).

    Program, w zależności od wyniku sprawdzenia, musi zwracać jako wynik funkcji main kod sukcesu (zero) bądź porażki (wartość większa od zera). Osoby piszące w Pythonie powinny użyć sys.exit(0) albo sys.exit(1), a w Javie należy wywołać metodę System.exit z odpowiednim argumentem.

    Programy tego typu używane są w systemach monitorowania usług sieciowych. Jeśli na filmie z centrum zarządzania siecią widać ekran z listą serwerów i usług, a przy nich zielone komunikaty „OK” i gdzieniegdzie czerwone komunikaty błędów, to za tymi kolorami kryją się uruchamiane w regularnych odstępach czasu programy sprawdzające status danej usługi.

    To zadanie jest ważne — zaimplementowany program trzeba oddać najpóźniej we wtorek 5 maja. Nie będzie możliwości oddawania poprawionych wersji w późniejszym terminie, ten program jest na to zbyt prosty. Z tego samego powodu przy wystawianiu zaliczeń będę to zadanie brał z wagą 25% w porównaniu do zadań z palindromami.

    3. (nieobowiązkowe) Sprawdź, czy w Twoim ulubionym języku programowania jest standardowa biblioteka pozwalająca serializować i deserializować dane w formacie JSON. Zapoznaj się z nią, spróbuj znaleźć przykład pokazujący zamianę ciągu bajtów / znaków na struktury danych tego języka programowania (czyli deserializację ciągu).
