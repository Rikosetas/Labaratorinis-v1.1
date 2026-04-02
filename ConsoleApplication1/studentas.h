#ifndef STUDENTAS_H
#define STUDENTAS_H

#include <string>
#include <vector>
#include <iostream>

class Studentas
{
private:
    std::string vardas_;
    std::string pavarde_;
    std::vector<int> nd_;
    int n_;
    int egzaminas_;

public:
    Studentas( );
    Studentas( std::istream& is );
    Studentas( const Studentas& other );
    Studentas& operator=( const Studentas& other );
    ~Studentas( );

    inline std::string vardas( ) const { return vardas_; }
    inline std::string pavarde( ) const { return pavarde_; }
    inline int egzaminas( ) const { return egzaminas_; }
    inline const std::vector<int>& nd( ) const { return nd_; }
    inline int n( ) const { return n_; }

    void setVardas( const std::string& v ) { vardas_ = v; }
    void setPavarde( const std::string& p ) { pavarde_ = p; }
    void setEgzaminas( int e ) { egzaminas_ = e; }
    void setNd( const std::vector<int>& nd ) { nd_ = nd; }
    void setN( int n ) { n_ = n; }

    double galBalas( bool mediana ) const;
    std::istream& readStudent( std::istream& is );
};

bool comparePagalVarda( const Studentas& a, const Studentas& b );
bool comparePagalPavarde( const Studentas& a, const Studentas& b );

#endif
