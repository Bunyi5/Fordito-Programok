// Fordítás: g++ lexikal.cpp -o lexikal
// Futtatás: ./lexikal
#include <iostream>
#include <cstring>
#include <ctype.h>

using namespace std;

bool isItOutOfBounds(int i, int arraySize)
{
    if (i >= arraySize)
    {
        return true;
    }
    return false;
}

bool isItEndOfLine(char c)
{
    if (c == '$')
    {
        return true;
    }
    return false;
}

bool isItAlphabetic(char c)
{
    if ((c >= 'a' && c <= 'z'))
    {
        return true;
    }
    return false;
}

int main()
{
    //Beolvasás
    cout << "Please enter the text:" << endl;
    string text;
    getline(cin, text);

    //Szöveg átalakítása karakter tömbbé
    char char_array[text.length()];
    strcpy(char_array, text.c_str());

    //Segéd változók deklarálása
    string result_string;
    bool error = false;
    string good_chars = "{(:<> $";

    //Karakter tömb végigjárása
    for (int i = 0; i < sizeof(char_array); i++)
    {

        //Kezdőállapot ismeretlen karakter hibakezelése
        if (!isItAlphabetic(char_array[i]) && !isdigit(char_array[i]) && (good_chars.find(char_array[i]) == string::npos))
        {
            cout << char_array[i] << endl;
            error = true;
            break;
        }

        //Betűt vagy számot kap
        if (isItAlphabetic(char_array[i]))
        {
            result_string.append("<azonosito>");

            while (isItAlphabetic(char_array[i]) || isdigit(char_array[i]))
            {
                if (isItOutOfBounds(i + 1, sizeof(char_array)))
                {
                    break;
                }
                i++;
            }
        }
        else if (isdigit(char_array[i]))
        {
            result_string.append("<konstans>");

            while (isdigit(char_array[i]))
            {
                if (isItOutOfBounds(i + 1, sizeof(char_array)))
                {
                    break;
                }
                i++;
            }
        }

        //Kapcsos zárójelet kap
        if (char_array[i] == '{')
        {
            int count = 0;
            result_string.append("<{}kommentar");

            if (isItOutOfBounds(i + 1, sizeof(char_array)))
            {
                error = true;
                break;
            }
            i++;

            while (!(char_array[i] == '}' && count == 0))
            {
                if (char_array[i] == '{') //Kapcsos zárójelen belül kapcsos zárójelet kap
                {
                    count++;
                    result_string.append("<{}kommentar" + to_string(count + 1));
                }
                else if (char_array[i] == '}')
                {
                    result_string.append(">");
                    count--;
                }

                if (isItOutOfBounds(i + 1, sizeof(char_array)) || isItEndOfLine(char_array[i]))
                {
                    error = true;
                    break;
                }
                i++;
            }

            if (error)
            {
                break;
            }
            result_string.append(">");
        }

        //Gömbölyű zárójelet kap
        if (char_array[i] == '(')
        {
            if (isItOutOfBounds(i + 1, sizeof(char_array)))
            {
                break;
            }
            i++;

            if (isItEndOfLine(char_array[i]))
            {
                error = true;
                break;
            }

            if (char_array[i] == '*')
            {

                int count = 0;
                result_string.append("<(**)kommentar");

                if (isItOutOfBounds(i + 1, sizeof(char_array)) || isItOutOfBounds(i + 2, sizeof(char_array)))
                {
                    error = true;
                    break;
                }
                i++;

                while (!(char_array[i] == '*' && char_array[i + 1] == ')') || count != 0)
                {
                    if (char_array[i] == '(' && char_array[i + 1] == '*') //Gömbölyű zárójelen belül gömbölyű zárójelet kap
                    {
                        count++;
                        result_string.append("<(**)kommentar" + to_string(count + 1));
                        i++;
                    }

                    if (isItOutOfBounds(i + 1, sizeof(char_array)))
                    {
                        error = true;
                        break;
                    }

                    if (char_array[i] == '*' && char_array[i + 1] == ')')
                    {
                        result_string.append(">");
                        count--;
                        i++;
                    }

                    if (isItOutOfBounds(i + 1, sizeof(char_array)) || isItOutOfBounds(i + 2, sizeof(char_array)) || isItEndOfLine(char_array[i]))
                    {
                        error = true;
                        break;
                    }
                    i++;
                }

                if (error)
                {
                    break;
                }
                i++;

                result_string.append(">");
            }
            else
            {
                i--;
            }
        }

        //Kettőspontot kap
        if (char_array[i] == ':')
        {
            int j = i + 1;

            if (isItOutOfBounds(j, sizeof(char_array)))
            {
                break;
            }

            if (isItEndOfLine(char_array[j]))
            {
                error = true;
                break;
            }

            if (char_array[j] == '=')
            {
                result_string.append("<ertekadas>");
                i++;
            }
        }

        //Kisebb jelet kap
        if (char_array[i] == '<')
        {
            int j = i + 1;

            if (isItOutOfBounds(j, sizeof(char_array)))
            {
                break;
            }

            if (isItEndOfLine(char_array[j]))
            {
                error = true;
                break;
            }

            if (char_array[j] == '=')
            {
                result_string.append("<kisebbegyenlo>");
                i++;
            }
            else if (char_array[j] == '>')
            {
                result_string.append("<nemegyenlo>");
                i++;
            }
        }
        else if (char_array[i] == '>') //Nagyobb jelet kap
        {
            int j = i + 1;

            if (isItOutOfBounds(j, sizeof(char_array)))
            {
                break;
            }

            if (isItEndOfLine(char_array[j]))
            {
                error = true;
                break;
            }

            if (char_array[j] == '=')
            {
                result_string.append("<nagyobbegyenlo>");
                i++;
            }
        }

        //Sorvége jelet kap
        if (isItEndOfLine(char_array[i]))
        {
            result_string.append("<vege>");
        }
    }

    if (error)
    {
        cout << "Error!" << endl;
    }
    else
    {
        cout << result_string << endl;
    }

    return 0;
}