#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(img.empty()){
        cout << "image read failed" << endl;
        return -1;
    }
    Mat edge = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);
    /* 高斯平滑，自適應閾值處理 */
    adaptiveThreshold(img, edge, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 0);
    /* 形態學處理，底帽變換 */
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(edge, edge, MORPH_BLACKHAT, kernel, Point(-1, -1), 3);
    // imshow("blackHat", edge);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edge, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    /* 創建一張畫布 */
    Mat canvas = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);
    /* 得到最小外包圓 */
    float radi, MaxRadius = 0.0;
    Point2f cent, MaxCenter = Point2f(0.0, 0.0);
    Mat cir = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);

    /* 繪制外包凸包 */
    vector<Point> polys;
    /* 遍歷， 計算點集的集合 */
    for(int i = 0; i < contours.size(); i++){
        /* 繪制輪廓 */
        drawContours(img, contours, i, Scalar(0), 2, LINE_MAX);
        /* 繪制外包園 */
        minEnclosingCircle(contours.at(i), cent, radi);
        if(MaxRadius < radi){
            MaxRadius = radi;
            MaxCenter = cent;
        }
        /* 繪制凸包 */
        approxPolyDP(contours.at(i), polys, 2, 0);
        for(int j = 0; j < polys.size(); j++){
            int k = (j + 1) % polys.size(); 
            cv::line(canvas, polys[j], polys[k], Scalar(255), 2, LINE_MAX);
        }
    }
    circle(canvas, MaxCenter, MaxRadius, Scalar(255), 1, LINE_MAX);
    imshow("images", img);
    imshow("canvas", canvas);

    waitKey(0);
    return 0;
}