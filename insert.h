#ifndef _INSERT_H
#define _INSERT_H

#include "phone.h"
#include "date.h"


class INSERT {
public:
    INSERT();
    INSERT(Phone phone, int service, Date date, double sum);
    Phone GetPhone() const;
    int GetService() const;
    Date GetDate() const;
    double GetSum() const;
private:
    Phone _phone;
    int _service;
    Date _date;
    double _sum;
};


#endif
