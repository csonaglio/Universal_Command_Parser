//*********************************************************************************************************************
// Universal Command Parser - UCP
// File: UCPParser.c
// 2022 - Claudio Sonaglio
//*********************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "UCPParser.h"
#include "UCPFunctions.h"

//*********************************************************************************************************************
// UCP read Hex parameter with Separator Char
// *input - input string initial pointer
// size - pointer to a variable with size of the resulting hex memory (in bytes) - Max 255!!! ; 0 = as long as it goes
// sep - separator char, optional for IDs for example, such as '-' or ':'
// end - ending char - default shall be 0x0 (string end)
// *col - pointer to the column counter outside this function to keep sync
// *var - pointer to the initial position of the variable that will get the return value
// This function may return less data than asked. It´s up to the caller to evaluate this.
// If more data is available than asked, it´s an error. There must be an end to the sequence at the max size
//*********************************************************************************************************************
bool UCPHex (unsigned char *input, unsigned char *size, unsigned char sep, unsigned char end, unsigned int *col, unsigned char *var)
{
    unsigned char state = UCPHEX_WFD1;                  // Initial state is Waiting For Digit 1
    unsigned char dig, byte;                            // read digit & stored byte
    unsigned char qual;                                 // input char qualifier for the processing {null, end, sep, digit, other}
    unsigned int c = 0;                                 // Zero columns read
    unsigned char bytecnt = 0;                          // Zero bytes read
    bool ret = false;                                   // return (possible error) flag
    bool stb = false;                                   // store byte flag
    bool err = false;                                   // No error so far

    do
    {
        dig = Ch2Nib(input[c]);                         // evaluates the digit anyway
        // input char qualifier sequence
        if (UCPSTEND == input[c])                       // if string end
        { qual = UCPHEX_QN; } else
            if (end == input[c])                        // if end char
            { qual = UCPHEX_QE; } else
                if ((sep == input[c]) &&                // if separator found (and being used)
                    (sep != UCPHEX_NOSEP))
                { qual = UCPHEX_QS; } else
                    if (dig != 0xFF)                    // if valid digit
                    { qual = UCPHEX_QD; } else
                    { qual = UCPHEX_QO; }               // otherwise...
        switch (qual)                                   // Depending on the qualification round before...
        {
        case UCPHEX_QN:                                 // in the case of a string end
        case UCPHEX_QE:                                 // in the case of an ending char (programmable)
            ret = true;                                 // ...we will always abort the loop
            switch (state)
            {
            case UCPHEX_WFD1:                           // WAIT FOR DIGIT 1
                if (0 == bytecnt)                       // if no data received yet...
                { err = true; }                         // SURE ERROR
                else                                    // ...there is data already stored
                {   if (sep == input[c-1])              // if a separator was in the previous position...
                    { err = true; }                     // SURE ERROR
                    else
                    {   if (UCPHEX_NOSEP != sep)        // if we are not using separators, then (and only then)...
                        { stb = true; }                 // store valid data
                    }                                   // ...otherwise, valid data from previous round is already stored
                }
                break;
            case UCPHEX_WFD2:                           // WAIT FOR DIGIT 2
                if (sep == UCPSTEND)                    // if not using a separator char...
                { err = true; }                         // ...error, since without sep we need alwasy nibble pairs
                else                                    // ...otherwise it´s a valid sequence
                { stb = true; }                         // store valid data
                break;
            case UCPHEX_WFSP:                           // WAIT FOR SEPARATOR CHAR
                stb = true;                             // store valid data since it´s a valid sequence always we get here
                break;
            default:                                    // or any other case
                err = true;                             // SURE ERROR
                break;
            }
            break;
        case UCPHEX_QS:                                 // in the case of a separator char (programmable)
            if ((UCPHEX_WFD2 == state) ||
                (UCPHEX_WFSP == state))                 // if wait for separator or wait for digit2...
            {   state = UCPHEX_WFD1;                    // ...next digit shall be another nibble from a pair
                stb = true; }                           // ...mark to store a valid byte in process
            else                                        // otherwise, separator is not a valid option
            {   ret = true;                             // abort loop
                err = true; }                           // SURE ERROR
            break;
        case UCPHEX_QD:                                 // in the case of a valid digit
            switch (state)                              // evaluate the string state machine...
            {
            case UCPHEX_WFD1:                           // WAIT FOR DIGIT 1
                byte = dig;                             // stores 1st nibble as lowest part
                state = UCPHEX_WFD2;                    // waits for the next digit (if it will arrive...)
                break;
            case UCPHEX_WFD2:                           // WAIT FOR DIGIT 2
                byte *= 0x10;                           // move first nibble to the upper position
                byte += dig;                            // stores 1st nibble as lowest part
                if (UCPHEX_NOSEP == sep)                // if number shall not have separator chars...
                {   state = UCPHEX_WFD1;                // ...next digit shall be another nibble from a pair
                    stb = true; }                       // ...mark to store a new byte
                else
                { state = UCPHEX_WFSP; }                // ...otherwise, wait for the separator char
                break;
            case UCPHEX_WFSP:                           // WAIT FOR SEPARATOR CHAR
            default:                                    // or any other case
                ret = true;                             // abort loop
                err = true;                             // SURE ERROR
                break;
            }
            break;
        default:                                        // in any other case for the input char
            ret = true;                                 // abort loop
            err = true;                                 // SURE ERROR
            break;
        }
        if (true == stb)                                // if marked to store a new byte explicitly
        {
            *(var+bytecnt) = byte;                      // ...store data in the current var position
            byte = 0;                                   // ...restart byte
            stb = false;                                // ...restart flag
            bytecnt++;                                  // ...increment stored bytes
            if ((*size !=0) && (bytecnt >= *size))      // if size of data is defined and reached...
            { ret = true; }                             // abort loop but NO ERROR
        }
        if (false == ret)                               // only if not aborting loop
        { c++; }                                        // increment internal column counter
    } while (false == ret);                             // WHILE not aborted loop...
    *col = (*col + c);                                  // update external columnt counter
    *size = bytecnt;                                    // ... update size with received bytes
    return (!err);                                      // return if ok or error
}

