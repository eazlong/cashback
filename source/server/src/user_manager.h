#pragma once

#include "datas.h"

class persistance;

class user_manager
{
public:
	virtual ~user_manager(void);

	static user_manager* get_instance()
	{
		return m_instance;
	}
	
	int regist( user_info* info );

protected:
	user_manager(void);
	user_manager( const user_manager& other );

private:
	static user_manager* m_instance;

	persistance* m_persistance;

};
