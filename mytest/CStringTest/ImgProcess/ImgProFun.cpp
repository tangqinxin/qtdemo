#include"ImgProFun.h"

// 函数基本过程：
// 1 对原图InputImg高斯滤波
// 2 灰度阈值滤波，将图片转换为二值图
// 3 根据2中的二值图进行面积滤波，前3步的本质是得到一个mask
// 4 根据3中的面积滤波结果，得到最终的滤波后图片,每个像素和InputImg在亮的部分是相同的
cv::Mat FillterSmallArea(cv::Mat& InputImg, double m_AreaThreshold, double m_percent) {
	
	cv::Mat tempImg;
	InputImg.copyTo(tempImg);//新建一个矩阵
	if (tempImg.type() != CV_8UC1) {
		cv::cvtColor(tempImg, tempImg, cv::COLOR_BGR2GRAY);//如果输入是RGB图，则RGB转灰度图
	}

	// 1 高斯滤波
	cv::GaussianBlur(tempImg, tempImg, cv::Size(5, 5), 1.6, 1.6);//对原图进行高斯滤波得到I
	
	// 2 将图片转换为二值图
	double maxval;
	minMaxLoc(tempImg, NULL, &maxval, NULL, NULL);	// 找到最大灰度值
	double percent = m_percent;//分割比例
	cv::threshold(tempImg, tempImg, maxval*percent, 255, cv::THRESH_BINARY);//先进行二值化，便于进行后续的面积分割
	
	// 3 对tempImg进行面积滤波
	std::vector<std::vector<cv::Point>> ContourPtSet;//边界轮廓集
	cv::findContours(tempImg, ContourPtSet, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//仅提取外轮廓
	
	AreaThresholdJudge AreaJudge(m_AreaThreshold);//建立函数符
	ContourPtSet.erase(std::remove_if(ContourPtSet.begin(), ContourPtSet.end(), AreaJudge), ContourPtSet.end());//面积滤波去除

	cv::Mat mask = cv::Mat::zeros(tempImg.size(), tempImg.type());//创建掩膜
	cv::drawContours(mask, ContourPtSet, -1, cv::Scalar(255), CV_FILLED);//这个函数本身就进行了处理，它修改了tempImg矩阵
	
	// 4 根据滤波的情况对原图像素进行修改
	InputImg.copyTo(tempImg, mask);

	// 5 效果验证，展示掩膜与处理后的效果
	//cv::namedWindow("mask", CV_WINDOW_NORMAL);
	//cv::imshow("mask", mask);//此处为测试掩膜代码
	//cv::namedWindow("ImgAfterProcess",CV_WINDOW_NORMAL);
	//imshow("ImgAfterProcess", tempImg);

	return tempImg;
}


// **********Steger中心线提取函数********* //
vector<Point2f> StegerLine(cv::Mat& InputImg)
{
	Mat img0 = InputImg;//此为原图，有背景
	Mat img;//此为处理图
	img0.copyTo(img);//拷贝矩阵

	//高斯滤波
	img.convertTo(img, CV_32FC1);
	GaussianBlur(img, img, Size(0, 0), 6, 6);

	//一阶偏导数
	Mat m1, m2;
	m1 = (Mat_<float>(1, 2) << 1, -1);  //x偏导
	m2 = (Mat_<float>(2, 1) << 1, -1);  //y偏导

	Mat dx, dy;
	filter2D(img, dx, CV_32FC1, m1);
	filter2D(img, dy, CV_32FC1, m2);

	//二阶偏导数
	Mat m3, m4, m5;
	m3 = (Mat_<float>(1, 3) << 1, -2, 1);   //二阶x偏导
	m4 = (Mat_<float>(3, 1) << 1, -2, 1);   //二阶y偏导
	m5 = (Mat_<float>(2, 2) << 1, -1, -1, 1);   //二阶xy偏导

	Mat dxx, dyy, dxy;
	filter2D(img, dxx, CV_32FC1, m3);
	filter2D(img, dyy, CV_32FC1, m4);
	filter2D(img, dxy, CV_32FC1, m5);

	//hessian矩阵
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
				if (fabs(eValue.at<float>(0, 0)) >= fabs(eValue.at<float>(1, 0)))  //求特征值最大时对应的特征向量
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

	cvtColor(img0, img0, CV_GRAY2BGR);//不需要验证时可注释掉
	vector<Point2f> LaserLinePtSet;//输出的点集
	for (int k = 0; k<Pt.size() / 2; k++)
	{
		Point2f rpt;
		rpt.x = Pt[2 * k + 0];
		rpt.y = Pt[2 * k + 1];
		LaserLinePtSet.push_back(rpt);//将点加入到输出点集集合中
		circle(img0, rpt, 1, Scalar(0, 0, 255));
	}
	//以下部分为验证代码
	namedWindow("result", CV_WINDOW_NORMAL);
	imshow("result", img0);
	imwrite("output.bmp", img0);

	return LaserLinePtSet;
}