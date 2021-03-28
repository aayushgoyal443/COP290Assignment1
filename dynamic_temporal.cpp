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

vector<vector<long double>> abcx;

struct parameters
{
    Mat fr; // current frame
    Mat pr; // previous frame
    int id;
};

// Function to find the queue density and fill the "aa" vector
void *process_frame(void *t1)
{
    struct parameters *abc;
    abc = (struct parameters *)t1;
    Mat gray_frame = abc->fr;
    Mat prvs_frame = abc->pr;

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

    long double cc = sum(flow_binary)[0];
    abcx[(abc->id) % NUM_THREADS].push_back(cc);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    NUM_THREADS = stoi(argv[1]);
    abcx = vector<vector<long double>>(NUM_THREADS);
    auto start = high_resolution_clock::now();
    pthread_t threads[NUM_THREADS];
    parameters xyz[NUM_THREADS];
    VideoCapture cap("trafficvideo.mp4"); //video filename is given as argument

    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video file" << endl;
        cin.get();
        return -1;
    }
    Mat prvs;
    cap.read(prvs);

    //coordinates for finding the homography matrix for angle correction of each frame
    int x0 = 472, y0 = 52, x1 = 472, y1 = 830, x2 = 800, y2 = 830, x3 = 800, y3 = 52;
    vector<Point2f> a = {Point2f(x0, y0), Point2f(x1, y1), Point2f(x2, y2), Point2f(x3, y3)};
    vector<Point2f> b = {Point2f(984, 204), Point2f(264, 1068), Point2f(1542, 1068), Point2f(1266, 222)};

    region.x = a[0].x;
    region.y = a[0].y;
    region.width = a[2].x - a[0].x;
    region.height = a[2].y - a[0].y;

    H = findHomography(b, a); //homography matrix

    cvtColor(prvs, prvs, COLOR_BGR2GRAY);
    warpPerspective(prvs, prvs, H, prvs.size());
    prvs = prvs(region);

    int count = 0;
    void *status;
    int rc;
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

        int thread_no = count % NUM_THREADS;

        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, H, frame.size());
        frame = frame(region);

        //////// CREATING THREADS FOR THEM
        if (count < NUM_THREADS)
        {
            xyz[thread_no].fr = frame;
            xyz[thread_no].id = count;
            xyz[thread_no].pr = prvs;
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
            xyz[thread_no].pr = prvs;
            int rc = pthread_create(&threads[thread_no], NULL, process_frame, (void *)&xyz[thread_no]);
        }
        count++;
        prvs = frame;
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_join(threads[i], &status);
    }

    //////////// FOR STORING OUTPUT /////////

    long frx = 0;
	for (int i = 0; i < abcx.size(); i++)
		frx += abcx[i].size();
	aa.resize(frx);
	for (int i = 0; i < abcx.size(); i++)
	{
		for (int j = 0; j < abcx[i].size(); j++)
		{
			aa[j * abcx.size() + i] = abcx[i][j];
		}
	}

    long double tot = (prvs.rows)*(prvs.cols)* 255;
    ofstream answer;
    answer.open("dynamic/temporal/"+to_string(NUM_THREADS)+".txt");
    answer << "time_sec\tDynamic_density\n";
    for (int i = 0; i < aa.size(); i++)
        answer << (long double)(i + 1) / 15 << "\t" << aa[i] / tot << "\n";
    answer.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken with " << NUM_THREADS << " threads: " << duration.count() / 1000000 << "s\n";

    return 0;
}