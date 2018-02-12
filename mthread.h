#ifndef MTHREAD_H
#define MTHREAD_H
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
#include <jsoncpp/json/json.h>
#include <mysql/mysql.h>
#include <event.h>
#include <string>
#include <vector>
#include <errno.h>
#include "use_mysql.h"
#include "view.h"
#include "decrypt.h"

extern map<int,int> online;//在线表id -->>>fd

using namespace std;

using namespace Json;

void broadcast_online(int fd);//给fd广播在线好友

void broadcast_user(int id,bool flag);//某id上线或者离线的广播信息.

int offline(int fd);//fd离线

class mthread
{

	public:
		mthread();
		//监听套接字的读写事件
		static void io_cb(int fd,short ev,void *arg);
		static void fd_cb(int fd,short ev,void *arg);//管道监听事件的回调，接收任务
		static void *task(void *arg);//线程执行的主函数	
		~mthread();
	public:
		pthread_t th_id;//线程id
		struct event_base*base;//每个线程有一个事件循环
		vector<int> fds; //线程监听的fd
		int fd_main[2];  //和主进程通信的sockfd
		contral cmd;
};

class mthread_pool
{
	public:
		mthread_pool(int num = 3);
		~mthread_pool();
		int GetMinFd();//得到最小负载线程的0管道
	private:
		size_t pth_num;//线程个数
		vector<mthread*>th;//指向工作线程的指针vector
};
#endif
