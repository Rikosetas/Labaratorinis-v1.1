#include "io.h"
#include "studentas_utils.h"
#include "skaiciavimas.h"
#include "exceptions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <chrono>

bool skaitytiSveika( int& reiksme, int min_val, int max_val )
{
    if ( !( std::cin >> reiksme ) )
    {
        std::cin.clear( );
        std::cin.ignore( std::numeric_limits<std::streamsize>::max( ), '\n' );
        return false;
    }

    if ( reiksme < min_val || reiksme > max_val )
        return false;

    return true;
}

template <typename Stream>
void spausdintiIStream( Stream& out, const std::vector<Studentas>& studentai, bool mediana )
{
    out << "\n" << std::string( 70, '-' ) << "\n";

    out << std::left << std::setw( 15 ) << "Pavarde"
        << std::setw( 15 ) << "Vardas"
        << std::setw( 20 ) << "Galutinis (Vid.)";

    if ( mediana ) out << std::setw( 20 ) << "Galutinis (Med.)";

    out << "\n" << std::string( 70, '-' ) << "\n";

    for ( const auto& s : studentai )
    {
        double vid = s.galBalas( false );

        out << std::left << std::setw( 15 ) << s.pavarde( )
            << std::setw( 15 ) << s.vardas( )
            << std::fixed << std::setprecision( 2 )
            << std::setw( 20 ) << vid;

        if ( mediana )
        {
            double med = s.galBalas( true );
            out << std::setw( 20 ) << med;
        }

        out << "\n";
    }

    out << std::string( 70, '-' ) << "\n";
}

void spausdintiRezultatus( std::vector<Studentas>& studentai, int m, bool mediana )
{
    int rusiavimas = pasirinktiRusiavima( );
    rusiuotiStudentus( studentai, rusiavimas );

    int outputPasirinkimas;
    std::cout << "\nIsvedimo vieta:\n  1 - Ekranas\n  2 - Failas\nPasirinkimas: ";

    while ( !skaitytiSveika( outputPasirinkimas, 1, 2 ) ) {
        std::cout << "Neteisinga reiksme. Pasirinkite 1 arba 2: ";
    }

    if ( outputPasirinkimas == 1 )
    {
        spausdintiIStream( std::cout, studentai, mediana );
    }
    else
    {
        std::ofstream stream( "rezultatai.txt" );
        if ( !stream.is_open( ) )
            throw FailoKlaida( "Nepavyko atidaryti failo rezultatai.txt rasymui." );

        spausdintiIStream( stream, studentai, mediana );
        std::cout << "Rezultatai issaugoti faile: rezultatai.txt\n";
    }
}

std::vector<Studentas> ivestiRankiniu( int& m, int& n )
{
    std::cout << "Kiek namu darbu? ";

    while ( !skaitytiSveika( n, 1, 100 ) ) {
        std::cout << "Neteisinga reiksme. Iveskite 1-100: ";
    }

    m = 0;
    std::vector<Studentas> studentai;

    while ( true )
    {
        Studentas studentas;

        std::cout << "\n--- Studentas #" << ( m + 1 ) << " (arba iveskite 'baigti') ---\n";
        std::cout << "Vardas: ";

        std::string v;
        std::cin >> v;

        if ( v == "baigti" )
            break;

        studentas.setVardas( v );

        std::cout << "Pavarde: ";
        std::string p;
        std::cin >> p;
        studentas.setPavarde( p );

        studentas.setN( n );
        std::vector<int> nd( n );

        for ( int j = 0; j < n; j++ )
        {
            int val;
            std::cout << "  ND" << ( j + 1 ) << " balas (1-10): ";
            while ( !skaitytiSveika( val, 1, 10 ) ) {
                std::cout << "  Neteisinga reiksme (1-10): ";
            }
            nd[j] = val;
        }

        studentas.setNd( nd );

        int egz;
        std::cout << "Egzamino balas (1-10): ";
        while ( !skaitytiSveika( egz, 1, 10 ) ) {
            std::cout << "  Neteisinga reiksme (1-10): ";
        }
        studentas.setEgzaminas( egz );

        studentai.push_back( studentas );
        m++;
    }

    return studentai;
}

std::vector<Studentas> nuskaitytiStudentus( )
{
    std::ifstream stream( "kursiokai.txt" );
    if ( !stream.is_open( ) )
        throw FailoKlaida( "Nepavyko atidaryti failo kursiokai.txt." );

    auto start = std::chrono::high_resolution_clock::now( );

    std::vector<Studentas> out;

    std::string line;
    if ( !std::getline( stream, line ) )
        return out;

    while ( std::getline( stream, line ) )
    {
        if ( line.empty( ) )
            continue;

        std::stringstream ss( line );
        out.push_back( Studentas( ss ) );
    }

    auto end = std::chrono::high_resolution_clock::now( );
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( end - start );
    std::cout << "Nuskaityti " << out.size( ) << " studentu per " << std::fixed << std::setprecision( 3 ) << elapsed.count( ) << " ms\n";

    return out;
}
