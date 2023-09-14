#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using namespace std;

// --------------------- Red, White, Green, Yellow, Blue

vector<string> ColorKeys{"clr_r", "clr_w", "clr_g", "clr_y", "clr_b"};

// --------------------- Brit, Swede, Dane, Norwegian, German

vector<string> NationalityKeys{"ntn_B", "ntn_S", "ntn_D", "ntn_N", "ntn_G"};

// --------------------- Tea, Coffee, Milk, Beer, Water

vector<string> BeverageKeys{"bvg_t", "bvg_c", "bvg_m", "bvg_b", "bvg_w"};

// --------------------- Pall Mall, Dunhill, Blends, Prince, Bluemasters

vector<string> CigarKeys{"cig_p", "cig_d", "cig_b", "cig_c", "cig_s"};

// --------------------- Cats, Horse, Dogs, Fish, Birds

vector<string> PetKeys{"pet_c", "pet_h", "pet_d", "pet_f", "pet_b"};

const int numHouses = 5;

void PrintClause(vector<int> clause)
{
    cout << endl;

    for (int i = 0; i < clause.size(); i++)
    {
        cout << clause[i] << " ";
    }

    cout<<"0";
}

vector<vector<int>> BaseContraintsGeneration(vector<string> propertyKeys, map<pair<int, string>, int> literalMap)
{
    vector<vector<int>> clausesList;

    vector<vector<int>> houseHasAtLeastOneProp;
    vector<vector<vector<int>>> houseHasAtMostOneProp;

    // Building constraints for each house, each house should have at least one and at most one property.

    for (int houseNumber = 1; houseNumber <= numHouses; houseNumber++)
    {
        vector<int> houseHasAtLeastOnePropTempClause;

        for (int propIndex = 0; propIndex < propertyKeys.size(); propIndex++)
        {

            int literal_HousePropPair1 = literalMap[{houseNumber, propertyKeys[propIndex]}];
            houseHasAtLeastOnePropTempClause.push_back(literal_HousePropPair1);

            for (int propIndex2 = propIndex + 1; propIndex2 < propertyKeys.size(); propIndex2++)
            {
                int literal_HousePropPair2 = literalMap[{houseNumber, propertyKeys[propIndex2]}];

                vector<int> houseHasAtMostOnePropTempClause;
                houseHasAtMostOnePropTempClause.push_back(-1 * literal_HousePropPair1);
                houseHasAtMostOnePropTempClause.push_back(-1 * literal_HousePropPair2);

                PrintClause(houseHasAtMostOnePropTempClause);
                clausesList.push_back(houseHasAtMostOnePropTempClause);
            }
        }

        PrintClause(houseHasAtLeastOnePropTempClause);
        clausesList.push_back(houseHasAtLeastOnePropTempClause);
    }

    // Building constraints for each property value, where property applies to at least one house and at most one house

    vector<vector<int>> atLeastOneHouseHasProp;
    vector<vector<vector<int>>> atMostOneHouseHasProp;

    for (int propertyIndex = 0; propertyIndex < propertyKeys.size(); propertyIndex++)
    {
        vector<int> atLeastOneHouseHasPropTempClause;

        for (int houseIndex = 1; houseIndex <= numHouses; houseIndex++)
        {
            int literal_HousePropPair1 = literalMap[{houseIndex, propertyKeys[propertyIndex]}];

            atLeastOneHouseHasPropTempClause.push_back(literal_HousePropPair1);

            for (int houseIndex2 = houseIndex + 1; houseIndex2 <= numHouses; houseIndex2++)
            {

                int literal_HousePropPair2 = literalMap[{houseIndex2, propertyKeys[propertyIndex]}];

                vector<int> atMostOneHouseHasPropTempClause;
                atMostOneHouseHasPropTempClause.push_back(-1 * literal_HousePropPair1);
                atMostOneHouseHasPropTempClause.push_back(-1 * literal_HousePropPair2);

                PrintClause(atMostOneHouseHasPropTempClause);
                clausesList.push_back(atMostOneHouseHasPropTempClause);
            }
        }

        PrintClause(atLeastOneHouseHasPropTempClause);
        clausesList.push_back(atLeastOneHouseHasPropTempClause);
    }

    return clausesList;
}

