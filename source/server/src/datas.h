#pragma once

#include <string>

enum user_type
{
	CUSTOMER,
	MERCHANT
};

typedef struct user_info_t
{
	user_info_t( const std::string& user_name, user_type usertype = CUSTOMER, const std::string& user_password = "", 
		const std::string& user_address = "", const std::string& user_tel = "" )
		: name( user_name ), password( user_password ),address( user_address ), tel( user_tel ), type( usertype )
	{
		token = 0;
	}

	std::string name;
	std::string password;
	std::string address;
	std::string tel;
	user_type   type;

	unsigned long token; //用于用户的当次登录认证
}user_info;

typedef struct base_info_t
{
	std::string	  name;
	user_type     type;
	unsigned long token;
}base_info;

typedef struct cashback_generate_request_t
{
	base_info     base;
	float         cash;
	std::string   merchant_name;
	std::string   clerk;
}cashback_generate_request;

typedef struct cashback_generate_ack_t
{
	std::string	  name;
	user_type     type;
	unsigned long btoken;
	float         cashback;
}cashback_generate_ack;

typedef struct cashback_confirm_request_t
{
	base_info     base;
	float         cash;
	float         cashback;
	std::string   merchant_name;
	std::string   clerk;
	std::string   customer_name;
	unsigned long btoken;
}cashback_confirm_request;

typedef struct cashback_confirm_ack_t
{
	std::string	  name;
	user_type     type;
	unsigned long btoken;
	float         cashback;
}cashback_confirm_ack;

typedef struct trade_t
{
	float       cash;
	float       cashback;
	std::string name;
	time_t      time;
}trade;

typedef struct get_reqeusting_trade_request_t
{
	base_info     base;
	std::string   clerk;
}get_reqeusting_trade_request;

typedef struct get_reqeusting_trade_ack_t
{
	std::string	  name;
	float         cash;
	float         cashback;
}get_reqeusting_trade_ack;