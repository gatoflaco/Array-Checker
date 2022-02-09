/* LA-Checker by Isaac Jung
Last updated 02/09/2022

|===========================================================================================================|
|   This header contains a class for managing the array in a more oganized fashion. There are some methods  |
| the user of this class can call to automate the analysis of the array.                                    |
|===========================================================================================================| 
*/

#include "Factor.h"

class Array
{
    public:
        Array(InputInfo *in);
        bool has_strength_2();
        bool has_locating_property();
        ~Array();

    private:
        int num_tests;
        int num_factors;
        Factor *factors;
};