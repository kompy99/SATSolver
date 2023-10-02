/*
Written by Varun Komperla
GT ID: xx3926404
GT Email: vkomperla3@gatech.edu
*/

#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <chrono>
#include <csignal>
#include <ctime>
#include <unordered_map>
#include <unordered_set>

enum HeuristicType
{
    RANDOM = 1,
    TWO_CLAUSE = 2,
    JW = 3
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
    int DPLLCalls;
    int clauseCount;
    int literalCount;

    void SolveSAT(std::vector<std::set<int>> clauses, int numLiterals)
    {
        std::vector<int> assignments, finalAssignments;

        DPLLCalls = 0;
        clauseCount = clauses.size();
        literalCount = numLiterals;

        std::chrono::_V2::system_clock::time_point start = std::chrono::high_resolution_clock::now();

        finalAssignments = DPLL(clauses, assignments, numLiterals);

        // Get the current time after executing the code
        std::chrono::_V2::system_clock::time_point end = std::chrono::high_resolution_clock::now();

        // Calculate the duration it took to execute the code
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::vector<bool> variableAssignments(numLiterals, false);

        if (!finalAssignments.empty())
        {
            // std::cout << std::endl
            //           << "SAT" << std::endl;

            // for (int i = 0; i < finalAssignments.size(); i++)
            // {
            //     variableAssignments[abs(finalAssignments[i]) - 1] = (finalAssignments[i] > 0);
            // }

            // for (int i = 0; i < variableAssignments.size(); i++)
            // {
            //     if (variableAssignments[i])
            //     {
            //         std::cout << " " << (i + 1);
            //     }
            //     else
            //     {
            //         std::cout << " " << -1 * (i + 1);
            //     }

            //     if ((i + 1) % 5 == 0)
            //     {
            //         std::cout << "\n\n";
            //     }
            // }

            std::time_t currentTime = std::time(0);

            // Convert the current time to a string
            std::string currentTimeString = std::ctime(&currentTime);

            std::cout << std::endl
                      << "1, " << DPLLCalls << ", " << duration << ", " << clauses.size() << ", " << numLiterals << ", " << currentTimeString << std::flush;
        }
        else
        {
            std::time_t currentTime = std::time(0);

            // Convert the current time to a string
            std::string currentTimeString = std::ctime(&currentTime);
            std::cout << std::endl
                      << "0, " << DPLLCalls << ", " << duration << ", " << clauses.size() << ", " << numLiterals << ", " << currentTimeString << std::flush;
        }
    }

private:
    HeuristicType heuristic;

