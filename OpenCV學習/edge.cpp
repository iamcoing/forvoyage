#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>    
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if( img.empty() ){
        perror("imread");
        exit(-1);
    }
    Mat s = getStructuringElement(MORPH_RECT, Size(11, 11));

    Mat erod;
    erode(img, erod, s, Point(-1, -1), 1);
    imshow("I", img);
    imshow("E", erod);

    waitKey(0);

    return 0;
}