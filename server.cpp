#include "server.h"

map<int,int> online;//在线表id -->>>fd
void accept_cb(int sockfd,short ev,void *arg)//客户链接过来的事件发生
{	
	static int i =0;
	qqserver *_this = (qqserver*)arg;
	struct sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	int cfd = accept(sockfd,(struct sockaddr*)&caddr,&len);
	if(cfd == -1){ cout<<pthread_self()<<"reccept failed"<<endl;return;}
	cout<<"recve fd "<<i++<<endl;
	cout<<"IP:"<<inet_ntoa(caddr.sin_addr)<<endl;
	write(_this->th_pool->GetMinFd(),(char*)&cfd,4);

}

qqserver::qqserver(int port,int num) //端口号
{
	listen_fd = create_socket(port);//创建server端socket
	cout<<"listen fd"<<listen_fd<<endl;
	listen_port = port;
	th_num = num;
	if (listen_fd == 0){
		cout<<"listen socket 创建失败"<<endl;
		exit(-1);//创建socket失败，程序退出
	}

	base  = event_init();

	th_pool = new mthread_pool();

	struct event* sock_ev = event_new(base,listen_fd,EV_READ|EV_PERSIST,accept_cb,this);

	event_add(sock_ev,NULL);

	event_base_dispatch(base);

	event_free(sock_ev);
}


int qqserver::create_socket(int port)
{	
	int sockfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("bind port %d failed %s\n", port, strerror(errno));
		return 0;
	}
	if (listen(sockfd, 300) == -1)
	{
		printf("listen failed %s\n", strerror(errno));
		return 0;
	}
	return sockfd;//返回listen的socket描述符
}


qqserver::~qqserver()
{
	delete th_pool;
	event_base_free(base);
	close(listen_fd);
}

