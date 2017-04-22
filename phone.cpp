#include "phone.h"
#include <stdexcept>


Phone::Phone() {
	memcpy(_number, "925-067-18-32", 13);
	_number[13] = '\0';
}

Phone::Phone(const char *phone) {
	if (strlen(phone) != 13)
		throw 3;
	for (size_t i = 0; i < strlen(phone); ++i) {
		if (i == 3 || i == 7 || i == 10) {
			if (phone[i] != '-')
				throw 4;
		}
		else {
			if (!isdigit(phone[i]))
				throw 5;
		}
	}
	memcpy(_number, phone, 13);
	_number[13] = '\0';
}

void Phone::SetRand() {
	for (int i = 0; i < 13; ++i)
		if (i == 3 || i == 7 || i == 10)
			_number[i] = '-';
		else
			_number[i] = '0' + rand() % 10;
	_number[13] = '\0';
}

const char *Phone::GetNumber() const {
	return _number;
}

bool Phone::operator==(const Phone &phone) {
	for (size_t i = 0; i < 14; ++i)
		if (_number[i] != phone.GetNumber()[i])
			return false;
	return true;
}

void Phone::operator=(const Phone &phone) {
        memcpy(_number, phone._number, 13);
        _number[13] = '\0';
}

