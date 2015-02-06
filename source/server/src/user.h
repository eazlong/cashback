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
	user_info*  m_user_info; //�û�������Ϣ
};


class customer : public user
{
public:

};

class merchant : public user
{
public:
	
private:
	rule*                  m_rule;       //�Żݹ���
	std::list<std::string> m_clerk_list; //�ѵ�¼�ĵ�Ա��
};