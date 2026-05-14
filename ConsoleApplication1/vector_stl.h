#pragma once
#include <stdexcept>

template<typename T>
class Vector {
private:
	T* data_ = nullptr;
	size_t size_ { }; // currently koks dydis
	size_t capacity_ { }; // maksimalus galimas dydis

public:
	// constructor
	Vector( ) : data_( nullptr ), size_( 0 ), capacity_( 0 ) { };

	// destructor
	~Vector( ) { delete[ ] data_; };

	// copy constructor
	Vector( const Vector& other ) : data_( new T [ other.capacity_ ] ), size_( other.size_ ), capacity_( other.capacity_ )
	{ 
		for ( size_t i = 0; i < size_; i++ )
			data_ [ i ] = other.data_ [ i ];
	};

	// copy asignment
	Vector& operator=( const Vector& other )
	{
		if ( this == &other ) return *this;

		delete[ ] data_;

		size_ = other.size_;
		capacity_ = other.capacity_;
		data_ = new T [ capacity_ ];

		for ( size_t i = 0; i < size_; i++ )
			data_ [ i ] = other.data_ [ i ];

		return *this;
	};

	// move constructor
	Vector( Vector&& other ) : data_( other.data_ ), size_( other.size_ ), capacity_( other.capacity_ ) 
	{
		other.data_ = nullptr;
		other.size_ = NULL;
		other.capacity_ = NULL;
	}

	// move asigment
	Vector& operator=( Vector&& other )
	{
		if ( this == &other ) return *this;

		delete[ ] data_;

		data_ = other.data_;
		size_ = other.size_;
		capacity_ = other.capacity_;

		other.data_ = nullptr;
		other.size_ = NULL;
		other.capacity_ = NULL;

		return *this;
	}

public:
	bool operator==( const Vector& other ) const 
	{
		if ( size_ != other.size_ )
			return false;

		for ( size_t i = 0; i < size_; i++ )
			if ( data_ [ i ] != other.data_ [ i ] )
				return false;

		return true;
	}

	bool operator<( const Vector& other ) const
	{
		for ( size_t i = 0; i < size_ && i < other.size_; i++ )
		{
			if ( data_ [ i ] < other.data_ [ i ] )
				return true;

			if ( data_ [ i ] > other.data_ [ i ] )
				return false;
		}

		return size_ < other.size_;
	}

	bool operator!=( const Vector& other ) const { return !( *this == other ); }

	bool operator<=( const Vector& other ) const { return !( other < *this ); }

	bool operator>( const Vector& other ) const { return other < *this; }

	bool operator>=( const Vector& other ) const { return !( *this < other ); }

private:
	void internal_grow( size_t wanted = NULL )
	{
		size_t new_capacity = wanted == NULL ? ( capacity_ == 0 ? 1 : capacity_ * 2 ) : wanted;

		T* new_data = new T [ new_capacity ];
		for ( size_t i = 0; i < size_; i++ )
			new_data [ i ] = data_ [ i ];

		delete[ ] data_;
		data_ = new_data;

		capacity_ = new_capacity;
	}

public:
	T& operator[]( size_t index ) 
	{
		return data_ [ index ];
	}

	const T& operator[]( size_t index ) const
	{
		return data_ [ index ];
	}

	T& at( size_t index ) 
	{
		if ( index >= size_ )
			throw std::out_of_range( "OOB Access" );

		return data_ [ index ];
	}

	T& front( )
	{
		return data_ [ 0 ];
	};

	const T& front( ) const 
	{
		return data_ [ 0 ];
	}

	T& back( ) 
	{
		return data_ [ size_ - 1 ];
	}

	const T& back( ) const 
	{
		return data_ [ size_ - 1 ];
	}

	T* data( )
	{
		return data_;
	}

	bool empty( )
	{
		return size_ == 0;
	}

	size_t size( ) const
	{
		return size_;
	}

	size_t capacity( ) const
	{
		return capacity_;
	}

	size_t max_size( ) const
	{
		return std::numeric_limits<T>::max( ) / sizeof( T );
	}

	void reserve( size_t new_cap )
	{
		if ( new_cap <= capacity( ) )
			return;

		T* new_data = new T [ new_cap ];
		for ( size_t i = 0; i < size_; i++ )
			new_data [ i ] = data_ [ i ];

		delete[ ] data_;
		data_ = new_data;

		capacity_ = new_cap;
	}

	void shrink_to_fit( )
	{
		if ( size( ) == capacity( ) )
			return;

		T* new_data = new T [ size_ ];
		for ( size_t i = 0; i < size_; i++ )
			new_data [ i ] = data_ [ i ];

		delete[ ] data_;

		data_ = new_data;
		capacity_ = size_;
	}

public:
	using itterator = T*;
	using const_itterator = const T*;

	iterator begin( ) { return data_; }
	iterator end( ) { return data_ + size_; }

	const_iterator cbegin( )const { return data_; }
	const_iterator cend( )  const { return data_ + size_; }

public:
	void push_back( const T& value )
	{
		if ( size_ == capacity_ )
			internal_grow( );

		data_ [ size_++ ] = value;
	}

	void pop_back( )
	{
		if ( empty( ) )
			throw std::runtime_error( "Vector is empty you cannot pop back on an empty instance" );

		size_--;
	}

	void clear( )
	{
		size_ = 0;
	}

	itterator insert( const_itterator pos, const T& value )
	{
		size_t index = pos - begin( );
		if ( index > size_ )
			throw std::out_of_range( "index out of range" );

		if ( size_ == capacity_ )
			internal_grow( );

		for ( size_t i = size_; i > index; i-- )
			data_ [ i ] = data_ [ i - 1 ];

		data_ [ index ] = value;
		size_++;

		return data_ + index;
	}

	itterator erase( const_itterator pos )
	{
		size_t index = pos - begin( );
		if ( index > size_ )
			throw std::out_of_range( "index out of range" );

		for ( size_t i = index; i < size_ - 1; i++ )
			data_ [ i ] = data_ [ i + 1 ];

		size_--;
		return data_ + index;
	}

	template<typename ... Args>
	itterator emplace( const_itterator pos, Args&&... args )
	{
		size_t index = pos - begin( );
		if ( index > size_ )
			throw std::out_of_range( "index out of range" );

		if ( size_ == capacity_ )
			internal_grow( );

		for ( size_t i = size_; i > index; i-- )
			data_ [ i ] = data_ [ i - 1 ];

		data_ [ index ] = T( std::forward<Args>(args)... );
		size_++;

		return data_ + index;
	}

	template<typename ... Args>
	void emplace_back( Args&&... args )
	{
		if ( size_ == capacity_ )
			internal_grow( );

		data_ [ index ] = T( std::forward<Args>( args )... );
		size_++;
	}

	void resize( size_t count, const T& value = T( ) )
	{
		if ( count == size_ )
			return;

		if ( count < size_ ) 
		{
			size_ = count;
			return;
		}

		if ( count > capacity_ ) 
			internal_grow( count );

		for ( size_t i = size_; i < count; i++ )
			data_ [ i ] = value;
	}

	void swap( Vector& other )
	{
		T* t_data = data_;
		size_t t_size = size_;
		size_t t_cap = capacity_;

		data_ = other.data_;
		size_ = other.size_;
		capacity_ = other.capacity_;

		other.data_ = t_data;
		other.size_ = t_size;
		other.capacity_ = t_cap;
	}
};