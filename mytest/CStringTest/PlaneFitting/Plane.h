#ifndef  PLANE_H
#define  PLANE_H

#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include<iostream>
#include <vector>
#include<math.h>
#include"myfun.h"
#include<fstream>
using namespace cv;

class LaserPlane {
private:
	typedef struct {
		//Z=a0*X+a1*Y+a2;
		double a0;
		double a1;
		double a2;
	}PlaneCoeffs;
	typedef struct {
		double kx;
		double ky;
		double kz;
		Point3f PassPt;
	}Line;
	Line BaseLine;
	PlaneCoeffs PCoeffs;

public:
	LaserPlane();
	~LaserPlane();
	void PlaneFitting(vector<Point3f> &PointsCameraCoordinate);//利用三维点云集合进行拟合平面
	void CleanCoeffs();//清空激光平面的系数
	void ComputeImgPtOnPlane(Mat& M,vector<Point2f>& ImgPtSet,vector<Point3f>& PtsInCamera);//输入相机内参，像素坐标点集，输出他们在相机坐标系下的三维坐标
	void SetBaseLine(Mat& M, vector<Point2f>& BaseLineImgPtSet);//设定一条基准线作为参考坐标
	void SetBaseLineWith3DPts(vector<Point3f>& PtsInCamera);//利用空间中的三维点云坐标设置基线
	float PointToBaseLine(Point3f& Pt);//计算单点Pt到基线的距离
	void ComputePtDistToBaseLine(vector<Point3f>& PtSet, vector<float>& DistIn3D);//计算空间中的点集到直线的距离
	void DisplayCoeffs();//打印平面参数
	void DisplayBaseLine();//打印baseline的信息
	float NormPoint3f(Point3f& vec);//计算向量的距离
	void SetBaseLineParams(float _kx, float _ky, float _kz, Point3f& pt);//人为设定基线参数
	void SavePlaneParams();//保存激光平面参数

};


#endif // ! PLANE_H
