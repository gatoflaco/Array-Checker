/* LA-Checker by Isaac Jung
Last updated 03/20/2022

|===========================================================================================================|
|   This file contains just the deconstructor for the Factor class. The Interaction class, which is also    |
| defined in factor.h, has no deconstructor, because it requires no extra heap memory when instantiated.    |
|===========================================================================================================| 
*/

#include "factor.h"
#include <algorithm>

/* CONSTRUCTOR - initializes the object
 * - overloaded: this is the default with no parameters, and should not be used
*/
Single::Single()
{
    factor = -1;
    value = -1;
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this version can set its fields based on parameters
*/
Single::Single(int f, int v)
{
    factor = f;
    value = v;
    // rows will be built later
}

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
Interaction::Interaction(std::vector<Single*> *s)
{
    // fencepost start: let the Interaction be the strength 1 interaction involving just the 0th Single in s
    singles.push_back(s->at(0));
    rows = s->at(0)->rows;

    // fencepost loop: for any t > 1, rows of the Interaction is the intersection of each Single's rows
    for (int i = 1; i < s->size(); i++) {
      singles.push_back(s->at(i));
      std::set<int> temp;
      std::set_intersection(rows.begin(), rows.end(),
        s->at(i)->rows.begin(), s->at(i)->rows.end(), std::inserter(temp, temp.begin()));
      rows = temp;
    }
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this is the default with no parameters, and should not be used
*/
Factor::Factor()
{
    id = -1;
    level = -1;
    singles = nullptr;
}

/* CONSTRUCTOR - initializes the object
 * - overloaded: this version can set its fields based on parameters
*/
Factor::Factor(int i, int l, Single **ptr_array)
{
    id = i;
    level = l;
    singles = ptr_array;
}

/* DECONSTRUCTOR - frees memory
*/
Factor::~Factor()
{
    for (int i = 0; i < level; i++) delete singles[i];
    delete[] singles;
}