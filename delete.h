#ifndef _DELETE_H
#define _DELETE_H

#include "date.h"
#include "phone.h"


class DELETE {
public:
    DELETE();
    DELETE(int service, Phone phone, Date dateMin, Date dateMax, int sumMin, int sumMax);
    int GetService() const;
    Date GetMinDate() const;
    Date GetMaxDate() const;
    Phone GetPhone() const;
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
