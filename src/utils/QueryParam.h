#ifndef QUERYPARAM_H
#define QUERYPARAM_H
#include <string>

template<typename T>
class QueryParam {
public:
	std::string datatype_;
	T value_;

	QueryParam(std::string datatype, T value)
		: datatype_{datatype}, value_{value}
	{}
};

#endif
