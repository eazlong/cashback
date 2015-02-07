#include "codec_test.h"
#include "../src/codec.h"
#include "../src/datas.h"

CPPUNIT_TEST_SUITE_REGISTRATION( codec_test );

codec_test::codec_test(void)
{
}

codec_test::~codec_test(void)
{
}

void codec_test::test_regester_codec()
{
	user_info_codec c;
	void* d = c.decode("");
	CPPUNIT_ASSERT( NULL == d );
	std::string msg = "user_name:zhangsam\npassword:******\ntype:1\ntoken:10000";
	d = c.decode( msg );
	CPPUNIT_ASSERT( d != NULL );
	user_info* info = (user_info*)d;
	CPPUNIT_ASSERT( info->name == "zhangsam" );
	CPPUNIT_ASSERT( info->password == "******" );
	CPPUNIT_ASSERT( info->token == 10000 );
	delete info;
}

void codec_test::test_generate_request_codec()
{
	cashback_generate_request_codec gc;
	void* d = gc.decode("");
	CPPUNIT_ASSERT( NULL == d );
	std::string msg = "user_name:wangwu\ntype:1\ntoken:10000\ncash:100.00\nclerk:xiaoming";
	d = gc.decode( msg );
	CPPUNIT_ASSERT( d != NULL );
	cashback_generate_request* info = (cashback_generate_request*)d;
	CPPUNIT_ASSERT( info->base.name == "wangwu" );
	CPPUNIT_ASSERT( info->base.type == MERCHANT );
	CPPUNIT_ASSERT( info->base.token == 10000 );
	CPPUNIT_ASSERT( info->cash == 100.00 );
	CPPUNIT_ASSERT( info->clerk == "xiaoming" );
	delete info;
}

