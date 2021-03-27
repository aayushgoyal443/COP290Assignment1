#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <chrono>
using namespace cv;
using namespace std;
using namespace std::chrono;

vector<long double> aa; //for Dynamic density
int NUM_THREADS = 0;

Mat H;
Rect region; //region shows the section of window which is to be cropped
vector<Rect> stripes;
Mat gray_frame;

Mat prvs_frame;

vector<int> blacksum;

// Function to find the queue density and fill the "aa" vector
void *findDynamic(void *t1)
{
	int *id1 = (int *)t1;
	int id = *id1;
	Rect strip = stripes[id];
	Mat img3;
	Mat gray_frame1 = gray_frame(strip);
	Mat prvs_frame1 = prvs_frame(strip);
	
	Mat flow(prvs_frame1.size(), CV_32FC2);
	calcOpticalFlowFarneback(prvs_frame1, gray_frame1, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
    Mat flow_parts[2];
    split(flow, flow_parts);
    Mat magnitude, angle, magn_norm;
    cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
    angle *= ((1.f / 360.f) * (180.f / 255.f));
    //build hsv image
    Mat _hsv[3], hsv, hsv8, bgr;
    _hsv[0] = angle;
    _hsv[1] = Mat::ones(angle.size(), CV_32F);
    _hsv[2] = magn_norm;
    merge(_hsv, 3, hsv);
    hsv.convertTo(hsv8, CV_8U, 255.0);
    cvtColor(hsv8, bgr, COLOR_HSV2BGR);
    cvtColor(bgr,flow,COLOR_BGR2GRAY);
    Mat flow_binary;
    threshold(flow, flow_binary, 10, 255, THRESH_BINARY);

	blacksum[id] = sum(flow_binary)[0];
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	NUM_THREADS = stoi(argv[1]);

	auto start = high_resolution_clock::now();

	VideoCapture cap("trafficvideo.mp4"); //video filename is given as argument

	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get();
		return -1;
	}
	Mat prvs1, prvs1_changed;
	cap.read(prvs1);

	cvtColor(prvs1, prvs1_changed, COLOR_BGR2GRAY);

	//coordinates for finding the homography matrix for angle correction of each frame
	int x0 = 472, y0 = 52, x1 = 472, y1 = 830, x2 = 800, y2 = 830, x3 = 800, y3 = 52;
	vector<Point2f> a = {Point2f(x0, y0), Point2f(x1, y1), Point2f(x2, y2), Point2f(x3, y3)};
	vector<Point2f> b = {Point2f(984, 204), Point2f(264, 1068), Point2f(1542, 1068), Point2f(1266, 222)};

	region.x = a[0].x;
	region.y = a[0].y;
	region.width = a[2].x - a[0].x;
	region.height = a[2].y - a[0].y;

	H = findHomography(b, a); //homography matrix

	warpPerspective(prvs1_changed, prvs1_changed, H, prvs1_changed.size()); //angle corrected

	prvs1_changed = prvs1_changed(region); //cropped background

	/////// Making NUM_THREADS number regions to divide into strips

	int hori = prvs1_changed.cols / NUM_THREADS;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		Rect r;
		if (i > 0)
			r.x = stripes[i - 1].x + stripes[i - 1].width;
		else
			r.x = 0;
		r.y = 0;
		if (i != NUM_THREADS - 1)
			r.width = hori;
		else
			r.width = prvs1_changed.cols - r.x;
		r.height = prvs1_changed.rows;
		stripes.push_back(r);
	}

	int ind[NUM_THREADS] = {};
	for (int i = 0; i < NUM_THREADS; i++)
		ind[i] = i;

	int count = 0;

	pthread_t threads[NUM_THREADS];
	int rc;
	Mat gray_frame1;
	Mat frame;
	// cout <<"bonjour\n";
	while (true)
	{
		bool bSuccess = cap.read(frame);
		if (bSuccess == false)
		{
			// cout << "Found the end of the video" << endl;
			break;
		}
		// cout << "Frame number: " << count << "\n";

		cvtColor(frame, gray_frame1, COLOR_BGR2GRAY);
		warpPerspective(gray_frame1, gray_frame1, H, gray_frame1.size());
		gray_frame1 = gray_frame1(region);

		//////// CREATING THREADS FOR THEM
		if (count == 0)
		{
			blacksum = vector<int>(NUM_THREADS, 0);
			gray_frame = gray_frame1;
			prvs_frame = prvs1_changed;
			for (int i = 0; i < NUM_THREADS; i++)
			{
				// cout << "main() : creating thread, " << i << endl;
				rc = pthread_create(&threads[i], NULL, findDynamic, &ind[i]);
				if (rc)
				{
					cout << "Error:unable to create thread," << rc << endl;
					exit(-1);
				}
			}
		}
		else
		{
			void *status;
			for (int i = 0; i < NUM_THREADS; i++)
			{
				rc = pthread_join(threads[i], &status);
				if (rc)
				{
					cout << "Error:unable to join," << rc << endl;
					exit(-1);
				}
				// cout << "Main: completed thread id :" << i;
				// cout << "  exiting with status :" << status << endl;
			}
			int sum = 0;
			for (auto x : blacksum)
				sum += x;

			aa.push_back(sum);

			blacksum = vector<int>(NUM_THREADS, 0);
			gray_frame = gray_frame1;
			prvs_frame = prvs1_changed;
			for (int i = 0; i < NUM_THREADS; i++)
			{
				// cout << "main() : creating thread, " << i << endl;
				rc = pthread_create(&threads[i], NULL, findDynamic, &ind[i]);
				if (rc)
				{
					cout << "Error:unable to create thread," << rc << endl;
					exit(-1);
				}
			}
		}
		prvs1_changed = gray_frame1;
		count++;
	}
	void *status;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		rc = pthread_join(threads[i], &status);
		if (rc)
		{
			cout << "Error:unable to join," << rc << endl;
			exit(-1);
		}
		// cout << "Main: completed thread id :" << i;
		// cout << "  exiting with status :" << status << endl;
	}
	int sum = 0;
	for (auto x : blacksum)
		sum += x;

	aa.push_back(sum);

	//////////// FOR STORING OUTPUT /////////

	long double tot = (prvs1_changed.rows) * (prvs1_changed.cols) * 3 * 255;
	ofstream answer;
	answer.open("dynamic_spatial.txt");
	answer << "time_sec\tDynamic_density\n";
	for (int i = 0; i < aa.size(); i++)
		answer << (long double)(i + 1) / 15 << "\t" << aa[i] / tot << "\n";
	answer.close();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken with " << NUM_THREADS << " threads: " << duration.count() / 1000000 << "s\n";

	return 0;
}