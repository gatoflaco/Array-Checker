/* LA-Checker by Isaac Jung
Last updated 02/19/2022

|===========================================================================================================|
|   This file containes the meat of this project's logic. The constructor for the Array class has a pointer |
| to an InputInfo object which should have already called its process_info() method. Using that, the Array  |
| object under construction is able to organize the data structures that support the analysis of the given  |
| locating array. The has_strength_2() and has_locating_property() methods can then be called at will.      |
| The has_strength_2() method simply ensures that all possible 2-way interactions are present in the array. |
| The has_locating_property() method checks every row, every pair of interactions, and ensures that the set |
| of rows in which one interaction occurs is not a subset of the set of rows in which the other occurs. To  |
| better understand the purpose of these methods and their logic, refer to the README.                      |
|===========================================================================================================| 
*/

#include "Array.h"
#include <iostream>
#include <algorithm>

// method forward declarations
static void print_failure(int f1, int v1, int f2, int v2);
static void print_failure(int i1f1, int i1v1, int i1f2, int i1v2, std::set<int> *i1r,
    int i2f1, int i2v1, int i2f2, int i2v2, std::set<int> *i2r);

/* CONSTRUCTOR - initializes the object
*/
Array::Array(InputInfo *in)
{
    num_tests = in->num_rows;
    num_factors = in->num_cols;
    factors = new Factor[num_factors];
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
}

/* SUB METHOD: has_strength_2 - performs the analysis for coverage
 * 
 * parameters:
 * - none
 * 
 * returns:
 * - true if the array has strength 2, false if not
*/
bool Array::has_strength_2()
{
    printf("CHECKING COVERAGE....\n\n");
    bool passed = true;
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
    printf("COVERAGE CHECK: %s\n\n", passed ? "PASSED" : "FAILED");
    return passed;
}

/* SUB METHOD: has_locating_property - performs the analysis for the locating property
 * 
 * parameters:
 * - none
 * 
 * returns:
 * - true if the array has the locating property, false if not
*/
bool Array::has_locating_property()
{
    printf("CHECKING LOCATING PROPERTY....\n\n");
    bool passed = true;
    int num_interactions = num_factors*(num_factors-1)/2;
    std::set<int> *occurrences1, *occurrences2;
    
    for (Row test : rows) { // for every row (test) in the array
        for (int i1 = 0; i1 < num_interactions - 1; i1++) { // for every interaction in the row but the last
            for (int i2 = i1 + 1; i2 < num_interactions; i2++) {    // for every following interaction
                occurrences1 = &test.interactions.at(i1)->occurrences;    // the set of all rows in which interaction 1 occurs
                occurrences2 = &test.interactions.at(i2)->occurrences;    // the set of all rows in which interaction 2 occurs

                // see if i1's rows are a subset of i2's; if so, i1 is wiped out by i2 and cannot be located
                if (std::includes(occurrences2->begin(), occurrences2->end(), occurrences1->begin(), occurrences1->end())) {
                    if (!test.interactions.at(i1)->is_locating) continue;   // may want to comment this line
                    print_failure(
                        test.interactions.at(i1)->this_factor->id, test.interactions.at(i1)->this_val,
                        test.interactions.at(i1)->other_factor->id, test.interactions.at(i1)->other_val,
                        occurrences1,
                        test.interactions.at(i2)->this_factor->id, test.interactions.at(i2)->this_val,
                        test.interactions.at(i2)->other_factor->id, test.interactions.at(i2)->other_val,
                        occurrences2);
                    test.interactions.at(i1)->is_locating = false;
                    passed = false;
                }

                // see if i2's rows are a subset of i1's (they could even be subsets of each other; equal)
                if (std::includes(occurrences1->begin(), occurrences1->end(), occurrences2->begin(), occurrences2->end())) {
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
                }
            }
        }
    }//*/
    printf("LOCATING CHECK: %s\n\n", passed ? "PASSED" : "FAILED");
    return passed;
}

/* DECONSTRUCTOR - frees memory
*/
Array::~Array()
{
    delete[] factors;
}



// ==============================   LOCAL HELPER METHODS BELOW THIS POINT   ============================== //

static void print_failure(int f1, int v1, int f2, int v2)
{
    printf("\t-- INTERACTION NOT FOUND --\n");
    printf("\t(%d, %d) and (%d, %d)\n\n", f1, v1, f2, v2);
}

static void print_failure(int i1f1, int i1v1, int i1f2, int i1v2, std::set<int> *i1o,
    int i2f1, int i2v1, int i2f2, int i2v2, std::set<int> *i2o)
{
    std::string str;

    printf("\t-- INTERACTION NOT LOCATING --\n");
    printf("\t(%d, %d) and (%d, %d)\n", i1f1, i1v1, i1f2, i1v2);
    printf("\trows: { ");
    str = "";
    for (int row : *i1o) str += std::to_string(row + 1) + ", ";
    printf("%s }\n", str.substr(0, str.size() - 2).c_str());

    printf("\t-> These occurrences form a subset of:\n");
    printf("\t   (%d, %d) and (%d, %d)\n", i2f1, i2v1, i2f2, i2v2);
    printf("\t   rows: { ");
    str = "";
    for (int row : *i2o) str += std::to_string(row + 1) + ", ";
    printf("%s }\n\n", str.substr(0, str.size() - 2).c_str());
}