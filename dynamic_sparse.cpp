// @TODO: Need to change the filter for threshold in all files

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
Mat frame;
Mat prvs_frame;
Mat toShow, mask;
vector<Scalar> colors;
vector<Point2f> corner_points_prev, corner_points_next;

// Function to find the queue density and fill the "aa" vector
void findSparse()
{
    TermCriteria criteria = TermCriteria((TermCriteria::COUNT)+(TermCriteria::EPS),5,0.04);
    vector<uchar> status;
    vector<float> error;
    calcOpticalFlowPyrLK(prvs_frame, gray_frame, corner_points_prev, corner_points_next,status,error,Size(15,15),2,criteria);

    vector<Point2f> forNextIter;
    int xyz=corner_points_prev.size();
    for(int i=0;i<xyz,i++){
        if(status[i]==1){
            forNextIter.push_back(corner_points_next[i]);
            line(mask, corner_points_prev[i], corner_points_next[i],colors[i], 2);
            circle(frame, corner_points_next[i], 5,colors[i], -1);
        }
    }
    add(gray_frame, mask, toShow);
    
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

    String window_name = "sparseDynamic";  //window names

    namedWindow(window_name, WINDOW_NORMAL);
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
    int count = 1;

    for(int i=0;i<100;i++){
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r,g,b));
    }

    mask = Mat::zeros(prev_frame.size(), prev_frame.type());
    goodFeaturesToTrack(prvs_frame, corner_points_prev, 50, 0.2, 10);

    while (true)
    {
        bool bSuccess = cap.read(frame);
        if (bSuccess == false)
        {
            cout << "Found the end of the video" << endl;
            break;
        }
        // cout << "Frame number: " << count << "\n";
        if (count % 5!=0){
            count++;
            continue;
        }
        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
        warpPerspective(gray_frame, gray_frame, H, gray_frame.size());
        gray_frame = gray_frame(region);

        findSparse();
        //aa.push_back(sum);

        imshow(window_name,toShow);
        prvs_frame = gray_frame;
        count++;
    }

    //////////// FOR STORING OUTPUT /////////

    // long double tot = (prvs_frame.rows) * (prvs_frame.cols) * 255;
	// ofstream answer;
	// answer.open("dynamic_baseline.txt");
	// answer << "time_sec\tdynamic_density\n";
	// for (int i = 0; i < aa.size(); i++)
	// 	answer << (long double)(5*i + 1) / 15 << "\t" << aa[i] / tot << "\n";
	// answer.close();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken: " << duration.count() / 1000000 << "s\n";

    return 0;
}