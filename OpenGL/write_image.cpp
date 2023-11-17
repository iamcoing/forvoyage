#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
	Size boardSize, imageSize;
	string outputFilename;
	string inputFilename = "";

	cv::CommandLineParser parser(argc, argv, 
			"{w||}{h||}{@input||}");

	imageSize.width  = parser.get<int>("w");
	imageSize.height = parser.get<int>("h");
	inputFilename	 = parser.get<string>("@input");

	cout << imageSize << "  " << inputFilename << endl;

    return 0;
}
