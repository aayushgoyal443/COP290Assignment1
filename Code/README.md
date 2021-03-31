## Steps to run the code of Part1:

1. Clone this repository using `git clone https://github.com/aayushgoyal443/COP290Assignment1` or download as Zip and extract.
2. To run the code, open the Directory in which **Assignment1.cpp** file is present.
3. Put the images whose perspective or view needs to be changed in the same directory as Assignment1.cpp
4. Build and install OpenCV in your machine if you already haven't. Here are guides to do the same:
    * [For Ubuntu](http://techawarey.com/programming/install-opencv-c-c-in-ubuntu-18-04-lts-step-by-step-guide/) 
    * [For Windows](https://cv-tricks.com/how-to/installation-of-opencv-4-1-0-in-windows-10-from-source/)
    * [For MacOS](https://docs.opencv.org/master/d0/db2/tutorial_macos_install.html)
5. Launch the terminal in that directory.
6. Give the `make allpart1` command. This should produce a binary file named "part1" in the same directory.  
(Make sure the **Makefile** is present in the directory)
7. Now run the command `./part1 <file_name>` . Replace <file_name> with the name of your image file.
8. If your image file is empty an exception will be raised, otherwise a window named "Original Image" will open.
9. Click 4 points on this window to choose the points about which you want to change the Homography.  
(You don't need to worry about the order in which you choose the points, **the code automatically arranges them in the required order**)
10. You should see 2 new windows on the screen namely, "Projected Image" and "Cropped Image". These images also get saved in the same directory.
11. Press Escape key to close the windows.
12. Run `make clean` to delete the part1 file.

    **Note:** while changing the homography of an image, you will need to change the values of destination points in the Assignment1.cpp file. 
    This can be done by changing the values stored in the 'a' vector in line 109.
    
## Steps to run the code of Part2:
1. To run the code, open the Directory in which **assignment1_2.cpp** file is present.
2. Put the videos whose Queue Density and Dynamic Density needs to be found in the same directory as assignment1_2.cpp
3. Launch terminal in that directory.
4. Give the `make allpart2` command. This should produce a binary file named "part2" in the same directory.  
(Make sure the **Makefile** is present in the directory)
5. Now run the command `./part2 <file_name>` . Replace <file_name> with the name of your video file.
6. **You should see the output in a "out.txt" file.** This out.txt has three entries namely Frame number, Queue density, Dynamic density which denote the respective values in this order.
7. Run `make clean` to delete the part2 file.

## Steps to run the code of Part3:
1. Clone this repository and open the **Code** Directory.
2. **Put the "trafficvideo.mp4"** file in the Code folder.
3. Launch terminal in "Code" Directory.
4. Give the `make prepare_queue_exe` command. This will create executable files for all the methods implemented for calculating Queue Density.  
Simillarly give the command `make prepare_dynamic_exe` command. This will create executable files for all the methods implemented for calculating Dynamic Density.
5. Now run the command `./<type>_<method_name>.exe <video_filename> <parameter>` for running for a type of density and particular method in that. Replace:
    * \<type> -> dynamic or queue
    * \<method_name> -> sub_sampling, resolution, spatial, temporal, sparse (onyl in case of dynamic density)
    * \<video_filename> -> Name of the videofile, currently working for "trafficvideo.mp4"
    * \<parameter> -> Parameter value for that particular method.
        * For sub_sampling -> No. of frames dropped (should be whole number and less than 32bits)
        * For resolution -> The factor by which height and wwidth needs to be reduced (Should be Natural number)
        * For spatial -> The Number of threads (Should be Natural number)
        * For temporal -> The Number of threads (Should be Natural number)
        * For Baseline and sparse -> No parameter should be given, leave the parameter part from run command.    
6. The data file are stored in the this manner: \<type>/<method_name>/\<parameter>.txt
8. Run `make clean` to delete all the *.exe files.

    **Note:** while changing the homography of an image, the current values have been hard-coded for "trafficvideo.mp4", you will need to change the values of interested corners and their respective destination points in the *.cpp files if you wish to run the code for some other video file. Interested points need to be changed in the "b" vector and destination points need to be changed in the "a" vector. Values should be put in anti-clockwise order starting from the top-left corner.  
    Simillary current "background.jpg" stores the backgroung for "traficvideo.mp4" only. Incase you want to run the code for some other video file, you have to replace the background.jpg with that image (image name should not alter). This background.jpg will be taken as reference to calculate the values of Queue Density.

### To obtain the values of run-time and data for a method:
1. Run the command `make <type>_<method-name>` where type is either queue or dynamic, and method_name is the name of the method, for eg. make `queue_sub_sampling` will run the program for 6 different cases of frames dropped parameter and store the run-time values in "queue_runtimes.txt" file. 

### To check the whole data:
1. Run the command `make run_<type>` where type is either queue or dynamic. This will run all the methods and there various parameters for required type of density. This is what we used for gathering all the data. 