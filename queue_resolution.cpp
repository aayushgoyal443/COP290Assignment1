#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <chrono>
using namespace cv;
using namespace std;
using namespace std::chrono;

vector<long double> aa; //for queue density

Mat bg_changed; //grayscale background image
Mat H;
Rect region; //region shows the section of window which is to be cropped
Mat gray_frame;

// Function to find the queue density and fill the "aa" vector
int findQueue()
{
	Mat img3;
	absdiff(gray_frame, bg_changed, img3);
	Mat img3_binary;
	threshold(img3, img3_binary, 25, 255, THRESH_BINARY);
	return sum(img3_binary)[0];
}

int main(int argc, char *argv[])
{
	auto start = high_resolution_clock::now();

	VideoCapture cap("trafficvideo.mp4"); //video filename is given as argument

	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get();
		return -1;
	}

	Mat bg = imread("background.jpg"); //background image
    int old_rows = bg.rows; //1080
    int old_cols = bg.cols; //1920
    int factor = stoi(argv[1]);
    int new_rows = old_rows/factor;
    int new_cols = old_cols/factor;
    resize(bg, bg, Size(new_cols,new_rows));

	cvtColor(bg, bg_changed, COLOR_BGR2GRAY);

	//coordinates for finding the homography matrix for angle correction of each frame
	int x0 = 472, y0 = 52, x1 = 472, y1 = 830, x2 = 800, y2 = 830, x3 = 800, y3 = 52;
	vector<Point2f> a = {Point2f(x0, y0), Point2f(x1, y1), Point2f(x2, y2), Point2f(x3, y3)};
	vector<Point2f> b = {Point2f(984, 204), Point2f(264, 1068), Point2f(1542, 1068), Point2f(1266, 222)};

    for (auto &u: a){
        u.x /=  factor;
        u.y /= factor;
        u.x = floor(u.x);
        u.y = floor(u.y);
    }
    for (auto &u: b){
        u.x /=  factor;
        u.y /= factor;
        u.x = floor(u.x);
        u.y = floor(u.y);
    }

	H = findHomography(b, a); //homography matrix

	warpPerspective(bg_changed, bg_changed, H, bg_changed.size()); //angle corrected

	region.x = a[0].x;
	region.y = a[0].y;
	region.width = a[2].x - a[0].x;
	region.height = a[2].y - a[0].y;

	bg_changed = bg_changed(region); //cropped background

	int count = 0;

	Mat frame;
	while (true)
	{
		bool bSuccess = cap.read(frame);
        
		if (bSuccess == false)
		{
			// cout << "Found the end of the video" << endl;
			break;
		}
		// cout << "Frame number: " << count << "\n";
        resize(frame, frame, Size(new_cols,new_rows));
		cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
		warpPerspective(gray_frame, gray_frame, H, gray_frame.size());
		gray_frame = gray_frame(region);

		int sum = findQueue();
		aa.push_back(sum);

		count++;
	}

	//////////// FOR STORING OUTPUT /////////

	long double tot = (bg_changed.rows) * (bg_changed.cols) * 255;
	ofstream answer;
	answer.open("queue/resolution/"+to_string(factor)+ ".txt");
	answer << "time_sec\tqueue_density\n";
	for (int i = 0; i < aa.size(); i++)
		answer << (long double)(i + 1) / 15 << "\t" << aa[i] / tot << "\n";
	answer.close();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken if reduced by factor "+to_string(factor) +": " << duration.count() / 1000000 << "s\n";	

	return 0;
}