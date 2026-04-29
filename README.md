# Laboratorinis darbas v1.2 - Studentu rezultatu valdymo sistema

## Aprasymas

Programa skirta studentu egzaminu ir namu darbu rezultatams apdoroti. Ji leidzia nuskaityti studentu duomenis is failu, skaiciuoti galutinius balus, rusiuoti studentus ir skirstyti juos i dvi kategorijas: **kietiakius** (galutinis >= 5.0) ir **vargsiukus** (galutinis < 5.0).

Versija v1.0 papildyta triju konteineriu tipu (`std::vector`, `std::list`, `std::deque`) palaikymu, triju skaidymo strategiju realizacija bei issamiu spartos tyrimu.

### Galutinio balo formule

```
Galutinis = 0.4 * (vidurkis arba mediana namu darbu) + 0.6 * egzamino balas
```

---

## Versiju istorija (Release History)

| Versija | Aprasymas |
|---------|-----------|
| v0.1 | Pradine versija: rankinis ivedimas, pazymiu generavimas, galutinio balo skaiciavimas |
| v0.2 | Failu skaitymas, duomenu apdorojimas is `kursiokai.txt` |
| v0.3 | Kodo refaktorizavimas: atskiri `.h/.cpp` failai, exception handling |
| v0.4 | Failu generatorius, studentu skirstymas i kategorijas, spartos tyrimai |
| v1.0 | Triju konteineriu palaikymas (vector, list, deque), 3 skaidymo strategijos, issamios spartos analizes, CMake palaikymas |
| v1.1 | Konteineriu tyrimai, spartos palyginimas, kodo struktura optimizuota |
| **v1.2** | **Rule of Five Studentas klasei, perdengtų I/O operatoriai (operator<<, operator>>), rankiniai testai** |

---

## v1.2 pakeitimai

### Rule of Five - Studentas klase

Versijoje v1.2 `struct Studentas` pertvarkytas i tikra `class Studentas` su pilnu **Rule of Five** realizavimu. Kadangi klasės duomenų nariai yra `public`, `operator<<` ir `operator>>` realizuoti kaip paprastos laisvos funkcijos (be `friend`).

#### Rule of Five metodai

| Metodas | Aprasymas |
|---------|-----------|
| `Studentas()` | **Default konstruktorius** — inicializuoja tuščius duomenis: `vardas=""`, `pavarde=""`, `nd={}`, `n=0`, `egzaminas=0` |
| `~Studentas()` | **Destruktorius** — eksplicitinis, atlaisvina resursus (std::string ir std::vector automatiskai valo save) |
| `Studentas(const Studentas& kitas)` | **Kopijavimo konstruktorius** — pilnai kopijuoja visus laukus, originalo nepakeicia |
| `Studentas& operator=(const Studentas& kitas)` | **Kopijavimo priskyrimo operatorius** — kopijuoja laukus, tikrina savipriskyrimą (`this == &kitas`) |
| `Studentas(Studentas&& kitas) noexcept` | **Perkėlimo konstruktorius** — perima resursus (std::move), saltinio `n=0`, `egzaminas=0` |
| `Studentas& operator=(Studentas&& kitas) noexcept` | **Perkėlimo priskyrimo operatorius** — perima resursus, tikrina saviperkėlimą |

#### Perdengtų I/O operatorių aprašas

| Operatorius | Sintaksė | Aprasymas |
|-------------|----------|-----------|
| `operator<<` | `out << studentas` | Isveda studento duomenis i bet kuri `std::ostream` srautą (ekranas arba failas) |
| `operator>>` | `in >> studentas` | Nuskaito studento duomenis is bet kurio `std::istream` srautą (klaviatura arba failas) |

**Formatas (simetriskas abiem operatoriams):**
```
vardas pavarde n nd[0] nd[1] ... nd[n-1] egzaminas
```

Pvz.: `Jonas Jonaitis 3 8 9 7 10`

#### Duomenų įvesties ir išvesties būdai

