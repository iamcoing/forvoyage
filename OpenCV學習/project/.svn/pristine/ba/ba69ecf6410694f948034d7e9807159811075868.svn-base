#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

#define ADDFUNC 1
#define MULTIFUNC 0

Mat img, norImg, hlsImg, lsImg;
int width, height, L, S;
string winName = "alter";
int MaxValue = 100;

void callBack_LS(int, void *)
{
    //將歸一化的BGR格式轉換爲HLS格式
    cvtColor(norImg, hlsImg, COLOR_BGR2HLS);
#if ADDFUNC
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            Vec3f hls = hlsImg.at<Vec3f>(r, c);
            hls = Vec3f(hls[0], 
            hls[1] + S/double(MaxValue) > 1 ? 1 : (hls[1] + S / double(MaxValue)),
            hls[2] + L/double(MaxValue) > 1 ? 1 : (hls[2] + L / double(MaxValue))),            
            hlsImg.at<Vec3f>(r, c) = hls;
        }
    }
#elif MULTIFUNC
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            Vec3f hls = hlsImg.at<Vec3f>(r, c);
            hls = Vec3f(hls[0], 
            hls[1] * (1 + S/double(MaxValue)) > 1 ? 1 : (hls[1]*( 1 + S / double(MaxValue))),
            hls[2] * (1 + L/double(MaxValue)) > 1 ? 1 : (hls[2]*( 1 + L / double(MaxValue)))),
            hlsImg.at<Vec3f>(r, c) = hls;
        }
    }
#endif
    cvtColor(hlsImg, lsImg, COLOR_HLS2BGR);
    imshow(winName, lsImg);
}

int main(int argc, const char * argv[])
{
    img = imread(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
    if(img.empty()){
        cout << "image read failed" << endl;
        return -1;
    }
    imshow("src", img);
    width = img.cols;
    height = img.rows;

    img.convertTo(norImg, CV_32FC1, 1 / 255.0, 0);
    namedWindow(winName, WINDOW_AUTOSIZE);

    createTrackbar("光亮度", winName, &S, MaxValue, callBack_LS);
    createTrackbar("飽和度", winName, &L, MaxValue, callBack_LS);

    callBack_LS(0, 0);
    waitKey(0);

    return 0;
}