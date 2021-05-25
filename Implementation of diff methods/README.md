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