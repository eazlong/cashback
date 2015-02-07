#include "persistance.h"
#include "error_code.h"

bool db::is_user_exist( const std::string& user_name, user_type type )
{
	std::map<key, user_info*>::iterator it = m_user_map.find( make_pair( user_name, type ) );
	return (it != m_user_map.end());
}

int db::get_user_info( user_info* info )
{
	std::map<key, user_info*>::iterator it = m_user_map.find( make_pair( info->name, info->type ) );
	if ( it != m_user_map.end() )
	{
		if ( info->password != it->second->password )
		{
			return PASSWORD_ERROR;
		}
		return SUCCESS;
	}
	return USER_NOT_FOUND;
}

int db::persist_user( user_info* info )
{
	user_info* temp = new user_info( info->name, info->type, info->password );
	m_user_map.insert( make_pair( make_pair( info->name, info->type ), temp ) );
	return SUCCESS;
}



