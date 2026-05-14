#include "studentas_utils.h"
#include "skaiciavimas.h"
#include "studentu_io.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

void generuotiPazymius( Studentas& s, int n )
{
    s.n = n;
    s.nd.resize( n );

    for ( int i = 0; i < n; i++ )
        s.nd [ i ] = rand( ) % 10 + 1;

    s.egzaminas = rand( ) % 10 + 1;
}

void generuotiVarda( Studentas& s, int indeksas )
{
    std::string vardai[ ] = { "Jonas", "Petras", "Ona", "Marta", "Lukas", "Egle", "Tomas", "Inga" };
    std::string pavardes[ ] = { "Jonaitis", "Petraitis", "Kazlauskas", "Stankevicious", "Vaitkus", "Lukosius" };


    s.vardas = vardai [ indeksas % 8 ];
    s.pavarde = pavardes [ indeksas % 6 ];
}

int pasirinktiRusiavima( )
{
    int pasirinkimas;


    std::cout << "\nRusiavimo pasirinkimas:\n";
    std::cout << "  1 - Pagal varda\n";
    std::cout << "  2 - Pagal pavarde\n";
    std::cout << "  3 - Pagal galutini (vidurkis)\n";
    std::cout << "  4 - Pagal galutini (mediana)\n";
    std::cout << "Pasirinkimas: ";

    while ( !skaitytiSveika( pasirinkimas, 1, 4 ) ) {
        std::cout << "Neteisinga reiksme. Pasirinkite 1-4: ";
    }

    return pasirinkimas;
}

void rusiuotiStudentus( std::vector<Studentas>& studentai, int rusiavimas )
{
    switch ( rusiavimas ) {
    case 1:
        std::sort( studentai.begin( ), studentai.end( ), [ ] ( const Studentas& a, const Studentas& b ) {
            return a.vardas < b.vardas;
            } );
        break;
    case 2:
        std::sort( studentai.begin( ), studentai.end( ), [ ] ( const Studentas& a, const Studentas& b ) {
            return a.pavarde < b.pavarde;
            } );
        break;
    case 3:
        std::sort( studentai.begin( ), studentai.end( ), [ ] ( const Studentas& a, const Studentas& b ) {
            double ga = skaiciuotiGalutini( skaiciuotiVidurki( a.nd, a.n ), a.egzaminas );
            double gb = skaiciuotiGalutini( skaiciuotiVidurki( b.nd, b.n ), b.egzaminas );
            return ga > gb;
            } );
        break;
    case 4:
        std::sort( studentai.begin( ), studentai.end( ), [ ] ( const Studentas& a, const Studentas& b ) {
            double ga = skaiciuotiGalutini( skaiciuotiMediana( a.nd, a.n ), a.egzaminas );
            double gb = skaiciuotiGalutini( skaiciuotiMediana( b.nd, b.n ), b.egzaminas );
            return ga > gb;
            } );
        break;
    }
}
