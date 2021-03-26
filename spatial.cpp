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

Mat bg_changed; //grayscale background image
Mat H;
Rect region; //region shows the section of window which is to be cropped
vector<Rect> stripes;
Mat gray_frame;

vector<int> blacksum;

// Function to find the queue density and fill the "aa" vector
void *findQueue(void *t1)
{
    int *id1 = (int *)t1;
    int id = *id1;
    Rect strip = stripes[id];
    Mat img3;
    Mat gray_frame1 = gray_frame(strip);
    Mat bg_changed1 = bg_changed(strip);
    absdiff(gray_frame1, bg_changed1, img3);
    Mat img3_binary;
    threshold(img3, img3_binary, 25, 255, THRESH_BINARY);
    for (int i = 0; i < img3_binary.rows; i++)
    {
        for (int j = 0; j < img3_binary.cols; j++)
        {
            Vec3b bgrPixel = img3_binary.at<Vec3b>(i, j);
            // pixel values
            blacksum[id] += bgrPixel.val[0];
            blacksum[id] += bgrPixel.val[1];
            blacksum[id] += bgrPixel.val[2];
        }
    }
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

    Mat bg = imread("background.jpg"); //background image

    cvtColor(bg, bg_changed, COLOR_BGR2GRAY);

    //coordinates for finding the homography matrix for angle correction of each frame
    int x0 = 472, y0 = 52, x1 = 472, y1 = 830, x2 = 800, y2 = 830, x3 = 800, y3 = 52;
    vector<Point2f> a = {Point2f(x0, y0), Point2f(x1, y1), Point2f(x2, y2), Point2f(x3, y3)};
    vector<Point2f> b = {Point2f(984, 204), Point2f(264, 1068), Point2f(1542, 1068), Point2f(1266, 222)};

    H = findHomography(b, a); //homography matrix

    warpPerspective(bg_changed, bg_changed, H, bg_changed.size()); //angle corrected

    region.x = a[0].x;
    region.y = a[0].y;
    region.width = a[2].x - a[0].x;
    region.height = a[2].y - a[0].y;

    bg_changed = bg_changed(region); //cropped background

    /////// Making NUM_THREADS number regions to divide into strips

    // cout << "cols in picture: "<<bg_changed.cols<<"\n";  The value is 328
    int hori = bg_changed.cols / NUM_THREADS;   // if it won't diviide it then core dump error gets thrown
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
            r.width = bg_changed.cols - r.x;
        r.height = bg_changed.rows;
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
    while (true)
    {
        bool bSuccess = cap.read(frame);
        if (bSuccess == false)
        {
            cout << "Found the end of the video" << endl;
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
            for (int i = 0; i < NUM_THREADS; i++)
            {
                // cout << "main() : creating thread, " << i << endl;
                rc = pthread_create(&threads[i], NULL, findQueue, &ind[i]);
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
            for (int i = 0; i < NUM_THREADS; i++)
            {
                // cout << "main() : creating thread, " << i << endl;
                rc = pthread_create(&threads[i], NULL, findQueue, &ind[i]);
                if (rc)
                {
                    cout << "Error:unable to create thread," << rc << endl;
                    exit(-1);
                }
            }
        }

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

    long double tot = (bg_changed.rows) * (bg_changed.cols) * 3 * 255;
    ofstream answer;
    answer.open("spatial_threaded.txt");
    answer << "time_sec,queue_density\n";
    for (int i = 0; i < aa.size(); i++)
        answer << (long double)(i + 1) / 15 << "," << aa[i] / tot << "\n";
    answer.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken to run the program: " << duration.count() / 1000000 << endl;

    pthread_exit(NULL);
    return 0;
}