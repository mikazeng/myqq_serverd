#include"use_mysql.h"

mysql_use::mysql_use(
		const char * server,
		const char * user ,
		const char * password,
		const char * database
		)
{
	conn =mysql_init(NULL);
	if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
}

mysql_use::	~mysql_use()
{
	mysql_close(conn);
	mysql_free_result(res);
}

bool mysql_use::IsRight(const char * id,const char * pass)
{
	char  sql_md[128]={0};
	sprintf(sql_md,"select * from user where id=%s and password=%s",id,pass);	
	mysql_query(conn,sql_md);
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	/*while(row !=NULL)
	  {
	  printf("%s\t %s",row[0],row[1]);
	  row = mysql_fetch_row(res);
	  }*/
	if(row != NULL)
		return true;
	else
		return false;
}

bool mysql_use::Add_user(const char * id,const char * pass)
{
	if(id ==NULL||pass ==NULL) return false;
	char  sql_md[128]={0};
	sprintf(sql_md,"select * from user where id=%s",id);	
	mysql_query(conn,sql_md);
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	if(row != NULL)
		return false;
	memset(sql_md,0,128);
	sprintf(sql_md,"insert into user (id,password) values('%s','%s');",id,pass);
	puts(sql_md);
	if( mysql_query(conn,sql_md)==0)
		return true;
	else
		return false;
}