//*********************************************************************************************************************
// UCP read unsigned Dec parameter
// *input - input string initial pointer
// min - minimum acceptable value
// max - maximum acceptable value - both determine the size of the data (1, 2 or 4 bytes)
// *col - pointer to the column counter outside this function to keep sync
// *var - pointer to the initial position of the variable that will get the return value (type shall be coherent)
//*********************************************************************************************************************
bool UCPuDec (unsigned char *input, unsigned int long min, unsigned int long max, unsigned int *col, void *var)
{
    unsigned int c = 0;
    bool ret = true;                                    // return ok is the default
    unsigned long acc_u = 0;
    unsigned char max_digits, digcnt =0, dig;

    if (max < 10) { max_digits = 1; }                   // define the max numer of digits based on the max value
    else if (max < 100) { max_digits = 2; }
    else if (max < 1000) { max_digits = 3; }
    else if (max < 10000) { max_digits = 4; }
    else if (max < 100000) { max_digits = 5; }
    else if (max < 1000000) { max_digits = 6; }
    else if (max < 10000000) { max_digits = 7; }
    else if (max < 100000000) { max_digits = 8; }
    else if (max < 1000000000) { max_digits = 9; }
    else { max_digits = 10; }

    while ((true == ret) && (digcnt < max_digits))
    {                                                    // scan the next positions to get number
        acc_u *= 10;
        dig = Ch2Dig(input[c]);
        if (0xFF == dig)
        {
            if (0 == digcnt)                            // if first digit wrong...
            {
                c++; digcnt++;                          // inc counters
                ret = false;                            // ...error for sure!!!
            }
            else
            {
                break;                                  // abort read loop. What´s accumulated may be a valid number.
            }
        }
        else                                            // valid digits
        {
            acc_u += dig;                               // accumulate value for the final read
            c++; digcnt++;                              // inc counters
        }
    }
    if ((acc_u < min) || (acc_u > max))                 // if value read outside valid range...
    {
        ret = false;                                    // ..error
    }
    else
    {
        if (acc_u <= 0xFF)                              // acc_u has 1 byte
        {
            *((unsigned char *)(var)) = (unsigned char)(acc_u);
        }
        else if (acc_u <= 0xFFFF)                       // acc_u has 2 bytes
        {
            *((unsigned int *)(var)) = (unsigned int)(acc_u);
        }
        else                                            // acc_u has 4 bytes
        {
            *((unsigned long int *)(var)) = (unsigned long int)(acc_u);
        }
    }
    *(col) = *(col) + c;
    return ret;
}

//*********************************************************************************************************************
// UCP clean the input from acceptable extra chars and adapt other things, such as upper / lower case
// *input - input string initial pointer
//*********************************************************************************************************************
void UCPClean(unsigned char *input)
{
#if UCP_CASE_SENSITIVE==0
    unsigned int col, len;
    unsigned char chr;

    col = 0;
    len = strlen((char *)input);
    for (col=0; col<len; col++)
    {
        chr = input[col];
        if ((chr >= 'a') & (chr <= 'z'))
        {
            chr = chr - 'a' + 'A';
        }
        input[col] = chr;
    }
#endif // UCP_CASE_SENSITIVE
}

