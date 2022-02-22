/* LA-Checker by Isaac Jung
Last updated 02/22/2022

|===========================================================================================================|
| == Usage ==                                                                                               |
|   This program verifies the correctness of a locating array's coverage and locating properties. To use    |
| this program, compile using the makefile and run the executable without any extra arguments. This is done |
| by first running the following command from within the directory of the makefile:                         |
|       make                                                                                                |
| The command will create an executable called "check", unless the executable already exists and is up to   |
| date. Once the executable is created, the program can be used at will by the following command:           |
|       ./check                                                                                             |
| Running the program like this will appear to do nothing. This is because it will begin by trying to read  |
| input from the user.                                                                                      |
|   While it is ok to manually provide input, this is not recommended. Instead, consider redirecting input  |
| from a file. The format of the file, encased in *asterisks for spacing clarity, should be as follows:     |
|       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *       |
|       *v2.0                                                                                       *       |
|       *R C                                                                                        *       |
|       *L_1 L_2 ... L_F                                                                            *       |
|       *0                                                                                          *       |
|       *0                                                                                          *       |
|       *...                                                                                        *       |
|       *0                                                                                          *       |
|       *# # # # # # # #                                                                            *       |
|       *# # # # # # # #                                                                            *       |
|       *...                                                                                        *       |
|       *# # # # # # # #                                                                            *       |
|       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *       |
| Clarification: The first line MUST consist of the string "v2.0". On the second line, give the number of   |
| rows in the LA, R, followed by the number of columns in the LA, C, separated by whitespace. On the third  |
| line, give the number of levels that each factor can take on, respectively, each separated by whitespace. |
| Starting at the fourth line, there must be (C+1) lines consisting of just the number 0. After that, there |
| begins the LA. It should hold that the LA has as many rows as was specified by R and as many columns as   |
| was specified by C. Furthermore, every column should consist of values that are within the corresponding  |
| number of levels given on the third line. All values in a given row of the LA should be separated by      |
| whitespace, and all rows should be on their own lines. Typically the file format is a TSV (tab separated  |
| values), meaning that the whitespace characters are tabs. However, it is fine to use standard whitespace  |
| as well. Check the Sample-Input/ directory for examples of the file format in action. To redirect input   |
| from a file, invoke the executable like this:                                                             |
|       ./check < Sample-Input/input.txt                                                                    |
| This would automatically read lines from "input.txt" with no extra input required. Replace the path with  |
| that of the file to be analyzed.                                                                          |
|   The output of this program is a simple confirmation of the validity of the locating array. If the input |
| format is violated, the program will attempt to inform the user why. If the input format is valid but the |
| array is not covering, the program will list what levels/interactions are missing. Otherwise, the program |
| will output a line to inform the user that the array is covering. Then, if the array is not locating, the |
| program will list what levels/interactions conflict on what row numbers. Otherwise, the program will      |
| output a line to inform the user that the array is locating.                                              |
|                                                                                                           |
| == About Locating Arrays: Purpose of the Program ==                                                       |
|   TO BE WRITTEN                                                                                           |
|                                                                                                           |
| == Implementation Details ==                                                                              |
|   TO BE WRITTEN                                                                                           |
|                                                                                                           |
| == Other Notes ==                                                                                         |
|   TO BE WRITTEN                                                                                           |
|===========================================================================================================|
*/

#include "InputInfo.h"
#include "Array.h"

// method forward declarations
int conclusion(int d, int t, bool isLA);

/* MAIN METHOD: main - called when program is executed
 *
 * parameters:
 * - argc: number of arguments given by caller (including the token used to call)
 * - argv: vector containing the arguments given by the caller; can have exactly 0 or 2 additional arguments
 *     - if 2 additional arguments provided, should be ints specifying d and t
 * 
 * returns:
 * - exit code representing the state of the program (0 means the program finished successfully)
*/
int main(int argc, char *argv[])
{
    int d = 1, t = 2;   // d is the number of t-way interactions in script T', where t is strength
    if (argc > 1) {
        try {
            d = atoi(argv[1]);
        } catch (...) {
            printf("WARNING: BAD ARG: %s\n\t- continuing with d = 1\n", argv[1]);
        }
        if (argc > 2) {
            try {
                t = atoi(argv[2]);
            } catch (...) {
                printf("WARNING: BAD ARG: %s\n\t- continuing with t = 2\n", argv[2]);
            }
        }
        else {
            printf("WARNING: EXPECTED ANOTHER ARG\n\t- continuing with t = 2\n");
        }
    }

    InputInfo in;
	int status = in.process_input();
    if (status == -1) exit(1);
    if (status == 1) return conclusion(d, t, false);
    Array array(&in);
    if (!array.has_strength_2()) return conclusion(d, t, false);
    if (!array.has_locating_property()) return conclusion(d, t, false);
    return conclusion(d, t, true);
}

/* HELPER METHOD: conclusion - prints the final conclusion for the user
 * 
 * parameters:
 * - isLA: whether or not the array is a locating array
 * 
 * returns:
 * - always 0, allowing the caller (main) to return the return from this function, representing success
*/
int conclusion(int d, int t, bool isLA)
{
    printf("CONCLUSION: The array %s (%d, %d)-locating.\n", isLA? "is" : "is not", d, t);
    return 0;
}