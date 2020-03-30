// NetBinSocket.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<future>
#include<thread>
#include<vector>
#include <iostream>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
//using namespace std;
void initialization();
int const SocketMaxSize = 5;
std::vector<SOCKET> v;
int main()
{
    initialization();
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//定义发送缓冲区和接受缓冲区
	char send_buf[100];
	char recv_buf[100];
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5010);
	SOCKET s_accept;
	SOCKADDR_IN accept_addr;
	SOCKET s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		std::cout << "套接字绑定失败！" << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "套接字绑定成功！" << std::endl;
	}
	//设置套接字为监听状态
	if (listen(s_server, SOMAXCONN) < 0) {
		std::cout << "设置监听状态失败！" << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "设置监听状态成功！" << std::endl;
	}
	std::cout << "服务端正在监听连接，请稍候...." << std::endl;
	//接受连接请求
	
	auto w = std::async(std::launch::async, [&] {
		while (true)
		{
			if (v.size() < ::SocketMaxSize)
			{
				for (int i = v.size(); i < ::SocketMaxSize; i++)
				{
					len = sizeof(SOCKADDR);
					s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
					if (s_accept!=SOCKET_ERROR)
					{
						v.push_back(s_accept);
						std::cout<< inet_ntoa(accept_addr.sin_addr)
							<< "已经连接上\r\n"<<std::endl;
					}
					std::cout << "连接建立，准备接受数据" << std::endl;
				}
			}
		}
	});
	
	
	/*if (s_accept == SOCKET_ERROR) {
		std::cout << "连接失败！" << std::endl;
		WSACleanup();
		return 0;
	}*/
	
	while (1) {
		recv_len = recv(s_accept, recv_buf, 100, 0);
		if (recv_len < 0) {
			std::cout << "接受失败！" << std::endl;
			break;
		}
		else {
			std::cout << "客户端信息:" << recv_buf << std::endl;
		}
		std::cout << "请输入回复信息:";
		std::cin >> send_buf;
		send_len = send(s_accept, send_buf, 100, 0);
		if (send_len < 0) {
			std::cout << "发送失败！" << std::endl;
			std::cout << "发送失败！" << std::endl;
			std::cout << "发送失败！" << std::endl;
			break;
		}
	}
	//关闭套接字
	closesocket(s_server);
	closesocket(s_accept);
	//释放DLL资源
	WSACleanup(); 
    std::cout << "Hello World!\n";
}
void initialization()
{
    WORD w_req = MAKEWORD(2, 2);
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		std::cout << "初始化套接字库失败！" << std::endl;
	}
	else {
		std::cout << "初始化套接字库成功！" << std::endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		std::cout << "套接字库版本号不符！" << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "套接字库版本正确！" << std::endl;
	} 
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