//*********************************************************************************************************************
// UCP parse the input command vector using the configured tables
// *input - input string initial pointer
// This one calls the Error processing or the final Function processing for each possible valid sequence.
// There is a buffer called UCPPath which stores the intermediate lines followed before reaching the end. This
// allows for recovering the whole sentence if needed (since there may be multiple ways to end up in a line).
//*********************************************************************************************************************
void UCPParse(unsigned char *input)
{
    unsigned int col = 0, lin=0;

    unsigned char UCPPath[UCP_PATH_MAX_SIZE] = { 0, };           // Path variables - maybe move to global later
//    unsigned char val[32];
    unsigned char c = 0;
    bool srch = true;
    bool err = false;
    bool nxt = false;

    while (true == srch)                                        // While no aborted & still inside the input (one more for the string end)
    {
        while((UCPParseTable[lin].txt[c] == input[col + c]) &&  // While still same text and nobody aborted search...
              (true == srch))
        {
            if (UCPSTEND == input[col + c])                     // If both string ended...
            {
                srch = false;                                   // ...abort search for perfect match
            }
            else
            {
                c++;                                            // ...keep scanning all characters
                if (UCPNXBLK == UCPParseTable[lin].txt[c])      // if we reached a next block connector...
                {
                    nxt = true;                                 // ...signal to update position
                }
            }
        }
        if (true == nxt)
        {
            UCPPath[UCPPath[0]+1] = lin; UCPPath[0]++;          // Stores the Parser path through table (in lines) for further use
            lin = UCPParseTable[lin].nidx;                      // ...points to the next section to search
            col += c;                                           // update input column counter
            c = 0;                                              // starts over block column counter
            nxt = false;                                        // return to inside the same block state
        }
        while((UCPParseTable[lin].txt[c] != input[col + c]) &&  // While not finding a new matching line and nobody aborted
              (true == srch))                                   // (from previous block, we cannot be in a block ending line yet)
        {
            if (UCPBKEND == UCPParseTable[lin].txt[0])          // If we got to the last line of the block...
            {
                err = true;                                     // ...error, since could not find any line that matches
                srch = false;                                   // ...not searching anymore
            }
            else
            {
                if (UCPParseTable[lin].txt[0] != UCPNXBLK)      // If we didn´t reach a block end (we ruled out input end above)...
                {
                    switch (UCPParseTable[lin].txt[c])          // Depending on the next char of the reference...
                    {
                    case UCPSTEND:                              // ...if string end, input has still chars...
                        if (0 == c)                             // ...only if 1st position...
                        {
                            lin++;
                        }
                        else                                    // in any other position, this is an error
                        {
                            err = true;                         // ...we got to a point where no match was possible
                            srch = false;                       // ...abort the search, error.
                        }
                        break;
                    case UCPNXBLK:
                        err = true;                             // ...TABLE ERROR!!! DEBUG IT!!! Next Block should have happened above...
                        srch = false;                           // ...abort the search, error.
                        break;
                    case UCPSQEND:
                        srch = false;                           // ...abort the search, Valid Sequence End.
                        break;
                    default:                                    // ...there should be more to parse
                        if (UCPSTEND == input[col + c])         // if input ended...
                        {
                            err = true;                         // ...we got to a point where no match was possible
                            srch = false;                       // ...abort the search, error.
                        }
                        else
                        {
                            lin++;                              // Search for a next line that may have match
                        }
                        break;
                    }
                }
                else                                            // if finds...
                {
                    err = true;                                 // ...we got to a point where no match was possible
                    srch = false;                               // ...abort the search, error.
                }
            }
        }
        // We just left a search for a new matching line...
        if (true == srch)                                       // In case we are still searching...
        {
            for (unsigned char x = 0; x < c; x++)               // searches the preceding part of the strings to see if matches...
            {
                if (UCPParseTable[lin].txt[x] != input[col + x])// ...if any part is wrong...
                {
                    err = true;                                 // ...we got to a point where no match was possible
                    srch = false;                               // ...abort the search, error.
                }
            }
        }
    }
    col += c;                                                   // update input column counter, just in case
    if (false == err)                                           // If no error recorded...
    {
        UCPPath[UCPPath[0]+1] = lin; UCPPath[0]++;              // Adds last step to the Path
    }
    if (err)
    {
        UCPError();                                             // Calls Error Handling
    }
    else
    {
        UCPFunction(&input[col], UCPPath);                     // Calls Main Function process
    }
}
