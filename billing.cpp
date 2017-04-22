#include "billing.h"


Billing::Billing() {
	SetCorrect(Phone(), 1, Date(), 1);
}

Billing::Billing(Phone phone, int service, Date dateTime, double sum) {
	SetCorrect(phone, service, dateTime, sum);
}

void Billing::SetBilling(Phone phone, int service, Date dateTime, double sum) {
	SetCorrect(phone, service, dateTime, sum);
}

void Billing::SetRand() {
	_phone.SetRand();
	_service = 1 + rand() % 4;
	_dateTime.SetRand();
	_sum = (rand() / (double)RAND_MAX) * (rand() % 15000);
}

void Billing::SetCorrect(Phone phone, int service, Date dateTime, double sum) {
	if ((service >= 1 && service <= 4) && (sum >= 0 && sum <= 15000)) {
		_phone = phone;
		_service = service;
		_dateTime = dateTime;
		_sum = sum;
	} else
		throw std::invalid_argument("incorrect service and/or sum\n");
}

void Billing::ChangeService(int service) {
	_service = service;
}

void Billing::ChangePhone(Phone phone) {
	_phone = phone;
}

void Billing::ChangeSum(double sum) {
	_sum = sum;
}

int Billing::GetService() const {
	return _service;
}

Date Billing::GetDate() const {
	return _dateTime;
}

Phone Billing::GetPhone() const {
	return _phone;
}

double Billing::GetSum() const {
	return _sum;
}

int Billing::WriteBin(int fd) {
	if (write(fd, _phone.GetNumber(), 14) != 14)
		return 0;
	if (write(fd, &_service, sizeof(_service)) != sizeof(_service))
                return 0;
	if (write(fd, _dateTime.GetDate(), 20) != 20)
                return 0;
	if (write(fd, &_sum, sizeof(double)) != sizeof(double))
                return 0;
	return 1;
}

int Billing::ReadBin(int fd) {
	char number[14];
	int service;
	double sum;
	char date[20];
	int status;
	if ((status = read(fd, number, 14)) != 14) {
		if (status == 0)
			return 0;
		else
			return -1;
	}
	if (read(fd, &service, sizeof(int)) != sizeof(int))
                	return -1;
	if (read(fd, date, 20) != 20)
                	return -1;
	if (read(fd, &sum, sizeof(double)) != sizeof(double))
			 return -1;
	SetCorrect(Phone(number), service, Date(date), sum);
	return 1;
}

void Billing::WriteTXT(FILE *fout) {
	fprintf(fout, "%s ", _phone.GetNumber());
	fprintf(fout, "%d ", _service);
	fprintf(fout, "%s ", _dateTime.GetDate());
	fprintf(fout, "%f\n", _sum);
}


