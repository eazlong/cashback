#include "codec.h"
#include "datas.h"
#include <assert.h>
#include <sstream>
#include "user.h"

void* key_value_codec::decode( const std::string& message )
{
	if ( message.empty() )
		return NULL;

	std::stringstream ss( message );
	std::string temp;
	void* buf = get_buffer();
	while ( getline( ss, temp ) )
	{
		if ( !temp.empty() )
		{
			size_t s = temp.find(SEPARATOR_1);
			std::string& key = temp.substr( 0, s );
			std::string& val = temp.substr( s+1 );
			if ( !fill_buffer( buf, key, val ) )
			{
				release_buffer( buf );
				return NULL;
			}
		}
	}
	return buf;
}

bool user_info_codec::fill_buffer( void* buf, const std::string& key, const std::string& val )
{
	user_info* info = (user_info*)buf;

	if ( key == "user_name" )
	{
		info->name = val;
	}
	else if ( key == "password" )
	{
		info->password = val;
	}
	else if ( key == "type" )
	{
		info->type = (user_type)atoi(val.c_str());
	}
	else if ( key == "token" )
	{
		info->token = atoi(val.c_str());
	}
	else
	{
		return false;
	}

	return true;
}

void* user_info_codec::get_buffer()
{
	return new user_info( "" );
}

void user_info_codec::release_buffer( void* buf )
{
	assert( buf != NULL );

	delete (user_info*)buf;
}


std::string user_info_codec::encode( void* data )
{
	assert( data != NULL );
	
	user* usr = (user*)data;
	std::ostringstream ss;
	ss << "token:" << usr->get_token();
	return ss.str();
}


bool cashback_generate_request_codec::fill_buffer( void* buf, const std::string& key, const std::string& val )
{
	cashback_request* gen = (cashback_request*)buf;
	if ( key == "user_name" )
	{
		gen->base.name = val;
	}
	else if ( key == "type" )
	{
		gen->base.type = (user_type)atoi(val.c_str());
	}
	else if ( key == "token" )
	{
		gen->base.token = atoi(val.c_str());
	}
	else if ( key == "clerk" )
	{
		gen->clerk = val;
	}
	else if ( key == "cash" )
	{
		gen->cash = (float)atof(val.c_str());
	}
	else if ( key == "merchant" )
	{
		gen->merchant_name = val;
	}
	else if ( key == "tradetype" )
	{
		gen->ttype = (trade_type)atoi(val.c_str());
	}
	else if ( key == "shared_list" )
	{
		decode_shared_list( val, gen->shares );
	}
	else
	{
		return false;
	}

	return true;
}

void cashback_generate_request_codec::decode_shared_list( const std::string& msg, std::map<std::string, shared_info>& shares )
{
	std::stringstream ss( msg );
	std::string temp;
	while ( getline( ss, temp, SEPARATOR_2.at(0) ) )
	{
		if ( temp.empty() )
			continue;
	
		shared_info info;
		std::string name;
		std::istringstream iss(temp);
		iss >> name >> info.merchant >> info.amount >> info.expiration;
		info.group = "";
		shares.insert( make_pair( name, info ) );
	}
}

void* cashback_generate_request_codec::get_buffer()
{
	return new cashback_request();
}

void cashback_generate_request_codec::release_buffer( void* buf )
{
	assert( buf != NULL );

	delete (cashback_request*)buf;
}

std::string cashback_generate_request_codec::encode( void* data )
{
	assert( data != NULL );

	cashback_generate_ack* ack = (cashback_generate_ack*)data;
	std::ostringstream ss;
	ss << "name:" << ack->name << "\ntype:" << (int)ack->type << "\nbtoken:" << ack->btoken << "\ncashback:" << ack->cashback;
	return ss.str();
}

bool cashback_confirm_request_codec::fill_buffer( void* buf, const std::string& key, const std::string& val )
{
	cashback_confirm_request* confirm = (cashback_confirm_request*)buf;
	if ( key == "user_name" )
	{
		confirm->base.name = val;
	}
	else if ( key == "type" )
	{
		confirm->base.type = (user_type)atoi(val.c_str());
	}
	else if ( key == "token" )
	{
		confirm->base.token = atoi(val.c_str());
	}
	else if ( key == "clerk" )
	{
		confirm->clerk = val;
	}
	else if ( key == "cash" )
	{
		confirm->cash = (float)atof(val.c_str());
	}
	else if ( key == "cashback" )
	{
		confirm->cashback = (float)atof(val.c_str());
	}
	else if ( key == "btoken" )
	{
		confirm->btoken = atoi(val.c_str());
	}
	else if ( key == "merchant" )
	{
		confirm->merchant_name = val;
	}
	else if ( key== "customer" )
	{
		confirm->customer_name = val;
	}
	else if ( key== "tradetype" )
	{
		confirm->ttype = (trade_type)atoi(val.c_str());
	}
	else
	{
		return false;
	}

	return true;
}

void* cashback_confirm_request_codec::get_buffer()
{
	return new cashback_confirm_request();
}

void cashback_confirm_request_codec::release_buffer( void* buf )
{
	assert( buf != NULL );

	delete (cashback_confirm_request*)buf;
}

std::string cashback_confirm_request_codec::encode( void* data )
{
	assert( data != NULL );
	std::ostringstream ss;
	ss << "balance:" << *(float*)data;
	return ss.str();
}

