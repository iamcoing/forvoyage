#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "radar_tex0.hpp"
#include "radar_tex1.hpp"
#include "radar_tex2.hpp"
#include "radar_tex3.hpp"
#include "radar_tex4.hpp"
#include "radar_tex5.hpp"
#include "radar_tex6.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    cv::Mat Img0(Size(256, 256), CV_8UC4, radar_tex_color0);
    cv::Mat Img1(Size(256, 256), CV_8UC4, radar_tex_color1);
    cv::Mat Img2(Size(256, 256), CV_8UC4, radar_tex_color2);
    cv::Mat Img3(Size(256, 256), CV_8UC4, radar_tex_color3);
    cv::Mat Img4(Size(256, 256), CV_8UC4, radar_tex_color4);
    cv::Mat Img5(Size(256, 256), CV_8UC4, radar_tex_color5);
    cv::Mat Img6(Size(256, 256), CV_8UC4, radar_tex_color6);

    cvtColor(Img0, Img0, COLOR_RGBA2BGRA);

    imwrite("radar/image0.png", Img0);
    imwrite("radar/image1.png", Img1);
    imwrite("radar/image2.png", Img2);
    imwrite("radar/image3.png", Img3);
    imwrite("radar/image4.png", Img4);
    imwrite("radar/image5.png", Img5);
    imwrite("radar/image6.png", Img6);

    waitKey(0);

    return 0;
}