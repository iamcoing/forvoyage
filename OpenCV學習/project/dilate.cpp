#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cstdio>

#define IMGNAME "outimg"

using namespace cv;
using namespace std;

int r = 1; //结构元半径
int MAX_R = 20; //设置的最大半径

int i = 1; //迭代次数
int MAX_I = 50; //设置的最大迭代次数

Mat img;
Mat out;

void callBack(int num, void * args)
{
    //创建XY方向的矩形结构元
    Mat s = getStructuringElement(MORPH_RECT, Size(2 * r + 1, 2 * r + 1));
    
    //膨胀操作
    morphologyEx(img, out, MORPH_GRADIENT, s, Point(-1, -1), i);
    //显示处理结果
    imshow(IMGNAME, out);
}

int main(int argc, const char * argv[])
{
    img = imread(argv[1], IMREAD_GRAYSCALE);
    if( img.empty() ){
        perror("imread");
        exit(-1);
    }
    //创建显示膨胀效果的窗口
    namedWindow(IMGNAME, WINDOW_FULLSCREEN);
    //创建调节r的进度条
    createTrackbar("radius", IMGNAME, &r, MAX_R, callBack);
    createTrackbar("iteration", IMGNAME, &i, MAX_I, callBack);
    //预加载一次图片显示
    callBack(0, 0);

    waitKey(0);

    return 0;
}