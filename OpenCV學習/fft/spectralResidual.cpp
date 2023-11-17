/**
 * @file spectralResidual.cpp
 * @author your name (you@domain.com)
 * @brief 譜殘差法檢測圖像的顯著性
 * @version 0.1
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void fft2Image(InputArray img, OutputArray FFTimg)
{
    Mat i = img.getMat();
    int rows = i.rows;
    int cols = i.cols;

    /* 補足矩陣，能夠做快速傅立葉變換 */
    int rPadded = getOptimalDFTSize(rows);
    int cPadded = getOptimalDFTSize(cols);

    Mat temp;
    copyMakeBorder(i, temp, 0, rPadded - rows, 0, cPadded - cols, BORDER_CONSTANT, Scalar::all(0));
    dft(temp, FFTimg, DFT_COMPLEX_OUTPUT);
}

void amplitudeSpectrum(InputArray _srcFFT, OutputArray _dstSpectrum)
{
    CV_Assert(_srcFFT.channels() == 2);
    vector<Mat> FFT2Channel;
    /* 分離矩陣，計算矩陣的平方根 */
    split(_srcFFT, FFT2Channel);
    magnitude(FFT2Channel[0], FFT2Channel[1], _dstSpectrum);
}

/* 得到灰度矩陣 */
Mat graySpectrum(Mat spectrum)
{
    Mat dst;
    log(spectrum + 1, dst);
    normalize(dst, dst, 0, 1, NORM_MINMAX);
    dst.convertTo(dst, CV_8UC1, 1.0, 0);

    return dst;
}

/* 得到相位矩陣 */
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
    /* 制作中間轉換用矩陣 */
    Mat fftemp;
    img.convertTo(fftemp, CV_64FC1, 1.0 / 255);

    Mat FFT;
    fft2Image(fftemp, FFT);

    Mat amp;
    amplitudeSpectrum(FFT, amp);

    Mat logAmplitude;
    log(amp + 1.0, logAmplitude);

    Mat smoothgrayamp;
    /* 均值平滑 */
    boxFilter(logAmplitude, smoothgrayamp, CV_32FC1, Size(3, 3));

    Mat spectralResidual = smoothgrayamp.clone();
    logAmplitude - smoothgrayamp;
    
    Mat phaseMat;
    phaseMat = phaseSpectrum(FFT);

    Mat cosSpectrum(phaseMat.size(), CV_64FC1);
    Mat sinSpectrum(phaseMat.size(), CV_64FC1);

    for(int r = 0; r < phaseMat.rows; r++){
        for(int c = 0; c < phaseMat.cols; c++){
            cosSpectrum.at<double>(r, c) = cos(phaseMat.at<double>(r, c));
            sinSpectrum.at<double>(r, c) = sin(phaseMat.at<double>(r, c));
        }
    }
    cosSpectrum.convertTo(spectralResidual, CV_64FC1);
    exp(spectralResidual, spectralResidual);
    Mat real      = spectralResidual.mul(cosSpectrum);
    Mat imaginary = spectralResidual.mul(sinSpectrum);
    // cout << spectralResidual.cols << " " << cosSpectrum.cols << " " << spectralResidual.type() << " " << spectralResidual.rows << " " << cosSpectrum.rows << " " << cosSpectrum.type() << endl;
    cout << "|*******************************************|" << endl;
 
    vector<Mat> realAndImag;
    realAndImag.push_back(real);
    realAndImag.push_back(imaginary);
    Mat complex;
    merge(realAndImag, complex);
   
    Mat ifft2;
    dft(complex, ifft2, DFT_COMPLEX_OUTPUT + DFT_INVERSE);
    Mat ifft2Amp;
    amplitudeSpectrum(ifft2, ifft2Amp);
    pow(ifft2Amp, 2.0, ifft2Amp);
    GaussianBlur(ifft2Amp, ifft2Amp, Size(11, 11), 2.5);
    normalize(ifft2Amp, ifft2Amp, 1.0, 0, NORM_MINMAX);
    pow(ifft2Amp, 0.5, ifft2Amp);

    Mat saliencyMap;
    ifft2Amp.convertTo(saliencyMap, CV_8UC1, 255);

    imshow("saliency", saliencyMap);

    waitKey(0);

    return 0;
}