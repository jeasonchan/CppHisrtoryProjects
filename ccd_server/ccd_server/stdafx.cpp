// stdafx.cpp : 只包括标准包含文件的源文件
// ccd_server.pch 将作为预编译标头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h" //这里面包含了各种声明，想用这个头文件对应的cpp当中的东西，就要包含此头文件

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用

struct mystruct pass_struct;//用来传递给子线程的结构体

void myswap(int& a, int &b) {
	int temp = a;
	a = b;
	b = temp;

}
