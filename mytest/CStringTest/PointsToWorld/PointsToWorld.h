#ifndef _POINTSTOWORLD
#define _POINTSTOWORLD

#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include<iostream>
#include <vector>
using namespace cv;
using namespace std;

/*  给定外参R，t，将点投影到世界坐标系（棋盘系）中
	相机内参M-I
	外参矩阵序列RMatVec-I
	相机平移矩阵序列-I
	像素点序列ImgPointsSet-I
	世界坐标系WorldCoordinate-O
	点误差TotalError-O
*/
void PointsToWorld(Mat& M,Mat& R,Mat& tVec,vector<Point2f>& ImgPointsSet,vector<Point3f>& WorldCoordinate,vector<float>&TotalError);



#endif // !_POINTSTOWORLD

