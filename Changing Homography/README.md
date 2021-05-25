## Steps to change Homography:

1. Clone this repository and change Directory to "Changing Homography" in terminal.
2. Put the images whose perspective or view needs to be changed in this directory.
3. Give the `make` command. This should produce a binary file named "part1" in the same directory.  
(Make sure the **Makefile** is present in the directory)
7. Now run the command `./homography.exe <file_name>` . Replace <file_name> with the name of your image file.
8. If your image file is empty an exception will be raised, otherwise a window named "Original Image" will open.
9. Click 4 points on this window to choose the points about which you want to change the Homography.  
(You don't need to worry about the order in which you choose the points, **the code automatically arranges them in the required order**)
10. You should see 2 new windows on the screen namely, "Projected Image" and "Cropped Image". These images also get saved in the same directory.
11. Press Escape key to close the windows.
12. Run `make clean` to delete the part1 file.

    **Note:** while changing the homography of an image, you will need to change the values of destination points in the Assignment1.cpp file. 
    This can be done by changing the values stored in the 'a' vector in line 109.
    
