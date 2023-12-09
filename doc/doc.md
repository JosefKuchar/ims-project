---
bibliography: doc/doc.bib
biblio-style: czechiso
---

# Úvod

V této práci je řešena implementace celulárního automatu [@ims č.s. 216], která bude použita pro sestavení modelu [@ims č.s. 7] jednoproudé okružní křižovatky (též kruhového objezdu).
Na základě modelu a simulačních experimentů [@ims č.s. 9] bude ukázáno chování systému [@ims č.s. 18] v podmínkách běžného dopravního provozu.
Smyslem experimentů je demonstrovat, že změnou parametrů okružní křižovatky (například změnou vnějšího rádiusu), lze zvýšit kapacita (propustnost) těchto křižovatek.

## Kdo se na práci podílel

### Autoři

- Josef Kuchař (xkucha28)
- Matej Sirovatka (xsirov00)

### Literatura

- https://web.archive.org/web/20060221103353/http://www.rpi.edu/dept/cits/files/ops.ppt
- https://pdodds.w3.uvm.edu/teaching/courses/2009-08UVM-300/docs/others/2005/wang2005a.pdf
- https://www.dopravniinzenyrstvi.cz/clanky/poznatky-z-vyzkumu-kapacity-vjezdu-do-okruzni-krizovatky/

## V jakém prostředí a za jakých podmínek probíhalo experimentální ověřování validity modelu

# Rozbor tématu a použitých metod/technologií

## Popis použitých postupů pro vytvoření modelu a zdůvodnění, proč jsou pro zadaný problém vhodné

Celá implementace byla vypracovaná v jazyce C++ a jeho standardní knihovny. Pro spracování argumenů Prpříkazové řádky byla použita knihovna `getopt_long` [TODO].
Pro překlad byl použit nástroj `GNU Make` [TODO]. Tyto nástroje jsou pro náš účel ideálni díky rozsáhle dokumentaci, jednoduchost použití, rychlosti a také díky tomu, že existuje mnoho zdrojů pro případné řešeni problémů.

## Popis původu použitých metod/technologií

TODO

# Koncepce - implementační témata

# Architektura simulačního modelu/simulátoru

Po spuštení programu se pomocí třídy `Args` zpracují argumenty příkazové řádky. Tyto argumenty jsou uložené ve tříde `Settings`, která je implementována jako návrhový vzor Jedináček.

Po zpracování argumentů se vytvoří instance třídy `Simulation`, která se stará o samotnou simulaci. Tato třída zabaluje všechny ostatní části simulace, jako kruhový objezd, příjezdové a výstupní cesty. Všechny tyto části jsou implementovány jako objekty, které dědí od třídy `Road`. Tato třída představuje cestu, bez ohledu na její typ (kruhový objezd, příjezdovou a výtsupní cestu). Obsahuje 2 1D vektory, `mRoad` a `mNextRoad` a metodu `update`, která vykoná aktualizaci všech buněk cesty a výsledek uloží do následující cesty. Na konci kroku se pomocí funkce `applyUpdate` nahradí `mRoad` za `mNextRoad` a `mNextRoad` je vynulována.

Jednotlivé dědící třídy, např. `Roundabout`, přepisují metodu `update` a implementují vlastní logiku pro aktualizaci stavu buněk. V základním nastavení jsou ramena křižovatky 4, tedy třída `Simulation` obsahuje 9 instancí objektů derivovaných od třídy `Road`.
1 pro samotný kruhový objezd (`Roundabout`), 4 pro příjedové cesty cesty (`Approach`), 4 pro odchozí cesty (`Outgoing`). Objekty třídy `Ougoing` také zbírají statistiky o průjezdech vozidel a ukládají je do objektu `Statistics`, který je také implementován jako návrhový vzor `Jedináček`.
Ten je po skončení simulace vypíše do logovacího souboru ve formátu CSV.

Jedna buňka je modelovaná třídou `Cell`. Buňka má svůj typ (výčtový typ `CellType`) a odkaz na společná metadata vozidla `CellMeta`. Metadata vozidla obsahují informace o rychlosti, jakým výjezdem vozidlo pojede z okružní křižovatky a podobně. Díky této architektuře se dají podstatné informace o vozidle aktualizovat na jednom místě - nedochází tak k nekonzistenci stavu.

## Mapování abstraktního modelu do simulačního modelu

# Podstata simulačních experimentů a jejich průběh

## Postup experimentování

## Jednotlivé experimenty

## Závěry experimentů

# Shrnutí simulačních experimentů a závěr
