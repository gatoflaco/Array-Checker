/* LA-Checker by Isaac Jung
Last updated 03/13/2022

|===========================================================================================================|
|   This file containes the meat of this project's logic. The constructor for the Array class has a pointer |
| to an InputInfo object which should have already called its process_info() method. Using that, the Array  |
| object under construction is able to organize the data structures that support the analysis of the given  |
| locating array. The is_covering() and is_locating() methods can then be called at will.      |
| The is_covering() method simply ensures that all possible 2-way interactions are present in the array. |
| The is_locating() method checks every row, every pair of interactions, and ensures that the set |
| of rows in which one interaction occurs is not a subset of the set of rows in which the other occurs. To  |
| better understand the purpose of these methods and their logic, refer to the README.                      |
|===========================================================================================================| 
*/

#include "array.h"
#include <iostream>
#include <algorithm>

// method forward declarations
static void print_failure(int f1, int v1, int f2, int v2);
static void print_failure(int i1f1, int i1v1, int i1f2, int i1v2, std::set<int> *rows,
    int i2f1, int i2v1, int i2f2, int i2v2);

/* CONSTRUCTOR - initializes the object
 * - overloaded: this is the default with no parameters, and should not be used
*/
Array::Array()
{
    d = -1; t = -1; delta = -1; true_delta = -1;
    num_tests = -1; num_factors = -1;
    factors = nullptr;
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this version can set its fields based on its parameters
*/
Array::Array(Parser *in, int d_in, int t_in, int delta_in)
{
    d = d_in; t = t_in; delta = delta_in;
    true_delta = INT32_MAX;  // use a ridiculously high value to represent non-initialized
    num_tests = in->num_rows;
    num_factors = in->num_cols;
    if(d <= 0 && d > num_tests) {
        printf("NOTE: bad value for d, continuing with d = 1\n");
        d = 1;
    }
    if(t <= 0 && t > num_factors) {
        printf("NOTE: bad value for t, continuing with t = 2\n");
        t = 2;
    }
    if(delta <= 0) {
        printf("NOTE: bad value for δ, continuing with δ = 1\n");
        delta = 1;
    }
    //factors = new Factor[num_factors];
    /*
    for (int row = 0; row < num_tests; row++) {
        rows.push_back(Row());
    }

    Interaction *pairs;
    Interaction *interaction;
    int idx;    // used to properly iterate over the array of interactions for a given element of factors[]
    for (int i = 0; i < num_factors - 1; i++) { // for every factor (column) but the last
        factors[i].id = i;
        factors[i].level = in->levels.at(i);
        factors[i].interactions_size = num_factors-i-1;
        factors[i].interactions = new Interaction*[num_factors-i-1];
        idx = 0;
        for (int j = i + 1; j < num_factors; j++) { // for every following factor (all columns to the right)
            pairs = new Interaction[in->levels.at(i)*in->levels.at(j)];
            for (int this_level = 0; this_level < in->levels.at(i); this_level++) {  // for all possible values of this factor
                for (int other_level = 0; other_level < in->levels.at(j); other_level++) {   // for all possible values of the other factor
                    interaction = &pairs[this_level*in->levels.at(j)+other_level];
                    interaction->this_factor = &factors[i];
                    interaction->other_factor = &factors[j];
                    interaction->this_val = this_level;
                    interaction->other_val = other_level;
                    interaction->is_locating = true;
                    for (int row = 0; row < num_tests; row++) {   // go find rows where this interaction occurs
                        if (in->array.at(row)[i] == this_level && in->array.at(row)[j] == other_level) {
                            interaction->occurrences.insert(row);  // to track the rows in which this interaction occurs
                            rows.at(row).interactions.push_back(interaction);   // to track the interactions in a given row
                        }
                    }
                }
            }
            factors[i].interactions[idx++] = pairs;
        }
    }
    // the final column is a fencepost case; it differs from the rest in not needing to track interactions
    factors[num_factors-1].id = num_factors-1;
    factors[num_factors-1].level = in->levels.at(num_factors-1);
    factors[num_factors-1].interactions_size = 0;
    factors[num_factors-1].interactions = nullptr;
    */
}

/* SUB METHOD: is_covering - performs the analysis for coverage
 * 
 * parameters:
 * - none
 * 
 * returns:
 * - true if the array has strength 2, false if not
*/
bool Array::is_covering()
{
    if (o != silent) printf("Checking coverage....\n\n");
    bool passed = true;
    /* TODO: fix this
    int idx;
    for (int i = 0; i < num_factors - 1; i++) { // for every factor (column) but the last
        idx = 0;
        for (int j = i + 1; j < num_factors; j++) { // for every following factor (all columns to the right)
            for (int this_level = 0; this_level < factors[i].level; this_level++) {  // for all possible values of this factor
                for (int other_level = 0; other_level < factors[j].level; other_level++) {   // for all possible values of the other factor
                    if (factors[i].interactions[idx][this_level*other_level+other_level].occurrences.size() == 0) {    // if the interaction was not present
                        print_failure(i, this_level, j, other_level);
                        passed = false;
                    }
                }
            }
            idx++;
        }
    }
    */
    if (o != silent) printf("COVERAGE CHECK: %s\n\n", passed ? "PASSED" : "FAILED");
    return passed;
}

/* SUB METHOD: is_locating - performs the analysis for location
 * 
 * parameters:
 * - none
 * 
 * returns:
 * - true if the array has location, false if not
*/
bool Array::is_locating()
{
    if (o != silent) printf("Checking location....\n\n");
    bool passed = true;
    /* TODO: fix this
    int num_interactions = num_factors*(num_factors-1)/2;
    std::set<int> *occurrences1, *occurrences2;
    
    for (Row test : rows) { // for every row (test) in the array
        for (int i1 = 0; i1 < num_interactions - 1; i1++) { // for every interaction in the row but the last
            for (int i2 = i1 + 1; i2 < num_interactions; i2++) {    // for every following interaction
                occurrences1 = &test.interactions.at(i1)->occurrences;    // the set of all rows in which interaction 1 occurs
                occurrences2 = &test.interactions.at(i2)->occurrences;    // the set of all rows in which interaction 2 occurs

                // see if i1's rows are a subset of i2's; if so, i1 is wiped out by i2 and cannot be located
                if (*occurrences1 == *occurrences2) {
                    if (!test.interactions.at(i1)->is_locating) continue;   // may want to comment this line
                    print_failure( test.interactions.at(i1)->this_factor->id, test.interactions.at(i1)->this_val,
                        test.interactions.at(i1)->other_factor->id, test.interactions.at(i1)->other_val, occurrences1,
                        test.interactions.at(i2)->this_factor->id, test.interactions.at(i2)->this_val,
                        test.interactions.at(i2)->other_factor->id, test.interactions.at(i2)->other_val);
                    test.interactions.at(i1)->is_locating = false;
                    passed = false;
                }

                // see if i2's rows are a subset of i1's (they could even be subsets of each other; equal)
                /*if (std::includes(occurrences1->begin(), occurrences1->end(), occurrences2->begin(), occurrences2->end())) {
                    if (!test.interactions.at(i2)->is_locating) continue;
                    print_failure(
                        test.interactions.at(i2)->this_factor->id, test.interactions.at(i2)->this_val,
                        test.interactions.at(i2)->other_factor->id, test.interactions.at(i2)->other_val,
                        occurrences2,
                        test.interactions.at(i1)->this_factor->id, test.interactions.at(i1)->this_val,
                        test.interactions.at(i1)->other_factor->id, test.interactions.at(i1)->other_val,
                        occurrences1);
                    test.interactions.at(i2)->is_locating = false;
                    passed = false;
                }//
            }
        }
    }
    */
    if (o != silent) printf("LOCATION CHECK: %s\n\n", passed ? "PASSED" : "FAILED");
    return passed;
}

/* SUB METHOD: is_detecting - performs the analysis for location
 * 
 * parameters:
 * - none
 * 
 * returns:
 * - true if the array has location, false if not
*/
bool Array::is_detecting()
{
    if (o != silent) printf("Checking detection....\n\n");
    bool passed = true;
    if (o != silent) printf("DETECTION CHECK: %s\n\n", passed ? "PASSED" : "FAILED");
    return passed;
}

/* DECONSTRUCTOR - frees memory
*/
Array::~Array()
{
    //delete[] factors;
}



// ==============================   LOCAL HELPER METHODS BELOW THIS POINT   ============================== //

static void print_failure(int f1, int v1, int f2, int v2)
{
    printf("\t-- INTERACTION NOT FOUND --\n");
    printf("\t(f%d, %d) and (f%d, %d)\n\n", f1, v1, f2, v2);
}

static void print_failure(int i1f1, int i1v1, int i1f2, int i1v2, std::set<int> *rows,
    int i2f1, int i2v1, int i2f2, int i2v2)
{
    printf("\t-- INTERACTIONS NOT LOCATING --\n");
    printf("\tInteraction 1: (f%d, %d) and (f%d, %d)\n", i1f1, i1v1, i1f2, i1v2);
    printf("\tInteraction 2: (f%d, %d) and (f%d, %d)\n", i2f1, i2v1, i2f2, i2v2);
    printf("\trows: { ");
    std::string str = "";
    for (int row : *rows) str += std::to_string(row + 1) + ", ";
    printf("%s }\n\n", str.substr(0, str.size() - 2).c_str());
}