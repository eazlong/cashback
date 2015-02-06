#include "user_manager.h"
#include "log.h"
#include "error_code.h"
#include "persistance.h"

user_manager* user_manager::m_instance = new user_manager;

user_manager::user_manager(void)
{
}

user_manager::~user_manager(void)
{
}

int user_manager::regist( user_info* info )
{
	if ( info == NULL )
		return INVALID_PRAMETER;

	unsigned long user_id = m_persistance->get_user_id( info->name, info->type );
	if ( user_id != -1 )
	{
		error( "user:" << info->name << " with type:" << info->type << " is exists!" );
		return USER_EXISTS;
	}
	
	if ( !m_persistance->persist_user( info ) )
	{
		error( "user:" << info->name << " persist failed!" );
		return PERSIST_FAILED;
	}
	
	user_id = m_persistance->get_user_id( info->name, info->type );



	m_user_map.insert( std::make_pair( user_id, usr ) );

	return SUCCESS;
}

user* user_manager::create_user( user_info* info )
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

	return usr;
}

int user_manager::login( user_info* info )
{
	if ( !get_user_info( info ) )
		return USER_VERIFY_FAILED;
	
	m_user_map.insert( std::make_pair( info->id, create_user(info) ) )
	return SUCCESS;
}

int user_manager::logout( const std::string& name )
{
	
}
