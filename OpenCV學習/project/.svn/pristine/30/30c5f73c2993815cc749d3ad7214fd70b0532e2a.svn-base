#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    Mat I = imread(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
    if( I.empty() ){
        perror("imread");
        exit(-1);
    }
    Mat tmp;
    //灰度值归一化, 将图像矩阵中的数据转换为0-1以内
    I.convertTo(tmp, CV_64F, 2.0 / 255, 0);
    //伽马变换
    Mat O;
    pow(tmp, 0.5, O);

    vector<Mat> planes;
    split(I, planes);

    imshow("B", planes[0]);
    imshow("G", planes[1]);
    imshow("R", planes[2]);

    // cout << I << endl;
    // imshow("output", O);
    // imshow("input", I);
    // imshow("tmp", tmp);

    O.convertTo(O, CV_8U, 255, 0);
    waitKey(0);

    return 0;
}