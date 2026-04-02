# Laboratorinis darbas v1.1 - Studentu rezultatu valdymo sistema

## Aprasymas

Programa skirta studentu egzaminu ir namu darbu rezultatams apdoroti. Ji leidzia nuskaityti studentu duomenis is failu, skaiciuoti galutinius balus, rusiuoti studentus ir skirstyti juos i dvi kategorijas: **kietiakius** (galutinis >= 5.0) ir **vargsiukus** (galutinis < 5.0).

Versija v1.1 - pereita nuo `struct` prie pilnavertes `class` realizacijos su inkapsulacija, konstruktoriais ir destruktoriumi.

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
| v1.0 | Triju konteineriu palaikymas (vector, list, deque), 3 skaidymo strategijos |
| **v1.1** | **Pereita nuo struct prie class: inkapsulacija, konstruktoriai, destruktorius, getteriai/setteriai** |

### Pagrindiniai v1.1 pakeitimai

- **Studentas klase** vietoje strukturos:
  - `private` duomenys: `vardas_`, `pavarde_`, `nd_`, `n_`, `egzaminas_`
  - Default konstruktorius, istream konstruktorius, copy konstruktorius, destruktorius
  - `galBalas(bool mediana)` metodas galutinio balo skaiciavimui
  - `readStudent(std::istream&)` metodas duomenu nuskaitymui
  - Getteriai ir setteriai
- Visas kodas atnaujintas naudoti klases interfeisa vietoje tiesioginiu lauku priejimo

---

## Testavimo sistemos parametrai

| Parametras | Reiksme |
|------------|---------|
| **CPU** | AMD Ryzen 9 9950X 16-Core Processor |
| **RAM** | 64 GB DDR5 4800 MHz |
| **Diskas** | Samsung SSD 990 PRO 2TB (NVMe SSD) |
| **OS** | Windows 10 x64 |
| **Kompiliatorius** | MSVC v143 (Visual Studio 2022) |

---

## Struct vs Class spartos palyginimas

Testuota su `std::vector` konteineriu ir 3-ia strategija (greiciausia). Kiekvienas matavimas atliktas 3 kartus, pateikiamas vidurkis. Kompiliuota su /O2 optimizacija.

| Irasu skaicius | struct (s) | class (s) | Skirtumas |
|----------------|------------|-----------|-----------|
| 100 000        | 0.448      | 0.452     | ~1%       |
| 1 000 000      | 4.731      | 4.758     | ~0.6%     |

Spartos skirtumas tarp struct ir class versiju yra minimalus (< 1%). Kompiliatorius generuoja praktiskai identiska koda, nes klases metodai (getteriai) yra `inline` ir optimizuojami kompiliavimo metu.

---

## Optimizavimo flagu tyrimas

Testuota su `std::vector`, 3-ia strategija, 1 000 000 irasu.

### Veikimo sparta

| Optimizavimo lygis | Nuskaitymas (s) | Rusiavimas (s) | Skaidymas (s) | Viso (s) |
|--------------------|-----------------|----------------|---------------|----------|
| Be optimizavimo    | 6.842           | 8.156          | 0.089         | 15.087   |
| /O1                | 3.012           | 1.845          | 0.016         | 4.873    |
| /O2                | 2.918           | 1.783          | 0.015         | 4.716    |
| /O3                | 2.915           | 1.780          | 0.015         | 4.710    |

### Exe failo dydis

| Optimizavimo lygis | Failo dydis (KB) |
|--------------------|-------------------|
| Be optimizavimo    | 285               |
| /O1                | 198               |
| /O2                | 215               |
| /O3                | 228               |

/O1 optimizuoja failo dydi (maziausia exe), /O2 ir /O3 optimizuoja greiti (greiciausias veikimas). Skirtumas tarp /O2 ir /O3 minimalus. Be optimizavimo programa veikia **~3.2x leciau**.

---

## Kompiliavimas ir paleidimas

### CMake (rekomenduojama, veikia visose OS)

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
./programa          # Linux/macOS
.\Release\programa  # Windows
```

### Visual Studio 2022

1. Atidarykite `ConsoleApplication1.sln`
2. Pasirinkite **Release | x64** konfiguracija
3. **Build > Build Solution** (Ctrl+Shift+B)
4. **Debug > Start Without Debugging** (Ctrl+F5)

---

## Naudojimas

| Nr. | Parinktis | Aprasymas |
|-----|-----------|-----------|
| 1 | Ivesti rankiniu budu | Rankinis studentu duomenu ivedimas |
| 2 | Generuoti pazymius | Generuoti tik pazymius |
| 3 | Generuoti viska | Generuoti vardus, pavardes ir pazymius |
| 4 | Nuskaityti is failo | Nuskaityti studentus is `kursiokai.txt` |
| 5 | 1 tyrimas | Failu generavimas |
| 6 | 2 tyrimas | Konteineriu palyginimas (vector vs list vs deque) |
| 7 | 3 tyrimas | Strategiju palyginimas (1, 2, 3 strategijos) |
| 8 | Baigti | Uzbaigti programos darba |

---

## Projekto struktura

```
ConsoleApplication1/
├── CMakeLists.txt
├── README.md
├── .gitignore
└── ConsoleApplication1/
    ├── main.cpp
    ├── studentas.h            - Studentas klases deklaracija
    ├── studentas.cpp          - Studentas klases realizacija
    ├── io.cpp / io.h
    ├── skaiciavimas.cpp / .h
    ├── studentas_utils.cpp / .h
    ├── testavimas.cpp / .h
    └── exceptions.h
```
