#pragma once

class processer
{
public:
	virtual ~processer(void) {}

	virtual int process( void* data ) = 0;
};

class register_processer : public processer
{
public:
	virtual int process( void* data );
};