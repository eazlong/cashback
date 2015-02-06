#pragma once
#include <map>
#include <string>

class codec;
class processer;

class process_center
{
public:
	virtual ~process_center(void);
	static process_center* get_instance();
	
	//for add or remove system modules dynamic.
	//key: the message type
	bool add_processer( const std::string& key, codec* msg_codec, processer* msg_processer );
	bool remove_processer( const std::string& key );

	int process( const std::string& key, const std::string& message );
protected:
	//for singleton.
	process_center(void);
	process_center( const process_center& other );

private:
	static process_center* m_instance;
	
	typedef std::map<std::string, std::pair<codec*, processer*> > processer_map;
	processer_map m_processer_map;
};