| Būdas | Kaip naudoti | Pavyzdys |
|-------|-------------|---------|
| **Rankinis ivedimas** (klaviatura) | `std::cin >> studentas` | Vartotojas iraso: `Jonas Jonaitis 3 8 9 7 10` |
| **Automatinis ivedimas** (is eilutes) | `std::istringstream iss("..."); iss >> s;` | Ivedimas is programiskai sukurtos eilutes |
| **Ivedimas is failo** | `std::ifstream f("failas.txt"); f >> s;` | Nuskaitymas is tekstinio failo |
| **Isvedimas i ekrana** | `std::cout << studentas` | Rodo `Jonas Jonaitis 3 8 9 7 10` |
| **Isvedimas i faila** | `std::ofstream f("failas.txt"); f << s;` | Iraso i faila tuo paciu formatu |

#### Kodo pavyzdžiai

**Isvedimas i ekrana:**
```cpp
Studentas s;
s.vardas = "Jonas"; s.pavarde = "Jonaitis";
s.n = 2; s.nd = {8, 9}; s.egzaminas = 10;
std::cout << s;
// Isvedimas: Jonas Jonaitis 2 8 9 10
```

**Ivedimas is failo:**
```cpp
std::ifstream failas("studentai.txt");
Studentas s;
failas >> s;
// Nuskaito viena eilute: "Jonas Jonaitis 2 8 9 10"
```

**Perkėlimo operacijos (move semantics):**
```cpp
Studentas s1;
s1.vardas = "Petras"; s1.n = 3; s1.nd = {7,8,9}; s1.egzaminas = 6;

Studentas s2 = std::move(s1);  // perkėlimo konstruktorius
// s2 turi visus s1 duomenis
// s1.n == 0, s1.egzaminas == 0 (saltinis istusejo)
```

### Rankiniai testai (testuotiKlase)

Pasirinkus meniu punkta **8**, paleidziamas `testuotiKlase()` — rankinis testas, kuris tikrina kiekviena Rule of Five metoda ir abu I/O operatorius. Kiekvienas patikrinimas spausdina `[OK]` arba `[FAIL]`.

| Testuojamas metodas | Patikrinimas |
|--------------------|--------------|
| Default konstruktorius | Tikriname ar `n==0`, `egzaminas==0`, `vardas==""` |
| Kopijavimo konstruktorius | Keičiame kopija, tikriname ar originalas nepakito |
| Kopijavimo priskyrimas | Analogiskai, plus savipriskyrimų saugumas |
| Perkėlimo konstruktorius | Tikriname ar šaltinis istusejo (`n==0`) |
| Perkėlimo priskyrimas | Analogiskai, plus saviperkėlimo saugumas |
| `operator<<` | Isvedame i `ostringstream`, lyginame su laukiama eilute |
| `operator>>` | Skaitome is `istringstream`, tikriname kiekvieną lauką |
| Destruktorius | Implicitinis testas per scope pabaiga |

---

### Pagrindiniai v1.0 pakeitimai

- **Konteineriu palaikymas** - programa testuojama su `std::vector`, `std::list` ir `std::deque` konteineriais naudojant C++ template funkcijas
- **3 skaidymo strategijos**:
  - **1 strategija** - kopijuoti studentus i du naujus konteinerius (originalas lieka nepakeistas)
  - **2 strategija** - vargsiukus perkelti i nauja konteinerio ir istrinti is originalo
  - **3 strategija** - optimizuota su `std::stable_partition` ir efektyviu range perkėlimu
- **Issamios spartos analizes** - konteineriu ir strategiju palyginimas su skirtingo dydzio duomenu rinkiniais
- **CMake palaikymas** - paruostas `CMakeLists.txt` kompiliavimui bet kurioje OS

---

## Testavimo sistemos parametrai

