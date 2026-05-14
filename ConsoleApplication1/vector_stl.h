#pragma once

#include <stdexcept>
#include <limits>
#include <utility>
#include <initializer_list>
#include <iterator>
#include <algorithm>

/**
 * @file vector_stl.h
 * @brief Sablono klase Vector<T>, atkartojanti std::vector funkcionaluma.
 */

 /**
  * @brief Dinaminis masyvas, atkartojantis std::vector elgesi.
  *
  * Klase realizuoja: Rule of Five (kopijavimo / perkelimo ctor + operator=),
  * elementu prieigos metodus (operator[], at, front, back, data),
  * dydzio valdyma (size, capacity, empty, reserve, resize, shrink_to_fit, clear),
  * keitimo metodus (push_back, pop_back, insert, erase, emplace, emplace_back, swap),
  * iteratorius (begin, end, cbegin, cend) ir lyginimo operatorius.
  *
 * Papildomai realizuotas reallocation skaitliukas - @ref realloc_count() grazina
 * kiek kartu vidiniu buferiu perskirstymas (capacity perskaiciavimas) buvo atliktas.
 *
 * Pavyzdys:
 * @code
 * Vector<int> v;
 * v.push_back(1);
 * v.push_back(2);
 * v.push_back(3);
 * for (int x : v) std::cout << x << " "; // 1 2 3
 * std::cout << v.realloc_count();         // 3 (1->2, 2->4)
 * @endcode
 *
 * @tparam T elemento tipas.
 */
template<typename T>
class Vector {
private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    size_t realloc_count_ = 0;

public:
    using iterator = T*;
    using const_iterator = const T*;

public:
    /// @brief Default konstruktorius - tuscias vektorius.
    Vector( ) : data_( nullptr ), size_( 0 ), capacity_( 0 ), realloc_count_( 0 ) { }

    /**
     * @brief Konstruktorius su pradiniu dydziu.
     * @param count elementu skaicius.
     * @param value pradine reiksme (default = T()).
     */
    explicit Vector( size_t count, const T& value = T( ) ) : data_( nullptr ), size_( 0 ), capacity_( 0 ), realloc_count_( 0 )
    {
        if ( count > 0 )
        {
            reserve( count );

            for ( size_t i = 0; i < count; i++ )
                data_ [ i ] = value;

            size_ = count;
        }
    }

    /// @brief Konstruktorius is initializer_list (Vector<int> v = {1,2,3}).
    Vector( std::initializer_list<T> il ) : data_( nullptr ), size_( 0 ), capacity_( 0 ), realloc_count_( 0 )
    {
        reserve( il.size( ) );

        for ( const auto& v : il )
            data_ [ size_++ ] = v;
    }

    /// @brief Destruktorius.
    ~Vector( ) { delete[ ] data_; }

    /// @brief Kopijavimo konstruktorius.
    Vector( const Vector& other ) : data_( nullptr ), size_( other.size_ ), capacity_( other.capacity_ ), realloc_count_( 0 )
    {
        if ( capacity_ > 0 )
        {
            data_ = new T [ capacity_ ];

            for ( size_t i = 0; i < size_; i++ )
                data_ [ i ] = other.data_ [ i ];
        }
    }

    /// @brief Kopijavimo priskyrimas.
    Vector& operator=( const Vector& other )
    {
        if ( this == &other ) return *this;

        delete[ ] data_;

        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = capacity_ > 0 ? new T [ capacity_ ] : nullptr;

        for ( size_t i = 0; i < size_; i++ )
            data_ [ i ] = other.data_ [ i ];

        return *this;
    }

    /// @brief Perkelimo konstruktorius.
    Vector( Vector&& other ) noexcept : data_( other.data_ ), size_( other.size_ ), capacity_( other.capacity_ ), realloc_count_( other.realloc_count_ )
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.realloc_count_ = 0;
    }

    /// @brief Perkelimo priskyrimas.
    Vector& operator=( Vector&& other ) noexcept
    {
        if ( this == &other ) return *this;

        delete[ ] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        realloc_count_ = other.realloc_count_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.realloc_count_ = 0;

        return *this;
    }

