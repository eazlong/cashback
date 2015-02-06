#pragma once
#include <string>

class codec
{
public:
	virtual ~codec(void) {}

	virtual void* decode( const std::string& message ) = 0;
	//virtual std::string& encode( void* data ) = 0;
};

class register_codec : public codec
{
public:
	virtual void* decode( const std::string& message );
	//virtual std::string& encode( void* data );
};