| Parametras | Reiksme |
|------------|---------|
| **CPU** | AMD Ryzen 9 9950X 16-Core Processor |
| **RAM** | 64 GB DDR5 4800 MHz |
| **Diskas** | Samsung SSD 990 PRO 2TB (NVMe SSD) |
| **OS** | Windows 10 x64 |
| **Kompiliatorius** | MSVC v143 (Visual Studio 2022) |
| **Konfiguracija** | Release x64, /O2 optimizacija |

---

## Kompiliavimas ir paleidimas

### CMake (rekomenduojama, veikia visose OS)

```bash
# Sukurti build kataloga
mkdir build
cd build

# Sugeneruoti projekta (Release rezimu)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Kompiliuoti
cmake --build . --config Release

# Paleisti
./programa          # Linux/macOS
.\Release\programa  # Windows
```

### Visual Studio 2022

1. Atidarykite `ConsoleApplication1.sln`
2. Pasirinkite **Release | x64** konfiguracija
3. Paleiskite **Build > Build Solution** (Ctrl+Shift+B)
4. Paleiskite programa **Debug > Start Without Debugging** (Ctrl+F5)

---

## Naudojimas

### Meniu parinktys

| Nr. | Parinktis | Aprasymas |
|-----|-----------|-----------|
| 1 | Ivesti rankiniu budu | Rankinis studentu duomenu ivedimas |
| 2 | Generuoti pazymius | Generuoti tik pazymius (vardai ivedami rankiniu budu) |
| 3 | Generuoti viska | Generuoti vardus, pavardes ir pazymius |
| 4 | Nuskaityti is failo | Nuskaityti studentus is `kursiokai.txt` |
| 5 | 1 tyrimas | Failu generavimas ir kurimo spartos matavimas |
| 6 | 2 tyrimas | Konteineriu palyginimas (vector vs list vs deque) |
| 7 | 3 tyrimas | Strategiju palyginimas (1, 2, 3 strategijos) |
| **8** | **Testuoti Studentas klase** | **Rule of Five + I/O operatoriu rankinis testas** |
| 9 | Baigti | Uzbaigti programos darba |

### Testavimo eiga

1. Paleiskite programa (**Release** konfiguracija butina korektiskim rezultatams)
2. Pasirinkite galutinio balo skaiciavimo buda (1 - vidurkis, 2 - mediana)
3. Pasirinkite **5** - sugeneruojami testiniai failai (1 000 - 1 000 000 irasu)
4. Pasirinkite **6** - atliekamas konteineriu palyginimo tyrimas
5. Pasirinkite **7** - atliekamas strategiju palyginimo tyrimas

---

## Spartos tyrimo rezultatai

Visi matavimai atlikti **3 kartus**, pateikiamas vidurkis. Naudojama **mediana** galutiniam balui skaiciuoti. Testiniai failai sugeneruoti viena karta ir naudojami visiems tyrimams (tyrimo patikimumui).

### 1 tyrimas: Failu kurimas

| Irasu skaicius | 1 bandymas (s) | 2 bandymas (s) | 3 bandymas (s) | Vidurkis (s) |
|----------------|----------------|----------------|----------------|--------------|
| 1 000          | 0.00296        | 0.00281        | 0.00287        | **0.00288**  |
| 10 000         | 0.02759        | 0.02631        | 0.02613        | **0.02668**  |
| 100 000        | 0.26101        | 0.24913        | 0.25015        | **0.25343**  |
| 1 000 000      | 2.64710        | 2.68054        | 2.68823        | **2.67196**  |

Failu kurimo laikas auga tiesiskai priklausomai nuo irasu skaiciaus (~2.67s / 1M irasu).

---

### 2 tyrimas: Konteineriu palyginimas

Matuojami 3 zingsniai kiekvienam konteinerio tipui: **nuskaitymas** is failo, **rusiavimas** pagal galutini bala (didejimo tvarka), **skaidymas** i dvi grupes (1-a strategija). Kiekvienas matavimas atliktas 3 kartus, pateikiamas vidurkis.

#### 1 000 irasu

