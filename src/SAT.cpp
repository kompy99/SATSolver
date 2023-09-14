#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>

enum HeuristicType
{
    RANDOM = 1,
    TWO_CLAUSE = 2
};

std::string toLowerString(std::string text)
{
    for (char &c : text)
    {
        c = std::tolower(c);
    }

    return text;
}

class SATSolver
{
public:
    virtual void SolveSAT(std::vector<std::set<int>> clauses, int numLiterals) = 0;
};

class NaiveDPLLSolver : public SATSolver
{
public:
    NaiveDPLLSolver(HeuristicType selectedHeuristic)
    {
        heuristic = selectedHeuristic;
    }

    void SolveSAT(std::vector<std::set<int>> clauses, int numLiterals)
    {
        std::vector<int> assignments, finalAssignments;

        DPLLCalls = 0;
        finalAssignments = DPLL(clauses, assignments, numLiterals);
        std::vector<bool> variableAssignments(numLiterals, false);

        if (!finalAssignments.empty())
        {
            std::cout << std::endl
                      << "SAT" << std::endl;

            for (int i = 0; i < finalAssignments.size(); i++)
            {
                variableAssignments[abs(finalAssignments[i]) - 1] = (finalAssignments[i] > 0);
            }

            for (int i = 0; i < variableAssignments.size(); i++)
            {
                if (variableAssignments[i])
                {
                    std::cout << " " << (i + 1);
                }
                else
                {
                    std::cout << " " << -1 * (i + 1);
                }

                if ((i + 1) % 5 == 0)
                {
                    std::cout << "\n\n";
                }
            }
        }
        else
        {
            std::cout << std::endl
                      << "UNSAT";
        }

        std::cout << "\n\n---------  PERFORMANCE  --------\n\n";
        std::cout << "Number of times splitting rule applied: " << DPLLCalls;
    }

private:
    HeuristicType heuristic;
    int DPLLCalls;

    std::vector<int> DPLL(std::vector<std::set<int>> clauses, std::vector<int> assignments, int numLiterals)
    {
        DPLLCalls++;

        UnitClausePropagation(clauses, assignments);

        if (clauses.empty())
        {
            // std::cout << std::endl
            //          << "Branch path - " << branchPath << "\tSAT!!!";
            return assignments;
        }

        if (ContainsEmptyClause(clauses))
        {
            return std::vector<int>();
        }

        int propSelected = SelectProp(clauses, assignments, numLiterals);
        int truthValueAssignment = SelectRandomTruthValueAssignment();

        // std::string newBranchPath = branchPath + " L";
        int propAssigned = propSelected * truthValueAssignment;

        // std::cout << std::endl
        //          << "Branch path - " << newBranchPath << "\tAssigning prop = " << propAssigned << std::endl;

        assignments.push_back(propAssigned);
        std::vector<int> returnedAssignments = DPLL(AssignPropAndSimplifyClauses(clauses, propSelected * truthValueAssignment), assignments, numLiterals);

        if (!returnedAssignments.empty())
        {
            return returnedAssignments;
        }
        else
        {

            // std::cout << std::endl
            //          << "Branch path - " << newBranchPath << "\tConflict! Last assigned prop = " << propSelected * truthValueAssignment << std::endl;

            // newBranchPath = branchPath + " R";
            assignments.pop_back();
            int propAssigned = propSelected * truthValueAssignment * -1;

            // std::cout << std::endl
            //          << "Branch path - " << newBranchPath << "\tAssigning prop = " << propAssigned << std::endl;
            assignments.push_back(propAssigned);
            return DPLL(AssignPropAndSimplifyClauses(clauses, propAssigned), assignments, numLiterals);
        }
    }

    int SelectRandomTruthValueAssignment()
    {
        int truthValues[] = {-1, 1};
        int randomIndex = GenerateRandomNumber(0, 1);
        return truthValues[randomIndex];
    }

    int SelectProp(std::vector<std::set<int>> clauses, std::vector<int> assignments, int numLiterals)
    {
        if (heuristic == HeuristicType::RANDOM)
        {
            return RandomDecisionHeuristic(numLiterals, assignments);
        }
        else if (heuristic == HeuristicType::TWO_CLAUSE)
        {
            int prop = TwoClauseHeuristic(clauses);
            if (!prop)
            {
                prop = RandomDecisionHeuristic(numLiterals, assignments);
            }
            return prop;
        }
        else
        {
            throw std::runtime_error("Invalid Heuristic!!");
        }
    }

    int RandomDecisionHeuristic(int numLiterals, std::vector<int> assignments)
    {
        std::set<int> unassignedPropList;
        for (int i = 1; i <= numLiterals; i++)
        {
            unassignedPropList.insert(i);
        }

        for (int assignment : assignments)
        {
            unassignedPropList.erase(abs(assignment));
        }

        int randomIndex = GenerateRandomNumber(0, unassignedPropList.size() - 1);
        auto randomElement = next(unassignedPropList.begin(), randomIndex);
        int selectedProp = *randomElement;
        return selectedProp;
    }

