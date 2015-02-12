#pragma once

#include "datas.h"
#include <string>
#include <map>
#include <WinSock2.h>
#include <mysql.h>
#pragma comment( lib, "libmysql.lib" )

class persistance
{
public:
	virtual ~persistance(void) {}
	
	virtual bool is_user_exist( const std::string& user_name, user_type type ) = 0;
	virtual int get_user_info( user_info* info ) = 0;
	virtual int persist_user( user_info* info ) = 0;
};

class memory_persistance : public persistance	
{
public:
	virtual bool is_user_exist( const std::string& user_name, user_type type );
	virtual int get_user_info( user_info* info );
	virtual int persist_user( user_info* info );
private:
	typedef std::pair<std::string, user_type> key;
	std::map< key, user_info* > m_user_map;
};

class db : public persistance
{
public:
	db( const std::string& ip, const std::string& user, const std::string& password, const std::string& dbname, unsigned short port );
	virtual ~db();

	virtual bool is_user_exist( const std::string& user_name, user_type type );
	virtual int get_user_info( user_info* info );
	virtual int persist_user( user_info* info );
protected:
	bool init_db( const std::string& db_name );

protected:
	MYSQL      m_mysql;
	MYSQL_RES  m_mysql_res;
};