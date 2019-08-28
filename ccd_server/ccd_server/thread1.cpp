/*
�����߳̽���ȫ�ֱ����ṹ�� pass_struct�ĵ�ַ����ȡ�ļ�·������Ƭ������������ͼƬ�����Աȣ�ʼ�ձ�����һ��ԭʼ��Ƭ��ÿһ�ź͵�һ��ԭʼ��Ƭ�Աȣ�
��������ƣ���������µ�ȫ�ֱ����У������̼߳���Ǹ�ȫ�ֱ���
*/


#include "stdafx.h"
#include<Windows.h>
#include<iostream>

#include<vector>
#include"thread1.h"

//opencv����ļ�
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2\opencv.hpp"


using namespace std;
using namespace cv;

extern struct mystruct pass_struct;//�������ݸ����̵߳Ľṹ��
extern vector<int> para;

//�����mat�ǵ�ͨ��ͼƬ��limit������ֵ������ֵ�ǵ�ͨ��ͼ�У�����ֵ����limit�����ص������Խ��ͬ��Խ��
int bSums(Mat src, uchar limit);


unsigned int WINAPI thread1(void* arg) {
	string original_file_name;
	Mat original_image;
	int current_grabbedImages;
		
	if (pass_struct.grabbedImages == 1) {
		original_file_name = pass_struct.image_path;
		original_image = imread(original_file_name, IMREAD_GRAYSCALE).clone();//����Ҷ�ͼ��ʼ�ձ���һ��
		current_grabbedImages = 1;

	}
	while (true) {
		if (pass_struct.grabbedImages > current_grabbedImages) {//��ȫ����������ļ������ڵ�ǰ����ʱ��˵�����¹�һ��
			string current_file_name = pass_struct.image_path;
			Mat new_image = imread(current_file_name,IMREAD_GRAYSCALE);//��ѭ���ڲ����壨������Ϊ�˷�ֹ��ֻ��Ϊ�����þֲ��������Զ�����
			Mat diff_image;
			absdiff(original_image,new_image,diff_image);
			if (bSums(diff_image, 40) > 1000) {
				imwrite(current_file_name.substr(0, current_file_name.length() - 4) + "_diff.jpg", diff_image, para);
				pass_struct.result = false;
			}
			
			







			current_grabbedImages = pass_struct.grabbedImages;//���µ�ǰ������������
		}
		else {

			Sleep(200);//��������ѭ��ѭ���Ĺ��죬�����������
		}



		
	}

	



	return 0;
}


/******************************************************************************/

int bSums(Mat src,uchar limit)
{

	int counter = 0;
	//�������������ص�
	Mat_<uchar>::iterator it = src.begin<uchar>();
	Mat_<uchar>::iterator itend = src.end<uchar>();
	for (; it != itend; ++it)
	{
		if ((*it) >limit) counter += 1;//��ֵ�������ص���0����255
	}
	return counter;
}