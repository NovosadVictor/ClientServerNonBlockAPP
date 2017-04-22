#ifndef _SELECT_H_
#define _SELECT_H_

#include "date.h"
#include "phone.h"


class SELECT {
public:
	SELECT();

	SELECT(Phone phone, int service, Date dateMin, Date dateMax, int sumMin, int sumMax);

	Phone GetPhone() const;

	int GetService() const;

	Date GetMinDate() const;

	Date GetMaxDate() const;

	int GetMinSum() const;

	int GetMaxSum() const;

private:
	Phone _phone;
	int _service;
	Date _dateMin;
	Date _dateMax;
	int _sumMin;
	int _sumMax;
};


#endif
