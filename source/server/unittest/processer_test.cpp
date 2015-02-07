#include "processer_test.h"
#include "../src/datas.h"
#include "../src/processer.h"
#include "../src/persistance.h"
#include "../src/user_manager.h"
#include "../src/error_code.h"
#include "../src/codec.h"

CPPUNIT_TEST_SUITE_REGISTRATION( processer_test );

processer_test::processer_test(void)
{
}

processer_test::~processer_test(void)
{
}

void processer_test::test_register_process()
{
	user_info_codec cc;
	regist_processer p(&cc);
	std::string ret;
	CPPUNIT_ASSERT( INVALID_PRAMETER == p.process( "", ret) );
	std::string msg = "user_name:zhangsam1\npassword:******\ntype:1\n";
	CPPUNIT_ASSERT( SUCCESS == p.process( msg, ret ) );
}

void processer_test::test_generate_request_process()
{
	cashback_generate_request_codec cc;
	generate_request_processer p(&cc);
	std::string ret;
	CPPUNIT_ASSERT( INVALID_PRAMETER == p.process( "", ret) );
	std::string msg = "user_name:wangwu\ntype:1\ntoken:10000\ncash:100.00\nclerk:xiaoming";;
	CPPUNIT_ASSERT( SUCCESS == p.process( msg, ret ) );
}
