#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cstdio>

using namespace cv;
using namespace std;

void drawHistogram(Mat & image)
{
    const int channels[1] = { 0 };
	const int bins[1] = { 256 };
	float hranges[2] = { 0,255 };
	const float* ranges[1] = { hranges };
	int dims = image.channels();
	if (dims == 3) {
		vector<Mat> bgr_plane;
		split(image, bgr_plane);
		Mat b_hist;
		Mat g_hist;
		Mat r_hist;
		// 计算Blue, Green, Red通道的直方图
		calcHist(&bgr_plane[0], 1, 0, Mat(), b_hist, 1, bins, ranges);
		calcHist(&bgr_plane[1], 1, 0, Mat(), g_hist, 1, bins, ranges);
		calcHist(&bgr_plane[2], 1, 0, Mat(), r_hist, 1, bins, ranges);
		// 显示直方图
		int hist_w = 512;
		int hist_h = 400;
		int bin_w = cvRound((double)hist_w / bins[0]);
		Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);
		// 归一化直方图数据
		normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		// 绘制直方图曲线
		for (int i = 1; i < bins[0]; i++) {
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);

		}
		// 显示直方图
		namedWindow("Histogram dims3", WINDOW_AUTOSIZE);
		imshow("Histogram dims3", histImage);
    }else {
		
		Mat hist;
		// 计算Blue, Green, Red通道的直方图
		calcHist(&image, 1, 0, Mat(), hist, 1, bins, ranges);
		// 显示直方图
		int hist_w = 512;
		int hist_h = 400;
		int bin_w = cvRound((double)hist_w / bins[0]);
		Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);
		// 归一化直方图数据
		normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		// 绘制直方图曲线
		for (int i = 1; i < bins[0]; i++) {
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
		}
		// 显示直方图
		namedWindow("Histogram other", WINDOW_AUTOSIZE);
		imshow("Histogram other", histImage);
    }
}

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
    if( !img.data ){
        perror("imread");
        return -1;
    }

    Mat dst;
    //指定颜色调整对比度的可能？
    //指定区域调整对比度的可能？
    //目前以调整亮度的上限和下限改变对比度
    // normalize(img, dst, 255, 127, NORM_MINMAX, CV_8U);

    // imshow("src", img);
    // imshow("dst", dst);

    // Mat histogram = calGrayHist(dst);
    // imshow("Gray", histogram);

    drawHistogram(img);

    waitKey(0);
    
    return 0;
}
