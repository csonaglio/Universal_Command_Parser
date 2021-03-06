//*********************************************************************************************************************
// Universal Command Parser - UCP
// File: UCPTable.c
// 2022 - Claudio Sonaglio
//*********************************************************************************************************************
#include "UCPParser.h"

UCPTableType UCPParseTable[UCP_TABLE_SIZE] =
{
//  { "string<sq>", next line }
    { "",           0 }, //  0
    { "AT\x1",      3 }, //  1
    { UCPBKENDSTR,  0 },
    { "",           0 }, //  3
    { "+\x1",       8 }, //  4
    { "?",          0 }, //  5
    { "Z",          0 }, //  6
    { UCPBKENDSTR,  0 },
    { "ADR=\x2",    0 }, //  8
    { "APPEUI=\x2", 0 }, //  9
    { "APPSKEY=\x2",0 }, // 10
    { "ATCONF=\x2", 0 }, // 11
    { "BAND=\x2",   0 }, // 12
    { "BAT=?",      0 }, // 13
    { "CERTIF=\x2", 0 }, // 14
    { "CLASS=\x2",  0 }, // 15
    { "DADDR=\x2",  0 }, // 16
    { "DCS=\x2",    0 }, // 17
    { "DEUI=\x2",   0 }, // 18
    { "DR=\x2",     0 }, // 19
    { "JN1DL=\x2",  0 }, // 20
    { "JN2DL=\x2",  0 }, // 21
    { "JOIN=\x2",   0 }, // 22
    { "LTIME=?",    0 }, // 23
    { "NWKID=\x2",  0 }, // 24
    { "NWKSKEY=\x2",0 }, // 25
    { "PGSLOT=\x2", 0 }, // 26
    { "RX1DL=\x2",  0 }, // 27
    { "RX2DL=\x2",  0 }, // 28
    { "RX2DR=\x2",  0 }, // 29
    { "RX2FQ=\x2",  0 }, // 30
    { "SEND=\x2",   0 }, // 31
    { "TOF\x2",     0 }, // 32
    { "TRSSI\x2",   0 }, // 33
    { "TRX=\x2",    0 }, // 34
    { "TTH=\x2",    0 }, // 35
    { "TTONE\x2",   0 }, // 36
    { "TTX=\x2",    0 }, // 37
    { "TXP=\x2",    0 }, // 38
    { "VER=?",      0 }, // 39
    { "VL=\x2",     0 }, // 40
    { UCPBKENDSTR,  0 }
};
