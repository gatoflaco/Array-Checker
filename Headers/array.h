/* LA-Checker by Isaac Jung
Last updated 03/12/2022

|===========================================================================================================|
|   This header contains classes for managing the array in an automated fashion. The Row class is just a    |
| helper used by the Array class and should not be instantiated by any source file other than the one       |
| associated with this header. The Array class is the interface with which other source files should work.  |
| It contains a constructor that builds all the internal data structures, thereby allowing the instantiator |
| to immediately call other methods which carry out analyses on this data. See check.cpp for an example.    |
| Please note this file is not intended to be highly integrable with other projects working with arrays. It |
| supports the very specific purposes of the LA-Checker project and is not generalized for anything else.   |
|===========================================================================================================| 
*/

#include "parser.h"
#include "factor.h"

// I wasn't sure what to name this, except after the formal parameter used in Dr. Colbourn's definitions
// Note that Colbourn's definitions use a script T: 𝒯 
// This class is used to more easily compare the row coverage of different size-d sets of t-way interactions
class T
{
    public:
        std::vector<Interaction*> interactions; // for easier access to the interactions themselves

        // each interaction in a given T set has its own version of this; the ρ associated with a T is simply
        std::set<int> rows;  // the union of the ρ's for each interaction in that T

         // this helper bool is for remembering that this particular T is not locating; that is, it should be
        bool is_locating;   // set true upon instantion, but false once determined to violate the property
                            // once; it can be checked to cut down on computation time when false
        
        bool is_detecting;  // same as above
};

class Array
{
    public:
        int true_delta; // the true maximum separation is tracked and updated during a detection check

        // checks whether the array is covering; this means that every interaction of strength t occurs in
        bool is_covering();     // the array at least δ times; i.e., in δ unique tests

        // checks whether the array is (d, t, δ)-locating; this means that for every pair of size-d sets of
        bool is_locating();     // t-way interactions, the rows covered by those size-d-sets is not equal

        // checks whether the array is (d, t, δ)-detecting; this extends the locating definition such that
        // for every pair of size-d sets of t-way interactions, the rows covered by one set must include at
        bool is_detecting();    // least δ rows not covered by the other set, and vice versa

        out_mode o; // this dictates how much output should be printed; see parser.h for typedef
        Array();    // default constructor, don't use this
        Array(Parser *in, int d_in = 1, int t_in = 2, int delta_in = 1);    // (1, 2, 1)-properties by default
        ~Array();   // deconstructor

    private:
        int d;  // this is the size of the sets of t-way interactions; the sets are what need to be compared
        int t;  // this is the strength of interest; see README for details on the strength of an interaction
        int delta;  // this is the desired separation of the array; it affects the is_detecting() method
        int num_tests;      // just a field to reference the upper bound on iterating through rows
        int num_factors;    // just a field to reference the upper bound on iterating through columns
        Factor *factors;    // pointer to the start of an array of Factor objects placed next to each other
        std::set<T*> Ts;     // a set of all size-d sets of t-way interactions constructable from the array
        std::set<Interaction*> interactions;    // set of all individual interactions

        // this utility method is called in the constructor to fill out the set of all interactions
        void build_t_way_interactions(int i, int t, std::vector<Single*> interaction_so_far);

        // after the above method completes, call this one to fill out the set of all Ts
        void build_size_d_Ts(int i, int d, std::vector<T*> T_so_far);
};