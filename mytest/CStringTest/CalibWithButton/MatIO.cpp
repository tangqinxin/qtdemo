#include"MatIO.h"

//该函数输入流使用引用
void WriteMatToFile(fstream& file, Mat& mat) {
	switch (mat.type())
	{
	case(CV_32FC1):
		for (int i = 0; i< mat.rows; i++) {
			for (int j = 0; j < mat.cols; j++) {
				file << mat.at<float>(i, j) << " ";
			}
			file << endl;
		}
		break;
	case(CV_64FC1):
		for (int i = 0; i< mat.rows; i++) {
			for (int j = 0; j < mat.cols; j++) {
				file << mat.at<double>(i, j) << " ";
			}
			file << endl;
		}
	default:
		break;
	}

}


//此函数使用index作为位置判断，不使用引用
void ReadMatFromFile(fstream& file, Mat& mat, int Index) {
	bool flag_ok = false;//判定是否到达指定矩阵
	switch (mat.type())
	{
	case(CV_32FC1):
		float ftemp;
		for (int k = 0; k < Index; k++) {
			if (k == Index - 1) {
				flag_ok = true;
			}
			for (int i = 0; i < mat.rows; i++) {
				for (int j = 0; j < mat.cols; j++) {
					if (flag_ok) {
						file >> mat.at<float>(i, j);
					}
					else {
						file >> ftemp;//如果没有到达，直接读入temp舍弃
					}
				}
			}
		}
		break;
	case(CV_64FC1):
		double dtemp;
		for (int k = 0; k < Index; k++) {
			if (k == Index - 1) {
				flag_ok = true;
			}
			for (int i = 0; i < mat.rows; i++) {
				for (int j = 0; j < mat.cols; j++) {
					if (flag_ok) {
						file >> mat.at<double>(i, j);
					}
					else {
						file >> dtemp;//如果没有到达，直接读入temp舍弃
					}
				}
			}
		}
		break;
	default:
		break;
	}
}


// 输入文件名，读取index矩阵到mat
void ReadMatFromFileName(string FileName, Mat& mat, int Index) {
	fstream file;
	file.open(FileName);
	ReadMatFromFile(file, mat, Index);
	file.close();
}