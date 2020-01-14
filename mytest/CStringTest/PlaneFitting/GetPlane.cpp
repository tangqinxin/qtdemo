#include <iostream>
#include "Plane.h"
#include <vector>
#include <fstream>
#include "MatIO.h"
using namespace std;
int main() {
	LaserPlane p;
	fstream fileA,fileb;
	fileA.open("PlaneFitA.txt");
	fileb.open("PlaneFitb.txt");
	vector<Point3f> CamPts;
	int numofpts = 10067;
	//将文件中的点读入向量中
	for (int i = 0; i < numofpts; i++) {
		Point3f temppt;
		float x;
		float y;
		float abandon;
		float z;
		fileA >> x;
		fileA >> y;
		fileA >> abandon;
		fileb >> z;
		temppt.x = x;
		temppt.y = y;
		temppt.z = z;
		CamPts.push_back(temppt);
	}
	p.PlaneFitting(CamPts);//平面系数拟合
	p.DisplayCoeffs();//打印参数

	// 待测量中心线准备
	fstream MeasureLineFile;
	MeasureLineFile.open("MidLineImgPts.txt");
	vector<Point2f> ImgPts;
	int NumOfLinePts=1671;
	for (int i = 0; i < NumOfLinePts; i++) {
		Point2f SingleImgPt;
		MeasureLineFile >> SingleImgPt.x;
		MeasureLineFile >> SingleImgPt.y;
		ImgPts.push_back(SingleImgPt);
	}
	// 内参矩阵准备
	Mat matlabM = Mat::zeros(3, 3, CV_64FC1);
	ReadMatFromFileName("matlabM.txt", matlabM, 1);
	matlabM = matlabM.t();//matlab矩阵要进行转置
	// 输出的结果向量
	vector<Point3f> MeasurePtsInC;
	p.ComputeImgPtOnPlane(matlabM, ImgPts, MeasurePtsInC);

	cout << "测量结果" << endl;
	cout << MeasurePtsInC << endl;

	// 直线拟合
	fstream FittingPtsFile;
	FittingPtsFile.open("matlabPts.txt");
	vector<Point3f> FitPtSet;
	int NumOfFitPtSet = 10;
	
	for (int i = 0; i < NumOfFitPtSet; i++) {
		Point3f TempFitPt;
		FittingPtsFile >> TempFitPt.x;
		FittingPtsFile >> TempFitPt.y;
		FittingPtsFile >> TempFitPt.z;
		FitPtSet.push_back(TempFitPt);
	}

	p.SetBaseLineWith3DPts(FitPtSet);
	p.DisplayBaseLine();

	// 点集合使用直线拟合的点集，计算他们到直线的距离
	// Point3f是float类型,Point3d是double类型
	vector<float> DistSet;
	cout << "距离计算" << endl;
	p.ComputePtDistToBaseLine(FitPtSet, DistSet);
	for (int i = 0; i < DistSet.size(); i++) {
		cout << DistSet[i] << endl;
	}

	//测试距离计算是否合理
	cout << "输入已知点，测试是否合理" << endl;
	Point3f pt(0, 0, 0);
	p.SetBaseLineParams(1, 0, 0, pt);// 设定基线为过0点的，方向为(1,0,0)的线
	Point3f pt2(0, 1, 1);//被测试点
	vector<Point3f> ptset2;
	vector<float> DistSet2;
	ptset2.push_back(pt2);// 要测量的点是pt2,结果存储在DistSet2中
	p.ComputePtDistToBaseLine(ptset2, DistSet2);
	for (int i = 0; i < DistSet2.size(); i++) {
		cout << DistSet2[i] << endl;
	}

	//测试单点距离函数函数
	cout << "单点距离函数" << endl;
	Point3f testpt1(1, 1, 0);
	Point3f testpt2(0, 1, 1);	
	Point3f testpt3(0, 1, 2);
	
	cout << p.PointToBaseLine(testpt1) << endl;
	cout << p.PointToBaseLine(testpt2) << endl;
	cout << p.PointToBaseLine(testpt3) << endl;

	return 0;
}