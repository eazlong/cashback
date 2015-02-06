#pragma once

#include <cppunit/extensions/HelperMacros.h>

class user_test : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(user_test);
	CPPUNIT_TEST_SUITE_END();

public:
	user_test(void);
	~user_test(void);
};
