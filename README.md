# SHA256 - Secure Hash Algorithm
##### Author:   Timothy Cassidy            
##### ID:   G00333333
## What is a secure hash algorithm?
The Secure Hash Algorithm, or SHA, is a set of algorithms developed by the National Institutes of Standards and Technology (NIST) and 
other government and private parties. These secure encryption or "file check" functions have arisen to meet some of the top 
cybersecurity issues of the 21st century. A hash function is a type of mathematical function which turns data into a fingerprint of that 
data called a hash, the actual hashing is preformed by an algorithm which takes the input data and turns it into an output of a fixed 
length, which represents the fingerprint of the data, or message digest.

*Secure Hash Standard Publication:* https://www.nist.gov/publications/secure-hash-standard

## What is SHA256?
SHA256 is a one-way hashing function, outlined in the NIST standard. A one-way function is a function which cannot be inverted, with 
regards to SHA256 it is currently not possible to backtrack to the original input from the output(checksum, data fingerprint, message 
digest) of that particular input, so it is seen as a one-way function.

The SHA256 algorithm takes in a message(input of data), the size of which must be less than 2<sup>64</sup> bits. The message is then 
processed in 512 bit chunks, until the end of the input is reached. The processing involves implementing the various steps outlined in 
the SHA Standard. After this a '1' bit is added to the end of the input, followed by zero or more '0' bits until we are left with a 
chunk with a size of 448 bits. The number of bits in the original input is then appended to the end of the final chunk as a Big-Endian 
64 bit int. The output of the algorithm is a *Message Digest*, with a size of 256 bits.

## Project Task
"You must write a program in the C programming language that calculatesthe SHA-256 checksum of an input. The algorithm is specified in 
the SecureHash Standard document supplied by the (United States) National Instituteof Standards and Technology. The only pre-requisite 
is that your programperforms the algorithm — you are free to decide what input the algorithmshould be performed on. I suggest you allow 
the user to input some free textor a filename via the command line."

## Project Functions
## Macro Functions
### SHA Standard Supplied Functions
The ROTR and SHR functions can be found on pages 5 - 6 of the SHA Standard.
The CH, Maj, SIG0, SIG1, sig0 and sig1 functions can be found on page 10 of the SHA Standard.
### Endian Flipping Macros
The lilToBigEndian32 macro swaps the Endian ordering format of any 32bit integer passed to it.
This function has been adapted from: http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/

The lilToBigEndian64 macro swaps the Endian ordering format of any 64bit integer passed to it.
This function has been adapted from: http://www.mit.edu/afs.new/sipb/project/merakidev/include/bits/byteswap.h
## Functions
### main function
The main function first checks to see if any command-line arguments have been passed in, if not an error is displayed to the user 
followed by a help message.

Next we declare an instance of the buffer_state struct, this is then passed, along with the command-line args to the initializeState 
function.

Next the file passed in is opened, if this fails an error message and a help message are output. If the file is successfully opened the 
sha256 function is called with the buffer_state struct being passed through.
### sha256 function
The sha256 function follows the steps outlined in section 6.2.2 of the SHA Standard. This method provides the hashing of each 512 bit 
message block.
### nextMesgBlock
The nextMsgBlock function follows the steps outlined in section 5.1.1 of the SHA Standard. This method provides the padding of the final 
message block(s) as needed.
### initializeState function
This function initilises buffer_state struct is initialized, this struct is used to store the message processing state between 
functions. This struct manages the following state variables:
- verbose: This is used to store the verbose option, 1 if verbose and 0 if not.
- endianness: This is used to store the endianness of the system & compiler, 1 if Little-Endian and 0 if Big-Endian.
- M[16]: This is used to store the current 512 bit message chunk as an array of 16 32-bit unsigned integers.
- nobits: This is used to store the number of bits processed from the input.
- status S: This is an enum value which is used to control the padding step.
- file: This is a pointer to the input file.
- filePointer: This is where the current position of the file pointer is stored.
### endianness function
This function is used to detect if the system & compiler is Big-Endian or Little-Endian, 1 if Little-Endian and 0 if not. This function 
has been adapted from: http://cs-fundamentals.com/tech-interview/c/c-program-to-check-little-and-big-endian-architecture.php

## How To Run 
### Compile
In order to compile the C script you must use the following command on your terminal:
*gcc -o (name_of_output_file) (C_file_name)*
Example:
*gcc -o sha256 sha256.c*
### Execution
In order to execute this script you must enter the following command on your terminal after the compile step:
*./(name_of_output_file) (input_file)*
Example:
*./sha256 Test.txt*
### Verbose option
If the user requires detailed information at run-time the '-v' option can be passed as the *first* arg iat the execution step, then 
followed by the file to be processed.
*./(name_of_output_file) -v (input_file)*
Example:
*./sha256 -v Test.txt*

## Issues Encountered
### Little endian machine effecting correct output
The SHA Standard assumes that all values are stored in Big-Endian, because we were using a system and compiler which stored data in 
Little-Endian form conversion from Little to Big Endian needed to be preformed. Because of the secure nature of the SHA standard, any 
small change in the processing resulted in a massive change to the output message digest. This made it difficult to test, due to not 
being able to step through issues with output.

## Testing
In the nextMsgBlock function, there is a commented out testing section. This was the main way I tested the results of my implementation. 
The number of bits is first hardcoded, overriding the value read in from the file, then any number of hex values can be hardcoded to use 
as input.

I used 2 online converters to get an correct value to test against, these were:
- https://defuse.ca/checksums.htm#checksums
- https://md5file.com/calculator 

#### Test Case 1
The string "abc" was hardcoded as above, which yielded the following result:
- 3da407cc c039fbf1 f4cd205b 5bfe8539 4c20648f 2f928e42 b5a3223f 0dfc7a56
This however, was not the desired output, which was:
- ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad

#### Test Case 2
The empty string "" was hardcoded as above, which yielded the following result:
- 479e53f6 19f4afdb 308eb75c a9941148 1aa47d3f 201727fc 3bf5c4e7 06d06d5d
This however, was not the desired output, which was:
- e3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855

## Future Improvements
As you can see from the output of the test cases above, my implementation of the SHA256 algorithm is outputting an incorrect result. In 
order to debug this I will source or develop a correct step-by-step output for each variable change made in the sha256 function. This 
will give me a working example to compare to, which should help me source the issue in my implementation. 

The issue of the conversion of Endianness, as outlined above, could be the source of the incorrect output but further exploration into 
this is needed.