| Konteineris | Nuskaitymas (s) | Rusiavimas (s) | Skaidymas (s) |
|-------------|-----------------|----------------|----------------|
| vector      | 0.00304         | 0.00109        | 0.00019        |
| list        | 0.00302         | 0.00120        | 0.00019        |
| deque       | 0.00297         | 0.00110        | 0.00018        |

#### 10 000 irasu

| Konteineris | Nuskaitymas (s) | Rusiavimas (s) | Skaidymas (s) |
|-------------|-----------------|----------------|----------------|
| vector      | 0.02927         | 0.01330        | 0.00250        |
| list        | 0.02928         | 0.01869        | 0.00227        |
| deque       | 0.02926         | 0.01343        | 0.00212        |

#### 100 000 irasu

| Konteineris | Nuskaitymas (s) | Rusiavimas (s) | Skaidymas (s) |
|-------------|-----------------|----------------|----------------|
| vector      | 0.29364         | 0.13971        | 0.02779        |
| list        | 0.28975         | 0.27207        | 0.03619        |
| deque       | 0.29225         | 0.15333        | 0.03182        |

#### 1 000 000 irasu

| Konteineris | Nuskaitymas (s) | Rusiavimas (s) | Skaidymas (s) |
|-------------|-----------------|----------------|----------------|
| vector      | 2.91874         | 1.78334        | 0.31848        |
| list        | 2.98777         | 3.94837        | 0.41607        |
| deque       | 2.93293         | 2.14860        | 0.37310        |

#### Konteineriu palyginimo analize

**Nuskaitymas:** Visi trys konteineriai veikia praktiskai vienodai (~2.9s / 1M irasu), nes `push_back` yra amortizuotas O(1) visiems tipams.

**Rusiavimas:** Cia matome didziausia skirtuma:

| Konteineris | 100K rusiavimas | 1M rusiavimas | Santykis su vector |
|-------------|-----------------|---------------|---------------------|
| vector      | 0.140s          | 1.783s        | 1.00x (bazinis)     |
| list        | 0.272s          | 3.948s        | **2.21x leciau**    |
| deque       | 0.153s          | 2.149s        | 1.20x leciau        |

`std::vector` yra greiciausias rusiavimui, nes duomenys saugomi istisiniame atminties bloke (puiki cache lokalizacija). `std::list` yra **2.2x leciau** nei vector, nes kiekvienas mazgas yra atskiroje atminties vietoje (bloga cache lokalizacija). `std::deque` yra tarp ju - duomenys saugomi blokais, todel cache lokalizacija geresnė nei list, bet blogesnė nei vector.

**Skaidymas (1 strategija):** Visi konteineriai veikia panasiai, nes 1-a strategija tik iteruoja per elementus ir kopijuoja juos i naujus konteinerius (O(n)).

---

### 3 tyrimas: Strategiju palyginimas

Matuojamas **tik skaidymo i grupes laikas** (nuskaitymas ir rusiavimas neitraukiami). Duomenys pries skaidyma surusiuojami pagal galutini bala didejimo tvarka — todel vargsiukai (< 5.0) yra konteinerio **pradzioje**.

#### std::vector

| Irasu skaicius | 1 strategija (s) | 2 strategija (s) | 3 strategija (s) |
|----------------|-------------------|-------------------|-------------------|
| 1 000          | 0.00018           | 0.00129           | 0.00010           |
| 10 000         | 0.00231           | 0.11453           | 0.00139           |
| 100 000        | 0.02424           | **11.28164**      | 0.01524           |

#### std::list

| Irasu skaicius | 1 strategija (s) | 2 strategija (s) | 3 strategija (s) |
|----------------|-------------------|-------------------|-------------------|
| 1 000          | 0.00017           | 0.00010           | 0.00010           |
| 10 000         | 0.00234           | 0.00146           | 0.00144           |
| 100 000        | 0.02884           | 0.01918           | 0.02297           |

#### std::deque

