#include"PointsToCamera.h"

void PointsToCamera(vector<Point3f>& PointsWorldCoordinate, Mat& R, Mat& t, vector<Point3f>& PointsCameraCoordinate) 
{
	Mat tempMat = Mat::zeros(3, 1, CV_32FC1);
	for (int i = 0; i < PointsWorldCoordinate.size(); i++) {
		*tempMat.ptr<float>(0, 0) = PointsWorldCoordinate[i].x;
		*tempMat.ptr<float>(1, 0) = PointsWorldCoordinate[i].y;
		*tempMat.ptr<float>(2, 0) = PointsWorldCoordinate[i].z;
		tempMat = R*tempMat + t;//计算{C}下的坐标
		Point3f tempPoint;
		tempPoint.x = *tempMat.ptr<float>(0, 0);
		tempPoint.y = *tempMat.ptr<float>(1, 0);
		tempPoint.z = *tempMat.ptr<float>(2, 0);
		PointsCameraCoordinate.push_back(tempPoint);
	}
}

double TwoPointsDist(Point3f& pt1, Point3f& pt2) {
	double res = 0;
	double dx = pt1.x - pt2.x;
	double dy = pt1.y - pt2.y;
	double dz = pt1.z - pt2.z;
	res = sqrt(dx*dx + dy*dy + dz*dz);
	return res;
}
