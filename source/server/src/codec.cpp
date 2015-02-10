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
			size_t s = temp.find(":");
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
	else
	{
		return false;
	}

	return true;
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
