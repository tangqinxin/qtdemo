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
	//file.close();//必须要先关掉再打开
	//file.open("RMat.txt", ios::app);
	//WriteMatToFile(file, R2);

	/* *第二次调整后的Format代码 
	fstream file;
	file.open("RMat.txt");
	//清空txt中的内容
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
	cout << Rvec[3] << endl;//此处的向量的元素值为3，说明vector的pushback确实拷贝了一个矩阵进去

	ReadMatFromFile(infile, Rvec[2], 2);
	cout << Rvec[2] << endl;//此处输出[8,8,8]说明受到了引用的影响

	Mat InR2 = Mat::zeros(3, 3, CV_32FC1);
	string FileName= "RMat.txt";
	ReadMatFromFileName(FileName, InR2, 7);
	cout << InR2 << endl;
	ReadMatFromFileName(FileName, InR2, 2);
	cout << InR2 << endl;
	*/

	Mat R = Mat::zeros(3, 3, CV_64FC1);

	string filename = "RMat.txt";
	ReadMatFromFileName(filename, R, 2);//读取第二个旋转矩阵
	cout << std::fixed << std::setprecision(12);
	cout << R << endl;

	return 0;
}