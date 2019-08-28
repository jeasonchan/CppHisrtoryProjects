/*
�����߳̽���ȫ�ֱ����ṹ�� pass_struct�ĵ�ַ����ȡ�ļ�·������Ƭ������������ͼƬ�����Աȣ�ʼ�ձ�����һ��ԭʼ��Ƭ��ÿһ�ź͵�һ��ԭʼ��Ƭ�Աȣ�
��������ƣ���������µ�ȫ�ֱ����У������̼߳���Ǹ�ȫ�ֱ���
*/


#include "stdafx.h"
#include<Windows.h>
#include<iostream>

#include<vector>


//opencv����ļ�
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2\opencv.hpp"
//#include<opencv2/ml.hpp>
//#include<opencv2/ml/ml.hpp> ��ʵ֤��������Ҫ������һ�еļ��ɣ���Ϊ���е�ͷ�ļ�������һ�仰�����ǰ��������ͷ�ļ�


using namespace std;
using namespace cv;

extern struct mystruct pass_struct;//�������ݸ����̵߳Ľṹ��
extern vector<int> para;

unsigned int WINAPI thread1(void* arg) {
	string original_file_name;
	Mat original_image;
	int current_grabbedImages;
		
	if (pass_struct.grabbedImages == 1) {
		original_file_name = pass_struct.image_path;
		original_image = imread(original_file_name).clone();//ʼ�ձ���һ��
		current_grabbedImages = 1;

	}
	while (true) {//��
		if (pass_struct.grabbedImages > current_grabbedImages) {
			Mat new_image = imread(pass_struct.image_path);//��ѭ���ڲ����壨������Ϊ�˷�ֹ��ֻ��Ϊ�����þֲ��������Զ�����
			Mat diff_image;
			absdiff(original_image,new_image,diff_image);
			
			
			
			
			
			imwrite("d:/diff_image.jpg", diff_image,para );
			//threshold(diff_image, diff_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			//imwrite("d:/diff_image2.jpg", diff_image, para);







			current_grabbedImages = pass_struct.grabbedImages;
		}
		else {

			Sleep(200);//��������ѭ��ѭ���Ĺ��죬�����������
		}



		
	}

	



	return 0;
}


