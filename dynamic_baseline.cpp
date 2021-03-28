#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <chrono>
using namespace cv;
using namespace std;
using namespace std::chrono;

vector <long double> aa; //for Dynamic density

Mat H;
Rect region;
Mat gray_frame;
Mat prvs_frame;

// Function to find the queue density and fill the "aa" vector
int findDynamic()
{
    Mat flow(prvs_frame.size(), CV_32FC2);
    calcOpticalFlowFarneback(prvs_frame, gray_frame, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
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
    cvtColor(bgr, flow, COLOR_BGR2GRAY);
    Mat flow_binary;
    threshold(flow, flow_binary, 10, 255, THRESH_BINARY);

    return sum(flow_binary)[0];
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

    H = findHomography(b, a); //homography matrix

    warpPerspective(prvs_frame, prvs_frame, H, prvs_frame.size()); //angle corrected

    prvs_frame = prvs_frame(region); //cropped background

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
        // if (count % 5!=0){
        //     count++;
        //     continue;
        // }
        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
        warpPerspective(gray_frame, gray_frame, H, gray_frame.size());
        gray_frame = gray_frame(region);

        int sum = findDynamic();
        aa.push_back(sum);

        prvs_frame = gray_frame;
        count++;
    }

    //////////// FOR STORING OUTPUT /////////

    long double tot = (prvs_frame.rows) * (prvs_frame.cols) * 255;
	ofstream answer;
	answer.open("dynamic/baseline.txt");
	answer << "time_sec\tdynamic_density\n";
	for (int i = 0; i < aa.size(); i++)
		answer << (long double)(i + 1) / 15 << "\t" << aa[i] / tot << "\n";
	answer.close();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken using dense optical flow: " << duration.count() / 1000000 << "s\n";

    return 0;
}