#include <stdbool.h>
#include <stdio.h>

char *append_str(char *strA, char *strB)
{
    int i = 0;
    for (i = 0 ; strB[i]!=0 ; i++)
        strA[i] = strB[i];
    return strA + i;
}

char *append_hex(char *str, char tmp)
{
	static const char hex[] = "0123456789ABCDEF";
	str[0] = hex[(tmp >> 4) & 0xF];
	str[1] = hex[(tmp >> 0) & 0xF];
    return str + 2;
}

char *append_digit(char *str, int val, char padCount)
{
    int i = 0;
    int unsigned exponent = 1;
    bool padding = false;
    int charCount = 0;
    for (i = 0 ; i < padCount ; i++)
    {
        exponent *= 10;
    }
    while (exponent != 0)
    {
	    int digit = 0;
        while (val >= exponent)
        {
            digit++;
            val -= exponent;
        }
        if (digit != 0)
            padding = true;
        if (digit != 0 || padding == true || exponent == 1)
        {
            str[charCount] = digit + 0x30;
            charCount++;
        }
        exponent /= 10;
    }
    return str + charCount;
}

int main(void)
{
    char str[1000]={0};
    char *strPtr = str;
    strPtr = append_hex(strPtr, '4');
    strPtr = append_str(strPtr, " hello");
    strPtr = append_str(strPtr, " world ");
    strPtr = append_digit(strPtr, 3456, 7);
    strPtr = append_str(strPtr, "B");
    printf(str);
}