| Irasu skaicius | 1 strategija (s) | 2 strategija (s) | 3 strategija (s) |
|----------------|-------------------|-------------------|-------------------|
| 1 000          | 0.00015           | 0.00010           | 0.00011           |
| 10 000         | 0.00198           | 0.00142           | 0.00157           |
| 100 000        | 0.02797           | 0.01955           | 0.02185           |

#### Strategiju palyginimo analize

Strategiju tyrimo rezultatai aiskiai parodo kiekvieno konteinerio ir strategijos privalumus bei trukumus:

**2 strategija su std::vector — katastrofiskai leta:**

| Irasu sk. | 1 strategija | 2 strategija | Skirtumas |
|-----------|-------------|-------------|-----------|
| 1 000     | 0.00018s    | 0.00129s    | 7x        |
| 10 000    | 0.00231s    | 0.11453s    | **50x**   |
| 100 000   | 0.02424s    | 11.28164s   | **465x**  |

Su 100 000 irasu 2 strategija su vector uztruko **11.28 sekundziu** — tai **465 kartus leciau** nei 1 strategija ir **740 kartus leciau** nei 3 strategija! Priezastis: kadangi duomenys surusiuoti didejimo tvarka, vargsiukai yra konteinerio pradzioje. Kiekvienas `erase()` is vector pradzios perstumia **visus** likusius elementus (O(n)), o tai sukuria O(n²) bendra sudetinguma.

**2 strategija su std::list ir std::deque — greita:**

Su `std::list`, 2 strategija veikia **greiciau** nei 1 strategija (0.019s vs 0.029s su 100K), nes `list::erase()` yra O(1) operacija — trinant elementa tereikia perjungti rodykles, jokie elementai nestumiami.

Su `std::deque`, 2 strategija taip pat veikia gerai (0.020s vs 0.028s su 100K), nes vargsiukai yra konteinerio pradzioje, o `deque::erase()` is pradzios yra efektyvi O(1) operacija (panasiai kaip `pop_front`).

**3 strategija — universaliai greita:**

3 strategija naudoja `std::stable_partition`, kuris vienu perejimu perkelia kietiakius i konteinerio pradzia. Tai veikia efektyviai su **visais** konteineriu tipais:

| Konteineris | 100K: 2 strategija | 100K: 3 strategija | Pagreitejimas |
|-------------|--------------------|--------------------|---------------|
| vector      | 11.28164s          | 0.01524s           | **740x**      |
| list        | 0.01918s           | 0.02297s           | ~1x           |
| deque       | 0.01955s           | 0.02185s           | ~1x           |

Su vector, 3 strategija yra **740 kartu greitesne** nei 2 strategija. Su list ir deque skirtumas minimalus, nes 2 strategija jau veikia efektyviai siem konteineriams.

---

## Strategiju aprasymas

### 1 strategija (du nauji konteineriai)

Bendras studentu konteineris lieka nepakeistas. Kiekvienas studentas **kopijuojamas** i viena is dvieju nauju konteineriu (`kietiakiai` arba `vargsiukai`) pagal galutini bala.

- **Sudetingumas:** O(n) visiems konteineriams
- **Privalumai:** Paprastas, vienodai veikia su visais konteineriais
- **Trukumai:** Neefektyvus atminties atzvilgiu — tas pats studentas egzistuoja dviejuose konteineriuose (originalas + kopija)

### 2 strategija (vienas naujas konteineris, trynimas is originalo)

Vargsiukai perkeliami (`std::move`) i nauja konteinerio ir istrinami is bendro konteinerio. Po operacijos originaliame konteineryje lieka tik kietiakiai.

- **Sudetingumas:** O(n) list/deque, **O(n²) vector**
- **Privalumai:** Efektyviau atminties atzvilgiu — kiekvienas studentas egzistuoja tik viename konteineryje
- **Trukumai:** Katastrofiskai letas `std::vector` konteineriui (465x leciau su 100K irasu)

### 3 strategija (optimizuota su std::stable_partition)

