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

// 利用三维点云集合进行拟合平面
// Z=a0*X+a1*Y+a2;
void LaserPlane::PlaneFitting(vector<Point3f> &PointsCameraCoordinate) {
	int NumOfPts = PointsCameraCoordinate.size();
	Mat A = Mat::ones(NumOfPts, 3, CV_64FC1);//[Xci,Yci,1]*[a0,a1,a2]'=[Zci]
	Mat b = Mat::zeros(NumOfPts, 1, CV_64FC1);

	for (int i = 0; i < NumOfPts; i++) {
		*A.ptr<double>(i, 0) = PointsCameraCoordinate[i].x;
		*A.ptr<double>(i, 1) = PointsCameraCoordinate[i].y;//A的第三列统一为1

		*b.ptr<double>(i, 0) = PointsCameraCoordinate[i].z;
	}
	Mat a_coefficient = (A.t()*A).inv()*(A.t()*b);
	for (int i = 0; i < 3; i++) {
		PCoeffs.a0 = a_coefficient.at<double>(0, 0);
		PCoeffs.a1 = a_coefficient.at<double>(1, 0);
		PCoeffs.a2 = a_coefficient.at<double>(2, 0);
	}
}

//清空激光平面的系数
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

// 输入相机内参，像素坐标点集，输出他们在相机坐标系下的三维坐标
// 这个函数是用于最后的测量结果部分
/* 
	a0*Xc+a1*Yc+a2=Zc;
	Zc*(a0*Xn+a1*Yn-1)=-a2;
	Zc=-a2/(a0*Xn+a1*Yn-1);
*/
void LaserPlane::ComputeImgPtOnPlane(Mat& M, vector<Point2f>& ImgPtSet, vector<Point3f>& PtsInCamera)
{
	int NumOfPts = ImgPtSet.size();
	//构建uvPoints矩阵
	Mat uvPts = Mat::ones(3, NumOfPts, CV_64FC1);
	for (int i = 0; i < NumOfPts; i++) {
		*uvPts.ptr<double>(0, i) = ImgPtSet[i].x;
		*uvPts.ptr<double>(1, i) = ImgPtSet[i].y;
	}
	// 构建齐次矩阵
	Mat XnYnMat = M.inv()*uvPts;
	
	// 构建结果矩阵
	vector<double> LineZc(NumOfPts, 0);
	vector<double> LineXc(NumOfPts, 0);
	vector<double> LineYc(NumOfPts, 0);
	double Xn;//临时变量
	double Yn;

	// 计算像素点在空间的坐标
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


//设定一条基准线作为参考坐标
void LaserPlane::SetBaseLine(Mat& M, vector<Point2f>& BaseLineImgPtSet) {
	//预设一个容器装baseline的point3f
	vector<Point3f> PtsInCamera;

	//计算baseline的point3f
	ComputeImgPtOnPlane(M, BaseLineImgPtSet, PtsInCamera);

	//利用空间点云进行设置基线
	SetBaseLineWith3DPts(PtsInCamera);
}


//利用空间中的点云进行设置基线
void LaserPlane::SetBaseLineWith3DPts(vector<Point3f>& PtsInCamera)
{
	int NumOfPts = PtsInCamera.size();
	Mat A1 = Mat::ones(NumOfPts, 2,CV_64FC1);
	Mat b1 = Mat::zeros(NumOfPts, 1, CV_64FC1);
	Mat A2 = Mat::ones(NumOfPts, 2, CV_64FC1);
	Mat b2 = Mat::zeros(NumOfPts, 1, CV_64FC1);

	// 设置A1，b1,A2,b2
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
	// 将方向向量单位化
	double VecLen = sqrt(BaseLine.kx*BaseLine.kx + BaseLine.ky*BaseLine.ky + BaseLine.kz*BaseLine.kz);
	BaseLine.kx = BaseLine.kx/VecLen;
	BaseLine.ky = BaseLine.ky/VecLen;
	BaseLine.kz = BaseLine.kz/VecLen;

	BaseLine.PassPt.x= *k1b1.ptr<double>(1, 0);
	BaseLine.PassPt.y = *k2b2.ptr<double>(1, 0);
	BaseLine.PassPt.z = 0;
}

// Pt为基线外一点M1，基线过M0，方向向量为[kx,ky,kz]
float LaserPlane::PointToBaseLine(Point3f & Pt)
{
	float SingleDist = 0;
	Point3f VecM0ToM1;

	Point3f DirUnitVec;
	DirUnitVec.x = BaseLine.kx;
	DirUnitVec.y = BaseLine.ky;
	DirUnitVec.z = BaseLine.kz;

	//设定基线外一点
	VecM0ToM1 = Pt - BaseLine.PassPt;
	
	//计算投影向量
	Point3f ProjVecM0ToM1 = VecM0ToM1.ddot(DirUnitVec)*DirUnitVec;

	//计算垂直向量
	Point3f PerpendicularVecOfM0ToM1 = VecM0ToM1 - ProjVecM0ToM1;

	//计算垂直距离
	SingleDist = NormPoint3f(PerpendicularVecOfM0ToM1);
	return SingleDist;
}

// 计算点集到基线坐标
// 假设要求的线外一点为M1,基线过M0,方向向量为[kx,ky,kz]
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
		//计算投影向量
		Point3f ProjVecM0ToM1 = VecM0ToM1.ddot(DirUnitVec)*DirUnitVec;
		
		//计算垂直向量
		Point3f PerpendicularVecOfM0ToM1 = VecM0ToM1 - ProjVecM0ToM1;
		
		//计算垂直距离
		SingleDist = NormPoint3f(PerpendicularVecOfM0ToM1);
		DistIn3D.push_back(SingleDist);
	}
}

//打印平面参数
void LaserPlane::DisplayCoeffs()
{
	std::cout << "a0= "<<PCoeffs.a0 << std::endl;
	std::cout << "a1= " << PCoeffs.a1 << std::endl;
	std::cout << "a2= " << PCoeffs.a2 << std::endl;
}

//打印基线信息
void LaserPlane::DisplayBaseLine()
{
	std::cout << "法向量为" << std::endl;
	std::cout << "kx=" << BaseLine.kx << std::endl;
	std::cout << "ky=" << BaseLine.ky << std::endl;
	std::cout << "kz=" << BaseLine.kz << std::endl;
	std::cout << "通过的点坐标" << std::endl;
	std::cout << BaseLine.PassPt<< std::endl;
}

//三维向量的norm
float LaserPlane::NormPoint3f(Point3f & vec)
{
	double Norm = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	return Norm;
}

// 自己设定基准线的参数
void LaserPlane::SetBaseLineParams(float _kx, float _ky, float _kz, Point3f & pt)
{
	BaseLine.kx = _kx;
	BaseLine.ky = _ky;
	BaseLine.kz = _kz;
	BaseLine.PassPt = pt;
}

//保存激光平面参数
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
