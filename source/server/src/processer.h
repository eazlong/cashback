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

//�û�ע��
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

//�û���¼
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

//�û��ǳ�
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

//�̼һ����û����������Ż�ȯ
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

//�̼������Ż�ȯ���û�ɨ���ά�뷢��ȷ����Ϣ
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

//�û����������Ż�ȯ���̼�ȷ��
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

//��Աˢ������Ҫȷ�ϵĽ�����Ϣ
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

//�û�ˢ�����
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

//�û����Ѵ���ȯ
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

//���ѹ���
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

//�������
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

