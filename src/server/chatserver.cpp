#include "chatserver.hpp"
#include "json.hpp"
using json = nlohmann::json;
#include<functional>
#include "chatservice.hpp"


ChatServer::ChatServer(EventLoop *loop,
                const InetAddress& listenAddr,
                const string & nameArg)
                :_server(loop,listenAddr,nameArg),_loop(loop)
                {
                    _server.setConnectionCallback(std::bind(&ChatServer::Connection,this,_1));

                    _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));

                    _server.setThreadNum(4);

                    
                }

void ChatServer::start()
{
    _server.start();
}

void ChatServer::Connection(const TcpConnectionPtr& conn)
{
    if(!conn->connected())
    {
        conn->shutdown();
    }

}
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                Buffer *buffer,
                Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    json js = json::parse(buf);


    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    msgHandler(conn,js,time);
}