#include "user_manager.h"
#include "log.h"
#include "user.h"
#include "error_code.h"
#include "persistance.h"
#include <algorithm>
#include <assert.h>

online_user_manager* online_user_manager::m_instance = new online_user_manager;

online_user_manager::online_user_manager(void)
:m_persistance( NULL )
{
}

online_user_manager::~online_user_manager(void)
{
}

int online_user_manager::regist( user_info* info )
{
	if ( info == NULL )
		return INVALID_PRAMETER;

	assert( m_persistance != NULL );

	if ( m_persistance->is_user_exist( info->name, info->type ) )
	{
		error_log( "user:" << info->name << " with type:" << info->type << " is exists!" );
		return USER_EXISTS;
	}
	
	if ( m_persistance->persist_user( info ) != SUCCESS )
	{
		error_log( "user:" << info->name << " persist failed!" );
		return PERSIST_FAILED;
	}

	return login( info );
}

bool online_user_manager::insert_user( user_info* info )
{
	user* usr;
	switch( info->type )
	{
	case CUSTOMER:
		usr = new customer( info );
		break;
	case MERCHANT:
		usr = new merchant( info );
		break;
	}

	return m_user_map.insert( make_pair( make_pair(info->name, info->type), usr ) ).second;
}

int online_user_manager::login( user_info* info )
{
	if ( NULL == info )
	{
		return INVALID_PRAMETER;
	}

	assert( m_persistance != NULL );

	int retcode = m_persistance->get_user_info( info, true ); //验证用户是否合法
	if ( SUCCESS != retcode )
	{
		return retcode;
	}

	
	std::map<key, user*>::iterator it = m_user_map.find( make_pair( info->name, info->type ) );
	if ( it != m_user_map.end() ) //已经登录
	{
		//更新登录token
		it->second->login();
	}
	else
	{
		insert_user( info );
	}

	return SUCCESS;
}

int online_user_manager::logout( user_info* info )
{
	if ( info == NULL || ( info->type != CUSTOMER && info->type != MERCHANT ) )
	{
		return INVALID_PRAMETER;
	}

	std::map<key, user*>::iterator it = m_user_map.find( make_pair( info->name, info->type ) );
	if ( it != m_user_map.end() )
	{
		if ( info->token == it->second->get_token() )
		{
			if ( it->second->logout() )
			{
				delete it->second;
				m_user_map.erase( it );
			}
			return SUCCESS;
		}

		return TOKEN_ERROR;
	}
	
	return USER_NOT_FOUND;
}

persistance* online_user_manager::set_persistance( persistance* p )
{
	persistance* temp = m_persistance;
	m_persistance = p;
	return temp;
}

user* online_user_manager::get_user( const std::string& name, user_type type ) const
{
	if ( name.empty() || ( type != CUSTOMER && type != MERCHANT ) )
	{
		return NULL;
	}

	std::map<key, user*>::const_iterator it = m_user_map.find( make_pair( name, type ) );
	if ( it != m_user_map.end() )
	{
		return it->second;
	}

	return NULL;
}

int online_user_manager::verify( const base_info& base, user** usr /*= NULL */ )
{
	user* u = get_user( base.name, base.type );
	if ( u == NULL )
	{
		return USER_NOT_FOUND;
	}

	if ( u->get_token() != base.token )
	{
		return TOKEN_ERROR;
	}

	if ( usr != NULL )
	{
		*usr = u;
	}

	return SUCCESS;
}
