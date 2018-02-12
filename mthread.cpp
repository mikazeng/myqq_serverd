#include"mthread.h"
//广播之前在线的Id信息给fd
void broadcast_online(int fd)
{
	Value root;
	root["Msg_Type"] = BROAD_ADD;//上线通知

	map<int,int>::iterator it = online.begin();

	for( it= online.begin();it!=online.end();++it)
	{
		if(it->second !=fd)
		{
			root["id"]= it->first;
			cout<<"two bro to"<<fd<<endl;
			cout<<root.toStyledString()<<endl;
			send(fd,root.toStyledString().c_str(),strlen(root.toStyledString().c_str()),0);
		}
	}
}
//广播某id的上线/下线消息
void broadcast_user(int id,bool flag)
{
	Value root;
	if(flag)
		root["Msg_Type"] = BROAD_ADD;//id上线通知
	else
		root["Msg_Type"] = BROAD_DEL;//id下线通知

	root["id"]= id;

	cout<<"online size"<<online.size()<<endl;

	map<int,int>::iterator it = online.begin();

	cout<<root.toStyledString()<<endl;

	for(it= online.begin();it!=online.end();++it)
	{
		cout<<"brod fd"<<it->second<<endl;

		send(it->second,root.toStyledString().c_str(),strlen(root.toStyledString().c_str()),0);
	}
}
//有fd下线
//更新最新的在线表
//广播该离线id 
int offline(int fd)
{
	map<int,int>::iterator it = online.begin();
	int id = -1;
	for( ;it!=online.end();++it)
	{
		if(it->second == fd)
		{
			id = it->first;
			online.erase(it);
			break ;
		}
	}
	broadcast_user(id,false);
	printf("fd=%d client  id=%d 下线了\n",fd,id);
	close(fd);
	return id;
}
mthread::mthread():th_id(0)
{
	if(socketpair(AF_UNIX,SOCK_STREAM,0,fd_main)==-1)
		cout<<"socketpair faidlen"<<endl;
	pthread_create(&th_id,NULL,task,this);
}
//监听套接字的读写事件
  void mthread::io_cb(int fd,short ev,void *arg)
{
	char buff[256]={0};
	int recvlen = 0;

	mthread*_this = (mthread*)arg;

	if(ev & EV_READ)
	{
		if((recvlen=recv(fd,buff,255,0)) <= 0)
		{
			offline(fd);
		}
		else
		{
			cout<<"=========密文======================="<<endl<<endl;
			cout<<"buff:::"<<buff<<endl<<endl;;
			cout<<"recvlen:::"<<recvlen<<endl<<endl;
			cout<<"=========解密后======================="<<endl<<endl;
			my_decrypt rsa_de(buff,recvlen);
			cout<<"de:::"<<rsa_de.de;
			cout<<"len::"<<strlen(rsa_de.de)<<endl;
			string s(rsa_de.de);//先用于构造string
			Value root;
			Reader read;
			if(!read.parse(s,root))//再解析
			{
				cout<<"接收的json 解析出错"<<endl;
				return;
			}
			/*
			   cout<<"id::"<<root["id"].asInt()<<endl;
			   cout<<"passwd::"<<root["password"].asString()<<endl;
			 */
			_this->cmd.process(fd,root);
		}
	}
}
 void mthread::fd_cb(int fd,short ev,void *arg)//管道监听事件的回调，接收任务
{
	mthread*_this = (mthread*)arg;
	int cfd;
	read(fd,(char *)&cfd,4);
	cout<<"["<<pthread_self()%1000<<"]"<<"thread recev new fd "<<cfd<<endl;

	_this->fds.push_back(cfd);
	//监听该事件
	struct event * io_fd = event_new(_this->base,cfd,EV_READ|EV_PERSIST,io_cb,_this);

	event_add(io_fd,NULL);

}
//线程执行的主函数	
void*  mthread::task(void *arg)
{
	mthread*_this = (mthread*)arg;
	cout<<"create thread:"<<pthread_self()%1000<<endl;
	_this->base = event_init(); //初始化base
	assert(_this->base);
	//为线程添加管道监听事件 
	struct event *pipe_ev = event_new(_this->base,_this->fd_main[1],EV_READ|EV_PERSIST,fd_cb,_this);
	cout<<"fd_main[1]"<<_this->fd_main[1]<<endl;
	assert(pipe_ev);
	event_add(pipe_ev,NULL);//第二个参数是监听的时间，超时则直接返回 
	event_base_dispatch(_this->base);//开始循环监听 
}
mthread::~mthread()
{
	event_base_free(base);
}

mthread_pool::mthread_pool(int num):pth_num(num)
{
	for(int i =0;i<num;++i)
	{
		th.push_back(new mthread());
	}
}
mthread_pool::~mthread_pool()
{
	for(int i =th.size()-1;i>=0;--i){
		delete th[i];
		th.pop_back();
	}
}
 int mthread_pool::GetMinFd()//得到最小负载线程的0管道
{

	static size_t Ronbin = 0;
	int  pre = th[Ronbin % pth_num]->fd_main[0];
	Ronbin++;
	for(int i =0;i<th.size();++i)
		cout<<"["<<th[i]->th_id%1000<<"]thread's fds:"<<th[i]->fds.size()<<endl;
	return pre;
	/*
	   int fds = th[0]->fds.size();
	   cout<<"fds"<<fds<<endl;
	   int min = th[0]->fd_main[0]; 

	   for(int i =0;i<th.size();++i){
	   if(th[i]->fds.size() < fds)
	   {
	   min = th[i]->fd_main[0];
	   fds = th[i]->fds.size();
	   cout<<i<<"if min"<<min<<endl;
	   }
	   cout<<"["<<i%1000<<"]thread:"<<th[i]->fds.size()<<endl;
	   }
	   cout<<"min"<<min<<endl;
	   cout<<"online size"<<online.size()<<endl<<endl;
	   return min;
	 */

}
