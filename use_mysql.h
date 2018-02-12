#ifndef MY_SQL_H
#define MY_SQL_H
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
class mysql_use
{

	public:
		mysql_use(
				const char * server="localhost",
				const char * user = "run",
				const char * password = "32875094",
				const char * database = "db1"
				);

		~mysql_use();

		bool IsRight(const char * id,const char * pass);

		bool Add_user(const char * id,const char * pass);
	private:
		MYSQL *conn;
		MYSQL_RES *res;
		MYSQL_ROW row;
};
#endif
