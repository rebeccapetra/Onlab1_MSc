# Onlab1

Kódmagyarázat

# PrefixTrie osztály:
   Ez az az osztály, ami segít a bináris prefix fa felépítésében, tárolásában.
   Egy csomópont tartalmazza a nexthop azonosítót és pointereket a leszármazottakra.
   Függvényei (konstruktoron és destruktoron kívül):
   
   - isLeaf() : megmondja egy adott csomópontról, hogy vannak-e gyerekei vagy ő maga már levél-e
   - justOneLeaf() : igazat ad vissza, ha a csomópontnak csak egy leszármazottja van
   - createMissingLeaf() : (ez a függvény csak akkor kerül használtara, ha a node-nak csak egy levele van) létrehozza a hiányzó levelét a node-nak
   - isEmpty() : ellenőrzi, hogy a csomópont tartalmaz e nexthop azonosítót
   
   - addNewIP(root, ip, mask, index) : a beolvasás során ez a függvény adja hozzá a fához az új IP címeket és írja bele a nexthop azonosítót ha van
	
  
 # HopTable osztály:
   Ez az osztály tárolja a nexthopokat és a hozzájuk tartozó azonosítókat, illetve az ezek használatához szükséges fontosabb függvényeket
   A nexthop-azonosító párosok egy std::vector tömbben vannak eltárolva. Az azonosító esetünkben az index lesz. pl. HopTable[1] = 2345 (2345 a nexthop IP-cím, 1 az azonosító)
   Függvényei:
   - returnKeyValue(ip) : visszaadja az IP-címhez tartozó azonosítót
   - insertNewHopIP(ip) : hozzáad a vector-tömbhöz egy új nexthopot
   - printHopTable() : ez csak a tesztelhetőség miatt szükséges segédfüggvény, ami kiíratja a nexthop táblát
   - getCount() : visszaadja a nexthop tábla méretét


# Trunk struktúra, showTrunks(), printTree()
   Ezek nem általam írt kódrészletek, a prefix fa kiíratásához használtam, hogy strukturáltan tudjam kiíratni a könnyebb ellenőrizhetőségért
 
 
