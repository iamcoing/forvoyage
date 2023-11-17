#include <map>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

map<vector<int>, vector<Point>> HTLine(Mat I, Mat & accumulator, float stepTheta = 1, float stepRho = 1)
{
    //图像的宽，高
    int rows = I.rows;
    int cols = I.cols;
    //可能出现的最大垂线长度
    int L = round(sqrt(pow(rows - 1, 2.0) + pow(cols - 1, 2.0))) + 1;
    //初始化投票器
    int numtheta = int(180.0 / stepTheta);
    int numRho   = int(2 * L / stepRho + 1);
    accumulator  = Mat::zeros(Size(numtheta, numRho), CV_32SC1);
    //初始化map类，用于存储共线的点
    map<vector<int>, vector<Point>> lines;
    for(int i = 0; i < numRho; i++){
        for(int j = 0; j < numtheta; j++){
            lines.insert(make_pair(vector<int>(j, i), vector<Point>()));
        }
    }
    //投票计数
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            if(I.at<uchar>(Point(x, y)) == 255){
                for(int m = 0; m < numtheta; m++){
                    //对每一个角度，计算对应的rho值
                    float rho1 = x * cos(stepTheta * m / 180.0 * CV_PI);
                    float rho2 = y * sin(stepTheta * m / 180.0 * CV_PI);
                    float rho  = rho1 + rho2;
                    //计算投票到哪一个区域
                    int n = int(round(rho + L) / stepRho);
                    //累加1
                    accumulator.at<int>(n, m) += 1;
                    //记录该点
                    lines.at(vector<int>(m, n)).push_back(Point(x ,y));
                }
            }
        }
    }
    return lines;
}

int main(int argc, const char * argv[])
{
    //输入图像
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(img.empty()){
        perror("image read");
        exit(-1);
    }
    //图像边缘检测
    Mat edge;   
    Canny(img, edge, 50, 200);
    //霍夫直线检测
    Mat accu; //投票器
    map<vector<int>, vector<Point>> lines;
    lines = HTLine(edge, accu);
    //投票器的灰度级可视化
    double maxValue;
    minMaxLoc(accu, NULL, &maxValue, NULL, NULL);
    //数据类型转换， 对投票器进行灰度化
    Mat grayAccu;
    accu.convertTo(grayAccu, CV_32FC1, 1.0 / maxValue);
    namedWindow("投票器的灰度显示", WINDOW_NORMAL);
    resizeWindow("投票器的灰度显示", Size(1280, 720));
    imshow("投票器的灰度显示", grayAccu);
    //画出投票数大于某一阈值的直线
    int vote = 150;
    for(int r = 1; r < accu.rows - 1; r++){
        for(int c = 1; c < accu.cols - 1; c++){
            int current = accu.at<int>(r, c);
            //画直线
            if(current > vote){
                int lt = accu.at<int>(r - 1, c - 1);
                int t  = accu.at<int>(r - 1, c);
                int rt = accu.at<int>(r - 1, c + 1);
                int l  = accu.at<int>(r, c - 1);
                int ri = accu.at<int>(r, c + 1);
                int lb = accu.at<int>(r + 1, c - 1);
                int b  = accu.at<int>(r - 1, c);
                int rb = accu.at<int>(r + 1, c + 1);
                //判断该位置是不是局部最大值
                if(current > lt && current > t && current > rt && current > l && current > r && current > rb && current > b && current > lb){
                    vector<Point> linePoint = lines.at(vector<int>(c, r));
                    int len = linePoint.size();
                    cv::line(img, linePoint.at(0), linePoint.at(len - 1), Scalar(127, 127, 127), 2, CV_AA);
                }
            }
        }
    }
    imshow("img", img);
    waitKey(0);

    return 0;
}