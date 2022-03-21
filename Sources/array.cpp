/* LA-Checker by Isaac Jung
Last updated 03/20/2022

|===========================================================================================================|
|   This file containes the meat of this project's logic. The constructor for the Array class has a pointer |
| to an InputInfo object which should have already called its process_info() method. Using that, the Array  |
| object under construction is able to organize the data structures that support the analysis of the given  |
| locating array. The is_covering(), is_locating(), and is_detecting() methods can then be called at will.  |
| The is_covering() method simply ensures that all possible t-way interactions are present in the array.    |
| The is_locating() method checks every pair of size-d sets of t-way interactions, and ensures that the set |
| of rows in which one set of interactions occurs is not equal to the set of rows in which the other does.  |
| The is_detecting() method compares all t-way interactions with all size-d sets of interactions to ensure  |
| that if the interaction is not part of the set, it must occur in at least δ rows distinct from those in   |
| in which the set occurs. For a better understanding these methods and their logic, refer to the README.   |
|===========================================================================================================| 
*/

#include "array.h"
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>

// method forward declarations
static void print_failure(Interaction *interaction);
static void print_failure(int i1f1, int i1v1, int i1f2, int i1v2, std::set<int> *rows,
    int i2f1, int i2v1, int i2f2, int i2v2);
static void print_singles(Factor **factors, int num_factors);
static void print_interactions(std::vector<Interaction*> interactions);
static void print_sets(std::set<T*> sets);

