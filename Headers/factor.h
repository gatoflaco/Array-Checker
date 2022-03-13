/* LA-Checker by Isaac Jung
Last updated 03/13/2022

|===========================================================================================================|
|   This header contains classes used for organizing data associated with the arrays used by check.cpp. See |
| parser.cpp for a look at how these classes are used. The idea behind the organization is also covered in  |
| the README. The short of it is that it is useful to know in what rows a given interaction can occur, and  |
| it is also useful to have a class representing an entire column in the array. Each column corresponds to  |
| factor, and each factor has an associated range of levels it can take on.
|===========================================================================================================| 
*/

#pragma once
#ifndef FACTOR
#define FACTOR

#include "parser.h"
#include <set>

// basically just a tuple, but with a set of rows in which it occurs
class Single
{
    public:
        int f;  // represents the factor, or column of the array
        int v;  // represents the actual value of the factor
        std::set<int> rows;    // tracks the set of rows in which this (factor, value) occurs

};

// only for 2-way interactions; not yet generalized to any t-way interaction
class Interaction
{
    public:
        //int strength;   // the interaction strength t is the number of (factor, value) tuples involved
        std::vector<Single> factors;  // the actual list of (factor, value) tuples

        // this tracks the set of tests (represented as row numbers) in which this interaction occurs;
        std::set<int> rows;    // this row coverage is vital to analyzing the locating and detecting properties
};

// think of this class as containing the information associated with a single column in the array
class Factor
{
    public:
        int id;
        int level;
        int interactions_size;
        Interaction **interactions;
        ~Factor();
};

#endif // FACTOR
