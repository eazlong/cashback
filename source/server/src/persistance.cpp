#include "persistance.h"
#include "error_code.h"
#include "log.h"
#include <sstream>

bool memory_persistance::is_user_exist( const std::string& user_name, user_type type )
{
	std::map<key, user_info*>::iterator it = m_user_map.find( make_pair( user_name, type ) );
	return (it != m_user_map.end());
}

int memory_persistance::get_user_info( user_info* info, bool verify_password )
{
	std::map<key, user_info*>::iterator it = m_user_map.find( make_pair( info->name, info->type ) );
	if ( it != m_user_map.end() )
	{
		if ( info->password != it->second->password && verify_password )
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
	if ( NULL == mysql_real_connect( &m_mysql, ip.c_str(), user.c_str(), password.c_str(), NULL, port, 0, 0 ) )
	{
		std::ostringstream oss;
		oss << "connect to mysql failed! address:" << ip << ":" << port << " user:" << user;
		std::exception e( oss.str().c_str() );
		throw std::exception( e );
	}
	
	std::string sql = "create database if not exists " + dbname;
	mysql_query( &m_mysql, sql.c_str() );
	sql = "use " + dbname;
	mysql_query( &m_mysql, sql.c_str() );

	if ( !init_db( dbname ) )
	{
		std::exception e( "init database failed!" );
		throw std::exception( e );
	}

	free_result();
}

db::~db()
{
	mysql_close( &m_mysql );
}

bool db::is_user_exist( const std::string& user_name, user_type type )
{
	std::string sql = "select * from user where name=\"" + user_name + "\" and type=" + ((type==CUSTOMER)?"0":"1");
	if ( SUCCESS != mysql_query( &m_mysql, sql.c_str() ) )
	{
		return false;
	}
	MYSQL_RES * result = mysql_store_result( &m_mysql );
	int count = (int)mysql_num_rows( result );
	mysql_free_result(result); 
	return 1 == count;
}

bool db::init_db( const std::string& db_name )
{
	mysql_query( &m_mysql, "create table if not exists user( id int auto_increment primary key not null, name varchar(20) not null, type tinyint, unique(name, type), password varchar(20), phone varchar(20), address varchar(100) )" );
	mysql_query( &m_mysql, "create table if not exists friend( user_id int, foreign key(user_id) references user(id), friend_id int, foreign key(friend_id) references user(id), friend_group varchar(20), nickname varchar(20));" );
	mysql_query( &m_mysql, "create table if not exists shared( id int auto_increment primary key, user_id int, foreign key(user_id) references user(id), merchant_id int, foreign key(merchant) references user(id), amount float, friend_group varchar(20), expiration date );" );
	mysql_query( &m_mysql, "create table if not exists account( user_id int, foreign key(user_id) references user(id), merchant_id int, foreign key(merchant) references user(id), amount float, expiration date );" );
	return true;
}


int db::get_user_info( user_info* info, bool verify_password )
{
	std::string sql = "select * from user where name=\"" + info->name + "\" and type=" + ((info->type==CUSTOMER)?"0":"1");
	if ( SUCCESS != mysql_query( &m_mysql, sql.c_str() ) )
	{
		return USER_NOT_FOUND;
	}
	
	MYSQL_RES *res = mysql_store_result( &m_mysql );
	if ( res == NULL )
	{
		return USER_NOT_FOUND;
	}

	MYSQL_ROW row = mysql_fetch_row( res );
	if ( row == NULL )
	{
		return USER_NOT_FOUND;
	}
	
	if ( verify_password && strcmp( info->password.c_str(), row[3] ) != 0)
	{
		return PASSWORD_ERROR;
	}

	info->id = atoi(row[0]);
	info->password = row[3];
	info->tel = row[4];
	info->address = row[5];

	mysql_free_result( res );

	return SUCCESS;
}

int db::get_user_info( int user_id, user_info* info )
{
	std::string sql = "select * from user where id=" + user_id ;
	if ( SUCCESS != mysql_query( &m_mysql, sql.c_str() ) )
	{
		return USER_NOT_FOUND;
	}

	MYSQL_RES *res = mysql_store_result( &m_mysql );
	if ( res == NULL )
	{
		return USER_NOT_FOUND;
	}

	MYSQL_ROW row = mysql_fetch_row( res );
	if ( row == NULL )
	{
		return USER_NOT_FOUND;
	}

	info->id = atoi(row[0]);
	info->password = row[3];
	info->tel = row[4];
	info->address = row[5];

	mysql_free_result( res );

	return SUCCESS;
}

int db::persist_user( user_info* info )
{
	std::string sql = "insert into user( name, type, password, phone, address) values( \"" + info->name 
		+ "\"," + ((info->type==CUSTOMER)?"0":"1") + ",\"" + info->password + "\",\"" + info->tel + "\",\"" + info->address + "\");";
	if ( SUCCESS != mysql_query( &m_mysql, sql.c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_EXISTS;
	}

	free_result();

	return SUCCESS;
}

void db::free_result()
{
	MYSQL_RES * result;
	do 
	{ 
		result = mysql_store_result( &m_mysql );
		mysql_free_result(result); 
	}while( !mysql_next_result( &m_mysql ) );
}

int db::add_friend( int user_id, int friend_id, const std::string& group, const std::string& nickname )
{
	std::ostringstream oss;
	oss << "insert into friend values( " << user_id << "," << friend_id << ",\"" << group.c_str() << "\" ,\"" << nickname.c_str() << "\");";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_EXISTS;
	}
	
	return SUCCESS;
}

int db::del_friend( int user_id, int friend_id )
{
	std::ostringstream oss;
	oss << "delete from friend where user_id = " << user_id << " and friend_id =" << friend_id <<";";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_NOT_FOUND;
	}

	return SUCCESS;
}

