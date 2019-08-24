# SHA256 (Secure Hash Algorithm 256)

The SHA256 hashing algorithm is used to take input data (files) and turns that data into an output of a fixed length. This algorithm was created by me in C as part of my studies for Theory of Algorithms in GMIT. 

## Getting Started

Before attempting to run this program you must first ensure that you have GCC installed onto your computer (link can we found here: https://www.guru99.com/c-gcc-install.html). With GCC installed you next just have to follow these simple steps in order to get the program to run. 
```
1.) Clone this repo or download it
```
```
2.) Open up a cmd window in the projects root directory
```
```
3.) With GCC installed type into the cmd window gcc -o sha256 sha256.c
```
```
4.) This will compile the program for you and allow it to be ran, next type sha256 [FILENAMEHERE]
```
```
5.) This will hash what ever file which name you entred into the file name here slot and then output the hash of the file.
```


### How It Went

Unfortunately after some tests I discovered that my algorithm was not hashing the files correctly. Contained within the project is a small text document with the input "abc". When hashed with SHA256 the hashed value should be 
```
ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
```
but for some reason the output of my program returns a hash of: 
```
e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
```
After attempting many tests to try and fix this error I simply cannot find the cause. Various different refactoring’s of the bit operations and hash computations have given me various different hashing outputs so my guess would be that the problem lies within there but unfortunately I cannot seem to find it.


### Problems

Apart from the issue that there seems to be some kind of calculation error within my algorithm. there was another huge problem that I attempted to fix which was in regards to endians. During early development it was found that the program was reading files in little endian format. Due to This problem the hash would work, but not work correctly. The hash would return a string of correct length but due to reading little endian the hash would be wrong. The SHA256 documentation asks for the input to be in big endian value so it can be correctly hashed and tested to others to ensure correct hashing. I originally set out to fix this problem by using a tutorial that I found online in regards to the SHA 256 algorithm where macros were used but due to the fact my hashes are still not displaying correctly I cannot tell if the code I attempted to implement will work or not.



## Conclusion

Due to an error I cannot find the algorithm does not work according to plan. If I had a bit more time I would be able to fix it but time is against me currently. I am nearly positive this error exists within either how I have wrote the bit operations or the hash computation but I have double checked all resources and videos provided to me and cannot seem to find where I am going wrong.


## Research. 

* [Dr. Ian Mcloughlins Microsoft Streams](https://web.microsoftstream.com/user/20b32719-41e8-4560-9f7f-c83ba751229c) 
* [SHA256 Documentation](https://www.nist.gov/publications/secure-hash-standard) 
* [Details On Big Endian](https://www.youtube.com/watch?v=FZeLKrQTZtE) 
* [Operators For C](https://www.tutorialspoint.com/cprogramming/c_operators.htm) 
* [Website Used For Testing Hash Values Of Files] (https://emn178.github.io/online-tools/sha256_checksum.html)






