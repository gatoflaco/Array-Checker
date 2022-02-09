/* LA-Checker by Isaac Jung
Last updated 02/09/2022

|===========================================================================================================|
|   This file contains just the deconstructor for the Factor class. The Interaction class, which is also    |
| defined in Factor.h, has no deconstructor, because it requires no extra heap memory when instantiated.    |
|===========================================================================================================| 
*/

#include "Factor.h"

/* DECONSTRUCTOR - frees memory
*/
Factor::~Factor()
{
    for (int i = 0; i < interactions_size; i++) delete[] interactions[i];
    delete[] interactions;
}