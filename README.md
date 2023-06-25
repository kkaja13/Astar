# A* algoritmus - pro hledání nejkratší cesty v grafu

### 1) Použité knihovny
iostream: obsahuje standardní funkce pro vstup a výstup
fstream: slouží ke čtení grafu, tj. matice reprezentující bludiště
queue: implementace fronty, použité pro ukládání buněk na hranici
vector: slouží k ukládání dat (matice buněk, cesta, atd.)
string: použité ke čtení grafu ze souboru
cmath: zde pro výpočet absolutní hodnoty ve funkci heuristic

### 2) Popis a diskuze zvolených algoritmů
Funkce search(Cell start, Cell end) hledá nejkratší cestu od počátečního uzlu k cíli, další prvek vždy vybere z prioritní fronty, kde se navrchu nachází buňka s nejmenší hodnotou f = g + h. Hodnota g je vzdálenost od startu do aktuálního uzlu, takže součet předchozího g rodiče a vzdálenosti mezi rodičem a daným uzlem. Když uvažujeme čtverec o straně 10, pak po diagonále je vzdálenost 14 (odmocnina z 100 + 100), vertikální a horizontální soused je vzdálen 10. Hodnota h je heuristika, odhad vzdálenosti do cíle.

Funkce bool has_solution slouží k rozhodnutí, jestli existuje řešení. Vrátíme hodnotu true ve chvíli, kdy jsme našli cílový uzel. Pokud totiž neexistuje řešení (start/end jsou mimo rozsah, fronta je prázdná, tzn. neexistuje cesta k cíli), nevytváříme ani prošlou cestu.

### 3) Použití
v herních aplikacích pro hledání nejkratší cesty mezi dvěma body
k plánování tras a v navigačních systémech, hledání dopravního spojení

### 4) Příklad testů metod 
viz soubor main.cpp
vstupem (viz graf.txt) matice sousednosti, před ní je uvedený počet řádků a sloupců
matice reprezentuje vzdálenost jednotlivých uzlů, "0" v mřížce znázorňuje volný průchod, "1" překážku

### 7) Diskuze výběru algoritmů
Heurestická funkce využívá Diagonal distance, která se hodí pro pohyb osmi směry (vertikální, horizontální, diagonální), jiná implementace by zahrnovala Manhattan distance (součet absolutních hodnot rozdílů souřadnic v ose x a ose y), jež by měla být rychlejší ale méně efektivní, nebo Euclidean distance (analogie Pythagorova, odmocnina součtu čtverců rozdílů souřadnic v ose x a ose y). Je možné se zabývat konkrétními případy a použití co neefektivnější heurestiky.

Otevřený seznam (open list) uchovává neprozkoumané uzly, jež má smysl procházet. Vhodné je využít prioritní frontu, ze které snadno vyjmeme buňky s nejmenší hodnotou f.
Třída std::priority_queue implementuje prioritní frontu přes binární haldu s časovou složitostí O(log n), kde n je počet prvků ve frontě. Tak je zajištěno, že prvek s nejnižší f hodnotou je vždy na vrcholu fronty.

Namísto standartní implementace uzavřeného seznamu (closed list) jsem použila matici "explored", která obsahuje hodnoty bool, jestli byla buňka už prozkoumána. Takhle můžeme již prošlé buňky při procházení sousedů aktuálního prvku cesty přeskočit.

### 9) Diskuze k implementaci - možnosti dalšího vylepšení
Jiný způsob implementace prioritní fronty je přes Fibonacciho haldu, která může být časově výhodnější, ale je zároveň složitější a nezaručuje vždy rychlejší řešení.
