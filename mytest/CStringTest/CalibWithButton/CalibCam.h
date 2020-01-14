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

// ���̽ǵ������m_width X m_height - I
// ����ÿ�����ӵı߳�m_square_size - I
// ��ȡ��Ƭ����Ϣtxt�ļ�strFileName - I
void CalibWithButton(std::string strFileName, int m_width, int m_height, float m_square_size);//��StrFileName����ȥ��ȡҪ�궨�Ĳ������ļ���

#endif //!CALIBCAM_H