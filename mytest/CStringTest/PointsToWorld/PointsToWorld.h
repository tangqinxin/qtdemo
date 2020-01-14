#ifndef _POINTSTOWORLD
#define _POINTSTOWORLD

#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include<iostream>
#include <vector>
using namespace cv;
using namespace std;

/*  �������R��t������ͶӰ����������ϵ������ϵ����
	����ڲ�M-I
	��ξ�������RMatVec-I
	���ƽ�ƾ�������-I
	���ص�����ImgPointsSet-I
	��������ϵWorldCoordinate-O
	�����TotalError-O
*/
void PointsToWorld(Mat& M,Mat& R,Mat& tVec,vector<Point2f>& ImgPointsSet,vector<Point3f>& WorldCoordinate,vector<float>&TotalError);



#endif // !_POINTSTOWORLD

