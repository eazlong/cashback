#include "process_center.h"
#include "codec.h"
#include "processer.h"
#include "log.h"

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

bool process_center::add_processer( const std::string& key, codec* msg_codec, processer* msg_processer )
{
	if( key.empty() || msg_codec == NULL || msg_processer == NULL )
		return false;

	std::pair<processer_map::iterator, bool> result = m_processer_map.insert( std::make_pair(key, std::make_pair(msg_codec, msg_processer) ) );
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


int process_center::process( const std::string& key, const std::string& message )
{
	processer_map::iterator it = m_processer_map.find( key );
	if ( it == m_processer_map.end() )
	{
		error( "Get processer with key \"" << key << "\" failed!" );
		return -1;
	}
	
	codec* msg_codec = it->second.first;
	void* decoded_data = msg_codec->decode( message );
	if ( NULL == decoded_data )
	{
		error( "decode message \"" << message << "\" with key \"" << key << "\" failed!" );
		return -1;
	}

	processer* msg_processer = it->second.second;
	return msg_processer->process( decoded_data );
}

