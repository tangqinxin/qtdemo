#include<iostream>
#include"ImgProFun.h"
#pragma warning(disable:4996)

using namespace std;
using namespace cv;


int main() {
	// ��һ���֣�ͼ��Ԥ����
	//Mat SrcImg=imread("D:\\StructureLine Project\\build-RailMeasurement-Desktop_Qt_5_9_0_MSVC2015_64bit-Debug\\data\\test.bmp", IMREAD_GRAYSCALE);//qt������
	Mat SrcImg = imread("D:\\�ṹ��ʵ������\\2019-11-20\\1_nl.bmp", IMREAD_GRAYSCALE);//ʹ��ʵ�������
	cvNamedWindow("ԭͼ", CV_WINDOW_NORMAL);
	imshow("ԭͼ", SrcImg);
	Mat ImgAfterProcess = FillterSmallArea(SrcImg, 300,0.2);//���ҵķ����Ƚ���Ԥ����

	// �ڶ����֣�����������ȡ
	//Mat SrcImg = imread("D:\\�ṹ��ʵ������\\2019-11-20\\1_nl.bmp", IMREAD_GRAYSCALE);
	cout<<StegerLine(ImgAfterProcess).size()<<endl;
	
	waitKey(0);
	return 0;
}

