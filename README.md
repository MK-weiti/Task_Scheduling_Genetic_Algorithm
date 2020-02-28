# Task Scheduling Genetic Algorithm


###### How to use
Program has hard coded some properties which you can change in main file. Some of the values were precisely selected to maximize chance of getting optimal solution. Other values are depentend on the type of problem you want to run. More information inside main file.

Program can be compiled via Visual Studio for Windows or Makefile for Linux (in src directory).


###### Program`s arguments 
(order does not matter, file name at his first character must not be a number)
- number of processors
- path to file, where file stores for each tasks duration and deadline as non-negative integers.


###### Output
Firstly it prints out all tasks information from file. 
- dt - duration time
- d - deadline

Secondly it prints out which tasks on which processor should be run and in what order.

Finally, it displays the sum of delays, which means how much time has elapsed from task`s deadline.


###### Details
Program was written in C++ based on compiler version 11 or higher.

Program was based on work of https://www.ijeat.org/wp-content/uploads/papers/v3i2/B2337123213.pdf?fbclid=IwAR2-kA-EXHTsgbEnGUKI85XgFWoopB0_zjtnstP8W6R0Npz_LncgKaqwE_A.