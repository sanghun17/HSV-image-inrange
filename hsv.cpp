#include <iostream> 
#include <opencv2/core/mat.hpp> 
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>


int mode = 2; // mode1 : for red , mode2: for gree2
using namespace cv;
using namespace std;

static void on_trackbar(int, void*) {

}

int main(int argc, char** argv) {

	string imageName("ss (142).jpg"); // by default
	if (argc > 1) {
		imageName = argv[1];
	}


	Mat image;
	image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file
	if (image.empty()) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	//int height = image.rows;
	//int width = image.cols;

	//resize
	Mat image_resized;

	resize(image, image_resized, Size(512, 384), 0, 0, INTER_LINEAR);

	medianBlur(image_resized, image_resized, 3);
	
	namedWindow("찾을 색범위 설정", WINDOW_NORMAL);
	
	//트랙바 생성
	createTrackbar("LowH", "찾을 색범위 설정", 0, 179, on_trackbar); //Hue (0 - 179)
	createTrackbar("HighH", "찾을 색범위 설정", 0, 179, on_trackbar);

	createTrackbar("LowS", "찾을 색범위 설정", 0, 255, on_trackbar); //Saturation (0 - 255)
	createTrackbar("HighS", "찾을 색범위 설정", 0, 255, on_trackbar);

	createTrackbar("LowV", "찾을 색범위 설정", 0, 255, on_trackbar); //Value (0 - 255)
	createTrackbar("HighV", "찾을 색범위 설정", 0, 255, on_trackbar);

	createTrackbar("C", "찾을 색범위 설정", 0, 20, on_trackbar);

	setTrackbarPos("LowH", "찾을 색범위 설정", 0);
	setTrackbarPos("HighH", "찾을 색범위 설정", 179);
	setTrackbarPos("LowS", "찾을 색범위 설정", 0);
	setTrackbarPos("HighS", "찾을 색범위 설정", 255);
	setTrackbarPos("LowV", "찾을 색범위 설정", 0);
	setTrackbarPos("HighV", "찾을 색범위 설정", 255);
	setTrackbarPos("Low C", "찾을 색범위 설정", 0);
	setTrackbarPos("High C", "찾을 색범위 설정", 20);

	Mat image_hsv;
	//HSV로 변환
	cvtColor(image_resized, image_hsv, COLOR_BGR2HSV);
	int RESIZE_LED = 4;
	while (1) {
		resize(image, image_resized, Size(512, 384), 0, 0, INTER_LINEAR);
		medianBlur(image_resized, image_resized, 3);
		int LowH = getTrackbarPos("LowH", "찾을 색범위 설정");
		int HighH = getTrackbarPos("HighH", "찾을 색범위 설정");
		int LowS = getTrackbarPos("LowS", "찾을 색범위 설정");
		int HighS = getTrackbarPos("HighS", "찾을 색범위 설정");
		int LowV = getTrackbarPos("LowV", "찾을 색범위 설정");
		int HighV = getTrackbarPos("HighV", "찾을 색범위 설정");
		int C = getTrackbarPos("C", "상수");


		Mat image_binary;
		//지정한 HSV 범위를 이용하여 영상을 이진화
		inRange(image_hsv, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), image_binary);
		uchar* image_data = (uchar*)image_resized.data;
		uchar* data = (uchar*)image_binary.data;
		int length = (2048 / RESIZE_LED) * (1536 / RESIZE_LED);

		if (mode == 1) {
			for (int i = 0; i < length; i++) {
				if (image_data[3 * i + 2] < (image_data[3 * i] + image_data[3 * i + 1]) / 2 + C)
					data[i] = 0;

			}
		}

		if (mode == 2) {
			for (int i = 0; i < length; i++) {
			if (image_data[3 * i + 1] < (image_data[3 * i + 2] + image_data[3 * i]) / 2 + C)
				data[i] = 0;
			}
		}
		medianBlur(image_binary, image_binary, 3);
		medianBlur(image_binary, image_binary, 3);
		medianBlur(image_binary, image_binary, 3);
		
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		findContours(image_binary, contours, hierarchy, 0, 4, Point());


		Mat drawing = Mat::zeros(image_binary.size(), CV_8UC3); for (int i = 0; i < contours.size(); i++) { Scalar color = Scalar(0,0,255); drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point2i()); }


		namedWindow("원본 영상", WINDOW_KEEPRATIO);
		imshow("원본 영상", image_resized);
		namedWindow("이진화", WINDOW_KEEPRATIO);
		imshow("이진화", drawing);


		if (waitKey(1) == 27)
			break;

		
	}
	destroyAllWindows();

	return 0;
}