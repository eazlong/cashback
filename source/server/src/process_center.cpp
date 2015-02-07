#include "process_center.h"
#include "codec.h"
#include "processer.h"
#include "log.h"
#include "error_code.h"

process_center* process_center::m_instance = new process_center();

process_center::process_center(void)
{
}

process_center::~process_center(void)
{
}

process_center* process_center::get_instance()
{
	return m_instance;
}

bool process_center::add_processer( const std::string& key, processer* msg_processer )
{
	if( key.empty() || msg_processer == NULL )
		return false;

	std::pair<processer_map::iterator, bool> result = m_processer_map.insert( std::make_pair( key, msg_processer ) );
	return result.second;
}

bool process_center::remove_processer( const std::string& key )
{
	if ( key.empty() )
		return false;

	processer_map::iterator it = m_processer_map.find( key );
	if ( it != m_processer_map.end() )
	{
		m_processer_map.erase( it );
		return true;
	}

	return false;
}


int process_center::process( const std::string& key, const std::string& message, std::string& ret_msg )
{
	processer_map::iterator it = m_processer_map.find( key );
	if ( it == m_processer_map.end() )
	{
		ret_msg = "internal error!";
		error_log( "Get processer with key \"" << key << "\" failed!" );
		return INVALID_PRAMETER;
	}
	
	processer* msg_processer = it->second;
	int ret = msg_processer->process( message, ret_msg );
	if ( ret != SUCCESS )
	{
		error_log( "process message \"" << message << "\" with key \"" << key << "\" failed!" );
		return ret;
	}
	
}

