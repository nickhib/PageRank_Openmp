# PageRank_Openmp

The testing environment is on ecc linux on the o251-01 node where i believe we have 8
cores. With these cores we are able to test with further speed up. The idea is to make sure that
it scales to double the cores to make sure we find the cap of speed up.
I changed the sequential a little from the original to be able to take in file names from the
console . now lets time the sequential version on 4 nodes and 9 edges, we then will take the
average of 5 to make sure we have the average to see a more accurate representation of the
time it takes to complete
![image](https://github.com/user-attachments/assets/6d7ad167-0267-4ce4-b3bc-a17f2027725f)

Now lets check if the answer we have obtained at the end was correct by using a
checker found on google at “http://computerscience.chemeketa.edu/ascholer/cs160/WebApps/
PageRank/”
![image](https://github.com/user-attachments/assets/43c1faac-e0df-425a-9f00-62bf8d0ee783)

 I will now time for threads up till 4 because after 4 it does not split work up evenly since
theirs 4 nodes.
![image](https://github.com/user-attachments/assets/605c119e-4938-415c-b47d-c50977ce79e4)
![image](https://github.com/user-attachments/assets/288d10b4-2baa-4648-a543-f666c47f0754)


