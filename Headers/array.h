/* LA-Checker by Isaac Jung
Last updated 02/25/2022

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

#include "factor.h"

class Row;  // forward declaration, see line 48 for definition

class Array
{
    public:
        Array(Parser *in, int d = 1, int t = 2); // checking for (1, 2)-properties by default

        // checks whether the array is covering; this means that every interaction of strength t occurs in
        bool is_covering();     // the array at least δ times; i.e., in δ unique tests

        // checks whether the array is (d, t, δ)-locating; this means that for every pair of size-d sets of
        bool is_locating();     // t-way interactions, the rows covered by those size-d-sets is not equal

        // checks whether the array is (d, t, δ)-detecting; this extends the locating definition such that
        // for every pair of size-d sets of t-way interactions, the rows covered by one set must include at
        bool is_detecting();    // least δ rows not covered by the other set, and vice versa

        ~Array();

    private:
        int d;  // this is the size of the sets of t-way interactions; the sets are what need to be compared
        int t;  // this is the strength of interest; see README for details on the strength of an interaction
        int δ;  // this is the desired separation of the array; it affects the is_detecting() method
        int true_δ;         // the true maximum separation is tracked and updated a
        int num_tests;      // just a field to reference the upper bound on iterating through rows
        int num_factors;    // just a field to reference the upper bound on iterating through columns
        Factor *factors;    // pointer to the start of an array of Factor objects placed next to each other
        std::set<𝒯*> 𝒯s;     // a set of all size-d sets of t-way interactions constructable from the array
        std::set<Interaction*> interactions;    // set of all individual interactions
};

// I wasn't sure what to name this, except after the formal parameter used in Dr. Colbourn's definitions
// This class is used to more easily compare the row coverage of different size-d sets of t-way interactions
class 𝒯
{
    public:
        std::vector<Interaction*> interactions; // for easier access to the interactions themselves

        // each interaction in a given 𝒯 set has its own version of this; the ρ associated with a 𝒯 is simply
        std::set<int> ρ;  // the union of the ρ's for each interaction in that 𝒯
};