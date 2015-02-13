#include "friends_manager.h"

friend_manager::friend_manager(int id)
:m_userid(id)
{
}

friend_manager::~friend_manager(void)
{
}

bool friend_manager::add_friend( const friend_info& info )
{
	int id = m_persistance->get_user_id( info.name, CUSTOMER );
	if ( id < 0 )
	{
		return false;
	}

	if ( m_persistance->add_friend( m_userid, id , info.group, info.nickname ) < 0 )
	{
		return false;
	}

	return true;
}

bool friend_manager::del_friend( const std::string& name )
{
	int id = m_persistance->get_user_id( name, CUSTOMER );
	if ( id < 0 )
	{
		return false;
	}

	if ( m_persistance->del_friend( m_userid, id ) < 0 )
	{
		return false;
	}

	return true;
}

bool friend_manager::change_group( const std::string& name, const std::string& group )
{
	int id = m_persistance->get_user_id( name, CUSTOMER );
	if ( id < 0 )
	{
		return false;
	}

	if ( m_persistance->change_group( m_userid, id, group ) < 0 )
	{
		return false;
	}

	return true;
}

bool friend_manager::get_friends( const std::string& group, std::list<friend_info>& friends )
{
	if (  m_persistance->get_friends( m_userid, group, friends ) < 0 )
	{
		return false;
	}

	return true;
}

std::string friend_manager::get_group( const std::string& name )
{
	friends_map::iterator it = m_friend_map.find(name);
	if ( it == m_friend_map.end() )
	{
		return "";
	}
	
	return it->second.group;
}
