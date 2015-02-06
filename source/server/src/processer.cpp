#include "processer.h"
#include "datas.h"
#include "user_manager.h"

int register_processer::process( void* data )
{
	if ( NULL == data )
		return -1;
	
	user_info* info = (user_info*)data;
	
	return user_manager::get_instance()->regist( info );
}
