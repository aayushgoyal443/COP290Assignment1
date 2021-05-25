## Steps to install OpenCV

Build and install OpenCV in your machine if you already haven't. Here are guides to do the same:
* [For Ubuntu](http://techawarey.com/programming/install-opencv-c-c-in-ubuntu-18-04-lts-step-by-step-guide/) 
* [For Windows](https://cv-tricks.com/how-to/installation-of-opencv-4-1-0-in-windows-10-from-source/)
* [For MacOS](https://docs.opencv.org/master/d0/db2/tutorial_macos_install.html)

## Changing Homography

The cameras are placed such that we don't get the direct upper view of the road. Thus we need to change the view of the image and crop it to get the Rectangular view of the road.  
Add the image and run the code to change the homography. Instructions for use are given in the "Homography" folder's README.md
    
## Queue and Dynamic Density Calculation

To calculate the Queue and Dynamic density of a Road. Place the video in the folder and then run the code. Instructions for use are given in the "Queue and Dynamic Density Calculation" folder's README.md

## Implementation of diff methods

We used different methods to decrease the run-time of video processing. The methods applied are: sub-sampling, scaling (reduction in resolution), spatial threading, temporal threading. The code for these is given in this folder. Further description is given in the Folder's README.md

## Utility vs Runtime Analysis

This contains the precentage error and run-time analysis of finding queue density and dynamic density using various methods like sub-sampling, reduction in resolution, spatial threading and temporal threading. The directory structure is given in that Folder's README.md