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

	if ( m_persistance->get_user( info, NULL, NULL ) )
	{
		error( "user:" << info->name << " with type:" << info->type << " is exists!" );
		return USER_EXISTS;
	}
	
	if ( !m_persistance->persist_user( info ) )
	{
		error( "user:" << info->name << " persist failed!" );
		return PERSIST_FAILED;
	}

	return SUCCESS;
}
