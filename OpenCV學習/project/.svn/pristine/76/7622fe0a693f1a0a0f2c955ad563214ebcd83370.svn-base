#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cstdio>

using namespace cv;
using namespace std;

#define WINDNAME "picture1"

Mat img; 
Mat pImg;

Point2f  IPoint, pIPoint;
int i = 0, j = 0;
Point2f src[4];
Point2f dst[4];

void mouse_Img(int event, int x, int y, int flags, void * param)
{
    switch(event)
    {
    case CV_EVENT_LBUTTONDOWN:
        //recode position x, y
        IPoint = Point2f(x, y);
        break;
    case CV_EVENT_LBUTTONUP:
        src[i] = IPoint;
        circle(img, src[i], 6, Scalar(0), 2);
        i++;
        break;
    default:
        break;
    }
}

void mouse_pImg(int event, int x, int y, int flags, void * param)
{
    switch(event)
    {
    case CV_EVENT_LBUTTONDOWN:
        //recode position x, y
        pIPoint = Point2f(x, y);
        break;
    case CV_EVENT_LBUTTONUP:
        dst[j] = pIPoint;
        circle(pImg, dst[j], 6, Scalar(0), 2);
        j++;
        break;
    default:
        break;
    }
}    

int main(int argc, const char * argv[])
{ 
    img = imread("1234.jpg", CV_LOAD_IMAGE_ANYCOLOR);
    if( img.empty() ){
        printf("read image failed\n");
        exit(-1);
    }
    pImg = 255 * Mat::ones(img.size(), CV_8UC1);

    //在原因窗口上，定义鼠标事件
    namedWindow("img", WINDOW_AUTOSIZE);
    setMouseCallback("img", mouse_Img, NULL);

    //在输出窗口上，定义鼠标事件
    namedWindow("pImg", WINDOW_AUTOSIZE);
    setMouseCallback("pImg", mouse_pImg, NULL);

    imshow("img", img);
    imshow("pImg", pImg);
    while( !(i == 4 && j == 4) )
    {
        imshow("img", img);
        imshow("pImg", pImg);
        if(waitKey(50) == 'q')
            break;
    }
    imshow("img", img);
    imshow("pImg", pImg);

    //remove mouse event
    setMouseCallback("img", NULL, NULL);
    setMouseCallback("pImg", NULL, NULL);

    //计算投影交换矩阵
    Mat p = getPerspectiveTransform(src, dst);

    Mat result;
    warpPerspective(img, result, p, pImg.size());

    imshow("mirrior", result);

    waitKey(0);
}