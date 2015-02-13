#pragma once

#include <map>
#include <list>
#include <set>
#include "datas.h"
#include "account.h"
#include "share_manager.h"
#include "friends_manager.h"

class rule;

class user
{
public:
	user( user_info* info, account* accnt = NULL );
	virtual ~user(void);

	virtual account* get_account() 
	{
		return m_account;
	}
	
	virtual int get_token() const
	{
		return m_user_info.token;
	}

	virtual void login() = 0;
	virtual bool logout() = 0;

	friend_manager* get_friends_manager() const
	{
		return m_friends_manager;
	}

	share_manager* get_shared_manager() const
	{
		return m_share_manager;
	}

protected:
	friend_manager*               m_friends_manager; //好友管理
	share_manager*                 m_share_manager;   //分享信息管理
	account*                       m_account;         //账号
	user_info                      m_user_info;       //用户基本信息
};


class customer : public user
{
public:
	customer( user_info* info )
		:user(info), m_requst(NULL)
	{

	}
	
	virtual ~customer()
	{
	
	}

	virtual void login()
	{
		m_user_info.token ++ ;
	}
	
	virtual bool logout()
	{
		return true;
	}

	void trading( cashback_request* request )
	{
		m_requst = new cashback_request;
		m_requst->base.name = request->base.name;
		m_requst->merchant_name = request->merchant_name;
		m_requst->cash = request->cash;
		m_requst->ttype = request->ttype;
		m_requst->shares = request->shares;
	}

	void finish_trade()
	{
		m_account->sub( m_requst->cash, m_requst->merchant_name );
		delete m_requst;
		m_requst = NULL;
	}
protected:
	cashback_request* m_requst;
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

	int get_cashback( float cash, const std::string& clerk, unsigned long& bussiness_token, float& cashback );
	int request_cashback( float cash, trade_type type, const std::string& clerk, std::string& request_customer );
	bool is_clerk_exist( const std::string& clerk );
	bool complete_trade( float cash, unsigned long btoken );
	bool get_requesting_trade( const std::string& clerk, trade& t );
	std::set<std::string>* get_clerk_list()
	{
		return &m_clerk_set;
	}
private:
	int                    m_login_count;
	rule*                  m_rule;       //优惠规则
	std::set<std::string>  m_clerk_set;  //店员列表
	std::map< unsigned long, trade >          m_uncomplete_trade; //商户发起
	std::map< std::string, std::list<trade> > m_requesting_trade;
};