#include "testavimas.h"
#include "skaiciavimas.h"
#include "studentas_utils.h"
#include "exceptions.h"
#include "io.h"

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

static void spausdintiRezultata( const std::string& testas, bool rezultatas )
{
    std::cout << "  " << ( rezultatas ? "[OK]  " : "[FAIL]" ) << " " << testas << "\n";
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

    // --- 1. Default konstruktorius ---
    std::cout << "1. Default konstruktorius:\n";
    {
        Studentas s;
        spausdintiRezultata( "vardas == \"\"",    s.vardas    == "" );
        spausdintiRezultata( "pavarde == \"\"",   s.pavarde   == "" );
        spausdintiRezultata( "n == 0",            s.n         == 0  );
        spausdintiRezultata( "egzaminas == 0",    s.egzaminas == 0  );
        spausdintiRezultata( "nd.empty()",        s.nd.empty()      );
    }

    // --- 2. Kopijavimo konstruktorius ---
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

    // --- 3. Kopijavimo priskyrimo operatorius ---
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

        // savipriskyrimass
        s1 = s1;
        spausdintiRezultata( "savipriskyrimass saugus",    s1.vardas == "Jonas" );
    }

    // --- 4. Perkėlimo konstruktorius ---
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

    // --- 5. Perkėlimo priskyrimo operatorius ---
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

        // saviperkėlimas
        s2 = std::move( s2 );
        spausdintiRezultata( "saviperkėlimas saugus", s2.n == 3 );
    }

    // --- 6. operator<< ---
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

    // --- 7. operator>> ---
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

        // nuskaitymas is failo
        std::ifstream failas( "test_studentas.txt" );
        Studentas sIsFailo;
        failas >> sIsFailo;
        failas.close();
        spausdintiRezultata( "operator>> is failo: vardas == \"Jonas\"",
                             sIsFailo.vardas == "Jonas" );
        spausdintiRezultata( "operator>> is failo: n == 3",
                             sIsFailo.n == 3 );
    }

    // --- 8. Destruktorius ---
    std::cout << "\n8. Destruktorius:\n";
    {
        {
            Studentas s = sukurtiTestoStudenta();
        }
        spausdintiRezultata( "destruktorius iskviestas be klaidu (scope pabaiga)", true );
    }

    std::cout << "\n=========================================\n";
    std::cout << "Testai baigti.\n\n";
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
        "studentai_10000000.txt"
    };
    const int bandymu_sk = 3;

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

        for ( int i = 0; i < 5; i++ )
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
}
