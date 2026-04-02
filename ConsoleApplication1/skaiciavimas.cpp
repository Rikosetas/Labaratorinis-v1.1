#include "skaiciavimas.h"
#include <algorithm>

double skaiciuotiVidurki( const std::vector<int>& nd, int n )
{
    if ( n == 0 )
        return 0.0;

    double suma = 0;
    for ( int i = 0; i < n; i++ )
        suma += nd[i];

    return suma / n;
}

double skaiciuotiMediana( const std::vector<int>& nd, int n )
{
    if ( n == 0 )
        return 0.0;

    std::vector<int> copy = nd;
    std::sort( copy.begin( ), copy.end( ) );

    double rezultatas;

    if ( n % 2 == 0 )
        rezultatas = ( copy[n / 2 - 1] + copy[n / 2] ) / 2.0;
    else
        rezultatas = copy[n / 2];

    return rezultatas;
}

double skaiciuotiGalutini( double nd_rezultatas, int egz )
{
    return 0.4 * nd_rezultatas + 0.6 * egz;
}
