#pragma once
#include "datas.h"
class codec
{
public:
	virtual ~codec(void) {}

	virtual void* decode( const std::string& message ) = 0;
	virtual std::string encode( void* data ) = 0;
	virtual void release_buffer( void* buf ) = 0;
};

class key_value_codec : public codec
{
public:
	virtual ~key_value_codec(void) {}

	virtual void* decode( const std::string& message );

protected:
	virtual bool fill_buffer( void* buf, const std::string& key, const std::string& val ) = 0;
	virtual void* get_buffer() = 0;
	virtual void release_buffer( void* buf ) = 0;
};


class user_info_codec : public key_value_codec
{
public:
	virtual std::string encode( void* data );
protected:
	virtual bool fill_buffer( void* buf, const std::string& key, const std::string& val );
	virtual void* get_buffer();
	virtual void release_buffer( void* buf );
};

class cashback_generate_request_codec : public key_value_codec
{
public:
	virtual std::string encode( void* data );
protected:
	virtual bool fill_buffer( void* buf, const std::string& key, const std::string& val );
	virtual void* get_buffer();
	virtual void release_buffer( void* buf );

	void decode_shared_list( const std::string& msg, std::map<std::string, shared_info>& shared );
};

class cashback_confirm_request_codec : public key_value_codec
{
public:
	virtual std::string encode( void* data );
protected:
	virtual bool fill_buffer( void* buf, const std::string& key, const std::string& val );
	virtual void* get_buffer();
	virtual void release_buffer( void* buf );
};

class get_requesting_trade_codec : public key_value_codec
{
public:
	virtual std::string encode( void* data );
protected:
	virtual bool fill_buffer( void* buf, const std::string& key, const std::string& val );
	virtual void* get_buffer();
	virtual void release_buffer( void* buf );
};

class friend_manager_codec : public key_value_codec
{
public:
	virtual std::string encode( void* data );
protected:
	virtual bool fill_buffer( void* buf, const std::string& key, const std::string& val );
	virtual void* get_buffer();
	virtual void release_buffer( void* buf );
};

class shared_manager_codec : public key_value_codec
{
public:
	virtual std::string encode( void* data );
protected:
	virtual bool fill_buffer( void* buf, const std::string& key, const std::string& val );
	virtual void* get_buffer();
	virtual void release_buffer( void* buf );
};
