#Benchmarking/measuring the difference between linear and binary search for sorted arrays of various sizes.



* OS: Windows 8.1 64-bit 
* CPU: AMD A8-6410
* Language: C++
* Compiler: GNU C++ Compiler


To Build Project: 
g++ -ansi -Wall benchmark.cpp -o benchmark.out

Link to Graph:

https://docs.google.com/a/umbc.edu/spreadsheets/d/1PJJG4J7nPmnSL4zvBeo-E-eS4K9s4Xu1fYbby8EV4Vk/edit?usp=sharing



After running my project, I found that overall, binary search tends to be much more time efficient than linear search. First, I initialized C++'s 
std::vector that uses contiguous storage locations for its elements. Vectors have easily variable sizes (and can be easily resized), so I thought that that would be good for this implementation. I initialized the vector with sequential integers from 0 to the maximum size that I declared as a constant. I then added code perform the algorithms of linear and binary search. The linear search looks through the list, one at a time, until the desired value is found (this yields a complexity of O(n)). The binary search finds an equal partition (midpoint) of the array, and if the desired value is equal to the midpoint element, then the position is returned. If not, the search continues on either the lower or upper half of the array. Because of this, the complexity of binary search is O(log n). By increasing the maximum value (MAX constant) that goes into the array, I was able to see the difference in time for the different array values. 

As the size of the array increases incrementally, we can see that binary search tends to be faster on arrays of larger sizes. This is due to the fact that the number of search operations grows more slowly than the list does, because the search values are being halved every time an element is being searched for. Since linear search looks at each element, the time taken to search the list grows at the same rate that the list does. So as the list grows larger, it takes longer. For a small list, linear search is faster, since it looks at the elements of the list in order, and if the list is small, that is a quick process. Binary search works better for large arrays, however, the array must be sorted first. 
