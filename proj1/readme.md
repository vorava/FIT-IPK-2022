# IPK projekt 1

- Autor: Vojtěch Orava (xorava02)

Tento projekt reprezentuje HTTP server, který zpracovává 3 typy požadavků. Projekt je napsán v jazyce C. Server zpracovává příkazy **load**, **cpu-name** a **hostname** (viz Příklady použití).

## Prerekvizity

Server ke správné funkčnosti potřebuje kromě běžných C knihoven pouze knihovnu **socket**.

	#include <sys/socket.h>


## Kompilace a spuštění

Kompilace se spouští v kořenové adresáři projektu příkazem

	make

Samotné spuštění se provádí příkazem

	./hvinfosvc <PORT>

kde port udává číslo portu pro připojení k serveru.
Server po spuštění vypíše informace o serveru (jméno, jméno CPU, aktuální využití CPU) a poté vypisuje jednotlivé přijaté požadavky od klientů.

	./hinfosvc 12345
    -------Server Info-------
    Port: 12345
    Host name: DESKTOP-2A2332
    CPU name: Intel(R) Core(TM) i5-2520M CPU @ 2.50GHz
    CPU load: 50%
    1) Client requested CPU name
    2) Client requested CPU load
    3) Client requested host name

## Příklady použití

Zde jsou popsány jednotlivé příkazy zpracovávané serverem.
 - **localhost** značí jméno serveru
 - **12345** označuje port přiřazený při spuštění

#### Load

Požadavek:

	curl http://localhost:12345/load
Příkladová odpověď serveru:

	60%
#### CPU name

Požadavek:

	curl http://localhost:12345/cpu-name
Příkladová odpověď serveru:

	Intel(R) Core(TM) i5-2520M CPU @ 2.50GHz

#### Hostname

Požadavek:

	curl http://localhost:12345/hostname

Příkladová odpověď serveru:

    DESKTOP-2AL332

## Ukončení serveru

Server lze ukončit (vypnout) pomocí CTRL+C

## Návratové hodnoty

- -1 jiná chyba
- -2 chyba v získávaní informací (load/cpuname/hostname)
- -3 chyba nastavení socketu
- -4 špatně zadané číslo portu (např. mimo rozsah <1024; 65535>)
- -5 chybný počet argumentů
