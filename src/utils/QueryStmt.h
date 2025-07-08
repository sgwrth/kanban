#ifndef SELECTSTMT_H
#define SELECTSTMT_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include "../utils/QueryParam.h"
#include "../../external/sqlite/sqlite3.h"

class QueryStmt {
public:
	sqlite3_stmt* stmt_{};
	std::string sql_query_{};
	std::unordered_map<int, QueryParam> parameters{};

	static QueryStmt create()
	{
		return QueryStmt();
	}

	QueryStmt& set_sql_query(std::string sql_query)
	{
		this->sql_query_ = sql_query;
		return *this;
	}

	QueryStmt& prepare(sqlite3* db)
	{
		int rc = sqlite3_prepare_v2(db, this->sql_query_.c_str(), -1, &this->stmt_, 0); /* stmt_ by ref. */
		return *this;
	}

	QueryStmt& add_param(const int pos, const QueryParam& param)
	{
		std::visit([&](auto&& value) { /* &: Capture 'this' by reference. */
			using V = std::decay_t<decltype(value)>; /* Get stripped-down type of value. */
			if constexpr (std::is_same_v<V, std::string>) {
				int rc = sqlite3_bind_text(this->stmt_, pos, value.c_str(), -1, SQLITE_TRANSIENT);
			}
			if constexpr (std::is_same_v<V, int>) {
				int rc = sqlite3_bind_int(this->stmt_, pos, value);
			}
		}, param.value_);
		return *this;
	}

	QueryStmt build()
	{
		return *this;
	}
};

#endif