public:
    /// @brief Lyginimo operatorius == (elementu palyginimas).
    bool operator==( const Vector& other ) const
    {
        if ( size_ != other.size_ )
            return false;

        for ( size_t i = 0; i < size_; i++ )
            if ( data_ [ i ] != other.data_ [ i ] )
                return false;

        return true;
    }

    /// @brief Lyginimo operatorius < (leksikografinis).
    bool operator<( const Vector& other ) const
    {
        for ( size_t i = 0; i < size_ && i < other.size_; i++ )
        {
            if ( data_ [ i ] < other.data_ [ i ] ) return true;
            if ( data_ [ i ] > other.data_ [ i ] ) return false;
        }
        return size_ < other.size_;
    }

    bool operator!=( const Vector& other ) const { return !( *this == other ); }
    bool operator<=( const Vector& other ) const { return !( other < *this ); }
    bool operator>( const Vector& other )  const { return other < *this; }
    bool operator>=( const Vector& other ) const { return !( *this < other ); }

private:
    /**
     * @brief Vidinis buferiu perskirstymas.
     * @param wanted norimas naujas capacity (0 = automatinis x2 augimas).
     */
    void internal_grow( size_t wanted = 0 )
    {
        size_t new_capacity = wanted == 0
            ? ( capacity_ == 0 ? 1 : capacity_ * 2 )
            : wanted;

        T* new_data = new T [ new_capacity ];
        for ( size_t i = 0; i < size_; i++ )
            new_data [ i ] = std::move( data_ [ i ] );

        delete[ ] data_;
        data_ = new_data;
        capacity_ = new_capacity;

        realloc_count_++;
    }

public:
    /// @brief Elemento prieiga be ribu tikrinimo.
    T& operator[]( size_t index ) { return data_ [ index ]; }
    const T& operator[]( size_t index ) const { return data_ [ index ]; }

    /// @brief Elemento prieiga su ribu tikrinimu (gali mesti std::out_of_range).
    T& at( size_t index )
    {
        if ( index >= size_ )
            throw std::out_of_range( "Vector::at index out of range" );

        return data_ [ index ];
    }

    const T& at( size_t index ) const
    {
        if ( index >= size_ )
            throw std::out_of_range( "Vector::at index out of range" );

        return data_ [ index ];
    }

    T& front( ) { return data_ [ 0 ]; }
    const T& front( ) const { return data_ [ 0 ]; }

    T& back( ) { return data_ [ size_ - 1 ]; }
    const T& back( )  const { return data_ [ size_ - 1 ]; }

    T* data( ) { return data_; }
    const T* data( ) const { return data_; }

    /// @brief Tikrina ar vektorius tuscias.
    bool empty( ) const { return size_ == 0; }

    /// @brief Grazina elementu skaiciu.
    size_t size( ) const { return size_; }

    /// @brief Grazina capacity (kiek elementu telpa be reallocation).
    size_t capacity( ) const { return capacity_; }

    /// @brief Maksimalus galimas elementu skaicius.
    size_t max_size( ) const { return std::numeric_limits<size_t>::max( ) / sizeof( T ); }

    /// @brief Grazina kiek kartu ivyko vidinis perskirstymas.
    size_t realloc_count( ) const { return realloc_count_; }

    /**
     * @brief Rezervuoja vietos bent new_cap elementu, jei dar nera.
     * @param new_cap norima capacity reiksme.
     */
    void reserve( size_t new_cap )
    {
        if ( new_cap <= capacity_ ) return;
        internal_grow( new_cap );
    }

    /**
     * @brief Sumazina capacity iki size (jei reikia).
     */
    void shrink_to_fit( )
    {
        if ( size_ == capacity_ ) return;

        if ( size_ == 0 )
        {
            delete[ ] data_;
            data_ = nullptr;

            capacity_ = 0;
            realloc_count_++;
            return;
        }

        T* new_data = new T [ size_ ];

        for ( size_t i = 0; i < size_; i++ )
            new_data [ i ] = std::move( data_ [ i ] );

        delete[ ] data_;

        data_ = new_data;
        capacity_ = size_;
        realloc_count_++;
    }

