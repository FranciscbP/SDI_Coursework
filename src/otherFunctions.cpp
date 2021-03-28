#include "headers/otherFunctions.h"

//Convert Const Char to Int
int convertCharToInt(char *chr)
{
    string str(chr);
    int num = stoi(str);

    return num;
}
