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
	void PlaneFitting(vector<Point3f> &PointsCameraCoordinate);//������ά���Ƽ��Ͻ������ƽ��
	void CleanCoeffs();//��ռ���ƽ���ϵ��
	void ComputeImgPtOnPlane(Mat& M,vector<Point2f>& ImgPtSet,vector<Point3f>& PtsInCamera);//��������ڲΣ���������㼯������������������ϵ�µ���ά����
	void SetBaseLine(Mat& M, vector<Point2f>& BaseLineImgPtSet);//�趨һ����׼����Ϊ�ο�����
	void SetBaseLineWith3DPts(vector<Point3f>& PtsInCamera);//���ÿռ��е���ά�����������û���
	float PointToBaseLine(Point3f& Pt);//���㵥��Pt�����ߵľ���
	void ComputePtDistToBaseLine(vector<Point3f>& PtSet, vector<float>& DistIn3D);//����ռ��еĵ㼯��ֱ�ߵľ���
	void DisplayCoeffs();//��ӡƽ�����
	void DisplayBaseLine();//��ӡbaseline����Ϣ
	float NormPoint3f(Point3f& vec);//���������ľ���
	void SetBaseLineParams(float _kx, float _ky, float _kz, Point3f& pt);//��Ϊ�趨���߲���
	void SavePlaneParams();//���漤��ƽ�����

};


#endif // ! PLANE_H
