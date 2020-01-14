#include"CalibCam.h"

//��StrFileName����ȥ��ȡҪ�궨�Ĳ������ļ���
void CalibWithButton(std::string strFileName,int m_width,int m_height,float m_square_size) {
	// ·����ʼ��
	string DirFilePath = "CameraCalibrate";
	string RMatFileName = DirFilePath + "\\" + "RMat.txt";
	string CameraIntrinsicFileName = DirFilePath + "\\" + "CamIntrinsicMat.txt";
	string tvecFileName= DirFilePath + "\\" + "tvec.txt";
	string disCoeffsFileName= DirFilePath + "\\" + "distCoeffs.txt";
	string RTMatFileName= DirFilePath + "\\" + "RTMat.txt";
	string CalibrateResult= DirFilePath + "\\" + "caliberation_result.txt";

	// ����һ���ļ��б����ļ�
	MakeMyDir(DirFilePath);
	// �궨����
	Size chessBoardSize = Size(m_width, m_height);/* �궨����ÿ�С��еĽǵ��� */

	// �������ξ���
	fstream RMatFile;
	fstream CamIntrinsicMatFile;
	fstream tvecFile;
	fstream disCoeffsFile;
	fstream RTMatFile;
	//�������
	RMatFile.open(RMatFileName, ios::out | ios::trunc);//�����ξ���
	CamIntrinsicMatFile.open(CameraIntrinsicFileName, ios::out | ios::trunc);//����ڲξ���
	tvecFile.open(tvecFileName, ios::out | ios::trunc);//���ƽ������
	disCoeffsFile.open(disCoeffsFileName, ios::out | ios::trunc);//��ջ�������
	RTMatFile.open(RTMatFileName, ios::out | ios::trunc);//�����ξ���
													     //�ر��ļ���
	RMatFile.close();
	CamIntrinsicMatFile.close();
	tvecFile.close();
	disCoeffsFile.close();
	RTMatFile.close();
	//���´򿪣���ʼд
	RMatFile.open(RMatFileName, ios::app);//β�����
	CamIntrinsicMatFile.open(CameraIntrinsicFileName, ios::app);//β�����
	tvecFile.open(tvecFileName, ios::app);//β�����
	disCoeffsFile.open(disCoeffsFileName, ios::app);//β�����
	RTMatFile.open(RTMatFileName, ios::app);//β�����

	RMatFile << std::fixed << std::setprecision(12);
	CamIntrinsicMatFile << std::fixed << std::setprecision(12);
	disCoeffsFile << std::fixed << std::setprecision(12);
	tvecFile << std::fixed << std::setprecision(12);
	RTMatFile << std::fixed << std::setprecision(12);



	//����������ļ���
	ifstream fin(strFileName); /* �궨����ͼ���ļ���·�� */
	ofstream fout(CalibrateResult);//����궨������

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

			pointsAllImage.push_back(pointsPerImage);  //���������ؽǵ�
		}
	}
	int totalImageNumber = pointsAllImage.size();

	//������������궨  
	//cout << "��ʼ�궨������������";
	/*������ά��Ϣ*/
	vector<vector<Point3f>> idealPoints; /* ����궨���Ͻǵ����ά���� */
										 /*�������*/
	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* ������ڲ������� */
	vector<int> point_counts;  // ÿ��ͼ���нǵ������  
	Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0)); /* �������5������ϵ����k1,k2,p1,p2,k3 */
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
				idealPoint.x = i*m_square_size;
				idealPoint.y = j*m_square_size;
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
	calibrateCamera(idealPoints, pointsAllImage, imageSize, cameraMatrix, distCoeffs, rotateVectors, translationVectors, 0);
	cout << "�궨��ɣ�\n";

	WriteMatToFile(CamIntrinsicMatFile, cameraMatrix);//���ڲ�M�����txt�ļ���
	WriteMatToFile(disCoeffsFile, distCoeffs);//������ϵ��k����������txt�ļ���

											  //�Ա궨�����������  
	cout << "��ʼ���۱궨���������������\n";
	double singleImageError = 0.0;            /* ÿ��ͼ�������� */
	double singleImageAverageError = 0.0;     /* ÿ��ͼ���ƽ����� */
	double totalImageError = 0.0;             /* ����ͼ��������ܺ� */
	double totalImageAverageError = 0.0;      /* ����ͼ���ƽ�������ܺ� */
	vector<Point2f> image_points2;            /* �������¼���õ���ͶӰ�� */

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

		fout << "��" << i + 1 << "��ͼ�����" << singleImageError << "����\t";
		totalImageError += singleImageError;

		singleImageAverageError = singleImageError / point_counts[i];

		fout << "��" << i + 1 << "��ͼ��ĵ��ǵ�ƽ����" << singleImageAverageError << "����" << endl;
		totalImageAverageError += singleImageAverageError;
	}
	
	fout << "��ƽ����" << totalImageError / totalImageNumber << "����\t\t";


	fout << "���嵥�ǵ�ƽ����" << totalImageAverageError / totalImageNumber << "����" << endl << endl;
	std::cout << "������ɣ�" << endl;
	//���涨����      
	std::cout << "\n��ʼ���涨����������������" << endl;
	Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* ����ÿ��ͼ�����ת���� */

	fout << "����ڲ�������" << endl;
	fout << cameraMatrix << endl << endl;

	fout << "����ϵ����\n";
	fout << distCoeffs << endl << endl << endl;

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

		WriteMatToFile(RMatFile, rotation_matrix);//�����ǰ��Ƭ����ξ���
		RMatFile << endl;

		WriteMatToFile(tvecFile, translationVectors[i]);//�����ǰ��Ƭ��ƽ������
		tvecFile << endl;

		WriteMatToFile(RTMatFile, extrinsicsMat);//�����ǰ��Ƭ��ƽ������
		RTMatFile << endl;
	}
	std::cout << "��ɱ���" << endl;
	fout << endl;

}