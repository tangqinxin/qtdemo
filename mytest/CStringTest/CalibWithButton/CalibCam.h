#ifndef CALIBCAM_H
#define CALIBCAM_H
#include<string>
#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "MatIO.h"
#include "myfun.h"
using namespace std;
using namespace cv;

// 棋盘角点的排列m_width X m_height - I
// 棋盘每个格子的边长m_square_size - I
// 读取照片的信息txt文件strFileName - I
void CalibWithButton(std::string strFileName, int m_width, int m_height, float m_square_size);//从StrFileName里面去读取要标定的参数的文件名

#endif //!CALIBCAM_H