# Array-Checker
### Author: Isaac Jung

## Credits
[Stephen A. Seidel](https://www.linkedin.com/in/stephen-seidel-18607069/)
- Author of the [v4-la-tools project](#additional-links) which this project aims to supplement

[Violet R. Syrotiuk, Ph.D.](https://isearch.asu.edu/profile/518662)
- Supervisor of this project
- Author of research on locating arrays

[Charles J. Colbourn, Ph.D.](https://isearch.asu.edu/profile/384970)
- Advisor to the author of this project
- Credited for [proposing locating and detecting arrays](#additional-links)

[Michael Poirier-Shelton](https://github.com/MichaelPS1995)
- Tester
- Peer who worked with author on the problem

## Overview
The purpose of this program is to verify whether test suites of enumerated 2-dimensional arrays have the covering, locating, and detecting properties. For those arrays which are detecting, the program can also give the highest separation. [Details and definitions](#details-and-definitions) are given further on. The inspiration of this project was [Stephen Seidel's](#credits) [v4-la-tools](#additional-links), intended as a supplementary tool to verify the correctness of output produced by Stephen's program.

## Usage
```
./check [d|] [t|] [δ|] [-[flags*]|]
```
Example (including input redirection, **which is recommended**):
```
./check 1 2 -hl < Sample-Input/Colbourn1.tsv
```
- This would run the program with d = 1, t = 2, in "halfway" output mode, looking for only the locating property, using the file `Sample-Input/Colbourn.tsv` as a source of input. Read on for full details.
### Compiling
Included is a simple makefile for quick compilation of the exectuable. After downloading the project, the makefile can be used in a terminal by running the following command from within its directory:
```
make
```
This will create the executable with the name "check" in the same directory as the makefile, unless the executable already exists and is up to date. Of course, the makefile can be edited, or compilation can be done manually, for a different executable.
### Running
After compiling, the executable may be started at will. For example, from within the same directory, the following command will run the program once:
```
./check
```
Running the program like this, it will appear to get stuck waiting for input forever. This is because the program reads from standard in. While it is ok to manually provide input, this is not recommended. Instead, consider redirecting input from a file. To do this, invoke the executable like this:
```
./check < Sample-Input/example.tsv
```
This would automatically read lines from `Sample-Input/example.tsv` with no extra input required. Replace the path with that of the file to be analyzed. Note that if no [additional arguments](#options) are given, the program has certain default behaviors, including default d, t values (read about this in the [options](#command-line-arguments) section further below).
### Input
The format of the input should be as follows:
```
v2.0
R C
L_1 L_2 ... L_C
0
0
...
0
# # # # # # # #
# # # # # # # #
...
# # # # # # # #
```
- The first line MUST consist of the string "v2.0".
- On the second line, give the number of rows in the array, R, followed by the number of columns in the array, C, separated by whitespace.
- On the third line, give the number of levels that each factor can take on, respectively, each separated by whitespace. The number of values given here should be equal to the value of C given on the first line.
- Starting at the fourth line, there must be (C + 1) lines consisting of just the number 0. If you are wondering why... I'm not sure myself. This program's primary purpose is to validate output from Stephen's program, and his output does this for some reason.
- After that, there begins the array. It should hold that the array has as many rows as was specified by R and as many columns as was specified by C. Furthermore, every column should consist of values that are within the corresponding number of levels given on the third line. All values in a given row of the array should be separated by whitespace, and all rows should be on their own lines.
Violating the format will result in some sort of error, meaning the program will not attempt to analyze anything. The program is capable of some very basic error identification, to assist you in fixing small issues in your input. Typically the file format is a TSV (tab separated values), meaning that any whitespace characters are actually tabs. However, it is fine to use standard whitespace as well, meaning a TSV is not mandatory. Check the `Sample-Input/` directory for examples of the file format in action. Also note that any additional lines after the array will not be looked at, meaning you can use that space to add notes or other useful info without disrupting the program.
### Output
By default, when there are no input format errors, the output of this program shows high level checks performed on the array. Specifically, it will look like this:
```
Reading input....

Checking coverage....

[coverage_issue*]

COVERAGE CHECK: [PASSED|FAILED]

Checking location....

[location_issue*]

LOCATION CHECK: [PASSED|FAILED]

Checking detection....

[detection_issue*]

DETECTION CHECK: [PASSED|FAILED]

CONCLUSIONS:
  The array is [not|] t-covering.
  The array is [not|] (d, t)-locating.
  The array is [not|] (d, t, δ)-detecting.
  [The greatest separation is actually Δ.|]
```
- Note that d, t, and δ will be actual numbers. By default, d will be 1, t will be 2, and δ will be 1, but they can be [specified](#command-line-arguments) when calling the executable.
- The value of Δ may or may not even be reported, as it only applies when the array is detecting, but the true separation is greater than what was requested by δ.
- The output may be made more or less verbose than this using various [flags](#flags) on the command line. 
- The `x_issue` lines are just indented lists of anything that violates property `x`. Their exact format depends on `x`. Below, an explanation of each is given alongside an example.
1. `coverage_issue`:
```
-- 3-WAY INTERACTION NOT PRESENT --
{(f0, 2), (f2, 3), (f3, 0)}
```
  - This says that the 3-way interaction between factor 0 with value 2, factor 2 with value 3, and factor 3 with value 0, is not present on any row of the array. Note that different values of d do not affect this output; only t does. This example would be for a user-specified t = 3.
2. `location_issue`:
```
-- DISTINCT SETS WITH EQUAL ROWS --
Set 1: { {(f0, 1), (f3, 0)}; {(f0, 1), (f3, 1)} }
Set 2: { {(f1, 1), (f3, 0)}; {(f1, 1), (f3, 1)} }
Rows: { 9, 10 }
```
  - This shows two sets of user-specified magnitude d = 2, each consisting of two 2-way interactions. The interpretation of this output is that the rows in which the first set of interactions occurs is equal to the rows in which the second set of interaction occurs, specifically rows 9 and 10. For more information on why this is an issue, refer to the [details and definitions](#details-and-definitions) section.
3. `detection_issue`:
```
-- ROW DIFFERENCE LESS THAN 2 --
Int: {(f0, 0), (f3, 2)}, { 2, 4 }
Set: { {(f0, 0), (f2, 1)} }, { 3, 4, 5 }
Dif: { 2 }
```
  - This would be during a check for (1, 2)-detection with a desired separation of δ = 2. The output shows that the set of rows in which the interaction occurs, {2, 4}, has only 1 element unique from those in which the given set occurs, {3, 4, 5}. That row, {2}, is reported, more than anything to show that the magnitude of the set difference is indeed less than δ. Again, refer to the [details and definitions](#details-and-definitions) section for a better understanding of where this is coming from.

## Options
The program may be invoked with a number of additional command line arguments and flags to alter its behavior. This is different from I/O redirection. Refer to the [usage](#usage) section for a better visual of what it looks like. This section describes the details:
- Despite the simplified visual in the usage section, [command line arguments](#command-line-arguments) and [flags](#flags) may actually come in either order. They are distingushed by a hyphen character (-).
- The command line arguments should *not* have leading hyphens and are simply delimited by whitespace. The relative order of these arguments **actually matters**. While flags can be mixed in anywhere between the arguments, the arguments are interpretted like this: the first integer encountered is assumed to be t. If a second integer is encountered, it is assumed to be t with the previous t assumed to instead be d. If a third is encountered, it is assumed to be δ. This means that in order to specify d, you must also specify t, and in order to specify δ, you must also specify both d and t.
- The flags are demarcated by a leading hyphen. Flags may use separate hyphens or share a single one. To "share" a single hyphen, additional flags beyond the first must succeed each other directly, i.e., without any whitespace. If whitespace is used between flags, a hyphen must be prepended for each of the whitespace-separated groups of flags.
- If the program cannot interpret a command line argument, it will ignore it and continue, possibly using default values/behaviors.
### Command Line Arguments
d: an integer bounded between 1 and t, inclusive
- Represents the magnitude of sets of interactions used; these sets are used in comparisons necessary to analyzing the locating and detecting properties of arrays.
- If not given, 1 is used by default.

t: an integer bounded between 1 and the number of factors, inclusive
- Represents how many (factor, value) tuples are included in an interaction; 1-way interactions by definition are just single (factor, value) tuples.
- If not given, 2 is used by default.

δ: an integer bounded between 1 and the number of tests, inclusive
- Represents [separation](#details-and-definitions), a strictness on the detection check; the higher the value of δ, the more strict the check. Realistically, the upper bound is much lower than the total number of tests.
- If not given, 1 is used by default.
- Goes unused if the c or l flags are given but not the d flag.
### Flags
v: verbose
- States what flags are set, as well as the relevant values of d, t, and δ, prior to reading input.
- Continues checking properties even when an earlier property was found to fail. Because detecing implies locating, and locating implies covering, if the coverage or location check fails, the program will normally not bother to check the more strict properties as they fail by definition. In verbose mode, however, the program will explicitly check for all properties requested.
- Not mutually exclusive with the h nor the s flags, but likely counterintuitive, especially with the s flag.

h: halfway
- Does not print any `coverage_issue`, `location_issue`, or `detection_issue`, but still signals what checks it is doing.
- Mutually exclusive with the s flag; if both are specified, the last one seen takes priority.

s: silent
- Prints only the conclusion, and without the `CONCLUSION` header.
- Mutually exclusive with the h flag; if both are specified, the last one seen takes priority.

c: coverage
- Coverage is computed and reported.
- Can be combined with the l and d flags to report more than one thing. Not specifying any is the same as specifying all three. Note that specifying all three and then continuing to specify more will "loop" the program's interpretation. e.g. -cldc will actually only check coverage.

l: location
- Location is computed and reported. This necessarily means coverage is computed internally, but the checking won't be shown unless the c flag is also set.
- Can be combined with the l and d flags to report more than one thing. Not specifying any is the same as specifying all three. Note that specifying all three and then continuing to specify more will "loop" the program's interpretation. e.g. -cd -lcl will actually check coverage and location.

d: detection
- Detection is computed and reported. This necessarily means coverage and location are computed internally, but the checking won't be shown unless the c and l flags are set, respectively.
- Can be combined with the l and d flags to report more than one thing. Not specifying any is the same as specifying all three. Note that specifying all three and then continuing to specify more will "loop" the program's interpretation. e.g. -dlcdlc -d will actually only check detection

## Details and Definitions
The actual implementation of this program is simple in terms of flow. The program begins by getting input from the command line and standard in. After doing basic syntax parsing and semantic error checking, if all looks fine, it passes the info from input to an Array object constructor, which organizes the data in the array into groups of vectors and sets. When this is done, the main program simply calls Array methods on the instantiated object to perform each check requested. If any of the less strict checks fail, by definition the more strict ones will too, so the program doesn't bother computing them and jumps to a conclusion (unless verbose mode is enabled, in which case it will compute everything requested regardless). The following block describes the flow of the Array object construction and checks. To best understand any unfamiliar terms, be sure to check the definitions listed later on.

  The Array constructor creates Factor objects representing the columns in the array. It checks how many levels each Factor has, and for each factor, it instantiates Single objects for every value that factor can take on, in an array of pointers. A Single object consists of the (factor, value) pair along with the rows in which it occurs. At instantiation, a Single object has an empty set of rows in which it occurs. After all of the Single object pointer arrays are created for every Factor, the Array constructor begins actually processing the Parser object's array. For every (row, column) pair, it uses a map to find the associated Single object and update its set of rows in which it occurs to include the newly found row. At the end, the Array has access to every (factor, value) that could be present based on the provided array and the levels of each column, as well as the sets of rows in which those (factor, value) Singles occur.
  The Array constructor then uses a somewhat complicated recursive method to construct all possible t-way Interactions among those Singles, simultaneously filling out the rows in which those Interactions occur (defined as the intersection of the sets of rows in which all of the Singles involved occur). Whereas the Singles were stuck in pointer arrays associated with each Factor (because an Interaction such as {(0, 0), (0, 1)} should not be possible; interactions *must* be between different factors), all of the Interaction objects have their pointers added to a single vector; if size-d sets of Interactions are later needed, no distinct categories of Interactions are needed to build them. After all possible Interaction pointers have been added to the vector, the Array object has enough information to compute strength t coverage.
  Next, the Array constructor checks if location or detection were requested (via the Parser object's fields). If so, it also must build all possible size-d sets of the t-way interactions. Note that the name of this class is simply T. Building all possible Ts requires a method very similar to building all possible Interactions. It is slightly simpler due to the linear nature of the vector containing all Interaction pointers. As size-d sets are created, they are inserted into a set; conceptually, the order does not matter. At the end of this step, the Array object has enough information to compute location and detection. If neither was requested, this entire phase is skipped, saving computation time.
  For checking t-coverage, the is_covering() method is called by the main program. The method iterates through all Interactions and simply ensures that the magnitude of the set of rows associated with each Interaction is at least t. If it is not, the issue is logged for the user to see which Interaction does not appear enough times (the program could detect a coverage issue earlier if a Single itself did not have an adequate-size set of rows, but the design decision was made to not check for that). As soon as a single issue is discovered, the Array by definition lacks t-coverage. Therefore, when certain flags are set to avoid extra work, this method will immediately return false in this situation. Otherwise, it will simply set a boolean and continue reporting any other issues it finds, waiting to return false till the end. If all Interactions satisfy t-coverage, the method returns true.
  For checking (d, t)-location, the is_locating() method is called by the main program. The method considers all pairs of Ts and ensures that no two Ts share the same set of rows in which they occur. If any two distinct Ts have the same set of rows, it is reported as an issue to the user. Just like with the coverage check, a single issue means the entire method should return false, and setting certain flags can make the method return false upon encountering any issue at all. Otherwise, the method reports all issues discovered. If all pairs of distinct Ts satisfy (d, t)-location, the method returns true.
  For checking (d, t, δ)-detection, the is_detecting() method is called by the main program. The method iterates over all Interactions, and for each Interaction, it iterates over all Ts. Then, for a given (Interaction, T) pair, the method ensures that either (a) the Interaction is a member of T, or (b) the set difference between the Interaction's rows and the T's rows is at least δ in magnitude. If the separation between a given Interaction and T is less than δ, the user is informed of the issue, and as with the previous methods, a boolean is set to false. Also like the previous methods, it is possible to immediately return false upon discovering a single issue, when certain flags are set. If the method finishes iterating and finds no (d, t, δ)-detection issues, the method returns true.
  During a check for detection, the Array also updates a `true_delta` field, defined simply as the minumum separation between all (Interaction, T) pairs. This is useful for letting the user know that there may be an even higher separation than they intended (not necessarily a bad thing).

Below is a non-comprehensive list of fundamental key words and concepts, ordered alphabetically. When a **bolded** term is used in a definition, that term can be found defined elsewhere in the list. The definitions and explanations here are not worded with formal mathematical notation, but rather with relatively plain English descriptions. For precise definitions and the full context of the problem, check out the [paper linked further down](#additional-links).
- **Array**: A 2-dimensional matrix of n rows by m columns, where each of the m columns represent **factors**, and each of the n rows represents a single **test**. Typically, the context is that the number of factors is already set, and we wish to find a set of tests that satisfy certain useful properties. The *exhaustive* set takes into account every possible combination of values assigned to the different factors in every possible **interaction**. Such a test suite would have the properties we want, but is unreasonably large for anything beyond the most trivial of systems, so we wish to form an array with minimal rows that still satisfies the desired properties. In general, these properties are **coverage**, **location**, and **detection**, listed in order of increasing strictness.
- **Coverage**: A covering **array** A of **strength** t is one in which every **interaction** T of strength t occurs at least once. It could be noted that for a (d, t, δ)-**detecting** array, every interaction of strength t must actually be covered at least δ times. However, the program doesn't try to factor in this logic when checking for coverage. It simply checks the set of rows in which the interaction occurs, ρ(A, T), for every interaction T in the array A. If |ρ(A, T)| = 0 for any T in A, the program reports it as a coverage issue. If, after checking all interactions, any issues were found, the program concludes that the array lacks all of coverage, **location**, and detection properties. The coverage property is a baseline requirement for a test suite which can identify the presence of interaction faults.
- **Detection**: A (d, t)-detecting **array** A of **strength** t can identify a set of d faulty t-way **interactions** when the set of *candidate* faults in A is at most d. The way that this is ensured is by checking that for every possible pair of t-way interactions T with size-d sets of t-way interactions 𝒯, if ρ(A, T) (the set of rows in A in which T occurs) is a subset of ρ(A, 𝒯) (the set of rows in which 𝒯 occurs), T MUST be a member of 𝒯. When a given test passes, it is known that every interaction in the test must not be faulty. Then, if a test fails, all the interactions in that test are suspect, minus the ones that are already excused for being present in some other passing test. In this way, the *candidate* t-way interactions can be narrowed down to a set, and if that set has at most d members, the (d, t)-detecting array confirms that those interactions are in fact the faulty ones. The way this program verifies detection is actually to take the set difference between ρ values for T and 𝒯; |ρ(A, T) - ρ(A, 𝒯)| must be at least 1. The reasoning is hard to follow, so it is highly recommended to just refer to [Dr. Colbourn's paper](#additional-links) for the best explanation and examples. The concept is made more confusing, however, when considering the need to safety-buffer for outliers and other bad test results. After all, a single bad test can destroy the ability to detect even if the test suite was designed to have the detection property. The solution is to overcompensate, requiring |ρ(A, T) - ρ(A, 𝒯)| to be at least δ. A (d, t, δ)-detecting array, then, can still detect a faulty set of d t-way interactions even if a given interaction is present in (δ - 1) bad test results. The value δ is said to be the *separation*. This program can check for a desired separation in a (d, t, δ)-detecting array, and report any pairs of T, 𝒯 that don't meet the standard. However, the program will also output the "true" separation of any valid (d, t)-detecting array A, defined as the maximum δ for which A would be (d, t)-detecting.
- **Interaction**: A t-way interaction is a grouping of t **factors** that have been assigned specific levels, and is often associated with a **test** in an **array**. Such an interaction is also said to have **strength** t. For example, if a test over the four factors {a, b, c, d} assigns {a1b0c2d0}, then one of the 1-way interactions in this test is a1, another is b0, and so on. Some of the 2-way interactions in the test include a1b0, a1c2, b0d0, and c2d0. A 3-way interaction might be a0c2d0, and there is only one 4-way interaction, which represents the whole test. When a test is faulty, is cannot be attributed to just one of the t factors being some level. After all, if that factor with that level appears in other tests, but those tests pass, what then? In reality, interactions among varying numbers of factors with different levels may be responsible for a given failing test. The aim is to design a test suite such that, should a test fail, it would be possible to narrow down or possibly even identify with some certainty which interaction(s) might be faulty.
- **Location**: A (d, t)-locating **array** A of **strength** t is able to *locate* any size d set of t-way **interactions** in A. Somewhat formally, this means that if every possible set 𝒯 is formed from from d pairs of t-way interactions in A, no two pairs of 𝒯 will share the same set of rows in which they occur. Note that the set of rows in which a 𝒯 occurs, ρ(A, 𝒯), is simply the union of all the sets of rows in which the t-way interactions it contains occur. For a better understanding, consider the following description of how this program handles the check. Assume that there is a data structure 𝒯s that is a set of all size d sets 𝒯. The program takes every possible pair of two of these 𝒯 sets in 𝒯s, 𝒯1 and 𝒯2, and checks if ρ(A, 𝒯1) = ρ(A, 𝒯2). If they are equal, the program reports that pair as a location issue. If, after checking all pairs of sets, any issues were found, the program concludes that the array lacks the location and **detection** properties for the given values of d and t. The location property is a step up from the **covering** property, and can reveal the location of t-way interaction faults. However, it is possible for ambiguity in how many of these faults are responsible. This is actually where the reasoning for creating d-size sets comes from; it narrows down the cause of a system fault to a set of d t-way interactions.
- **Factor**: A parameter in a complex engineered system. Every factor has a range of values it can be assigned; these are called the factor's *levels*, and are restricted to integers. If a factor is represented by the letter a, and it can take on 6 levels, 0-5, then the notation a2 would reprent the factor a with the value 2 assigned to it. Because factors directly correspond to columns in an n x m **array** A, an element in A represents an assignment to some factor, often denoted in the form (factor, value). This program will output pairs of (factor, value) assignments simarly; if the factors in A were {a, b, c, d}, the a2 example would be written (f0, 2). This is because the program enumerates the factors for generalizations. Note that it counts from 0.
- **Strength**: Put simply, for an **interaction** to have strength t is synonymous with calling it a t-way interaction. The strength is the number of (factor, level)s involved in the interaction.
- **Test**: An assignment of values to each **factor** in the system, selected from the valid levels they can take on. When a test is actually carried out, the system will produce some output, for which the experimenters will have some definition for passing. The goal is to identify a selection of value assignements that can cause a test to fail, and then eventually identify what it was. The cause of a failing test is said to be one or more faulty **interactions**.

## Additional Links
[Stephen Seidel's v4-la-tools](https://github.com/sseidel16/v4-la-tools)
- Project which inspired this one
- Capable of generating (1, 2)-locating arrays from scratch, given factors and their levels

[My Array-Generator](https://github.com/gatoflaco/Array-Generator)
- Tool to generate the types of arrays that this project verifies
- The input-format-simple branch of this project is meant to augment the generator in a convenient manner.

Colbourn and McClary, *[Locating and Detecting Arrays for Interaction Faults](https://drops.dagstuhl.de/opus/volltexte/2009/2240/pdf/09281.ColbournCharles.Paper.2240.pdf)*
- Paper cited as first to propose locating and detecting arrays
- Formal definitions for covering, locating, and detecting arrays can be found here
