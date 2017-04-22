#include "update.h"


UPDATE::UPDATE() {
    _service = 0;
    _phone = Phone();
}

UPDATE::UPDATE(double sum, int service,
               Phone phone,
               int serviceFilter,
               Phone phoneFilter,
               Date dateMinFilter,
               Date dateMaxFilter,
               int sumMinFilter,
               int sumMaxFilter) {
    _sum = sum;
    _service = service;
    _phone = phone;
    _serviceFilter = serviceFilter;
    _phoneFilter = phoneFilter;
    _dateMinFilter = dateMinFilter;
    _dateMaxFilter = dateMaxFilter;
    _sumMinFilter = sumMinFilter;
    _sumMaxFilter = sumMaxFilter;
}

int UPDATE::GetService() const {
    return _service;
}

Phone UPDATE::GetPhone() const {
    return _phone;
}

int UPDATE::GetFilterService() const {
    return _serviceFilter;
}

Phone UPDATE::GetFilterPhone() const {
    return _phoneFilter;
}

Date UPDATE::GetFilterDateMin() const {
    return _dateMinFilter;
}

Date UPDATE::GetFilterDateMax() const {
    return _dateMaxFilter;
}

int UPDATE::GetFilterSumMin() const {
    return _sumMinFilter;
}

int UPDATE::GetFilterSumMax() const {
    return _sumMaxFilter;
}

double UPDATE::GetSum() const {
    return _sum;
}