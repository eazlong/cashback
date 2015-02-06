#pragma once

#include <map>
#include <list>
#include <string>
#include "datas.h"

class rule;

class user
{
public:
	user(void);
	virtual ~user(void);

protected:
	typedef std::map< std::string, std::list<std::string> > friends_map; //group, friend user name
	friends_map m_friend_map;//
	//account*    m_account;
	user_info*  m_user_info; //用户基本信息
};


class customer : public user
{
public:

};

class merchant : public user
{
public:
	
private:
	rule*                  m_rule;       //优惠规则
	std::list<std::string> m_clerk_list; //已登录的店员名
};