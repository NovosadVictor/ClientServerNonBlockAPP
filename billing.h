#ifndef _BILLING_H_
#define _BILLING_H_

#include <iostream>
#include "date.h"
#include "phone.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>


class Billing {
public:
	Billing();
	Billing(Phone phone, int service, Date dateTime, double sum);
	void SetValues(const char *billing);
	void SetInfo(const char *info);
	void SetBilling(Phone phone, int service, Date dateTime, double sum);
	void SetRand();
	int GetService() const;
	Date GetDate() const;
	Phone GetPhone() const;
	double GetSum() const;
	void ChangeService(int service);
	void ChangePhone(Phone phone);
	void ChangeSum(double sum);
	int ReadBin(int fd);
	int WriteBin(int fd);
	void WriteTXT(FILE *fout);
private:
	void SetCorrect(Phone phone, int service, Date dateTime, double sum);
	Phone _phone;
	int _service;
	Date _dateTime;
	double _sum;
};


#endif
