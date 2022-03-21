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