# EntropyPasses osztály:
 Ez az osztály tartamazza azokat a függvényeket, lépéseket, amik az entópiakiszámításhoz szükségesek.
 Ezen osztály egyik függvényéhez(pass_three_bfs) volt szükség a pass3_return_type struktúra bevezetésére (lásd a függvény tárgyalásában)
 Függvényei:
    
   - pass_one_recursive(node, key): megkapja a FIB tábla alapján felépített fának a gyökerét, majd rekurzívan végigmegy a fán. A függvény lényege,hogy itt történik a leafpushing, aminek az első lépése, hogy kibővítjük a fát új csomópontokkal ott, ahol a node-nak csak egy leszármazottja van. Ez mellett pedig a belső csomópontokban lévő értékek 'lenyomódnak' a levelekbe.
          Működés:
           1. levél csomópontban vagyunk? 
            > ha igen, és a csomópont üres, odaadjuk neki azt az azonosítót, amit a rekurzív függvény paraméterként megkapott a szülőtől, utána vége a futásnak
           2. ha nem, akkor megnézzük, hogy egy levele van-e csak a csomópontnak?
            > ha igen, létrehozzuk a hiányzó levelet
           3. üres a csomópont amiben vagyunk? 
            > ha üres, a gyerekeire meghívott rekurzív függvény a key paraméterben azt az értéket kapja meg, amit ő is kapott a key változóban
            > ha nem üres, azt az azonosítót adjuk tovább a gyerekeinek, amit ő tartalmazott; az ő azonosítóját pedig töröljük
           4. meghívjuk a rekurzív függvényt a leszármazottakra 
       
   - pass_two_recursive(node): az előbbi függvény által létrejött gráfon végzünk egyszerűsítéseket, ugyanis előfordulhat, hogy egy csomópont két levele ugyanazt az értéket tartalmazza, ami redundanciához vezet és ilyenkor ezt a két levelet törölhetjük, a bennük lévő azonosítót pedig a szülő csomópontba származtathatjuk
          Működés:
           1. levél csomópontban vagyunk?
            > ha igen, véget ér a futás
           2. ha nem levél csomópontban vagyunk, meghívjuk a függvényt a csomópont gyerekeire is
           3. ellenőrizzük, hogy a csomópont gyerekei levelek-e, és ha azok akkor megnézzük hogy a bennük lévő azonosító megegyezik-e
            > ha igen, töröljük a két levelet, az azonosítót pedig megkapja a szülő 
        
   - pass_three_bfs(root, hoptablesize): ennek a függvénynek a visszatérési értéke a fentebb említett pass3_return_type struktúra; a függvény célja, hogy BFS bejárással végigmenjen a létrejött leafpushed egyszerűsített fán és létrehozza az S_i és S_alfa értékeket, amire majd entrópiát lehet számolni
        Továbbá ebben a függvényben számoltam össze a BFS bejárás alatt, hogy a nexthoptáblákban lévő elemek hányszor szerepelnek a fában, és hogy hány levele van a fának.
          Működés:
          S_i: 0-1-ek sorozatát tartalmazza, 0-t ha a csomópont belső csomópont, 1-et ha levél, szintenként végighaladva a fán
          S_alfa: ugyanezzel a bejárással tartalmazza a levelekben lévő azonosítók értékét
          CounterTable: nexthoptábla méretű vector tömb, ami azt tartalmazza, hogy az azonosítók hányszor szerepelnek a fában
          count_leaf: levelek darabszáma
          1. BFS bejárással egy queue-t használva végigjártam a fát
            > ha belső csomópontban vagyunk, S_i-hez hozzáfűzünk egy 0-t
            > ha levél csomópontban vagyunk, S_i-hez hozzáfűzünk egy 1-t, S_alfa-hez pedig az index értékét, a CounterTable megfelelő elemét növeljük egyel, illetve a count_leaf-et is növeljük
          2. a visszatérési érték pedig a pass3_return_type, ami tartalmazza az S_alfa-t, S_i-t, a CounterTable-t és count_leaf értékeket
      
      
   - returnEntropy(CounterTable,leafnumber): a CounterTable az előző függvény által visszaadott vector tömb, ami a nexthopok előfordulását tartalmazza; a leafnumber pedig a levelek száma
        Ebben a függvényben az entrópia számításhoz megadott képleteket használjuk.
        H(S_alfa) = 2 + summa(p_i*log_2(1/p_i));
        p_i - az adott azonosító előfordulása a végső fában ( képlete : i azonosító előfordulása/összes levél darabszáma)

# Main függvény:
Itt történik az osztályok példányosítása, a beolvasás és a megfelelő függvények meghívása.
Beolvasás:
-soronként olvasunk be
-mivel tudjuk hogy minden sor ugyanazt a sablont követi (x.y.z.w/k q.a.s.t), ezért scanf-fel könnyedén ki lehet emelni a számokat külön (x,y,z,w,k,q,a,s,t) a sorokból
-az ip címeket a számok eltolásával létrehozzuk 
-az első ip címet bitsettel eltároljuk
-hozzáadjuk őket a prefix fához és a nexthop táblához
Ezt követően futtatjuk rajtuk az EntropyPasses függvényeit.

Ellenőrzésképpen ki lehet íratni minden függvény előtt-után a prefix-fát, a nexthoptáblát, a CounterTable-t, és még akár soronként annak is az értékét, hogy mi kerül be a prefix-fába (az addNewIP függvény tartalmaz egy kikommentezett cout sort, ami kiírja az IP-t bináris formában, a netmaszkot, az azonosítót amit a csomópont megkap)
