#include "delete.h"

/*
class DELETE {
public:
    DELETE();
    DELETE(int service);
    DELETE(Phone phone);
    DELETE(Date dateMin, Date dateMax);
    DELETE(int service, Phone phone);
    DELETE(int service, Phone phone, Date dateMin, Date dateMax);
    int GetService() const;
    Date GetMinDate() const;
    Date GetMaxDate() const;
    Phone GetPhone();
private:
    Date _dateMin;
    Date _dateMax;
    Phone _phone;
    int _service;
};
*/

DELETE::DELETE() {
    _service = 0;
    _dateMin = Date();
    _dateMax = Date(2100, 12, 30, 23, 59, 59);
    _phone = Phone("7777777777");
}

DELETE::DELETE(int service, Phone phone, Date dateMin, Date dateMax, int sumMin, int sumMax) {
    _service = service;
    _phone = phone;
    _dateMin = dateMin;
    _dateMax = dateMax;
    _sumMin = sumMin;
    _sumMax = sumMax;
}

int DELETE::GetService() const {
    return _service;
}

Date DELETE::GetMinDate() const {
    return _dateMin;
}

Date DELETE::GetMaxDate() const {
    return _dateMax;
}

Phone DELETE::GetPhone() const{
    return _phone;
}

int DELETE::GetMinSum() const {
    return _sumMin;
}

int DELETE::GetMaxSum() const {
    return _sumMax;
}
