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

float base_account::add( float cash, const std::string& merch )
{
	m_balance += cash;
	return m_balance;
}

float base_account::sub( float cash, const std::string& merch )
{
	m_balance -= cash;
	return m_balance;
}

float base_account::get_balance( const std::string& merchant_name ) const
{
	return m_balance;
}
