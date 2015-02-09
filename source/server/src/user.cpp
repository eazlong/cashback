#include "user.h"
#include "log.h"
#include "rule.h"
#include "error_code.h"

user::user( /*user_info* info*/ )
:m_user_info("")
{
	m_user_info.token = 10000;
}

user::~user(void)
{
}

merchant::merchant( user_info* info )
{
	m_rule = new discount;
	m_clerk_set.insert("xiaoming");
	m_clerk_set.insert("laowang");
}

int merchant::get_cashback( float cash, const std::string& clerk, unsigned long& bussiness_token, float& cashback )
{
	int ret = m_rule->get_cashback( cash, cashback );
	if ( ret != SUCCESS )
	{
		debug_log( "clerk:" << clerk << " try to create a cashback failed. cash:" << cash << " cash back:" << cashback << " error:" << ret );
		return ret;
	}

	bussiness_token = 10001;
	debug_log( "clerk:" << clerk << " create a cashback. cash:" << cash << " cash back:" << cashback << " bussiness token:" << bussiness_token );

	trade t;
	t.cash = cash;
	t.cashback = cashback;
	t.name = clerk;
	t.ttype = CASHBACK_GENERATE;
	m_uncomplete_trade.insert( make_pair(bussiness_token, t) );

	return SUCCESS;
}

bool merchant::is_clerk_exist( const std::string& clerk )
{
	return m_clerk_set.find( clerk ) != m_clerk_set.end();
}

bool merchant::complete_trade( float cash, unsigned long btoken )
{
	std::map< unsigned long, trade >::iterator it = m_uncomplete_trade.find( btoken );
	if ( it == m_uncomplete_trade.end() )
	{
		return false;
	}
	m_uncomplete_trade.erase( it );
	debug_log( "the trade with token:" << btoken << " is completed!" );
	return true;
}

int merchant::request_cashback( float cash, trade_type type, const std::string& clerk, std::string& request_customer )
{
	float cashback = 0;
	int ret = m_rule->get_cashback( cash, cashback );
	if ( ret != SUCCESS )
	{
		return ret;
	}

	trade t;
	t.cash = cash;
	t.cashback = type==CASHBACK_GENERATE?cashback:0; //使用优惠券的交易不再生成优惠。
	t.name = request_customer;
	t.ttype = type;

	m_requesting_trade[clerk].push_back( t );

	return SUCCESS;
}

bool merchant::get_requesting_trade( const std::string& clerk, trade& t )
{
	if ( m_requesting_trade[clerk].empty() )
		return false;

	t = m_requesting_trade[clerk].front();
	m_requesting_trade[clerk].pop_front();
	return true;
}
