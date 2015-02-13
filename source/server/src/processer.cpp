#include "processer.h"
#include "datas.h"
#include "error_code.h"
#include "user_manager.h"
#include "codec.h"
#include "user.h"
#include "log.h"
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
		error_log( "decode message \"" << message << "\" failed!" );
		return DECODE_FAILED;
	}

	int ret = main_process( decoded_data, ret_msg );

	m_codec->release_buffer( decoded_data );
	
	return ret;
}



int regist_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	user_info* info = (user_info*)data;
	int ret = online_user_manager::get_instance()->regist( info );
	if ( SUCCESS == ret )
	{
		user* usr = online_user_manager::get_instance()->get_user( info->name, info->type );
		assert( usr != NULL );
		ret_msg = m_codec->encode( usr );
		return SUCCESS;
	}

	return ret;
}

int login_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	user_info* info = (user_info*)data;
	int ret = online_user_manager::get_instance()->login( info );
	if ( SUCCESS == ret )
	{
		user* usr = online_user_manager::get_instance()->get_user( info->name, info->type );
		assert( usr != NULL );
		ret_msg = m_codec->encode( usr );
		return SUCCESS;
	}

	return ret;
}

int logout_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}
	user_info* info = (user_info*)data;
	return online_user_manager::get_instance()->logout( info );
}


int generate_request_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	cashback_request* request = (cashback_request*)data;
	user* usr;
	int ret = online_user_manager::get_instance()->verify( request->base, &usr );
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
			user *u = online_user_manager::get_instance()->get_user( request->merchant_name, MERCHANT );
			if ( u == NULL )
			{
				return USER_NOT_FOUND;
			}

			merchant* mc = dynamic_cast<merchant*>(u);
			ret = mc->request_cashback( request->cash, request->ttype, request->clerk, request->base.name ); //请求生成优惠券
			if ( SUCCESS != ret )
			{
				return ret;
			}
		}
		break;
	}
	
	return SUCCESS;
}

int cashback_customer_confirm_processer::main_process( void* data, std::string& ret_msg )
{
	//身份验证
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	cashback_confirm_request* request = (cashback_confirm_request*)data;
	user* usr;
	int ret = online_user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	//商户认证
	user * u = online_user_manager::get_instance()->get_user( request->merchant_name, MERCHANT );
	if ( u == NULL )
	{
		return USER_NOT_FOUND;
	}

	merchant* mc = dynamic_cast<merchant*>( u );
	if ( u->get_token() == request->btoken )
	{
		float balance = usr->get_account()->sub( request->cash, request->merchant_name );
		ret_msg = m_codec->encode( (void*)&balance );
		//usr->add_log();
		return SUCCESS;
	}
	else
	{
		if ( !mc->complete_trade( request->cash, request->btoken ) )
		{
			return ILLEGAL_TRADE;
		}

		//消费完成
		float balance = usr->get_account()->add( request->cashback, request->merchant_name );

		ret_msg = m_codec->encode( (void*)&balance );
		//usr->add_log();
		return SUCCESS;
	}
	
}

int cashback_merchant_confirm_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}
	
	//商户认证
	cashback_confirm_request* request = (cashback_confirm_request*)data;
	user* usr;
	int ret = online_user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	//用户认证
	user * u = online_user_manager::get_instance()->get_user( request->customer_name, CUSTOMER );
	if ( u == NULL )
	{
		return USER_NOT_FOUND;
	}
	
	//交易完成
	switch ( request->ttype )
	{
	case CASHBACK_GENERATE:
		u->get_account()->add( request->cashback, request->merchant_name );
		break;
	case CASHBACK_COST:
		customer * c = (customer *)u;
		c->finish_trade();
		break;
	}
	
	return SUCCESS;
}

int get_reqesting_trade_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	get_reqeusting_trade_request* request = (get_reqeusting_trade_request*)data;
	user* usr;
	int ret = online_user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	assert( request->base.type == MERCHANT );
	merchant* mc = dynamic_cast<merchant*>( usr );
	trade t;
	if ( !mc->get_requesting_trade( request->clerk, t ) )
	{
		return ILLEGAL_TRADE;
	}
	
	ret_msg = m_codec->encode( &t );
	return SUCCESS;
}

int refresh_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	cashback_confirm_request* request = (cashback_confirm_request*)data;
	user* usr;
	int ret = online_user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	assert( request->base.type == CUSTOMER );

	float balance = usr->get_account()->get_balance( request->merchant_name );

	ret_msg = m_codec->encode( (void*)&balance );

	return SUCCESS;
}

int cost_cashback_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	cashback_request* request = (cashback_request*)data;
	user* usr;
	int ret = online_user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}
	
	//获取用户余额
	float balance = usr->get_account()->get_balance( request->merchant_name );
	if ( balance < request->cash )
	{
		return INSUFFICIENT_BALANCE;
	}

	//商户认证
	user * u = online_user_manager::get_instance()->get_user( request->merchant_name, MERCHANT );
	if ( u == NULL )
	{
		return USER_NOT_FOUND;
	}

	//共享券消费
	std::map<std::string, shared_info*> already;
	float cash = 0.0;
	if ( !request->shares.empty() )
	{
		for ( std::map<std::string, shared_info>::iterator it = request->shares.begin();
			it != request->shares.end(); it++ )
		{
			if ( !cost_shares( request->base.name, it->first, it->second ) ) 
			{
				cancel_trade( already );
				return ILLEGAL_TRADE;
			}

			already.insert( make_pair(it->first, &it->second) );
			cash += it->second.amount;
		}
	}
	
	//请求消费，等待商户确认
	merchant* mc = dynamic_cast<merchant*>(u);
	ret = mc->request_cashback( request->cash+cash, request->ttype, request->clerk, request->base.name ); 
	if ( SUCCESS != ret )
	{
		cancel_trade( already );
		return ret;
	}

	customer* c = (customer*)usr;
	c->trading( request );

	return SUCCESS;
}

