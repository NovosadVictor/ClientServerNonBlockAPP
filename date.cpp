#include "date.h"
#include <stdexcept>


Date::Date() {
	SetCorrect(1900, 1, 1, 0, 0, 0);
}

Date::Date(int y, int m, int d, int h, int min, int sec) {
	SetCorrect(y, m, d, h, min, sec);
}

Date::Date(const char *date) {
	char year[5];
	char month[3];
	char day[3];
	char hour[3];
	char minute[3];
	char second[3];
	memcpy(year, date, 4);
	year[4] = '\0';
	memcpy(month, date + 5, 2);
	month[2] = '\0';
	memcpy(day, date + 8, 2);
	day[2] = '\0';
	memcpy(hour, date + 11, 2);
	hour[2] = '\0';
	memcpy(minute, date + 14, 2);
	minute[2] = '\0';
	memcpy(second, date + 17, 2);
	second[2] = '\0';
	SetCorrect(atoi(year), atoi(month), atoi(day), atoi(hour), atoi(minute), atoi(second));
}

void Date::SetRand() {
	int y = 1900 + rand() % 200;
	int m = 1 + rand() % 12;
	int d = 1 + rand() % 30;
	int h = rand() % 24;
	int min = rand() % 60;
	int sec = rand() % 60;
	SetCorrect(y, m, d, h, min, sec);
}

void Date::SetCorrect(int y, int m, int d, int h, int min, int sec) {
	 if ((y >= 1900 && y <= 2100)
            && (m >= 1 && m <= 12)
            && (d >= 1 && d <= 30)
            && (h >= 0 && h <= 23)
            && (min >= 0 && min <= 59)
            && (sec >= 0 && sec <= 59)) {
		_year = y;
		_month = m;
		_day = d;
		_hour = h;
		_minute = min;
		_second = sec;
		SetDate();
	}
	else
		throw 2;
}

void Date::SetDate() {
	snprintf(_date, 20, "%4d-%02d-%02d:%02d:%02d:%02d", _year, _month, _day, _hour,
									_minute, _second);
	_date[19] = '\0'; 	
}

void Date::operator=(const Date &d) {
	SetCorrect(d._year, d._month, d._day, d._hour, d._minute, d._second);
}

bool Date::operator<=(const Date &date) const{
	return ((_year < date._year) ||
		(_year == date._year && _month < date._month) ||
		(_year == date._year && _month == date._month && _day < date._day) ||
		(_year == date._year && _month == date._month && _day == date._day && _hour < date._hour) ||
		(_year == date._year && _month == date._month && _day == date._day && _hour == date._hour && _minute < date._minute) ||
		(_year == date._year && _month == date._month && _day == date._day && _hour == date._hour && _second <= date._second));
}

bool Date::operator>=(const Date &date) const{
	return ((_year > date._year) ||
			(_year == date._year && _month > date._month) ||
			(_year == date._year && _month == date._month && _day > date._day) ||
			(_year == date._year && _month == date._month && _day == date._day && _hour > date._hour) ||
			(_year == date._year && _month == date._month && _day == date._day && _hour == date._hour && _minute > date._minute) ||
			(_year == date._year && _month == date._month && _day == date._day && _hour == date._hour && _second >= date._second));
}

const char *Date::GetDate() const {
	return _date;
}
