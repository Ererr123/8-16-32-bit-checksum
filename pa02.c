/*=============================================================================
| Assignment: pa02 - Calculating an 8, 16, or 32 bit
| checksum on an ASCII input file
|
| Author: Dylan Palmer
| Language: c
|
| To Compile: gcc -o pa02 pa02.c //Caution - expecting input parameters
|
| To Execute: c -> ./pa02 inputFile.txt 8
| where inputFile.txt is an ASCII input file
| and the number 8 could also be 16 or 32
| which are the valid checksum sizes, all
| other values are rejected with an error message
| and program termination
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Spring 2023
| Instructor: McAlpin
| Due Date: 4/23/23
|
+=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//prototypes
char* readPlaintext (char *arr, char *filename, int keysize);
long unsigned eightBitCheckSum(long unsigned checksum, char *plaintext);
long unsigned sixteenBitCheckSum(long unsigned checksum, char*plaintext);
long unsigned thirtytwoBitCheckSum(long unsigned checksum, char *plaintext);

int main(int argc, char *argv[])
{
    //variables
    int bitSize = atoi(argv[2]);
    long unsigned checksum = 0;
    char *plaintext = NULL;
    int placeholder = 0;

    //If bitsize isn't 8,16, or 32 terminate
    if(bitSize != 8 && bitSize != 16 && bitSize != 32)
    {
       fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
        return 0;
    }

    //read in plaintext
    plaintext = readPlaintext(plaintext, argv[1], bitSize);

    if(bitSize == 8)
    {
        checksum = eightBitCheckSum(checksum,plaintext);
    }
    if(bitSize == 16)
    {
        checksum = sixteenBitCheckSum(checksum,plaintext);
    }
    if(bitSize == 32)
        checksum = thirtytwoBitCheckSum(checksum, plaintext);
    //print out the plaintext and checksum
    printf("\n");
    for(int x = 0;x < strlen(plaintext);x++)
    {
        if(placeholder == 80)
        {
            printf("\n");
            placeholder = 0;
        }
        printf("%c",plaintext[x]);
        placeholder++;
    }
    int size = strlen(plaintext);
    printf("\n");
    printf("%2d bit checksum is %8lx for all %4d chars\n", bitSize, checksum, size);

    free(plaintext);

    return 0;
}

/*
Read in information form textfile
*/
char* readPlaintext (char *arr, char *filename, int bitsize)
{
    FILE *fp = fopen(filename, "r");
    char n = ' ';
    int i = 0;
    int size = 0;
    int padding = 0;
    if(fp == NULL)
    {
        printf("Error.Could not read file");
        exit(EXIT_FAILURE);
    }
    while((i = fgetc(fp)) != EOF)
    {
        n = (char) i;
        arr = realloc(arr, size+1);
        arr[size] = n;
        size++;
    }

    padding = (strlen(arr) * 8) % bitsize;
    if(padding != 0)
    {
        for(int i = 0; i <(bitsize - padding)/8;i++)
        {
        arr = realloc(arr, size+1);
        arr[size] = 'X';
        size++; 
        }
    }
    arr = realloc(arr, size+1);
    arr[size] = '\0';
    fclose(fp);
    return arr;
}

/*
Code for 8-bit Checksum
change each value to unsigned char and add them
loop until \0
mask checksum
*/
long unsigned eightBitCheckSum(long unsigned checksum, char *plaintext)
{
   int count = 0;
    for(int i = 0; plaintext[i] != '\0'; i++)
    {
        count = ((unsigned char) plaintext[i]);
        checksum = (checksum + count);
    }
    return checksum & 0xff;
}

/*
Code for 16-bit Checksum
count = change each value to unsigned char
count8 = change each value to unsigned char bit and leftshit by 8
add them up and increment the loop by 2
loop until \0
mask checksum
*/
long unsigned sixteenBitCheckSum(long unsigned checksum, char *plaintext)
{
    int count = 0;
    int count8 = 0;
    for(int i = 0; plaintext[i] != '\0' && plaintext[i+1] != '\0'; i+=2)
    {
        count = ((unsigned char) plaintext[i+1]);
        count8 = ((unsigned char) plaintext[i] <<8);
        checksum += (count8 + count);
    }
    return checksum & 0xffff;
}

/*
Code for 16-bit Checksum
count = change each value to unsigned char
count8 = change each value to unsigned char bit and leftshit by 8
count16 = change each value to unsigned char bit and leftshit by 16
count24 = change each value to unsigned char bit and leftshit by 16
add them up and increment the loop by 4
loop until \0
mask checksum
*/
long unsigned thirtytwoBitCheckSum(long unsigned checksum, char *plaintext)
{
    int count = 0;
    int count8 = 0;
    int count16 = 0;
    int count24 = 0;
    for(int i = 0; plaintext[i] != '\0' && plaintext[i+1] != '\0' && plaintext[i+2] != '\0' && plaintext[i+3] != '\0'; i+=4)
    {   
        count = ((unsigned char) plaintext[i+3]);

        count8 = ((unsigned char) plaintext[i+2] <<8);

        count16 = ((unsigned char) plaintext[i+1] <<16);

        count24 = ((unsigned char) plaintext[i] <<24);
  
        checksum += (count + count8 + count16 + count24);
    }
    return checksum & 0xffffffff;
}

/*=============================================================================
| I Dylan Palmer (dy407034) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+============================================================================*/