    std::vector<int> DPLL(std::vector<std::set<int>> clauses, std::vector<int> assignments, int numLiterals)
    {
        DPLLCalls++;

        if (heuristic == HeuristicType::JW)
        {
            // PureLiteralElimination(clauses, assignments);
        }
        
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

        int propAssigned = SelectProp(clauses, assignments, numLiterals);

        // std::string newBranchPath = branchPath + " L";
        // int propAssigned = propSelected * truthValueAssignment;

        // std::cout << std::endl
        //          << "Branch path - " << newBranchPath << "\tAssigning prop = " << propAssigned << std::endl;

        assignments.push_back(propAssigned);
        std::vector<int> returnedAssignments = DPLL(AssignPropAndSimplifyClauses(clauses, propAssigned), assignments, numLiterals);

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
            propAssigned = propAssigned * -1;

            // std::cout << std::endl
            //          << "Branch path - " << newBranchPath << "\tAssigning prop = " << propAssigned << std::endl;
            assignments.push_back(propAssigned);
            return DPLL(AssignPropAndSimplifyClauses(clauses, propAssigned), assignments, numLiterals);
        }
    }

    int SelectRandomTruthValueAssignment()
    {
        int truthValues[] = {-1, 1};
        int randomIndex = GenerateRandomNumber(1, 100);
        return truthValues[randomIndex % 2];
    }

    int SelectProp(std::vector<std::set<int>> clauses, std::vector<int> assignments, int numLiterals)
    {
        if (heuristic == HeuristicType::RANDOM)
        {
            int truthValueAssignment = SelectRandomTruthValueAssignment();
            return truthValueAssignment * RandomDecisionHeuristic(numLiterals, assignments);
        }
        else if (heuristic == HeuristicType::TWO_CLAUSE)
        {
            int truthValueAssignment = SelectRandomTruthValueAssignment();
            int prop = TwoClauseHeuristic(clauses);
            if (!prop)
            {
                prop = RandomDecisionHeuristic(numLiterals, assignments);
            }
            return truthValueAssignment * prop;
        }
        else if (heuristic == HeuristicType::JW)
        {
            // int prop = JWHeuristic(clauses, numLiterals);
            int prop = JWTwoHeuristic(clauses, numLiterals);
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

    // Function to select literal using Jeroslow-Wang method
    int JWHeuristic(std::vector<std::set<int>> clauses, int numLiterals)
    {
        std::map<int, float> jwFunctionMap;

        for (std::set<int> clause : clauses)
        {
            int size = clause.size();

            for (auto iter = clause.begin(); iter != clause.end(); iter++)
            {
                float jwValue = pow(2, -1 * size);

                if (jwFunctionMap.count(*iter))
                {
                    jwFunctionMap[*iter] += jwValue;
                }
                else
                {
                    jwFunctionMap.insert({*iter, jwValue});
                }
            }
        }

        int selectedProp = 0;

        if (!jwFunctionMap.empty())
        {
            auto maxElement = jwFunctionMap.begin();
            for (auto iter = jwFunctionMap.begin(); iter != jwFunctionMap.end(); iter++)
            {
                if (iter->second > maxElement->second)
                {
                    maxElement = iter;
                }
            }

            for (auto iter = jwFunctionMap.begin(); iter != jwFunctionMap.end();)
            {
                if (iter->second < maxElement->second)
                {
                    iter = jwFunctionMap.erase(iter);
                }
                else
                {
                    iter++;
                }
            }

            int randomIndex = GenerateRandomNumber(0, jwFunctionMap.size() - 1);
            auto randomElement = next(jwFunctionMap.begin(), randomIndex);
            selectedProp = randomElement->first;
        }
        else
        {
            throw std::runtime_error("JW Function map is empty!!");
        }

        return selectedProp;
    }

    int JWTwoHeuristic(std::vector<std::set<int>> clauses, int numLiterals)
    {
        std::map<int, float> jwFunctionMap;

        for (std::set<int> clause : clauses)
        {
            int size = clause.size();
                
            for (auto iter = clause.begin(); iter != clause.end(); iter++)
            {
                float jwValue = pow(2, -1 * size);

                if(jwFunctionMap.count(*iter))
                {
                    jwFunctionMap[*iter] += jwValue;
                }
                else
                {
                    jwFunctionMap.insert({*iter, jwValue});
                }
            }
        }

        // Compute two sided jeroslow-wang function
        std::map<int, float> twoSidedJWFunctionMap;

        for (auto iter = jwFunctionMap.begin(); iter != jwFunctionMap.end(); iter++)
        {
            int literal = iter->first;
            float jwValue = iter->second;

            if (literal > 0)
            {
                if (jwFunctionMap.count(-literal))
                {
                    jwValue += jwFunctionMap[-literal];
                }
            }
            else
            {
                if (jwFunctionMap.count(-literal))
                {
                    jwValue += jwFunctionMap[-literal];
                }
            }

            twoSidedJWFunctionMap.insert({abs(literal), jwValue});
        }

        int selectedProp = 0;

        if (!twoSidedJWFunctionMap.empty())
        {
            auto maxElement = twoSidedJWFunctionMap.begin();
            for (auto iter = twoSidedJWFunctionMap.begin(); iter != twoSidedJWFunctionMap.end(); iter++)
            {
                if (iter->second > maxElement->second)
                {
                    maxElement = iter;
                }
            }

            for (auto iter = twoSidedJWFunctionMap.begin(); iter != twoSidedJWFunctionMap.end();)
            {
                if (iter->second < maxElement->second)
                {
                    iter = twoSidedJWFunctionMap.erase(iter);
                }
                else
                {
                    iter++;
                }
            }

            int randomIndex = GenerateRandomNumber(0, twoSidedJWFunctionMap.size() - 1);
            auto randomElement = next(twoSidedJWFunctionMap.begin(), randomIndex);
            
            if (jwFunctionMap[randomElement->first] >= jwFunctionMap[-1 * randomElement->first])
            {
                selectedProp = randomElement->first;
            }
            else
            {
                selectedProp = -1 * randomElement->first;
            }
            
        }
        else
        {
            throw std::runtime_error("Two-sided JW Function map is empty!!");
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

    void PureLiteralElimination(std::vector<std::set<int>> &clauses, std::vector<int> &assignments)
    {
        std::unordered_set<int> pureLiterals;

        // Count occurrences of literals in the clauses
        std::unordered_map<int, int> literalCounts;

        for (const auto &clause : clauses)
        {
            for (int literal : clause)
            {
                literalCounts[literal]++;
            }
        }

        for (const auto &pair : literalCounts)
        {
            int literal = pair.first;
            int count = pair.second;

            if (!(literalCounts[literal] && literalCounts[-literal]))
            {
                pureLiterals.insert(literal);
            }
        }

        for (int pureLiteral : pureLiterals)
        {
           // std::cout<<"Pure literal eliminated - "<<pureLiteral<<std::endl;
            assignments.push_back(pureLiteral);
            clauses = AssignPropAndSimplifyClauses(clauses, pureLiteral);
        }
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

std::vector<std::set<int>> GenerateRandom3Clauses(int numLiterals, int numClauses, int randomSeed)
{
    std::vector<std::set<int>> clauses(numClauses);
    int numLiteralsInClause = 3;

    std::mt19937 rng(randomSeed);
    std::uniform_int_distribution<int> literalDistribution(1, numLiterals);
    std::uniform_int_distribution<int> equalDistribution(1, 100);

    for (int i = 0; i < numClauses; i++)
    {
        std::set<int> tempClause;
        std::set<int> clause;

        while (tempClause.size() < numLiteralsInClause)
        {
            int randomLiteral = literalDistribution(rng);
            // randomLiteral = (equalDistribution(rng) % 2 == 0) ? randomLiteral : -1 * randomLiteral;
            tempClause.insert(randomLiteral);
        }

        for (auto iter = tempClause.begin(); iter != tempClause.end(); iter++)
        {
            int literal = (equalDistribution(rng) % 2 == 0) ? *iter : -1 * (*iter);
            clause.insert(literal);
        }

        clauses[i] = clause;
    }

    return clauses;
}

void PrintClauses(std::vector<std::set<int>> clauses)
{
    std::cout << std::endl;
    for (std::set<int> clause : clauses)
    {
        for (int literal : clause)
        {
            std::cout << literal << " ";
        }

        std::cout << std::endl;
    }
}

NaiveDPLLSolver *globalObjectPtr = nullptr;
int64_t globalTimeoutDuration;

void handleSIGTERM(int signum)
{

    if (globalObjectPtr != nullptr)
    {
        std::cout << std::endl
                  << "2, " << globalObjectPtr->DPLLCalls << ", " << globalTimeoutDuration * 1000 << ", " << globalObjectPtr->clauseCount << ", " << globalObjectPtr->literalCount << std::flush;
    }
    exit(signum);
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, handleSIGTERM);

    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0] << " <iteration_number> <heuristic> <number_of_variables> <ratio> <timeout_duration>" << std::endl;
        return 1;
    }

    int iterationNumber = std::atoi(argv[1]);
    HeuristicType heuristic = (HeuristicType)std::atoi(argv[2]);
    int numVariables = std::atoi(argv[3]);
    float LNratio = std::atof(argv[4]);
    globalTimeoutDuration = std::atoi(argv[5]);

    // int numIterations = 1;
    // int numVariables = 4;
    // HeuristicType heuristic = (HeuristicType) 2;
    // float LNratio = 2;

    std::mt19937 rng(iterationNumber * numVariables * LNratio);
    std::uniform_int_distribution<int> seedGenerator(1, numVariables * iterationNumber);

    int numClauses = (int)(LNratio * numVariables);
    std::vector<std::set<int>> clauses = GenerateRandom3Clauses(numVariables, numClauses, seedGenerator(rng));
    // Printing clauses
    // PrintClauses(clauses);
    NaiveDPLLSolver randomSolver(heuristic);
    globalObjectPtr = &randomSolver;
    randomSolver.SolveSAT(clauses, numVariables);
    return 0;
}