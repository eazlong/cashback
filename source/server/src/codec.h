#pragma once
#include <string>

class codec
{
public:
	virtual ~codec(void) {}

	virtual void* decode( const std::string& message ) = 0;
	virtual std::string encode( void* data ) = 0;
};

class user_info_codec : public codec
{
public:
	virtual void* decode( const std::string& message );
	virtual std::string encode( void* data );
};

class callback_generate_request_codec : public codec
{
public:
	virtual void* decode( const std::string& message );
	virtual std::string encode( void* data );
};

class callback_confirm_request_codec : public codec
{
public:
	virtual void* decode( const std::string& message );
	virtual std::string encode( void* data );
};
