#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void test01()
{
    float test = 100.0;
    
    for(int i = 0; i < 10000; i++){
        test -= 0.1;
        if(test != 0){
            cout << test << " cout: " << i <<  endl;
        }
        if(test == 0)
            break;
    }

    return;
}

int main(int argc, const char * argv[])
{
    test01();
    // float value = 0.0;
    // int a_;
    // Mat test(Size(1220, 1220), CV_8UC4, Scalar(0, 97, 0, 64));
    // a_ = (int)value; 
    // // cout << (float)(a_) << endl;
    // // cout << test << endl;
    // for(int r = 0; r < test.rows; r++){
    //     for(int c = 0; c < test.cols; c++);
    //     //     cout << *(char *)(test.data + (r + 1) * c);
    //     // cout << endl;
    // }
    // namedWindow("test", WINDOW_NORMAL);
    // resizeWindow("test", Size(1200, 1200));
    // moveWindow("test", 500, 500);
    // imshow("test", test);
    // waitKey(0);

    return 0;
}