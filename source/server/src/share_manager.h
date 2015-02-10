#pragma once

#include <string>
#include <map>
#include <list>
#include "datas.h"

class share_manager
{
public:
	share_manager( );
	virtual ~share_manager(void);

	//·ÖÏí
	virtual int share( float cash, const std::string& group_name, const std::string& merchant, date d );
	virtual int cancel_share( const std::string& group_name, const std::string& merchant );
	virtual int get_shared_cash( const std::string& merchant, std::list<shared_info>& shared_infos );

private:
	std::map< std::string, std::list<shared_info>> m_shared_map;
};