/* CONSTRUCTOR - initializes the object
 * - overloaded: this is the default with no parameters, and should not be used
*/
Interaction::Interaction()
{
    // nothing to do
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this version can set its fields based on a premade vector of Single pointers
*/
Interaction::Interaction(std::vector<Single*> *temp)
{
    // fencepost start: let the Interaction be the strength 1 interaction involving just the 0th Single in temp
    singles.push_back(temp->at(0));
    rows = temp->at(0)->rows;

    // fencepost loop: for any t > 1, rows of the Interaction is the intersection of each Single's rows
    for (int i = 1; i < temp->size(); i++) {
      singles.push_back(temp->at(i));
      std::set<int> temp_set;
      std::set_intersection(rows.begin(), rows.end(),
        temp->at(i)->rows.begin(), temp->at(i)->rows.end(), std::inserter(temp_set, temp_set.begin()));
      rows = temp_set;
    }
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this is the default with no parameters, and should not be used
*/
T::T()
{
    // nothing to do
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this version can set its fields based on a premade vector of Interaction pointers
*/
T::T(std::vector<Interaction*> *temp)
{
    // fencepost start: let the Interaction be the strength 1 interaction involving just the 0th Single in s
    interactions.push_back(temp->at(0));
    rows = temp->at(0)->rows;

    // fencepost loop: for any t > 1, rows of the Interaction is the intersection of each Single's rows
    for (int i = 1; i < temp->size(); i++) {
      interactions.push_back(temp->at(i));
      std::set<int> temp_set;
      std::set_union(rows.begin(), rows.end(),
        temp->at(i)->rows.begin(), temp->at(i)->rows.end(), std::inserter(temp_set, temp_set.begin()));
      rows = temp_set;
    }
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this is the default with no parameters, and should not be used
*/
Array::Array()
{
    d = -1; t = -1; delta = -1; true_delta = -1;
    v = v_off; o = normal; p = all;
    num_tests = -1; num_factors = -1;
    factors = nullptr;
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this version can set its fields based on a pointer to a Parser object
*/
Array::Array(Parser *in)
{
    d = in->d; t = in->t; delta = in->delta;
    true_delta = INT32_MAX;  // use a ridiculously high value to represent non-initialized
    v = in->v; o = in->o; p = in->p;
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

    // build all Singles, associated with an array of Factors
    factors = new Factor*[num_factors];
    for (int i = 0; i < num_factors; i++) {
        factors[i] = new Factor(i, in->levels.at(i), new Single*[in->levels.at(i)]);
        for (int j = 0; j < factors[i]->level; j++)
            factors[i]->singles[j] = new Single(i, j);
    }
    for (int row = 0; row < in->num_rows; row++)
        for (int col = 0; col < in->num_cols; col++)
            factors[col]->singles[in->array.at(row)[col]]->rows.insert(row + 1);
    if (v == v_on) print_singles(factors, num_factors);

    // build all Interactions
    std::vector<Single*> temp_singles;
    build_t_way_interactions(0, t, &temp_singles);
    if (v == v_on) print_interactions(interactions);

    // build all Ts
    if (p == c_only) return;    // no need to spend effort building Ts if they won't be used
    std::vector<Interaction*> temp_interactions;
    build_size_d_sets(0, d, &temp_interactions);
    if (v == v_on) print_sets(sets);
}

/* HELPER METHOD: build_t_way_interactions - initializes the interactions vector recursively
 * - the factors array must be initialized before calling this method
 * - top down recursive; auxilary caller should use 0, t, and an empty vector as initial parameters
 *   --> do not use the interactions vector itself as the parameter
 * - this method should not be called more than once
 * 
 * parameters:
 * - start: left side of factors array at which to begin the outer for loop
 * - t: desired strength of interactions
 * - singles_so_far: auxilary vector of pointers used to track the current combination of Singles
 * 
 * returns:
 * - void, but after the method finishes, the Array's interactions vector will be initialized
*/
void Array::build_t_way_interactions(int start, int t, std::vector<Single*> *singles_so_far)
{
    // base case: interaction is completed and ready to store
    if (t == 0) {
        Interaction *new_interaction = new Interaction(singles_so_far);
        interactions.push_back(new_interaction);
        return;
    }

    // recursive case: need to introduce another loop for higher strength
    for (int col = start; col < num_factors - t + 1; col++) {
        for (int level = 0; level < factors[col]->level; level++) {
            singles_so_far->push_back(factors[col]->singles[level]);    // note these are Single *
            build_t_way_interactions(col+1, t-1, singles_so_far);
            singles_so_far->pop_back();
        }
    }
}

/* HELPER METHOD: build_size_d_sets - initializes the sets set recursively (a set of sets of interactions)
 * - the interactions vector must be initialized before calling this method
 * - top down recursive; auxilary caller should use 0, d, and an empty set as initial parameters
 *   --> do not use the sets set itself as the parameter
 * - this method should not be called more than once
 * 
 * parameters:
 * - start: left side of interactions vector at which to begin the for loop
 * - d: desired magnitude of sets
 * - interactions_so_far: auxilary vector of pointers used to track the current combination of Interactions
 * 
 * returns:
 * - void, but after the method finishes, the Array's sets set will be initialized
*/
void Array::build_size_d_sets(int start, int d, std::vector<Interaction*> *interactions_so_far)
{
    // base case: set is completed and ready to store
    if (d == 0) {
        T *new_set = new T(interactions_so_far);
        sets.insert(new_set);
        return;
    }

    // recursive case: need to introduce another loop for higher magnitude
    for (int i = start; i < interactions.size() - d + 1; i++) {
        interactions_so_far->push_back(interactions[i]);    // note these are Interaction *
        build_size_d_sets(i+1, d-1, interactions_so_far);
        interactions_so_far->pop_back();
    }
}

/* SUB METHOD: is_covering - performs the analysis for coverage
 * 
 * parameters:
 * - report: when true, output is based on flags, else there will be no output whatsoever
 * 
 * returns:
 * - true if the array has t-way coverage, false if not
*/
bool Array::is_covering(bool report)
{
    if (report && o != silent) printf("Checking coverage....\n\n");
    bool passed = true;
    for (Interaction *i : interactions) {
        if (i->rows.size() == 0) {  // coverage issue
            if (o != normal) return false;  // if not reporting failures, can reduce work
            print_failure(i);
            passed = false;
        }
    }
    if (report && o != silent) printf("COVERAGE CHECK: %s\n\n", passed ? "PASSED" : "FAILED");
    return passed;
}

/* SUB METHOD: is_locating - performs the analysis for location
 * 
 * parameters:
 * - report: when true, output is based on flags, else there will be no output whatsoever
 * 
 * returns:
 * - true if the array has (d-t)-location, false if not
*/
bool Array::is_locating(bool report)
{
    if (report && o != silent) printf("Checking location....\n\n");
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
    if (report && o != silent) printf("LOCATION CHECK: %s\n\n", passed ? "PASSED" : "FAILED");
    return passed;
}

/* SUB METHOD: is_detecting - performs the analysis for location
 * 
 * parameters:
 * - report: when true, output is based on flags, else there will be no output whatsoever
 * 
 * returns:
 * - true if the array has (d, t, δ)-detection, false if not
*/
bool Array::is_detecting(bool report)
{
    if (report && o != silent) printf("Checking detection....\n\n");
    bool passed = true;
    if (report && o != silent) printf("DETECTION CHECK: %s\n\n", passed ? "PASSED" : "FAILED");
    return passed;
}

/* DECONSTRUCTOR - frees memory
*/
Array::~Array()
{
    for (int i = 0; i < num_factors; i++) delete factors[i];
    delete[] factors;
    for (Interaction *i : interactions) delete i;
    for (T *s : sets) delete s;
}



// ==============================   LOCAL HELPER METHODS BELOW THIS POINT   ============================== //

static void print_failure(Interaction *interaction)
{
    printf("\t-- %d-WAY INTERACTION NOT PRESENT --\n", interaction->singles.size());
    std::string output("\t{");
    for (Single *s : interaction->singles)
        output += "(f" + std::to_string(s->factor) + ", " + std::to_string(s->value) + "), ";
    output = output.substr(0, output.size() - 2) + "}\n";
    std::cout << output << std::endl;
    //printf("\t(f%d, %d) and (f%d, %d)\n\n", f1, v1, f2, v2);
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

static void print_singles(Factor **factors, int num_factors)
{
    int pid = getpid();
    printf("\n==%d== Listing all Singles below:\n\n", pid);
    for (int col = 0; col < num_factors; col++) {
        printf("Factor %d:\n", factors[col]->id);
        for (int level = 0; level < factors[col]->level; level++) {
            printf("\t(f%d, %d): {", factors[col]->singles[level]->factor, factors[col]->singles[level]->value);
            for (int row : factors[col]->singles[level]->rows) printf(" %d", row);
            printf(" }\n");
        }
        printf("\n");
    }
}

static void print_interactions(std::vector<Interaction*> interactions)
{
    int pid = getpid();
    printf("\n==%d== Listing all Interactions below:\n\n", pid);
    int i = 0;
    for (Interaction *interaction : interactions) {
        interaction->id = ++i;
        printf("Interaction %d:\n\tInt: {", i);
        for (Single *s : interaction->singles) printf(" (f%d, %d)", s->factor, s->value);
        printf(" }\n\tRows: {");
        for (int row : interaction->rows) printf(" %d", row);
        printf(" }\n\n");
    }
}

static void print_sets(std::set<T*> sets)
{
    int pid = getpid();
    printf("\n==%d== Listing all Ts below:\n\n", pid);
    int i = 0;
    for (T *s : sets) {
        printf("Set %d:\n\tSet: {", ++i);
        for (Interaction *interaction : s->interactions) printf(" %d", interaction->id);
        printf(" }\n\tRows: {");
        for (int row : s->rows) printf(" %d", row);
        printf(" }\n\n");
    }
}