# Synchronization Tools
This project was done as part of course **CS-3523** which was organized by **Dr. Sathya Peri** which aims aims at implementing some Synchronization techniques which are used widely across parallelism.

## About the project

The following projects implements the mutual exclusion algorithms **Compare and Swap(CAS)**, **Test and Set(TAS)**, **Bounded-CAS**. The performances of the mutual exclusion algorithms are compared based on the average time taken by each thread to enter critical section and the worst case time taken by a process to enter critical section vs number of threads.

## Authors

- [@sahishnu1123](https://www.github.com/sahishnu1123)


## Tech Stack

**Command Line:** C++
## Prerequisite

Check if a suitable compiler exists for compiling the C++ code

for g++ compiler:

    g++ --version

for clang compiler:

    clang --version

If there isn't any compiler installed, install it using the documentation for the compiler of C++ based on your OS.


## Install and Usage

### Installing project

Clone from repo:

    git clone https://github.com/sahishnu1123/Syncronization-tools

### Running the Program

After cloning is done redirect control to project folder using:

    cd ./Syncronization-tools

For compiling the CAS file run:

    g++ -std=c++20 -o cas.out CAS.cpp -lpthread

For executing the program:

    ./cas.out

For compiling the TAS file run:

    g++ -std=c++20 -o tas.out TAS.cpp -lpthread

For executing the program:

    ./tas.out

For compiling the Cas-Bounded file run:

    g++ -std=c++20 -o casb.out CAS-Bounded.cpp -lpthread

For executing the program:

    ./casb.out


## Input

Input to the program is read from the file **inp-params.txt**. The input should be given in the format:

N K &lambda;1 &lambda;2 

- **P:** Number of threads used in the program
- **C:** Number of times each each thread enters CS
- **&lambda;1:** is the mean of exponential distributes delay values in critical section in milliseconds.
- **&lambda;2:** is the mean of exponential distributes delay values in remainder section in milliseconds.


## Output

- The logs for Cas.cpp is printed to **output_cas.txt**.
- The logs for Tas.cpp is printed to **output_tas.txt**.
- The logs for Cas-Bounded.cpp is printed to **output_casb.txt**.

## Documentation

The explanation and report for the whole project can be found in **Report.pdf**
