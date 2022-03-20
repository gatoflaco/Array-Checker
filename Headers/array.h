/* LA-Checker by Isaac Jung
Last updated 03/19/2022

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
        // the array at least 1 time (TODO: extend this to at least δ times for (t, δ)-coverage)
        bool is_covering(bool report = true);

        // checks whether the array is (d, t)-locating; this means that for every pair of size-d sets of
        // t-way interactions, the rows covered by those sets are not equal
        bool is_locating(bool report = true);

        // checks whether the array is (d, t, δ)-detecting; this checks for all t-way interactions, for all
        // size-d sets, T is a member of the set OR T's rows minus the set's rows has >= δ elements
        bool is_detecting(bool report = true);

        Array();    // default constructor, don't use this
        Array(Parser *in);  // constructor that takes an initialized Parser object
        ~Array();   // deconstructor

    private:
        int d;  // this is the size of the sets of t-way interactions; the sets are what need to be compared
        int t;  // this is the strength of interest; see README for details on the strength of an interaction
        int delta;  // this is the desired separation of the array; it affects the is_detecting() method
        verb_mode v;    // unused at the moment
        out_mode o;     // this dictates how much output should be printed; see parser.h for typedef
        prop_mode p;    // this is used to avoid building sets if it won't be needed anyway
        int num_tests;      // just a field to reference the upper bound on iterating through rows
        int num_factors;    // just a field to reference the upper bound on iterating through columns
        Factor *factors;    // pointer to the start of an array of Factor objects placed next to each other
        std::set<T*> sets;  // a set of all size-d sets of t-way interactions constructable from the array
        std::vector<Interaction*> interactions; // list of all individual interactions

        // this utility method is called in the constructor to fill out the vector of all interactions
        // almost certainly needs to be recursive in order to handle arbitrary values of t
        void build_t_way_interactions(int i, int t, std::vector<Single*> interaction_so_far);

        // after the above method completes, call this one to fill out the set of all size-d sets
        // almost certainly needs to be recursive in order to handle arbitrary values of d
        void build_size_d_Ts(int i, int d, std::set<Interaction*> set_so_far);
};