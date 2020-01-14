#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "MatIO.h"
using namespace std;
using namespace cv;

int main() {
	// �궨����
	Size chessBoardSize = Size(11, 8);/* �궨����ÿ�С��еĽǵ��� */
	Size square_size = Size(3, 3);/* ʵ�ʲ����õ��ı궨����ÿ�����̸�Ĵ�С */
	
	// �������ξ���
	fstream RMatFile;
	fstream CamIntrinsicMatFile;
	fstream tvecFile;
	fstream disCoeffsFile;
	fstream RTMatFile;
	//�������
	RMatFile.open("RMat.txt", ios::out | ios::trunc);//�����ξ���
	CamIntrinsicMatFile.open("CamIntrinsicMat.txt", ios::out | ios::trunc);//����ڲξ���
	tvecFile.open("tvec.txt", ios::out | ios::trunc);//���ƽ������
	disCoeffsFile.open("distCoeffs.txt", ios::out | ios::trunc);//��ջ�������
	RTMatFile.open("RTMat.txt", ios::out | ios::trunc);//�����ξ���
	//�ر��ļ���
	RMatFile.close();
	CamIntrinsicMatFile.close();
	tvecFile.close();
	disCoeffsFile.close();
	RTMatFile.close();
	//���´򿪣���ʼд
	RMatFile.open("RMat.txt", ios::app);//β�����
	CamIntrinsicMatFile.open("CamIntrinsicMat.txt", ios::app);//β�����
	tvecFile.open("tvec.txt", ios::app);//β�����
	disCoeffsFile.open("distCoeffs.txt", ios::app);//β�����
	RTMatFile.open("RTMat.txt", ios::app);//β�����

	RMatFile << std::fixed << std::setprecision(12);
	CamIntrinsicMatFile << std::fixed << std::setprecision(12);
	disCoeffsFile << std::fixed << std::setprecision(12);
	tvecFile << std::fixed << std::setprecision(12);
	RTMatFile << std::fixed << std::setprecision(12);



	//����������ļ���
	ifstream fin("calibdata.txt"); /* �궨����ͼ���ļ���·�� */
	ofstream fout("caliberation_result.txt");//

	//��ȡÿһ��ͼ�񣬴�����ȡ���ǵ㣬Ȼ��Խǵ���������ؾ�ȷ��
	cout << "��ʼ��ȡ�ǵ㡭����������\n";
	int imageNumbers = 0;  /* ͼ������ */
	Size imageSize;  /* ͼ��ĳߴ� */
	vector<Point2f> pointsPerImage;  /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	vector<vector<Point2f>> pointsAllImage; /* �����⵽�����нǵ� */
	vector<String> imgName;
	string filename;


	while (getline(fin, filename))
	{
		imageNumbers++;
		imgName.push_back(filename);
		// ���ڹ۲�������  
		Mat inputImage = imread(filename, IMREAD_GRAYSCALE);//ֱ���ԻҶ�ͼ��ʽ��ȡ
		if (imageNumbers == 1)  //�����һ��ͼƬʱ��ȡͼ������Ϣ  
		{
			imageSize.width = inputImage.cols;
			imageSize.height = inputImage.rows;
			cout << "imageSize.width = " << imageSize.width << endl;
			cout << "imageSize.height = " << imageSize.height << endl << endl;
		}
		cout << "imageNumbers = " << imageNumbers << endl;

		/* ��ȡ�ǵ� */
		if (0 == findChessboardCorners(inputImage, chessBoardSize, pointsPerImage))  //����⵽�ǵ㣬���ط���ֵ
		{
			cout << "can not find chessboard corners!\n"; //�Ҳ����ǵ�
														  //exit(1);
			continue;
		}
		else
		{
			//Mat view_gray;
			//cvtColor(imageInput, view_gray, CV_RGB2GRAY);
			/* �����ؾ�ȷ�� */
			//find4QuadCornerSubpix(inputImage, pointsPerImage, Size(5, 5)); //�Դ���ȡ�Ľǵ���о�ȷ��
			cornerSubPix(inputImage, pointsPerImage, Size(15, 15), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

			cout << "��ӡ��⵽�Ľǵ���Ϣ" << endl << pointsPerImage << endl;
			cout << "��ӡimage_points_buf�ĸ�ʽ" << pointsPerImage.size() << endl;

			pointsAllImage.push_back(pointsPerImage);  //���������ؽǵ�
														   /* ��ͼ������ʾ�ǵ�λ�� */

			cout << "��ӡimage_points_seq�ĸ�ʽ" << pointsAllImage.size() << endl;
			drawChessboardCorners(inputImage, chessBoardSize, pointsPerImage, false); //������ͼƬ�б�ǽǵ�
			namedWindow("Camera Calibration", WINDOW_NORMAL);
			imshow("Camera Calibration", inputImage);//��ʾͼƬ
			waitKey(5);//��ͣ0.5S
		}
	}
	int totalImageNumber = pointsAllImage.size();
	cout << "totalImageNumber = " << totalImageNumber << endl << endl;


	for (int i = 0; i<totalImageNumber; i++)
	{
		cout << "�� " << i << " ͼƬ������ --> : \t";
		for (int j = 0; j < chessBoardSize.height*chessBoardSize.width; j++)
		{
			
			//ios_base::fmtflags initial;
			//initial = cout.setf(ios_base::fixed);
			//cout.precision(3);
			//������еĽǵ�  
			cout << "X -->" << pointsAllImage[i][j].x << "\t";
			cout << "Y -->" << pointsAllImage[i][j].y << endl;
			//cout.setf(initial);
		}
	}
	cout << "��һ���ǵ���ȡ��ɣ�\n\n";

	//������������궨  
	cout << "��ʼ�궨������������";
	/*������ά��Ϣ*/
	vector<vector<Point3f>> idealPoints; /* ����궨���Ͻǵ����ά���� */
	/*�������*/
	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* ������ڲ������� */
	vector<int> point_counts;  // ÿ��ͼ���нǵ������  
	Mat distCoeffs = Mat(1, 8, CV_32FC1, Scalar::all(0)); /* �������5������ϵ����k1,k2,p1,p2,k3 */
	vector<Mat> rotateVectors;  /* ÿ��ͼ�����ת���� */
	vector<Mat> translationVectors; /* ÿ��ͼ���ƽ������ */
	/* ��ʼ���궨���Ͻǵ����ά���� */
	for (int n = 0; n<totalImageNumber; n++)
	{
		vector<Point3f> idealPointSet;
		for (int i = 0; i<chessBoardSize.height; i++)
		{
			for (int j = 0; j<chessBoardSize.width; j++)
			{
				Point3f idealPoint;
				/* ����궨�������������ϵ��z=0��ƽ���� */
				idealPoint.x = i*square_size.width;
				idealPoint.y = j*square_size.height;
				idealPoint.z = 0;
				idealPointSet.push_back(idealPoint);
			}
		}
		idealPoints.push_back(idealPointSet);
	}
	/* ��ʼ��ÿ��ͼ���еĽǵ��������ٶ�ÿ��ͼ���ж����Կ��������ı궨�� */
	for (int i = 0; i<totalImageNumber; i++)
	{
		point_counts.push_back(chessBoardSize.width*chessBoardSize.height);
	}
	/* ��ʼ�궨 */
	calibrateCamera(idealPoints, pointsAllImage, imageSize, cameraMatrix, distCoeffs, rotateVectors, translationVectors, CV_CALIB_RATIONAL_MODEL);
	cout << "�궨��ɣ�\n";
	
	//cout << cameraMatrix << endl;
	//int a = cameraMatrix.type();
	//double b = cameraMatrix.at<double>(0, 0);
	//unsigned c = cameraMatrix.at<unsigned>(0, 0);
	//cout << b << endl;
	//cout << (double)c << endl;
	//auto pval = cameraMatrix.ptr<float>(0, 0);
	//cout << (float)*pval << endl;

	WriteMatToFile(CamIntrinsicMatFile, cameraMatrix);//���ڲ�M�����txt�ļ���
	WriteMatToFile(disCoeffsFile, distCoeffs);//������ϵ��k����������txt�ļ���

	//�Ա궨�����������  
	cout << "��ʼ���۱궨���������������\n";
	double singleImageError = 0.0;            /* ÿ��ͼ�������� */
	double singleImageAverageError = 0.0;     /* ÿ��ͼ���ƽ����� */
	double totalImageError = 0.0;             /* ����ͼ��������ܺ� */
	double totalImageAverageError = 0.0;      /* ����ͼ���ƽ�������ܺ� */
	vector<Point2f> image_points2;            /* �������¼���õ���ͶӰ�� */
	cout << "\tÿ��ͼ��ı궨��\n";
	fout << "ÿ��ͼ��ı궨��\n";
	for (int i = 0; i<totalImageNumber; i++)
	{
		vector<Point3f> idealPointSet = idealPoints[i];
		/* ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ�� */
		projectPoints(idealPointSet, rotateVectors[i], translationVectors[i], cameraMatrix, distCoeffs, image_points2);
		/* �����µ�ͶӰ��;ɵ�ͶӰ��֮������*/
		vector<Point2f> tempImagePoint = pointsAllImage[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		singleImageError = norm(image_points2Mat, tempImagePointMat, NORM_L2);      //�������1������2��ӦԪ�صĲ�ֵ��Ȼ����������ֵƽ�����ۼӺ�
		std::cout << "��" << i + 1 << "��ͼ�����" << singleImageError << "����\t";
		fout << "��" << i + 1 << "��ͼ�����" << singleImageError << "����\t";
		totalImageError += singleImageError;

		singleImageAverageError = singleImageError / point_counts[i];
		std::cout << "��" << i + 1 << "��ͼ��ĵ��ǵ�ƽ����" << singleImageAverageError << "����" << endl;
		fout << "��" << i + 1 << "��ͼ��ĵ��ǵ�ƽ����" << singleImageAverageError << "����" << endl;
		totalImageAverageError += singleImageAverageError;
	}
	std::cout << "��ƽ����" << totalImageError / totalImageNumber << "����\t\t";
	fout << "��ƽ����" << totalImageError / totalImageNumber << "����\t\t";

	std::cout << "���嵥�ǵ�ƽ����" << totalImageAverageError / totalImageNumber << "����" << endl;
	fout << "���嵥�ǵ�ƽ����" << totalImageAverageError / totalImageNumber << "����" << endl << endl;
	std::cout << "������ɣ�" << endl;
	//���涨����      
	std::cout << "\n��ʼ���涨����������������" << endl;
	Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* ����ÿ��ͼ�����ת���� */
	cout << "����ڲ�������" << endl;
	cout << cameraMatrix << endl << endl;
	fout << "����ڲ�������" << endl;
	fout << cameraMatrix << endl << endl;
	cout << "����ϵ����\n";
	cout << distCoeffs << endl << endl << endl;
	fout << "����ϵ����\n";
	fout << distCoeffs << endl << endl << endl;

	//WriteMatToFile(CamIntrinsicMatFile, cameraMatrix);//���ڲ�M�����txt�ļ���
	//WriteMatToFile(disCoeffsFile, distCoeffs);//������ϵ��k����������txt�ļ���

	for (int i = 0; i < totalImageNumber; i++)
	{
		fout << "��" << i + 1 << "��ͼ�����ת������" << endl;
		fout << rotateVectors[i] << endl;
		/* ����ת����ת��Ϊ���Ӧ����ת���� */
		Rodrigues(rotateVectors[i], rotation_matrix);
		fout << "��" << i + 1 << "��ͼ�����ת����" << endl;
		fout << rotation_matrix << endl;
		fout << "��" << i + 1 << "��ͼ���ƽ��������" << endl;
		fout << translationVectors[i] << endl << endl;

		Mat extrinsicsMat(4, 4, CV_64FC1, Scalar(0));
		rotation_matrix.copyTo(extrinsicsMat(Rect(0, 0, 3, 3)));
		translationVectors[i].copyTo(extrinsicsMat(Rect(3, 0, 1, 3)));
		extrinsicsMat.at<double>(3, 3) = 1;
		cout << "��" << i + 1 << "��ͼ�����ξ���" << endl;
		cout << extrinsicsMat << endl << endl;

		WriteMatToFile(RMatFile, rotation_matrix);//�����ǰ��Ƭ����ξ���
		RMatFile << endl;
		
		WriteMatToFile(tvecFile, translationVectors[i]);//�����ǰ��Ƭ��ƽ������
		tvecFile << endl;

		WriteMatToFile(RTMatFile, extrinsicsMat);//�����ǰ��Ƭ��ƽ������
		RTMatFile << endl;
	}
	std::cout << "��ɱ���" << endl;
	fout << endl;

	//������в��Ժ���



	return 0;
}

