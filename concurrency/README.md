
#System Details:
* OS: Debian Linux 32-Bit VM
* CPU: AMD A8-6410
* Language: C++11
* Compiler: GNU G++ Compiler with C++11 compiler flag
* Cache Size: 2048 KB
* Num CPUs: 1 (on VM) 4 (on Windows Machine)
* Threading Library Used: POSIX Thread (pthread) API for C++

To compile:
g++ -std=c++11 -pthread -Ofast assn5.cpp -o assn5
./assn5

Link to Google Spreadsheet:
https://docs.google.com/spreadsheets/d/1DXhYve9fPmf30Zi5JcKRpW8HkPX4mCcfNbJd5rWDaNc/edit

setAffinity function is adapted from the example at the link below: 
http://stackoverflow.com/questions/24645880/set-cpu-affinity-when-create-a-thread
