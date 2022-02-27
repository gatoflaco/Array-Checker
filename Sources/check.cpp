/* LA-Checker by Isaac Jung
Last updated 02/22/2022

|===========================================================================================================|

|===========================================================================================================|
*/

#include "parser.h"
#include "array.h"

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

    Parser p;
	int status = p.process_input();
    if (status == -1) exit(1);
    if (status == 1) return conclusion(d, t, false);

    Array array(&p);
    if (!array.is_covering()) return conclusion(d, t, false);
    if (!array.is_locating()) return conclusion(d, t, false);
    if (!array.is_detecting()) return conclusion(d, t, false);
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
    printf("CONCLUSION: The array %s (%d, %d)-locating.\n", (isLA? "is" : "is not"), d, t);
    return 0;
}