bool get_requesting_trade_codec::fill_buffer( void* buf, const std::string& key, const std::string& val )
{
	get_reqeusting_trade_request* requesting = (get_reqeusting_trade_request*)buf;
	if ( key == "user_name" )
	{
		requesting->base.name = val;
	}
	else if ( key == "type" )
	{
		requesting->base.type = (user_type)atoi(val.c_str());
	}
	else if ( key == "token" )
	{
		requesting->base.token = atoi(val.c_str());
	}
	else if ( key == "clerk" )
	{
		requesting->clerk = val;
	}
	else
	{
		return false;
	}

	return true;
}

void* get_requesting_trade_codec::get_buffer()
{
	return new get_reqeusting_trade_request();
}

void get_requesting_trade_codec::release_buffer( void* buf )
{
	assert( buf != NULL );

	delete (get_reqeusting_trade_request*)buf;
}

std::string get_requesting_trade_codec::encode( void* data )
{
	assert( data != NULL );

	trade* ack = (trade*)data;
	std::ostringstream ss;
	ss << "cash:" << ack->cash << "\ncashback:" << ack->cashback << "\ncustomer:" << ack->name << "\ntradetype:" << ack->ttype;
	return ss.str();
}

//base; operation; friend_name; nickname; group;
//base; operation; friend_name;
//base; operation;
// add del get

std::string friend_manager_codec::encode( void* data )
{
	if ( data == NULL )
		return "";

	std::list<friend_info>* friends = (std::list<friend_info>*)data;
	std::ostringstream ss;
	ss << "friends_list:";
	for ( std::list<friend_info>::iterator it = friends->begin(); 
		it != friends->end(); it++ )
	{
		ss << it->name << SEPARATOR_SPACE << it->group << SEPARATOR_SPACE << it->nickname << ";";
	}
	return ss.str();
}

bool friend_manager_codec::fill_buffer( void* buf, const std::string& key, const std::string& val )
{
	friend_manager_request* requesting = (friend_manager_request*)buf;
	if ( key == "user_name" )
	{
		requesting->base.name = val;
	}
	else if ( key == "type" )
	{
		requesting->base.type = (user_type)atoi(val.c_str());
	}
	else if ( key == "token" )
	{
		requesting->base.token = atoi(val.c_str());
	}
	else if ( key == OPERATION )
	{
		requesting->otype = (operation_type)atoi(val.c_str());
	}
	else if ( key == "friend_name" )
	{
		requesting->friends.name = val;
	}
	else if ( key == "group" )
	{
		requesting->friends.group = val;
	}
	else if ( key == "nickname" )
	{
		requesting->friends.nickname = val;
	}
	else
	{
		return false;
	}

	return true;
}

void* friend_manager_codec::get_buffer()
{
	return new friend_manager_request();
}

void friend_manager_codec::release_buffer( void* buf )
{
	assert( buf != NULL );
	delete (friend_manager_request*)buf;
}

std::string shared_manager_codec::encode( void* data )
{
	if ( data == NULL )
		return "";

	std::ostringstream ss;
	ss << "shared_list:";

	std::map< std::string,std::list<shared_info> >* shares_map = (std::map< std::string,std::list<shared_info> >*)data;
	for ( std::map< std::string,std::list<shared_info> >::iterator it_map=shares_map->begin();
		it_map != shares_map->end(); it_map ++ )
	{
		std::list<shared_info>& shareds = it_map->second;
		for ( std::list<shared_info>::iterator it = shareds.begin(); 
			it != shareds.end(); it++ )
		{
			ss << it_map->first << SEPARATOR_SPACE << it->merchant << SEPARATOR_SPACE << it->amount << SEPARATOR_SPACE << it->expiration << ";";
		}
	}
	
	return ss.str();
}

//base; merchant; type-self or shared
//base; merchant; group; cash; expiration;
//base; merchant; group;
//base; merchant; group; cash; expiration; friend_name;
bool shared_manager_codec::fill_buffer( void* buf, const std::string& key, const std::string& val )
{
	shared_manager_request* requesting = (shared_manager_request*)buf;
	if ( key == USER_NAME )
	{
		requesting->base.name = val;
	}
	else if ( key == "type" )
	{
		requesting->base.type = (user_type)atoi(val.c_str());
	}
	else if ( key == "token" )
	{
		requesting->base.token = atoi(val.c_str());
	}
	else if ( key == OPERATION )
	{
		requesting->otype = (operation_type)atoi(val.c_str());
	}
	else if ( key == "get" )
	{
		requesting->get_from = val;
	}
	else if ( key == "merchant" )
	{
		requesting->shared.merchant = val;
	}
	else if ( key == "group" )
	{
		requesting->shared.group = val;
	}
	else if ( key == "cash" )
	{
		requesting->shared.amount = (float)atof(val.c_str());
	}
	else if ( key == "expiration" )
	{
		requesting->shared.expiration = atof(val.c_str());
	}
	else
	{
		return false;
	}

	return true;
}

void* shared_manager_codec::get_buffer()
{
	return new shared_manager_request();
}

void shared_manager_codec::release_buffer( void* buf )
{
	assert( buf != NULL );

	delete (shared_manager_request*)buf;
}
