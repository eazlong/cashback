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
	
	callback_generate_request_codec grc;
	generate_request_processer grp( &grc );
	center->add_processer( "generator_request", &grp );

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

	pos = ret.find(":");
	std::string merchant_token = ret.substr( pos+1 );
	msg = "user_name:wangwu\ntype:1\ntoken:";
	msg += merchant_token;
	msg += "\ncash:100.00\nclerk:xiaoming";
	CPPUNIT_ASSERT( center->process( "generator_request", msg, ret ) == SUCCESS );
	
	pos = ret.find("btoken:");
	size_t pos2 = ret.find( '\n', pos );
	std::string btoken = ret.substr( pos+strlen("btoken:"), pos2-pos-strlen("btoken:") );
	
	CPPUNIT_ASSERT( center->process( "register", "user_name:zhaoliu\npassword:******\ntype:0", ret ) == SUCCESS );
	pos = ret.find(":");
	token = ret.substr( pos+1 );
	
	callback_confirm_request_codec ccrc;
	cashback_customer_confirm_processer ccp( &ccrc );
	center->add_processer( "cashback_confirm", &ccp );

	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	msg += "\nmerchant:wangwu\ncash:100.00\ncashback:10.00\nbtoken:" + btoken;
	CPPUNIT_ASSERT( center->process( "cashback_confirm", msg, ret ) == SUCCESS );

	msg = "user_name:zhaoliu\ntype:0\ntoken:" + token;
	msg += "\nmerchant:wangwu\ncash:90.00\n";
	CPPUNIT_ASSERT( center->process( "generator_request", msg, ret ) == SUCCESS );
	
	msg = "user_name:wangwu\ntype:1\ntoken:";
	msg += merchant_token;
	msg += "\nclerk:xiaoming";
	CPPUNIT_ASSERT( center->process( "get_reqesting_trade", msg, ret ) == SUCCESS );

	CPPUNIT_ASSERT( center->process( "merchant_confirm", msg, ret ) == SUCCESS );

	CPPUNIT_ASSERT( center->process( "refresh", msg, ret ) == SUCCESS );
	
	CPPUNIT_ASSERT( center->remove_processer( "register" ) );
	delete cc;
	delete lip;
	delete lop;
	delete rp;
}

