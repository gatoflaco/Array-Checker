/* LA-Checker by Isaac Jung
Last updated 02/25/2022

|===========================================================================================================|
|   This header contains a class used for processing input. Should the input format change, this class can  |
| be updated accordingly. The way this class is used is to have the main code instantiate a single Parser   |
| object, then call the process_input() method. This will handle basic syntax and semantic checking on the  |
| input read from standard in (or, the process_input() method could be defined to read from a file). If no  |
| errors are found, the process_input() method should also assign to the Parser's other public fields based |
| on the input read. Most noteably, the Parser object stores the 2d array of ints itself - the array that   |
| should eventually be checked for covering, locating, and detecting properties. The main program can then  |
| decide what how it wishes to work with this array more easily.                                            |
|===========================================================================================================| 
*/

#pragma once
#ifndef PARSER
#define PARSER

#include <string>
#include <vector>

class Parser
{
    public:
        int num_rows = 0;
        int num_cols = 0;
        std::vector<int> levels;
        std::vector<int*> array;
        int process_input();
        ~Parser();

    private:
        void trim(std::string &s);
        void syntax_error(int lineno, std::string expected, std::string actual, bool verbose = true);
        void semantic_error(int lineno, int row, int col, int level, int value, bool verbose = true);
        void other_error(int lineno, std::string line, bool verbose = true);
};

#endif // PARSER
