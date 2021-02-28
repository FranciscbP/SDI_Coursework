#include "headers/otherFunctions.h"


int convertCharToInt(char *chr)
{
    string str(chr);
    int num = stoi(str);

    return num;
}
