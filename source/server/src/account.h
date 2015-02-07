#pragma once

class account
{
public:
	account(void);
	virtual ~account(void);

	virtual float add( float cash ) = 0;
	virtual float sub( float cash ) = 0;
	virtual float get_balance() const = 0;
};

class base_account : public account
{
public:
	base_account(void);
	virtual ~base_account(void);

	virtual float add( float cash );
	virtual float sub( float cash );
	virtual float get_balance() const;
protected:
	float m_balance;
};
