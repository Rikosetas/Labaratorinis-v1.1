#include "testavimas.h"
#include "skaiciavimas.h"
#include "studentas_utils.h"
#include "exceptions.h"
#include "studentu_io.h"
#include "vector_stl.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <deque>

static int g_ok   = 0;
static int g_fail = 0;

static void spausdintiRezultata( const std::string& testas, bool rezultatas )
{
    std::cout << "  " << ( rezultatas ? "[OK]  " : "[FAIL]" ) << " " << testas << "\n";
    if ( rezultatas ) ++g_ok; else ++g_fail;
}

static Studentas sukurtiTestoStudenta()
{
    Studentas s;
    s.vardas    = "Jonas";
    s.pavarde   = "Jonaitis";
    s.n         = 3;
    s.nd        = { 8, 9, 7 };
    s.egzaminas = 10;
    return s;
}

void testuotiKlase()
{
    std::cout << "\n======== Studentas klases testai ========\n\n";

    std::cout << "0. Zmogus abstrakti klase:\n";
    spausdintiRezultata( "std::is_abstract<Zmogus>::value == true",
                         std::is_abstract<Zmogus>::value == true );
    spausdintiRezultata( "std::is_abstract<Studentas>::value == false",
                         std::is_abstract<Studentas>::value == false );
    spausdintiRezultata( "Studentas yra isvestine is Zmogus",
                         std::is_base_of<Zmogus, Studentas>::value == true );

    std::cout << "\n1. Default konstruktorius:\n";
    {
        Studentas s;
        spausdintiRezultata( "vardas == \"\"",    s.vardas    == "" );
        spausdintiRezultata( "pavarde == \"\"",   s.pavarde   == "" );
        spausdintiRezultata( "n == 0",            s.n         == 0  );
        spausdintiRezultata( "egzaminas == 0",    s.egzaminas == 0  );
        spausdintiRezultata( "nd.empty()",        s.nd.empty()      );
    }

    std::cout << "\n2. Kopijavimo konstruktorius:\n";
    {
        Studentas s1 = sukurtiTestoStudenta();
        Studentas s2( s1 );

        spausdintiRezultata( "s2.vardas == \"Jonas\"",      s2.vardas    == "Jonas"    );
        spausdintiRezultata( "s2.pavarde == \"Jonaitis\"",  s2.pavarde   == "Jonaitis" );
        spausdintiRezultata( "s2.n == 3",                  s2.n         == 3          );
        spausdintiRezultata( "s2.egzaminas == 10",         s2.egzaminas == 10         );
        spausdintiRezultata( "s2.nd == {8,9,7}",           s2.nd        == std::vector<int>{ 8, 9, 7 } );

        s2.vardas    = "Petras";
        s2.nd[ 0 ]   = 1;
        spausdintiRezultata( "originalas nepakito (vardas)", s1.vardas  == "Jonas" );
        spausdintiRezultata( "originalas nepakito (nd[0])",  s1.nd[ 0 ] == 8       );
    }

    std::cout << "\n3. Kopijavimo priskyrimas (operator=):\n";
    {
        Studentas s1 = sukurtiTestoStudenta();
        Studentas s2;
        s2 = s1;

        spausdintiRezultata( "s2.vardas == \"Jonas\"",     s2.vardas    == "Jonas"    );
        spausdintiRezultata( "s2.n == 3",                  s2.n         == 3          );
        spausdintiRezultata( "s2.egzaminas == 10",         s2.egzaminas == 10         );
        spausdintiRezultata( "s2.nd == {8,9,7}",           s2.nd        == std::vector<int>{ 8, 9, 7 } );

        s2.pavarde = "Kazlauskas";
        spausdintiRezultata( "originalas nepakito (pavarde)", s1.pavarde == "Jonaitis" );

        s1 = s1;
        spausdintiRezultata( "savipriskyrimass saugus",    s1.vardas == "Jonas" );
    }

    std::cout << "\n4. Perkėlimo konstruktorius (move ctor):\n";
    {
        Studentas s1 = sukurtiTestoStudenta();
        Studentas s2( std::move( s1 ) );

        spausdintiRezultata( "s2.vardas == \"Jonas\"",     s2.vardas    == "Jonas"    );
        spausdintiRezultata( "s2.n == 3",                  s2.n         == 3          );
        spausdintiRezultata( "s2.egzaminas == 10",         s2.egzaminas == 10         );
        spausdintiRezultata( "s2.nd == {8,9,7}",           s2.nd        == std::vector<int>{ 8, 9, 7 } );
        spausdintiRezultata( "s1.n == 0 (saltinis istusejo)", s1.n         == 0 );
        spausdintiRezultata( "s1.egzaminas == 0",             s1.egzaminas == 0 );
    }

    std::cout << "\n5. Perkėlimo priskyrimas (move operator=):\n";
    {
        Studentas s1 = sukurtiTestoStudenta();
        Studentas s2;
        s2 = std::move( s1 );

        spausdintiRezultata( "s2.vardas == \"Jonas\"",     s2.vardas    == "Jonas"    );
        spausdintiRezultata( "s2.n == 3",                  s2.n         == 3          );
        spausdintiRezultata( "s2.egzaminas == 10",         s2.egzaminas == 10         );
        spausdintiRezultata( "s2.nd == {8,9,7}",           s2.nd        == std::vector<int>{ 8, 9, 7 } );
        spausdintiRezultata( "s1.n == 0 (saltinis istusejo)", s1.n         == 0 );
        spausdintiRezultata( "s1.egzaminas == 0",             s1.egzaminas == 0 );

        s2 = std::move( s2 );
        spausdintiRezultata( "saviperkėlimas saugus", s2.n == 3 );
    }

    std::cout << "\n6. operator<< (isvedimas):\n";
    {
        Studentas s = sukurtiTestoStudenta();
        std::ostringstream oss;
        oss << s;
        std::string rezultatas = oss.str();
        std::string laukiamas  = "Jonas Jonaitis 3 8 9 7 10";
        spausdintiRezultata( "operator<< ekrane: \"" + rezultatas + "\"",
                             rezultatas == laukiamas );

        std::ofstream failas( "test_studentas.txt" );
        failas << s;
        failas.close();
        spausdintiRezultata( "operator<< iraše i faila", true );
    }

    std::cout << "\n7. operator>> (ivedimas):\n";
    {
        std::istringstream iss( "Ona Kazlauskiene 2 6 8 9" );
        Studentas s;
        iss >> s;
        spausdintiRezultata( "vardas == \"Ona\"",          s.vardas    == "Ona"          );
        spausdintiRezultata( "pavarde == \"Kazlauskiene\"", s.pavarde  == "Kazlauskiene" );
        spausdintiRezultata( "n == 2",                     s.n         == 2              );
        spausdintiRezultata( "nd == {6,8}",                s.nd        == std::vector<int>{ 6, 8 } );
        spausdintiRezultata( "egzaminas == 9",             s.egzaminas == 9              );

        std::ifstream failas( "test_studentas.txt" );
        Studentas sIsFailo;
        failas >> sIsFailo;
        failas.close();
        spausdintiRezultata( "operator>> is failo: vardas == \"Jonas\"",
                             sIsFailo.vardas == "Jonas" );
        spausdintiRezultata( "operator>> is failo: n == 3",
                             sIsFailo.n == 3 );
    }

    std::cout << "\n8. Destruktorius:\n";
    {
        {
            Studentas s = sukurtiTestoStudenta();
        }
        spausdintiRezultata( "destruktorius iskviestas be klaidu (scope pabaiga)", true );
    }

    std::cout << "\n=========================================\n";
    std::cout << "Rezultatai: " << g_ok << " OK, " << g_fail << " FAIL\n";
    if ( g_fail == 0 )
        std::cout << "Visi testai praejo sekmingai!\n\n";
    else
        std::cout << "Kai kurie testai nepraejo!\n\n";

    g_ok = 0;
    g_fail = 0;
}

