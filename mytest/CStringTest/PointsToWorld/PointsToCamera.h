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

/*  给定外参R，t，将点从棋盘坐标系投影到相机坐标系中

棋盘坐标系点云-I
外参矩阵序列RMatVec-I
相机平移矩阵序列-I
相机坐标系坐标-O
*/
void PointsToCamera(vector<Point3f>& PointsWorldCoordinate, Mat& R, Mat& t, vector<Point3f>& PointsCameraCoordinate);

double TwoPointsDist(Point3f& pt1, Point3f& pt2);

#endif // !_POINTSTOCAMERA
