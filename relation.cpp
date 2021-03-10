#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

//Szabályok konfigurálása
map<string, string> rules = {
    {"S1", "T+S"},
    {"S2", "T"},
    {"T1", "a"},
    {"T2", "b"},
};

//String keresése a szabályokban
vector<pair<string, string>> getOneTypeRule(string search)
{
    vector<pair<string, string>> result;

    for (map<string, string>::iterator it = rules.begin(); it != rules.end(); it++)
    {
        if (it->first.find(search) != string::npos)
        {
            pair<string, string> row;
            row.first = it->first;
            row.second = it->second;
            result.push_back(row);
        }
    }
    return result;
}

int main()
{
    const string searchString = "b+a";
    vector<string> noneTerminals = {"S", "T"};

    bool success = false;
    int iterator = 0;
    vector<string> result;
    string build = "S";

    while (!success)
    {
        string buildFirst(1, build[0]);
        string searchChar(1, searchString[iterator]);

        vector<pair<string, string>> searchedRules = getOneTypeRule(buildFirst);

        if (searchChar == buildFirst) //Sikeres input illesztés
        {
            result.push_back(buildFirst);
            build.erase(0, 1);
            iterator++;
            if (iterator == searchString.length() && build.empty())
            {
                success = true;
                break;
            }
        }
        else if (searchedRules.size() > 0) //Nem terminális kiterjesztése
        {
            result.push_back(searchedRules[0].first);
            build.replace(build.find(buildFirst), buildFirst.length(), searchedRules[0].second);
        }
        else //Sikertelen input illesztés
        {
            while (!result.empty())
            {
                string resultLast = result[result.size() - 1];
                string lastLetter = resultLast.substr(0, resultLast.length() - 1);

                if (find(noneTerminals.begin(), noneTerminals.end(), lastLetter) != noneTerminals.end()) //Nem terminális esete
                {
                    int lastNumber = resultLast[resultLast.length() - 1] - '0';

                    vector<pair<string, string>> searchedBackRules = getOneTypeRule(lastLetter);

                    if (searchedBackRules.size() > lastNumber) //Nem terminális alternatív kiterjesztése
                    {
                        result[result.size() - 1] = searchedBackRules[lastNumber].first;
                        build.replace(build.find(searchedBackRules[lastNumber - 1].second),
                                      searchedBackRules[lastNumber - 1].second.length(),
                                      searchedBackRules[lastNumber].second);
                        break;
                    }
                    else //Visszalépés előző állapotra ha a nem terminális alternatív kiterjesztése elfogyott
                    {
                        result.erase(result.end() - 1);
                        build.replace(build.find(searchedBackRules[lastNumber - 1].second),
                                      searchedBackRules[lastNumber - 1].second.length(),
                                      lastLetter);
                    }
                }
                else //Terminális esete
                {
                    result.erase(result.end() - 1);
                    build.insert(0, resultLast);
                    iterator--;
                }
            }

            if (result.empty()) //Hiba nem lehetséges a végkonfiguráció
            {
                break;
            }
        }
    }

    //Eredmény kiírása
    if (success)
    {
        string resultString;
        for_each(result.begin(), result.end(), [&](const std::string &piece) { resultString += piece; });
        cout << "Sikeres elemzes!" << endl;
        cout << "Eredmeny: " << resultString << endl;
    }
    else
    {
        cout << "Sikertelen elemzes!" << endl;
        cout << "Nincs végkonfiguráció!" << endl;
    }
}