#ifndef STUDENTAS_UTILS_H
#define STUDENTAS_UTILS_H

#include "studentas.h"
#include <vector>

void generuotiPazymius( Studentas& s, int n );
void generuotiVarda( Studentas& s, int indeksas );
int pasirinktiRusiavima( );
void rusiuotiStudentus( std::vector<Studentas>& studentai, int rusiavimas );

#endif
