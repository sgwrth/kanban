#ifndef SELECTSTMT_H
#define SELECTSTMT_H
#include <iostream>
#include <string>
#include <unordered_map>
#include "../utils/QueryParam.h"
#include "../../external/sqlite/sqlite3.h"

template<typename T>
class SelectStmt {
public:
	sqlite3_stmt* stmt_{};
	std::string sql_query_{};
	std::unordered_map<int, QueryParam<T>> parameters{};

	static SelectStmt<T> create()
	{
		return SelectStmt<T>();
	}

	SelectStmt<T>& setSqlQuery(std::string sql_query)
	{
		this->sql_query_ = sql_query;
		return *this;
	}

	SelectStmt<T>& prepare(sqlite3* db)
	{
		int rc = sqlite3_prepare_v2(db, this->sql_query_.c_str(), -1, &this->stmt_, 0); /* stmt_ by ref. */
		return *this;
	}

	SelectStmt<T>& addParam(int position, QueryParam<T> parameter)
	{
		if (parameter.datatype_ == "text") {
			std::cout << "Looking good.\n";
			int rc = sqlite3_bind_text(this->stmt_, position, parameter.value_, -1, SQLITE_STATIC);
		}
		return *this;
	}

	SelectStmt<T>& build()
	{
		return *this;
	}
};

#endif
