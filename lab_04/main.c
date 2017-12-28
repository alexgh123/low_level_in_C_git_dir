// ----------------------------------------------------------------------
// File: main.c
// ----------------------------------------------------------------------
// Description:
//           This file generates a dynamically allocated array of random
//           numbers. It prints the random numbers to the screen, as well
//           as different rolling sums, taking into account integer
//           overflows. The intention of the program is to display how
//           different data types can handle (and not handle) integer
//           overflows.
//
// Created: 2017-10-26 (A.Hardt)
// ----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

#define SUCCESS 0
#define MALLOC_ERROR -3
#define MAX_OBJECTS 100
#define COLUMN_WIDTH 13
#define MAX_RAND 1234567890
#define RED "\x1B[31m"
#define RESET_COLOR "\x1B[0m"
#define HEADER_LINE "-------------"
#define HEADER_FORMAT_1 "%*s %*s %*s %*s\n"
#define HEADER_FORMAT_2 "%*u %*u %*u %*llu\n"
#define COLUMN_FORMAT_1 "%*u"
#define COLUMN_FORMAT_2 "%*s"
#define COLUMN_FORMAT_3 "%*u %*llu\n"

// ---------------------------------------------------------------------
// Function:
//     display
// Inputs:
//     An array of unsigned ints, and the length of that array
// Outputs:
//     Four columns of output, with appropriately labeled headers
//           1. The random number generated/stored in array
//           2. A "safeRollingTotal", a rolling total of every number
//              printed to screen from arrayIt will only increment if
//              there is no integer overflow.
//           3. An "unsafeRollingTotal", it will continue to increment even
//              if there is an overflow.
//           4. A "llRollingTotal" which, because it is a long long data
//              type, should be able to hold the rolling total, and not
//              overflow
//
// Description:
//     Takes in an array, and it's length, prints out the integers in the
//     array, as well as a rolling sum.
//
// ---------------------------------------------------------------------
void display(unsigned int *values, unsigned int num_values)
{
        // set up rolling total variables
        unsigned int safeRollingTotal = 0;
        unsigned int unsafeRollingTotal = 0;
        unsigned long long llRollingTotal = 0; // why not "unsigned long long int"?

        // Print column headers, line 1
        printf(HEADER_FORMAT_1, COLUMN_WIDTH, "", COLUMN_WIDTH, "Unsigned", COLUMN_WIDTH, "unsigned", COLUMN_WIDTH, "unsigned" ); // surpised by capitolization choices here

        // Print column headers, line 2
        printf(HEADER_FORMAT_1, COLUMN_WIDTH, "", COLUMN_WIDTH, "int", COLUMN_WIDTH, "int", COLUMN_WIDTH, "long long" );

        // Print column headers, line 3
        printf(HEADER_FORMAT_1, COLUMN_WIDTH, "", COLUMN_WIDTH, "Subtotal", COLUMN_WIDTH, "Subtotal", COLUMN_WIDTH, "Subtotal" );

        // Print column headers, line 4
        printf(HEADER_FORMAT_1, COLUMN_WIDTH, "Random", COLUMN_WIDTH, "w/ Rollover", COLUMN_WIDTH, "w/o Rollover", COLUMN_WIDTH, "w/o Rollover" );

        // Print column headers, line 5
        printf(HEADER_FORMAT_1, COLUMN_WIDTH, "Number", COLUMN_WIDTH, "Detection", COLUMN_WIDTH, "Detection", COLUMN_WIDTH, "Detection" );

        // Print column headers, line 6
        printf(HEADER_FORMAT_1, COLUMN_WIDTH, HEADER_LINE, COLUMN_WIDTH, HEADER_LINE, COLUMN_WIDTH, HEADER_LINE, COLUMN_WIDTH, HEADER_LINE );

        // Iterate through random numbers in array, print out numbers per logic:
        for(int i = 0; i < num_values; i++){

                // Calculate the rolling total, ignoring integer overflow
                unsafeRollingTotal = unsafeRollingTotal + values[i];

                //calculate rolling total with a long long to handle larger numbers:
                llRollingTotal = llRollingTotal + values[i];

                 // two display options based on whether there is an integer overflow or not:
                if ((values[i] > 0) && (values[i] > (UINT_MAX - safeRollingTotal))) {
                        //integer overflow
                        printf(COLUMN_FORMAT_1, COLUMN_WIDTH, values[i]);
                        printf(RED);
                        printf(COLUMN_FORMAT_2, COLUMN_WIDTH+1, "REJECTED"); // need to plus 1 for alignment issues with string
                        printf(RESET_COLOR);
                        printf(COLUMN_FORMAT_3, COLUMN_WIDTH+1, unsafeRollingTotal, COLUMN_WIDTH, llRollingTotal);
                } else {
                        // no integerger overflow
                        safeRollingTotal = safeRollingTotal + values[i];
                        printf(HEADER_FORMAT_2, COLUMN_WIDTH, values[i], COLUMN_WIDTH, safeRollingTotal, COLUMN_WIDTH, unsafeRollingTotal, COLUMN_WIDTH, llRollingTotal);
                }
        }
} // end display()


// **********************************************************************
// **************************  M  A  I  N  ******************************
// **********************************************************************
int main(void)
{
        int result = SUCCESS;
        unsigned int num;

        // 1. randomly determine length of list
            // get a random number,
        srandom(time(NULL));
        num = (random() % MAX_OBJECTS) + 1;

        // set up a pointer for an array that will hold random values
        unsigned int * randomNumsArray = NULL;

        // 2. allocate memory for list
        // set up size of array of unsigned ints
        errno = 0;
        randomNumsArray = malloc(sizeof(unsigned int)*num);
        if ((errno != 0) || (randomNumsArray == NULL)){
                perror("Malloc failed");
                exit(MALLOC_ERROR);
        }

        // 3. Fill the array/memory with random numbers for the length of array
        for (int i = 0; i < num; i++){
                // generate randoms
                unsigned int rand_num = (random() % MAX_RAND);
                // no "+1" b/c each num is *less than* max rand
                // put them into array
                randomNumsArray[i] = rand_num;
        }

        // display the random numbers in array...in four columns specified
        display(randomNumsArray, num);

        // free random numbers array
        free(randomNumsArray);
        randomNumsArray = NULL;

        return result;
} // main()

// End of main.c