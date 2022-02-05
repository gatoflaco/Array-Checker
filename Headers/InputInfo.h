/* LA-Checker by Isaac Jung
Last updated 02/04/2022

|===========================================================================================================|
|   This header contains a class used for processing input. Should the input format change, this class can  |
| be updated accordingly.                                                                                   |
|===========================================================================================================| 
*/

#pragma once
#ifndef INPUTINFO
#define INPUTINFO

#include <string>
#include <vector>

class InputInfo
{
    public:
        int num_rows = 0;
        int num_cols = 0;
        std::vector<int> levels;
        std::vector<int*> array;
        int process_input();
        ~InputInfo();

    private:
        void trim(std::string &s);
        void syntax_error(int lineno, std::string expected, std::string actual, bool verbose = true);
        void semantic_error(int lineno, int row, int col, int level, int value, bool verbose = true);
        void other_error(int lineno, std::string line, bool verbose = true);
};

#endif // INPUTINFO
