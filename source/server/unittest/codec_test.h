#pragma once

#include <cppunit/extensions/HelperMacros.h>

class codec_test : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(codec_test);
	CPPUNIT_TEST( test_regester_codec );
	CPPUNIT_TEST( test_generate_request_codec );
	CPPUNIT_TEST_SUITE_END();

public:
	codec_test(void);
	~codec_test(void);
	
	void test_regester_codec();
	void test_generate_request_codec();
};
