#ifndef VIEW_H
#define VIEW_H
#include <pthread.h>
#include <iostream>
#include <vector>
#include <event.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <functional>
#include <algorithm>
#include <jsoncpp/json/json.h>
#include <mysql/mysql.h>
#include <event.h>
#include <string>
#include <vector>
#include <errno.h>
#include "use_mysql.h"
using namespace std;
using namespace Json;

typedef enum{
	SYS,
	REGIST,
	LOGIN,
	SEND,
	GROUP,
	BROAD_ADD,
	BROAD_DEL,
	SYS_REGI,
}MSG_TYPE;

extern map<int,int> online;//在线表id -->>>fd

extern void broadcast_online(int fd);

extern void broadcast_user(int id,bool flag);

extern int offline(int fd);

class view
{
	public:
		mysql_use _sql;
		virtual void process(Value& data,int fd)=0;
};

class login_view:public view
{
	public:
		void process(Value& root,int fd);
};

class regist_view:public view{
	public:
		void process(Value& root,int fd);
};

class send_view:public view{
	public:
		void process(Value& root,int fd);
};

class contral//读事件发生时，回调函数内调用线程内有这个类
{
	public:
		contral();//初始化视图
		void process(int fd,Value& data);//回调函数中直接调用处理fd的data

	private:
		map<int,view*> model_map;
};
#endif
