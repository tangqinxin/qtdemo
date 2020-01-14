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
	//���ļ��еĵ����������
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
	p.PlaneFitting(CamPts);//ƽ��ϵ�����
	p.DisplayCoeffs();//��ӡ����

	// ������������׼��
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
	// �ڲξ���׼��
	Mat matlabM = Mat::zeros(3, 3, CV_64FC1);
	ReadMatFromFileName("matlabM.txt", matlabM, 1);
	matlabM = matlabM.t();//matlab����Ҫ����ת��
	// ����Ľ������
	vector<Point3f> MeasurePtsInC;
	p.ComputeImgPtOnPlane(matlabM, ImgPts, MeasurePtsInC);

	cout << "�������" << endl;
	cout << MeasurePtsInC << endl;

	// ֱ�����
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

	// �㼯��ʹ��ֱ����ϵĵ㼯���������ǵ�ֱ�ߵľ���
	// Point3f��float����,Point3d��double����
	vector<float> DistSet;
	cout << "�������" << endl;
	p.ComputePtDistToBaseLine(FitPtSet, DistSet);
	for (int i = 0; i < DistSet.size(); i++) {
		cout << DistSet[i] << endl;
	}

	//���Ծ�������Ƿ����
	cout << "������֪�㣬�����Ƿ����" << endl;
	Point3f pt(0, 0, 0);
	p.SetBaseLineParams(1, 0, 0, pt);// �趨����Ϊ��0��ģ�����Ϊ(1,0,0)����
	Point3f pt2(0, 1, 1);//�����Ե�
	vector<Point3f> ptset2;
	vector<float> DistSet2;
	ptset2.push_back(pt2);// Ҫ�����ĵ���pt2,����洢��DistSet2��
	p.ComputePtDistToBaseLine(ptset2, DistSet2);
	for (int i = 0; i < DistSet2.size(); i++) {
		cout << DistSet2[i] << endl;
	}

	//���Ե�����뺯������
	cout << "������뺯��" << endl;
	Point3f testpt1(1, 1, 0);
	Point3f testpt2(0, 1, 1);	
	Point3f testpt3(0, 1, 2);
	
	cout << p.PointToBaseLine(testpt1) << endl;
	cout << p.PointToBaseLine(testpt2) << endl;
	cout << p.PointToBaseLine(testpt3) << endl;

	return 0;
}