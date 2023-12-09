---
bibliography: doc/doc.bib
biblio-style: czechiso
---

# Úvod

V této práci je řešena implementace celulárního automatu [@ims č.s. 216], která bude použita pro sestavení modelu [@ims č.s. 7] jednoproudé okružní křižovatky (též kruhového objezdu).
Na základě modelu a simulačních experimentů [@ims č.s. 9] bude ukázáno chování systému [@ims č.s. 18] v podmínkách běžného dopravního provozu.
Smyslem experimentů je demonstrovat, že změnou parametrů okružní křižovatky, lze zvýšit kapacita (propustnost) těchto křižovatek.

## Kdo se na práci podílel

### Autoři

- Josef Kuchař (xkucha28)
- Matej Sirovatka (xsirov00)

### Literatura

V této práci jsme neměli pomoc odborného konzultanta, proto jsme čerpali z dostupné literatury. Implementace vychází primáně z článku "A Realistic Cellular Automata Model to Simulate Traffic Flow at Urban Roundabouts" [TODO].

## V jakém prostředí a za jakých podmínek probíhalo experimentální ověřování validity modelu

Validita modelu byla ověřovaná vůči datům z článku [TODO], ze kterého vychází naše implementace. Referenční implementace neexistuje, proto jsme se museli spolehnout na dostupná data z toho článku.

Jako druhý nezávislý zdroj referenčních dat jsme použili data z článku v časopisu Dopravní inženýrství [TODO].

# Rozbor tématu a použitých metod/technologií

TODO

## Popis použitých postupů pro vytvoření modelu a zdůvodnění, proč jsou pro zadaný problém vhodné

TODO - treba dodelat

Celá implementace byla vypracovaná v jazyce C++ a jeho standardní knihovny. Pro spracování argumenů Prpříkazové řádky byla použita knihovna `getopt_long` [TODO].

## Popis původu použitých metod/technologií

Veškerý kód je vlastním dílem napsaný autory této zprávy, žádné části nejsou převzaté. Implementace byla vypracovaná v jazyce C++ (standardu C++ 20) a jeho standardní knihovny.

Pro překlad byl použit nástroj `GNU Make` [TODO].

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

Experimenty probíhaly pomocí implementovaného simulačního nástroje. Všechny experimenty byly spuštěny na dobu 36000 kroků simulace, což odpovídá 10 hodinám reálného času. Tato doba byla zvolena na základě experimentů v článku [TODO], kde byla tato doba také zvolena.

## Jednotlivé experimenty

### Experiment 1

V tomto experimentu byl testován vliv průmeru okružní křižovatky na její propustnost. byl měněn v rozmezí 20 až 60 metrů.

TODO Graf

## Závěry experimentů

Bylo provedeno TODO experimentů v situacích popsaných výše. Z experimentů lze odvodit chování systémů s dostatečnou věrohodností a experimentální prověřování těchto ... situací již napřinese další výsledky, neboť ...

# Shrnutí simulačních experimentů a závěr

Z výsledků experimentů vyplývá, že změnou vnějšího rádiusu okružní křižovatky lze zvýšit její propustnost při předpokladu, že již není dostatečně velká. Validita modelu byla úspěšně ověřena, viz kapitola [TODO]. V rámci projektu vznikl nástroj, který vychází z modelu jednoproudové okružní křižovatky a byl implementován v jazyce C++ a jeho standardní knihovny. Nástroj je schopen simulačního experimentování s parametry křižovatky a výstupními daty jsou statistiky o průjezdech vozidel. Jednotlivé parametry lze měnit jednoduše pomocí argumentů příkazové řádky.

- https://web.archive.org/web/20060221103353/http://www.rpi.edu/dept/cits/files/ops.ppt
- https://pdodds.w3.uvm.edu/teaching/courses/2009-08UVM-300/docs/others/2005/wang2005a.pdf
- https://www.dopravniinzenyrstvi.cz/clanky/poznatky-z-vyzkumu-kapacity-vjezdu-do-okruzni-krizovatky/