bool cost_cashback_processer::cancel_trade( std::map<std::string, shared_info*>& already )
{
	//如果交易中有优惠券验证失败，回滚，取消这次交易。
	for ( std::map<std::string, shared_info*>::iterator it_already = already.begin();
		it_already != already.end(); it_already ++ )
	{
		user* f = online_user_manager::get_instance()->get_user( it_already->first, CUSTOMER );
		if ( f == NULL )
		{
			return false;
		}

		f->get_shared_manager()->add_shared_cash(it_already->second->group, it_already->second->merchant, it_already->second->amount );
	}
	return true;
}

bool cost_cashback_processer::cost_shares( const std::string& name, const std::string& friend_name, shared_info& shares )
{
	user* f = online_user_manager::get_instance()->get_user( friend_name, CUSTOMER );
	if ( f == NULL )
		return false;
	std::string group = f->get_friends_manager()->get_group( name );
	if ( group.empty() )
		return false;
	shares.group = group;
	shared_info info;
	if ( SUCCESS != f->get_shared_manager()->get_shared_cash(group, shares.merchant, info ) )
		return false;

	if ( info.amount < shares.amount )
		return false;
	
	if ( SUCCESS != f->get_shared_manager()->cost_shared_cash( group, shares.merchant, name, shares.amount ) )
		return false;

	return true;
}

int friends_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	friend_manager_request* request = (friend_manager_request*)data;
	user* usr;
	int ret = online_user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	if ( request->base.type == MERCHANT )
	{
		ret_msg = "merchant doesn't support friend manager yet";
		return OPERATION_UNSUPPORT;
	}

	customer* cst = (customer*)usr;
	switch ( request->otype )
	{
	case ADD:
		if ( !cst->get_friends_manager()->add_friend( request->friends ) )
			ret = USER_EXISTS;
		break;
	case DEL:
		if ( !cst->get_friends_manager()->del_friend( request->friends.name ) )
			ret = USER_NOT_FOUND;
		break;
	case GET:
		{
			std::list<friend_info> friends;
			cst->get_friends_manager()->get_friends( "", friends );
			ret_msg = m_codec->encode( &friends );
		}
		break;
	default:
		ret_msg = "operation type error!";
		return OPERATION_UNSUPPORT;
	}

	return ret;
}

int shared_processer::main_process( void* data, std::string& ret_msg )
{
	if ( NULL == data )
	{
		return INVALID_PRAMETER;
	}

	shared_manager_request* request = (shared_manager_request*)data;
	user* usr;
	int ret = online_user_manager::get_instance()->verify( request->base, &usr );
	if ( SUCCESS != ret )
	{
		return ret;
	}

	if ( request->base.type == MERCHANT )
	{
		ret_msg = "merchant doesn't support shared manager yet";
		return OPERATION_UNSUPPORT;
	}
	
	customer* cst = (customer*)usr;
	switch ( request->otype )
	{
	case ADD:
		ret = cst->get_shared_manager()->share( request->shared );
		if ( ret == SUCCESS )
		{
			cst->get_account()->sub( request->shared.amount, request->shared.merchant );
		}
		break;
	case DEL:
		ret = cst->get_shared_manager()->cancel_share( request->shared.group, request->shared.merchant );
		if ( ret == SUCCESS )
		{
			cst->get_account()->add( request->shared.amount, request->shared.merchant );
		}
		break;
	case GET:
		{
			std::map< std::string,std::list<shared_info> > shares_map;
			if ( request->get_from == "self" )
			{
				std::list<shared_info> shares;
				ret = cst->get_shared_manager()->get_shared_cash( request->shared.merchant, shares );
				shares_map.insert( make_pair( request->base.name, shares ) );
			}
			else if ( request->get_from == "friends" )
			{
				//取得好友列表
				std::list<friend_info> friends;
				if ( !cst->get_friends_manager()->get_friends( "", friends ) )
					return FAILED;
				for ( std::list<friend_info>::iterator it = friends.begin(); 
					it != friends.end(); it ++ )
				{
					std::list<shared_info> shares;
					user* f = online_user_manager::get_instance()->get_user( it->name, CUSTOMER );
					if ( f == NULL )
					{
						continue;
					}
					std::string group = f->get_friends_manager()->get_group( request->base.name );
					if ( group == "" )
					{
						continue;
					}
					shared_info info;
					if ( SUCCESS == f->get_shared_manager()->get_shared_cash( group, request->shared.merchant, info ) )
					{
						shares.push_back( info );
					}
					shares_map.insert( make_pair( it->name, shares ) );
				}
			}
			ret_msg = m_codec->encode( &shares_map );
		}
		break;
	default:
		ret_msg = "operation type error!";
		return OPERATION_UNSUPPORT;
	}

	return ret;
}
