#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    Mat src = imread(argv[1], CV_LOAD_IMAGE_IGNORE_ORIENTATION);
    if(src.empty()){
        return -1;
    }

    //构建CLAHE的对象
    Ptr<CLAHE> clahe = createCLAHE(2.0, Size(8 ,8));
    Mat CLAHE;
    Mat HE;
    //限制对比度的自适应直方图
    clahe->apply(src, CLAHE);

    //使用HE均衡化
    normalize(src, HE, 255, -255, NORM_MINMAX, CV_8U);

    //显示原图及均衡化的效果
    imshow("src", src);
    imshow("HE", HE);
    imshow("CLAHE", CLAHE);

    waitKey(0);

    return 0;
}