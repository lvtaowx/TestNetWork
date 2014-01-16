/*
 * Server.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: lvanlv
 */

#include <stdio.h>
#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include <TcpServer.h>
#include <EventLoop.h>
#include <InetAddress.h>
#include <Buffer.h>

using namespace netlib::net;

class EchoServer
{
public:
	EchoServer(EventLoop* loop, const InetAddress& addr)
		: server_(loop, addr, "echoServer")
	{
		server_.setConnetionCallback(boost::bind(&EchoServer::onConnection, this, _1));
		server_.setMessageCallback(boost::bind(&EchoServer::messageCb, this, _1, _2));
		server_.setThreadNum(0);
	}

	void start()
	{
		server_.start();
	}

	void onConnection(const TcpConnectionPtr& conn)
	{
		char* buf = "this is server service hello world";
		conn->send(buf);
		printf(" %s %s\n", __FILE__, __FUNCTION__);
	}

	void messageCb(const TcpConnectionPtr& conn, Buffer* buf)
	{
		std::string msg(buf->retrieveAllToString());
		std::cout << "msg  " << msg <<std::endl;

		conn->send(msg.c_str());
	}

private:
	TcpServer server_;
};

int main(int argc, char* argv[])
{
	InetAddress addr("127.0.0.1", 1988);
	EventLoop loop;

	EchoServer echoServer(&loop, addr);
	echoServer.start();
	loop.loop();
}
