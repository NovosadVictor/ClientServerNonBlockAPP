#ifndef _PHONE_H_
#define _PHONE_H_

#include <stdlib.h>
#include <string.h>


class Phone {
public:
    Phone();
    Phone(const char *phone);
	void SetRand();
	bool operator==(const Phone &phone);
	void operator=(const Phone &phone);
    const char *GetNumber() const;
private:
    char _number[14];
};

#endif
