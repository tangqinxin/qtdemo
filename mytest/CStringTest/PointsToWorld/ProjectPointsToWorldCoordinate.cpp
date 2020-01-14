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
	Mat R = Mat::zeros(3, 3, CV_32FC1);//��ξ���
	Mat M = Mat::zeros(3, 3, CV_32FC1);//�ڲξ���
	Mat t = Mat::zeros(3, 1, CV_32FC1);//ƽ������
	Mat distCoeffs = Mat::zeros(1, 8, CV_32FC1);//����ϵ��
	Mat ImgPtSetMat = Mat::zeros(88, 2, CV_32FC1);//ͼ���������

	int Index = 1;
	ReadMatFromFileName("RMat.txt", R, Index);
	ReadMatFromFileName("CamIntrinsicMat.txt", M, Index);
	ReadMatFromFileName("tvec.txt", t, Index);
	ReadMatFromFileName("distCoeffs.txt", distCoeffs, Index);
	//ReadMatFromFileName("ImgPtSet.txt", ImgPtSetMat, Index);//����ʹ��matlab����ʱʹ�ô˶δ���

	Mat inputImage = imread("D:\\qtdemo\\mytest\\CStringTest\\Calibrate\\image\\1_l.bmp",IMREAD_GRAYSCALE);//��ȡ��Ƭ
	Mat newimage = inputImage.clone();

	vector<Point2f> ImgPtSet;//���ڱ���һ��ͼƬ�ϵ����̽ǵ��x,y����
	vector<float> TotalError;
	vector<Point3f> WorldCoordinate;
	Size chessBoardSize = Size(11, 8);/* �궨����ÿ�С��еĽǵ��� */
	Size square_size = Size(3, 3);/* ʵ�ʲ����õ��ı궨����ÿ�����̸�Ĵ�С */
	Size image_size;// ͼƬ�ߴ�
	image_size.width = inputImage.cols;
	image_size.height = inputImage.rows;
	
	Mat RR = Mat::eye(3, 3, CV_32FC1);
	Mat mapx = Mat(image_size, CV_32FC1);
	Mat mapy = Mat(image_size, CV_32FC1);

	//8�����궨����,��֤��ó����۾��ǲ�������ȥ������˳��
	//Mat k1Tok4distCoeffs = Mat::zeros(1, 4, CV_32FC1);
	//k1Tok4distCoeffs.at<float>(0, 0) = distCoeffs.at<float>(0, 0);
	//k1Tok4distCoeffs.at<float>(0, 1) = distCoeffs.at<float>(0, 1);
	//k1Tok4distCoeffs.at<float>(0, 2) = distCoeffs.at<float>(0, 2);
	//k1Tok4distCoeffs.at<float>(0, 3) = distCoeffs.at<float>(0, 3);


	 //����ͼƬ
	//initUndistortRectifyMap(M, distCoeffs, RR, M, image_size, CV_32FC1, mapx, mapy);
	//remap(inputImage, newimage, mapx, mapy, INTER_LINEAR);

	//// �޽���Ƭ�δ���
	findChessboardCorners(inputImage, chessBoardSize, ImgPtSet);//�õ���ȡͼ��ǵ�
	cornerSubPix(inputImage, ImgPtSet, Size(15, 15), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
	// �н���Ƭ��C++����
	//findChessboardCorners(newimage, chessBoardSize, ImgPtSet);//�õ���ȡͼ��ǵ�
	//cornerSubPix(newimage, ImgPtSet, Size(15, 15), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

	// //����ʹ��matlab����ʱʹ�ô˶δ���
	//for (int i = 0; i < 88; i++) {
	//	Point2f Coner;
	//	Coner.x = ImgPtSetMat.at<float>(i, 0);
	//	Coner.y = ImgPtSetMat.at<float>(i, 1);
	//	ImgPtSet.push_back(Coner);
	//}

	cout << ImgPtSet<< endl;

	//M = M.t();//����ʹ��matlab����ʱʹ�øöδ���
	//cout << M << endl;
	vector<Point2f> newImgPtSet;
	undistortPoints(ImgPtSet, newImgPtSet, M, distCoeffs,cv::noArray(),M);
	cout << "�µ����ص�" << endl;
	cout << newImgPtSet<< endl;
	PointsToWorld(M, R, t, newImgPtSet, WorldCoordinate,TotalError);//�õ���������ϵ�ĵ�

	ofstream cvfile("C++CodeResult.txt");
	cout << "��άλ������" << endl;
	cvfile << "��άλ������" << endl;
	for (int i = 0; i < WorldCoordinate.size(); i++) {
		cout << WorldCoordinate[i] << endl;
		cvfile << WorldCoordinate[i] << endl;
	}
	cout << "���" << endl;
	cvfile << "���" << endl;
	for (int i = 0; i < TotalError.size(); i++) {
		cout << TotalError[i] << endl;
		cvfile << TotalError[i] << endl;
	}
	vector<Point3f> PointsCameraCoordinates;
	PointsToCamera(WorldCoordinate, R, t, PointsCameraCoordinates);
	cout << "�������ϵ�µ���ά����λ��" << endl;
	cvfile << "�������ϵ�µ���ά����λ��" << endl;
	for (int i = 0; i < PointsCameraCoordinates.size(); i++) {
		cout << PointsCameraCoordinates[i] << endl;
		cvfile << PointsCameraCoordinates[i] << endl;
	}

	cout << "��������" << endl;
	cvfile << "��������" << endl;
	for (int i = 1; i < PointsCameraCoordinates.size(); i++) {
		cout<<TwoPointsDist(PointsCameraCoordinates[i], PointsCameraCoordinates[i - 1])<<endl;
		cvfile << TwoPointsDist(PointsCameraCoordinates[i], PointsCameraCoordinates[i - 1]) << endl;
	}

	return 0;
}