void generuotiFaila( const std::string& failoVardas, int irasu_sk, int nd_kiekis )
{
    std::ofstream out( failoVardas );
    if ( !out.is_open( ) )
        throw FailoKlaida( "Nepavyko sukurti failo: " + failoVardas );

    out << std::left
        << std::setw( 25 ) << "Vardas"
        << std::setw( 27 ) << "Pavarde";

    std::stringstream a2;

    for ( int i = 1; i <= nd_kiekis; i++ )
        a2 << std::setw( 10 ) << ( "ND" + std::to_string( i ) );

    out << a2.str( );

    out << std::setw( 10 ) << "Egz." << "\n";

    for ( int i = 1; i <= irasu_sk; i++ )
    {
        std::stringstream a3;

        a3 << std::left
            << std::setw( 25 ) << ( "Vardas" + std::to_string( i ) )
            << std::setw( 27 ) << ( "Pavarde" + std::to_string( i ) )
            << std::right;

        for ( int j = 0; j < nd_kiekis; j++ )
            a3 << std::setw( 10 ) << ( rand( ) % 10 + 1 );

        a3 << std::setw( 10 ) << ( rand( ) % 10 + 1 ) << "\n";

        out << a3.str( );
    }

    out.close( );
}

std::vector<Studentas> nuskaitytiIsFailo( const std::string& failoVardas )
{
    return nuskaitytiIsFailoT<std::vector<Studentas>>( failoVardas );
}

