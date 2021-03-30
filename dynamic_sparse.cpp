#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <chrono>

using namespace std::chrono;
using namespace cv;
using namespace std;

Mat H;
Rect region;
Mat gray_frame;
Mat frame;
Mat prvs_frame;
vector<Point2f> corner_points_prev, corner_points_next;
vector<long double> aa;

void findSparse()
{
	TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
	vector<uchar> status;
	vector<float> error;
	calcOpticalFlowPyrLK(prvs_frame, gray_frame, corner_points_prev, corner_points_next, status, error, Size(15, 15), 2, criteria);
	int xyz = corner_points_prev.size();
	long double s = 0;
	for (int i = 0; i < xyz; i++)
	{
		if (status[i] == 1)
		{
			Point2f p1, p2;
			p1 = corner_points_prev[i];
			p2 = corner_points_next[i];
			long double dis = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
			s += dis;
		}
	}
	aa.push_back(s);
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
	String window_name = "Traffic_Video"; //window names
	//namedWindow(window_name, WINDOW_NORMAL);

	Mat prvs;
	cap.read(prvs);

	cvtColor(prvs, prvs_frame, COLOR_BGR2GRAY);
	//coordinates for finding the homography matrix for angle correction of each frame
	int x0 = 472, y0 = 52, x1 = 472, y1 = 830, x2 = 800, y2 = 830, x3 = 800, y3 = 52;

	vector<Point2f> a = {Point2f(x0, y0), Point2f(x1, y1), Point2f(x2, y2), Point2f(x3, y3)};
	vector<Point2f> b = {Point2f(984, 204), Point2f(264, 1068), Point2f(1542, 1068), Point2f(1266, 222)};

	region.x = a[0].x;
	region.y = a[0].y;
	region.width = a[2].x - a[0].x;
	region.height = a[2].y - a[0].y;
	H = findHomography(b, a);									   //homography matrix
	warpPerspective(prvs_frame, prvs_frame, H, prvs_frame.size()); //angle corrected
	prvs_frame = prvs_frame(region);							   //cropped background
	int count = 0;
	
	goodFeaturesToTrack(prvs_frame, corner_points_prev, 100, 0.05, 100);

	while (true)
	{
		bool bSuccess = cap.read(frame);
		if (bSuccess == false)
		{
			// cout << "Found the end of the video" << endl;
			break;
		}

		// if (count % 5 != 0)
		// {
		// 	count++;
		// 	continue;
		// }
		cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
		warpPerspective(gray_frame, gray_frame, H, gray_frame.size());
		gray_frame = gray_frame(region);
		findSparse();
		//imshow(window_name,toShow);
		prvs_frame = gray_frame;
		count++;

		corner_points_prev.clear();
		goodFeaturesToTrack(prvs_frame, corner_points_prev, 100, 0.05, 100);
		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stopping the video" << endl;
			break;
		}
	}

	ofstream answer;
	answer.open("dynamic/sparse.txt");
	//below part stores output in out.txt
	answer << "time_sec\tdynamic_density\n";
	long double big = 0;
	for (int i = 0; i < aa.size(); i++)
		big = max(big, aa[i]);
	for (int i = 0; i < aa.size(); i++)
		answer << (long double)(i + 1) / 15 << "\t" << aa[i] / (big + 1) << "\n";
	answer.close();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken using sparse optical flow: " << duration.count() / 1000000 << "s\n";
	return 0;
}