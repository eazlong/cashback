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
	register_codec c;
	void* d = c.decode("");
	CPPUNIT_ASSERT( NULL == d );
	std::string msg = "user_name:zhangsam\r\npassword:******\r\n";
	d = c.decode( msg );
	CPPUNIT_ASSERT( d != NULL );
	delete (user_info*)d;
}
