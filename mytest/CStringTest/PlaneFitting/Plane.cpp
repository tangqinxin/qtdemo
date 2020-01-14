#include"Plane.h"

LaserPlane::LaserPlane() {
	PCoeffs.a0 = 0;
	PCoeffs.a1 = 0;
	PCoeffs.a2 = 0;
	BaseLine.kx = 0;
	BaseLine.ky = 0;
	BaseLine.kz = 0;
	BaseLine.PassPt.x = 0;
	BaseLine.PassPt.y = 0;
	BaseLine.PassPt.z = 0;
}

LaserPlane::~LaserPlane() {}

// ������ά���Ƽ��Ͻ������ƽ��
// Z=a0*X+a1*Y+a2;
void LaserPlane::PlaneFitting(vector<Point3f> &PointsCameraCoordinate) {
	int NumOfPts = PointsCameraCoordinate.size();
	Mat A = Mat::ones(NumOfPts, 3, CV_64FC1);//[Xci,Yci,1]*[a0,a1,a2]'=[Zci]
	Mat b = Mat::zeros(NumOfPts, 1, CV_64FC1);

	for (int i = 0; i < NumOfPts; i++) {
		*A.ptr<double>(i, 0) = PointsCameraCoordinate[i].x;
		*A.ptr<double>(i, 1) = PointsCameraCoordinate[i].y;//A�ĵ�����ͳһΪ1

		*b.ptr<double>(i, 0) = PointsCameraCoordinate[i].z;
	}
	Mat a_coefficient = (A.t()*A).inv()*(A.t()*b);
	for (int i = 0; i < 3; i++) {
		PCoeffs.a0 = a_coefficient.at<double>(0, 0);
		PCoeffs.a1 = a_coefficient.at<double>(1, 0);
		PCoeffs.a2 = a_coefficient.at<double>(2, 0);
	}
}

//��ռ���ƽ���ϵ��
void LaserPlane::CleanCoeffs() {
	PCoeffs.a0 = 0;
	PCoeffs.a1 = 0;
	PCoeffs.a2 = 0;
	BaseLine.kx = 0;
	BaseLine.ky = 0;
	BaseLine.kz = 0;
	BaseLine.PassPt.x = 0;
	BaseLine.PassPt.y = 0;
	BaseLine.PassPt.z = 0;
}

// ��������ڲΣ���������㼯������������������ϵ�µ���ά����
// ����������������Ĳ����������
/* 
	a0*Xc+a1*Yc+a2=Zc;
	Zc*(a0*Xn+a1*Yn-1)=-a2;
	Zc=-a2/(a0*Xn+a1*Yn-1);
*/
void LaserPlane::ComputeImgPtOnPlane(Mat& M, vector<Point2f>& ImgPtSet, vector<Point3f>& PtsInCamera)
{
	int NumOfPts = ImgPtSet.size();
	//����uvPoints����
	Mat uvPts = Mat::ones(3, NumOfPts, CV_64FC1);
	for (int i = 0; i < NumOfPts; i++) {
		*uvPts.ptr<double>(0, i) = ImgPtSet[i].x;
		*uvPts.ptr<double>(1, i) = ImgPtSet[i].y;
	}
	// ������ξ���
	Mat XnYnMat = M.inv()*uvPts;
	
	// �����������
	vector<double> LineZc(NumOfPts, 0);
	vector<double> LineXc(NumOfPts, 0);
	vector<double> LineYc(NumOfPts, 0);
	double Xn;//��ʱ����
	double Yn;

	// �������ص��ڿռ������
	for (int i = 0; i < NumOfPts; i++) {
		Xn = *XnYnMat.ptr<double>(0, i);
		Yn = *XnYnMat.ptr<double>(1, i);
		LineZc[i] = (-1 * PCoeffs.a2) / (PCoeffs.a0*Xn + PCoeffs.a1*Yn - 1);
		LineXc[i] = Xn*LineZc[i];
		LineYc[i] = Yn*LineZc[i];
		Point3f tempPt;
		tempPt.x = LineXc[i];
		tempPt.y = LineYc[i];
		tempPt.z = LineZc[i];
		PtsInCamera.push_back(tempPt);
	}
}


//�趨һ����׼����Ϊ�ο�����
void LaserPlane::SetBaseLine(Mat& M, vector<Point2f>& BaseLineImgPtSet) {
	//Ԥ��һ������װbaseline��point3f
	vector<Point3f> PtsInCamera;

	//����baseline��point3f
	ComputeImgPtOnPlane(M, BaseLineImgPtSet, PtsInCamera);

	//���ÿռ���ƽ������û���
	SetBaseLineWith3DPts(PtsInCamera);
}


