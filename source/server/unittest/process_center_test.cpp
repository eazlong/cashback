#include "process_center_test.h"
#include "../src/process_center.h"
#include "../src/codec.h"
#include "../src/processer.h"


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
	CPPUNIT_ASSERT( !center->add_processer( "register", NULL, NULL ) );
	codec* c = new register_codec;
	processer* p = new register_processer;
	CPPUNIT_ASSERT( !center->add_processer( "", c, p ) );
	CPPUNIT_ASSERT( center->add_processer( "register", c, p ) );
	CPPUNIT_ASSERT( !center->add_processer( "register", c, p ) );
	CPPUNIT_ASSERT( center->remove_processer( "register" ) );
	delete c;
	delete p;
}

void process_center_test::test_process()
{
	process_center* center = process_center::get_instance();
	center->add_processer( "register", new register_codec, new register_processer );

	center->process( "register", "" );
}

