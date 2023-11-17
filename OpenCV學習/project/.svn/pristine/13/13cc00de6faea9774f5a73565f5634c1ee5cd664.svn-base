#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

enum ZERO_CROSS_TYPE{
    ZERO_CROSS_DEFULT,
    ZERO_CROSS_MEAN,
};

extern Mat LoG(InputArray image, float sigma, int win);
extern Mat DoG(Mat image, float sigma, int win, float k = 1.1);

//上下左右方式寻找过零点
void zero_cross_defalut(InputArray _src, OutputArray _dst)
{
    Mat src = _src.getMat();
    //判断位深
    CV_Assert( (src.type() == CV_32FC1) );
    _dst.create(src.size(), CV_8UC1);
    Mat dst = _dst.getMat();
    //输入图像矩阵的高、宽
    int rows = src.rows;
    int cols = src.cols;
    //零交叉点
    for(int r = 1; r < rows - 2; r++)
    {
        for(int c = 1; c < cols - 2; c++)
        {
            //上下方向
            if(src.at<float>(r - 1, c) * src.at<float>(r + 1, c) < 0)
            {
                dst.at<uchar>(r, c) = 255;
                continue;
            }
            //左右方向
            if(src.at<float>(r, c - 1) * src.at<float>(r, c + 1) < 0)
            {
                dst.at<uchar>(r, c) = 255;
                continue;
            }
            //左上，右下方向
            if(src.at<float>(r - 1, c - 1) * src.at<float>(r + 1, c + 1) < 0)
            {
                dst.at<uchar>(r, c) = 255;
                continue;
            }
            //左上，右下方向
            if(src.at<float>(r + 1, c - 1) * src.at<float>(r - 1, c + 1) < 0)
            {
                dst.at<uchar>(r, c) = 255;
                continue;
            }
        }
    }
}

//邻域方式寻找过零点
void zero_cross_mean(InputArray _src, OutputArray _dst)
{
    Mat src = _src.getMat();
    //判断位深
    _dst.create(src.size(), CV_8UC1);
    Mat dst = _dst.getMat();
    int rows = src.rows;
    int cols = src.cols;
    double minValue;
    double maxValue;
    //存储四个方向的均值
    Mat temp(1, 4, CV_32FC1);
    //零交叉点
    for(int r = 0 + 1; r < rows - 1; r++){
        for(int c = 0 + 1; c < cols - 1; c++){
            //左上方向
            Mat left_top(src, Rect(c - 1, r - 1, 2, 2));
            temp.at<float>(0, 0) = mean(left_top)[0];
            //右上方向
            Mat right_top(src, Rect(c, r - 1, 2, 2));
            temp.at<float>(0, 1) = mean(right_top)[0];
            //左下方向
            Mat left_bottom(src, Rect(c - 1, r, 2, 2));
            temp.at<float>(0, 2) = mean(left_bottom)[0];
            //右下方向
            Mat right_bottom(src, Rect(c, r, 2, 2));
            temp.at<float>(0, 2) = mean(right_bottom)[0];
            minMaxLoc(temp, &minValue, &maxValue);
            //最大值和最小值异号，则位置为过零点
            if(minValue * maxValue < 0)
                dst.at<uchar>(r, c) = 255;
        }
    }
}

#define LOG (1)
#define DOG (0)

Mat Marr_Hildreth(InputArray image, int win, float sigma, ZERO_CROSS_TYPE type)
{
#if LOG
    //高斯拉普拉斯
    Mat OFG = LoG(image, sigma, win);
#elif DOG
    Mat OFG = DoG(image, sigma, win);
#endif
    Mat zeroCrossImage;
    switch(type){
    case ZERO_CROSS_DEFULT:
        zero_cross_defalut(OFG, zeroCrossImage);
        break;
    case ZERO_CROSS_MEAN:
        zero_cross_mean(OFG, zeroCrossImage);
        break;
    default:
        cout << "unknown zeroCross type" << endl;
        break;
    }
    return zeroCrossImage;
}


int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(img.empty()){
        cout << "image read failed" << endl;
        return -1;
    }
    Mat marr_hild = Marr_Hildreth(img, 25, 4, ZERO_CROSS_DEFULT);

    imshow("Marr_Hildreth", marr_hild);
    waitKey(0);

    return 0;    
}