//���ÿռ��еĵ��ƽ������û���
void LaserPlane::SetBaseLineWith3DPts(vector<Point3f>& PtsInCamera)
{
	int NumOfPts = PtsInCamera.size();
	Mat A1 = Mat::ones(NumOfPts, 2,CV_64FC1);
	Mat b1 = Mat::zeros(NumOfPts, 1, CV_64FC1);
	Mat A2 = Mat::ones(NumOfPts, 2, CV_64FC1);
	Mat b2 = Mat::zeros(NumOfPts, 1, CV_64FC1);

	// ����A1��b1,A2,b2
	for (int i = 0; i < NumOfPts; i++) {
		*A1.ptr<double>(i, 0) = PtsInCamera[i].z;
		*A2.ptr<double>(i, 0) = PtsInCamera[i].z;
		*b1.ptr<double>(i, 0) = PtsInCamera[i].x;
		*b2.ptr<double>(i, 0) = PtsInCamera[i].y;
	}

	Mat k1b1 = (A1.t()*A1).inv()*(A1.t()*b1);
	Mat k2b2 = (A2.t()*A2).inv()*(A2.t()*b2);
	BaseLine.kx = *k1b1.ptr<double>(0, 0);
	BaseLine.ky = *k2b2.ptr<double>(0, 0);
	BaseLine.kz = 1;
	// ������������λ��
	double VecLen = sqrt(BaseLine.kx*BaseLine.kx + BaseLine.ky*BaseLine.ky + BaseLine.kz*BaseLine.kz);
	BaseLine.kx = BaseLine.kx/VecLen;
	BaseLine.ky = BaseLine.ky/VecLen;
	BaseLine.kz = BaseLine.kz/VecLen;

	BaseLine.PassPt.x= *k1b1.ptr<double>(1, 0);
	BaseLine.PassPt.y = *k2b2.ptr<double>(1, 0);
	BaseLine.PassPt.z = 0;
}

// PtΪ������һ��M1�����߹�M0����������Ϊ[kx,ky,kz]
float LaserPlane::PointToBaseLine(Point3f & Pt)
{
	float SingleDist = 0;
	Point3f VecM0ToM1;

	Point3f DirUnitVec;
	DirUnitVec.x = BaseLine.kx;
	DirUnitVec.y = BaseLine.ky;
	DirUnitVec.z = BaseLine.kz;

	//�趨������һ��
	VecM0ToM1 = Pt - BaseLine.PassPt;
	
	//����ͶӰ����
	Point3f ProjVecM0ToM1 = VecM0ToM1.ddot(DirUnitVec)*DirUnitVec;

	//���㴹ֱ����
	Point3f PerpendicularVecOfM0ToM1 = VecM0ToM1 - ProjVecM0ToM1;

	//���㴹ֱ����
	SingleDist = NormPoint3f(PerpendicularVecOfM0ToM1);
	return SingleDist;
}

// ����㼯����������
// ����Ҫ�������һ��ΪM1,���߹�M0,��������Ϊ[kx,ky,kz]
void LaserPlane::ComputePtDistToBaseLine(vector<Point3f>& PtSet, vector<float>& DistIn3D) {
	int NumOfPts = PtSet.size();
	float SingleDist=0;
	Point3f VecM0ToM1;

	Point3f DirUnitVec;
	DirUnitVec.x = BaseLine.kx;
	DirUnitVec.y = BaseLine.ky;
	DirUnitVec.z = BaseLine.kz;

	for (int i = 0; i < NumOfPts; i++) {
		VecM0ToM1 = PtSet[i] - BaseLine.PassPt;
		//����ͶӰ����
		Point3f ProjVecM0ToM1 = VecM0ToM1.ddot(DirUnitVec)*DirUnitVec;
		
		//���㴹ֱ����
		Point3f PerpendicularVecOfM0ToM1 = VecM0ToM1 - ProjVecM0ToM1;
		
		//���㴹ֱ����
		SingleDist = NormPoint3f(PerpendicularVecOfM0ToM1);
		DistIn3D.push_back(SingleDist);
	}
}

//��ӡƽ�����
void LaserPlane::DisplayCoeffs()
{
	std::cout << "a0= "<<PCoeffs.a0 << std::endl;
	std::cout << "a1= " << PCoeffs.a1 << std::endl;
	std::cout << "a2= " << PCoeffs.a2 << std::endl;
}

//��ӡ������Ϣ
void LaserPlane::DisplayBaseLine()
{
	std::cout << "������Ϊ" << std::endl;
	std::cout << "kx=" << BaseLine.kx << std::endl;
	std::cout << "ky=" << BaseLine.ky << std::endl;
	std::cout << "kz=" << BaseLine.kz << std::endl;
	std::cout << "ͨ���ĵ�����" << std::endl;
	std::cout << BaseLine.PassPt<< std::endl;
}

//��ά������norm
float LaserPlane::NormPoint3f(Point3f & vec)
{
	double Norm = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	return Norm;
}

// �Լ��趨��׼�ߵĲ���
void LaserPlane::SetBaseLineParams(float _kx, float _ky, float _kz, Point3f & pt)
{
	BaseLine.kx = _kx;
	BaseLine.ky = _ky;
	BaseLine.kz = _kz;
	BaseLine.PassPt = pt;
}

//���漤��ƽ�����
void LaserPlane::SavePlaneParams()
{
	MakeMyDir(".\\CameraCalibrate");
	string FilePath = ".\\CameraCalibrate";
	string FileName1 = FilePath + "\\" + "PlaneCoeffs.txt";
	string FileName2 = FilePath + "\\" + "BaseLineInfo.txt";
	std::fstream PlaneParamStream(FileName1);
	std::fstream PlaneBaseLineStream(FileName2);
	PlaneParamStream << PCoeffs.a0 << " " << PCoeffs.a1 << " " << PCoeffs.a2 << std::endl;
	PlaneBaseLineStream << BaseLine.kx << std::endl;
	PlaneBaseLineStream << BaseLine.ky << std::endl;
	PlaneBaseLineStream << BaseLine.kz << std::endl;
	PlaneBaseLineStream << BaseLine.PassPt.x << " " << BaseLine.PassPt.y << " " << BaseLine.PassPt.z << std::endl;
}
