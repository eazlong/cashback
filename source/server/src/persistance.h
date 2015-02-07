#pragma once

#include "datas.h"
#include <string>
#include <map>

class persistance
{
public:
	virtual ~persistance(void) {}
	
	virtual bool is_user_exist( const std::string& user_name, user_type type ) = 0;
	virtual int get_user_info( user_info* info ) = 0;
	virtual int persist_user( user_info* info ) = 0;
};

class db : public persistance	
{
public:
	virtual bool is_user_exist( const std::string& user_name, user_type type );
	virtual int get_user_info( user_info* info );
	virtual int persist_user( user_info* info );
private:
	typedef std::pair<std::string, user_type> key;
	std::map< key, user_info* > m_user_map;
};