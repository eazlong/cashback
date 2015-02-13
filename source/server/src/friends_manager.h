#pragma once

#include <string>
#include <map>
#include <list>
#include "datas.h"

class friend_manager
{
public:
	friend_manager( int id );
	virtual ~friend_manager(void);

	//∫√”—π‹¿Ì
	virtual bool add_friend( const friend_info& info );
	virtual bool del_friend( const std::string& name );
	virtual bool change_group( const std::string& name, const std::string& group );

	virtual bool get_friends( const std::string& group, std::list<friend_info>& friends );
	virtual std::string get_group( const std::string& name );

protected:
	typedef std::map< std::string, friend_info > friends_map; //friend user name
	friends_map                    m_friend_map;
	int                            m_userid;
	db*                            m_persistance;
};
