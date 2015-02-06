#pragma once

class persistance
{
public:
	virtual ~persistance(void) {}

	virtual int get_user() = 0;
	virtual int persist_user() = 0;
};
