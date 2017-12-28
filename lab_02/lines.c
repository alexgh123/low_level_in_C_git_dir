// ----------------------------------------------------------------------------
//File: lines.c
//
//Description: a program that displays a requested number of horizontal lines
//             at random starting locations of the terminal.
//
//Syntax: To use the program:
//        1. Compile
//        2. Run $> ./lines number length
//            ie $> ./days 5 3
//        3. Expected output:
//               5 lines of length 3 printed randomly on screen
//
//-----------------------------------------------------------------------------

//include statements
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

//define statements
#define ARGNUM 3
#define LINE_MAX 1000
#define NUM_MAX 40
#define BASE_TEN 10

// global
int Max_rows = 0;
int Max_cols = 0;

// functions
char display_line(int r_col, int r_row, int length)
{
        //if line is going to overflow to next line, change it's length
        if(r_col + length >= Max_cols) {
                length = Max_cols - r_col +1;
        }

        // create the line to draw
        char *line_to_draw = malloc(length + 1);
        memset(line_to_draw, '-', length);
        line_to_draw[length] = '\0';

        // move cursor to random coordinates
        printf("\033[%d;%dH", r_row, r_col);

        // print line at location specified above
        printf("%s\n",line_to_draw);
        fflush(stdout);
        sleep(1); // usleep(500)...miliseconds...most satisfying visually
        free(line_to_draw);
        return(0);
}

//main
int main(int argc, char *argv[])
{

        //2. The  command-line  syntax  shall be: lines number length where:
                // number =  the number  of  lines to  display
                // length =  the length  of  each  horizontal  line
        if(argc != ARGNUM) {
                printf("Please enter 2 arguments in this format:\n");
                printf("\t\t./lines number_of_lines length_of_lines\n");
                printf("For example:\n");
                printf("\t\t$>./lines 4 5\n");
                exit(-1);
        }


        // local
        char *number_input = argv[1];
        char *length_input = argv[2];

        // convert input1 to integer and check for errors
        char *end = NULL;
        errno = 0;
        int number = strtol(number_input, &end, BASE_TEN);

        // check input conversion
        if ((errno != 0) || (*end != '\0')){
               printf("Please enter a valid number\n");
               printf("For example:\n");
               printf("\t\t$>./lines 4 5\n");
               exit(-1);
        }

        // convert input2 to integer and check for errors
        char *end1 = NULL;
        errno = 0;
        int length = strtol(length_input, &end1, BASE_TEN);
        if ((errno != 0) || (*end1 != '\0')){
               printf("Please enter a valid length\n");
               printf("For example:\n");
               printf("\t\t$>./lines 4 5\n");
               exit(-1);
        }

        // 2. Determine console size
        struct winsize win;
        ioctl(0, TIOCGWINSZ, &win);
        // get max rows value, immedietly subtract 1 so bottom row isn't used
        Max_rows = win.ws_row-1;
        Max_cols = win.ws_col;

        // seed random number generator with time
        srandom(time(NULL));

        //3. The  number  of  lines can be  any number  between 1 and 1000  (inclusive).
        if((number < 1) || (number > LINE_MAX)) {
                printf("Invalid entry\n");
                printf("The number of lines must be between 1-1000\n");
                printf("You entered: %d\n", number);
                exit(-1);
        }
        //4. The  length  must  be  a number  between 1 and 40  (inclusive).
        if ((length < 0) || (length > NUM_MAX)) {
               printf("Invalid entry\n");
               printf("Please ensure your 'length' argument is between 1 and 40\n");
               printf("You entered: %d\n", length);
               exit(-1);
        }

        // initialize an array:
        int * rowNums = NULL;
        int * colNums = NULL;

        // set memory for coordinate array, check for error
        errno = 0;
        rowNums = malloc(sizeof(int)*number);
        if ((errno != 0) || (rowNums == NULL)){
                perror("Malloc failed");
                exit(-1);
        }

        // set memory for coordinate array, check for error
        errno = 0;
        colNums = malloc(sizeof(int)*number);
        if ((errno != 0) || (colNums == NULL)){
                perror("Malloc failed");
                exit(-1);
        }

        // put random variables in array w/in a loop
        for (int i = 0; i < number; i++){
                // generate randoms
                int r_row = (random() % Max_rows)+1; // the "+1" makes sure i don't get a "0 row"
                int r_col = (random() % Max_cols)+1;
                // put them into array
                rowNums[i] = r_row;
                colNums[i] = r_col;
        }

        //clear screen
        printf("\033[2J");

        // print lines to screen
        for(int i = 0; i < number; i++){
                display_line(colNums[i], rowNums[i], length);
        }

        // once done, move cursor to bottom of page:
        printf("\033[%d;%dH", Max_rows+1, 0);

        free(rowNums);
        free(colNums);
        return(0);
}
