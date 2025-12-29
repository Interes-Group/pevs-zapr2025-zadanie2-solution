# PEVŠ ZAPR 2025 - Semestrálny projekt 2 - Riešenie

# Čitateľský denník

![C 17](https://img.shields.io/badge/language-C17-blue)
[![Static Badge](https://img.shields.io/badge/Web-ZARP.Interes.Group-aquamarine)](https://zapr.interes.group/assignments/assignments2)

Tento projekt je konzolová aplikácia napísaná v jazyku C (štandard C17), ktorá slúži na evidenciu prečítaných kníh a
sledovanie pokroku v čítaní. Program umožňuje používateľovi ukladať záznamy o knihách vrátane detailov ako názov, autor,
žáner, dátumy čítania, hodnotenie a osobné poznámky.

## Funkcionalita

Program podporuje dva hlavné príkazy:

1. **`new`**: Pridanie nového záznamu do denníka.
    * Povinné parametre: `--name`, `--author`, `--genre`, `--start`.
    * Nepovinné parametre: `--end`, `--score` (1-5), `--note`.
2. **`list`**: Zobrazenie uložených záznamov.
    * Možnosť filtrovania podľa žánru (`--genre`), stavu čítania (`--reading`, `--completed`) alebo minimálneho skóre (
      `--score`).

## Ako program spustiť

### Kompilácia

Pre skompilovanie programu použi priložený `Makefile` (ak je k dispozícii) alebo priamo kompilátor GCC:

```bash
gcc -std=c17 -o bin/journal src/main.c
```

### Príklady použitia

**Pridanie novej knihy:**

```shell script
./bin/journal new --name "Zaklínač" --author "Andrzej Sapkowski" --genre "fantasy" --start "2023-10-01" --score 5
```

**Zobrazenie všetkých rozčítaných kníh:**

```shell script
./bin/journal list --reading
```

---

## Ošetrenie vstupov a validácia

Program kladie dôraz na stabilitu a správnosť dát:

* **Validácia dátumov:** Implementovaná funkcia `is_valid_date` kontroluje, či je dátum v správnom ISO 8601 formáte (
  `YYYY-MM-DD`). Kontroluje nielen formát reťazca, ale aj logickú správnosť (počet dní v mesiaci, priestupné roky).
* **Práca so súborom:** Záznamy sa ukladajú do textového súboru `reading_journal.txt` s použitím oddeľovača `|`. Program
  korektne zvláda chýbajúce voliteľné polia.
* **Správa pamäte:** Dynamicky alokované polia pre poznámky a dátumy sú po použití uvoľnené pomocou `free()`, čím sa
  predchádza únikom pamäte (memory leaks).

---

## Zaujímavosti implementácie

### 1. Pointery na funkcie (Function Pointers)

Jednou z najzaujímavejších častí kódu je použitie pointerov na funkcie pri listovaní záznamov. Namiesto písania
viacerých takmer identických funkcií pre každý typ filtra, program používa jednu univerzálnu funkciu `list_entries`. Tá
prijíma pointer na filter:

```c++
void list_entries(bool (*filter)(JournalEntry *, char *), char *filter_argv)
```

Tento prístup umožňuje dynamicky meniť logiku filtrovania za behu programu, čo robí kód čistým, modulárnym a ľahko
rozšíriteľným.

### 2. Spracovanie argumentov z príkazového riadka

Program manuálne spracováva argumenty typu `key-value` (napr. `--name "Kniha"`), čo poskytuje lepšiu kontrolu nad
vstupom používateľa v porovnaní s jednoduchým pozičným načítaním.

### 3. Perzistencia dát

Dáta nie sú držané len v pamäti, ale sú serializované do textového formátu a opätovne parsované pri každom spustení
pomocou funkcie `strsep`, čo simuluje správanie jednoduchej databázy.
