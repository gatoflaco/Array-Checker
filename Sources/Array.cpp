/* LA-Checker by Isaac Jung
Last updated 02/09/2022

|===========================================================================================================|
|   This file containes the meat of this project's logic. The constructor for the Array class has a pointer |
| to an InputInfo object which should have already called its process_info() method. Using that, the Array  |
| object under construction is able to organize the data structures that support the analysis of the given  |
| locating array. The has_strength_2() and has_locating_property() methods can then be called right away.   |
|===========================================================================================================| 
*/

#include "Array.h"
#include <iostream>

/* CONSTRUCTOR - initializes the object
*/
Array::Array(InputInfo *in)
{
    num_tests = in->num_rows;
    num_factors = in->num_cols;
    factors = new Factor[num_factors];

    Interaction *pair;
    int idx;
    for (int i = 0; i < num_factors - 1; i++) { // for every factor (column) but the last
        factors[i].id = i;
        factors[i].level = in->levels.at(i);
        factors[i].interactions_size = num_factors-i-1;
        factors[i].interactions = new Interaction*[num_factors-i-1];
        idx = 0;
        for (int j = i + 1; j < num_factors; j++) { // for every following factor (all columns to the right)
            pair = new Interaction[in->levels.at(i)*in->levels.at(j)];
            for (int this_level = 0; this_level < in->levels.at(i); this_level++) {  // for all possible values of this factor
                for (int other_level = 0; other_level < in->levels.at(j); other_level++) {   // for all possible values of the other factor
                    pair[this_level*other_level+other_level].other = &factors[j];  // give a pointer to that factor
                    pair[this_level*other_level+other_level].this_val = this_level;
                    pair[this_level*other_level+other_level].other_val = other_level;
                    for (int row = 0; row < in->num_rows; row++) {   // go find rows where this interaction occurs
                        if (in->array.at(row)[i] == this_level && in->array.at(row)[j] == other_level) {
                            pair[this_level*other_level+other_level].rows.insert(row);
                        }
                    }
                }
            }
            factors[i].interactions[idx++] = pair;
        }
    }
    // the final column is a fencepost case; it differs from the rest in not needing to track interactions
    factors[num_factors-1].id = num_factors-1;
    factors[num_factors-1].level = in->levels.at(num_factors-1);
    factors[num_factors-1].interactions_size = 0;
    factors[num_factors-1].interactions = nullptr;//*/
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
                    if (factors[i].interactions[idx][this_level*other_level+other_level].rows.size() == 0) {    // if the interaction was not present
                        printf("\t-- INTERACTION NOT FOUND --\n");
                        printf("\tfactor:\t%2d,\tvalue:\t%2d\n", i, this_level);
                        printf("\tfactor:\t%2d,\tvalue:\t%2d\n\n", j, other_level);
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
    return true;
    /*
    for each row r
        for each interaction i1 in r
            for each interaction i2 in r
                next if i1==12
                i1.rows
                i2.rows
                intersection of i1.rows and i2.rows must not be equal to the union??
                (basically there must be at least one row not shared between them)
    */
}

/* DECONSTRUCTOR - frees memory
*/
Array::~Array()
{
    delete[] factors;
}