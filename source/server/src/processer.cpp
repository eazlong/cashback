#include "processer.h"
#include "datas.h"
#include "error_code.h"
#include "user_manager.h"
#include "codec.h"
#include "user.h"
#include <assert.h>

int processer::process( const std::string& message, std::string& ret_msg )
{
	if ( message.empty() )
	{
		return INVALID_PRAMETER;
	}

	void* decoded_data = m_codec->decode( message );
	if ( NULL == decoded_data )
	{
		ret_msg = "internal error!";
		//error_log( "decode message \"" << message << "\" failed!" );
		return DECODE_FAILED;
	}

	return process_data( decoded_data, ret_msg );
}



int regist_processer::process_data( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	user_info* info = (user_info*)data;
	int ret = user_manager::get_instance()->regist( info );
	if ( SUCCESS == ret )
	{
		user* usr = user_manager::get_instance()->get_user( info->name, info->type );
		assert( usr != NULL );
		ret_msg = m_codec->encode( usr );
		return SUCCESS;
	}

	return ret;
}

int login_processer::process_data( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	user_info* info = (user_info*)data;
	int ret = user_manager::get_instance()->login( info );
	if ( SUCCESS == ret )
	{
		user* usr = user_manager::get_instance()->get_user( info->name, info->type );
		assert( usr != NULL );
		ret_msg = m_codec->encode( usr );
		return SUCCESS;
	}

	return ret;
}

int logout_processer::process_data( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}
	user_info* info = (user_info*)data;
	return user_manager::get_instance()->logout( info );
}


int generate_request_processer::process_data( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	cashback_generate_request* request = (cashback_generate_request*)data;
	user* usr;
	int ret = user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	unsigned long bussiness_token;
	float cashbak;
	switch ( request->base.type )
	{
	case MERCHANT: //商户生成优惠券
		{
			merchant* mc = dynamic_cast<merchant*>(usr);
			if ( !mc->is_clerk_exist( request->clerk ) )
			{
				return CLERK_NOT_EXISTS;
			}

			ret = mc->get_cashback( request->cash, request->clerk, bussiness_token, cashbak );
			if ( SUCCESS != ret )
			{
				return ret;
			}

			cashback_generate_ack ack;
			ack.btoken = bussiness_token;
			ack.cashback = cashbak;
			ack.name = request->base.name;
			ack.type = MERCHANT;
			ret_msg = m_codec->encode( &ack );
		}
		break;
	case CUSTOMER://用户请求生成优惠券
		{
			user *u = user_manager::get_instance()->get_user( request->merchant_name, MERCHANT );
			if ( u == NULL )
			{
				return USER_NOT_FOUND;
			}

			merchant* mc = dynamic_cast<merchant*>(u);
			ret = mc->request_cashback( request->cash, request->clerk, request->base.name ); //请求生成优惠券
			if ( SUCCESS != ret )
			{
				return ret;
			}
		}
		break;
	}
	
	return SUCCESS;
}

int cashback_customer_confirm_processer::process_data( void* data, std::string& ret_msg )
{
	//身份验证
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	cashback_confirm_request* request = (cashback_confirm_request*)data;
	user* usr;
	int ret = user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	//商户认证
	user * u = user_manager::get_instance()->get_user( request->merchant_name, MERCHANT );
	if ( u == NULL )
	{
		return USER_NOT_FOUND;
	}

	merchant* mc = dynamic_cast<merchant*>( u );
	if ( !mc->complete_trade( request->cash, request->btoken ) )
	{
		return ILLEGAL_TRADE;
	}

	//消费完成
	float balance = usr->get_account()->add( request->cashback );

	ret_msg = m_codec->encode( (void*)&balance );
	//usr->add_log();
	return SUCCESS;
}

int cashback_merchant_confirm_processer::process_data( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	cashback_confirm_request* request = (cashback_confirm_request*)data;
	user* usr;
	int ret = user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	//用户认证
	user * u = user_manager::get_instance()->get_user( request->customer_name, CUSTOMER );
	if ( u == NULL )
	{
		return USER_NOT_FOUND;
	}
	
	//消费完成
	float balance = u->get_account()->add( request->cashback );
	
	return SUCCESS;
}
