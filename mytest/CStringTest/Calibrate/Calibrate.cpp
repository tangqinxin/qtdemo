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
	// 标定参数
	Size chessBoardSize = Size(11, 8);/* 标定板上每行、列的角点数 */
	Size square_size = Size(3, 3);/* 实际测量得到的标定板上每个棋盘格的大小 */
	
	// 输出内外参矩阵
	fstream RMatFile;
	fstream CamIntrinsicMatFile;
	fstream tvecFile;
	fstream disCoeffsFile;
	fstream RTMatFile;
	//清空内容
	RMatFile.open("RMat.txt", ios::out | ios::trunc);//清空外参矩阵
	CamIntrinsicMatFile.open("CamIntrinsicMat.txt", ios::out | ios::trunc);//清空内参矩阵
	tvecFile.open("tvec.txt", ios::out | ios::trunc);//清空平移向量
	disCoeffsFile.open("distCoeffs.txt", ios::out | ios::trunc);//清空畸变向量
	RTMatFile.open("RTMat.txt", ios::out | ios::trunc);//清空外参矩阵
	//关闭文件流
	RMatFile.close();
	CamIntrinsicMatFile.close();
	tvecFile.close();
	disCoeffsFile.close();
	RTMatFile.close();
	//重新打开，开始写
	RMatFile.open("RMat.txt", ios::app);//尾部添加
	CamIntrinsicMatFile.open("CamIntrinsicMat.txt", ios::app);//尾部添加
	tvecFile.open("tvec.txt", ios::app);//尾部添加
	disCoeffsFile.open("distCoeffs.txt", ios::app);//尾部添加
	RTMatFile.open("RTMat.txt", ios::app);//尾部添加

	RMatFile << std::fixed << std::setprecision(12);
	CamIntrinsicMatFile << std::fixed << std::setprecision(12);
	disCoeffsFile << std::fixed << std::setprecision(12);
	tvecFile << std::fixed << std::setprecision(12);
	RTMatFile << std::fixed << std::setprecision(12);



	//总体输出的文件流
	ifstream fin("calibdata.txt"); /* 标定所用图像文件的路径 */
	ofstream fout("caliberation_result.txt");//

	//读取每一幅图像，从中提取出角点，然后对角点进行亚像素精确化
	cout << "开始提取角点………………\n";
	int imageNumbers = 0;  /* 图像数量 */
	Size imageSize;  /* 图像的尺寸 */
	vector<Point2f> pointsPerImage;  /* 缓存每幅图像上检测到的角点 */
	vector<vector<Point2f>> pointsAllImage; /* 保存检测到的所有角点 */
	vector<String> imgName;
	string filename;


	while (getline(fin, filename))
	{
		imageNumbers++;
		imgName.push_back(filename);
		// 用于观察检验输出  
		Mat inputImage = imread(filename, IMREAD_GRAYSCALE);//直接以灰度图方式读取
		if (imageNumbers == 1)  //读入第一张图片时获取图像宽高信息  
		{
			imageSize.width = inputImage.cols;
			imageSize.height = inputImage.rows;
			cout << "imageSize.width = " << imageSize.width << endl;
			cout << "imageSize.height = " << imageSize.height << endl << endl;
		}
		cout << "imageNumbers = " << imageNumbers << endl;

		/* 提取角点 */
		if (0 == findChessboardCorners(inputImage, chessBoardSize, pointsPerImage))  //若检测到角点，返回非零值
		{
			cout << "can not find chessboard corners!\n"; //找不到角点
														  //exit(1);
			continue;
		}
		else
		{
			//Mat view_gray;
			//cvtColor(imageInput, view_gray, CV_RGB2GRAY);
			/* 亚像素精确化 */
			//find4QuadCornerSubpix(inputImage, pointsPerImage, Size(5, 5)); //对粗提取的角点进行精确化
			cornerSubPix(inputImage, pointsPerImage, Size(15, 15), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

			cout << "打印检测到的角点信息" << endl << pointsPerImage << endl;
			cout << "打印image_points_buf的格式" << pointsPerImage.size() << endl;

			pointsAllImage.push_back(pointsPerImage);  //保存亚像素角点
														   /* 在图像上显示角点位置 */

			cout << "打印image_points_seq的格式" << pointsAllImage.size() << endl;
			drawChessboardCorners(inputImage, chessBoardSize, pointsPerImage, false); //用于在图片中标记角点
			namedWindow("Camera Calibration", WINDOW_NORMAL);
			imshow("Camera Calibration", inputImage);//显示图片
			waitKey(5);//暂停0.5S
		}
	}
	int totalImageNumber = pointsAllImage.size();
	cout << "totalImageNumber = " << totalImageNumber << endl << endl;


	for (int i = 0; i<totalImageNumber; i++)
	{
		cout << "第 " << i << " 图片的数据 --> : \t";
		for (int j = 0; j < chessBoardSize.height*chessBoardSize.width; j++)
		{
			
			//ios_base::fmtflags initial;
			//initial = cout.setf(ios_base::fixed);
			//cout.precision(3);
			//输出所有的角点  
			cout << "X -->" << pointsAllImage[i][j].x << "\t";
			cout << "Y -->" << pointsAllImage[i][j].y << endl;
			//cout.setf(initial);
		}
	}
	cout << "第一个角点提取完成！\n\n";

	//以下是摄像机标定  
	cout << "开始标定………………";
	/*棋盘三维信息*/
	vector<vector<Point3f>> idealPoints; /* 保存标定板上角点的三维坐标 */
	/*内外参数*/
	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 摄像机内参数矩阵 */
	vector<int> point_counts;  // 每幅图像中角点的数量  
	Mat distCoeffs = Mat(1, 8, CV_32FC1, Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
	vector<Mat> rotateVectors;  /* 每幅图像的旋转向量 */
	vector<Mat> translationVectors; /* 每幅图像的平移向量 */
	/* 初始化标定板上角点的三维坐标 */
	for (int n = 0; n<totalImageNumber; n++)
	{
		vector<Point3f> idealPointSet;
		for (int i = 0; i<chessBoardSize.height; i++)
		{
			for (int j = 0; j<chessBoardSize.width; j++)
			{
				Point3f idealPoint;
				/* 假设标定板放在世界坐标系中z=0的平面上 */
				idealPoint.x = i*square_size.width;
				idealPoint.y = j*square_size.height;
				idealPoint.z = 0;
				idealPointSet.push_back(idealPoint);
			}
		}
		idealPoints.push_back(idealPointSet);
	}
	/* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */
	for (int i = 0; i<totalImageNumber; i++)
	{
		point_counts.push_back(chessBoardSize.width*chessBoardSize.height);
	}
	/* 开始标定 */
	calibrateCamera(idealPoints, pointsAllImage, imageSize, cameraMatrix, distCoeffs, rotateVectors, translationVectors, CV_CALIB_RATIONAL_MODEL);
	cout << "标定完成！\n";
	
	//cout << cameraMatrix << endl;
	//int a = cameraMatrix.type();
	//double b = cameraMatrix.at<double>(0, 0);
	//unsigned c = cameraMatrix.at<unsigned>(0, 0);
	//cout << b << endl;
	//cout << (double)c << endl;
	//auto pval = cameraMatrix.ptr<float>(0, 0);
	//cout << (float)*pval << endl;

	WriteMatToFile(CamIntrinsicMatFile, cameraMatrix);//将内参M输出到txt文件中
	WriteMatToFile(disCoeffsFile, distCoeffs);//将畸变系数k输出到输出到txt文件中

	//对标定结果进行评价  
	cout << "开始评价标定结果………………\n";
	double singleImageError = 0.0;            /* 每幅图像的总误差 */
	double singleImageAverageError = 0.0;     /* 每幅图像的平均误差 */
	double totalImageError = 0.0;             /* 所有图像的误差的总和 */
	double totalImageAverageError = 0.0;      /* 所有图像的平均误差的总和 */
	vector<Point2f> image_points2;            /* 保存重新计算得到的投影点 */
	cout << "\t每幅图像的标定误差：\n";
	fout << "每幅图像的标定误差：\n";
	for (int i = 0; i<totalImageNumber; i++)
	{
		vector<Point3f> idealPointSet = idealPoints[i];
		/* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
		projectPoints(idealPointSet, rotateVectors[i], translationVectors[i], cameraMatrix, distCoeffs, image_points2);
		/* 计算新的投影点和旧的投影点之间的误差*/
		vector<Point2f> tempImagePoint = pointsAllImage[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		singleImageError = norm(image_points2Mat, tempImagePointMat, NORM_L2);      //求解数组1和数组2对应元素的差值，然后求解各个差值平方的累加和
		std::cout << "第" << i + 1 << "幅图像的误差：" << singleImageError << "像素\t";
		fout << "第" << i + 1 << "幅图像的误差：" << singleImageError << "像素\t";
		totalImageError += singleImageError;

		singleImageAverageError = singleImageError / point_counts[i];
		std::cout << "第" << i + 1 << "幅图像的单角点平均误差：" << singleImageAverageError << "像素" << endl;
		fout << "第" << i + 1 << "幅图像的单角点平均误差：" << singleImageAverageError << "像素" << endl;
		totalImageAverageError += singleImageAverageError;
	}
	std::cout << "总平均误差：" << totalImageError / totalImageNumber << "像素\t\t";
	fout << "总平均误差：" << totalImageError / totalImageNumber << "像素\t\t";

	std::cout << "总体单角点平均误差：" << totalImageAverageError / totalImageNumber << "像素" << endl;
	fout << "总体单角点平均误差：" << totalImageAverageError / totalImageNumber << "像素" << endl << endl;
	std::cout << "评价完成！" << endl;
	//保存定标结果      
	std::cout << "\n开始保存定标结果………………" << endl;
	Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */
	cout << "相机内参数矩阵：" << endl;
	cout << cameraMatrix << endl << endl;
	fout << "相机内参数矩阵：" << endl;
	fout << cameraMatrix << endl << endl;
	cout << "畸变系数：\n";
	cout << distCoeffs << endl << endl << endl;
	fout << "畸变系数：\n";
	fout << distCoeffs << endl << endl << endl;

	//WriteMatToFile(CamIntrinsicMatFile, cameraMatrix);//将内参M输出到txt文件中
	//WriteMatToFile(disCoeffsFile, distCoeffs);//将畸变系数k输出到输出到txt文件中

	for (int i = 0; i < totalImageNumber; i++)
	{
		fout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
		fout << rotateVectors[i] << endl;
		/* 将旋转向量转换为相对应的旋转矩阵 */
		Rodrigues(rotateVectors[i], rotation_matrix);
		fout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
		fout << rotation_matrix << endl;
		fout << "第" << i + 1 << "幅图像的平移向量：" << endl;
		fout << translationVectors[i] << endl << endl;

		Mat extrinsicsMat(4, 4, CV_64FC1, Scalar(0));
		rotation_matrix.copyTo(extrinsicsMat(Rect(0, 0, 3, 3)));
		translationVectors[i].copyTo(extrinsicsMat(Rect(3, 0, 1, 3)));
		extrinsicsMat.at<double>(3, 3) = 1;
		cout << "第" << i + 1 << "幅图像的外参矩阵：" << endl;
		cout << extrinsicsMat << endl << endl;

		WriteMatToFile(RMatFile, rotation_matrix);//输出当前照片的外参矩阵
		RMatFile << endl;
		
		WriteMatToFile(tvecFile, translationVectors[i]);//输出当前照片的平移向量
		tvecFile << endl;

		WriteMatToFile(RTMatFile, extrinsicsMat);//输出当前照片的平移向量
		RTMatFile << endl;
	}
	std::cout << "完成保存" << endl;
	fout << endl;

	//下面进行测试函数



	return 0;
}

