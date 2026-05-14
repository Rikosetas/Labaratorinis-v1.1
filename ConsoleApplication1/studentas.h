#ifndef STUDENTAS_H
#define STUDENTAS_H

#include "zmogus.h"
#include <vector>
#include <iostream>

/**
 * @brief Studento duomenu klase, paveldinti is Zmogus.
 *
 * Realizuoja 5-iu metodu taisykle (Rule of Five): kopijavimo/perkelimo
 * konstruktorius, kopijavimo/perkelimo priskyrimo operatorius ir destruktorius.
 */
class Studentas : public Zmogus
{
public:
    std::vector<int> nd;
    int n;
    int egzaminas;

    /// @brief Default konstruktorius. Visi laukai nustatomi i 0/"".
    Studentas();
    ~Studentas() noexcept override;

    /// @brief Kopijavimo konstruktorius.
    Studentas( const Studentas& kitas );
    Studentas& operator=( const Studentas& kitas );

    /// @brief Perkelimo konstruktorius. Saltinis paliekamas tuscias.
    Studentas( Studentas&& kitas ) noexcept;
    Studentas& operator=( Studentas&& kitas ) noexcept;
};

std::ostream& operator<<( std::ostream& out, const Studentas& s );
std::istream& operator>>( std::istream& in, Studentas& s );

#endif
