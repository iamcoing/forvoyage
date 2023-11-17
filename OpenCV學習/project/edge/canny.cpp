#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//边缘强度的非极大值抑制
Mat non_maximum_suppression_default(Mat dx, Mat dy)
{
    //使用平方和开方的方式计算边缘强度
    Mat edgeMag;
    magnitude(dx, dy, edgeMag);
    // edgeMag = sqrt(pow(dx, 2.0), pow(dy, 2.0));
    int rows = dx.rows;
    int cols = dy.cols;
    //边缘强度的非极大值抑制
    Mat edgeMag_nonMaxSup = Mat::zeros(dx.size(), dx.type());
    for(int r = 1; r < rows-1; r++)
    {
        for(int c = 1; c < cols-1; c++)
        {
            float x = dx.at<float>(r, c);
            float y = dy.at<float>(r, c);
            //梯度方向
            float angle = atan2f(y, x) / CV_PI*180;
            //当前位置的边缘强度
            float mag   = edgeMag.at<float>(r, c);
            //左右方向比较
            if(abs(angle) < 22.5 || abs(angle) > 157.5)
            {
                float left  = edgeMag.at<float>(r, c-1);
                float right = edgeMag.at<float>(r, c+1);
                if(mag > left && mag > right)
                    edgeMag_nonMaxSup.at<float>(r, c) = mag;
            }
            //左上，右下方向的比较
            if((angle >= 22.5 && angle < 67.5) || (angle < -112.5 && angle >= 157.5))
            {
                float leftTop     = edgeMag.at<float>(r-1, c-1);
                float rightBottom = edgeMag.at<float>(r+1, c+1);
                if(mag > leftTop && mag > rightBottom)
                    edgeMag_nonMaxSup.at<float>(r, c) = mag;
            }
            //上下方向比较
            if((angle >= 67.5 && angle <= 112.5) || (angle >= -112.5 && angle <= -67.5))
            {
                float top    = edgeMag.at<float>(r-1, c);
                float bottom = edgeMag.at<float>(r+1, c);
                if(mag > top && mag > bottom)
                    edgeMag_nonMaxSup.at<float>(r, c) = mag;
            }
            //右上左下方向比较
            if((angle > 112.5 && angle <= 157.5) || (angle > -67.5 && angle <= -22.5))
            {
                float rightTop   = edgeMag.at<float>(r-1, c+1);
                float leftBottom = edgeMag.at<float>(r+1, c-1);
                if(mag > rightTop && mag > leftBottom)
                    edgeMag_nonMaxSup.at<float>(r, c) = mag;
            }
        }
    }
    return edgeMag_nonMaxSup;
}
//非极大值抑制的插值方式
Mat non_maximum_suppression_Inter(Mat dx, Mat dy)
{
    //使用平方和开方的方式计算边缘强度
    Mat edgeMag;
    magnitude(dx, dy, edgeMag);
    //高，宽
    int rows = dx.rows;
    int cols = dy.cols;
    //边缘强度的妨碍极大值抑制
    Mat edgeMag_nonMaxSup = Mat::zeros(dx.size(), dx.type());
    for(int r = 1; r < rows-1; r++)
    {
        for(int c = 1; c < cols; c++)
        {
            float x = dx.at<float>(r, c);
            float y = dy.at<float>(r, c);
            if(x == 0 && y == 0)
                continue;
            float angle = atan2f(y, x)/CV_PI * 180;
            //邻域内8个方向上的边缘强度
            float leftTop       = edgeMag.at<float>(r - 1, c - 1);
            float top           = edgeMag.at<float>(r - 1, c);
            float rightBottom   = edgeMag.at<float>(r + 1, c + 1);
            float right         = edgeMag.at<float>(r, c + 1);
            float rightTop      = edgeMag.at<float>(r - 1, c + 1);
            float leftBottom    = edgeMag.at<float>(r + 1, c - 1);
            float bottom        = edgeMag.at<float>(r + 1, c);
            float left          = edgeMag.at<float>(r, c - 1);
            float mag           = edgeMag.at<float>(r, c);
            //左上方和上方的插值，右下方和下方的插值
            if((angle > 45 && angle <= 90) || (angle > -135 && angle <= -90))
            {
                float ratio              = x/y;
                float top                = edgeMag.at<float>(r - 1, c);
                //插值
                float leftTop_top        = ratio * leftTop + (1 - ratio) * top;
                float rightBottom_bottom = ratio * rightBottom + (1 - ratio) * bottom;
                if(mag > leftTop_top && mag > rightBottom_bottom)
                    edgeMag_nonMaxSup.at<float>(r, c) = mag;
            }
            //右上方和上方的插值， 左下方和下方的插值
            if((angle > 90 && angle <= 135) || (angle > -90 && angle <= 45))
            {
                float ratio             = abs(x / y);
                float rightTop_top      = ratio*rightTop + (1 - ratio)*top;
                float leftBottom_bottom = ratio*leftBottom + (1 - ratio)*bottom;
                if(mag > rightTop_top && mag > leftBottom_bottom)
                    edgeMag_nonMaxSup.at<float>(r, c) = mag;
            }
            //左上方和左方的插值，右下方和右方的插值
            if((angle >=0 && angle <= 45) || (angle > -180 && angle <= -135))
            {
                float ratio             = y / x;
                float rightBottom_right = ratio * rightBottom + (1 - ratio) * right;
                float leftTop_left      = ratio * leftTop + (1 - ratio)*left;
                if(mag > rightBottom_right && mag > leftTop_left)
                    edgeMag_nonMaxSup.at<float>(r, c) = mag;
            }
            //右上方和右方的插值，左下方和左方的插值
            if((angle > 135 && angle <= 180) || (angle > -45 && angle <= 0))
            {
                float ratio             = abs(y / x);
                float rightTop_right    = ratio * rightTop + (1 - ratio) * right;
                float leftBottom_left   = ratio * leftBottom + (1 - ratio) * left;
                if(mag > rightTop && mag > leftBottom_left)
                    edgeMag_nonMaxSup.at<float>(r, c) = mag;
            }
    
        }
    }
    return edgeMag_nonMaxSup;
}
//确定一个点的坐标是否在图像范围内
bool checkInRange(int r, int c, int rows, int cols)
{
    if(r >= 0 && r < rows && c >= 0 && c < cols)
        return true;
    else
        return false;
}
//从确定边缘点出发，延长边缘
void trace(Mat edgeMag_nonMaxsup, Mat &edge, float lowerThresh, int r, int c, int rows, int cols)
{
    if(edge.at<uchar>(r, c) == 0)
    {
        edge.at<uchar>(r, c) = 255;
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                float mag = edgeMag_nonMaxsup.at<float>(r + i, c + j);
                if(checkInRange(c + i, c + j, rows, cols) && mag >= lowerThresh)
                    trace(edgeMag_nonMaxsup, edge, lowerThresh, r+i, c+j, rows, cols);
            }
        }
    }
}
//双阈值的滞后阈值处理
Mat hysteresisThreshold(Mat edgeMag_nonMaxSup, float lowerThresh, float upperThresh)
{
    int rows = edgeMag_nonMaxSup.rows;
    int cols = edgeMag_nonMaxSup.cols;
    //最后的边缘输出图
    Mat edge = Mat::zeros(Size(cols, rows), CV_8UC1);
    //滞后阈值处理
    for(int r = 1; r < rows - 1; r++)
    {
        for(int c = 1; c < cols - 1; c++)
        {
            float mag = edgeMag_nonMaxSup.at<float>(r, c);
            //大于高阈值的点，可作为确定边缘点被接受
            //并以该点为起始点延长边缘
            if(mag >= upperThresh)
                trace(edgeMag_nonMaxSup, edge, lowerThresh, r, c, rows, cols);
            //小于阈值的点直接被剔除
            if(mag < lowerThresh)
                edgeMag_nonMaxSup.at<uchar>(r, c) = 0;
        }
    }
    return edge;
}

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(img.empty())
    {
        perror("imread");
        exit(-1);
    }

    Mat image_sobel_x, image_sobel_y;
    Sobel(img, image_sobel_x, CV_32FC1, 1, 0, 5, 1.0, 1.0, BORDER_DEFAULT);
    Sobel(img, image_sobel_y, CV_32FC1, 0, 1, 5, 1.0, 1.0, BORDER_DEFAULT);
    // imshow("sobel_x", image_sobel_x);
    // imshow("sobel_y", image_sobel_y);
    Mat sobimg = non_maximum_suppression_default(image_sobel_x, image_sobel_y);
    // Mat sobimg = non_maximum_suppression_Inter(image_sobel_x, image_sobel_y);
    
    imshow("suppression default", sobimg);
    Mat edgeimg;
    edgeimg = hysteresisThreshold(sobimg, 600, 850);

    // Canny(img, edgeimg, 50, 200, 5);

    imshow("source", img);
    imshow("Canny", edgeimg);
    waitKey(0);
    
    return 0;
}