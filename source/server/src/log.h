#pragma once

#ifdef USE_DEF_LOG
#else
#include <iostream>
using namespace std;

#define debug( str ) cout << str << endl;
#define error( str ) cerr << str << endl;

#endif