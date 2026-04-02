#include "studentas.h"
#include "skaiciavimas.h"

Studentas::Studentas( ) : n_( 0 ), egzaminas_( 0 ) { }

Studentas::Studentas( std::istream& is ) : n_( 0 ), egzaminas_( 0 )
{
    readStudent( is );
}

Studentas::Studentas( const Studentas& other )
    : vardas_( other.vardas_ ), pavarde_( other.pavarde_ ),
      nd_( other.nd_ ), n_( other.n_ ), egzaminas_( other.egzaminas_ ) { }

Studentas& Studentas::operator=( const Studentas& other )
{
    if ( this != &other )
    {
        vardas_ = other.vardas_;
        pavarde_ = other.pavarde_;
        nd_ = other.nd_;
        n_ = other.n_;
        egzaminas_ = other.egzaminas_;
    }
    return *this;
}

Studentas::~Studentas( ) { }

double Studentas::galBalas( bool mediana ) const
{
    if ( mediana )
        return skaiciuotiGalutini( skaiciuotiMediana( nd_, n_ ), egzaminas_ );
    else
        return skaiciuotiGalutini( skaiciuotiVidurki( nd_, n_ ), egzaminas_ );
}

std::istream& Studentas::readStudent( std::istream& is )
{
    is >> vardas_ >> pavarde_;

    nd_.clear( );
    int val;
    while ( is >> val )
        nd_.push_back( val );

    if ( !nd_.empty( ) )
    {
        egzaminas_ = nd_.back( );
        nd_.pop_back( );
    }

    n_ = static_cast<int>( nd_.size( ) );
    return is;
}

bool comparePagalVarda( const Studentas& a, const Studentas& b )
{
    return a.vardas( ) < b.vardas( );
}

bool comparePagalPavarde( const Studentas& a, const Studentas& b )
{
    return a.pavarde( ) < b.pavarde( );
}
