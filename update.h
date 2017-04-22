#ifndef _UPDATE_H
#define _UPDATE_H

#include "date.h"
#include "phone.h"


class UPDATE {
public:
    UPDATE();
    UPDATE(double sum, int service,
           Phone phone,
           int serviceFilter,
           Phone phoneFilter,
           Date dateMinFilter,
           Date dateMaxFilter,
           int sumMinFilter,
           int sumMaxFilter
    );
    double GetSum() const;
    int GetService() const;
    Phone GetPhone() const;
    int GetFilterService() const;
    Phone GetFilterPhone() const;
    Date GetFilterDateMin() const;
    Date GetFilterDateMax() const;
    int GetFilterSumMin() const;
    int GetFilterSumMax() const;
private:
    double _sum;
    int _service;
    Phone _phone;
    int _serviceFilter;
    Phone _phoneFilter;
    Date _dateMinFilter;
    Date _dateMaxFilter;
    int _sumMinFilter;
    int _sumMaxFilter;
};

#endif