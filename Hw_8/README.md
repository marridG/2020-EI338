
**For Assignment 8, requirements details are,**

Please write a program in OpenMP, to compute the sum of a vector.  
+ Serial implement function of vector sum and parallel function of vector sum should be included.
+ Use the time of serial implement function as baseline, and test the time acceleration ratio of arrays in
different length under parallel function.
+ Code, result should be shown in this assignment.

**Here, files are given by,**  
+ [test file of OpenMP parallel API usage](./test.cpp)
+ [codes for the assignment](./vector_sum.cpp)

Implementation for the assignment highlights,  
+ Naive Latex output
+ Complete report 


**Execution Results**

 | Array Length | All Correct | Parallel | Sequential | Acceleration |
 | :----------: | :---------: | :------: | :--------: | :----------: |  
 | 1            | 1 |    0.000000 | 0.000000 | nan | 
 | 2            | 1 |    0.000000 | 0.000000 | nan | 
 | 4            | 1 |    0.000000 | 0.000000 | nan | 
 | 8            | 1 |    0.000000 | 0.000000 | nan | 
 | 16           | 1 |    0.000000 | 0.000000 | nan | 
 | 32           | 1 |    0.000000 | 0.000000 | nan | 
 | 64           | 1 |    0.000000 | 0.000000 | nan | 
 | 128          | 1 |    0.000000 | 0.000000 | nan | 
 | 256          | 1 |    0.000000 | 0.000000 | nan | 
 | 512          | 1 |    0.000000 | 0.000000 | nan | 
 | 1024         | 1 |    0.000000 | 0.000000 | nan | 
 | 2048         | 1 |    0.000000 | 0.000000 | nan | 
 | 4096         | 1 |    0.000000 | 0.000000 | nan | 
 | 8192         | 1 |    0.000000 | 0.000000 | nan | 
 | 16384        | 1 |    0.000000 | 0.000000 | nan | 
 | 32768        | 1 |    0.000000 | 0.000000 | nan | 
 | 65536        | 1 |    0.000000 | 0.000000 | nan | 
 | 131072       | 1 |    0.000000 | 0.000000 | nan | 
 | 262144       | 1 |    0.000000 | 0.000000 | nan | 
 | 524288       | 1 |    0.000000 | 0.015000 | 0.000000 | 
 | 1048576      | 1 |    0.000000 | 0.016000 | 0.000000 | 
 | 2097152      | 1 |    0.015000 | 0.016000 | 93.749166 | 
 | 4194304      | 1 |    0.015000 | 0.016000 | 93.750656 | 
 | 8388608      | 1 |    0.032000 | 0.047000 | 68.085244 | 
 | 16777216     | 1 |    0.062000 | 0.097000 | 63.917563 | 
 | 33554432     | 1 |    0.109000 | 0.172000 | 63.372074 | 
 | 67108864     | 1 |    0.235000 | 0.375000 | 62.666639 | 
 | 134217728    | 1 |    0.456000 | 0.687000 | 66.375559 | 
 | 268435456    | 1 |    0.921000 | 1.407000 | 65.458422 | 
 | 536870912    | 1 |    1.797000 | 2.782000 | 64.593823 | 
 | 1073741824   | 1 |    3.595000 | 5.610000 | 64.081993
