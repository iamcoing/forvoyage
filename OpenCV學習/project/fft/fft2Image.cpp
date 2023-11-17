#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void fft2Image(InputArray img, OutputArray F)
{
    Mat i = img.getMat();
    int rows = i.rows;
    int cols = i.cols;

    int rPadded = getOptimalDFTSize(rows);
    int cPadded = getOptimalDFTSize(cols);

    Mat f;
    copyMakeBorder(i, f, 0, rPadded - rows, 0, cPadded - cols, BORDER_CONSTANT, Scalar::all(0));
    dft(f, F, DFT_COMPLEX_OUTPUT);
}

void amplitudeSpectrum(InputArray _srcFFT, OutputArray _dstSpectrum)
{
    CV_Assert(_srcFFT.channels() == 2);
    vector<Mat> FFT2Channel;
    split(_srcFFT, FFT2Channel);
    magnitude(FFT2Channel[0], FFT2Channel[1], _dstSpectrum);
}

Mat graySpectrum(Mat spectrum)
{
    Mat dst;
    log(spectrum + 1, dst);
    normalize(dst, dst, 0, 1, NORM_MINMAX);
    dst.convertTo(dst, CV_8UC1, 255, 0);
    return dst;
}

Mat phaseSpectrum(Mat _srcFFT)
{
    Mat pha;
    pha.create(_srcFFT.size(), CV_64FC1);
    vector<Mat> FFT2Channel;
    split(_srcFFT, FFT2Channel);
    phase(FFT2Channel[0], FFT2Channel[1], pha, true);
    return pha;
}

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(img.empty()){
        cout << "image read failed" << endl;
        return -1;
    }
    Mat fI;
    img.convertTo(fI, CV_64FC1);
    Mat F;
    fft2Image(fI, F);
    /* 與逆變換作差作差，校驗傅立葉變換結果 */
    // Mat iF;
    // cv::dft(F, iF, (DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE));

    // Mat i = img(Rect(0, 0, img.cols, img.rows)).clone();
    // i.convertTo(i, CV_8U);
    // cout << i.channels() << endl;
    /* 顯示幅度圖的灰度級 */
    Mat amplitu = img(Rect(0, 0, img.cols, img.rows)).clone();
    amplitudeSpectrum(F, amplitu);
    
    Mat grayamplitu = img(Rect(0, 0, img.cols, img.rows)).clone();
    grayamplitu = graySpectrum(amplitu);
    imshow("AMP", grayamplitu);
    /* 顯示相位譜的灰度 */
    Mat phaseSpe = img(Rect(0, 0, img.cols, img.rows)).clone();
    phaseSpe = phaseSpectrum(F);
    imshow("phase", phaseSpe);

    int rows = img.rows, cols = img.cols;
    Mat fimg;
    img.convertTo(fimg, CV_64FC1);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if((r + c) % 2){
                img.at<double>(r, c) = -img.at<double>(r, c);
            }
        }
    }
    imshow("fimg", fimg);
    Mat imgfft2;
    fft2Image(fimg, imgfft2);
    Mat amSp;
    amplitudeSpectrum(imgfft2, amSp);
    Mat graySpe;
    graySpe = graySpectrum(amSp);
    imshow("amSp", graySpe);

    graySpe.convertTo(graySpe, CV_8U);
    graySpe *= 255;
    imshow("centerAmp", graySpe);

    waitKey(0);

    return 0;
}