void isvestiKategorijaIFaila( const std::string& failoVardas,
    const std::vector<Studentas>& studentai, bool mediana )
{
    std::ofstream out( failoVardas );
    if ( !out.is_open( ) )
        throw FailoKlaida( "Nepavyko sukurti failo: " + failoVardas );

    out << std::left
        << std::setw( 20 ) << "Pavarde"
        << std::setw( 20 ) << "Vardas"
        << std::setw( 20 ) << "Galutinis" << "\n";
    out << std::string( 60, '-' ) << "\n";

    for ( const auto& s : studentai )
    {
        double galutinis;

        if ( mediana )
            galutinis = skaiciuotiGalutini( skaiciuotiMediana( s.nd, s.n ), s.egzaminas );
        else
            galutinis = skaiciuotiGalutini( skaiciuotiVidurki( s.nd, s.n ), s.egzaminas );

        std::stringstream ss;

        ss << std::left
            << std::setw( 20 ) << s.pavarde
            << std::setw( 20 ) << s.vardas
            << std::fixed << std::setprecision( 2 )
            << std::setw( 20 ) << galutinis << "\n";

        out << ss.str( );
    }

    out.close( );
}

void tyrimas1_failuKurimas( )
{
    const int dydziai[] = { 1000, 10000, 100000, 1000000, 10000000 };
    const std::string pavadinimai[] = {
        "studentai_1000.txt",
        "studentai_10000.txt",
        "studentai_100000.txt",
        "studentai_1000000.txt",
        "studentai_10000000.txt"
    };
    const int nd_kiekis = 15;
    const int bandymu_sk = 3;

    std::cout << "\n================ 1 TYRIMAS: Failu kurimas ================\n\n";

    std::cout << std::left
        << std::setw( 18 ) << "Irasu sk."
        << std::setw( 18 ) << "1 bandymas (s)"
        << std::setw( 18 ) << "2 bandymas (s)"
        << std::setw( 18 ) << "3 bandymas (s)"
        << std::setw( 18 ) << "Vidurkis (s)" << "\n";
    std::cout << std::string( 90, '-' ) << "\n";

    for ( int i = 0; i < 5; i++ )
    {
        double laikai[bandymu_sk] = { };
        double suma = 0.0;

        for ( int b = 0; b < bandymu_sk; b++ )
        {
            std::remove( pavadinimai[i].c_str( ) );

            auto start = std::chrono::high_resolution_clock::now( );
            generuotiFaila( pavadinimai[i], dydziai[i], nd_kiekis );
            auto end = std::chrono::high_resolution_clock::now( );

            laikai[b] = std::chrono::duration<double>( end - start ).count( );
            suma += laikai[b];
        }

        std::cout << std::left << std::setw( 18 ) << dydziai[i]
            << std::fixed << std::setprecision( 5 )
            << std::setw( 18 ) << laikai[0]
            << std::setw( 18 ) << laikai[1]
            << std::setw( 18 ) << laikai[2]
            << std::setw( 18 ) << ( suma / bandymu_sk ) << "\n";
    }

    std::cout << "\nFailai sugeneruoti ir issaugoti.\n";
}

