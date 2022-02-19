/* LA-Checker by Isaac Jung
Last updated 02/09/2022

|===========================================================================================================|
|   This header contains classes used for organizing data associated with the arrays used by check.cpp. See |
| InputInfo.cpp for a look at how these classes are used.                                                   |
|===========================================================================================================| 
*/

#pragma once
#ifndef FACTOR
#define FACTOR

#include "InputInfo.h"
#include <set>

class Interaction;   // forward declaration needed here to avoid circular referencing

class Factor
{
    public:
        int id;
        int level;
        int interactions_size;
        Interaction **interactions;
        ~Factor();
};

// only for 2-way interactions; not yet generalized to any t-way interaction
class Interaction
{
    public:
        Factor *this_factor;
        Factor *other_factor;
        int this_val;
        int other_val;
        std::set<int> occurrences;
        bool is_locating;
};

#endif // FACTOR
