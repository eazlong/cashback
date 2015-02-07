#include "codec.h"
#include "datas.h"
#include <assert.h>
#include <sstream>
#include "user.h"

void* user_info_codec::decode( const std::string& message )
{
	if ( message.empty() )
		return NULL;

	user_info * info = new user_info( "" );
	
	std::stringstream ss( message );
	std::string temp;
	while ( getline( ss, temp ) )
	{
		if ( !temp.empty() )
		{
			size_t s = temp.find(":");
			std::string& key = temp.substr( 0, s );
			std::string& val = temp.substr( s+1 );
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
		}
	}

	return info;
}

std::string user_info_codec::encode( void* data )
{
	assert( data != NULL );
	
	user* usr = (user*)data;
	std::ostringstream ss;
	ss << "token:" << usr->get_token();
	return ss.str();
}

void* callback_generate_request_codec::decode( const std::string& message )
{
	if ( message.empty() )
		return NULL;

	std::stringstream ss( message );
	std::string temp;
	cashback_generate_request* gen = new cashback_generate_request();
	while ( getline( ss, temp ) )
	{
		if ( !temp.empty() )
		{
			size_t s = temp.find(":");
			std::string& key = temp.substr( 0, s );
			std::string& val = temp.substr( s+1 );
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
		}
	}

	return gen;
}

std::string callback_generate_request_codec::encode( void* data )
{
	assert( data != NULL );

	cashback_generate_ack* ack = (cashback_generate_ack*)data;
	std::ostringstream ss;
	ss << "name:" << ack->name << "\ntype:" << (int)ack->type << "\nbtoken:" << ack->btoken << "\ncashback:" << ack->cashback;
	return ss.str();
}


void* callback_confirm_request_codec::decode( const std::string& message )
{
	if ( message.empty() )
		return NULL;

	std::stringstream ss( message );
	std::string temp;
	cashback_confirm_request* gen = new cashback_confirm_request();
	while ( getline( ss, temp ) )
	{
		if ( !temp.empty() )
		{
			size_t s = temp.find(":");
			std::string& key = temp.substr( 0, s );
			std::string& val = temp.substr( s+1 );
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
			else if ( key == "cashback" )
			{
				gen->cashback = (float)atof(val.c_str());
			}
			else if ( key == "btoken" )
			{
				gen->btoken = atoi(val.c_str());
			}
			else if ( key == "merchant" )
			{
				gen->merchant_name = val;
			}
		}
	}

	return gen;
}

std::string callback_confirm_request_codec::encode( void* data )
{
	assert( data != NULL );
	std::ostringstream ss;
	ss << "balance:" << *(float*)data;
	return ss.str();
}
