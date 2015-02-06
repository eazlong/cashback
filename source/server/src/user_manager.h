#pragma once

#include "datas.h"

class persistance;

class user_manager
{
public:
	virtual ~user_manager(void);

	static user_manager* get_instance()
	{
		return m_instance;
	}
	
	int regist( user_info* info );
	int login( user_info* info );
	int logout( const std::string& name );

protected:
	user_manager(void);
	user_manager( const user_manager& other );

	user* create_user( user_info* info );

private:
	static user_manager* m_instance;

	persistance* m_persistance;
	std::map<unsigned long, user*> m_user_map;
};
