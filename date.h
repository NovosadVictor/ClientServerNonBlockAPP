#ifndef _DATE_H_
#define _DATE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


class Date {
public:
	Date();
	Date(int y, int m, int d, int h, int min, int sec);
	Date(const char *date);
	void SetRand();
	void SetDate();
	void operator=(const Date &d);
	bool operator <=(const Date &date) const;
	bool operator >=(const Date &date) const;
	const char *GetDate() const;
private:
	void SetCorrect(int y, int m, int d, int h, int min, int sec);
	char _date[20];
	int _year;
	int _month;
	int _day;
	int _hour;
	int _minute;
	int _second;
};

#endif
