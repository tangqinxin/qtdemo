#include"PointsToWorld.h"
void PointsToWorld(Mat& M, Mat& R, Mat& tVec, vector<Point2f>& ImgPointsSet,vector<Point3f>& WorldCoordinate, vector<float>&TotalError)
{
	int NumOfPts = ImgPointsSet.size();

	Mat T = R.t()*M.inv();

	// B的初始化，用于解方程
	Mat tempB = R.t()*tVec;//tempB=R'*t;
	Mat B = Mat::zeros(2, 1, CV_32FC1);
	B.at<float>(0, 0) = tempB.at<float>(0, 0);
	B.at<float>(1, 0) = tempB.at<float>(1, 0);//仅仅拷贝前2项
	float RHS3 = tempB.at<float>(2, 0);//这是R'*t的第三个元素
	float tz = tVec.at<float>(2, 0);


	// 初始化uvPoints矩阵
	// x1 y1 1
	// x1 y2 1
	Mat uvPointsMat = Mat::ones(NumOfPts, 3, CV_32FC1);//这里相当于对ImgPointsSet添加了一个维度
	for (int i = 0; i < NumOfPts; i++) {
		*(uvPointsMat.ptr<float>(i, 0)) = ImgPointsSet[i].x;
		*(uvPointsMat.ptr<float>(i, 1)) = ImgPointsSet[i].y;//对每一行进行修改
	}

	float R31 = R.at<float>(2, 0);
	float R32 = R.at<float>(2, 1);
	
	Mat abcMat = T*uvPointsMat.t();

	Mat SolveMat = Mat::zeros(2, 2, CV_32FC1);
	float error = 0;
	for (int i = 0; i < NumOfPts; i++) {
		float a = (*(abcMat.ptr<float>(0, i)));
		float b = (*(abcMat.ptr<float>(1, i)));
		float c = (*(abcMat.ptr<float>(2, i)));
		// 修正矩阵参数
		*SolveMat.ptr<float>(0, 0) = a*R31 - 1;
		*SolveMat.ptr<float>(0, 1) = a*R32;
		*SolveMat.ptr<float>(1, 0) = b*R31;
		*SolveMat.ptr<float>(1, 1) = b*R32-1;

		// 构建RHS
		Mat MinusTerm = Mat::zeros(2, 1, CV_32FC1);
		*MinusTerm.ptr<float>(0, 0) = a*tz;
		*MinusTerm.ptr<float>(1, 0) = b*tz;
		Mat RHS = B - MinusTerm;

		//得到解Mat Solution=[Xw;Yw]
		Mat Solution = SolveMat.inv()*RHS;

		//末端项作为误差测试
		 error =c*(R31*Solution.at<float>(0,0)+R32*Solution.at<float>(1,0)+tz)-RHS3 ;

		// 将Point3f点赋值并压入Worldcoordinate中
		Point3f WorldPoint;
		WorldPoint.x = Solution.at<float>(0, 0);//Xw
		WorldPoint.y = Solution.at<float>(1, 0);//Yw
		WorldPoint.z = 0;

		WorldCoordinate.push_back(WorldPoint);
		TotalError.push_back(error);
	}

}
