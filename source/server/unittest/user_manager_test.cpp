#include "user_manager_test.h"
#include "../src/error_code.h"
#include "../src/user_manager.h"
#include "../src/persistance.h"

CPPUNIT_TEST_SUITE_REGISTRATION( user_manager_test );

user_manager_test::user_manager_test(void)
{
	persistance* p = new db( "127.0.0.1", "root", "3721", "cashback", 3306 );//new memory_persistance();
	user_manager::get_instance()->set_persistance( p );
}

user_manager_test::~user_manager_test(void)
{
	persistance* p = user_manager::get_instance()->set_persistance( NULL );
	delete p;
}

void user_manager_test::test_regist()
{
	CPPUNIT_ASSERT( INVALID_PRAMETER == user_manager::get_instance()->regist( NULL ) );
	user_info info( "zhangsam", CUSTOMER, "******" );
	CPPUNIT_ASSERT( SUCCESS == user_manager::get_instance()->regist( &info ) );
	CPPUNIT_ASSERT( USER_EXISTS == user_manager::get_instance()->regist( &info ) );
}

void user_manager_test::test_login()
{
	CPPUNIT_ASSERT( INVALID_PRAMETER == user_manager::get_instance()->login( NULL ) );
	user_info info( "zhangsam", CUSTOMER, "******" );
	CPPUNIT_ASSERT( SUCCESS == user_manager::get_instance()->login( &info ) );
	CPPUNIT_ASSERT( SUCCESS == user_manager::get_instance()->login( &info ) );
	info.password = "*******";
	CPPUNIT_ASSERT( PASSWORD_ERROR == user_manager::get_instance()->login( &info ) );
	info.name = "lisi";
	CPPUNIT_ASSERT( USER_NOT_FOUND == user_manager::get_instance()->login( &info ) );
	info.name = "zhangsam";
	info.password = "******";
	info.type = MERCHANT;
	CPPUNIT_ASSERT( USER_NOT_FOUND == user_manager::get_instance()->login( &info ) );
	CPPUNIT_ASSERT( SUCCESS == user_manager::get_instance()->regist( &info ) );
	CPPUNIT_ASSERT( SUCCESS == user_manager::get_instance()->login( &info ) );
}

void user_manager_test::test_logout()
{
	user_info info( "" );
	CPPUNIT_ASSERT( USER_NOT_FOUND == user_manager::get_instance()->logout( &info ) );
	info.name = "zhangsam";
	//info.token = 10002;
	CPPUNIT_ASSERT( TOKEN_ERROR == user_manager::get_instance()->logout( &info ) );
	info.name = "lisi";
	CPPUNIT_ASSERT( USER_NOT_FOUND == user_manager::get_instance()->logout( &info ) );
	info.name = "zhangsam";
	info.type = MERCHANT;
	CPPUNIT_ASSERT( TOKEN_ERROR == user_manager::get_instance()->logout( &info ) );
}

