#pragma once
#include "datas.h"
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
	virtual int main_process( void* data, std::string& ret_msg ) = 0;

protected:
	codec* m_codec;
};

//用户注册
class regist_processer : public processer
{
public:
	regist_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
};

//用户登录
class login_processer : public processer
{
public:
	login_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
};

//用户登出
class logout_processer : public processer
{
public:
	logout_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
};

//商家或者用户请求生成优惠券
class generate_request_processer : public processer
{
public:
	generate_request_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
};

//商家生成优惠券后，用户扫描二维码发送确认信息
class cashback_customer_confirm_processer : public processer
{
public:
	cashback_customer_confirm_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
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
	virtual int main_process( void* data, std::string& ret_msg );
};

//店员刷新其需要确认的交易信息
class get_reqesting_trade_processer: public processer
{
public:
	get_reqesting_trade_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
};

//用户刷新余额
class refresh_processer: public processer
{
public:
	refresh_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
};

//用户消费代金券
class cost_cashback_processer: public processer
{
public:
	cost_cashback_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
	bool cost_shares( const std::string& name, const std::string& friend_name, shared_info& shared );
	bool cancel_trade( std::map<std::string, shared_info*>& already );
};

//好友管理
class friends_processer: public processer
{
public:
	friends_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
};

//分享管理
class shared_processer: public processer
{
public:
	shared_processer( codec* c )
		:processer( c )
	{
	}
protected:
	virtual int main_process( void* data, std::string& ret_msg );
};

