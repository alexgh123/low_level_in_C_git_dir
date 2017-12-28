// ----------------------------------------------------------------------------
//File: days.c
//
//Description: A file that takes user input of a month and day and returns the
//             number of days since January 1
//
//Syntax: To use the program:
//        1. Compile
//        2. Run $> ./days Mon day
//            ie $> ./days Dec 31
//        3. Expected output:
//               The number of days = 365
//
//-----------------------------------------------------------------------------

//include statements
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//define statements
#define JANSUM 0
#define FEBSUM 31
#define MARSUM 59
#define APRSUM 90
#define MAYSUM 120
#define JUNSUM 151
#define JULSUM 181
#define AUGSUM 212
#define SEPSUM 243
#define OCTSUM 273
#define NOVSUM 304
#define DECSUM 334
#define SHORTMONTHLENGTH 28
#define MEDMONTHLENGTH 30
#define LONGMONTHLENGTH 31

// global

// functions
bool isStringInArray(char *inputDay, char *month[], int monthLength)
{
        for(int i = 0; i < monthLength; i++) {
                        if (strcmp(inputDay, month[i]) == 0){
                                return true;
                        }
                }
        return false;
}
//main
int main(int argc, char *argv[])
{
        // local
        char * shortMonth[28] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28"};
        char * medMonth[30] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30"};
        char * longMonth[31] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"};
        int sumOfDays, totalDays, validDay = 0;

        //Error if too many or too few arguments were provided on the command-line.
        if(argc != 3) {
                printf("Please enter 2 arguments in this format:\n");
                printf("\t\t./days Mon day\n");
                printf("For example:\n");
                printf("\t\t$>./days Feb 2\n");
                exit(-1);
        }

        // logic for valid input month, also looks up to see if day is valid, calculates sum of days up to month
        if((strcmp(argv[1], "Jan") == 0) || (strcmp(argv[1], "jan") == 0)) {
                if (isStringInArray(argv[2], longMonth, LONGMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = JANSUM;
                }
        } else if ((strcmp(argv[1], "Feb") == 0) || (strcmp(argv[1], "feb") == 0)) {
                if (isStringInArray(argv[2], shortMonth, SHORTMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = FEBSUM;
                }
        } else if ((strcmp(argv[1], "Mar") == 0) || (strcmp(argv[1], "mar") == 0)) {
                if (isStringInArray(argv[2], longMonth, LONGMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = MARSUM;
                }
        } else if ((strcmp(argv[1], "Apr") == 0) || (strcmp(argv[1], "apr") == 0)) {
                if (isStringInArray(argv[2], medMonth, MEDMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = APRSUM;
                }
        } else if ((strcmp(argv[1], "May") == 0) || (strcmp(argv[1], "may") == 0)) {
                if (isStringInArray(argv[2], longMonth, LONGMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = MAYSUM;
                }
        } else if ((strcmp(argv[1], "Jun") == 0) || (strcmp(argv[1], "jun") == 0)) {
                if (isStringInArray(argv[2], medMonth, MEDMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = JUNSUM;
                }
        } else if ((strcmp(argv[1], "Jul") == 0) || (strcmp(argv[1], "jul") == 0)) {
                if (isStringInArray(argv[2], longMonth, LONGMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = JULSUM;
                }
        } else if ((strcmp(argv[1], "Aug") == 0) || (strcmp(argv[1], "aug") == 0)) {
                if (isStringInArray(argv[2], longMonth, LONGMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = AUGSUM;
                }
        } else if ((strcmp(argv[1], "Sep") == 0) || (strcmp(argv[1], "sep") == 0)) {
                if (isStringInArray(argv[2], medMonth, MEDMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = SEPSUM;
                }
        } else if ((strcmp(argv[1], "Oct") == 0) || (strcmp(argv[1], "oct") == 0)) {
                if (isStringInArray(argv[2], longMonth, LONGMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = OCTSUM;
                }
        } else if ((strcmp(argv[1], "Nov") == 0) || (strcmp(argv[1], "nov") == 0)) {
                if (isStringInArray(argv[2], medMonth, MEDMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = NOVSUM;
                }
        } else if ((strcmp(argv[1], "Dec") == 0) || (strcmp(argv[1], "dec") == 0)) {
                if (isStringInArray(argv[2], longMonth, LONGMONTHLENGTH)) {
                        validDay = 1;
                        sumOfDays = DECSUM;
                }
        } else {
                printf("The month you entered is not valid\n");
                printf("You entered: %s\n", argv[1]);
                exit(-1);
        }
        // if the validDay value wasn't set to one above, we error out.
        if(validDay != 1) {
                printf("The day of month you entered is not valid\n");
                printf("You entered: %s %s\n", argv[1], argv[2]);
                exit(-1);
        }
        // After verifying that  the input is  valid,  the program shall then  calculate the day
        totalDays = sumOfDays+atoi(argv[2]);
        printf("The number of days = %d\n", totalDays);

        // 7. When days does not detect  an  input error and is  able  to  calculate the
        // required  result, it  shall display the answer  and then  exit  with  a value of  0.
        return(0);
}