vector<vector<int>> GenerateNeighbourConstraints(string propertyKey1, string propertyKey2, map<pair<int, string>, int> literalMap)
{

    vector<vector<int>> clauses;

    for (int houseNum1 = 1; houseNum1 <= numHouses; houseNum1++)
    {
        for (int houseNum2 = houseNum1 + 2; houseNum2 <= numHouses; houseNum2++)
        {
            vector<int> clauseHolder1;
            vector<int> clauseHolder2;

            clauseHolder1.push_back(-1 * literalMap[{houseNum1, propertyKey1}]);
            clauseHolder1.push_back(-1 * literalMap[{houseNum2, propertyKey2}]);

            clauseHolder2.push_back(-1 * literalMap[{houseNum1, propertyKey2}]);
            clauseHolder2.push_back(-1 * literalMap[{houseNum2, propertyKey1}]);

            PrintClause(clauseHolder1);
            PrintClause(clauseHolder2);
            clauses.push_back(clauseHolder1);
            clauses.push_back(clauseHolder2);
        }

        vector<int> clauseHolderNotSameHouse;
        clauseHolderNotSameHouse.push_back(-1 * literalMap[{houseNum1, propertyKey1}]);
        clauseHolderNotSameHouse.push_back(-1 * literalMap[{houseNum1, propertyKey2}]);

        PrintClause(clauseHolderNotSameHouse);
        clauses.push_back(clauseHolderNotSameHouse);
    }

    return clauses;
}

vector<vector<int>> GenerateImplicationConstraints(string propertyKey1, string propertyKey2, map<pair<int, string>, int> literalMap)
{
    vector<vector<int>> clauses;

    for (int houseNum1 = 1; houseNum1 < numHouses; houseNum1++)
    {
        for (int houseNum2 = houseNum1 + 1; houseNum2 <= numHouses; houseNum2++)
        {
            if (houseNum1 != houseNum2)
            {
                vector<int> clauseHolder1;
                vector<int> clauseHolder2;

                clauseHolder1.push_back(-1 * literalMap[{houseNum1, propertyKey1}]);
                clauseHolder1.push_back(-1 * literalMap[{houseNum2, propertyKey2}]);

                clauseHolder2.push_back(-1 * literalMap[{houseNum1, propertyKey2}]);
                clauseHolder2.push_back(-1 * literalMap[{houseNum2, propertyKey1}]);

                PrintClause(clauseHolder1);
                PrintClause(clauseHolder2);
                clauses.push_back(clauseHolder1);
                clauses.push_back(clauseHolder2);
            }
        }
    }

    return clauses;
}

vector<vector<int>> GenerateFactConstraints(int houseNum, string propertyKey, map<pair<int, string>, int> literalMap)
{
    vector<vector<int>> clauses;

    vector<int> clauseHolder;

    clauseHolder.push_back(literalMap[{houseNum, propertyKey}]);

    clauses.push_back(clauseHolder);

    PrintClause(clauseHolder);

    return clauses;
}

vector<vector<int>> GenerateLeftOfConstraint(string propertyKeyLeft, string propertyKeyRight, map<pair<int, string>, int> literalMap)
{
    vector<vector<int>> clauses;

    vector<int> clauseHolder1;

    clauseHolder1.push_back(-1 * literalMap[{5, propertyKeyLeft}]);

    clauses.push_back(clauseHolder1);

    for (int houseNum1 = 1; houseNum1 <= numHouses - 1; houseNum1++)
    {
        for (int houseNum2 = 1; houseNum2 <= numHouses; houseNum2++)
        {
            if (houseNum2 != (houseNum1 + 1) && houseNum2 != houseNum1)
            {
                vector<int> clauseHolder2;

                clauseHolder2.push_back(-1 * literalMap[{houseNum1, propertyKeyLeft}]);
                clauseHolder2.push_back(-1 * literalMap[{houseNum2, propertyKeyRight}]);

                PrintClause(clauseHolder2);
                clauses.push_back(clauseHolder2);
            }
        }
    }

    return clauses;
}

