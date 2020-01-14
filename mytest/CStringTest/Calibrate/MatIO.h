#ifndef _MATIO
#define _MATIO
#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
// ��Matд��txt�ļ���
// д��֮ǰҪ�Ƚ�mat��size��ȷ���ã�����Mat::zeros(r,c)�����г�ʼ��
void WriteMatToFile(fstream& file, Mat& mat);

// ��Mat���뵽��Ӧ�ľ�����
// ����֮ǰҪ�Ƚ�mat��size��ȷ���ã�����Mat::zeros(r,c)�����г�ʼ��
// Index�������Ҫ����ȷ������1��ʼ��ȷ������ڼ�������
// �˺���ʹ��index��Ϊλ���жϣ���ʹ������
void ReadMatFromFile(fstream& file, Mat& mat, int Index);

// �����ļ�������ȡindex����mat
// û��ʹ�����ã��Ƽ�ʹ�øú������ж�ȡ
void ReadMatFromFileName(string FileName, Mat& mat, int Index);

#endif // !_MATIO


