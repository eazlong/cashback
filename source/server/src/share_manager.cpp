#include "share_manager.h"
#include "error_code.h"

share_manager::share_manager(void)
{
}

share_manager::~share_manager(void)
{
}

int share_manager::share( const shared_info& info )
{
	shared_info shared;
	if ( get_shared_cash(info.group, info.merchant, shared ) == SUCCESS )
	{
		return FAILED;
	}

	m_shared_map[info.merchant].push_back( info );
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

int share_manager::get_shared_cash( const std::string& group_name, const std::string& merchant, shared_info& shared )
{
	std::list<shared_info>& shares = m_shared_map[merchant];
	for ( std::list<shared_info>::iterator it = shares.begin();
		it != shares.end(); )
	{
		if ( it->group == group_name )
		{
			shared = (*it);
			return SUCCESS;
		}
	}
	
	return FAILED;
}

int share_manager::cost_shared_cash( const std::string& group_name, const std::string& merchant, const std::string& cost_name, float amount )
{
	std::list<shared_info>& shares = m_shared_map[merchant];
	for ( std::list<shared_info>::iterator it = shares.begin();
		it != shares.end(); )
	{
		if ( it->group == group_name )
		{
			if ( it->amount < amount )
				return FAILED;

			it->amount -= amount;
			return SUCCESS;
		}
	}

	return FAILED;
}

int share_manager::add_shared_cash( const std::string& group_name, const std::string& merchant, float amount )
{
	std::list<shared_info>& shares = m_shared_map[merchant];
	for ( std::list<shared_info>::iterator it = shares.begin();
		it != shares.end(); )
	{
		if ( it->group == group_name )
		{
			it->amount += amount;
		}
	}
	return SUCCESS;
}
