#include "persistance.h"
#include "error_code.h"
#include <sstream>

bool memory_persistance::is_user_exist( const std::string& user_name, user_type type )
{
	std::map<key, user_info*>::iterator it = m_user_map.find( make_pair( user_name, type ) );
	return (it != m_user_map.end());
}

int memory_persistance::get_user_info( user_info* info )
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

int memory_persistance::persist_user( user_info* info )
{
	user_info* temp = new user_info( info->name, info->type, info->password );
	m_user_map.insert( make_pair( make_pair( info->name, info->type ), temp ) );
	return SUCCESS;
}


db::db( const std::string& ip, const std::string& user, const std::string& password, const std::string& dbname, unsigned short port )
{
	mysql_init( &m_mysql );
	if ( !mysql_real_connect( &m_mysql, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, 0, 0 ) )
	{
		std::ostringstream iss;
		iss << "connect to mysql failed! address:" << ip << ":" << port << " user:" << user;
		std::exception e( iss.str().c_str() );
		throw std::exception( e );
	}
	
	std::string sql = "create database if not exists " + dbname;
	mysql_query( &m_mysql, sql.c_str() );

	if ( !init_db( dbname ) )
	{
		std::exception e( "init database failed!" );
		throw std::exception( e );
	}
}

db::~db()
{
	mysql_close( &m_mysql );
}

bool db::is_user_exist( const std::string& user_name, user_type type )
{
	return true;
}

bool db::init_db( const std::string& db_name )
{
	mysql_query( &m_mysql, "create table if not exists user( id int auto_increment primary key not null, name varchar(20) not null, type tinyint, phone varchar(20), address varchar(100) )" );
	mysql_query( &m_mysql, "create table if not exists friend( user_id int, foreign key(user_id) references user(id), friend_id int, foreign key(friend_id) references user(id), friend_group varchar(20) );" );
	mysql_query( &m_mysql, "create table if not exists shared( id int auto_increment primary key, user_id int, foreign key(user_id) references user(id), amount float, friend_group varchar(20), expiration date );" );
	mysql_query( &m_mysql, "create table if not exists account( user_id int, foreign key(user_id) references user(id), merchant int, foreign key(merchant) references user(id), amount float, expiration date );" );
	return true;
}


int db::get_user_info( user_info* info )
{
	return SUCCESS;
}

int db::persist_user( user_info* info )
{
	return SUCCESS;
}