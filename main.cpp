//Introduction to Image Processing #Homework 3 
//1023311
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <string>
using namespace cv;
using namespace std;

void histEqualize(Mat &src, Mat &dst); //Histogram equalization
void drawHist(const Mat &src, Mat &dst); // Draw Histogram

int main()
{
	Mat image, hist1, hist2;
	int histSize = 256;
	float range[] = { 0, 255 };
	const float* histRange = { range };

	string file;
	cout << "Enter the file name: ";
	cin >> file;
		
	image = imread(file, CV_LOAD_IMAGE_GRAYSCALE);

	if (!image.data) {// Check for invalid input
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}
	Mat out = Mat::zeros(image.size(), image.type());

	histEqualize(image, out); //eq

	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	imshow("Original", image);
	waitKey(0);

	calcHist(&image, 1, 0, Mat(), hist1, 1, &histSize, &histRange);
	Mat showHist1(256, 256, CV_8UC1, Scalar(255));
	drawHist(hist1, showHist1);

	namedWindow("Histogram(Original)", CV_WINDOW_AUTOSIZE);
	imshow("Histogram(Original)", showHist1);
	waitKey(0);

	namedWindow("Histogram Equalization", CV_WINDOW_AUTOSIZE);
	imshow("Histogram Equalization", out);
	waitKey(0);


	calcHist(&out, 1, 0, Mat(), hist2, 1, &histSize, &histRange);
	Mat showHist2(256, 256, CV_8UC1, Scalar(255));
	drawHist(hist2, showHist2);

	namedWindow("Histogram(Equalized)", CV_WINDOW_AUTOSIZE);
	imshow("Histogram(Equalized)", showHist2);
	waitKey(0);

	imwrite("Histogram_equalized.jpg", out);

	return 0;
}

void histEqualize(Mat &src, Mat &dst)
{
	int gray[256] {0}; //Ex: gray[0] = gray value 0 appaer times, gray[1] = gray value 1 appaer times, ...
	for (int y = 0; y < src.rows; y++) { 
		for (int x = 0; x < src.cols; x++) { //count each gray value appaer times
			gray[(int)src.at<uchar>(y, x)]++;
		} 
	} 
	
	int mincdf; //Minimum of cdf
	
	for (int i = 0; i < 255; i++) { //calculate cdf (Cumulative distribution function)
		gray[i + 1] += gray[i];
	}

	for (int i = 0; i < 255; i++) { //find minimum of cdf
		if (gray[i] != 0) {
			mincdf = gray[i];
			break;
		}
	}

	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			// h(v) = round(((cdf(v) - mincdf) / (M * N) - mincdf) * (L - 1)) ; L = 2^8
			dst.at<uchar>(y, x) = (uchar)round((((double)gray[(int)src.at<uchar>(y, x)] - mincdf) / (double)(src.rows * src.cols - mincdf)) * (double)255);
		}
	}
}

void drawHist(const Mat &src, Mat &dst)
{
	int histSize = 256;
	float histMax = 0;
	for (int i = 0; i < histSize; i++) {
		float temp = src.at<float>(i);
		if (histMax < temp) {
			histMax = temp;
		}
	}

	float scale = (0.9 * 256) / histMax;
	for (int i = 0; i < histSize; i++) {
		int intensity = static_cast<int>(src.at<float>(i)*scale);
		line(dst, Point(i, 255), Point(i, 255 - intensity), Scalar(0));
	}
}