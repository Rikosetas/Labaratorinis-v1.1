#ifndef STUDENTAS_H
#define STUDENTAS_H

#include "zmogus.h"
#include <vector>
#include <iostream>

class Studentas : public Zmogus
{
public:
    std::vector<int> nd;
    int n;
    int egzaminas;

    Studentas();
    ~Studentas() noexcept override;
    Studentas( const Studentas& kitas );
    Studentas& operator=( const Studentas& kitas );
    Studentas( Studentas&& kitas ) noexcept;
    Studentas& operator=( Studentas&& kitas ) noexcept;
};

std::ostream& operator<<( std::ostream& out, const Studentas& s );
std::istream& operator>>( std::istream& in, Studentas& s );

#endif
