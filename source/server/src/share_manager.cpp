#include "share_manager.h"
#include "error_code.h"

share_manager::share_manager(void)
{
}

share_manager::~share_manager(void)
{
}

int share_manager::share( float cash, const std::string& group_name, const std::string& merchant, date d )
{
	m_shared_map[merchant].push_back(  shared_info( merchant, group_name, cash, d ) );
	return SUCCESS;
}

int share_manager::cancel_share( const std::string& group_name, const std::string& merchant )
{
	std::list<shared_info>& shares = m_shared_map[merchant];
	for ( std::list<shared_info>::iterator it = shares.begin();
		it != shares.end(); )
	{
		if ( it->group == group_name )
		{
			shares.erase( it );
		}
		else
		{
			it ++;
		}
	}
	return SUCCESS;
}

int share_manager::get_shared_cash( const std::string& merchant, std::list<shared_info>& shared_infos )
{
	std::list<shared_info>& shares = m_shared_map[merchant];
	std::copy( shares.begin(), shares.end(), shared_infos.begin() );
	return SUCCESS;
}
