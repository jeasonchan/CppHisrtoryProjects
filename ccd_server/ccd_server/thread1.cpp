/*
从主线程接收全局变量结构题 pass_struct的地址，读取文件路径和照片个数，进行文图片分析对比，始终保留第一张原始照片，每一张和第一张原始照片对比，
如果有裂纹，将结果更新到全局变量中，主线线程检测那个全局变量
*/


#include "stdafx.h"
#include<Windows.h>
#include<iostream>

#include<vector>
#include"thread1.h"

//opencv相关文件
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2\opencv.hpp"


using namespace std;
using namespace cv;

extern struct mystruct pass_struct;//用来传递给子线程的结构体
extern vector<int> para;

//输入的mat是单通道图片，limit是像素值，返回值是单通道图中，像素值大于limit的像素点个数，越不同，越亮
int bSums(Mat src, uchar limit);


unsigned int WINAPI thread1(void* arg) {
	string original_file_name;
	Mat original_image;
	int current_grabbedImages;
		
	if (pass_struct.grabbedImages == 1) {
		original_file_name = pass_struct.image_path;
		original_image = imread(original_file_name, IMREAD_GRAYSCALE).clone();//读入灰度图，始终保留一份
		current_grabbedImages = 1;

	}
	while (true) {
		if (pass_struct.grabbedImages > current_grabbedImages) {//当全部变量里面的计数大于当前计数时，说明更新过一次
			string current_file_name = pass_struct.image_path;
			Mat new_image = imread(current_file_name,IMREAD_GRAYSCALE);//在循环内部定义（声明）为了防止，只是为了利用局部变量会自动销毁
			Mat diff_image;
			absdiff(original_image,new_image,diff_image);
			if (bSums(diff_image, 40) > 1000) {
				imwrite(current_file_name.substr(0, current_file_name.length() - 4) + "_diff.jpg", diff_image, para);
				pass_struct.result = false;
			}
			
			







			current_grabbedImages = pass_struct.grabbedImages;//更新当前计数的拍照数
		}
		else {

			Sleep(200);//不能让死循环循环的过快，否则造成阻塞
		}



		
	}

	



	return 0;
}


/******************************************************************************/

int bSums(Mat src,uchar limit)
{

	int counter = 0;
	//迭代器访问像素点
	Mat_<uchar>::iterator it = src.begin<uchar>();
	Mat_<uchar>::iterator itend = src.end<uchar>();
	for (; it != itend; ++it)
	{
		if ((*it) >limit) counter += 1;//二值化后，像素点是0或者255
	}
	return counter;
}