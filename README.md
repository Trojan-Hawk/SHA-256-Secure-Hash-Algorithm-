# SHA256 - Secure Hash Algorithm
##### Author:   Timothy Cassidy            
##### ID:   G00333333
## What is a secure hash algorithm?
The Secure Hash Algorithm, or SHA, is a set of algorithms developed by the National Institutes of Standards and Technology (NIST) and other
government and private parties. These secure encryption or "file check" functions have arisen to meet some of the top cybersecurity 
issues of the 21st century. A hash function is a type of mathematical function which turns data into a fingerprint of that data called a 
hash, the actual hashing is preformed by an algorithm which takes the input data and turns it into an output of a fixed length, which 
represents the fingerprint of the data, or message digest.

*Secure Hash Standard Publication:* https://www.nist.gov/publications/secure-hash-standard

## What is SHA256?
SHA256 is a one-way hashing function, outlined in the NIST standard. A one-way function is a function which cannot be inverted, with regards to SHA256 it is currently not possible to backtrack to the original input from the output(checksum, data fingerprint, message digest) of that particular input, so it is seen as a one-way function.

The SHA256 algorithm takes in a message(input of data), the size of which must be less than 2<sup>64</sup> bits. The message is then processed in 512 bit chunks, until the end of the input is reached. The processing involves implementing the various steps outlined in the SHA Standard. After this a '1' bit is added to the end of the input, followed by zero or more '0' bits until we are left with a chunk with a size of 448 bits. The number of bits in the original input is then appended to the end of the final chunk as a Big-Endian 64 bit int. The output of the algorithm is a *Message Digest*, with a size of 256 bits.

## Project Task
"You must write a program in the C programming language that calculatesthe SHA-256 checksum of an input. The algorithm is specified in the SecureHash Standard document supplied by the (United States) National Instituteof Standards and Technology. The only pre-requisite is that your programperforms the algorithm — you are free to decide what input the algorithmshould be performed on. I suggest you allow the user to input some free textor a filename via the command line."

## Project Functions
- List all the functions
- Explain in detail what each does
- Explain your reasoning for the non-obvious
- List sources where code has been adapted from

## Issues Encountered
### Little endian machine effecting correct output
The SHA Standard assumes that all values are stored in Big-Endian, because we were using a system and compiler which stored data in Little-Endian form conversion from Little to Big Endian needed to be preformed. Because of the secure nature of the SHA standard, any small change in the processing resulted in a massive change to the output message digest. This made it difficult to test, due to not being able to step through issues with output.

## Testing
In the 
- explain testing section in function
- List tested inputs, expected output and actual output

## Future Improvements
- Generate a correct example of how the algorithm should preform
- Find source issue(s) by steping through the program and outputting all variable changes in order to compare to correct example.
