# CIS 3319, Lab 2

## What I learned

Throughout this lab, I gained a much more in-depth understanding of HMAC, SHA1, and the purpose of hashing functions. I also became more familiar with the C programming language, learning how to use the function "calloc()" to allocate new memory and initalize it to zero. I also furthered my understanding of bit manipulation tactics in C, despite most of my program using a less efficient method for bit manipulation through arrays of integers.

## Steps of my work
For this lab assignment, the tools I used were the C programming language, GCC compiler, and GNU Make. I used a few C libraries, including the `<stdio.h>`, `<stdlib.h>`, `<math.h>`, and `<stdint.h>` for things like print functions, memory allocation functions, the power function, and 32 bit integer datatypes.

I followed the [SHA1](https://en.wikipedia.org/wiki/SHA-1) and [HMAC](https://en.wikipedia.org/wiki/HMAC) specifications found on Wikipedia, which are based on RFC 3714 and RFC 2104 respectively.

The major problem that I encountered throughout the lab was bit manipulation. Bit manipulation in general is pretty difficult, and in a language like C you can run into lots of issues with data type sizes not matching your specifications. The final hash of the SHA1 algorithm required a 160-bit value but I've never used anything that large in C. When I ran into problems like these, I opted for a much less efficient option of storing binary numbers in integer arrays using integer values `1` and `0`. This is super inefficient and should never be done in the industry, but its just a proof of concept showing I understand the concept of SHA1 and HMAC. Despite this being super inefficient, this allowed me to operate on numbers of any size, whether it be 160-bits or 32-bits all the same. I had to write my own functions to perform bitwise operations on these binary integer arrays too.