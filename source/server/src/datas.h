#pragma once

#include <string>

enum user_type
{
	CUSTOMER,
	MERCHANT
};

typedef struct user_info_t
{
	unsigned long id;
	std::string name;
	std::string password;
	std::string address;
	std::string tel;
	user_type   type;
}user_info;

