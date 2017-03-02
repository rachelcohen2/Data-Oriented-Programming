# Measuring the difference between Array of Structures and Structure of Array forms for a simple fluid simulation.


Test Computer: 
* OS: Debian 32-bit VM 
* CPU: AMD A8-6410

Language: C++
Compiler: GNU C++ Compiler with C++ 11 compiler flag

To Build Project:

AoS Code:
g++ -std=c++11 AoS.cpp -o AoS.out
./AoS.out

SoA Code: 
g++ -std=c++11 SoA.cpp -o SoA.out
./SoA.out

(both files include the same Vec3.hpp)

Timing Data/Particle Numbers: 
Both AoS and SoA code are simulated for one step with 1,000,000 particles with the particle radius and
baseDensity of 0.005 per the homework instructions. 

One step fluid simulation with 1964806 particles
AoS Time: 36.410635 seconds

One step fluid simulation with 1964806 particles
SoA Time: 336.113025 seconds