int db::change_group( int user_id, int friend_id, const std::string& group )
{
	std::ostringstream oss;
	oss << "update friend set group=\"" << group << "\" where user_id = " << user_id << " and friend_id =" + friend_id <<";";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_NOT_FOUND;
	}

	return SUCCESS;
}


int db::get_friends( int user_id, const std::string& group, std::list<friend_info>& friends )
{
	std::ostringstream oss;
	oss << "select id from friend where user_id=" << user_id ;
	if ( !group.empty() )
	{
		oss << " and group=\"" << group +"\"";
	}

	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_NOT_FOUND;
	}

	MYSQL_RES *res = mysql_store_result( &m_mysql );
	if ( res == NULL )
	{
		return USER_NOT_FOUND;
	}

	MYSQL_ROW row = mysql_fetch_row( res );
	while ( row != NULL )
	{
		int friend_id = itoa(row[1]);
		user_info u;
		get_user_info( user_id, &u );
		friend_info f( u.name, row[2], row[3] );
		friends.push_back( f );
		row = mysql_fetch_row( res );
	}
	mysql_free_result( res );
	return SUCCESS;
}


std::string db::get_group( int user_id, const std::string& friend_name )
{
	int id = get_user_id( friend_name, CUSTOMER );
	if ( id < 0 )
	{
		return "";
	}

	std::ostringstream oss;
	oss << "select group from friend where user_id=" << user_id << " and friend_id=" << id;
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return "";
	}

	MYSQL_RES *res = mysql_store_result( &m_mysql );
	if ( res == NULL )
	{
		return "";
	}

	MYSQL_ROW row = mysql_fetch_row( res );
	std::string r( row[0] );
	mysql_free_result( res );
	return r;
}


int db::add_account( int user_id, int merchant_id, float amount, mydate date )
{
	std::ostringstream oss;
	oss << "insert into account values( " << user_id << "," << merchant_id << "," << amount << "," << date << ");";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_EXISTS;
	}
	return SUCCESS;
}

int db::del_account( int user_id, int merchant_id )
{
	std::ostringstream oss;
	oss << "delete from account where user_id = " << user_id << " and merchant_id =" << merchant_id <<";";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_NOT_FOUND;
	}

	return SUCCESS;
}

int db::update_account( int user_id, int merchant_id, float amount )
{
	std::ostringstream oss;
	oss << "update account set amount=" << amount << " where user_id = " << user_id << " and merchant_id =" << merchant_id <<";";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_NOT_FOUND;
	}

	return SUCCESS;
}

//id int auto_increment primary key, user_id int, foreign key(user_id) references user(id), merchant_id int, foreign key(merchant) references user(id), amount float, friend_group varchar(20), expiration date

int db::add_shared( int user_id, int merchant_id, float amount, const std::string& group, mydate date )
{
	std::ostringstream oss;
	oss <<"insert into shared(user_id, merchant_id, amount, friend_group, expiration) values( " << user_id << "," << merchant_id << "," << amount << ",\"" << group << "\"," << date << ");";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_EXISTS;
	}
	return SUCCESS;
}

int db::del_shared( int share_id )
{
	std::ostringstream oss;
	oss << "delete from shared where id = " << share_id << ";";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_NOT_FOUND;
	}

	return SUCCESS;
}

int db::update_shared( int share_id, float amount )
{
	std::ostringstream oss;
	oss << "update shared set amount=" << amount << " where id = " << share_id << ";";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_NOT_FOUND;
	}

	return SUCCESS;
}

int db::get_user_id( const std::string& user_name, user_type type )
{
	std::ostringstream oss;
	oss << "select id from user where name=" << user_name << " and type=" << (int)type << ";";
	if ( SUCCESS != mysql_query( &m_mysql, oss.str().c_str() ) )
	{
		const char* p = mysql_error( &m_mysql );
		debug_log( "mysql_query failed in " << __FUNCTION__ << ":" << p );
		return USER_NOT_FOUND;
	}

	MYSQL_RES *res = mysql_store_result( &m_mysql );
	if ( res == NULL )
	{
		return USER_NOT_FOUND;
	}

	MYSQL_ROW row = mysql_fetch_row( res );
	if ( row == NULL )
	{
		return USER_NOT_FOUND;
	}

	int id = atoi( row[0] );
	
	mysql_free_result( res );
	return id;
}
