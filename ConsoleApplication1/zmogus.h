#ifndef ZMOGUS_H
#define ZMOGUS_H

#include <string>
#include <iostream>

class Zmogus
{
public:
    std::string vardas;
    std::string pavarde;

    Zmogus();
    virtual ~Zmogus() = 0;
};

#endif
