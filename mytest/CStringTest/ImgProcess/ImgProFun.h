#ifndef _IMGPROFUN
#define _IMGPROFUN
#include<opencv\cv.hpp>
#include<opencv\highgui.h>
#include<opencv2\imgproc.hpp>
#include<vector>
using namespace cv;

//  ********ͼ��Ԥ����*********//
// InputImg-����ͼƬ-I
// m_AreaThreshold-�����ֵ-I
// m_PixelThreshold-I �Ҷ���ֵ
// �����˳��ҶȲ���ĺ��������Ĳ���
cv::Mat FillterSmallArea(cv::Mat& InputImg, double m_AreaThreshold, double m_percent);

// **�жϵ�ǰContourSet�ڵĵ��Ƿ�ﵽ�����ֵ**//
// ������д��
// ȥ�����δ�ﵽ��ֵ�Ĳ���
class AreaThresholdJudge {
private:
	double AreaThreshold;
public:
	AreaThresholdJudge(double x) :AreaThreshold(x) {}
	bool operator()(const std::vector<cv::Point>& ContourSet) { return cv::contourArea(ContourSet) < AreaThreshold; }//С����ֵ����true
};

//��������ȡ����
vector<Point2f> StegerLine(Mat& InputImg);

#endif // !_IMGPROFUN
