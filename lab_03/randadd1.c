// ----------------------------------------------------------------------
// File: randadd1.c
// ----------------------------------------------------------------------
// Description: This file implements a program that will build a list of
//     random numbers and add them together. The length of the list is
//     also random. It will display the numbers selected and their total.
//
//     Internally, the list of numbers are generated first, and are
//     stored in a linked list. Each object of the list holds
//     a random number. The program then takes a pass through the list and
//     adds up the numbers in the list. The program then takes another
//     pass through the list to print out the numbers in the list as well
//     as the calculated total.
//
// Created: 2016-04-19 (P. Clark)
//
// Modified:
// 2016-10-19 (P. Clark)
//     Changed the program to limit the size of the random numbers,
//     such that overflows won't be a problem (which has not been
//     covered yet). Also changed the max size of the list to a smaller
//     number.
// 2017-10-13 (P. Clark)
//     Attempted to make the code consistent with the lecture matieral,
//     and to make it more readable.
// 2017-10-22 (A. Hardt)
//      Completed calc_total() and print_list() methods. Used Valgrind to
//      fix a memory error associated with free-ing up the linked list.
// ----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define MAX_OBJECTS  19
#define MAX_RANDOM   50

// error codes
#define SUCCESS 0
#define BAD_POINTER -1
#define INVALID_NUMBER -2
#define MALLOC_ERROR -3


typedef struct value value_t;
struct value {
        long int val;
        value_t *next;
};

// ---------------------------------------------------------------------
// Function:
//     print_list
// Inputs:
//     start
//         A pointer to the start of a linked list.
// Outputs:
//     function result:
//         An indicator of success or failure. A success is a zero,
//         while a failure is any other value.
// Description:
//     This function walks through the linked list, printing out each
//     value in the list.
// ---------------------------------------------------------------------
int print_list(value_t *start)
{
        int result = SUCCESS;
        int counter = 1;

        // Walk through linked list and print out each value
        if (start == NULL) {
                printf("Error: unexpected NULL pointer in print_list\n");
                result = BAD_POINTER;
        } else {
                value_t *ptr = start;
                while(ptr != NULL) {
                        printf("Number %2d = %2ld\n", counter, ptr->val);
                        ptr = ptr->next;
                        counter ++;
                }

        }
        return result;
} // print_list()


// ---------------------------------------------------------------------
// Function:
//     calc_total
// Inputs:
//     start
//         A pointer to the start of a linked list.
// Outputs:
//     total:
//         The total of all "val"s in the input linked list.
//     function result:
//         An indicator of success or failure. A success is a zero,
//         while a failure is any other value.
// Description:
//     Given the linked list passed as input, this function will calculate
//     the total of each 'val' within the list.
// ---------------------------------------------------------------------
int calc_total(value_t *start, long int *total)
{
        int result = SUCCESS;

        // walk through linked list, increment total for each value in linked list
        if (start == NULL) {
                printf("Error: unexpected NULL pointer in calc_total\n");
                result = BAD_POINTER;
        } else {
                value_t *ptr = start;
                while(ptr != NULL) {
                        *total = *total + ptr->val;
                        ptr = ptr->next;
                }
        }

        return result;
} // calc_total()


// ---------------------------------------------------------------------
// Function:
//     build_list
// Inputs:
//     num
//         A number indicating how many objects need to be created
//         in a linked list.
//     start
//         A pointer to memory that shall be used as the start of
//         the linked list.
// Outputs:
//     function result:
//         An indicator of success or failure. A success is a zero,
//         while a failure is any other value.
// Description:
//     This function creates a linked list of value_t objects. The
//     number of objects to create is given as an input 'num', which
//     must be between 1 and MAX_OBJECTS (inclusive). In addition,
//     this function initializes the 'val' member of each object to
//     a random integer in the range provided by MAX_RANDOM.
// ---------------------------------------------------------------------
int build_list(const unsigned int num, value_t *start)
{
        int result = SUCCESS;
        unsigned int i;
        value_t *last = NULL;
        value_t *new  = NULL;

        if (start == NULL) {
                printf("Error: unexpected NULL pointer in build_list\n");
                result = BAD_POINTER;
        } else if (num > MAX_OBJECTS) {
                printf("Error: invalid number of objects in build_list\n");
                result = INVALID_NUMBER;
        } else {
                // We have a valid starting pointer.
                // Initialize the initial structure
                start->val = (random() % MAX_RANDOM) + 1;
                start->next = NULL;

                // get additional structures and link them together
                for (i=1, last=start; i < num; ++i) {
                        errno = SUCCESS;
                        new = malloc(sizeof(value_t));
                        if ((new == NULL) || (errno != SUCCESS)) {
                                if (errno == SUCCESS) {
                                        result = MALLOC_ERROR;
                                } else {
                                        perror("malloc error");
                                        result = errno;
                                }
                                perror("malloc error");
                                break;
                        }
                        new->val = (random() % MAX_RANDOM) + 1;
                        new->next = NULL;
                        last->next = new;
                        last = new;
                }
        }

        return result;
} // build_list



// **********************************************************************
// **************************  M  A  I  N  ******************************
// **********************************************************************
int main(void)
{
        int result = SUCCESS;
        long int total = 0;
        unsigned int num;

        srandom(time(NULL));
        num = (rand() % MAX_OBJECTS) + 1;

        value_t *start = NULL;
        value_t *tmp1 = NULL;

        errno = SUCCESS;
        start = malloc(sizeof(value_t));

        printf("\nLength of list = %d\n\n", num);

        if ((errno != SUCCESS) || (start == NULL)) {
                if (errno == SUCCESS) {
                        result = MALLOC_ERROR;
                        printf("malloc error when starting the list\n");
                } else {
                        result = errno;
                        perror("Unable to start the list");
                }
        } else {
                // Build the linked list
                result = build_list(num, start);
                if (result != SUCCESS) {
                        printf("Error: problem building list\n");
                }
        }

        // Calculate the sum of the linked list
        if (result == SUCCESS) {
                result = calc_total(start, &total);
                if (result != SUCCESS) {
                        printf("Error calculating total\n");
                }
        }

        // Print out list
        if (result == SUCCESS) {
                result = print_list(start);
                if (result != SUCCESS) {
                        printf("Error printing the list\n");
                }
        }

        // Print the total
        if (result == SUCCESS) {
                printf("\nTotal = %ld\n", total);

                // Free up the linked list
                while (start != NULL){
                      tmp1 = start;
                      start = start->next;
                      free(tmp1);
                }
                start = NULL;
        }

        return result;
} // main()

// End of randadd1.c