#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const int width = 2560;
const int height = 1440;
const int framesize = width * height * 3/2;

int main(int argc, const char * argv[])
{
    ifstream fin;
    fin.open(argv[1], ios_base::in | ios_base::binary);
    if(fin.fail()){
        cout << "file input failed" << endl;
        return -1;
    }
    fin.seekg(0, ios::end);
    streampos ps = fin.tellg();
    unsigned long pixels = ps;
    cout << ps << endl;

    fin.close();

    FILE * filein = fopen(argv[1], "rb+");
    unsigned char * pYuvBuf = new unsigned char[framesize];

    fread(pYuvBuf, framesize, 1, filein);
    Mat yuvImg;
    yuvImg.create(height * 3 / 2, width, CV_8UC1);
    
    memcpy(yuvImg.data, pYuvBuf, framesize);
    
    Mat rgbImg;
    cvtColor(yuvImg, rgbImg, CV_YUV2BGR_NV12);

    Mat rgb = rgbImg(Range(720, 1440), Range(1280, 2560));

    imshow("Img", rgb);
    waitKey(0);

    return 0;
}