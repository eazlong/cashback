#include "codec.h"
#include "datas.h"

void* register_codec::decode( const std::string& message )
{
	if ( message.empty() )
		return NULL;

	user_info * info = new user_info;
	
	return info;
}
