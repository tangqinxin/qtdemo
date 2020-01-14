#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "MatIO.h"
#include "PointsToWorld.h"
#include "PointsToCamera.h"
int main() {
	Mat R = Mat::zeros(3, 3, CV_32FC1);//外参矩阵
	Mat M = Mat::zeros(3, 3, CV_32FC1);//内参矩阵
	Mat t = Mat::zeros(3, 1, CV_32FC1);//平移向量
	Mat distCoeffs = Mat::zeros(1, 8, CV_32FC1);//畸变系数
	Mat ImgPtSetMat = Mat::zeros(88, 2, CV_32FC1);//图像坐标矩阵

	int Index = 1;
	ReadMatFromFileName("RMat.txt", R, Index);
	ReadMatFromFileName("CamIntrinsicMat.txt", M, Index);
	ReadMatFromFileName("tvec.txt", t, Index);
	ReadMatFromFileName("distCoeffs.txt", distCoeffs, Index);
	//ReadMatFromFileName("ImgPtSet.txt", ImgPtSetMat, Index);//仅在使用matlab参数时使用此段代码

	Mat inputImage = imread("D:\\qtdemo\\mytest\\CStringTest\\Calibrate\\image\\1_l.bmp",IMREAD_GRAYSCALE);//读取照片
	Mat newimage = inputImage.clone();

	vector<Point2f> ImgPtSet;//用于保存一张图片上的棋盘角点的x,y坐标
	vector<float> TotalError;
	vector<Point3f> WorldCoordinate;
	Size chessBoardSize = Size(11, 8);/* 标定板上每行、列的角点数 */
	Size square_size = Size(3, 3);/* 实际测量得到的标定板上每个棋盘格的大小 */
	Size image_size;// 图片尺寸
	image_size.width = inputImage.cols;
	image_size.height = inputImage.rows;
	
	Mat RR = Mat::eye(3, 3, CV_32FC1);
	Mat mapx = Mat(image_size, CV_32FC1);
	Mat mapy = Mat(image_size, CV_32FC1);

	//8参数标定矫正,验证后得出结论就是不用特意去调参数顺序
	//Mat k1Tok4distCoeffs = Mat::zeros(1, 4, CV_32FC1);
	//k1Tok4distCoeffs.at<float>(0, 0) = distCoeffs.at<float>(0, 0);
	//k1Tok4distCoeffs.at<float>(0, 1) = distCoeffs.at<float>(0, 1);
	//k1Tok4distCoeffs.at<float>(0, 2) = distCoeffs.at<float>(0, 2);
	//k1Tok4distCoeffs.at<float>(0, 3) = distCoeffs.at<float>(0, 3);


	 //矫正图片
	//initUndistortRectifyMap(M, distCoeffs, RR, M, image_size, CV_32FC1, mapx, mapy);
	//remap(inputImage, newimage, mapx, mapy, INTER_LINEAR);

	//// 无矫正片段代码
	findChessboardCorners(inputImage, chessBoardSize, ImgPtSet);//得到提取图像角点
	cornerSubPix(inputImage, ImgPtSet, Size(15, 15), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
	// 有矫正片段C++代码
	//findChessboardCorners(newimage, chessBoardSize, ImgPtSet);//得到提取图像角点
	//cornerSubPix(newimage, ImgPtSet, Size(15, 15), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

	// //仅在使用matlab参数时使用此段代码
	//for (int i = 0; i < 88; i++) {
	//	Point2f Coner;
	//	Coner.x = ImgPtSetMat.at<float>(i, 0);
	//	Coner.y = ImgPtSetMat.at<float>(i, 1);
	//	ImgPtSet.push_back(Coner);
	//}

	cout << ImgPtSet<< endl;

	//M = M.t();//仅在使用matlab参数时使用该段代码
	//cout << M << endl;
	vector<Point2f> newImgPtSet;
	undistortPoints(ImgPtSet, newImgPtSet, M, distCoeffs,cv::noArray(),M);
	cout << "新的像素点" << endl;
	cout << newImgPtSet<< endl;
	PointsToWorld(M, R, t, newImgPtSet, WorldCoordinate,TotalError);//得到世界坐标系的点

	ofstream cvfile("C++CodeResult.txt");
	cout << "三维位置坐标" << endl;
	cvfile << "三维位置坐标" << endl;
	for (int i = 0; i < WorldCoordinate.size(); i++) {
		cout << WorldCoordinate[i] << endl;
		cvfile << WorldCoordinate[i] << endl;
	}
	cout << "误差" << endl;
	cvfile << "误差" << endl;
	for (int i = 0; i < TotalError.size(); i++) {
		cout << TotalError[i] << endl;
		cvfile << TotalError[i] << endl;
	}
	vector<Point3f> PointsCameraCoordinates;
	PointsToCamera(WorldCoordinate, R, t, PointsCameraCoordinates);
	cout << "相机坐标系下的三维坐标位置" << endl;
	cvfile << "相机坐标系下的三维坐标位置" << endl;
	for (int i = 0; i < PointsCameraCoordinates.size(); i++) {
		cout << PointsCameraCoordinates[i] << endl;
		cvfile << PointsCameraCoordinates[i] << endl;
	}

	cout << "两点间距离" << endl;
	cvfile << "两点间距离" << endl;
	for (int i = 1; i < PointsCameraCoordinates.size(); i++) {
		cout<<TwoPointsDist(PointsCameraCoordinates[i], PointsCameraCoordinates[i - 1])<<endl;
		cvfile << TwoPointsDist(PointsCameraCoordinates[i], PointsCameraCoordinates[i - 1]) << endl;
	}

	return 0;
}