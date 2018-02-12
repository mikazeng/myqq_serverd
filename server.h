#ifndef WORK_H
#define WORK_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <jsoncpp/json/json.h>
#include <mysql/mysql.h>
#include <event.h>
#include <string>
#include <vector>
#include <errno.h>
#include <map>
#include <iostream>
#include "mthread.h"
using namespace std;


class qqserver 
{
	public:
		qqserver(int port,int num = 3);

		~qqserver();

		int  accept_cfd();

	private:
		int  create_socket(int prot);//创建监听

		int listen_fd;

		int listen_port;

		int th_num;

		struct event_base *base;

		mthread_pool *th_pool;

		friend void accept_cb(int fd,short ev,void *arg);
};

#endif
