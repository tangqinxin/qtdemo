#include<iostream>
#include"ImgProFun.h"
#pragma warning(disable:4996)

using namespace std;
using namespace cv;


int main() {
	// 第一部分，图像预处理
	//Mat SrcImg=imread("D:\\StructureLine Project\\build-RailMeasurement-Desktop_Qt_5_9_0_MSVC2015_64bit-Debug\\data\\test.bmp", IMREAD_GRAYSCALE);//qt的数据
	Mat SrcImg = imread("D:\\结构光实验数据\\2019-11-20\\1_nl.bmp", IMREAD_GRAYSCALE);//使用实验的数据
	cvNamedWindow("原图", CV_WINDOW_NORMAL);
	imshow("原图", SrcImg);
	Mat ImgAfterProcess = FillterSmallArea(SrcImg, 300,0.2);//用我的方法先进行预处理

	// 第二部分，中心条纹提取
	//Mat SrcImg = imread("D:\\结构光实验数据\\2019-11-20\\1_nl.bmp", IMREAD_GRAYSCALE);
	cout<<StegerLine(ImgAfterProcess).size()<<endl;
	
	waitKey(0);
	return 0;
}

