# Universal_Command_Parser
Line oriented Universal Command Parser to be used in several situations, mainly AT Command FW in the form of an API.

Basic files of this API:
- UCPFunctions.c
- UCPFunctions.h
- UCPParser.c
- UCPParser.h
- UCPTable.c

Additional files as examples for the UPCParserTable structure (just copy the content of such files and replace all the test into UCPTable.c):
- UCPTable_AT_Cmd.txt
- UCPTable_Example_English.txt
- UCPTable_Example_Portuguese.txt

Additional files to allow the whole set to be compiled and tested under Code Blocks:
- UCP.cbp
- main.c

To help git to process the file structure properly:
- .gitignore
