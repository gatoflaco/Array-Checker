/* LA-Checker by Isaac Jung
Last updated 02/16/2022

|===========================================================================================================|
|   This header contains classes for managing the array in an automated fashion. The Row class is just a    |
| helper used by the Array class and should not be instantiated by any source file other than the one       |
| associated with this header. The Array class is the interface with which other source files should work.  |
| It contains a constructor that builds all the internal data structures, thereby allowing the instantiator |
| to immediately call other methods which carry out analyses on this data. See check.cpp for an example.    |
|===========================================================================================================| 
*/

#include "Factor.h"

class Row;  // forward declaration, see line 35 for definition

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
        std::vector<Row> rows;
};

// simple helper class whose only purpose is to add naming clarity to data management
// e.g., the Array field called rows could be a std::vector<std::vector<Interaction*>>, but the name
// and accessing of the field would be less easy to understand
class Row
{
    public:
        std::vector<Interaction*> interactions;
};