/* LA-Checker by Isaac Jung
Last updated 02/09/2022

|===========================================================================================================|
|   TO BE WRITTEN                                                                                           |
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
        Factor *other;
        int this_val;
        int other_val;
        std::set<int> rows;
};

#endif // FACTOR
