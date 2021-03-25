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

int NUM_THREADS = 0;

// function to give the fraction of road occupied

void get_frac(Mat frame, int flag)
{

    long double cc = 0;
    for (int i = 0; i < frame.rows; i++)
    {
        for (int j = 0; j < frame.cols; j++)
        {
            Vec3b bgrPixel = frame.at<Vec3b>(i, j);
            // pixel values
            cc += bgrPixel.val[0];
            cc += bgrPixel.val[1];
            cc += bgrPixel.val[2];
        }
    }

    if (flag == 0)
        aa.push_back(cc); //for queue density
}

int main(int argc, char *argv[])
{
    //for helping in case of invalid command
    //   if (argc==1){
    //       cout << "You need to pass both ./part2 and <video_file> name as parameters\n";
    //       return 0;
    //   }
    //   else if (argc>2){
    //       cout <<"Only 1 video file can be processed at a time.\n";
    //       return 0;
    //   }

    NUM_THREADS = 4;

    auto start = high_resolution_clock::now();

    VideoCapture cap("trafficvideo.mp4"); //video filename is given as argument

    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video file" << endl;
        cin.get();
        return -1;
    }

    Mat bg = imread("background.jpg"); //background image

    Mat bg_changed; //grayscale background image
    cvtColor(bg, bg_changed, COLOR_BGR2GRAY);

    String window_name = "Traffic_Video"; //window names

    //String window_name2 = "Binary video";

    namedWindow(window_name, WINDOW_NORMAL);
    //namedWindow(window_name2, WINDOW_NORMAL);

    //coordinates for finding the homography matrix for angle correction of each frame
    int x0 = 472, y0 = 52, x1 = 472, y1 = 830, x2 = 800, y2 = 830, x3 = 800, y3 = 52;
    vector<Point2f> a = {Point2f(x0, y0), Point2f(x1, y1), Point2f(x2, y2), Point2f(x3, y3)};
    vector<Point2f> b = {Point2f(984, 204), Point2f(264, 1068), Point2f(1542, 1068), Point2f(1266, 222)};

    Mat H = findHomography(b, a);                                  //homography matrix
    warpPerspective(bg_changed, bg_changed, H, bg_changed.size()); //angle corrected

    Rect region; //region shows the section of window which is to be cropped
    region.x = a[0].x;
    region.y = a[0].y;
    region.width = a[2].x - a[0].x;
    region.height = a[2].y - a[0].y;

    bg_changed = bg_changed(region); //cropped background

    int count = 0; //counter used to set fps processed

    while (true)
    {
        Mat frame;
        bool bSuccess = cap.read(frame);
        if (bSuccess == false)
        {
            cout << "Found the end of the video" << endl;
            break;
        }

        count++;

        // if (count % 5 != 0)
        //     continue; //setting fps processed using counter

        Mat gray_frame;
        cvtColor(frame, gray_frame, COLOR_BGR2GRAY); //grayscale current frame
        //show the frame in the created window

        imshow(window_name, frame);
        warpPerspective(gray_frame, gray_frame, H, gray_frame.size()); //angle corrected current frame
        gray_frame = gray_frame(region);                               //cropped current frame

        Mat img3;
        absdiff(gray_frame, bg_changed, img3); //after removing background

        Mat img3_binary;
        threshold(img3, img3_binary, 25, 255, THRESH_BINARY); //binary frame after removing bg

        // Uncomment for finding queue density
        // imshow(window_name2, img3_binary);
        get_frac(img3_binary, 0); //calculating density
        // Below is the part for calculating Dynamic density

        // if (waitKey(10) == 27)
        // {
        //     cout << "Esc key is pressed by user. Stopping the video" << endl;
        //     break;
        // }
    }

    //////////// FOR STORING OUTPUT /////////

    long double tot = (bg_changed.rows) * (bg_changed.cols) * 3 * 255;
    ofstream answer;
    answer.open("out.txt");
    answer << "time_sec,queue_density\n";
    for (int i = 0; i < aa.size(); i++)
        answer << (long double)(i + 1) / 15 << "," << aa[i] / tot << "\n";
    answer.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken to run the program: " << duration.count() / 1000000 << endl;

    return 0;
}