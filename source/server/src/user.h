#pragma once

#include <map>
#include <list>
#include <set>
#include "datas.h"
#include "account.h"

class rule;

class user
{
public:
	user( /*user_info* info*/ );
	virtual ~user(void);
	virtual int get_token() const
	{
		return m_user_info.token;
	}

	virtual account* get_account() 
	{
		return &m_account;
	}

	virtual void login() = 0;
	virtual bool logout() = 0;
	
protected:
	typedef std::map< std::string, std::list<std::string> > friends_map; //group, friend user name
	base_account     m_account;
	friends_map m_friend_map;//
	user_info   m_user_info; //用户基本信息
};


class customer : public user
{
public:
	customer( user_info* info ){}
	virtual void login()
	{
		m_user_info.token ++ ;
	}
	
	virtual bool logout()
	{
		return true;
	}
};

class merchant : public user
{
public:
	merchant( user_info* info );

	virtual void login()//商户可以重复登录，不用更新token
	{
		m_login_count ++;
	}
	virtual bool logout()
	{
		return ( --m_login_count == 0 );
	}

	int get_cashback( float cash, std::string& clerk, unsigned long& bussiness_token, float& cashback );
	int request_cashback( float cash, std::string& clerk, std::string& request_customer );
	bool is_clerk_exist( const std::string& clerk );
	bool complete_trade( float cash, unsigned long btoken );

	std::set<std::string>* get_clerk_list()
	{
		return &m_clerk_set;
	}
private:
	rule*                  m_rule;       //优惠规则
	std::set<std::string>  m_clerk_set;  //店员列表
	int                    m_login_count;
	struct trade
	{
		float       cash;
		float       cashback;
		std::string name;
		time_t      time;
	};
	std::map< unsigned long, trade > m_uncomplete_trade; //商户发起
	std::map< std::string, trade > m_requesting_trade;
};