#pragma once

class rule
{
public:
	rule(void);
	virtual ~rule(void);

	virtual int get_cashback( float cash, float& cashback ) = 0;
};

class discount : public rule
{
public:
	discount(void) {}
	virtual ~discount(void){}

	int get_cashback( float cash, float& cashback );
};
