#include "studentas.h"
#include <utility>

Studentas::Studentas()
    : vardas( "" ), pavarde( "" ), nd( ), n( 0 ), egzaminas( 0 )
{
}

Studentas::~Studentas()
{
}

Studentas::Studentas( const Studentas& kitas )
    : vardas( kitas.vardas ),
      pavarde( kitas.pavarde ),
      nd( kitas.nd ),
      n( kitas.n ),
      egzaminas( kitas.egzaminas )
{
}

Studentas& Studentas::operator=( const Studentas& kitas )
{
    if ( this == &kitas )
        return *this;

    vardas    = kitas.vardas;
    pavarde   = kitas.pavarde;
    nd        = kitas.nd;
    n         = kitas.n;
    egzaminas = kitas.egzaminas;

    return *this;
}

Studentas::Studentas( Studentas&& kitas ) noexcept
    : vardas( std::move( kitas.vardas ) ),
      pavarde( std::move( kitas.pavarde ) ),
      nd( std::move( kitas.nd ) ),
      n( kitas.n ),
      egzaminas( kitas.egzaminas )
{
    kitas.n         = 0;
    kitas.egzaminas = 0;
}

Studentas& Studentas::operator=( Studentas&& kitas ) noexcept
{
    if ( this == &kitas )
        return *this;

    vardas    = std::move( kitas.vardas );
    pavarde   = std::move( kitas.pavarde );
    nd        = std::move( kitas.nd );
    n         = kitas.n;
    egzaminas = kitas.egzaminas;

    kitas.n         = 0;
    kitas.egzaminas = 0;

    return *this;
}

// Formatas: vardas pavarde n nd[0] nd[1] ... nd[n-1] egzaminas
std::ostream& operator<<( std::ostream& out, const Studentas& s )
{
    out << s.vardas << " " << s.pavarde << " " << s.n;

    for ( int i = 0; i < s.n; i++ )
        out << " " << s.nd[ i ];

    out << " " << s.egzaminas;

    return out;
}

std::istream& operator>>( std::istream& in, Studentas& s )
{
    if ( !( in >> s.vardas >> s.pavarde >> s.n ) )
        return in;

    s.nd.resize( s.n );

    for ( int i = 0; i < s.n; i++ )
    {
        if ( !( in >> s.nd[ i ] ) )
            return in;
    }

    in >> s.egzaminas;

    return in;
}
