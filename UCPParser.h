//*********************************************************************************************************************
// Universal Command Parser - UCP
// File: UCPParser.h
// 2022 - Claudio Sonaglio
//*********************************************************************************************************************
#ifndef UCPPARSER_H
#define UCPPARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define UCP_CASE_SENSITIVE              1
#define UCP_MAX_COMMAND_LENGTH          512
#define UCP_TABLE_TEXT_SIZE             10
#define UCP_TABLE_SIZE                  50
#define UCP_PATH_MAX_SIZE               20

#define UCPSTEND                        0x0             // String End Marker (strict end only)
#define UCPNXBLK                        0x1             // Marks a pointer to the next block
#define UCPSQEND                        0x2             // Marks a sequence end (which may have more parameters after)
#define UCPBKEND                        0x3             // Sequence End (strictly ends the parser here)
#define UCPBKENDSTR                     "\x3"           // Block End String

#define UCPHEX_NOSEP                    0x0             // No separator in hex

// Qualifier codes for HEX Function
#define UCPHEX_QN                       0               // Qualifier Null (String end)
#define UCPHEX_QE                       1               // Qualifier End char (Programmable end char)
#define UCPHEX_QS                       2               // Qualifier Separator char (Programmable separator char)
#define UCPHEX_QD                       3               // Qualifier Digit char (Valid digit detected)
#define UCPHEX_QO                       4               // Qualifier Any other char
// States for HEX Function Detector
#define UCPHEX_WFD1                     0               // Wait for Digit 1
#define UCPHEX_WFD2                     1               // Wait for Digit 2
#define UCPHEX_WFSP                     2               // Wait for Separator Char

typedef unsigned char UCPchar;
typedef unsigned char UCPINDEX;

#define Ch2Dig(c)       (((c>='0')&&(c<='9'))?(c-'0'):0xFF)
#define Ch2Nib(c)       (((c>='0')&&(c<='9'))?(c-'0'):(((c>='a')&&(c<='f'))?(c-'a'+10):(((c>='A')&&(c<='F'))?(c-'A'+10):0xFF)))
#define Ch2Sig(c)       ((c=='-')?(-1):((c=='+')?(+1):(0)))
#define BitT(var,bits)  ((bits==(var & bits))?true:false)
#define BitG(var,bits)  (var & bits)

typedef struct
{
    UCPchar txt[UCP_TABLE_TEXT_SIZE];
    UCPINDEX nidx;
} UCPTableType;

UCPTableType UCPParseTable[UCP_TABLE_SIZE];

//*********************************************************************************************************************
// UCP read Hex parameter with Separator Char
// *input - input string initial pointer
// size - pointer to a variable with size of the resulting hex memory (in bytes) - Max 255!!! ; 0 = as long as it goes
// sep - separator char, optional for IDs for example, such as - or :
// end - ending char - default shall be 0x0 (string end)
// *col - pointer to the column counter outside this function to keep sync
// *var - pointer to the initial position of the variable that will get the return value
//*********************************************************************************************************************
bool UCPHex (unsigned char *input, unsigned char *size, unsigned char sep, unsigned char end, unsigned int *col, unsigned char *var);

//*********************************************************************************************************************
// UCP read unsigned Dec parameter
// *input - input string initial pointer
// min - minimum acceptable value
// max - maximum acceptable value - both determine the size of the data (1, 2 or 4 bytes)
// *col - pointer to the column counter outside this function to keep sync
// *var - pointer to the initial position of the variable that will get the return value (type shall be coherent)
//*********************************************************************************************************************
bool UCPuDec (unsigned char *input, unsigned int long min, unsigned int long max, unsigned int *col, void *var);

//*********************************************************************************************************************
// UCP clean the input from acceptable extra chars and adapt other things, such as upper / lower case
// *input - input string initial pointer
//*********************************************************************************************************************
void UCPClean(unsigned char *input);

//*********************************************************************************************************************
// UCP parse the input command vector using the configured tables
// *input - input string initial pointer
//*********************************************************************************************************************
void UCPParse(unsigned char *input);
#endif // UCPPARSER_H
