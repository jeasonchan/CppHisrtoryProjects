/*
 main.cpp: 主程序，主要功能包括：
1、启动socket服务
2、完成拍照任务
3、调用照片处理线程
4、

*/

#include "stdafx.h"
#include <iostream>

#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <winbase.h> 
#include <winerror.h>
#include <errhandlingapi.h>
#include <winnt.h>
#include <process.h>
#include <Windows.h>
#include<vector>

//pylon相关头文件
#include <pylon/PylonGUI.h>
#include <pylon/PylonIncludes.h>
#include"pylon/TlFactory.h"



using namespace std;
using namespace Pylon;

extern unsigned int WINAPI thread1(void* arg);//线程1，图像处理线程
extern struct mystruct pass_struct;//用来传递给子线程的结构体
unsigned int WINAPI crack_check_loop(void* arg);


const uint32_t c_countOfImagesToGrab = 1;
char to_client_success[] = ":00-01-02-00?";
char to_client_fail[] = ":00-01-02-01?";
char to_client_grab_fail[] = ":00-01-01-01?";
char to_client_grab_success[] = ":00-01-01-00?";
char to_client_crack_exist[] = ":00-01-04-01?";
char to_client_crack_notexist[] = ":00-01-04-00?";
vector<int> para;
SOCKET client_socket;


int main()
{	
	//防止第二个实例产生
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, _T("ccd_server"));
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		return FALSE;
	}
	
	WORD wVersionRequested; //typedef unsigned short WORD; 
	WSADATA wsaData; //typedef struct WSAData{} WSADATA； 
	int err;

	wVersionRequested = MAKEWORD(1, 1);//#define MAKEWORD(low,high) ((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8)),先位运算在按位或 

	err = WSAStartup(wVersionRequested, &wsaData);//第一步：加载 winsock 资源，并检查异常/错误 
	if (err != 0) {
		return 0;//main()主程序出口 
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {//处理异常，有异常就退出主程序 
		WSACleanup();//清理资源 
		return 0;//main()主程序出口 
	}

	SOCKET serve_socket = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN serve_sock_address;
	serve_sock_address.sin_addr.S_un.S_addr = INADDR_ANY;
	serve_sock_address.sin_family = AF_INET;
	serve_sock_address.sin_port = htons(7000);

	bind(serve_socket, (SOCKADDR*)&serve_sock_address, sizeof(SOCKADDR));

	listen(serve_socket,5);

	/**************************************************************************/
	PylonInitialize();
	
	cout << "ccd_server online" << endl;
	
	/**************************************************************************/

	SOCKADDR_IN client_socket_address;
	int length = sizeof(SOCKADDR);
	HANDLE crack_check_loop_thread=NULL;
restart: {
	cout << "尝试等待client socket" << endl;
	CloseHandle(crack_check_loop_thread);
	}

	client_socket = accept(serve_socket, (SOCKADDR*)&client_socket_address, &length);//此处运行在阻塞模式，不生成client_socket就不执行下一语句
	crack_check_loop_thread=(HANDLE)_beginthreadex(NULL, 0, crack_check_loop, NULL, 0, NULL);

	while (1)
	{
		char from_client[300] ="";//这里定义的是char型字符串，本质上就是char型数组，最后一位是\0
		

		int socket_conditon = recv(client_socket, from_client, sizeof(from_client), 0);
		if (socket_conditon <=0) goto restart; //socket意外断开后，去restart标签处等待重新连接，因为accept运行在阻塞模式
		cout << socket_conditon<<"  " << from_client << endl;
		
		if (pass_struct.result == false) cout << "出现裂纹"<<endl;


		if (from_client[8] == '1') { //拍照命令
			string from_client_string = from_client;
			int sub_length = from_client_string.find("?") - 10;
			cout << sub_length << endl;
			String_t file_name = (from_client_string.substr(10, sub_length)).c_str();
			int sub_length2 = from_client_string.find("?") - from_client_string.rfind("\\") - 1;//注意！！\这个符号是转译附，要查找\，必须找\\这个符号
			cout << sub_length2 << endl;
			String_t pure_file_name = (from_client_string.substr(from_client_string.rfind("\\") + 1, sub_length2)).c_str();
			cout << "file name:" << file_name << "           pure name:" << pure_file_name << endl;//正式程序要删除


			CTlFactory &TlFactory = CTlFactory::GetInstance();
			DeviceInfoList_t lstDevices;
			TlFactory.EnumerateDevices(lstDevices);
			if (!lstDevices.empty()) {//枚举的设备列表不为空时，创建相机对象
				CInstantCamera camera(TlFactory.CreateFirstDevice());
				camera.MaxNumBuffer = 5;
				CGrabResultPtr ptrGrabResult;
				camera.StartGrabbing(c_countOfImagesToGrab);
				while (camera.IsGrabbing()) {
					camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
					if (ptrGrabResult->GrabSucceeded()) {
						CImagePersistence::Save(ImageFileFormat_Jpeg, pure_file_name, ptrGrabResult, NULL);
						system("move "+pure_file_name+" "+file_name);
						pass_struct.grabbedImages++;//抓取照片数+1 
						pass_struct.image_path = file_name;//将file_name写入地址中，另一个线程检测到file_name变化就进行识别处理
						if (pass_struct.grabbedImages == 1) _beginthreadex(NULL, NULL, thread1, NULL, 0, NULL);//仅在拍下第一张图片时创建处理图像线程，开始处理图像
						
						cout << "抓取成功" << endl;
						send(client_socket, to_client_grab_success, sizeof(to_client_grab_success), 0);
						continue;
					}
					else {
						cout << "拍照失败" << endl;
						send(client_socket, to_client_grab_fail, sizeof(to_client_grab_fail), 0);
						continue;
					}
				}
				continue;//只是进入下一个循环等待指令
			}
			else {
					cout << "拍照失败" << endl;
					send(client_socket, to_client_grab_fail, sizeof(to_client_grab_fail), 0);
					continue;
			}
			
		}

		if (from_client[8] == '2') {//确认相机存在命令
			CTlFactory &TlFactory = CTlFactory::GetInstance();
			DeviceInfoList_t lstDevices;
			TlFactory.EnumerateDevices(lstDevices);

			if (!lstDevices.empty()) {//相机存在的情况 
				to_client_success[8] = '2';
				send(client_socket, to_client_success, sizeof(to_client_success), 0);
				continue;
			}
			else {//相机不存在的情况 
				to_client_fail[8] = '2';
				send(client_socket, to_client_fail, sizeof(to_client_fail), 0);
				continue;
			}

		}

		if (from_client[8] == '3') {//接收到指令后，退出当前实例/进程 

			break;
		}

		if (from_client[8] == '4') {//接收到裂纹查询指令
			if (pass_struct.result == false) send(client_socket, to_client_crack_exist, sizeof(to_client_crack_exist), 0);
			if (pass_struct.result==true) send(client_socket, to_client_crack_notexist, sizeof(to_client_crack_notexist), 0);
		}
		
	}
	closesocket(client_socket);
	PylonTerminate(); //不能清理资源，要不然会发生崩溃
	


    return 0;
}



/*************************************************************************/

unsigned int WINAPI crack_check_loop(void*arg) {
	while (true) {
		if (pass_struct.result == false) send(client_socket, to_client_crack_exist, sizeof(to_client_crack_exist), 0);//这里的client_socket已变成全局变量

		Sleep(1000);
	}

	return 0;
}