/* LA-Checker by Isaac Jung
Last updated 02/22/2022

|===========================================================================================================|

|===========================================================================================================|
*/

#include "parser.h"
#include "array.h"
#include <sys/types.h>
#include <unistd.h>


// ================================v=v=v== static global variables ==v=v=v================================ //

static verb_mode vm; // verbose mode
static out_mode om;  // output mode
static prop_mode pm; // property mode

// ================================^=^=^== static global variables ==^=^=^================================ //


// =========================v=v=v== static methods - forward declarations ==v=v=v========================= //

static void verbose_print(int d, int t, int δ);
static int conclusion(int t, bool is_covering);
static int conclusion(int d, int t, bool is_covering, bool is_locating);
static int conclusion(int d, int t, int δ, bool is_covering, bool is_locating, bool is_detecting);

// =========================^=^=^== static methods - forward declarations ==^=^=^========================= //


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
    Parser p(argc, argv);   // create Parser object that immediately processes arguments and flags
    int d = p.d, t = p.t, δ = p.δ;  // set values of d, t, and δ based on those processed by the Parser
    vm = p.v; om = p.o; pm = p.p;   // update flags based on those processed by the Parser
    if (vm == v_on) verbose_print(d, t, δ); // introductory status print when verbose mode enabled
	int status = p.process_input();  // read in and process the array
    if (status == -1) return 1; // exit immediately if there is a basic syntactic or semantic error
    if (status == 1) return 1;  // less trivial issue; another option: return conclusion(t, false);
    
    Array array(&p, d, t, δ);   // create Array object that immediately builds appropriate data structures
    bool is_covering = true, is_locating = true, is_detecting = true;
    if (pm == all || pm == c_only || pm == c_and_l || pm == c_and_d) {  // check coverage if requested
        is_covering = array.is_covering();
        if (!is_covering && vm == v_off) return conclusion(t, false);
    }
    if (pm == all || pm == l_only || pm == c_and_l || pm == l_and_d) {  // check location if requested
        is_locating = array.is_locating();
        if (!is_locating && vm == v_off) return conclusion(d, t, is_covering, false);
    }
    if (pm == all || pm == d_only || pm == c_and_d || pm == l_and_d) {  // check detection if requested
        is_detecting = array.is_detecting();
        if (!is_detecting) return conclusion(d, t, δ, is_covering, is_locating, false);
        conclusion(d, t, δ, is_covering, is_locating, true);
        if (array.Δ > δ) printf("The greatest separation is actually %d.\n", array.Δ);
        return 0;
    }

    return conclusion(d, t, is_covering, is_locating);  // note overloaded detection version cases covered above
}

/* HELPER METHOD: verbose_print - prints the introductory status when verbose mode is enabled
 * 
 * parameters:
 * - d: value of d read from the command line (default should be 1)
 * - t: value of t read from the command line (default should be 2)
 * - δ: value of δ read from the command line (default should be 1)
 * 
 * returns:
 * - void; simply prints to console
*/
static void verbose_print(int d, int t, int δ) {
    int pid = getpid();
    printf("==%d== Verbose mode enabled\n", pid);
    if (om == normal) printf("==%d== Output mode: normal\n", pid);
    else if (om == halfway) printf("==%d== Output mode: halfway\n", pid);
    else if (om == silent) printf("==%d== Output mode: silent\n", pid);
    else printf("==%d== Output mode: UNDEFINED\n", pid);
    if (pm == all) {
        printf("==%d== Checking: coverage, location, detection\n", pid);
        printf("==%d== Using d = %d, t = %d, δ = %d\n", pid, d, t, δ);
    } else if (pm == c_only) {
        printf("==%d== Checking: coverage\n", pid);
        printf("==%d== Using t = %d\n", pid, t);
    } else if (pm == l_only) {
        printf("==%d== Checking: location\n", pid);
        printf("==%d== Using d = %d, t = %d\n", pid, d, t);
    } else if (pm == d_only) {
        printf("==%d== Checking: detection\n", pid);
        printf("==%d== Using d = %d, t = %d, δ = %d\n", pid, d, t, δ);
    } else if (pm == c_and_l) {
        printf("==%d== Checking: coverage, location\n", pid);
        printf("==%d== Using d = %d, t = %d\n", pid, d, t);
    } else if (pm == c_and_d) {
        printf("==%d== Checking: coverage, detection\n", pid);
        printf("==%d== Using d = %d, t = %d, δ = %d\n", pid, d, t, δ);
    } else if (pm == l_and_d) {
        printf("==%d== Checking: location, detection\n", pid);
        printf("==%d== Using d = %d, t = %d, δ = %d\n", pid, d, t, δ);
    } else {
        printf("==%d== No properties to check\nQuitting...");
        exit(1);
    }
}

/* HELPER METHOD: conclusion - prints a final conclusion for the user
 * - overloaded: this version outputs the result of the coverage check
 *
 * parameters:
 * - t: the strength of interactions (how many (factor, values) in the interaction)
 * - is_covering: whether or not the array has t-way coverage
 * 
 * returns:
 * - always 0, allowing the caller to return the return from this function, representing success
*/
static int conclusion(int t, bool is_covering)
{
    if (om != silent) printf("CONCLUSIONS:\n");
    if (pm == all || pm == c_only || pm == c_and_l || pm == c_and_d)
        printf("The array %s %d-covering.\n", (is_covering ? "is" : "is not"), t);
    return 0;
}

/* HELPER METHOD: conclusion - prints a final conclusion for the user
 * - overloaded: this version outputs the result of the location check
 *
 * parameters:
 * - d: the number of t-way interactions in a 𝒯 (set of interactions)
 * - t: the strength of interactions (how many (factor, values) in the interaction)
 * - is_covering: whether or not the array has t-way coverage
 * - is_locating: whether or not the array is a (d, t)-locating array
 * 
 * returns:
 * - always 0, allowing the caller to return the return from this function, representing success
*/
static int conclusion(int d, int t, bool is_covering, bool is_locating)
{
    conclusion(t, is_covering);
    if (pm == all || pm == l_only || pm == c_and_l || pm == l_and_d)
        printf("The array %s (%d, %d)-locating.\n", (is_locating ? "is" : "is not"), d, t);
    return 0;
}

/* HELPER METHOD: conclusion - prints a final conclusion for the user
 * - overloaded: this version outputs the result of the detection check
 *
 * parameters:
 * - d: the number of t-way interactions in a 𝒯 (set of interactions)
 * - t: the strength of interactions (how many (factor, values) in the interaction)
 * - δ: the desired separation (only used for checking detection, check README for info)
 * - is_covering: whether or not the array has t-way coverage
 * - is_locating: whether or not the array is a (d, t)-locating array
 * - is_detecting: whether or not the array is a locating array
 * 
 * returns:
 * - always 0, allowing the caller to return the return from this function, representing success
*/
static int conclusion(int d, int t, int δ, bool is_covering, bool is_locating, bool is_detecting)
{
    conclusion(d, t, is_covering, is_locating);
    if (pm == all || pm == d_only || pm == c_and_d || pm == l_and_d)
        printf("The array %s (%d, %d, %d)-detecting.", (is_detecting ? "is" : "is not"), d, t, δ);
    return 0;
}