#include "process_center_test.h"
#include "../src/process_center.h"
#include "../src/codec.h"
#include "../src/processer.h"
#include "../src/error_code.h"
#include "../src/log.h"


CPPUNIT_TEST_SUITE_REGISTRATION(process_center_test);

void process_center_test::setUp( void )
{
	
}

void process_center_test::tearDown( void )
{
	
}

void process_center_test::test_add_proccesser()
{
	process_center* center = process_center::get_instance();
	CPPUNIT_ASSERT( center != NULL );
	CPPUNIT_ASSERT( !center->add_processer( "register", NULL ) );
	codec* c = new user_info_codec;
	processer* p = new regist_processer(c);
	CPPUNIT_ASSERT( !center->add_processer( "", p ) );
	CPPUNIT_ASSERT( center->add_processer( "register", p ) );
	CPPUNIT_ASSERT( !center->add_processer( "register", p ) );
	CPPUNIT_ASSERT( center->remove_processer( "register" ) );
	delete c;
	delete p;
}

void process_center_test::test_process()
{
	process_center* center = process_center::get_instance();
	user_info_codec* cc = new user_info_codec;
	regist_processer* rp = new regist_processer( cc );
	login_processer* lip = new login_processer( cc );
	logout_processer* lop = new logout_processer( cc );
	center->add_processer( "register", rp );
	center->add_processer( "login", lip );
	center->add_processer( "logout", lop );
	
	std::string ret;
	CPPUNIT_ASSERT( center->process( "register", "user_name:wangwu\npassword:******\ntype:1", ret ) == SUCCESS );
	debug_log( "\n" << ret );
	
	size_t pos = ret.find(":");
	std::string token = ret.substr( pos+1 );
	std::string msg = "user_name:wangwu\ntype:1\ntoken:";
	msg += token;
	CPPUNIT_ASSERT( center->process( "logout", msg, ret ) == SUCCESS );

	CPPUNIT_ASSERT( center->process( "login", "user_name:wangwu\npassword:******\ntype:1", ret ) == SUCCESS );
	CPPUNIT_ASSERT( center->process( "login", "user_name:wangwu\npassword:******\ntype:1", ret ) == SUCCESS );

	cashback_generate_request_codec grc;
	generate_request_processer grp( &grc );
	center->add_processer( "generator_request", &grp );
	//商户请求生成
	pos = ret.find(":");
	std::string merchant_token = ret.substr( pos+1 );
	msg = "user_name:wangwu\ntype:1\ntoken:";
	msg += merchant_token;
	msg += "\ncash:100.00\nclerk:xiaoming";
	CPPUNIT_ASSERT( center->process( "generator_request", msg, ret ) == SUCCESS );
	
	//用户注册
	pos = ret.find("btoken:");
	size_t pos2 = ret.find( '\n', pos );
	std::string btoken = ret.substr( pos+strlen("btoken:"), pos2-pos-strlen("btoken:") );
	CPPUNIT_ASSERT( center->process( "register", "user_name:zhaoliu\npassword:******\ntype:0", ret ) == SUCCESS );

	//用户扫描
	pos = ret.find(":");
	token = ret.substr( pos+1 );
	
	cashback_confirm_request_codec ccrc;
	cashback_customer_confirm_processer ccp( &ccrc );
	center->add_processer( "customer_confirm", &ccp );

	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	msg += "\nmerchant:wangwu\ncash:100.00\ncashback:10.00\nbtoken:" + btoken;
	CPPUNIT_ASSERT( center->process( "customer_confirm", msg, ret ) == SUCCESS );
	CPPUNIT_ASSERT( ret == "balance:10" );

	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	msg += "\nmerchant:wangwu\ncash:90.00\nclerk:xiaoming\ntradetype:0";
	CPPUNIT_ASSERT( center->process( "generator_request", msg, ret ) == SUCCESS );
	
	msg = "user_name:wangwu\ntype:1\ntoken:";
	msg += merchant_token;
	msg += "\nclerk:xiaoming";

	get_requesting_trade_codec grtc;
	get_reqesting_trade_processer grtp( &grtc );
	center->add_processer( "get_reqesting_trade", &grtp );
	CPPUNIT_ASSERT( center->process( "get_reqesting_trade", msg, ret ) == SUCCESS );
	
	msg = "user_name:wangwu\ntype:1\ntoken:";
	msg += merchant_token;
	msg += "\nclerk:xiaoming";
	msg += ret;
	cashback_merchant_confirm_processer cmcp( &ccrc );
	center->add_processer( "merchant_confirm", &cmcp );
	CPPUNIT_ASSERT( center->process( "merchant_confirm", msg, ret ) == SUCCESS );

	refresh_processer rfp( &ccrc );
	center->add_processer( "refresh", &rfp );
	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	CPPUNIT_ASSERT( center->process( "refresh", msg, ret ) == SUCCESS );
	CPPUNIT_ASSERT( ret == "balance:19" );

	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	msg += "\nmerchant:wangwu\ncash:10.00\n\nclerk:xiaoming\ntradetype:1";
	cost_cashback_processer costp( &grc );
	center->add_processer( "cost_cashback", &costp );
	CPPUNIT_ASSERT( center->process( "cost_cashback", msg, ret ) == SUCCESS );

	msg = "user_name:wangwu\ntype:1\ntoken:";
	msg += merchant_token;
	msg += "\nclerk:xiaoming";
 	CPPUNIT_ASSERT( center->process( "get_reqesting_trade", msg, ret ) == SUCCESS );
 
 	msg = "user_name:wangwu\ntype:1\ntoken:" + token + "\n" + ret;
 	CPPUNIT_ASSERT( center->process( "merchant_confirm", msg, ret ) == SUCCESS );

	center->add_processer( "refresh", &rfp );
	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	CPPUNIT_ASSERT( center->process( "refresh", msg, ret ) == SUCCESS );
	CPPUNIT_ASSERT( ret == "balance:9" );

	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	msg += "\nmerchant:wangwu\ncash:5.00\nbtoken:";
	msg += merchant_token;

	CPPUNIT_ASSERT( center->process( "customer_confirm", msg, ret ) == SUCCESS );
	CPPUNIT_ASSERT( ret == "balance:4" );

	CPPUNIT_ASSERT( center->process( "register", "user_name:zhouyi\npassword:******99\ntype:0", ret ) == SUCCESS );
	pos = ret.find(":");
	std::string zhouyi_token = ret.substr( pos+1 );

	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	msg += "\nfriend_name:zhouyi\nnickname:zy\ngroup:g1\noperation:0";
	friend_manager_codec fmc;
	friends_processer fp(&fmc);
	center->add_processer( "friends_manager", &fp );

	CPPUNIT_ASSERT( center->process( "friends_manager", msg, ret ) == SUCCESS );
	msg = "user_name:zhouyi\ntype:0\ntoken:" + zhouyi_token;
	msg += "\nfriend_name:zhaoliu\nnickname:zl\ngroup:g1\noperation:0";
	CPPUNIT_ASSERT( center->process( "friends_manager", msg, ret ) == SUCCESS );

	shared_manager_codec smc;
	shared_processer sp( &smc );
	center->add_processer( "shared_manager", &sp );
	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	msg += "\nmerchant:wangwu\ncash:3.00\ngroup:g1\noperation:0";
	CPPUNIT_ASSERT( center->process( "shared_manager", msg, ret ) == SUCCESS );
	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	CPPUNIT_ASSERT( center->process( "refresh", msg, ret ) == SUCCESS );
	CPPUNIT_ASSERT( ret == "balance:1" );

	msg = "user_name:zhouyi\ntype:0\ntoken:" + zhouyi_token;
	msg += "\nmerchant:wangwu\noperation:2\nget:friends";
	CPPUNIT_ASSERT( center->process( "shared_manager", msg, ret ) == SUCCESS );
	
	msg = "user_name:zhouyi\ntype:0\ntoken:" + zhouyi_token;
	msg += "\nmerchant:wangwu\ncash:0.00\nclerk:xiaoming\ntradetype:1\n";
	msg += ret;
	CPPUNIT_ASSERT( center->process( "cost_cashback", msg, ret ) == SUCCESS );
	
	msg = "user_name:wangwu\ntype:1\ntoken:";
	msg += merchant_token;
	msg += "\nclerk:xiaoming";
	CPPUNIT_ASSERT( center->process( "get_reqesting_trade", msg, ret ) == SUCCESS );

	msg = "user_name:wangwu\ntype:1\ntoken:" + token + "\n" + ret;
	CPPUNIT_ASSERT( center->process( "merchant_confirm", msg, ret ) == SUCCESS );

	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	CPPUNIT_ASSERT( center->process( "refresh", msg, ret ) == SUCCESS );
	CPPUNIT_ASSERT( ret == "balance:1" );

 	CPPUNIT_ASSERT( center->remove_processer( "register" ) );

	delete cc;
	delete lip;
	delete lop;
	delete rp;
}

