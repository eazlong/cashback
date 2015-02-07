#pragma once

#ifdef USE_DEF_LOG
#else
#include <iostream>
using namespace std;

#define debug_log( str ) cout << str << endl;
#define error_log( str ) cerr << str << endl;

#endif