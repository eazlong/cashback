#include "account.h"

account::account(void)
{
}

account::~account(void)
{
}

base_account::base_account( void )
{
	m_balance = 0;
}

base_account::~base_account( void )
{

}

float base_account::add( float cash )
{
	m_balance += cash;
	return m_balance;
}

float base_account::sub( float cash )
{
	m_balance -= cash;
	return m_balance;
}

float base_account::get_balance() const
{
	return m_balance;
}
