#pragma once
#include "log.h"
#ifdef _WIN32
#include 
#include <process.h> 
#define fectch_and_add( ptr, num ) InterlockedExchangeAdd( ptr, num )
#define sub_and_fetch( ptr, num ) InterlockedExchange( ptr, -(num) )
#else
#include <pthread.h>
#define fectch_and_add( ptr, num ) __sync_fetch_and_add( ptr, num )
#define sub_and_fetch( ptr, num ) __sync_fetch_and_add( ptr, num )
#endif

#ifndef NULL
#define NULL 0
#endif

////////////////////////////////////////////////////////////////////////////////

template <class T>
class sharedptr
{
public:
	sharedptr( T* v = NULL ) throw()
		: m_instance( v ), m_count( NULL )
	{
		if ( m_instance != NULL )
		{
			m_count = new int(1);
		}
	}

	sharedptr( const sharedptr<T>& v ) throw()
		: m_instance( v.m_instance ), m_count( v.m_count )
	{
		if ( m_instance != NULL )
		{
			debug_log( "SharePtr copy constructor instance:" << v.m_instance << " origin count:" << *v.m_count );
			fectch_and_add( m_count, 1 );
		}
	}

	~sharedptr() throw()
	{
		release();
	}

	//#ifdef _DEBUG
	int get_count() const
	{
		return (*m_count);
	}
	//#endif

public:
	operator T *() const throw()
	{
		return m_instance;
	}

	T& operator *() const throw()
	{
		return *m_instance;
	}

	bool operator!() const throw()
	{
		return (m_instance == NULL);
	}

	bool operator<(T* v) const throw()
	{
		return m_instance < v;
	}

	bool operator==(T* v) const throw()
	{
		return m_instance == v;
	}

	bool operator != ( T* v ) const throw()
	{
		return m_instance != v;
	}

	T* operator=(T* v) throw()
	{
		release();

		m_instance = v;

		if ( m_instance != NULL )
		{
			if ( m_count == NULL )
			{
				m_count = new int(1);
			}
			else
			{
				fectch_and_add( m_count, 1 );
			}
		}

		return m_instance;
	}

	T* operator=(const sharedptr<T>& v) throw()
	{
		release();

		m_instance = v.m_instance;
		m_count = v.m_count;

		if ( m_instance != NULL )
		{
			debug_log( "operator= instance:" << v.m_instance << " origin count:" << *v.m_count );
			fectch_and_add( m_count, 1 );
		}

		return m_instance;
	}

	T* operator->() const throw()
	{
		return m_instance;
	}
protected:
	void release()
	{
		if ( m_instance == NULL )
			return;

		debug_log( "Release instance:" << m_instance << " origin count:" << *m_count );
		if ( sub_and_fetch( m_count, 1 ) == 0 )
		{
			delete m_instance;
			m_instance = NULL;

			delete m_count;
			m_count = NULL;
		}
	}
private:
	T* m_instance;
	int* m_count;
};
