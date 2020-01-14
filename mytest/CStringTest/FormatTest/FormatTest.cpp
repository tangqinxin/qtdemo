#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <fstream>
#include<vector>
#include <iomanip>
#include"MatIO.h"

using namespace cv;
using namespace std;

int main() {
	//fstream file;
	//file.open("RMat.txt");
	//for (int i = 0; i < 3; i++) {
	//	for (int j = 0; j < 3; j++) {
	//		file >> R.at<float>(i, j);
	//	}
	//}
	//cout << R;

	//file.open("RMat.txt", ios::app);
	//WriteMatToFile(file, R);
	//file.close();//����Ҫ�ȹص��ٴ�
	//file.open("RMat.txt", ios::app);
	//WriteMatToFile(file, R2);

	/* *�ڶ��ε������Format���� 
	fstream file;
	file.open("RMat.txt");
	//���txt�е�����
	fstream f2;
	f2.open("RMat.txt", ios::out | ios::trunc);
	vector<Mat> Rvec;
	for (int i = 0; i < 10; i++) {
		Mat R = Mat(3, 3, CV_32FC1, Scalar::all(i));
		WriteMatToFile(file, R);
		file << endl;
		Rvec.push_back(R);
	}

	Mat InR = Mat::zeros(3, 3, CV_32FC1);
	fstream infile;
	infile.open("RMat.txt");
	ReadMatFromFile(infile, InR, 7);
	ReadMatFromFile(infile, InR, 2);

	cout << InR<<endl;
	cout << Rvec[3] << endl;//�˴���������Ԫ��ֵΪ3��˵��vector��pushbackȷʵ������һ�������ȥ

	ReadMatFromFile(infile, Rvec[2], 2);
	cout << Rvec[2] << endl;//�˴����[8,8,8]˵���ܵ������õ�Ӱ��

	Mat InR2 = Mat::zeros(3, 3, CV_32FC1);
	string FileName= "RMat.txt";
	ReadMatFromFileName(FileName, InR2, 7);
	cout << InR2 << endl;
	ReadMatFromFileName(FileName, InR2, 2);
	cout << InR2 << endl;
	*/

	Mat R = Mat::zeros(3, 3, CV_64FC1);

	string filename = "RMat.txt";
	ReadMatFromFileName(filename, R, 2);//��ȡ�ڶ�����ת����
	cout << std::fixed << std::setprecision(12);
	cout << R << endl;

	return 0;
}