#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

#include <cstdio>

using namespace cv;
using namespace std;

#define WINDNAME "picture1"

int main(int argc, const char * argv[])
{
    // Mat img = (Mat_<float>(2, 3) << 11.1, 22.2, 33.3, 44.4, 55.5, 66.6);

    // cout << "size: " << img.size() << endl;
    // cout << "row: " << img.rows << "  col: " << img.cols << endl;
    // cout << "channel: " << img.channels() << endl;
    // cout << "sqe: " << img.total() << endl;
    // cout << "dims: " << img.dims << endl;
  
    // if( img.isContinuous() ){
    //     const int * ptr = img.ptr<int>(0);
        // for(int i = 0; i < img.rows * img.cols; i++){
        //     cout << ptr[i] << " , ";
        //     if( ! ((i+ 1) % img.cols) ){
        //         cout << endl;
        //     }
        // }
    // }
    // else 
    //     for(int r = 0; r < img.rows; r++)
    //     {
    //         // const int * ptr = img.ptr<int>(r);

    //         for(int c = 0; c < img.cols; c++)
    //         {
    //             cout << img.at<float>(r, c) << ", ";
    //         }
    //         cout << endl;
    //     }

    // Vec<int, 3> vi(21, 32, 14);
    // cout << "rows " << vi.rows << endl;
    // cout << "cols " << vi.cols << endl;

    // Mat img = (Mat_<Vec3f>(2, 2) << Vec3f(1, 11, 21), Vec3f(2, 12, 22),
    //                                 Vec3f(3, 13, 23), Vec3f(4, 14, 34));

    // Vec3f *ptr = img.ptr<Vec3f>(0);
    // for(int n = 0; n < img.rows; n++)
    //     for(int m = 0; m < img.cols; m++){
    //         Vec3f * ptr = (Vec3f *)(img.data + m * img.step[0] + n * img.step[1]);
    //         cout << *ptr << endl;
    //     }

    // Mat planes[3];
    // split(img, planes);
    // for(int i = 0; i < img.rows; i++)
    // {
    //     cout << planes[i] << endl;
    // }

    // Mat plane[] = {
    //     (Mat_<int>(3, 2) << 1, 2, 3, 4, 5, 6),
    //     (Mat_<int>(3, 2) << 7, 8, 9, 10, 11, 12),
    //     (Mat_<int>(3, 2) << 13, 14, 15, 16, 17, 18),
    // };

    // Mat mati;
    // merge(plane, 3, mati);

    // Mat img = mati.rowRange(0, 2);
    // cout << img.rows << "  " << img.cols << endl;
    // for(int n = 0; n < img.rows; n++){
    //     const int * ptr = img.ptr<int>(n);
    //     for(int m = 0; m < img.cols; m++){
    //         cout << ptr[m] << ", ";
    //     }
    //     cout << endl;
    // }
    // cout << mati.rows << "    " << mati.cols << endl;

    Mat img = imread("1234.jpg", CV_LOAD_IMAGE_ANYCOLOR);
    if( img.empty() ){
        cout << "image read failed" << endl;
        exit(-1);
    }
    imshow("BGR", img);
    
    // vector<Mat> planes;
    // split(img, planes);

    // imshow("B", planes[0]);
    // imshow("G", planes[1]);
    // imshow("R", planes[2]);

    // waitKey(0);
    // Mat dts1, dts2;

    // imshow("source", img);
    // //create a scalte array;
    // Mat s = (Mat_<float>(2, 3) << 0.3, 0, 0, 0, 0.3, 0);
    // //shi yong warp fangfa 
    // warpAffine(img, dts1, s, Size(img.cols / 2, img.rows /2 ));
    // //shi yong resize fang fa
    // resize(img, dts2, Size(img.cols / 2, img.rows / 2), 0.2, 0.2);
    // imshow("warpAffine", dts1);
    // imshow("resize", dts2);

    // rotate(img, img, ROTATE_180);
    // imshow("180", img);
    // rotate(img, img, ROTATE_90_CLOCKWISE);
    // imshow("90", img);
    // rotate(img, img, ROTATE_90_COUNTERCLOCKWISE);
    // imshow("270", img);


    waitKey(0);

    return 0;
}