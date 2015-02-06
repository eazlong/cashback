#pragma once

#include <string>

enum user_type
{
	CUSTOMER,
	MERCHANT
};

typedef struct user_info_t
{
	std::string name;
	std::string password;
	user_type   type;
}user_info;