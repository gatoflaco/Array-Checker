/* LA-Checker by Isaac Jung
Last updated 02/08/2022

|===========================================================================================================|
|   This file contains definitions for methods used to analyze factors.                                     |
|===========================================================================================================| 
*/

#include "Factor.h"

Factor::~Factor()
{
    for (int i = 0; i < interactions_size; i++) delete[] interactions[i];
    delete[] interactions;
}