public:
    iterator       begin( ) { return data_; }
    iterator       end( ) { return data_ + size_; }

    const_iterator begin( )  const { return data_; }
    const_iterator end( )    const { return data_ + size_; }

    const_iterator cbegin( ) const { return data_; }
    const_iterator cend( )   const { return data_ + size_; }

public:
    /// @brief Prideda elementa i pabaiga (kopija).
    void push_back( const T& value )
    {
        if ( size_ == capacity_ )
            internal_grow( );

        data_ [ size_++ ] = value;
    }

    /// @brief Prideda elementa i pabaiga (perkelimas).
    void push_back( T&& value )
    {
        if ( size_ == capacity_ )
            internal_grow( );
        data_ [ size_++ ] = std::move( value );
    }

    /// @brief Pasalina paskutini elementa.
    void pop_back( )
    {
        if ( empty( ) )
            throw std::runtime_error( "Vector::pop_back on empty vector" );

        size_--;
    }

    /// @brief Istusina vektoriu (size_ = 0, capacity nepasikeicia).
    void clear( ) { size_ = 0; }

    /**
     * @brief Iterpia elementa pries pos pozicija.
     * @return iteratorius i naujai iterpta elementa.
     */
    iterator insert( const_iterator pos, const T& value )
    {
        size_t index = pos - data_;
        if ( index > size_ )
            throw std::out_of_range( "Vector::insert index out of range" );

        if ( size_ == capacity_ )
            internal_grow( );

        for ( size_t i = size_; i > index; i-- )
            data_ [ i ] = std::move( data_ [ i - 1 ] );

        data_ [ index ] = value;
        size_++;
        return data_ + index;
    }

    /**
     * @brief Pasalina elementa pos pozicijoje.
     * @return iteratorius i kita elementa po pasalintojo.
     */
    iterator erase( const_iterator pos )
    {
        size_t index = pos - data_;
        if ( index >= size_ )
            throw std::out_of_range( "Vector::erase index out of range" );

        for ( size_t i = index; i + 1 < size_; i++ )
            data_ [ i ] = std::move( data_ [ i + 1 ] );

        size_--;
        return data_ + index;
    }

    /**
     * @brief Iterpia konstrukuojant elementa pries pos pozicija (perfect forwarding).
     */
    template<typename ... Args>
    iterator emplace( const_iterator pos, Args&&... args )
    {
        size_t index = pos - data_;
        if ( index > size_ )
            throw std::out_of_range( "Vector::emplace index out of range" );

        if ( size_ == capacity_ )
            internal_grow( );

        for ( size_t i = size_; i > index; i-- )
            data_ [ i ] = std::move( data_ [ i - 1 ] );

        data_ [ index ] = T( std::forward<Args>( args )... );
        size_++;
        return data_ + index;
    }

    /// @brief Konstrukuoja elementa pabaigoje (perfect forwarding).
    template<typename ... Args>
    void emplace_back( Args&&... args )
    {
        if ( size_ == capacity_ )
            internal_grow( );

        data_ [ size_ ] = T( std::forward<Args>( args )... );
        size_++;
    }

    /**
     * @brief Pakeicia dydi i count, ipildo value, jei reikia padidinti.
     */
    void resize( size_t count, const T& value = T( ) )
    {
        if ( count == size_ ) return;

        if ( count < size_ )
        {
            size_ = count;
            return;
        }

        if ( count > capacity_ )
            internal_grow( count );

        for ( size_t i = size_; i < count; i++ )
            data_ [ i ] = value;

        size_ = count;
    }

    /// @brief Sukeicia dvieju vektoriu turini.
    void swap( Vector& other ) noexcept
    {
        std::swap( data_, other.data_ );
        std::swap( size_, other.size_ );
        std::swap( capacity_, other.capacity_ );
        std::swap( realloc_count_, other.realloc_count_ );
    }
};