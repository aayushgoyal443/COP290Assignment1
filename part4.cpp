#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <pthread.h>
#include <chrono>
using namespace std::chrono;

using namespace cv;
using namespace std;

int NUM_THREADS = 10;

Mat bg_changed;
Rect region;

struct have_fun
{
	Mat fr;
	Mat ho;
	int id;
};

vector<long double> aa; //for queue density
vector<long double> bb; //for dynamic density

vector<vector<long double>> abcx;

void *process_frame(void *t1)
{
	struct have_fun *abc;
	abc = (struct have_fun *)t1;

	Mat frame = abc->fr;
	Mat H = abc->ho;

	Mat gray_frame;
	cvtColor(frame, gray_frame, COLOR_BGR2GRAY); //grayscale current frame
	//show the frame in the created window
	//imshow(window_name, frame);
	warpPerspective(gray_frame, gray_frame, H, gray_frame.size()); //angle corrected current frame
	gray_frame = gray_frame(region);							   //cropped current frame
	Mat img3;
	absdiff(gray_frame, bg_changed, img3); //after removing background
	Mat img3_binary;
	threshold(img3, img3_binary, 25, 255, THRESH_BINARY);

	long double cc = sum(img3_binary)[0];
	//   cout<<abc->id<<endl;
	abcx[(abc->id) % NUM_THREADS].push_back(cc);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	NUM_THREADS = stoi(argv[1]);
	abcx = vector<vector<long double>>(NUM_THREADS);
	auto start = high_resolution_clock::now();
	pthread_t threads[NUM_THREADS];
	have_fun xyz[NUM_THREADS];
	VideoCapture cap("trafficvideo.mp4"); //video filename is given as argument
	int flag = 0;
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		//cin.get();
		return 0;
	}

	Mat bg = imread("background.jpg"); //background image

	//grayscale background image
	cvtColor(bg, bg_changed, COLOR_BGR2GRAY);

	String window_name = "Traffic_Video"; //window names
	//String window_name2 = "Binary video";
	//namedWindow(window_name, WINDOW_NORMAL);
	//namedWindow(window_name2, WINDOW_NORMAL);

	//coordinates for finding the homography matrix for angle correction of each frame
	int x0 = 472, y0 = 52, x1 = 472, y1 = 830, x2 = 800, y2 = 830, x3 = 800, y3 = 52;
	vector<Point2f> a = {Point2f(x0, y0), Point2f(x1, y1), Point2f(x2, y2), Point2f(x3, y3)};
	vector<Point2f> b = {Point2f(984, 204), Point2f(264, 1068), Point2f(1542, 1068), Point2f(1266, 222)};

	Mat H = findHomography(b, a);								   //homography matrix
	warpPerspective(bg_changed, bg_changed, H, bg_changed.size()); //angle corrected

	//region shows the section of window which is to be cropped
	region.x = a[0].x;
	region.y = a[0].y;
	region.width = a[2].x - a[0].x;
	region.height = a[2].y - a[0].y;

	bg_changed = bg_changed(region); //cropped background

	int count = 0; //counter used to set fps processed
	void *status;
	int rc;
	while (true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame);
		if (bSuccess == false)
		{
			// cout << "Found the end of the video" << endl;
			break;
		}

		int thread_no = count % NUM_THREADS;

		if (count < NUM_THREADS)
		{
			xyz[thread_no].fr = frame;
			xyz[thread_no].id = count;
			xyz[thread_no].ho = H;
			int rc = pthread_create(&threads[thread_no], NULL, process_frame, (void *)&xyz[thread_no]);
		}
		else
		{
			rc = pthread_join(threads[thread_no], &status);
			if (rc)
			{
				cout << "Error:unable to join," << rc << endl;
				exit(-1);
			}
			//cout <<count<<endl ;
			//cout << "  exiting with status :" << status << endl;
			xyz[thread_no].fr = frame;
			xyz[thread_no].id = count;
			xyz[thread_no].ho = H;
			int rc = pthread_create(&threads[thread_no], NULL, process_frame, (void *)&xyz[thread_no]);
		}
		count++;
	}
	for (int i = 0; i < NUM_THREADS; i++)
	{
		rc = pthread_join(threads[i], &status);
	}

	long frx = 0;
	for (int i = 0; i < abcx.size(); i++)
		frx += abcx[i].size();
	aa.resize(frx + 10);
	for (int i = 0; i < abcx.size(); i++)
	{
		for (int j = 0; j < abcx[i].size(); j++)
		{
			aa[j * abcx.size() + i] = abcx[i][j];
		}
	}

	long double tot = (328) * (778) * 3 * 255;
	ofstream answer;
	answer.open("out_thread.txt");
	//below part stores output in out.txt
	answer << "time_sec\tqueue_density\n";
	for (int i = 0; i < aa.size(); i++)
		answer << (long double)(NUM_THREADS * i + 1) / 15 << "\t" << aa[i] / tot << "\n";
	answer.close();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken with "<<NUM_THREADS<<" threads: " << duration.count() / 1000000 << "s\n";

	return 0;
}