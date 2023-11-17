#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    vector<Point2f> points;
    points.push_back(Point2f(1, 100));
    points.push_back(Point2f(50, 1));
    points.push_back(Point2f(1, 10));
    points.push_back(Point2f(5, 100));
    points.push_back(Point2f(20, 50));

    vector<Point2f> hull;
    convexHull(points, hull);
    Mat img = Mat::zeros(Size(200, 200), CV_8UC1);

    for(int i = 0; i < hull.size(); i++){
        int j = (i + 1) % hull.size();
        line(img, hull.at(i), hull.at(j), Scalar(255), 2);
    }

    imshow("convtexHull", img);
    waitKey(0);

    return 0;
}