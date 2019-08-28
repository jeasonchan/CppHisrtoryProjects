// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <string>
// TODO: 在此处引用程序需要的其他头文件

using namespace std;

struct  mystruct {
	string image_path;//全路径
	int grabbedImages = 0;//已经拍摄的照片数
	bool result = true;//变为false，说明出现裂纹
};


void myswap(int& a, int &b);//头文件中，只是声明，定义还是在和头文件同名的源文件中