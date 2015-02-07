#include "rule.h"
#include "error_code.h"

rule::rule(void)
{
}

rule::~rule(void)
{
}

int discount::get_cashback( float cash, float& cashback )
{
	cashback = cash*(float)0.10;
	return SUCCESS;
}