int main()
{
    vector<vector<string>> propertyKeys{
        ColorKeys,
        NationalityKeys,
        BeverageKeys,
        CigarKeys,
        PetKeys};

    vector<string> propertyNames{"Color", "Nationality", "Beverage", "Cigar", "Pet"};

    map<pair<int, string>, int> literalMap;

    vector<vector<int>> AllClauses;

    int variable = 1;

    cout<< "\nGenerating variables...";
    for (int property = 0; property < propertyKeys.size(); property++)
    {
        cout << "\n\nVariables for House - " << propertyNames[property] << "\n\n";
        for (int houseNum = 1; houseNum <= numHouses; houseNum++)
        {
            for (int propertyValueIndex = 0; propertyValueIndex < propertyKeys[property].size(); propertyValueIndex++)
            {
                cout << "(" << houseNum << ", " << propertyKeys[property][propertyValueIndex] << ") - " << variable << "\t ";
                literalMap.insert({{houseNum, propertyKeys[property][propertyValueIndex]}, variable});
                variable++;
            }

            cout << endl;
        }
    }

    for (int property = 0; property < propertyKeys.size(); property++)
    {
        cout<<"\n\nCreating base constraints for House-"<<propertyNames[property]<<"...\n\n";
        vector<vector<int>> baseClauses = BaseContraintsGeneration(propertyKeys[property], literalMap);
        AllClauses.insert(AllClauses.end(), begin(baseClauses), end(baseClauses));
    }

    vector<vector<int>> clauses;

    cout<<"\n\nCreating implication constraints for Brit and Red...\n\n";
    clauses = GenerateImplicationConstraints(NationalityKeys[0], ColorKeys[0], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating implication constraints for Swede and Dogs...\n\n";
    clauses = GenerateImplicationConstraints(NationalityKeys[1], PetKeys[2], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating implication constraints for Dane and Tea...\n\n";
    clauses = GenerateImplicationConstraints(NationalityKeys[2], BeverageKeys[0], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating implication constraints for Green and Coffee...\n\n";
    clauses = GenerateImplicationConstraints(ColorKeys[2], BeverageKeys[1], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating implication constraints for Pall Mall and Birds...\n\n";
    clauses = GenerateImplicationConstraints(CigarKeys[0], PetKeys[4], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating implication constraints for Yellow and Dunhill...\n\n";
    clauses = GenerateImplicationConstraints(ColorKeys[3], CigarKeys[1], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating neighbour constraints for Blends and Cats...\n\n";
    clauses = GenerateNeighbourConstraints(CigarKeys[2], PetKeys[0], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating neighbour constraints for Horse and Dunhill...\n\n";
    clauses = GenerateNeighbourConstraints(PetKeys[1], CigarKeys[1], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating implication constraints for Bluemasters and Beer...\n\n";
    clauses = GenerateImplicationConstraints(CigarKeys[4], BeverageKeys[3], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating implication constraints for German and Prince...\n\n";
    clauses = GenerateImplicationConstraints(NationalityKeys[4], CigarKeys[3], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating neighbour constraints for Norwegian and Bluehouse...\n\n";
    clauses = GenerateNeighbourConstraints(NationalityKeys[3], ColorKeys[4], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating neighbour constraints for Blends and Water...\n\n";
    clauses = GenerateNeighbourConstraints(CigarKeys[2], BeverageKeys[4], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating fact constraints for Milk and Center house...\n\n";
    clauses = GenerateFactConstraints(3, BeverageKeys[2], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating fact constraints for Norwegian in Blue house...\n\n";
    clauses = GenerateFactConstraints(1, NationalityKeys[3], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout<<"\n\nCreating constraints for Green to the left of White...\n\n";
    clauses = GenerateLeftOfConstraint(ColorKeys[2], ColorKeys[1], literalMap);
    AllClauses.insert(AllClauses.end(), begin(clauses), end(clauses));

    cout << "\n\nPrinting all clauses \n\n";

    cout << "p cnf 125 " << AllClauses.size();

    // for (int i = 0; i < AllClauses.size(); i++)
    // {
    //     cout << "\n";
    //     for (int j = 0; j < AllClauses[i].size(); j++)
    //     {
    //         cout << AllClauses[i][j] << " ";
    //     }
    //     cout << "0";
    // }
}