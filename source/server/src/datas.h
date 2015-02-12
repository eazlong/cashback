#pragma once

#include <string>
#include <list>
#include <map>

#define mydate double

static const std::string SEPARATOR_1 = ":";
static const std::string SEPARATOR_2 = ";";
static const std::string SEPARATOR_SPACE = " ";

static const std::string USER_NAME = "user_name";
static const std::string OPERATION = "operation";

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

enum operation_type
{
	ADD,
	DEL,
	GET,
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

	unsigned long token; //�û����ε�¼�ĺϷ�ƾ֤
}user_info;

typedef struct base_info_t
{
	std::string	  name;         //�û���
	user_type     type;         //�û�����
	unsigned long token;        //��¼��ʾ
}base_info;

typedef struct shared_info_t
{
	shared_info_t( const std::string& m, const std::string& g, float a, mydate e )
		:merchant( m ), group( g ), amount( a ), expiration( e )
	{
	}
	shared_info_t(){}
	shared_info_t( const shared_info_t& other )
	{
		merchant   = other.merchant;
		group      = other.group;
		amount     = other.amount;
		expiration = other.expiration;
	}
	shared_info_t& operator=( const shared_info_t& other )
	{
		if ( this == &other )
			return *this;
		merchant   = other.merchant;
		group      = other.group;
		amount     = other.amount;
		expiration = other.expiration;
		return *this;
	}
	
	std::string merchant;
	std::string group;
	float       amount;
	mydate        expiration;
}shared_info;


typedef struct cashback_request_t
{
	base_info     base;

	float         cash;          //���ѽ��
	std::string   merchant_name; //�̻���
	std::string   clerk;         //����Ա
	trade_type    ttype;         //��������:���� ���� ����
	std::map<std::string, shared_info> shares; //������ʱ��Ч������ʱ������ʹ�����ѹ�����Ż�ȯ
}cashback_request;

typedef struct cashback_generate_ack_t
{
	std::string	  name;         //�û���
	user_type     type;         //�û�����
	unsigned long btoken;       //����ƾ֤
	float         cashback;     //�Żݽ��
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
	float       cash;     //���ѽ��
	float       cashback; //���ɵ��Żݽ���ʹ���Żݽ�����ѵĽ�����Ϊ0.
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
	friend_info_t( const std::string& n, const std::string& g, const std::string& nn )
		:name( n ), group( g ), nickname( nn )
	{
	}
	
	friend_info_t(){}
	friend_info_t( const friend_info_t& other )
	{
		name = other.name;
		group = other.group;
		nickname = other.nickname;
	}

	std::string name;
	std::string group;
	std::string nickname;
}friend_info;


typedef struct friend_manager_request_t
{
	base_info     base;
	operation_type otype;
	friend_info   friends;
}friend_manager_request;

typedef struct shared_manager_request_t
{
	base_info      base;
	operation_type otype;
	shared_info    shared;
	std::string    get_from;
}shared_manager_request;
