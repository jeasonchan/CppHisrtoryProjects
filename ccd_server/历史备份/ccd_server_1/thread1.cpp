/*
从主线程接收全局变量结构题 pass_struct的地址，读取文件路径和照片个数，进行文图片分析对比，始终保留第一张原始照片，每一张和第一张原始照片对比，
如果有裂纹，将结果更新到全局变量中，主线线程检测那个全局变量
*/


#include "stdafx.h"
#include<Windows.h>
#include<iostream>

#include<vector>


//opencv相关文件
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2\opencv.hpp"
//#include<opencv2/ml.hpp>
//#include<opencv2/ml/ml.hpp> 事实证明，仅需要包含上一行的即可，因为此行的头文件仅包含一句话，就是包含上面的头文件


using namespace std;
using namespace cv;

extern struct mystruct pass_struct;//用来传递给子线程的结构体
extern vector<int> para;

unsigned int WINAPI thread1(void* arg) {
	string original_file_name;
	Mat original_image;
	int current_grabbedImages;
		
	if (pass_struct.grabbedImages == 1) {
		original_file_name = pass_struct.image_path;
		original_image = imread(original_file_name).clone();//始终保留一份
		current_grabbedImages = 1;

	}
	while (true) {//当
		if (pass_struct.grabbedImages > current_grabbedImages) {
			Mat new_image = imread(pass_struct.image_path);//在循环内部定义（声明）为了防止，只是为了利用局部变量会自动销毁
			Mat diff_image;
			absdiff(original_image,new_image,diff_image);
			
			
			
			
			
			imwrite("d:/diff_image.jpg", diff_image,para );
			//threshold(diff_image, diff_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			//imwrite("d:/diff_image2.jpg", diff_image, para);







			current_grabbedImages = pass_struct.grabbedImages;
		}
		else {

			Sleep(200);//不能让死循环循环的过快，否则造成阻塞
		}



		
	}

	



	return 0;
}


