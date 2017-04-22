#ifndef _PARSER_H_
#define _PARSER_H_

#include "billing.h"
#include "select.h"
#include "insert.h"
#include "delete.h"
#include "update.h"

#include <vector>


class Parser {
public:
	Parser();
	Parser(int &fd);
	void SetAllVector(int fd);
	void ParseRequest(const char *request, int &fd);
	int GetType() const;
	size_t GetTotal() const;
    void GetSelect(SELECT select, int flag);
	void GetInsert(INSERT insert);
	void GetDelete(DELETE delete_, int flag);
    void GetUpdate(UPDATE update, int flag);
	void GetSave(int &fd);
    std::vector<Billing> GetResponse() const;
private:
	size_t _total;
    int _type;
    std::vector<Billing> _AllVector;
    std::vector<Billing> _responseVector;
};


#endif
