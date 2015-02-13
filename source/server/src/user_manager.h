#pragma once

#include <map>
#include "datas.h"

class user;
class persistance;

class online_user_manager
{
public:
	virtual ~online_user_manager(void);

	static online_user_manager* get_instance()
	{
		return m_instance;
	}
	
	//返回旧的persistance,不存在返回0
	persistance* set_persistance( persistance* p );

	int regist( user_info* info );
	int login( user_info* info );
	int logout( user_info* info );
	
	int verify( const base_info& base, user** usr = NULL );
	user* get_user( const std::string& name, user_type type ) const;

protected:
	online_user_manager(void);
	online_user_manager( const online_user_manager& other );

	bool insert_user( user_info* info );

private:
	static online_user_manager* m_instance;

	persistance* m_persistance;
	typedef std::pair<std::string, user_type> key;
	std::map<key, user*> m_user_map;
};

