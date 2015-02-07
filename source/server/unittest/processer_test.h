#pragma once

#include <cppunit/extensions/HelperMacros.h>

class processer_test : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(processer_test);
	CPPUNIT_TEST( test_register_process );
	CPPUNIT_TEST( test_generate_request_process );
	CPPUNIT_TEST_SUITE_END();

public:
	processer_test(void);
	~processer_test(void);

	void test_register_process();
	void test_generate_request_process();
};
