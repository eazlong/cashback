#include <cppunit/extensions/HelperMacros.h>

class process_center_test : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(process_center_test);
	CPPUNIT_TEST( test_add_proccesser );
	CPPUNIT_TEST( test_process );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test_add_proccesser();
	void test_process();
};