void tyrimasKonteineriu( bool mediana )
{
    const int dydziai[] = { 1000, 10000, 100000, 1000000, 10000000 };
    const std::string pavadinimai[] = {
        "studentai_1000.txt",
        "studentai_10000.txt",
        "studentai_100000.txt",
        "studentai_1000000.txt",
        "studentai_10000000.txt"
    };
    const int bandymu_sk = 3;

    std::cout << "\n================ 2 TYRIMAS: Konteineriu palyginimas ================\n";
    std::cout << "Kiekvienas matavimas atliktas " << bandymu_sk
              << " kartus, pateikiamas vidurkis.\n\n";

    std::cout << std::left
        << std::setw( 12 ) << "Irasu sk."
        << std::setw( 14 ) << "Konteineris"
        << std::setw( 18 ) << "Nuskaitymas(s)"
        << std::setw( 18 ) << "Rusiavimas(s)"
        << std::setw( 18 ) << "Skaidymas(s)"
        << "\n";
    std::cout << std::string( 80, '-' ) << "\n";

    for ( int i = 0; i < 5; i++ )
    {
        {
            std::ifstream test( pavadinimai[i] );
            if ( !test.is_open( ) )
            {
                std::cout << std::left << std::setw( 12 ) << dydziai[i]
                    << "Failas nerastas! Pirma paleiskite 1 tyrima.\n";
                continue;
            }
        }

        double nusk, rus, skaid;

        benchmarkKonteineris<std::vector<Studentas>>(
            pavadinimai[i], mediana, bandymu_sk, nusk, rus, skaid );
        std::cout << std::left << std::setw( 12 ) << dydziai[i]
            << std::setw( 14 ) << "vector"
            << std::fixed << std::setprecision( 5 )
            << std::setw( 18 ) << nusk
            << std::setw( 18 ) << rus
            << std::setw( 18 ) << skaid << "\n";

        benchmarkKonteineris<std::list<Studentas>>(
            pavadinimai[i], mediana, bandymu_sk, nusk, rus, skaid );
        std::cout << std::left << std::setw( 12 ) << ""
            << std::setw( 14 ) << "list"
            << std::fixed << std::setprecision( 5 )
            << std::setw( 18 ) << nusk
            << std::setw( 18 ) << rus
            << std::setw( 18 ) << skaid << "\n";

        benchmarkKonteineris<std::deque<Studentas>>(
            pavadinimai[i], mediana, bandymu_sk, nusk, rus, skaid );
        std::cout << std::left << std::setw( 12 ) << ""
            << std::setw( 14 ) << "deque"
            << std::fixed << std::setprecision( 5 )
            << std::setw( 18 ) << nusk
            << std::setw( 18 ) << rus
            << std::setw( 18 ) << skaid << "\n";

        std::cout << std::string( 80, '-' ) << "\n";
    }

    std::cout << "\nTyrimas baigtas.\n";
}

void tyrimasStrategiju( bool mediana )
{
    const int dydziai[] = { 1000, 10000, 100000, 1000000, 10000000 };
    const std::string pavadinimai[] = {
        "studentai_1000.txt",
        "studentai_10000.txt",
        "studentai_100000.txt",
        "studentai_1000000.txt",
        //"studentai_10000000.txt"
    };

    const int bandymu_sk = 1;

    std::cout << "\n================ 3 TYRIMAS: Strategiju palyginimas ================\n";
    std::cout << "Kiekvienas matavimas atliktas " << bandymu_sk
              << " kartus, pateikiamas vidurkis.\n";
    std::cout << "Matuojamas TIK skaidymo i grupes laikas (be nuskaitymo ir rusiavimo).\n";

    const char* konteineriai[] = { "std::vector", "std::list", "std::deque" };

    for ( int k = 0; k < 3; k++ )
    {
        std::cout << "\n--- " << konteineriai[k] << " ---\n\n";
        std::cout << std::left
            << std::setw( 15 ) << "Irasu sk."
            << std::setw( 20 ) << "1 strategija(s)"
            << std::setw( 20 ) << "2 strategija(s)"
            << std::setw( 20 ) << "3 strategija(s)"
            << "\n";
        std::cout << std::string( 75, '-' ) << "\n";

        for ( int i = 0; i < sizeof( pavadinimai) / sizeof( pavadinimai[0]); i++ )
        {
            {
                std::ifstream test( pavadinimai[i] );
                if ( !test.is_open( ) )
                {
                    std::cout << std::left << std::setw( 15 ) << dydziai[i]
                        << "Failas nerastas!\n";
                    continue;
                }
            }

            double s1, s2, s3;

            if ( k == 0 )
                benchmarkStrategijos<std::vector<Studentas>>(
                    pavadinimai[i], mediana, bandymu_sk, s1, s2, s3 );
            else if ( k == 1 )
                benchmarkStrategijos<std::list<Studentas>>(
                    pavadinimai[i], mediana, bandymu_sk, s1, s2, s3 );
            else
                benchmarkStrategijos<std::deque<Studentas>>(
                    pavadinimai[i], mediana, bandymu_sk, s1, s2, s3 );

            std::cout << std::left << std::setw( 15 ) << dydziai[i]
                << std::fixed << std::setprecision( 5 )
                << std::setw( 20 ) << s1
                << std::setw( 20 ) << s2
                << std::setw( 20 ) << s3 << "\n";
        }
    }

    std::cout << "\nTyrimas baigtas.\n";

    std::cout << "\n--- Kietiakiai/vargsiukai issaugojimas i failus ---\n";

    bool rastasVienas = false;
    for ( int i = 0; i < 5; i++ )
    {
        std::ifstream test( pavadinimai[i] );
        if ( !test.is_open( ) )
            continue;
        test.close( );

        rastasVienas = true;
        std::string sk = std::to_string( dydziai[i] );

        std::vector<Studentas> base = nuskaitytiIsFailoT<std::vector<Studentas>>( pavadinimai[i] );
        rusiuotiPagalGalutini( base, mediana );

        std::vector<Studentas> data = base;
        std::vector<Studentas> kiet, varg;
        strategija1( data, kiet, varg, mediana );

        isvestiKategorijaIFaila( "kietiakiai_" + sk + ".txt", kiet, mediana );
        isvestiKategorijaIFaila( "vargsiukai_"  + sk + ".txt", varg, mediana );

        std::cout << "  " << std::setw( 10 ) << std::left << sk << " irasu  ->  "
                  << "kietiakiai_" << sk << ".txt (" << kiet.size( ) << "),  "
                  << "vargsiukai_"  << sk << ".txt (" << varg.size( ) << ")\n";
    }

    if ( !rastasVienas )
        std::cout << "  Nera failu - pirma paleiskite 1 tyrima.\n";
}

