// ----------------------------------------------------------------------
// File: mycopy.c
// ----------------------------------------------------------------------
// Description:
//              A program that copies an arbitrary file while changing
//              any lower-case characters to upper-case  characters
//              during  the copy. Number of characters changed,
//              punctuation tracked.
//
// Usage:
//              ./mycopy source destination
//
// Created: 2017-11-09 (A.Hardt)
// ----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#define SUCCESS 0
#define FAILURE -1
#define ARGNUM 3
#define SOURCE_FILE_NAME argv[1]
#define TARGET_FILE_NAME argv[2]
#define BUFSIZE 64

// **********************************************************************
// **************************  M  A  I  N  ******************************
// **********************************************************************
int main(int argc, char *argv[])
{

        char current_buf_of_chars[BUFSIZE];

        FILE * readFilePointer = NULL;
        FILE * writeFilePointer = NULL;

        int charsCopied = 0;
        int linesCopied = 0;
        int punctChars = 0;
        int charsChanged = 0;

        size_t readFileContents;
        struct stat metadata;

        //You should get two files on the command-line; otherwise, the program shall print a useful error message and exit with a non-zero value.
        if(argc != ARGNUM) {
                fprintf(stderr, "Please enter 2 arguments in this format:\n");
                fprintf(stderr, "\t\t$>./mycopy source_file target_file\n");
                fprintf(stderr, "For example:\n");
                fprintf(stderr, "\t\t$>./mycopy input.txt output.txt\n");
                exit(FAILURE);
        }

        // If the source and destination file names are the same, then the program shall print a useful error message and exit with a non-zero value. [Note that comparing the two input strings will be good enough for this exercise, but in  general would not normally be good enough. For example, this simple test would fail to detect that they refer to the same file if one string is an absolute path to a file,   while the other string is a relative path to  the same file].
        if(!strcmp(SOURCE_FILE_NAME, TARGET_FILE_NAME)){
                fprintf(stderr, "Please provide different source and target file names:\n");
                fprintf(stderr, "\t\t$>./mycopy name1 name2\n");
                fprintf(stderr, "For example:\n");
                fprintf(stderr, "\t\t$>./mycopy input.txt output.txt\n");
                exit(FAILURE);
        }

        //If the source file does not exist, then the program shall print a useful error message and exit with a non-zero value
        errno = SUCCESS;
        stat(SOURCE_FILE_NAME, &metadata);
        if (errno) {
                fprintf(stderr, "Error: Could not open source file\n");
                exit(FAILURE);
        }

        //If the destination file exists, then the program shall print a useful error message and exit with a non-zero value. Please be careful about the file name you give for the destination because if you do this wrong you may lose the content of that file.
        stat(TARGET_FILE_NAME, &metadata);
        //if there is an error on stat, that is good, we want there to be an error, the file does not exist
        // if there is no error, that means the file exists...and that is bad
        if (!errno) {
                errno = FAILURE;
                fprintf(stderr, "Error: Target File already exists.\n");
                exit(FAILURE);
        }

        // Attempt to read source file, check for errors
        errno = SUCCESS;
        readFilePointer = fopen(SOURCE_FILE_NAME,"r");
        if(errno != SUCCESS){

                fprintf(stderr, "Error opening: %s\n", SOURCE_FILE_NAME);

                // No fclose here because that causes a segmentation fault
                readFilePointer = NULL;

                exit(FAILURE);
        }

        // attempt to open target file location, check for errors
        errno = SUCCESS;
        writeFilePointer = fopen(TARGET_FILE_NAME, "w");
        if(errno != SUCCESS){

                fprintf(stderr, "Error, can not access : %s (likely a permission issue)\n", TARGET_FILE_NAME);

                fclose(readFilePointer);
                readFilePointer = NULL;

                // No fclose here because that causes a segmentation fault
                writeFilePointer = NULL;

                exit(FAILURE);
        }


        //loop through file until end of file is reached:
        while(!feof(readFilePointer)){

                // iterate through file, populate buffer with chars, readFileContents is being updated too
                readFileContents = fread(current_buf_of_chars,1,BUFSIZE,readFilePointer);

                // loop over each char
                for(int i = 0; i < readFileContents; i++){

                        // 7. The  program shall copy the contents of  the source  file into the destination file while also changing any lower-case character to an  upper-case  character   before  it  is  written.
                        if ( islower(current_buf_of_chars[i]) ){
                                current_buf_of_chars[i] = toupper(current_buf_of_chars[i]);
                                charsChanged ++;
                        }

                        if (current_buf_of_chars[i] == '\n'){
                                linesCopied ++;
                        }

                        if ( ispunct(current_buf_of_chars[i]) ){
                                punctChars ++;
                        }

                        charsCopied ++;
                }// end for loop

                //6. If the program cannot  create  the destination file,   then    it  shall   print   a   useful  error message and exit    with    a   non-zero    value.
                // 8. The  program must use the standard C file functions,  not the Unix file functions.
                // 9. The  program must use the buffer approach rather than reading and writing one character at a time.
                errno = SUCCESS;
                fwrite(current_buf_of_chars,1,readFileContents,writeFilePointer);
                if(errno != SUCCESS){
                        fprintf(stderr, "Error writing to: %s\n", TARGET_FILE_NAME);

                        fclose(readFilePointer);
                        readFilePointer = NULL;

                        fclose(writeFilePointer);
                        writeFilePointer = NULL;

                        exit(FAILURE);
                }

        }//end while loop

        // 10. The program shall   also    keep    track   of  the following   information while   performing  its job:
            // a.The number of characters copied.
            // b.The number of characters that were changed during  the copy.
            // c.The number of lines in the file that were copied. For our purposes, we  will simplify the interpretation  of  this requirement to  be  the number of times ‘\n’  is seen in the file.
            // d. The  number  of  punctuation characters  that    were    copied.
        printf("Number of characters copied  = %d\n", charsCopied);
        printf("Number of characters changed = %d\n", charsChanged);
        printf("Number of lines in the file  = %d\n", linesCopied);
        printf("Number of punctuation chars  = %d\n", punctChars);


        //fclose source file:

        errno = SUCCESS;
        fclose(readFilePointer);
        if(errno != SUCCESS){
                fprintf(stderr, "Error attempting to close: %s\n", SOURCE_FILE_NAME);
                exit(FAILURE);
        }
        readFilePointer = NULL;

        // fclose target file

        errno = SUCCESS;
        fclose(writeFilePointer);
        if(errno != SUCCESS){
                fprintf(stderr, "Error attempting to close: %s\n", TARGET_FILE_NAME);
                exit(FAILURE);
        }
        writeFilePointer = NULL;

        // if no failures, return 0
        return(SUCCESS);

} // end main