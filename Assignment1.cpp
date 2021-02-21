#include <opencv2/opencv.hpp>

#include <iostream>
#include "opencv2/highgui/highgui.hpp"



using namespace cv;

using namespace std;



vector<Point2f> b;



void hello_again(int event, int x, int y, int flags, void* userdata){
	cout<<x<<" "<<y<<endl;

    if  ( event == EVENT_LBUTTONDOWN ){

        b.push_back(Point2f(x,y));
        cout<<b.size()<<endl;

        cout<<x<<" "<<y<<endl;

    }

}



int main(int argc, char** argv){
    Mat img=imread("empty.jpg");
    if(img.empty()){

        cout<<"image not found"<<endl;

        return -1;

    }
    //cout<<img<<"\n";
    String window_name = "My First Video";


    namedWindow(window_name, WINDOW_NORMAL);

    imshow(window_name,img);
    waitKey(100);
    setMouseCallback(window_name,hello_again,NULL);
    vector<Point2f> a={Point2f(472,52),Point2f(472,830),Point2f(800,830),Point2f(800,52)};
    cout<<img.size()<<endl;

    while(true){
        if(b.size()==4){
            Mat H=findHomography(b,a);
            Mat img1_warp;
            warpPerspective(img,img1_warp,H,img.size());
            imshow(window_name,img1_warp);
            waitKey(100);
            break;
        }
        if(waitKey(10)==27)break;
    }

    waitKey(0);
    return 0;

}