#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], IMREAD_GRAYSCALE);
    if(img.empty()){
        perror("image read");    
        return -1;
    }
    vector<Vec3f> hcirs;
    HoughCircles(img, hcirs, CV_HOUGH_GRADIENT, 1, 70, 200, 60, 100);
    cout << "size " << hcirs.size() << " value " << hcirs.at(0)[2] << endl;

    for(int i = 0; i < hcirs.size(); i++){
        cv::circle(img, Point(hcirs.at(i)[0], hcirs.at(i)[1]), hcirs.at(0)[2], Scalar(255), 2);
    }

    imshow("circles", img);
    waitKey(0);

    return 0;   
}