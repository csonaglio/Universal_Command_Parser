//*********************************************************************************************************************
// Universal Command Parser - UCP
// File: UCPFunctions.c
// 2022 - Claudio Sonaglio
//*********************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "UCPParser.h"
#include "UCPFunctions.h"

//*********************************************************************************************************************
// UCP High Level Function calls
// *params - text parameters
// *path - parsing path
//*********************************************************************************************************************
void UCPFunction (UCPchar *params, unsigned char *path)
{
    unsigned char lin = (unsigned char)(path[path[0]]);         // Gets current line into the table
    UCPchar* string = UCPParseTable[lin].txt;                   // Points to the last step to find params still to be read
    unsigned char size = 1;
    unsigned char col=0;
    unsigned int value = 0;

    if (0x00 == (unsigned char)(*params))                        // String ended... nothing else to parse...
    {
        printf ("Function running... Line:(%d) - Params:(%s)\n", lin, (char *)params);
    }
    else
    {
        if (UCPSQEND == string[strlen(string)-1])
        {
            switch (lin)
            {
            case 8:     case 9:     case 10:    case 11:    case 12:
            case 14:    case 15:    case 16:    case 17:    case 18:
            case 19:    case 20:    case 21:    case 22:    case 24:
            case 25:    case 26:    case 27:    case 28:    case 29:
            case 30:    case 31:    case 32:    case 33:    case 34:
            case 35:    case 36:    case 37:    case 38:    case 40:
                if (false == UCPHex(params,&size,0x0,0x0,&col,(unsigned char*)(&value)))
                {
                    printf ("Parameter error!!!");
                }
                else
                {
                    printf ("Function returned value: %d\n", value);
                }
            break;
            default:
                printf ("Unexpected error!!!");
            break;
            }
        }
        else
        {
            printf ("Function running... Line:(%d) - Params:(%s)\n", lin, (char *)params);
        }
    }
}

//*********************************************************************************************************************
// UCP Error Handling
//*********************************************************************************************************************
void UCPError (void)
{
    printf ("Error\n");
}
