#ifndef QUERYPARAM_H
#define QUERYPARAM_H
#include <string>
#include <variant>

class QueryParam {
public:
	std::variant<std::string, int> value_;

	QueryParam(std::string value) : value_{value} {}
	QueryParam(int value) : value_{value} {}
};

#endif
