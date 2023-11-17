#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>
#include <unistd.h>
#include "transfer.hpp"

using namespace std;
using namespace cv;

pthread_t pid;
Mat Image;

void * image_test(void * arg)
{
	static int flag = 0;	
	vector<Mat> Plante;
	// Mat Plante[4];
	Mat output;
	while(1)
	{
		Image = imread("/home/ubuntu/test/pic/1111.png", CV_LOAD_IMAGE_UNCHANGED);
		if(Image.empty())
		{
			std::cout << "Image read failed" << std::endl;
			return (void *)-1;
		}
		split(Image, Plante);

		for(int r = 0; r < Image.rows; r++)
		{
			for(int c = 0; c < Image.cols; c++)
			{
				if(Plante.at(0).at<u_char>(r, c) > 0 && Plante.at(0).at<u_char>(r, c) < 127)
				{
					Plante.at(0).at<u_char>(r, c) *= 2;
				}
				else if(2 * Plante.at(0).at<u_char>(r, c) >= 255)
				{
					Plante.at(0).at<u_char>(r, c) =  255;
				}
				else
				{
					Plante.at(0).at<u_char>(r, c) = Plante.at(0).at<u_char>(r, c);
				}
			}
		}

		merge(Plante, output);
		if(!flag)
		{
			imwrite("outImage.png", output);
			flag = 1;
		}
	}
}

int main(int argc, const char * argv[])
{
	// FILE * fp = fopen("act_mark_avm.png", "w+");
	// Mat DstImg = Mat(Size(672, 91), CV_8UC4, Scalar(0xff, 0xff, 0xff, 0x0));
    Mat Img_cam = Mat(Size(1280, 800), CV_8UC4, GLubyte);
	// // Mat RoImg_cam = DstImg(cv::Rect(0, 0, Img_cam.cols, Img_cam.rows));
	// // Img_cam.copyTo(RoImg_cam);
	// Mat Img_omn = Mat(Size(444, 750), CV_8UC4, GLubyte_1);
	// Mat Img_111 = Mat(Size(444, 750), CV_8UC4, GLubyte_2);
	// // Mat RoImg_omn = DstImg(cv::Rect(495, 0, Img_omn.cols, Img_omn.rows));
	// // Img_omn.copyTo(RoImg_omn);
	// // cout << DstImg.cols << "  " << DstImg.rows << endl;
	// // cout <<  DstImg.channels() << endl;

	// // fwrite(DstImg.data, sizeof(char), 672 * 91 * 4, fp);
	// // imwrite("act_mark_img.png", DstImg);

	imshow("img_cam", Img_cam);
	// imshow("img_omn", Img_omn);
	// imshow("img_dst", Img_111);
    // imshow("img", DstImg);
	waitKey(0);
	
	// pthread_create(&pid, NULL, image_test, NULL);

	// while(1)
	// {
	// 	usleep(5000 * 1000);
	// }

    return 0;
}