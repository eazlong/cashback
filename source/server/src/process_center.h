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
	bool add_processer( const std::string& key, processer* msg_processer );
	bool remove_processer( const std::string& key );

	int process( const std::string& key, const std::string& message, std::string& ret_msg );
protected:
	//for singleton.
	process_center(void);
	process_center( const process_center& other );

private:
	static process_center* m_instance;
	
	typedef std::map<std::string, processer*> processer_map;
	processer_map m_processer_map;
};
