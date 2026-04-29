#ifndef STUDENTAS_H
#define STUDENTAS_H

#include <string>
#include <vector>
#include <iostream>

class Studentas
{
public:
    std::string vardas;
    std::string pavarde;
    std::vector<int> nd;
    int n;
    int egzaminas;

    Studentas();
    ~Studentas();
    Studentas( const Studentas& kitas );
    Studentas& operator=( const Studentas& kitas );
    Studentas( Studentas&& kitas ) noexcept;
    Studentas& operator=( Studentas&& kitas ) noexcept;
};

std::ostream& operator<<( std::ostream& out, const Studentas& s );
std::istream& operator>>( std::istream& in, Studentas& s );

#endif
