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
	virtual int get_user_info( user_info* info, bool verify_password  ) = 0;
	virtual int persist_user( user_info* info ) = 0;
};

class memory_persistance : public persistance	
{
public:
	virtual bool is_user_exist( const std::string& user_name, user_type type );
	virtual int get_user_info( user_info* info, bool verify_password  );
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
	virtual int get_user_info( user_info* info, bool verify_password );
	int get_user_info( int user_id, user_info* info );
	virtual int persist_user( user_info* info );
	virtual int get_user_id( const std::string& user_name, user_type type );

	int add_friend( int user_id, int friend_id, , const std::string& nickname );
	int del_friend( int user_id, int friend_id );
	int change_group( int user_id, int friend_id, const std::string& group );
	int get_friends( int user_id, const std::string& group, std::list<friend_info>& friends );
	std::string get_group( int user_id, const std::string& friend_name );

	int add_account( int user_id, int merchant_id, float amount, mydate date );
	int del_account( int user_id, int merchant_id );
	int update_account( int user_id, int merchant_id,  float amount );
	
	int add_shared( int user_id, int merchant_id, float amount, const std::string& group, mydate date );
	int del_shared( int share_id );
	int update_shared( int share_id, float amount );

protected:
	bool init_db( const std::string& db_name );
	void free_result();
	
protected:
	MYSQL      m_mysql;
};