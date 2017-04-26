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
	Parser(int fd);
	void SetAllVector();
	void ParseRequest(const char *request);
	int GetType() const;
	size_t GetTotal() const;
    void GetSelect(SELECT select, int flag);
	void GetInsert(INSERT insert);
	void GetDelete(DELETE delete_, int flag);
    void GetUpdate(UPDATE update, int flag);
	void GetSave();
    std::vector<size_t> GetResponse() const;
    std::vector<Billing> _AllVector;
private:
    int fd_;
	size_t _total;
    int _type;
    std::vector<size_t> _responseVector;
};


#endif
