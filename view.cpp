#include "view.h"
void login_view::process(Value& root,int fd)
{
	Value val;//发送给用户的通知消息
	val["Msg_Type"] = SYS;
	bool flag;

	int id = root["id"].asInt();
	if(flag = _sql.IsRight(root["id"].toStyledString().c_str(),
				root["password"].toStyledString().c_str()))
		val["flag"] = 1;//登录的结果
	else
		val["flag"] = 0;

	send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);

	if(flag)
	{
		online[root["id"].asInt()] = fd;//插入到在线表中

		int id = root["id"].asInt();

		cout<<online[root["id"].asInt()]<<"上线了"<<endl;

		broadcast_user(id,true);//广播

		sleep(1);//广播已经登录的用户

		broadcast_online(fd);
	}
}
void regist_view::process(Value& root,int fd)
{
	Value val;
	val["Msg_Type"] = SYS_REGI;
	cout<<"regist:";
	int n1 =strlen(root["id"].toStyledString().c_str()) - 1;
	int n2 = strlen(root["password"].toStyledString().c_str()) - 1;

	char * p = new char[n1];

	strncpy(p,root["id"].toStyledString().c_str(),n1);
	p[n1]=0;

	char * q = new char[n2];
	strncpy(q,root["password"].toStyledString().c_str(),n2);
	q[n2-1]=0;

	if(_sql.Add_user(p,q+1))

		val["re_flag"] = 1;
	else
		val["re_flag"] = 0;

	send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
	delete q;
	delete p;
}

void send_view::process(Value& root,int fd)
{
	int id = root["id"].asInt();

	cout<<root["mid"]<<":send msg to  "<<id<<endl;

	send(online[id],root.toStyledString().c_str(),strlen(root.toStyledString().c_str()),0);
}
contral::contral()//初始化视图
{
	model_map.insert(make_pair(REGIST,new regist_view));
	model_map.insert(make_pair(LOGIN,new login_view));
	model_map.insert(make_pair(SEND,new send_view));
	cout<<"model_map size::"<<model_map.size()<<endl;
}

void contral::process(int fd,Value& data)//回调函数中直接调用处理fd的data
{
	//选择对应的视图
	model_map[data["Msg_Type"].asInt()]->process(data,fd);
}