// =================================================================
// v3.0 tyrimai: std::vector vs Vector
// =================================================================

void tyrimasVectorPushBack( )
{
    const size_t dydziai[] = { 10000, 100000, 1000000, 10000000, 100000000 };
    const int n = sizeof( dydziai ) / sizeof( dydziai[0] );

    std::cout << "\n================ V3.0 TYRIMAS: push_back greitis (std::vector vs Vector) ================\n";
    std::cout << "Matuojama tuscio konteinerio uzpildymas push_back metodu.\n\n";

    std::cout << std::left
        << std::setw( 15 ) << "Elementu sk."
        << std::setw( 20 ) << "std::vector (s)"
        << std::setw( 20 ) << "Vector (s)"
        << std::setw( 15 ) << "Lyginimas"
        << "\n";
    std::cout << std::string( 70, '-' ) << "\n";

    for ( int i = 0; i < n; i++ )
    {
        size_t sz = dydziai[i];

        double t_std = 0.0;
        {
            auto t1 = std::chrono::high_resolution_clock::now( );
            std::vector<int> v1;
            for ( size_t k = 1; k <= sz; k++ )
                v1.push_back( static_cast<int>( k ) );
            auto t2 = std::chrono::high_resolution_clock::now( );
            t_std = std::chrono::duration<double>( t2 - t1 ).count( );
        }

        double t_my = 0.0;
        {
            auto t1 = std::chrono::high_resolution_clock::now( );
            Vector<int> v2;
            for ( size_t k = 1; k <= sz; k++ )
                v2.push_back( static_cast<int>( k ) );
            auto t2 = std::chrono::high_resolution_clock::now( );
            t_my = std::chrono::duration<double>( t2 - t1 ).count( );
        }

        double rel = ( t_std > 0.0 ) ? ( t_my / t_std ) : 0.0;

        std::cout << std::left << std::setw( 15 ) << sz
            << std::fixed << std::setprecision( 5 )
            << std::setw( 20 ) << t_std
            << std::setw( 20 ) << t_my
            << std::setprecision( 2 ) << "x" << rel << "\n";
    }

    std::cout << "\nTyrimas baigtas.\n";
}

void tyrimasVectorReallocations( )
{
    const size_t SZ = 100000000;

    std::cout << "\n================ V3.0 TYRIMAS: Atminties perskirstymai (push_back iki 100M) ================\n\n";

    std::cout << "Pildomas Vector iki " << SZ << " elementu, skaiciuojami perskirstymai...\n";

    Vector<int> v2;
    for ( size_t k = 1; k <= SZ; k++ )
        v2.push_back( static_cast<int>( k ) );

    std::cout << "\nVector rezultatai:\n";
    std::cout << "  Galutinis size:     " << v2.size( ) << "\n";
    std::cout << "  Galutinis capacity: " << v2.capacity( ) << "\n";
    std::cout << "  Perskirstymu sk.:   " << v2.realloc_count( ) << "\n";

    std::cout << "\nstd::vector turi panasu elgesi (capacity dvigubinasi).\n";
    std::cout << "Standartas neapibrezia tikslaus augimo koef., bet pas MSVC tai apie 1.5x,\n";
    std::cout << "todel std::vector reallocation skaicius bus 1-2 kartus didesnis nei Vector x2 augimo.\n";
}

