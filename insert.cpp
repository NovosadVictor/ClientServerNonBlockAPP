#include <stdexcept>

#include "insert.h"

/*
class INSERT {
public:
    INSERT();
    INSERT(Phone phone, int service, Date date, double sum);
    Phone getPhone() const;
    int getService() const;
    Date getDate() const;
    double getSum() const;
private:
    Phone _phone;
    int _service;
    Date _date;
    double _sum;
};
*/

INSERT::INSERT() {
    _phone = Phone();
    _service = 0;
    _date = Date();
    _sum = 0;
}

INSERT::INSERT(Phone phone, int service, Date date, double sum) {
    _phone = phone;
    _service = service;
    _date = date;
    _sum = sum;
}

Phone INSERT::GetPhone() const {
    return _phone;
}

int INSERT::GetService() const {
    return _service;
}

Date INSERT::GetDate() const {
    return _date;
}

double INSERT::GetSum() const {
    return _sum;
}


