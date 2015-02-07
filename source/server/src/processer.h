#pragma once
#include <string>
class codec;

class processer
{
public:
	processer( codec* c )
		:m_codec(c) 
	{
	}
	virtual ~processer(){}

	int process( const std::string& message, std::string& ret_msg );

protected:
	virtual int process_data( void* data, std::string& ret_msg ) = 0;

protected:
	codec* m_codec;
};

class regist_processer : public processer
{
public:
	regist_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int process_data( void* data, std::string& ret_msg );
};

class login_processer : public processer
{
public:
	login_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int process_data( void* data, std::string& ret_msg );
};

class logout_processer : public processer
{
public:
	logout_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int process_data( void* data, std::string& ret_msg );
};

class generate_request_processer : public processer
{
public:
	generate_request_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int process_data( void* data, std::string& ret_msg );
};

class cashback_customer_confirm_processer : public processer
{
public:
	cashback_customer_confirm_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int process_data( void* data, std::string& ret_msg );
};

//用户请求生成优惠券后，商家确认
class cashback_merchant_confirm_processer : public processer
{
public:
	cashback_merchant_confirm_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int process_data( void* data, std::string& ret_msg );
};