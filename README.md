# PageRank_Openmp

The testing environment is on ecc linux on the o251-01 node where i believe we have 8
cores. With these cores we are able to test with further speed up. The idea is to make sure that
it scales to double the cores to make sure we find the cap of speed up.
I changed the sequential a little from the original to be able to take in file names from the
console . now lets time the sequential version on 4 nodes and 9 edges, we then will take the
average of 5 to make sure we have the average to see a more accurate representation of the
time it takes to complete
![image](https://github.com/user-attachments/assets/6d7ad167-0267-4ce4-b3bc-a17f2027725f)
