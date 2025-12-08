# PEVŠ ZAPR 2025 - Semestrálny projekt 2

# Čitateľský denník

![C 17](https://img.shields.io/badge/language-C17-blue)
![Deadline 17.12.2025 23:59](https://img.shields.io/badge/deadline-17.12.2025%2023%3A59-red)
[![Github Classrom](https://img.shields.io/badge/GitHub-Classroom-green)](https://classroom.github.com/classrooms)
[![Static Badge](https://img.shields.io/badge/Web-ZARP.Interes.Group-aquamarine)](https://zapr.interes.group/assignments/assignments2)

Cieľom zadania je implementovať jednoduchú konzolovú aplikáciu (tzv. CLI - command line interface),
ktorá by slúžila používateľovi na zaznamenanie čitateľského denníka.

Program poskytne používateľovi možnosti pre vytvorenie a vypísanie kníh, ktoré používateľ prečítal,
či má práve rozčítané, na základe rôznych kritérií. Funkcionalita programu prebieha v jednom spustení,
kde pomocou vstupných argumentov používateľ zadáva akú akciu chce v rámci programu vykonať.

Program má pracovať so štruktúrami a dynamickými poliami pre pracovanie s dátami čitateľského denníka.
Čitateľských denník je ukladaný pri každom ukončení programu do súboru, z ktorého je vždy pri spustení
načítaný pre vykonanie požadovanej akcie (zápis nového záznamu či vypísanie existujuceho).

Program po skompilovaní je spustení z konzoly/terminálu s tzv. pomenovaným argumentami (options), napríklad:

```shell
./journal --help
```

## Čitateľský denník

Program má ponúkať funkcionalitu pre vedenie tzv. čitateľského denníka, t.j. má umožniť pridať záznam o práve čítanej
knihe, resp. záznam o prečítanej knihe a následne tieto záznamy prehľadávať.

Záznam v rámci denníka musí obsahovať minimálne dáta v rozsahu:

- názov knihy
- autor knihy
- žáner knihy
- dátum začatie čítania knihy
- dátum ukončenia čítania knihy
- subjektívne hodnotenie
- poznámka ku knihe

So záznamami má program pracovať pomocou zadania vstupných argumentov programu. Program má ukladať záznamy do súboru,
tak aby boli k dispozícii pri ďalšom spustení programu.

### Záznam knihy

Program pracuje so záznamom knihy a ponúka používateľovi buď pridať nový záznam, alebo vypísať existujúce záznamy
podľa zadaných kritérií.

O ktorú funkcionalitu programu ide je vyjadrené prvým argumentov, tzv. príkazom, programu.

- `new` - vytvorenie nového záznamu
- `list` - vypísanie záznamov kníh

Za príkazom programu nasledujú argumenty, ktoré sú určené pre daný príkaz. Spustenie programu by teda mohlo vyzerať
nasledovne:

```shell
./journal list --genre fantasy
```

### Vytvorenie záznamu o knihe

Program umožňuje používateľovi vytvoriť nový záznam o knihe pomocou príkazu `new` s nasledujúcimi argumentami:

| Argument                         | Dátový typ        | Povinný | Popis                                                                  |
|----------------------------------|-------------------|---------|------------------------------------------------------------------------|
| `--name NAZOV`                   | char*             | Áno     | Názov knihy.                                                           |
| `--author MENO_AUTORA`           | char*             | Áno     | Meno autora knihy.                                                     |
| `--genre ZANER`                  | char*             | Áno     | Žáner knihy.                                                           |
| `--start DATUM_ZACIATKU_CITANIE` | char* (ISO dátum) | Áno     | Dátum vo formáte ISO 8601.                                             |
| `--end DATUM_UKONCENIA_CITANIA`  | char* (ISO dátum) | Nie     | Dátum vo formáte ISO 8601.                                             |
| `--score HODNOTENIE`             | int               | Nie     | Hodnotenie knihy od 1 do 5 kde 1 je zlá kniha a 5 je vynikajúca kniha. |
| `--note POZNAMKA`                | char*             | Nie     | Dodatočná poznámka ku knihe.                                           |

Program môže by teda zavolaný napríklad:

- `./journal new --name "Hobbit" --author "J.R.R. Tolkien" --genre fantasy --start "2022-01-01" --score 4`
- `./journal new --name "The Great Gatsby" --author "F. Scott Fitzgerald" --genre fiction --start "2024-05-01"`
- `./journal new --name "Moby-Dick" --author "Herman Melville" --genre drama --start "2020-06-12" --end "2020-07-28" --score 3`
- `./journal new --name "Pride and Prejudice" --author "Jane Austen" --genre romance --start "2021-04-14" --end "2021-09-10" --score 2 --note "Lepšie než film"`

Po spustení programu, program musí komunikovať používateľovi status uloženia záznamu knihy, resp. chybu (napr. v prípade
chyby vstupu). Ak je uložené úspešné program vypíše údaje novo uloženého záznamu knihy.

### Vypísanie záznamov kníh

Program umožňuje používateľovi vypísať aktuálne uložené záznamy o knihách pomocou príkazu `list` s nasledujúcimi
nepovinnými argumentami:

| Argument             | Dátový typ | Popis                                                                                                                                                                                                    |
|----------------------|------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `--genre ZANER`      | char*      | Vypísanie kníh podľa žánru.                                                                                                                                                                              |
| `--reading`          | -          | Vypísanie kníh, ktoré majú definovaný dátum začatia čítania, ale nemajú definovaný dátum ukončenia. Argument nemá žiadnu ďalšiu hodnotu a je ho možné interpretovať ako tzv. 'boolean' prepínač / flag.  |
| `--completed`        | -          | Vypísanie kníh, ktoré majú definovaný dátum začatia čítania a zároveň má definovaný dátum ukončenia. Argument nemá žiadnu ďalšiu hodnotu a je ho možné interpretovať ako tzv. 'boolean' prepínač / flag. |
| `--score HODNOTENIE` | int        | Vypísanie kníh, ktoré majú definované skóre hodnotenia rovné alebo vyššie ako zadané argumentom.                                                                                                         |

Pre príkaz `list` je možné definovať maximálne jeden argument. Argumenty príkazu tak nie je možné kombinovať.
Ak nie je zadaný žiaden ďalší argument po príkaze `list` tak program vypíše informácie o všetkých, ktoré má uložené.

Program môže byť teda zavolaný napríklad:

- `./journal list --genre fantasy`
- `./journal list --reading`
- `./journal list --completed`
- `./journal list --score 4`

Program pre každý záznam knihy vypíše všetky údaje o ňom v čitateľnej podobe, kde jednotlivé záznamy sú viditeľne
oddelené od seba.

## Implementácia

V rámci implementácie môžte použiť všetky štandardné funkcie knižnice jazyka C, v štandarde C17. Kód musí byť
skompilovateľný základnou inštaláciou programu **GCC**, takže pozor na Windows/Mac špecifické kompilátory.

### Štruktúra záznamu

Záznam knihy má byť v rámci programu definovaný pomocou štruktúry jazyka C `typedef struct { ... }`.
Atribúty štruktúry, ktoré sú textové reťazce (meno, autor, žáner, dátumy, poznámka) môžu byť definované ako staticky
alokované polia `char[100]`.

Dátumy v rámci štruktúry sú definované ako reťazce znakov vo
formáte [štandardu ISO 8601](https://en.wikipedia.org/wiki/ISO_8601), t.j. `YYYY-MM-DD` (napr. `2022-01-01`).
Dátum nemusíte nijako ďalej spracovávať, pre vypísanie podľa definovanie kritérií stačí len kontrolovať prítomnosť
hodnoty v poli.

### Súbor záznamov programu

Program má pracovať s textovým súborom, do ktorého ukladá záznamy kníh (prí príkaze `new`) a zároveň ich z neho načíta
vždy pri spustení programu.

Súbor je ukladaný do formátu, kde jednotlivé atribúty záznamu knihy sú oddelené znakom `|` a každý záznam je na jeden
riadok.

```text
NAZOV|AUTOR|ZANER|START_DATUM|END_DATUM|SCORE|POZNAMKA
```

Ak nejaký atribút v rámci záznamu neexistuje je uložený iba prázdny reťazec `""`.

Súbor je uložený v rovnakom priečinku ako je umiestnený spustiteľný program (t.j. skompilovaný spustiteľný súbor
programu), t.j. relatívna cesta `./`, pod názvom `reading_journal.txt`.
**Súbor aj jeho umiestnenie je dôležité dodržať**.

**Príklad súboru:**

```text
Hobbit|J.R.R. Tolkien|fantasy|2022-01-01||4|
The Great Gatsby|F. Scott Fitzgerald|fiction|2024-05-01|||
Moby-Dick|Herman Melville|drama|2020-06-12|2020-07-28|3|
Pride and Prejudice|Jane Austen|romance|2021-04-14|2021-09-10|2|Lepšie než film
```

### Rozdelenie programu do funkcií

Funkcionalitu programu rozdelte do niekoľkých funkcií, ktoré následne použijete v programe. Program implementujte v
jednom súbore _main.c_, ktorý musí byť umiestnený v priečinku _src_ v tomto repozitáry. Ak je
zdrojový súbor umiestnený na inom mieste, alebo bude nazvaný iným menom, nebude braný pri kompilácii do úvahy, a teda
ani pri hodnotení.

Pri spracovaní vstupov sa môžte inšpirovať
článkom [Práca s argumentami programu v jazyku C](https://zapr.interes.group/guides/program_arguments/#spracovanie-prep%c3%adna%c4%8dov-flags)
na stránke predmetu, alebo inými technikami uvedenými na internete. Pre pokročilejších, pre spracovanie argumentov môžte
použiť aj funkciu `getopt_long` z knižnice `getopt.h`. Dokumentáciu nájdete
napríklad [tu](https://sourceware.org/glibc/manual/2.42/html_node/Getopt-Long-Options.html)
alebo [tu](https://linux.die.net/man/3/getopt_long).

Pri implementácii si môžte vypomáhať s AI nástrojmi, pre inšpiráciu, riešenie problémov, či testovanie avšak **je prísne
zakázané priame generovanie kódu vypracovania zadania**, či kopírovanie väčších častí kódu z AI či internetu. Táto práca
je ukážkou vašich schopností a vedomostí programovania nie definovania promptu do AI.

### Kompilácia

Pre testovanie je možné kód skompilovať príkazom:

```bash
gcc -std=c17 -o bin/journal -Wall -Wextra src/main.c
```

Kompilátor vytvorí spustený program v priečinku _bin_ v repozitáry.

### Git

Zdrojový kód tohto projektu je manažovaný verziovacím systémom Git. Pomocou Gitu je toto vypracovanie zadania aj
odovzdané. Pre oboznámenie práce s Gitom si prečítajte tento
článok [Git pre začiatočníkov](https://zapr.interes.group/guides/git-for-beginners/) alebo akýkoľvek iný tutoriál na
internete.

Pre kontrolu kompilácie je v repozitáry nastavená automatizácia cez GitHub Actions pipeline. Pipeline sa
spustí automaticky pri aktualizácii kódu vo vetve `main` (napr. `git push`). GitHub zdrojový súbor `src/main.c`
skompiluje pomocou poslednej verzie kompilátora **GCC** pre štandard jazyka C17. Pipeline následne pokračuje jednoduchým
testom spustenia programu. Spustenie je testované a zaznamenané pomocou
programu [tui-test](https://github.com/microsoft/tui-test) a na konci pipeline je vypísaný výsledok takého testu.
Pipeline vždy testuje posledný commit vo vetve `main` a testuje program s argumentom `--help`. Pre správnu kontrolu
programu musí nápoveda/pomocný text programu obsah slovo "Help".
Pipeline je možné spustiť ľubovoľný počet krát. Spustenie pipeline je možné vidieť v záložke _Actions_ vo vašom
repozitáry zadania.

## Hodnotenie

Zadanie je **ohodnotené 20 bodmi**. Odovzdaný program musí byť skompilovateľný kompilátorom GCC a spustiteľný, inak je
hodnotený 0 bodmi. Pri hodnotení vypracovania zadania sa kontroluje originalita odovzdaného vypracovania medzi všetkými
študentmi a zároveň aj voči AI nástrojom (kód vygenerovaný pomocou ChatGPT, Gemini a Claude Sonnet). Vypracovanie **so
zhodou vyššou ako 70% sú hodnotené 0 bodmi**. Hodnotený je iba kód ku poslednému commitu vo vetve `main`, ktorý bol
urobený do termínu odovzdania.

Vypracovanie bude **podrobené automatizovaným testom**, ktoré budú prevolávať program s definovanými argumentami a
kontrolovať obsah výstupu programu. Je teda veľmi dôležité dodržať špecifikáciu argumentov, ako aj formát výstupného
súboru programu. Výsledky automatizovaných testov budú zverejnené do tohto repozitára do vetvy `feedback`.
V prípade chyby v testoch, budú testy spustené znovu a nový výsledok zverejnený. Po uplynutí termínu odovzdania
zadania budú definície testov zverejnené a ak v nich nájdete chybu prosím nahláste
ju [sem ako nové GitHub issue](https://github.com/Interes-Group/pevs-zapr2025-zadanie1).

Vypracovanie bude hodnotené aj ručne. Pri hodnotení vypracovania sa bude prihliadať na:

- práca s premennými
- definovanie control flow programu
- definovanie vlastných funkcií a ich použitie
- definovanie štruktúry a jej použitie
- definovanie a alokácia dynamického pola
- práca so súborom
- spracovanie argumentov
- komunikácia chybového stavu programu používateľovi
- štrukturovanie kódu
- dodržanie špecifikácie zadania

## Odovzdanie

Po prihlásení sa na zadanie 2 cez [GitHub Classroom]() vám je automaticky vytvorená kópia repozitára, ktorý bude
nastavený ako privátny pre vás na vypracovanie (t.j. máte povolený commit a push do repozitára). Skontrolujte si, či sa
váš repozitár nachádza pod skupinou _Interes-Group_, inak vyučujúci nemajú prístup ku vášmu repozitáru a zadanie sa
nebude považovať za odovzdané. Ak sa vám repozitár nevytvorí ihneď kontaktuje vyučujúceho na MS Teams alebo na
univerzitnom
emaily. Je prísne zakázané dávať prístup k vášmu repozitára inému študentovi, alebo osobe, ktorá nie je vyučujúci.

V repozitáry by sa mali vytvoriť dve vetvy `main` a `feedback` a vytvorí sa tzv. pull request z vetvy `main` do vetvy
`feedback` pod menom _Feedback_, ktorý slúži na komunikáciu ohodnotenia vyučujúcim. **Váš kód pushujte (t.j. `git push`)
do vetvy `main`**. Hodnotenie vypracovania, ako aj komentár ku kódu,
uvidíte v danom pull requeste. Ak sa vám vetva `feedback` alebo pull request nevytvorí ihneď kontaktuje vyučujúceho na
MS Teams alebo na univerzitnom emaily.

V repozitáry upravujte iba súbory pod priečinkom **src**. Ostatné súbory je zakázané upravovať, predovšetkým súbory pre
kompiláciu, súbory obsahujúce GitHub pipeline (.github) a súbory obsahujúce automatizované testy (priečinok **test**).
Pri zmene týchto súborov je vypracovanie ohodnotené 0 bodmi.

Vypracovanie zadania priebežne commitujte (`git commit`)/pushujte (`git push`) do repozitára vytvoreného GitHubom
pre toto zadanie. Váš kód commitujte/pushujte do vetvy `main`. Hodnotený bude posledný commit/push do termínu odovzdania
vypracovania. Vypracovanie je nutné odovzdať/commitnúť/pushnúť do repozitára **do 17.12.2025 23:59**. Neodovzdanie je
hodnotené 0 bodmi.

V prípade otázok, alebo technických problémov môžete kontaktovať vyučujúcich na MS Teams alebo na univerzitnom emaily.