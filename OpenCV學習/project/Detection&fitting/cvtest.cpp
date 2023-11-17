#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

cv::Mat Create_Background_Image(Size size = Size(1920, 720), int ntype = CV_8UC4, Scalar color = Scalar(255, 255, 255, 0))
{
	cv::Mat Img(size, ntype, color);
	cv::Mat dstImg_Omni(Size(640, 720), CV_8UC4, Scalar(0, 0, 0, 0));
	cv::Mat dstImg_Camv(Size(1280, 720), CV_8UC4, Scalar(0, 0, 0, 0));

	cv::Mat ImgROI_Camv = Img(cv::Rect(0, 0, 1280, 720));
	cv::Mat ImgROI_Omni = Img(cv::Rect(1280, 0, 640, 720));

	dstImg_Omni.copyTo(ImgROI_Omni);
	dstImg_Camv.copyTo(ImgROI_Camv);

	return Img;
}

int main(int argc, const char * argv[])
{
	cv::Mat Img = Create_Background_Image();

	// imshow("bgp", bgp);
	imshow("img", Img);

	waitKey(0);

	return 0;
}
