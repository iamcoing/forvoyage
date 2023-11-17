#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    Mat img = Mat::zeros(Size(130, 130), CV_8UC1);
    
    vector<Point> contour;
    contour.push_back(Point(0, 0));
    contour.push_back(Point(50, 30));
    contour.push_back(Point(100, 100));
    contour.push_back(Point(100, 0));
    
    double length1 = arcLength(contour, false);
    double length2 = arcLength(contour, true);

    double area1   = contourArea(contour, false);
    double area2   = contourArea(contour, true);

    cout << "封閉的邊界  周長 " << length1 << " 面積 " << area1 << endl;
    cout << "打開的邊界  周長 " << length2 << " 面積 " << area2 << endl;
    
    for(int i = 0; i < contour.size() - 1; i++)
    {
        //用直線依次連接輪廓中相鄰的點
        line(img, contour[i], contour[i + 1], Scalar(255), 1);
    }
    //首尾相連
    line(img, contour[0], contour[contour.size() - 1], Scalar(255), 1);
    //標注點的位置
    circle(img, Point(80, 40), 3, Scalar(255), -1);
    circle(img, Point(50, 0), 3, Scalar(255), -1);
    circle(img, Point(40, 80), 3, Scalar(255), -1);

    double dist1 = pointPolygonTest(contour, Point2f(80, 40), true);
    double dist2 = pointPolygonTest(contour, Point2f(50, 0), true);
    double dist3 = pointPolygonTest(contour, Point2f(40, 80), true);

    cout << "dist1: " << dist1 << " dist2 " << dist2 << " dist3 " << dist3 << endl;

    imshow("img", img);
    waitKey(0);        

    return 0;
}