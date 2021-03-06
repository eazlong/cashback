#pragma once

#include <cppunit/extensions/HelperMacros.h>

class user_manager_test : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(user_manager_test);
	CPPUNIT_TEST( test_regist );
	CPPUNIT_TEST( test_login );
	CPPUNIT_TEST( test_logout );
	CPPUNIT_TEST_SUITE_END();

public:
	user_manager_test(void);
	~user_manager_test(void);

	void test_regist();
	void test_login();
	void test_logout();
};
