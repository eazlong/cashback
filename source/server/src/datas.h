#pragma once

#include <string>

#define date double

enum user_type
{
	CUSTOMER,
	MERCHANT
};

enum trade_type
{
	CASHBACK_GENERATE,
	CASHBACK_COST,
};

typedef struct user_info_t
{
	user_info_t( const std::string& user_name, user_type usertype = CUSTOMER, const std::string& user_password = "", 
		const std::string& user_address = "", const std::string& user_tel = "" )
		: name( user_name ), password( user_password ),address( user_address ), tel( user_tel ), type( usertype ),
		  token( 0 )
	{

	}
	
	user_info_t( const user_info_t& other )
	{
		name     = other.name;
		password = other.password;
		address  = other.address;
		tel      = other.tel;
		type     = other.type;
		token    = other.token;
	}

	std::string name;
	std::string password;
	std::string address;
	std::string tel;
	user_type   type;

	unsigned long token; //用户当次登录的合法凭证
}user_info;

typedef struct base_info_t
{
	std::string	  name;         //用户名
	user_type     type;         //用户类型
	unsigned long token;        //登录标示
}base_info;

typedef struct cashback_request_t
{
	base_info     base;

	float         cash;          //消费金额
	std::string   merchant_name; //商户名
	std::string   clerk;         //收银员
	trade_type    ttype;         //交易类型:生成 或者 消费
}cashback_request;

typedef struct cashback_generate_ack_t
{
	std::string	  name;         //用户名
	user_type     type;         //用户类型
	unsigned long btoken;       //交易凭证
	float         cashback;     //优惠金额
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
	trade_type    ttype;
}cashback_confirm_request;

typedef struct trade_t
{
	float       cash;     //消费金额
	float       cashback; //生成的优惠金额，在使用优惠金额消费的交易中为0.
	std::string name;
	trade_type  ttype;
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
	trade_type    ttype;
}get_reqeusting_trade_ack;

typedef struct friend_info_t
{
	friend_info_t( const std::string& n, const std::string& g )
		:name( n ), group( g )
	{
	}

	std::string name;
	std::string group;
}friend_info;

typedef struct shared_info_t
{
	shared_info_t( const std::string& m, const std::string& g, float a, date e )
		:merchant( m ), group( g ), amount( a ), expiration( e )
	{
	}
	std::string merchant;
	std::string group;
	float       amount;
	date        expiration;
}shared_info;
