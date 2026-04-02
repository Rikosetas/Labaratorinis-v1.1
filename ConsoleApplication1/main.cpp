#include "io.h"
#include "studentas_utils.h"
#include "exceptions.h"
#include "testavimas.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main( )
{
    std::srand( static_cast<unsigned>( std::time( nullptr ) ) );

    int skaiciavimas = { };
    std::cout << "Galutinio balo skaiciavimas:\n  1 - Vidurkis\n  2 - Mediana\nPasirinkimas: ";

    while ( !skaitytiSveika( skaiciavimas, 1, 2 ) ) {
        std::cout << "Neteisinga reiksme. Pasirinkite 1 arba 2: ";
    }

    bool mediana = ( skaiciavimas == 2 );

    int m { };
    int n { };
    int meniu;

    do {
        std::cout << "\n========== MENIU ==========\n";
        std::cout << "  1 - Ivesti duomenis rankiniu budu\n";
        std::cout << "  2 - Generuoti tik pazymius\n";
        std::cout << "  3 - Generuoti vardus, pavardes ir pazymius\n";
        std::cout << "  4 - Nuskaityti studentus is failo\n";
        std::cout << "  5 - 1 tyrimas: Failu generavimas\n";
        std::cout << "  6 - 2 tyrimas: Konteineriu palyginimas (vector, list, deque)\n";
        std::cout << "  7 - 3 tyrimas: Strategiju palyginimas (1, 2, 3 strategijos)\n";
        std::cout << "  8 - Baigti darba\n";
        std::cout << "Pasirinkimas: ";

        if ( !skaitytiSveika( meniu, 1, 8 ) )
        {
            std::cout << "Neteisinga reiksme.\n";
            continue;
        }

        try
        {
            switch ( meniu )
            {
            case 1:
            {
                std::vector<Studentas> studentai = ivestiRankiniu( m, n );

                if ( m > 0 )
                    spausdintiRezultatus( studentai, m, mediana );
                else
                    std::cout << "Nera studentu duomenu.\n";
                break;
            }

            case 2:
            {
                std::cout << "Studentu skaicius: ";
                while ( !skaitytiSveika( m, 1, 10000 ) ) {
                    std::cout << "Neteisinga reiksme: ";
                }

                std::cout << "Namu darbu skaicius: ";
                while ( !skaitytiSveika( n, 1, 100 ) ) {
                    std::cout << "Neteisinga reiksme: ";
                }

                std::vector<Studentas> studentai( m );

                for ( int i = 0; i < m; i++ )
                {
                    std::string v, p;
                    std::cout << "  Studentas #" << ( i + 1 ) << " vardas: ";
                    std::cin >> v;
                    studentai[i].setVardas( v );

                    std::cout << "  Studentas #" << ( i + 1 ) << " pavarde: ";
                    std::cin >> p;
                    studentai[i].setPavarde( p );

                    generuotiPazymius( studentai[i], n );
                }

                spausdintiRezultatus( studentai, m, mediana );
                break;
            }

            case 3:
            {
                std::cout << "Studentu skaicius: ";
                while ( !skaitytiSveika( m, 1, 10000 ) ) {
                    std::cout << "Neteisinga reiksme: ";
                }

                std::cout << "Namu darbu skaicius: ";
                while ( !skaitytiSveika( n, 1, 100 ) ) {
                    std::cout << "Neteisinga reiksme: ";
                }

                std::vector<Studentas> studentai( m );

                for ( int i = 0; i < m; i++ )
                {
                    generuotiVarda( studentai[i], i );
                    generuotiPazymius( studentai[i], n );
                }

                spausdintiRezultatus( studentai, m, mediana );
                break;
            }

            case 4:
            {
                std::vector<Studentas> studentai = nuskaitytiStudentus( );

                if ( studentai.size( ) > 0 )
                    spausdintiRezultatus( studentai, static_cast<int>( studentai.size( ) ), mediana );
                else
                    std::cout << "Nera studentu duomenu.\n";

                break;
            }

            case 5:
            {
                tyrimas1_failuKurimas( );
                break;
            }

            case 6:
            {
                tyrimasKonteineriu( mediana );
                break;
            }

            case 7:
            {
                tyrimasStrategiju( mediana );
                break;
            }

            case 8:
            {
                std::cout << "Programa baigta.\n";
                break;
            }
            }
        }
        catch ( const FailoKlaida& e )
        {
            std::cerr << "Failo klaida: " << e.what( ) << "\n";
        }
        catch ( const DuomenuKlaida& e )
        {
            std::cerr << "Duomenu klaida: " << e.what( ) << "\n";
        }
        catch ( const std::exception& e )
        {
            std::cerr << "Klaida: " << e.what( ) << "\n";
        }

    } while ( meniu != 8 );

    return 0;
}
