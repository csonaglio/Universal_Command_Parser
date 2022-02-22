//*********************************************************************************************************************
// Universal Command Parser - UCP
// 2022 - Claudio Sonaglio
//*********************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "UCPParser.h"

int main()
{
    unsigned char CommandLine[UCP_MAX_COMMAND_LENGTH];

// Test Hex decoder
    unsigned char dest[80], size, i, sep;
    unsigned int col = 0;

    while (NULL != gets((char *)CommandLine))
    {
        UCPClean(CommandLine);                  // "Cleans up" the incoming text line
//        Test Parser commands
        UCPParse(CommandLine);                  // Parses the incoming text line

// Test Hex decoder
/*        size = 0;
        col = 0;
        sep = 0;      // UCPHEX_NOSEP may be a case
        if (UCPHex(CommandLine,&size,sep, ')', &col,dest))
        {
            printf ("ok -");
            for (i=0; i<size; i++)
            {
                printf (" %02X",dest[i]);
            }
            printf("\n\n");
        }
        else
        {
            printf ("error - %d\n\n",col);
        } */
    }
    return 0;
}
