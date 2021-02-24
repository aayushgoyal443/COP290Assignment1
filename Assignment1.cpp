#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;


vector<Point2f> b;



/*callback function used by setMouseCallback*/

void hello_again(int evt, int x, int y, int flgs, void* usrdata){
	// cout<<x<<" "<<y<<endl;
    if  (evt == EVENT_LBUTTONDOWN ){
        b.push_back(Point2f(x,y));
        cout<<b.size()<<endl;
        cout<<x<<" "<<y<<endl;
    }
}

// This function gives the angle between 2 position vectors a and b, from a to b, between -pi to pi
long double angle(Point2f a, Point2f b){
    long double x1,x2,y1,y2,cross,dot,angle;
    x1 = a.x;
    x2 = b.x;
    y1 = a.y;
    y2 = b.y;
    cross = x1*y2-y1*x2;
    dot = x1*x2+y1*y2;
    cross/= sqrt(x1*x1 + y1*y1);
    cross/= sqrt(x2*x2 + y2*y2);
    angle = asin(cross);
    if (dot<0){
        if (angle < 0) angle = -1*(M_PI)- angle;
        else angle = M_PI - angle;
    }
    return angle;
}

/*reorders the points in a vector in anticlockwise sense*/ 
vector <Point2f> reorder(vector <Point2f> v){
    vector<Point2f> ordered;
    float val=v[0].y+v[0].x;
    ordered.push_back(v[0]);
    for (int i=1;i<4;i++){
        if (v[i].y+v[i].x < val){
            ordered.front() = v[i];
            val = v[i].y+v[i].x;
        }
        else if (v[i].y+v[i].x == val && v[i].y< ordered.front().y){
            ordered.front() = v[i];
        }
    }
    float x0= ordered.front().x,y0= ordered.front().y; 
    vector<Point2f> shifted;
    for(int i=0;i<4;i++){
        if (v[i].x == x0 && v[i].y== y0) continue;
        shifted.push_back({v[i].x-x0,v[i].y-y0});
    }
    // cout <<"coordinates of the shifted points\n";
    // for (int i=0;i<shifted.size();i++) cout << shifted[i].x <<" "<< shifted[i].y<<"\t"; 
    // cout <<"\n";
    vector<float> angles;
    float x;
    for (int i=0;i<3;i++){
        angles.push_back(angle(shifted[0],shifted[i]));
    }
    // cout <<"value of angles\n";
    // for (int i=0;i<angles.size();i++) cout << angles[i]<<endl;
    int index[3];
    index[0] = max_element(angles.begin(),angles.end()) - angles.begin();
    index[2] = min_element(angles.begin(),angles.end()) - angles.begin();
    index[1]= 3-(index[0]+index[2]);
    for (int i=0;i<3;i++){
        ordered.push_back(Point2f(shifted[index[i]].x+x0,shifted[index[i]].y+y0));
    } 
    // cout << "final coordinates\n";
    // for (int i=0;i<4;i++) cout << ordered[i].x <<" "<< ordered[i].y<<"\n"; 
    
    return ordered;
}


// argv[1] should contain the name of image file
int main(int argc, char** argv){

    if (argc==1){
        cout << "You need to pass both ./output and image_file name as parameters\n";
        return 0;
    }
    else if (argc>2){
        cout <<"Only 1 image file can be processed at a time.\n";
        return 0;
    }

    string name = argv[1];  // name of the inputt file.

    Mat original_img;
    try{
        original_img=imread(name);
    }
    catch(const exception& e){
        cout <<"File not found, make sure the image is in same directory";
        return -1;
    }
    
    Mat original_img=imread(name);

    Mat img;
    cvtColor(original_img,img,COLOR_BGR2GRAY);  // This converts the original image to grayscale image.

    if(img.empty()){
        cout<<"image not found"<<endl;
        return -1;
    }

    String window_name = "Original Image";           //window names
    String window_name1 = "Projected Image";
    String window_name2 = "Cropped Image";


    namedWindow(window_name, WINDOW_NORMAL);          //creating window
    imshow(window_name,img);

    //waitKey(100);

    setMouseCallback(window_name,hello_again,NULL);         //setting mouse callbacks for detecting mouse actions
    int x0=472, y0=52, x1=472, y1=830, x2=800, y2=830, x3=800, y3=52;        //coordinates of the points where the selected points on the window will be mapped

    vector<Point2f> a={Point2f(x0,y0),Point2f(x1,y1),Point2f(x2,y2),Point2f(x3,y3)};
    a= reorder(a);                                           // reordering the vector of selected points in anticlockwise direction
    cout<<img.size()<<endl;
    Mat img1_warp;
    string angle_corrected = "angle_corrected_"+name;

    while(true){

        // When we enter the while loop, the size of b is zero. After clicking 4 points on the window, size becomees 4 and this condition is evaluated
        if(b.size()==4){
            b = reorder(b);
            Mat H=findHomography(b,a);                  //homography matrix
            warpPerspective(img,img1_warp,H,img.size());              //img1_warp stores the image data after angle correction 

            namedWindow(window_name1, WINDOW_NORMAL);
            imshow(window_name1,img1_warp);                  //showing the image after angle correction

            //waitKey(100);
            imwrite(angle_corrected,img1_warp);              //saving the angle corrected image
            break;
        }
        if(waitKey(10)==27)return 0;    // Esc key to close the window
    }

    //waitKey(0);
    Rect region;                      //region shows the section of window which is to be cropped

    region.x=a[0].x;
    region.y=a[0].y;
    region.width=a[2].x-a[0].x;
    region.height=a[2].y-a[0].y;

    Mat cropped=img1_warp(region);           //image data of cropped image
    namedWindow(window_name2, WINDOW_NORMAL);
    imshow(window_name2,cropped);           //showing the cropped image
    string cropped_name = "cropped_"+name;
    imwrite(cropped_name,cropped);          //saving the cropped image
    
    waitKey(0);

    return 0;

}
