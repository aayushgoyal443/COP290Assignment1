The analysis folder contains the outputs obtained by using different methods along with their runtime and error analysis.
It has 4 subfolders:
1. queue- It contains the outputs (.txt file containing the queue density vs time and .png file repesenting its plot) obtained for queue density using different methods. It further contains 4 subfolders for the four methods implemented. The filename represents the parameter used in that particular method. Along with these subfolders, it also contains baseline output for queue density.
2. dynamic- It contains the outputs (.txt file containing the dynamic density vs time and .png file repesenting its plot) obtained for dynamic density using different methods. It further contains 4 subfolders for the four methods implemented. The filename represents the parameter used in that particular method. Along with these subfolders, it also contains baseline output for dynamic density.
3. queue_analysis- It contains the "runtime vs parameter" and "error vs parameter" analysis corresponding to the outputs in queue subfolder. 
4. dynamic_analysis- It contains the "runtime vs parameter" and "error vs parameter" analysis corresponding to the outputs in dynamic subfolder. 

Apart from these subfolders, analysis folder contains the following files:
- analysisReport.pdf - PDF file containing the runtime analysis and errors for different methods and parameters.
- findUtility.cpp - To find the percent error in case of resolution, temporal and spatial method.
- utility_subSampling.cpp - To find the percent error in case of subSampling method.
- plotscript.p - Script file to plot graphs.

================================================================================================================================================

Running utility and script files
1. findUtility.cpp - compile the file using g++ findUtility.cpp -o findutility and then run the executable using ./findutility <filename1> <filename2>. Here filename1 is the baseline output file and filename2 is the output file using one of the four methods.
2. utility_subSampling.cpp - compile the file using g++ utility_subSampling.cpp -o utility_subSampling and then run the executable using ./utility_subSampling <filename1> <filename2> <fr>. Here filename1 is the baseline output file, filename2 is the output file using one of the four methods and fr is the no. of frames being skipped.
3. plotscript.p - open gnuplot in your terminal. Change the filename in plotscript.p along with the labels required. Give command - load "plotscript.p". You will get the plot.
