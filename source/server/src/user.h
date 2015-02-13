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
	friend_manager*               m_friends_manager; //���ѹ���
	share_manager*                 m_share_manager;   //������Ϣ����
	account*                       m_account;         //�˺�
	user_info                      m_user_info;       //�û�������Ϣ
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

	virtual void login()//�̻������ظ���¼�����ø���token
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
	rule*                  m_rule;       //�Żݹ���
	std::set<std::string>  m_clerk_set;  //��Ա�б�
	std::map< unsigned long, trade >          m_uncomplete_trade; //�̻�����
	std::map< std::string, std::list<trade> > m_requesting_trade;
};