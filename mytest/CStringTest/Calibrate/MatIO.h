#ifndef _MATIO
#define _MATIO
#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
// 将Mat写到txt文件中
// 写入之前要先将mat的size给确定好，常用Mat::zeros(r,c)来进行初始化
void WriteMatToFile(fstream& file, Mat& mat);

// 将Mat读入到对应的矩阵中
// 读入之前要先将mat的size给确定好，常用Mat::zeros(r,c)来进行初始化
// Index的序号需要进行确定，从1开始，确定读入第几个矩阵
// 此函数使用index作为位置判断，不使用引用
void ReadMatFromFile(fstream& file, Mat& mat, int Index);

// 输入文件名，读取index矩阵到mat
// 没有使用引用，推荐使用该函数进行读取
void ReadMatFromFileName(string FileName, Mat& mat, int Index);

#endif // !_MATIO


