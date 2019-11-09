# Motion-Estimation
Motion Estimation- Optical flow(Pyramidal Lucas Kanade) and Descriptor matching(FAST&amp;FREAK,FAST&amp;LUCID)
Implementation in C++
Readme file

This project implements the following methods and compares their working
1. FAST for detecting key points and FREAK as descriptor
2. FAST for detecting key points and LUCID as descriptor
3. FAST for detecting key points and optical flow(Lucas-Kane) as descriptor
These methods are used for motion estimation of vehicles.

Compiling and running instructions:
1.Open the code in eclipse aide 
2.create configurable target for the project( right click on the project and select build targets then create. Then type the target name as project name and select OK.)
3.click on the build targets and double click on the target with project name to build it.
4.open terminal from the release folder of the project.
5. Type ./project_name to execute it.

Results:
time for fast + freak : 4.78
time for fast + lucid : 4.66
time optical flow    : 5.52

This zip folder contains 
Project_FF - contains code and input files for FAST + FREAK
Project_FL - contains code and input files for FAST + LUCID
Project_LKopt - contains code and input files for FAST + optical flow
output - folder consists of output samples of the project
	1. outputFF - output of FAST + FREAK
	2. outputFL - output of FAST + LUCID
	3. outputFF - output of FAST + optical flow
PDF report.



System specification:
Macbook Air - 128GB SSD and 8GB RAM.
Processor - core i5 
OS - 64 bit UBUNTU 18.04 LTS.
s
