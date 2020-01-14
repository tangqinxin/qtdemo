#ifndef _POINTSTOCAMERA
#define _POINTSTOCAMERA
#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include<iostream>
#include <vector>
#include<math.h>
using namespace cv;
using namespace std;

/*  �������R��t���������������ϵͶӰ���������ϵ��

��������ϵ����-I
��ξ�������RMatVec-I
���ƽ�ƾ�������-I
�������ϵ����-O
*/
void PointsToCamera(vector<Point3f>& PointsWorldCoordinate, Mat& R, Mat& t, vector<Point3f>& PointsCameraCoordinate);

double TwoPointsDist(Point3f& pt1, Point3f& pt2);

#endif // !_POINTSTOCAMERA
