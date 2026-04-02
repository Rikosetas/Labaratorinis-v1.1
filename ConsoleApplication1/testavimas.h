#ifndef TESTAVIMAS_H
#define TESTAVIMAS_H

#include "studentas.h"
#include "skaiciavimas.h"
#include "exceptions.h"

#include <string>
#include <vector>
#include <list>
#include <deque>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <iterator>

void generuotiFaila( const std::string& failoVardas, int irasu_sk, int nd_kiekis );
void tyrimas1_failuKurimas( );
void tyrimasKonteineriu( bool mediana );
void tyrimasStrategiju( bool mediana );

std::vector<Studentas> nuskaitytiIsFailo( const std::string& failoVardas );

void isvestiKategorijaIFaila( const std::string& failoVardas,
    const std::vector<Studentas>& studentai, bool mediana );

template<typename Container>
Container nuskaitytiIsFailoT( const std::string& failoVardas )
{
    std::ifstream stream( failoVardas );
    if ( !stream.is_open( ) )
        throw FailoKlaida( "Nepavyko atidaryti failo: " + failoVardas );

    Container out;
    std::string line;

    std::getline( stream, line );

    while ( std::getline( stream, line ) )
    {
        if ( line.empty( ) )
            continue;

        std::stringstream ss( line );
        out.push_back( Studentas( ss ) );
    }

    return out;
}

inline double apskaiciuotiGalutiniBala( const Studentas& s, bool mediana )
{
    return s.galBalas( mediana );
}

template<typename Container>
void rusiuotiPagalGalutini( Container& c, bool mediana )
{
    auto comp = [mediana]( const Studentas& a, const Studentas& b ) {
        return a.galBalas( mediana ) < b.galBalas( mediana );
    };

    if constexpr ( std::is_same_v<Container, std::list<Studentas>> )
        c.sort( comp );
    else
        std::sort( c.begin( ), c.end( ), comp );
}

template<typename Container>
void strategija1( const Container& studentai, Container& kietiakiai,
    Container& vargsiukai, bool mediana )
{
    for ( const auto& s : studentai )
    {
        if ( s.galBalas( mediana ) >= 5.0 )
            kietiakiai.push_back( s );
        else
            vargsiukai.push_back( s );
    }
}

template<typename Container>
void strategija2( Container& studentai, Container& vargsiukai, bool mediana )
{
    auto it = studentai.begin( );
    while ( it != studentai.end( ) )
    {
        if ( it->galBalas( mediana ) < 5.0 )
        {
            vargsiukai.push_back( std::move( *it ) );
            it = studentai.erase( it );
        }
        else
        {
            ++it;
        }
    }
}

template<typename Container>
void strategija3( Container& studentai, Container& vargsiukai, bool mediana )
{
    auto it = std::stable_partition( studentai.begin( ), studentai.end( ),
        [mediana]( const Studentas& s ) {
            return s.galBalas( mediana ) >= 5.0;
        } );

    if constexpr ( std::is_same_v<Container, std::list<Studentas>> )
    {
        vargsiukai.splice( vargsiukai.end( ), studentai, it, studentai.end( ) );
    }
    else
    {
        vargsiukai.insert( vargsiukai.end( ),
            std::make_move_iterator( it ),
            std::make_move_iterator( studentai.end( ) ) );
        studentai.erase( it, studentai.end( ) );
    }
}

template<typename Container>
void benchmarkKonteineris( const std::string& failas, bool mediana, int bandymu_sk,
    double& nusk_avg, double& rus_avg, double& skaid_avg )
{
    double nusk_suma = 0, rus_suma = 0, skaid_suma = 0;

    for ( int b = 0; b < bandymu_sk; b++ )
    {
        auto t1 = std::chrono::high_resolution_clock::now( );
        Container data = nuskaitytiIsFailoT<Container>( failas );
        auto t2 = std::chrono::high_resolution_clock::now( );

        rusiuotiPagalGalutini( data, mediana );
        auto t3 = std::chrono::high_resolution_clock::now( );

        Container kiet, varg;
        strategija1( data, kiet, varg, mediana );
        auto t4 = std::chrono::high_resolution_clock::now( );

        nusk_suma += std::chrono::duration<double>( t2 - t1 ).count( );
        rus_suma += std::chrono::duration<double>( t3 - t2 ).count( );
        skaid_suma += std::chrono::duration<double>( t4 - t3 ).count( );
    }

    nusk_avg = nusk_suma / bandymu_sk;
    rus_avg = rus_suma / bandymu_sk;
    skaid_avg = skaid_suma / bandymu_sk;
}

template<typename Container>
void benchmarkStrategijos( const std::string& failas, bool mediana, int bandymu_sk,
    double& str1_avg, double& str2_avg, double& str3_avg )
{
    Container original = nuskaitytiIsFailoT<Container>( failas );
    rusiuotiPagalGalutini( original, mediana );

    double suma1 = 0, suma2 = 0, suma3 = 0;

    for ( int b = 0; b < bandymu_sk; b++ )
    {
        {
            Container data = original;
            Container kiet, varg;
            auto t1 = std::chrono::high_resolution_clock::now( );
            strategija1( data, kiet, varg, mediana );
            auto t2 = std::chrono::high_resolution_clock::now( );
            suma1 += std::chrono::duration<double>( t2 - t1 ).count( );
        }
        {
            Container data = original;
            Container varg;
            auto t1 = std::chrono::high_resolution_clock::now( );
            strategija2( data, varg, mediana );
            auto t2 = std::chrono::high_resolution_clock::now( );
            suma2 += std::chrono::duration<double>( t2 - t1 ).count( );
        }
        {
            Container data = original;
            Container varg;
            auto t1 = std::chrono::high_resolution_clock::now( );
            strategija3( data, varg, mediana );
            auto t2 = std::chrono::high_resolution_clock::now( );
            suma3 += std::chrono::duration<double>( t2 - t1 ).count( );
        }
    }

    str1_avg = suma1 / bandymu_sk;
    str2_avg = suma2 / bandymu_sk;
    str3_avg = suma3 / bandymu_sk;
}

#endif
