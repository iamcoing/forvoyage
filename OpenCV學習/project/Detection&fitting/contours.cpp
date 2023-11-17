#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat img;

int main(int argc, const char * argv[])
{
    img = imread(argv[1], IMREAD_GRAYSCALE);
    if(img.empty()){
        cout << "image read failed" << endl;
        return -1;
    }
    Mat edge = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);
    GaussianBlur(img, edge, Size(3, 3), 0.5);
    Canny(edge, edge, 50, 200);
    imshow("canny", edge);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edge, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    Mat canvas = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);
    for(int i = 0; i < contours.size(); i++){
        drawContours(canvas, contours, i, Scalar(255, 255, 255), -1);
        imshow("contours", canvas);
    }

    waitKey(0);
    return 0;
}