#include "processer_test.h"
#include "../src/datas.h"
#include "../src/processer.h"

CPPUNIT_TEST_SUITE_REGISTRATION( processer_test );

processer_test::processer_test(void)
{
}

processer_test::~processer_test(void)
{
}

void processer_test::test_register_process()
{
	register_processer p;
	CPPUNIT_ASSERT( -1 == p.process( NULL ) );
	user_info info;
	info.name = "zhangsam";
	info.password = "******";
	info.type = CUSTOMER;
	CPPUNIT_ASSERT( 0 == p.process( &info ) );
}
