#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

//Szabályok konfigurálása
vector<pair<char, pair<int, string>>> rules = {
    {'S', {1, "aS"}},
    {'S', {2, "bAc"}},
    {'A', {3, "bAc"}},
    {'A', {4, "d"}},
};

//Illeszkedő szabály keresése
pair<int, string> getARule(char searchFirst, char buildFirst)
{
    pair<int, string> result;

    for (vector<pair<char, pair<int, string>>>::iterator it = rules.begin(); it != rules.end(); it++)
    {
        if (it->first == buildFirst && it->second.second[0] == searchFirst)
        {
            result = it->second;
        }
    }
    return result;
}

bool isItNoneTerminal(char c)
{
    if ((c >= 'A' && c <= 'Z'))
    {
        return true;
    }
    return false;
}

//Egy nem terminálisra az összes szabály
vector<string> getSameRulesFirst(char noneTerminal)
{
    vector<string> result;

    for (vector<pair<char, pair<int, string>>>::iterator it = rules.begin(); it != rules.end(); it++)
    {
        if (it->first == noneTerminal)
        {
            string rule(1, it->second.second[0]);
            result.push_back(rule);
        }
    }
    return result;
}

//Szabályok helyességének ellenőrzése
void checkRules(string *error)
{
    set<char> differentNoneTerminals;

    for (vector<pair<char, pair<int, string>>>::iterator it = rules.begin(); it != rules.end(); it++)
    {
        if (isItNoneTerminal(it->second.second[0]))
        {
            *error = "Minden helyettesitesi szabaly jobboldala terminalis betuvel kell kezdodjon!";
            break;
        }
        differentNoneTerminals.insert(it->first);
    }

    for (set<char>::iterator sit = differentNoneTerminals.begin(); sit != differentNoneTerminals.end(); sit++)
    {

        vector<string> sameNoneTerminalRules = getSameRulesFirst(*sit);

        for (vector<string>::iterator kl = sameNoneTerminalRules.begin(); kl != sameNoneTerminalRules.end(); kl++)
        {
            vector<string> sameNoneTerminalRulesMinusKL = sameNoneTerminalRules;
            sameNoneTerminalRulesMinusKL.erase(find(sameNoneTerminalRulesMinusKL.begin(), sameNoneTerminalRulesMinusKL.end(), *kl));

            for (vector<string>::iterator lj = sameNoneTerminalRulesMinusKL.begin(); lj != sameNoneTerminalRulesMinusKL.end(); lj++)
            {
                if (*kl == *lj)
                {
                    *error = "Minden A nemterminalis minden egymastol kulonbozo A -> Beta1 , A -> Beta2 helyettesitesi szabalyara Beta1 es Beta2 elso karaktere kulonbozo kell legyen!";
                }
            }
        }
    }
}

int main()
{
    string searchString = "aabbdcc#";

    bool success = false;
    string error = "";
    string build = "S#";
    string result;

    checkRules(&error);

    while (!success && error == "")
    {
        if (searchString[0] == '#' && build[0] == '#') //Végállapotot találtunk
        {
            success = true;
        }
        else if (searchString[0] == build[0]) //Pop egy terminálisra
        {
            searchString.erase(searchString.begin());
            build.erase(build.begin());
        }
        else //Nem terminális kicserélése szabályra
        {
            pair<int, string> rule = getARule(searchString[0], build[0]);

            if (rule.first == 0 && rule.second == "")
            {
                break;
            }

            build.replace(0, 1, rule.second);
            result.append("(" + to_string(rule.first) + ")");
        }
    }

    //Eredmény kiírása
    if (success)
    {
        cout << "A szo eleme a nyelvnek!" << endl;
        cout << "Eredmeny: " << result << endl;
    }
    else if (error != "")
    {
        cout << error << endl;
    }
    else
    {
        cout << "A szo nem eleme a nyelvnek!!" << endl;
    }

    return 0;
}