void tyrimasVectorStudentai( bool mediana )
{
    const int dydziai[] = { 100000, 1000000, 10000000 };
    const std::string pavadinimai[] = {
        "studentai_100000.txt",
        "studentai_1000000.txt",
        "studentai_10000000.txt"
    };
    const int bandymu_sk = 3;

    std::cout << "\n================ V3.0 TYRIMAS: Studentu programa std::vector vs Vector ================\n";
    std::cout << "Matuojamas pilnas darbas: nuskaitymas + rusiavimas + skaidymas (1 strategija).\n\n";

    std::cout << std::left
        << std::setw( 14 ) << "Irasu sk."
        << std::setw( 20 ) << "std::vector (s)"
        << std::setw( 20 ) << "Vector (s)"
        << "\n";
    std::cout << std::string( 60, '-' ) << "\n";

    for ( int i = 0; i < 3; i++ )
    {
        std::ifstream test( pavadinimai[i] );
        if ( !test.is_open( ) )
        {
            std::cout << std::left << std::setw( 14 ) << dydziai[i]
                << "Failas nerastas! Pirma paleiskite 1 tyrima.\n";
            continue;
        }
        test.close( );

        double suma_std = 0.0;
        double suma_my  = 0.0;

        for ( int b = 0; b < bandymu_sk; b++ )
        {
            auto t1 = std::chrono::high_resolution_clock::now( );
            std::vector<Studentas> data1 = nuskaitytiIsFailoT<std::vector<Studentas>>( pavadinimai[i] );
            rusiuotiPagalGalutini( data1, mediana );
            std::vector<Studentas> kiet1, varg1;
            strategija1( data1, kiet1, varg1, mediana );
            auto t2 = std::chrono::high_resolution_clock::now( );
            suma_std += std::chrono::duration<double>( t2 - t1 ).count( );
        }

        for ( int b = 0; b < bandymu_sk; b++ )
        {
            auto t1 = std::chrono::high_resolution_clock::now( );
            std::ifstream stream( pavadinimai[i] );
            std::string line;
            std::getline( stream, line );
            std::stringstream header( line );
            std::vector<std::string> cols;
            std::string col;
            while ( header >> col ) cols.push_back( col );
            size_t nd_count = cols.size( ) >= 3 ? cols.size( ) - 3 : 0;

            Vector<Studentas> data2;
            while ( std::getline( stream, line ) )
            {
                if ( line.empty( ) ) continue;
                std::stringstream ss( line );
                Studentas s;
                s.n = static_cast<int>( nd_count );
                s.nd.resize( nd_count );
                ss >> s.vardas >> s.pavarde;
                for ( size_t j = 0; j < nd_count; j++ ) ss >> s.nd[j];
                ss >> s.egzaminas;
                data2.push_back( s );
            }
            stream.close( );

            std::sort( data2.begin( ), data2.end( ), [mediana]( const Studentas& a, const Studentas& b ) {
                return apskaiciuotiGalutiniBala( a, mediana ) > apskaiciuotiGalutiniBala( b, mediana );
            } );

            Vector<Studentas> kiet2, varg2;
            for ( size_t k = 0; k < data2.size( ); k++ )
            {
                if ( apskaiciuotiGalutiniBala( data2[k], mediana ) >= 5.0 )
                    kiet2.push_back( data2[k] );
                else
                    varg2.push_back( data2[k] );
            }

            auto t2 = std::chrono::high_resolution_clock::now( );
            suma_my += std::chrono::duration<double>( t2 - t1 ).count( );
        }

        std::cout << std::left << std::setw( 14 ) << dydziai[i]
            << std::fixed << std::setprecision( 5 )
            << std::setw( 20 ) << ( suma_std / bandymu_sk )
            << std::setw( 20 ) << ( suma_my / bandymu_sk )
            << "\n";
    }

    std::cout << "\nTyrimas baigtas.\n";
}
