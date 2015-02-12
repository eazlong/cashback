#include "friends_manager.h"

friends_manager::friends_manager(void)
{
}

friends_manager::~friends_manager(void)
{
}

bool friends_manager::add_friend( const friend_info& info )
{
	return m_friend_map.insert( make_pair( info.name, info ) ).second;
}

bool friends_manager::del_friend( const std::string& name )
{
	friends_map::iterator it = m_friend_map.find(name);
	if ( it == m_friend_map.end() )
	{
		return false;
	}

	m_friend_map.erase( it );

	return true;
}

bool friends_manager::change_group( const std::string& name, const std::string& group )
{
	friends_map::iterator it = m_friend_map.find(name);
	if ( it == m_friend_map.end() )
	{
		return false;
	}

	it->second.group = group;

	return true;
}

bool friends_manager::get_friends( const std::string& group, std::list<friend_info>& friends )
{
	friends_map::iterator it = m_friend_map.begin();
	for ( ; it != m_friend_map.end(); it++ )
	{
		if ( group == "" ) //取所有好友
		{
			friends.push_back( it->second );
		}
		else if ( it->second.group == group )
		{
			friends.push_back( it->second );
		}
	}
	return true;
}

std::string friends_manager::get_group( const std::string& name )
{
	friends_map::iterator it = m_friend_map.find(name);
	if ( it == m_friend_map.end() )
	{
		return "";
	}
	
	return it->second.group;
}
