#include"ImgProFun.h"

// �����������̣�
// 1 ��ԭͼInputImg��˹�˲�
// 2 �Ҷ���ֵ�˲�����ͼƬת��Ϊ��ֵͼ
// 3 ����2�еĶ�ֵͼ��������˲���ǰ3���ı����ǵõ�һ��mask
// 4 ����3�е�����˲�������õ����յ��˲���ͼƬ,ÿ�����غ�InputImg�����Ĳ�������ͬ��
cv::Mat FillterSmallArea(cv::Mat& InputImg, double m_AreaThreshold, double m_percent) {
	
	cv::Mat tempImg;
	InputImg.copyTo(tempImg);//�½�һ������
	if (tempImg.type() != CV_8UC1) {
		cv::cvtColor(tempImg, tempImg, cv::COLOR_BGR2GRAY);//���������RGBͼ����RGBת�Ҷ�ͼ
	}

	// 1 ��˹�˲�
	cv::GaussianBlur(tempImg, tempImg, cv::Size(5, 5), 1.6, 1.6);//��ԭͼ���и�˹�˲��õ�I
	
	// 2 ��ͼƬת��Ϊ��ֵͼ
	double maxval;
	minMaxLoc(tempImg, NULL, &maxval, NULL, NULL);	// �ҵ����Ҷ�ֵ
	double percent = m_percent;//�ָ����
	cv::threshold(tempImg, tempImg, maxval*percent, 255, cv::THRESH_BINARY);//�Ƚ��ж�ֵ�������ڽ��к���������ָ�
	
	// 3 ��tempImg��������˲�
	std::vector<std::vector<cv::Point>> ContourPtSet;//�߽�������
	cv::findContours(tempImg, ContourPtSet, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//����ȡ������
	
	AreaThresholdJudge AreaJudge(m_AreaThreshold);//����������
	ContourPtSet.erase(std::remove_if(ContourPtSet.begin(), ContourPtSet.end(), AreaJudge), ContourPtSet.end());//����˲�ȥ��

	cv::Mat mask = cv::Mat::zeros(tempImg.size(), tempImg.type());//������Ĥ
	cv::drawContours(mask, ContourPtSet, -1, cv::Scalar(255), CV_FILLED);//�����������ͽ����˴������޸���tempImg����
	
	// 4 �����˲��������ԭͼ���ؽ����޸�
	InputImg.copyTo(tempImg, mask);

	// 5 Ч����֤��չʾ��Ĥ�봦����Ч��
	//cv::namedWindow("mask", CV_WINDOW_NORMAL);
	//cv::imshow("mask", mask);//�˴�Ϊ������Ĥ����
	//cv::namedWindow("ImgAfterProcess",CV_WINDOW_NORMAL);
	//imshow("ImgAfterProcess", tempImg);

	return tempImg;
}


// **********Steger��������ȡ����********* //
vector<Point2f> StegerLine(cv::Mat& InputImg)
{
	Mat img0 = InputImg;//��Ϊԭͼ���б���
	Mat img;//��Ϊ����ͼ
	img0.copyTo(img);//��������

	//��˹�˲�
	img.convertTo(img, CV_32FC1);
	GaussianBlur(img, img, Size(0, 0), 6, 6);

	//һ��ƫ����
	Mat m1, m2;
	m1 = (Mat_<float>(1, 2) << 1, -1);  //xƫ��
	m2 = (Mat_<float>(2, 1) << 1, -1);  //yƫ��

	Mat dx, dy;
	filter2D(img, dx, CV_32FC1, m1);
	filter2D(img, dy, CV_32FC1, m2);

	//����ƫ����
	Mat m3, m4, m5;
	m3 = (Mat_<float>(1, 3) << 1, -2, 1);   //����xƫ��
	m4 = (Mat_<float>(3, 1) << 1, -2, 1);   //����yƫ��
	m5 = (Mat_<float>(2, 2) << 1, -1, -1, 1);   //����xyƫ��

	Mat dxx, dyy, dxy;
	filter2D(img, dxx, CV_32FC1, m3);
	filter2D(img, dyy, CV_32FC1, m4);
	filter2D(img, dxy, CV_32FC1, m5);

	//hessian����
	double maxD = -1;
	int imgcol = img.cols;
	int imgrow = img.rows;
	vector<double> Pt;
	for (int i = 0; i<imgcol; i++)
	{
		for (int j = 0; j<imgrow; j++)
		{
			if (img0.at<uchar>(j, i)>50)
			{
				Mat hessian(2, 2, CV_32FC1);
				hessian.at<float>(0, 0) = dxx.at<float>(j, i);
				hessian.at<float>(0, 1) = dxy.at<float>(j, i);
				hessian.at<float>(1, 0) = dxy.at<float>(j, i);
				hessian.at<float>(1, 1) = dyy.at<float>(j, i);

				Mat eValue;
				Mat eVectors;
				eigen(hessian, eValue, eVectors);

				double nx, ny;
				double fmaxD = 0;
				if (fabs(eValue.at<float>(0, 0)) >= fabs(eValue.at<float>(1, 0)))  //������ֵ���ʱ��Ӧ����������
				{
					nx = eVectors.at<float>(0, 0);
					ny = eVectors.at<float>(0, 1);
					fmaxD = eValue.at<float>(0, 0);
				}
				else
				{
					nx = eVectors.at<float>(1, 0);
					ny = eVectors.at<float>(1, 1);
					fmaxD = eValue.at<float>(1, 0);
				}

				double t = -(nx*dx.at<float>(j, i) + ny*dy.at<float>(j, i)) / (nx*nx*dxx.at<float>(j, i) + 2 * nx*ny*dxy.at<float>(j, i) + ny*ny*dyy.at<float>(j, i));

				if (fabs(t*nx) <= 0.5 && fabs(t*ny) <= 0.5)
				{
					Pt.push_back(i);
					Pt.push_back(j);
				}
			}
		}
	}

	cvtColor(img0, img0, CV_GRAY2BGR);//����Ҫ��֤ʱ��ע�͵�
	vector<Point2f> LaserLinePtSet;//����ĵ㼯
	for (int k = 0; k<Pt.size() / 2; k++)
	{
		Point2f rpt;
		rpt.x = Pt[2 * k + 0];
		rpt.y = Pt[2 * k + 1];
		LaserLinePtSet.push_back(rpt);//������뵽����㼯������
		circle(img0, rpt, 1, Scalar(0, 0, 255));
	}
	//���²���Ϊ��֤����
	namedWindow("result", CV_WINDOW_NORMAL);
	imshow("result", img0);
	imwrite("output.bmp", img0);

	return LaserLinePtSet;
}