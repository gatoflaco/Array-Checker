/* LA-Checker by Isaac Jung
Last updated 02/09/2022

|===========================================================================================================|
|   TO BE WRITTEN                                                                                           |
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