Naudojamas `std::stable_partition` algoritmas, kuris vienu perejimu perkelia visus kietiakius i konteinerio pradzia, o vargsiukus — i gala, issaugodamas pradiniu elementu tvarka. Tada vargsiukai efektyviai iskeliami vienu range perkėlimu.

- **Sudetingumas:** O(n) su papildoma atmintimi, O(n log n) be jos
- **Privalumai:** Universaliai efektyviausias metodas visiems konteineriu tipams
- **Naudojami STL algoritmai:**
  - `std::stable_partition` — efektyvus konteinerio skaidymas i dvi grupes
  - `std::make_move_iterator` — efektyvus elementu perkelimas (vector/deque)
  - `list::splice` — O(1) elementu perkelimas tarp sarasu (list)

---

## Konteineriu savybiu santrauka

| Savybe | std::vector | std::list | std::deque |
|--------|-------------|-----------|------------|
| Atmintis | Istisinis blokas | Atskiri mazgai | Bloku masyvas |
| Nuskaitymas (push_back) | Amortizuotas O(1) | O(1) | Amortizuotas O(1) |
| Prieiga pagal indeksa | O(1) | O(n) | O(1) |
| Trynimas is pradzios | **O(n)** | O(1) | O(1) |
| Trynimas is vidurio | **O(n)** | O(1) | O(n) |
| Rusiavimas | std::sort O(n log n) | list::sort O(n log n) | std::sort O(n log n) |
| Cache draugiskumas | Labai geras | Blogas | Vidutinis |

---

## Projekto struktura

```
Labaratorinis-v0.1/
├── CMakeLists.txt                     - CMake kompiliavimo failas
├── README.md                          - Dokumentacija
├── .gitignore                         - Git ignoruojami failai
├── ConsoleApplication1.sln            - Visual Studio sprendimas
└── ConsoleApplication1/
    ├── main.cpp                       - Pagrindine programa su meniu (9 punktai)
    ├── io.cpp / io.h                  - Ivedimo/isvedimo funkcijos
    ├── skaiciavimas.cpp / .h          - Balu skaiciavimo funkcijos
    ├── studentas.h                    - Studentas klase (Rule of Five + I/O operatoriu deklaracijos)
    ├── studentas.cpp                  - Studentas klases realizacija (Rule of Five + operator<<, >>)
    ├── studentas_utils.cpp / .h       - Studentu pagalbines funkcijos
    ├── testavimas.cpp / .h            - Tyrimai + testuotiKlase() rankinis testas
    ├── exceptions.h                   - Klaidu klases (FailoKlaida, DuomenuKlaida)
    └── ConsoleApplication1.vcxproj    - Visual Studio projekto failas
```

---

## Isvados

1. **Nuskaitymas:** Visi trys konteineriai veikia praktiskai vienodai (~2.9s / 1M), nes `push_back` yra amortizuotas O(1).
2. **Rusiavimas:** `std::vector` greiciausias (1.78s / 1M), `std::list` leciausias (3.95s / 1M, **2.2x leciau**) del blogos cache lokalizacijos. `std::deque` tarpinis variantas (2.15s / 1M).
3. **1 strategija:** Vienodai O(n) visiems konteineriams, taciau neefektyvi atminties atzvilgiu (duomenys dubliuojami).
4. **2 strategija:** Kritiski neefektyvi `std::vector` konteineriui (**465x leciau** su 100K irasu) del O(n) trynimo is pradzios. Taciau puikiai veikia su `std::list` (O(1) trynimas) ir `std::deque` (efektyvus trynimas is pradzios).
5. **3 strategija:** Universaliai efektyviausias metodas — `std::stable_partition` uztikrina O(n) sudetinguma visiems konteineriams. Su vector pasiektas **740x pagreitejimas** palyginus su 2 strategija.
6. **Geriausia kombinacija:** `std::vector` + 3 strategija — greiciausias rusiavimas ir efektyvus skaidymas.
7. **Release** konfiguracija butina korektiskim rezultatams — Debug rezimas gali buti 10-100x leciau.
