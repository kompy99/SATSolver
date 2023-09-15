**Varun Komperla  
vkomperla3@gatech.edu  
GT ID: xx3926404**

I confirm that I have adhered to the Georgia Tech honor code.

# Table of Contents

- [Table of Contents](#table-of-contents)
- [SATSolver](#satsolver)
  - [Einstein Puzzle](#einstein-puzzle)
  - [Encoding of the Einstein Puzzle](#encoding-of-the-einstein-puzzle)
    - [Proposition List](#proposition-list)
      - [Variables for House-Color combinations](#variables-for-house-color-combinations)
      - [Variables for House-Nationality combinations](#variables-for-house-nationality-combinations)
      - [Variables for House-Beverage combinations](#variables-for-house-beverage-combinations)
      - [Variables for House-Cigar combinations](#variables-for-house-cigar-combinations)
      - [Variables for House-Pet combinations](#variables-for-house-pet-combinations)
    - [Constraints Generation](#constraints-generation)
      - [Base Constraints](#base-constraints)
      - [Implication-type constraints](#implication-type-constraints)
      - [Neighbour-type constraints](#neighbour-type-constraints)
      - [Fact-type constraint](#fact-type-constraint)
    - [Einstein Puzzle CNF](#einstein-puzzle-cnf)
  - [DPLL Solver](#dpll-solver)
- [Execute](#execute)
  - [Pre-requisites](#pre-requisites)
  - [Build and Run](#build-and-run)
    - [EinsteinCNF.cpp](#einsteincnfcpp)
    - [SAT.cpp](#satcpp)
  - [Output](#output)
- [Solution](#solution)



# SATSolver
This is a repository containing a DPLL implementation of a SAT solver which uses different types of decision heuristics.

This project was done as part of the ***CS-8803: Logic in Computer Science*** course at Georgia Tech. 

The objectives of this project was as follows:
1) Encode the _*Einstein Puzzle*_ as a SAT problem in CNF form.
2) Implement a DPLL SAT Solver
3) Use the SAT Solver to find a solution to the problem from _Objective 1_. 


## Einstein Puzzle

The Einstein Puzze is as follows:- 

There are five houses in five different colors. In each house lives a man with a different
nationality. The five owners drink a certain type of beverage, smoke a certain brand of cigar,
and keep a certain pet. No owners have the same pet, smoke the same brand of cigar or drink
the same beverage. The question is: “Who owns the fish?”

Hints:
- The Brit lives in the red house.
- The Swede keeps dogs as pets.
- The Dane drinks tea.
- The green house is on the left of the white house.
- The green house’s owner drinks coffee.
- The person who smokes Pall Mall rears birds.
- The owner of the yellow house smokes Dunhill.
- The man living in the center house drinks milk.
- The Norwegian lives in the first house.
- The man who smokes Blends lives next to the one who keeps cats.
- The man who keeps the horse lives next to the man who smokes Dunhill.
- The owner who smokes Bluemasters drinks beer.
- The German smokes Prince.
- The Norwegian lives next to the blue house.
- The man who smokes Blends has a neighbor who drinks water.

In order to solve the puzzle, you have to make some assumptions:
1. The owner is the resident of each house.
2. One of the residents owns the fish.
3. The term neighbor in the last hint refers only to a directly adjacent neighbor.
4. The houses are on the same side of the street.
5. They are next to each other, and are ordered from left to right as you face them rather than standing in front of a house facing the street (i.e. facing the same direction as the
house)

## Encoding of the Einstein Puzzle

### Proposition List
The following is the list of variables, where each variable is represented as a number:

#### Variables for House-Color combinations

| (House, Red) | (House, White) | (House, Green) | (House, Yellow) | (House, Blue) |
|----------------|----------------|----------------|----------------|----------------| 
|(1, clr_r) - 1 | (1, clr_w) - 2 | (1, clr_g) - 3 | (1, clr_y) - 4 | (1, clr_b) - 5 |  
|(2, clr_r) - 6 | (2, clr_w) - 7 | (2, clr_g) - 8 | (2, clr_y) - 9 | (2, clr_b) - 10 |  
|(3, clr_r) - 11 | (3, clr_w) - 12 | (3, clr_g) - 13 | (3, clr_y) - 14 | (3, clr_b) - 15 |  
|(4, clr_r) - 16 | (4, clr_w) - 17 | (4, clr_g) - 18 | (4, clr_y) - 19 | (4, clr_b) - 20 |  
|(5, clr_r) - 21 | (5, clr_w) - 22 | (5, clr_g) - 23 | (5, clr_y) - 24 | (5, clr_b) - 25 |

#### Variables for House-Nationality combinations

| (House, Brit) | (House, Swede) | (House, Dane) | (House, Norwegian) | (House, German) |
|----------------|----------------|----------------|----------------|----------------| 
|(1, ntn_B) - 26 | (1, ntn_S) - 27 | (1, ntn_D) - 28 | (1, ntn_N) - 29 | (1, ntn_G) - 30 |  
|(2, ntn_B) - 31 | (2, ntn_S) - 32 | (2, ntn_D) - 33 | (2, ntn_N) - 34 | (2, ntn_G) - 35 |  
|(3, ntn_B) - 36 | (3, ntn_S) - 37 | (3, ntn_D) - 38 | (3, ntn_N) - 39 | (3, ntn_G) - 40 |  
|(4, ntn_B) - 41 | (4, ntn_S) - 42 | (4, ntn_D) - 43 | (4, ntn_N) - 44 | (4, ntn_G) - 45 |  
|(5, ntn_B) - 46 | (5, ntn_S) - 47 | (5, ntn_D) - 48 | (5, ntn_N) - 49 | (5, ntn_G) - 50 |

#### Variables for House-Beverage combinations

| (House, Red) | (House, White) | (House, Green) | (House, Yellow) | (House, Blue) |
|----------------|----------------|----------------|----------------|----------------| 
|(1, clr_r) - 1 | (1, clr_w) - 2 | (1, clr_g) - 3 | (1, clr_y) - 4 | (1, clr_b) - 5 |  
|(2, clr_r) - 6 | (2, clr_w) - 7 | (2, clr_g) - 8 | (2, clr_y) - 9 | (2, clr_b) - 10 |  
|(3, clr_r) - 11 | (3, clr_w) - 12 | (3, clr_g) - 13 | (3, clr_y) - 14 | (3, clr_b) - 15 |  
|(4, clr_r) - 16 | (4, clr_w) - 17 | (4, clr_g) - 18 | (4, clr_y) - 19 | (4, clr_b) - 20 |  
|(5, clr_r) - 21 | (5, clr_w) - 22 | (5, clr_g) - 23 | (5, clr_y) - 24 | (5, clr_b) - 25 |

#### Variables for House-Cigar combinations

| (House, Pall Mall) | (House, Dunhill) | (House, Blends) | (House, Prince) | (House, Bluemasters) |
|--------------------|------------------|-----------------|-----------------|----------------------|
|(1, cig_p) - 76 | (1, cig_d) - 77 | (1, cig_b) - 78 | (1, cig_c) - 79 | (1, cig_s) - 80 |
|(2, cig_p) - 81 | (2, cig_d) - 82 | (2, cig_b) - 83 | (2, cig_c) - 84 | (2, cig_s) - 85 |
|(3, cig_p) - 86 | (3, cig_d) - 87 | (3, cig_b) - 88 | (3, cig_c) - 89 | (3, cig_s) - 90 |
|(4, cig_p) - 91 | (4, cig_d) - 92 | (4, cig_b) - 93 | (4, cig_c) - 94 | (4, cig_s) - 95 |
|(5, cig_p) - 96 | (5, cig_d) - 97 | (5, cig_b) - 98 | (5, cig_c) - 99 | (5, cig_s) - 100|

#### Variables for House-Pet combinations

| (House, Cat)  | (House, Horse) | (House, Dogs) | (House, Fish) |(House, Birds) |
| ----------| ----------|---------  |---------- |---------------|
|(1, pet_c) - 101 | (1, pet_h) - 102 | (1, pet_d) - 103 | (1, pet_f) - 104 | (1, pet_b) - 105|
|(2, pet_c) - 106 | (2, pet_h) - 107 | (2, pet_d) - 108 | (2, pet_f) - 109 | (2, pet_b) - 110|
|(3, pet_c) - 111 | (3, pet_h) - 112 | (3, pet_d) - 113 | (3, pet_f) - 114 | (3, pet_b) - 115|
|(4, pet_c) - 116 | (4, pet_h) - 117 | (4, pet_d) - 118 | (4, pet_f) - 119 | (4, pet_b) - 120|
|(5, pet_c) - 121 | (5, pet_h) - 122 | (5, pet_d) - 123 | (5, pet_f) - 124 | (5, pet_b) - 125|


$\therefore \;$  For each house, we have a boolean variable to represent each property of the house (Color, Beverage, Nationality, Cigar, Pet).

So, if the variable for *(1, clr_r)* is True

$\implies$ House #1 is red in color

and if the variable for *(1, clr_r)* is False

$\implies$ House #1 is **NOT** red in color

We used integers to represent these variables in order to represent the problem in [DIMACS](https://cs.rice.edu/~vardi/comp409/satformat.pdf) format.

### Constraints Generation

Given the problem and the hints we can generate different types of constraints:-

#### Base Constraints

For a given property type (Color, Beverage, etc.), we need to ensure that each value (Red, Blue, Water, Milk, etc.) is true for **at least** one house and also **at most** one house.

Also, for a given house, we need to ensure that **at least** and **at most** one value of a property type is true.

An example of these constraints for the color property are given below:

Given the variables for [houses-colors](#variables-for-house---color), we have the following constraints in DIMACS CNF format (each line is a separate clause):

- At least one house is color red:  
1 6 11 16 21 0  

- At most one house is color red:  
-1 -6 0  
-1 -11 0  
-1 -16 0  
-1 -21 0  
-6 -11 0  
-6 -16 0  
-6 -21 0  
-11 -16 0  
-11 -21 0  
-16 -21 0  

- House #1 is at least one color:  
1 2 3 4 5 0  

- House #1 is at most one color:  
-1 -2 0  
-1 -3 0  
-1 -4 0  
-1 -5 0  
-2 -3 0  
-2 -4 0  
-2 -5 0  
-3 -4 0  
-3 -5 0  
-4 -5 0  

#### Implication-type constraints

For example, one of the hints says that *"The Brit lives in the Red House"*. Let us call this an implication-type constraint.  
So, for instance, if the variable for (3, ntn_b) is true, then the variable for (3, clr_r) also **has to be** true and vice-versa. This means that if the man living in the 3rd house is the Brit, then the 3rd house **has to be** red in color, and vice versa.

Another way of saying this is that, 

If (3, ntn_b) = True,   
Then (1, clr_r), (2,clr_r), (4, clr_r) and (5, clr_r) must be False.

This means that if the man in the 3rd house is the Brit, then the 1st, 2nd, 4th and 5th houses **CANNOT** be red in color. This idea can be used to encode such a constraint in CNF form.

With (3, ntn_b), (1, clr_r), (2,clr_r), (4, clr_r) and (5, clr_r) as the propositions, we have

$\overline{((3, ntn\_b) \cap (1, clr\_r))} \cap \overline{((3, ntn\_b) \cap (2, clr\_r))} \cap \overline{((3, ntn\_b) \cap (4, clr\_r))} \cap \overline{((3, ntn\_b) \cap (5, clr\_r))}$

Applying De Morgan's law, we get:

$(\; \overline{(3, ntn\_b)} \; \cup \; \overline{(1, clr\_r)} \; ) \; \; \cap \; \;  (\; \overline{(3, ntn\_b)} \; \cup \; \overline{(2, clr\_r)} \; ) \; \; \cap \; \;  (\; \overline{(3, ntn\_b)} \; \cup \; \overline{(4, clr\_r)} \; ) \; \; \cap \; \;  (\; \overline{(3, ntn\_b)} \; \cup \; \overline{(5, clr\_r)} \; )$

which is in CNF form.

Using this idea, we can generate the following constraints (given the variables for [houses-colors](#variables-for-house---color) and [houses-nationalities](#variables-for-house---nationality)):

- Brit lives in Red House  
-26 -6 0  
-1 -31 0  
-26 -11 0  
-1 -36 0  
-26 -16 0  
-1 -41 0  
-26 -21 0  
-1 -46 0  
-31 -11 0  
-6 -36 0  
-31 -16 0  
-6 -41 0  
-31 -21 0  
-6 -46 0  
-36 -16 0  
-11 -41 0  
-36 -21 0  
-11 -46 0  
-41 -21 0  
-16 -46 0  


#### Neighbour-type constraints

For example, one of the hints says that *"The man who smokes Blends lives next to the one who keeps cats."*. Let us call this an neighbour-type constraint.

So, for instance, if the variable for (2, cig_b) is true, then one of the variables - (1, pet_c) or (3, pet_c) also **has to be** true and vice-versa. This means that if the man in the 2nd house smokes Blends, then either the man in the 1st house keeps cats, or the person in the 3rd house keeps cats.

Another way of saying this is that, 

If (2, cig_b) = True,   
Then (4, pet_c), (5, pet_c) must be False.

This means that if the man in the 2nd house smokes Blends, then the men in the 4th and 5th houses definitely DO NOT keep cats.

Same is true other way around, where say the man in the 3rd house keeps cats, then the men in the 1st and 5th houses definitely DO NOT smoke blends. This idea can be used to encode such a constraint in CNF form.

With (2, cig_b), (4, pet_c) and (5, pet_c) as propositions, we have:

$\overline{((2, cig\_b) \cap (4, pet\_c))} \cap \overline{((2, cig\_b) \cap (5, pet\_c))}$

Applying De Morgan's law, we get:

$(\; \overline{(2, cig\_b)} \; \cup \; \overline{(4, pet\_c)} \; ) \; \; \cap \; \;  ( \; \overline{(2, cig\_b)} \; \cup \; \overline{(5, pet\_c)} \; )$

which is in CNF form.

Using this idea, we can generate the following constraints (given the variables for [houses-cigars](#variables-for-house---cigar) and [houses-pets](#variables-for-house---pet)):

- The man who smokes Blends lives next to the one who keeps cats  
-78 -111 0  
-101 -88 0  
-78 -116 0  
-101 -93 0  
-78 -121 0  
-101 -98 0  
-78 -101 0  
-83 -116 0  
-106 -93 0  
-83 -121 0  
-106 -98 0  
-83 -106 0  
-88 -121 0  
-111 -98 0  
-88 -111 0  
-93 -116 0  
-98 -121 0  


#### Fact-type constraint

For example, one of the hints says that *"The man living in the center house drinks milk."*. Let us call this a fact-type constraint.

This just means that (3, bvg_m) has to be True. So this can be added as a constraint to the encoding as follows (using the variables for [houses-beverages](#variables-for-house---beverage)):

- The man living in the center house drinks milk  
63 0

### Einstein Puzzle CNF

 The above ideas can be applied to encode the base constraints and the hints of the Einstein puzzle in CNF.
 
- The Brit lives in the red house $\implies$ *Implication-type constraint*
- The Swede keeps dogs as pets $\implies$ *Implication-type constraint*
- The Dane drinks tea $\implies$ *Implication-type constraint*
- The green house is on the left of the white house $\implies$ *Neighbour-type constraint (with additional constraints to ensure left side only)*
- The green house’s owner drinks coffee $\implies$ *Implication-type constraint*
- The person who smokes Pall Mall rears birds $\implies$ *Implication-type constraint*
- The owner of the yellow house smokes Dunhill $\implies$ *Implication-type constraint*
- The man living in the center house drinks milk $\implies$ *Fact-type constraint*
- The Norwegian lives in the first house $\implies$ *Fact-type constraint*
- The man who smokes Blends lives next to the one who keeps cats $\implies$ *Neighbour-type constraint*
- The man who keeps the horse lives next to the man who smokes Dunhill $\implies$ *Neighbour-type constraint*
- The owner who smokes Bluemasters drinks beer $\implies$ *Implication-type constraint*
- The German smokes Prince $\implies$ *Implication-type constraint*
- The Norwegian lives next to the blue house $\implies$ *Neighbour-type constraint*
- The man who smokes Blends has a neighbor who drinks water $\implies$ *Neighbour-type constraint*
 
 The Einstein Puzzle was encoded in CNF form in DIMACS format and stored in the [*EinsteinDimacs.txt*](/src/EinsteinDimacs.txt) file.


[*EinsteinCNF.cpp*](/src/EinsteinCNF.cpp) contains the code that was used to encode the Einstein Puzzle in DIMACS format.

## DPLL Solver

The file [*SAT.cpp*](/src/SAT.cpp) contains the code for the DPLL solver which uses the random heuristic and the two-clause heuristic.

***Random Heuristic*** - All choices (propositions and truth values) are resolved randomly with a uniform distribution.

***Two-Clause Heuristic*** - Choose propositions with maximum occurrences in 2-clauses, i.e., clauses with two literals, and break ties randomly. Truth value assignment is done randomly.

# Execute

## Pre-requisites

- C++ should be installed.

## Build and Run

### EinsteinCNF.cpp

To build and run the [*EinsteinCNF.cpp*](/src/EinsteinCNF.cpp) program to see how the Einstein Puzzle is encoded into CNF format: 

1. Download the [*EinsteinCNF.cpp*](/src/EinsteinCNF.cpp) file to desired location.
2. Open terminal and navigate to location where file was downloaded.
3. Run the following commands in terminal.

        $ g++ --std=c++14 ./EinsteinCNF.cpp
        $ ./a.out > EinsteinCNFOutput.txt

This will run the [*EinsteinCNF.cpp*](/src/EinsteinCNF.cpp) file and store the output in a text file called *EinsteinCNFOutput.txt* in the same folder location.

**Notes:**
1. The *EinsteinCNFOutput.txt* file will contain the encoding of the problem in DIMACS CNF format towards the end of the file. This part can be copied to a new file to create a DIMACS file for the problem.*

2. The code inside the `PrintClause()` function in this file can be uncommented for more information about the clauses being generated for each constraint.

### SAT.cpp

To build and run the SAT solver on the Einstein Puzzle DIMACS file: 

1. Download the [*SAT.cpp*](/src/SAT.cpp) and [*EinsteinDimacs.txt*](/src/EinsteinDimacs.txt) files to desired location.
2. Open terminal and navigate to location where files were downloaded.
3. Run the following commands in terminal.

        $ g++ --std=c++14 ./SAT.cpp
        $ ./a.out

4. Enter input as per prompts.


        > Enter name of DIMACS file : 
          EinsteinDimacs.txt

        
        > No of propositions - 125
        > No of clauses - 793

        > Select heuristic (1 - Random, 2 - Two-Clause):
          2

## Output

The program will output:
1. **If problem is satisfiable:**  
   "SAT", followed by an assignment of the variables. A positive number indicates that the variable is assigned True, and a negative number indicates that the variable is assigned False.

2. **If problem is unsatisfiable:**  
   "UNSAT".

In both cases, the program will also output the number of splitting rule applications (or DPLL calls) that were made while attempting the solve the problem. The number of calls would depend on the size and form of the input as well as the decision heuristic selected.

An example output obtained from running the SAT solver above on the Einstein DIMACS file is shown below.

    Enter name of DIMACS file : 
    EinsteinDimacs.txt


    No of propositions - 125
    No of clauses - 793


    Select heuristic (1 - Random, 2 - Two-Clause):
    1

    SAT
    -1 -2 -3 4 -5

    -6 -7 -8 -9 10

    11 -12 -13 -14 -15

    -16 -17 18 -19 -20

    -21 22 -23 -24 -25

    -26 -27 -28 29 -30

    -31 -32 33 -34 -35

    36 -37 -38 -39 -40

    -41 -42 -43 -44 45

    -46 47 -48 -49 -50

    -51 -52 -53 -54 55

    56 -57 -58 -59 -60

    -61 -62 63 -64 -65

    -66 67 -68 -69 -70

    -71 -72 -73 74 -75

    -76 77 -78 -79 -80

    -81 -82 83 -84 -85

    86 -87 -88 -89 -90

    -91 -92 -93 94 -95

    -96 -97 -98 -99 100

    101 -102 -103 -104 -105

    -106 107 -108 -109 -110

    -111 -112 -113 -114 115

    -116 -117 -118 119 -120

    -121 -122 123 -124 -125



    ---------  PERFORMANCE  --------

    Number of times splitting rule applied: 1647

**Note:** The number for "splitting rule applied" will vary in each run as there is some randomness involved for the heuristics.

# Solution

This section describes the solution to the Einstein puzzle that was obtained using the DPLL SAT solver with both heuristics.

Using the assignments obtained (shown in the [Output](#output) section), we arrive at the solution to the puzzle:

| House 1   | House 2   | House 3   | House 4   | House 5       |
| ----------| ----------|---------  |---------- |---------------|
| Yellow    | Blue      | Red       | Green     | White         |
| Norwegian | Dane      | Brit      | German    | Swede         |
| Water     | Tea       | Milk      | Coffee    | Beer          |
| Dunhill   | Blends    | Pall Mall | Prince    | Bluemasters   |
| Cats      | Horse     | Birds     | Fish      | Dogs          |