    int TwoClauseHeuristic(std::vector<std::set<int>> clauses)
    {
        std::map<int, int> twoLiteralCount;

        for (std::set<int> clause : clauses)
        {
            if (clause.size() == 2)
            {
                for (auto iter = clause.begin(); iter != clause.end(); iter++)
                {
                    // std::cout << "\nChecking - " << (*iter) << "\n";

                    if (!twoLiteralCount.count(abs(*iter)))
                    {
                        twoLiteralCount.insert({abs(*iter), 1});
                    }
                    else
                    {
                        twoLiteralCount[abs(*iter)]++;
                    }
                }
            }
        }

        int selectedProp = 0;

        if (!twoLiteralCount.empty())
        {
            auto maxElement = twoLiteralCount.begin();
            for (auto iter = twoLiteralCount.begin(); iter != twoLiteralCount.end(); iter++)
            {
                if (iter->second > maxElement->second)
                {
                    maxElement = iter;
                }
            }

            // std::cout << "\nFound max(literal, count) = " << maxElement->first << ", " << maxElement->second << std::endl;

            for (auto iter = twoLiteralCount.begin(); iter != twoLiteralCount.end();)
            {
                if (iter->second < maxElement->second)
                {
                    // std::cout << "\nErasing (literal, count) = " << iter->first << ", " << iter->second << std::endl;
                    iter = twoLiteralCount.erase(iter);
                }
                else
                {
                    iter++;
                }
            }

            int randomIndex = GenerateRandomNumber(0, twoLiteralCount.size() - 1);
            auto randomElement = next(twoLiteralCount.begin(), randomIndex);
            selectedProp = randomElement->first;
        }

        return selectedProp;
    }

    int GenerateRandomNumber(int min, int max)
    {
        std::random_device randomDevice;
        std::mt19937 engine(randomDevice());
        std::uniform_int_distribution<> distribution(min, max);

        int randomIndex = distribution(engine);

        return randomIndex;
    }

    void UnitClausePropagation(std::vector<std::set<int>> &clauses, std::vector<int> &assignments)
    {
        bool unitClauseFound = true;

        while (unitClauseFound)
        {
            unitClauseFound = false;

            for (int i = 0; i < clauses.size(); i++)
            {
                if (clauses[i].size() == 1)
                {
                    unitClauseFound = true;
                    int unitProposition = *(clauses[i].begin());
                    assignments.push_back(unitProposition);
                    clauses = AssignPropAndSimplifyClauses(clauses, unitProposition);
                    break;
                }
            }
        }
    }

    std::vector<std::set<int>> AssignPropAndSimplifyClauses(std::vector<std::set<int>> clauses, int assignedProposition)
    {
        std::vector<std::set<int>>::iterator iter;

        for (iter = clauses.begin(); iter != clauses.end();)
        {
            if (iter->find(assignedProposition) != iter->end())
            {
                iter = clauses.erase(iter);
            }
            else
            {
                int assignedPropositionComplement = -1 * assignedProposition;
                auto propComplementIndex = iter->find(assignedPropositionComplement);

                if (propComplementIndex != iter->end())
                {
                    iter->erase(propComplementIndex);
                }

                iter++;
            }
        }

        return clauses;
    }

    bool ContainsEmptyClause(std::vector<std::set<int>> &clauses)
    {
        for (std::set<int> clause : clauses)
        {
            if (clause.empty())
            {
                return true;
            }
        }

        return false;
    }
};

int main()
{
    std::ifstream fin;
    std::string filename;
    std::cout << "Enter name of DIMACS file : " << std::endl;
    std::cin >> filename;


    fin.open(filename);
    
    if (!fin)
    {
        std::cout<<"\nFile open error!";
        exit(1);
    }

    std::string word1;
   
    fin >> word1;
    
    while (word1 != "p")
    {
        fin >> word1;
    }

    std::string problemFormat;

    fin>>problemFormat;

    if (toLowerString(problemFormat) != "cnf")
    {
      std::cout<<"\nInvalid problem format!!";
      exit(1);   
    }

    int literals;
    int rows;
    
    fin >> literals;
    fin >> rows;
    std::cout << "\n\nNo of propositions - " << literals << std::endl;
    std::cout << "No of clauses - " << rows << std::endl;

    std::vector<std::set<int>> clauses(rows);

    int num;
    for (int i = 0; i < rows; i++)
    {
        fin >> num;
        while (num != 0)
        {
            clauses[i].insert(num);
            fin >> num;
        }
    }

    int userInputHeuristic;
    std::cout << "\n\nSelect heuristic (1 - Random, 2 - Two-Clause):\n";
    std::cin >> userInputHeuristic;

    switch (userInputHeuristic)
    {
    case HeuristicType::RANDOM:
    {
        NaiveDPLLSolver randomSolver(HeuristicType::RANDOM);
        randomSolver.SolveSAT(clauses, literals);
        std::cout << std::endl << std::endl;
        break;
    }

    case HeuristicType::TWO_CLAUSE:
    {
        NaiveDPLLSolver twoClauseSolver(HeuristicType::TWO_CLAUSE);
        twoClauseSolver.SolveSAT(clauses, literals);
        std::cout << std::endl
             << std::endl;
        break;
    }

    default:
        std::cout << "Invalid Heuristic!! Try again!";
        exit(1);
    }

    return 0;
}