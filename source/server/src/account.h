#pragma once
#include <string>

class account
{
public:
	account(void);
	virtual ~account(void);

	virtual float add( float cash ) = 0;
	virtual float sub( float cash ) = 0;
	virtual float get_balance( const std::string& merchant_name ) const = 0;
};

class base_account : public account
{
public:
	base_account(void);
	virtual ~base_account(void);

	virtual float add( float cash );
	virtual float sub( float cash );
	virtual float get_balance( const std::string& merchant_name ) const;
protected:
	float m_balance;
};
