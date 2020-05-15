//helo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MaxMachineCodeLines 100

// File pointers
FILE *fp1;
FILE *idFptr;
FILE *tfptr;
FILE *stfptr;
FILE *Treefptr;
FILE *mcfptr;

// Global variables and Functions for LA
int index_ = 0;
char *buffer = NULL;
size_t size = 0;
int left = 0, right = 0, len;
char dest[5000];
char *type = NULL;
char *string[200][50];
char temp[30];
char buffer_[15], operators[] = "!{}()<>:,;+-*%=/", buffer2[3];
int count = 0;
//void printTokenLexPair(char *token, char lexeme);

// Global Variables and Functions for Parser
void tabs(int);
void statement();
void T();
void E();
void E1();
void F();
void T1();
void CId();
void RN();
void KN();
void RE();
void N();
void IfStatement();
void matched_stmnt();
void unmatched_stmnt();
void addNode(int spaces, char *val);
int i = 0;
bool MultiValDecl = false;
bool FuncDecl = false;
int curlBrak[50];
int cbIndex = 0;
bool statmentflag = false;
char **stringParser = NULL;
char **Lexemes = NULL;
int r = 0, c = 0;
int toks = 0, lexs = 0;
#define TRUE 1
int tabNo = 0;

// Global Variables and Functions for Translator
char *TemporaryNameGenerator();
bool Assignment();
void IdDeclaration();
void MulDiv();
void Loops();
bool IOFunctions();
bool isType(int s);
bool checkCompatibility(int a, int b);
bool isDeclared(int c);
char *alphabets[26] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
char n = 0;
int Translator_index = 0;
char *lastname;
char *seclastname;
char **variables = NULL;
int varindex_ = 0;
int Labels = 0;
int arrLabels[100];
int arrlabind = 0;
char **variabletypes = NULL;
int typeindex_ = 0;
int *variableAddress = NULL;
int addressindex = 0;
int savedIndex[100];
int savedIndex2[100];
int index_no = 0;
int index_no2 = 0;
int flag = 0;
int LineNumber = 0;
int VarAddress = 0;
int while_flag = 0;

//VM Code
char symbolTable[100][100];
int SymbolType[MaxMachineCodeLines + 1][1];
char quadraple[MaxMachineCodeLines + 1][100];

//Program Counter
int PC = 0;
// TYPE
#define INT 0
#define CHAR 1
//SRC
#define Src1 0
#define Src2 1
///OPCODES
#define IntermediateAssignment 0 // x=1
#define ADD 1
#define SUBTRACT 2
#define MULTIPLY 3
#define DIVIDE 4
#define IN 5
#define cout 6   //cout a variable
#define Icout 26 // cout immediate string/char/ digit
#define OUT 7
#define GOTO 8
#define ASSIGNMENT 9 //x= x2
#define IADD 11		 //x= x2+2
#define ISUBTRACT 12 //x= x2-2
#define IMULTIPLY 13 //x= x2*2
#define IDIVIDE 14   //x= x2/2
#define IFG 15		 // if x > y goto 200
#define IFGE 16		 //if x >= y goto 200
#define IFL 17		 // if x < y goto 200
#define IFLTE 18	 // if x <= y goto 200
#define IIFG 19		 // if x > 1 goto 200
#define IIFGE 20	 //if x >= 1 goto 200
#define IIFL 21		 // if x < 1 goto 200
#define IIFLTE 22	// if x <= 1 goto 200

#define IIFNEQ 23 // if x != 1 goto 200
#define IFNEQ 24  // if x != y goto 200
#define IIFEQ 25  // if x == 1 goto 200
#define IFEQ 27   // if x != y goto 200
// Global variables and functions for generating Machine code
char TACwords[1000][100];
int tac_index = 5;
void GenerateMachineCode();
void GetTACinArray();
void conditionSymbol();
void unconditionSymbol();
void AssignmentSymbol();
void IOSymbol();

void GenerateMachineCode()
{
	//printf("in generatemachinecode");
	mcfptr = fopen("machine-code.txt", "w");
	if (mcfptr == NULL)
	{
		printf("Could not open file");
		return;
	}
	fclose(mcfptr);

	GetTACinArray();

	while (strcmp(TACwords[tac_index], "$") != 0)
	{
		if (strcmp(TACwords[tac_index], "<-") == 0)
			AssignmentSymbol();
		else if (strcmp(TACwords[tac_index], "cout") == 0 || strcmp(TACwords[tac_index], "out") == 0 || strcmp(TACwords[tac_index], "in") == 0)
			IOSymbol();
		else if (strcmp(TACwords[tac_index], "if") == 0)
			conditionSymbol();
		else if (strcmp(TACwords[tac_index], "goto") == 0)
			unconditionSymbol();
		tac_index++;
	}
	// fclose(mcfptr);
}
int RetVariableAddress(int an_index)
{
	int k;
	for (k = 0; k < varindex_; k++)
		if (strcmp(TACwords[an_index], variables[k]) == 0)
			return variableAddress[k];
	return -1;
}

void AssignmentSymbol()
{

	int var1, var2, var3;
	var1 = RetVariableAddress(tac_index - 1);
	var2 = RetVariableAddress(tac_index + 1);
	var3 = RetVariableAddress(tac_index + 3);

	mcfptr = fopen("machine-code.txt", "a");
	if (strcmp(TACwords[tac_index + 2], "+") == 0)
	{
		if (var3 == -1)
			fprintf(mcfptr, "%s %d %d %s %d\n", TACwords[tac_index - 2], 11, var2, TACwords[tac_index + 3], var1);
		else
			fprintf(mcfptr, "%s %d %d %d %d\n", TACwords[tac_index - 2], 1, var2, var3, var1);
	}
	else if (strcmp(TACwords[tac_index + 2], "-") == 0)
	{
		if (var3 == -1)
			fprintf(mcfptr, "%s %d %d %s %d\n", TACwords[tac_index - 2], 12, var2, TACwords[tac_index + 3], var1);
		else
			fprintf(mcfptr, "%s %d %d %d %d\n", TACwords[tac_index - 2], 2, var2, var3, var1);
	}
	else if (strcmp(TACwords[tac_index + 2], "*") == 0)
	{
		if (var3 == -1)
			fprintf(mcfptr, "%s %d %d %s %d\n", TACwords[tac_index - 2], 13, var2, TACwords[tac_index + 3], var1);
		else
			fprintf(mcfptr, "%s %d %d %d %d\n", TACwords[tac_index - 2], 3, var2, var3, var1);
	}
	else if (strcmp(TACwords[tac_index + 2], "/") == 0)
	{
		if (var3 == -1)
			fprintf(mcfptr, "%s %d %d %s %d\n", TACwords[tac_index - 2], 14, var2, TACwords[tac_index + 3], var1);
		else
			fprintf(mcfptr, "%s %d %d %d %d\n", TACwords[tac_index - 2], 4, var2, var3, var1);
	}
	else
	{
		if (var2 == -1)
			fprintf(mcfptr, "%s %d %s %d %d\n", TACwords[tac_index - 2], 0, TACwords[tac_index + 1], -1, var1);
		else
			fprintf(mcfptr, "%s %d %d %d %d\n", TACwords[tac_index - 2], 9, var2, -1, var1);
	}
	fclose(mcfptr);
}

void IOSymbol()
{
	mcfptr = fopen("machine-code.txt", "a");
	int var1, var2, var3;
	var1 = RetVariableAddress(tac_index + 2);

	if (strcmp(TACwords[tac_index], "cout") == 0)
	{
		if (var1 == -1)
			fprintf(mcfptr, "%s %d %s %d %d\n", TACwords[tac_index - 1], 26, TACwords[tac_index + 2], -1, -1);
		else
			fprintf(mcfptr, "%s %d %d %d %d\n", TACwords[tac_index - 1], 6, var1, -1, -1);
	}
	else if (strcmp(TACwords[tac_index], "in") == 0)
	{
		fprintf(mcfptr, "%s %d %d %d %d\n", TACwords[tac_index - 1], 5, var1, -1, -1);
	}
	else if (strcmp(TACwords[tac_index], "out") == 0)
	{
		fprintf(mcfptr, "%s %d %d %d %d\n", TACwords[tac_index - 1], 7, var1, -1, -1);
	}
	fclose(mcfptr);
}

char *RetLabelLineNumber(int index_)
{
	int length = sizeof(TACwords);
	int z;
	char *word = NULL;
	word = (char *)malloc(sizeof(char) * 10);
	sprintf(word, "%s", TACwords[index_]);
	strcat(word, ":");
	for (z = 0; z <= length; z++)
		if (strcmp(word, TACwords[z]) == 0)
		{
			int p = 0;
			while (TACwords[z - 1][p] != '\0')
			{
				p++;
			}
			TACwords[z - 1][p - 1] = '\0';
			return TACwords[z - 1];
		}
	return NULL;
}

void conditionSymbol()
{
	//printf("in conditionsymbol");
	mcfptr = fopen("machine-code.txt", "a");
	int var1, var2, var3;
	var1 = RetVariableAddress(tac_index + 1);
	var2 = RetVariableAddress(tac_index + 3);

	if (strcmp(TACwords[tac_index + 2], "<") == 0)
	{
		if (var2 == -1)
			fprintf(mcfptr, "%s %d %d %s %s\n", TACwords[tac_index - 1], 21, var1, TACwords[tac_index + 3], RetLabelLineNumber(tac_index + 5));
		else
			fprintf(mcfptr, "%s %d %d %d %s\n", TACwords[tac_index - 1], 17, var1, var2, RetLabelLineNumber(tac_index + 5));
	}
	else if (strcmp(TACwords[tac_index + 2], ">") == 0)
	{
		if (var2 == -1)
			fprintf(mcfptr, "%s %d %d %s %s\n", TACwords[tac_index - 1], 19, var1, TACwords[tac_index + 3], RetLabelLineNumber(tac_index + 5));
		else
			fprintf(mcfptr, "%s %d %d %d %s\n", TACwords[tac_index - 1], 15, var1, var2, RetLabelLineNumber(tac_index + 5));
	}
	else if (strcmp(TACwords[tac_index + 2], "<=") == 0)
	{
		if (var2 == -1)
			fprintf(mcfptr, "%s %d %d %s %s\n", TACwords[tac_index - 1], 22, var1, TACwords[tac_index + 3], RetLabelLineNumber(tac_index + 5));
		else
			fprintf(mcfptr, "%s %d %d %d %s\n", TACwords[tac_index - 1], 18, var1, var2, RetLabelLineNumber(tac_index + 5));
	}
	else if (strcmp(TACwords[tac_index + 2], ">=") == 0)
	{
		if (var2 == -1)
			fprintf(mcfptr, "%s %d %d %s %s\n", TACwords[tac_index - 1], 20, var1, TACwords[tac_index + 3], RetLabelLineNumber(tac_index + 5));
		else
			fprintf(mcfptr, "%s %d %d %d %s\n", TACwords[tac_index - 1], 16, var1, var2, RetLabelLineNumber(tac_index + 5));
	}
	else if (strcmp(TACwords[tac_index + 2], "!=") == 0)
	{
		if (var2 == -1)
			fprintf(mcfptr, "%s %d %d %s %s\n", TACwords[tac_index - 1], 23, var1, TACwords[tac_index + 3], RetLabelLineNumber(tac_index + 5));
		else
			fprintf(mcfptr, "%s %d %d %d %s\n", TACwords[tac_index - 1], 24, var1, var2, RetLabelLineNumber(tac_index + 5));
	}
	else if (strcmp(TACwords[tac_index + 2], "==") == 0)
	{
		if (var2 == -1)
			fprintf(mcfptr, "%s %d %d %s %s\n", TACwords[tac_index - 1], 25, var1, TACwords[tac_index + 3], RetLabelLineNumber(tac_index + 5));
		else
			fprintf(mcfptr, "%s %d %d %d %s\n", TACwords[tac_index - 1], 27, var1, var2, RetLabelLineNumber(tac_index + 5));
	}
	tac_index += 5;
	fclose(mcfptr);
}

void unconditionSymbol()
{
	//printf("in unconditionsymbol");
	mcfptr = fopen("machine-code.txt", "a");
	fprintf(mcfptr, "%s %d %s %d %d\n", TACwords[tac_index - 1], 8, RetLabelLineNumber(tac_index + 1), -1, -1);
	tac_index += 1;
	fclose(mcfptr);
}

void GetTACinArray()
{

	FILE *fp;
	fp = fopen("tac.txt", "r");
	int j = 0;
	while (fscanf(fp, "%100s", TACwords[j]) != EOF)
	{
		j++;
	}
	strcpy(TACwords[j], "$");
	j++;
	/*int i;
	for (i=0;i<j;i++)
	{
		printf("%s\n", TACwords[i]);
	} */
	fclose(fp);
}

// code for Translator
void Translator()
{
	tfptr = fopen("tac.txt", "w");
	if (tfptr == NULL)
	{
		printf("Could not open file");
		return;
	}
	fprintf(tfptr, "//Three address code for test.cmm\n\n");
	fclose(tfptr);

	stfptr = fopen("translator-symboltable.txt", "w");
	if (stfptr == NULL)
	{
		printf("Could not open file");
		return;
	}
	fprintf(stfptr, "//Updated Symbol table by Translator for test.cmm\n");
	fclose(tfptr);
	tfptr = fopen("tac.txt", "a");
	stfptr = fopen("translator-symboltable.txt", "a");
	lastname = (char *)malloc(sizeof(char) * 10);
	seclastname = (char *)malloc(sizeof(char) * 10);
	variables = (char **)malloc(r * sizeof(char *));
	for (i = 0; i < r; i++)
		variables[i] = (char *)malloc(c * sizeof(char));
	variableAddress = (int *)malloc(c * sizeof(int));
	variabletypes = (char **)malloc(r * sizeof(char *));
	for (i = 0; i < r; i++)
		variabletypes[i] = (char *)malloc(c * sizeof(char));
	while (strcmp(stringParser[Translator_index], "$") != 0)
	{
		if (strcmp(stringParser[Translator_index], "<-") == 0)
		{
			Assignment();
		}
		if (isType(Translator_index))
		{
			IdDeclaration();
		}
		if (strcmp(stringParser[Translator_index], "while") == 0 || strcmp(stringParser[Translator_index], "for") == 0)
		{
			Loops();
		}
		if (strcmp(stringParser[Translator_index], "print") == 0 || strcmp(stringParser[Translator_index], "scan") == 0 || strcmp(stringParser[Translator_index], "return") == 0)
		{
			IOFunctions();
		}
		if (strcmp(stringParser[Translator_index], "}") == 0 && while_flag > 0)
		{
			arrlabind--;
			fprintf(tfptr, "%d) \tgoto Label%d\n", LineNumber, arrLabels[arrlabind]);
			while_flag--;
			LineNumber++;
			fprintf(tfptr, "%d) Label%d:\n", LineNumber, arrLabels[arrlabind] + 2);
			LineNumber++;
		}
		Translator_index++;
	}
	fclose(tfptr);
	fclose(stfptr);
	return;
}

bool checkCompatibility(int a, int b)
{
	if (isDeclared(a) && isDeclared(b))
	{
		int i1 = 0, i2 = 0;
		int z;
		for (z = 0; z <= varindex_; z++)
		{
			if (strcmp(Lexemes[a], variables[z]) == 0)
			{
				i1 = z;
				break;
			}
		}
		for (z = 0; z <= varindex_; z++)
		{
			if (strcmp(Lexemes[b], variables[z]) == 0)
			{
				i2 = z;
				break;
			}
		}
		if (strcmp(variabletypes[i1], variabletypes[i2]) == 0)
			return true;
	}

	else
		printf("Undeclared Identifier used in Assignment statement, %d\n", LineNumber++);

	printf("Undecompatible Identifier used in Assignment statement, %d\n", LineNumber++);
	return false;
}

bool isDeclared(int a)
{
	int sz = varindex_;
	int z;
	for (z = 0; z <= sz; z++)
		if (strcmp(Lexemes[a], variables[z]) == 0)
			return true;
	return false;
}

char *TemporaryNameGenerator()
{
	char *ch1 = (char *)malloc(sizeof(char) * 10);
	sprintf(ch1, "%s", "temp");
	strcat(ch1, alphabets[n]);
	n++;
	fprintf(stfptr, "%s , %s , %d\n", ch1, stringParser[savedIndex2[index_no2]], VarAddress);
	if (strcmp(stringParser[savedIndex2[index_no2]], "int") == 0)
		VarAddress = VarAddress + 4;
	else
		VarAddress++;
	strcpy(variables[varindex_], ch1);
	varindex_++;
	strcpy(variabletypes[typeindex_], stringParser[savedIndex2[index_no2]]);
	variableAddress[addressindex] = VarAddress;
	addressindex++;
	typeindex_++;
	return ch1;
}

void Loops()
{
	if (strcmp(stringParser[Translator_index], "while") == 0)
	{
		fprintf(tfptr, "%d) Label%d:\n", LineNumber, Labels);
		arrLabels[arrlabind] = Labels;
		LineNumber++;
		Labels++;
		fprintf(tfptr, "%d) \tif %s %s %s goto Label%d\n", LineNumber, Lexemes[Translator_index + 2], Lexemes[Translator_index + 3], Lexemes[Translator_index + 4], Labels);
		Translator_index += 5;
		Labels++;
		LineNumber++;
		fprintf(tfptr, "%d) \tgoto Label%d\n", LineNumber, Labels);
		Labels++;
		LineNumber++;
		fprintf(tfptr, "%d) Label%d:\n", LineNumber, arrLabels[arrlabind] + 1);
		LineNumber++;
		while_flag++;
		arrlabind++;
	}
}

bool IOFunctions()
{
	if (strcmp(stringParser[Translator_index], "print") == 0)
	{
		if (strcmp(stringParser[Translator_index + 1], ":") == 0 && strcmp(stringParser[Translator_index + 2], "ID") == 0 && strcmp(stringParser[Translator_index + 3], ";") == 0)
		{
			if (isDeclared(Translator_index + 2) == true)
			{
				fprintf(tfptr, "%d) \tcout << %s\n", LineNumber, Lexemes[Translator_index + 2]);
				LineNumber++;
			}
			else if (isDeclared(Translator_index + 2) == false)
			{
				printf("Undeclared Identifier used in print statement, %d\n", LineNumber++);
				exit(1);
			}
		}

		else if (strcmp(stringParser[Translator_index + 1], ":") == 0 && strcmp(stringParser[Translator_index + 2], "STR") == 0 && strcmp(stringParser[Translator_index + 3], ";") == 0)
		{
			fprintf(tfptr, "%d) \tcout << %s\n", LineNumber, Lexemes[Translator_index + 2]);
			LineNumber++;
		}
		Translator_index = Translator_index + 3;
	}

	else if (strcmp(stringParser[Translator_index], "scan") == 0)
	{
		if (strcmp(stringParser[Translator_index + 1], ":") == 0 && strcmp(stringParser[Translator_index + 2], "ID") == 0 && strcmp(stringParser[Translator_index + 3], ";") == 0)
		{
			if (isDeclared(Translator_index + 2) == true)
			{
				fprintf(tfptr, "%d) \tin >> %s\n", LineNumber, Lexemes[Translator_index + 2]);
				LineNumber++;
			}
			else if (isDeclared(Translator_index + 2) == false)
			{
				printf("Undeclared Identifier used in scan statement, %d\n", LineNumber++);
				exit(1);
			}
		}
		Translator_index = Translator_index + 3;
		return true;
	}

	else if (strcmp(stringParser[Translator_index], "return") == 0)
	{
		if (strcmp(stringParser[Translator_index + 1], "ID") == 0 && strcmp(stringParser[Translator_index + 2], ";") == 0)
		{
			if (isDeclared(Translator_index + 1) == true)
			{
				fprintf(tfptr, "%d) \tout >> %s\n", LineNumber, Lexemes[Translator_index + 1]);
				LineNumber++;
			}
			else if (isDeclared(Translator_index + 1) == false)
			{
				printf("Undeclared Identifier used in return statement, %d\n", LineNumber++);
				exit(1);
			}
		}
		Translator_index = Translator_index + 2;
		return true;
	}
}

bool Assignment()
{

	if (strcmp(stringParser[Translator_index + 2], ";") == 0)
	{

		if (strcmp(stringParser[Translator_index + 1], "ID") == 0)
		{
			if (checkCompatibility(Translator_index - 1, Translator_index + 1) == false)
			{

				exit(1);
			}
		}
		fprintf(tfptr, "%d) \t%s %s %s\n", LineNumber, Lexemes[Translator_index - 1], stringParser[Translator_index], Lexemes[Translator_index + 1]);
		Translator_index = Translator_index + 2;
		LineNumber++;
	}

	else if (strcmp(stringParser[Translator_index + 2], "AOP") == 0)
	{

		if (strcmp(stringParser[Translator_index + 1], "ID") == 0)
		{
			if (checkCompatibility(Translator_index - 1, Translator_index + 1) == false)
			{
				exit(1);
			}
		}
		savedIndex[index_no] = Translator_index;
		Expression();
		fprintf(tfptr, "%d) \t%s %s %s\n", LineNumber, Lexemes[savedIndex[index_no] - 1], stringParser[savedIndex[index_no]], lastname);
		LineNumber++;
	}
	return true;
}

Expression()
{
	Translator_index = Translator_index + 2;
	if (strcmp(stringParser[Translator_index + 1], "ID") == 0)
	{
		if (isDeclared(Translator_index + 1) == false)
		{
			printf("Undeclared Identifier used in Assignment statemnet, %d", LineNumber);
			exit(1);
		}
	}
	if (strcmp(stringParser[Translator_index + 2], ";") == 0)
	{
		if (strcmp(Lexemes[Translator_index], "+") == 0 || strcmp(Lexemes[Translator_index], "-") == 0 || strcmp(Lexemes[Translator_index], "*") == 0 || strcmp(Lexemes[Translator_index], "/") == 0)
		{
			char *a = (char *)malloc(sizeof(char) * 10);
			strcpy(a, TemporaryNameGenerator());
			if (flag == 0)
				fprintf(tfptr, "%d) \t%s <- %s %s %s\n", LineNumber, a, Lexemes[Translator_index - 1], Lexemes[Translator_index], Lexemes[Translator_index + 1]);
			else if (flag == 1)
			{
				fprintf(tfptr, "%d) \t%s <- %s %s %s\n", LineNumber, a, lastname, Lexemes[Translator_index], Lexemes[Translator_index + 1]);
				flag = 0;
			}
			strcpy(lastname, a);
			LineNumber++;
			Translator_index = Translator_index + 2;
		}
	}

	else if (strcmp(Lexemes[Translator_index + 2], "*") == 0 || strcmp(Lexemes[Translator_index + 2], "/") == 0)
	{
		index_no++;
		savedIndex[index_no] = Translator_index;
		MulDiv();
		char *a = (char *)malloc(sizeof(char) * 10);
		strcpy(a, TemporaryNameGenerator());
		fprintf(tfptr, "%d) \t%s <- %s %s %s\n", LineNumber, a, Lexemes[savedIndex[index_no] - 1], Lexemes[savedIndex[index_no]], lastname);
		strcpy(lastname, a);
		LineNumber++;
		index_no--;
	}

	else if (strcmp(Lexemes[Translator_index + 2], "+") == 0 || strcmp(Lexemes[Translator_index + 2], "-") == 0)
	{
		index_no++;
		savedIndex[index_no] = Translator_index;
		Expression();
		char *a = (char *)malloc(sizeof(char) * 10);
		strcpy(a, TemporaryNameGenerator());
		if (flag == 0)
		{
			fprintf(tfptr, "%d) \t%s <- %s %s %s\n", LineNumber, a, Lexemes[savedIndex[index_no] - 1], Lexemes[savedIndex[index_no]], lastname);
		}
		if (flag == 1)
		{
			fprintf(tfptr, "%d) \t%s <- %s %s %s\n", LineNumber, a, seclastname, Lexemes[savedIndex[index_no]], lastname);
			flag = 0;
		}
		strcpy(lastname, a);
		LineNumber++;
		index_no--;
	}
}

void MulDiv()
{
	Translator_index = Translator_index + 2;
	if (strcmp(stringParser[Translator_index + 1], "ID") == 0)
	{
		if (isDeclared(Translator_index + 1) == false)
		{
			printf("Undeclared Identifier used in Assignment statemnet, %d", LineNumber);
			exit(1);
		}
	}
	if (strcmp(stringParser[Translator_index + 2], ";") == 0)
	{
		if (strcmp(Lexemes[Translator_index], "*") == 0 || strcmp(Lexemes[Translator_index], "/") == 0)
		{
			char *a = (char *)malloc(sizeof(char) * 10);
			strcpy(a, TemporaryNameGenerator());
			fprintf(tfptr, "%d) \t%s = %s %s %s\n", LineNumber, a, Lexemes[Translator_index - 1], Lexemes[Translator_index], Lexemes[Translator_index + 1]);
			strcpy(lastname, a);
			LineNumber++;
			Translator_index = Translator_index + 2;
		}
	}
	else if (strcmp(Lexemes[Translator_index + 2], "*") == 0 || strcmp(Lexemes[Translator_index + 2], "/") == 0)
	{
		index_no++;
		savedIndex[index_no] = Translator_index;
		MulDiv();
		char *a = (char *)malloc(sizeof(char) * 10);
		strcpy(a, TemporaryNameGenerator());
		fprintf(tfptr, "%d) \t%s <- %s %s %s\n", LineNumber, a, Lexemes[savedIndex[index_no] - 1], Lexemes[savedIndex[index_no]], lastname);
		strcpy(lastname, a);
		LineNumber++;
		index_no--;
	}

	else if (strcmp(Lexemes[Translator_index + 2], "+") == 0 || strcmp(Lexemes[Translator_index + 2], "-") == 0)
	{
		char *a = (char *)malloc(sizeof(char) * 10);
		strcpy(a, TemporaryNameGenerator());
		fprintf(tfptr, "%d) \t%s = %s %s %s\n", LineNumber, a, Lexemes[Translator_index - 1], Lexemes[Translator_index], Lexemes[Translator_index + 1]);
		strcpy(lastname, a);
		strcpy(seclastname, a);
		flag = 1;
		LineNumber++;
		Expression();
	}
}

bool isType(int s)
{
	if (strcmp(stringParser[s], "int") == 0 || strcmp(stringParser[s], "char") == 0 || strcmp(stringParser[s], "bool") == 0 || strcmp(stringParser[s], "string") == 0 || strcmp(stringParser[s], "float") == 0)
		return true;
	return false;
}

void IdDeclaration()
{
	if (isType(Translator_index) == true)
	{
		index_no2++;
		savedIndex2[index_no2] = Translator_index;
	}

	if (strcmp(stringParser[Translator_index + 1], "ID") == 0 && strcmp(stringParser[Translator_index + 2], ";") == 0)
	{
		fprintf(stfptr, "%s , %s , %d\n", Lexemes[Translator_index + 1], stringParser[savedIndex2[index_no2]], VarAddress);
		strcpy(variables[varindex_], Lexemes[Translator_index + 1]);
		varindex_++;
		strcpy(variabletypes[typeindex_], stringParser[savedIndex2[index_no2]]);
		typeindex_++;
		variableAddress[addressindex] = VarAddress;
		addressindex++;
		Translator_index = Translator_index + 2;
		if (strcmp(stringParser[savedIndex2[index_no2]], "int") == 0)
			VarAddress = VarAddress + 4;
		else
			VarAddress++;
		index_no2--;
	}
	else if (isType(Translator_index + 1) && strcmp(stringParser[Translator_index + 2], "ID") == 0 && strcmp(stringParser[Translator_index + 3], ")") == 0)
	{
		fprintf(stfptr, "%s , %s , %d\n", Lexemes[Translator_index + 2], stringParser[savedIndex2[index_no2]], VarAddress);
		strcpy(variables[varindex_], Lexemes[Translator_index + 2]);
		varindex_++;
		strcpy(variabletypes[typeindex_], stringParser[savedIndex2[index_no2]]);
		typeindex_++;
		variableAddress[addressindex] = VarAddress;
		addressindex++;
		Translator_index = Translator_index + 3;
		if (strcmp(stringParser[savedIndex2[index_no2]], "int") == 0)
			VarAddress = VarAddress + 4;
		else
			VarAddress++;
	}
	else if (strcmp(stringParser[Translator_index + 1], "ID") == 0 && strcmp(stringParser[Translator_index + 2], ",") == 0)
	{
		fprintf(stfptr, "%s , %s , %d\n", Lexemes[Translator_index + 1], stringParser[savedIndex2[index_no2]], VarAddress);
		strcpy(variables[varindex_], Lexemes[Translator_index + 1]);
		varindex_++;
		strcpy(variabletypes[typeindex_], stringParser[savedIndex2[index_no2]]);
		typeindex_++;
		variableAddress[addressindex] = VarAddress;
		addressindex++;
		Translator_index = Translator_index + 2;
		if (strcmp(stringParser[savedIndex2[index_no2]], "int") == 0)
			VarAddress = VarAddress + 4;
		else
			VarAddress++;
		IdDeclaration();
	}
	else if (strcmp(stringParser[Translator_index + 1], "ID") == 0 && strcmp(stringParser[Translator_index + 2], "(") == 0)
	{
		fprintf(stfptr, "%s , %s , %d\n", Lexemes[Translator_index + 1], stringParser[savedIndex2[index_no2]], 0);
		Translator_index = Translator_index + 2;
	}
}

void addNode(int spaces, char *val)
{
	int k;

	char edgeNode[1000];
	memset(edgeNode, '\0', sizeof(edgeNode[0]));

	for (k = 0; k < spaces; k++)
		strcat(edgeNode, ":");

	strcat(edgeNode, "|__");
	strcat(edgeNode, val);
	strcat(edgeNode, "\n");

	fprintf(Treefptr, "%s", edgeNode);
}
void RTRN()
{
	tabNo++;

	if (strcmp(stringParser[i], "STR") == 0 || strcmp(stringParser[i], "ID") == 0)
	{
		addNode(tabNo, Lexemes[i]);
		i++;
		if (strcmp(stringParser[i], ";") == 0)
		{
			addNode(tabNo, ";");
			i++;
		}
		else
		{
			printf("missing ; in return stmnt\n");
			exit(1);
		}
	}

	tabNo--;
}
void tabs(int n)
{
}
int getTokens(char base[])
{

	int j = 0;
	char *token;
	char *rest = base;

	while ((token = strtok_r(rest, " ", &rest)) && j != 1)
	{
		j++;
	}
	strcpy(stringParser[toks], token);
	toks++;

	return toks;
}

int getLexemes(char base[])
{
	int flaga = 0;
	int ind, j = 0;
	for (ind = 0; TRUE; ind++)
	{
		if (base[ind] != ' ' && flaga >= 3)
		{
			Lexemes[lexs][j++] = base[ind];
		}
		else if (base[ind] == ' ')
		{
			flaga++;
			if (flaga >= 4)
			{
				lexs++;
				j = 0;
				break;
			}
		}
		else if (base[ind] == '\0')
		{
			lexs++;
			j = 0;
			break;
		}
	}
	return lexs;
}

void ExtractTokensAndLexemes()
{
	FILE *fpp;
	fpp = fopen("words.txt", "r"); /* should check the result */
	char line[256];
	fgets(line, sizeof(line), fpp);
	while (fgets(line, sizeof(line), fpp))
	{ //printf("%s line \n", line);
		lexs = getLexemes(line);
		toks = getTokens(line);
		//printf("lex %s  :  \t || tokes %s /n", lexs , toks);
	}
	//printf("\nLA DONE\n");

	//printf("%d %d",toks, lexs);
	fclose(fpp);
}

void Display()
{
	int ii;
	for (ii = 0; ii <= toks; ii++)
	{
		if (strcmp(stringParser[ii], "ID") == 0)
		{ //printf("lexems should be printed");
			printf("%s\n", Lexemes[ii]);
		}
		else
		{
			printf("%s\n", stringParser[ii]);
		}
	}
}

bool isNumber()
{
	if (strcmp(stringParser[i], "NUM") == 0)
	{
		return true;
	}
	return false;
}
bool Type(char *str)
{
	bool Typeflag = false;
	if ((strcmp(str, "int") == 0) || (strcmp(str, "bool") == 0) || (strcmp(str, "char") == 0) || (strcmp(str, "void") == 0))
	{
		Typeflag = true;
	}
	else
		Typeflag = false;
	return Typeflag;
}

bool isIdentifier(char *token)
{
	if (strcmp(token, "ID") == 0)
	{
		return true;
	}
	else
		return false;
}

void PRNT()
{

	if (strcmp(stringParser[i], ":") == 0)
	{
		tabNo++;
		addNode(tabNo, stringParser[i]);
		i++;
		if (isIdentifier(stringParser[i])) //<----
		{
			addNode(tabNo, Lexemes[i]);
			i++;

			if (strcmp(stringParser[i], ";") == 0)
			{
				addNode(tabNo, stringParser[i]);
				i++;
			}
			else
			{
				printf("missing ; in print stmnt\n");
				exit(1);
			}
		}
		else if (strcmp(stringParser[i], "STR") == 0) //<----
		{
			addNode(tabNo, stringParser[i]);
			i++;

			if (strcmp(stringParser[i], ";") == 0)
			{
				addNode(tabNo, stringParser[i]);
				i++;
			}
			else
			{
				printf("missing ; in print stmnt\n");
				exit(1);
			}
		}
		tabNo--;
	}
	else
		printf("ERROR !! missing :");
}
void SCN()
{
	tabNo++;

	if (strcmp(stringParser[i], ":") == 0)
	{
		addNode(tabNo, stringParser[i]);
		i++;

		if (isIdentifier(stringParser[i])) //<----
		{
			addNode(tabNo, stringParser[i]);

			i++;
			if (strcmp(stringParser[i], ";") == 0)
			{
				addNode(tabNo, stringParser[i]);
				i++;
			}
		}
	}
	tabNo--;
}
void CCId()
{
	if (strcmp(stringParser[i], ",") == 0)
	{
		tabNo++;
		addNode(tabNo, ",");

		addNode(tabNo, "CId");

		i++;
		CId();
		tabNo--;
	}
}
void CId() // cummulative Identifier
{
	tabNo++;
	if (Type(stringParser[i]))
	{

		addNode(tabNo, stringParser[i]);
		i++;
		if (isIdentifier(stringParser[i]))
		{
			addNode(tabNo, Lexemes[i]);
			addNode(tabNo, "CCId");
			i++;
			CCId();
		}
	}
	else if (isIdentifier(stringParser[i]))
	{
		addNode(tabNo, Lexemes[i]);
		addNode(tabNo, "CCId");
		i++;
		CCId();
	}
	tabNo--;
}
void PON()
{
	bool singleValDecl = false;
	if (strcmp(stringParser[i], ";") == 0)
	{
		tabNo++;
		addNode(tabNo, ";");
		tabNo--;
		i++;
		singleValDecl = true;
	}
	else if (strcmp(stringParser[i], ",") == 0)
	{
		tabNo++;
		addNode(tabNo, ",");
		addNode(tabNo, "CId");
		i++;
		CId();
		tabNo--;
		if (strcmp(stringParser[i], ";") == 0)
		{
			tabNo++;
			addNode(tabNo, ";");

			tabNo--;

			i++;
		}
	}
	else if (strcmp(stringParser[i], "(") == 0)
	{
		tabNo++;
		addNode(tabNo, "(");

		addNode(tabNo, "CId");

		i++;
		CId();
		tabNo--;
		if (strcmp(stringParser[i], ")") == 0)
		{
			tabNo++;
			addNode(tabNo, ")");

			tabNo--;

			FuncDecl = false;
			i++;
		}
	}
}

void RE()
{
	tabNo++;
	addNode(tabNo, "RE");
	N();
	RN();
	tabNo--;
}
void N()
{
	tabNo++;
	addNode(tabNo, "N");
	// printf("\n 1: %s " ,stringParser[i-2]  );
	// printf("\n 1: %s " ,stringParser[i-1]  );
	// printf("\n 1: %s " ,stringParser[i]  );
	// printf("\n 1: %s " ,stringParser[i+1]  );

	if (isIdentifier(stringParser[i]))
	{

		addNode(tabNo, Lexemes[i]);
		i++;
	}
	else if (isNumber())
	{

		addNode(tabNo, Lexemes[i]);
		i++;
	}
	else
	{

		printf("Error in RE");
	}
	tabNo--;
}
void RN()
{
	tabNo++;
	addNode(tabNo, "RN");
	if (strcmp(stringParser[i], "RLOP") == 0)
	{
		addNode(tabNo, Lexemes[i]);
		i++;
		N();
	}
	tabNo--;
}

void E()
{
	tabNo++;
	addNode(tabNo, "T");
	T();

	addNode(tabNo, "E1");
	E1();
	tabNo--;
}
void T()
{
	tabNo++;
	addNode(tabNo, "F");
	F();
	tabNo--;
}
void F()
{

	if (strcmp(stringParser[i], "LTRL") == 0)
	{
		tabNo++;
		addNode(tabNo, stringParser[i]);
		tabNo--;
		i++;
	}
	if (isIdentifier(stringParser[i]))
	{
		tabNo++;
		addNode(tabNo, Lexemes[i]);
		tabNo--;
		i++;
	}
	else if (isNumber())
	{
		tabNo++;
		addNode(tabNo, Lexemes[i]);
		tabNo--;
		i++;
	}
	else if (strcmp(stringParser[i], "(") == 0)
	{
		addNode(tabNo, "(");
		i++;
		tabNo++;
		addNode(tabNo, "E");
		E();
		tabNo--;
		if (strcmp(stringParser[i], ")") == 0)
		{
			addNode(tabNo, "E");

			i++;
		}
		else
			printf("ERROR ) missing");
	}
}
void E1()
{
	tabNo++;

	if (strcmp(stringParser[i], "AOP") == 0)
	{
		addNode(tabNo, Lexemes[i]);
		i++;

		addNode(tabNo, "T");
		T();
		addNode(tabNo, "E1");
		E1();
	}
	tabNo--;
}
void WHILE()
{
	tabNo++;

	if (strcmp(stringParser[i], "(") == 0)
	{
		addNode(tabNo, stringParser[i]);
		i++;
		RE();
		if (strcmp(stringParser[i], ")") == 0)
		{
			addNode(tabNo, stringParser[i]);
			i++;
		}
		else
		{
			printf("ERROR in While statement closing ) \n");
		}
	}
	else
	{
		printf("ERROR in While statement opening ( \n");
		exit(1);
	}
	tabNo--;
}
void statement()
{
	tabNo++;
	addNode(tabNo, "Statement");

	while (1)
	{
		bool flag = true;
		if (Type(stringParser[i])) // check if statemnet is of Function Declaration or variable declaration
		{
			// check if given statement is variable declaration or function declaration
			addNode(tabNo, stringParser[i]);
			i++;

			if (isIdentifier(stringParser[i]))
			{
				tabNo++;
				addNode(tabNo, Lexemes[i]);
				tabNo--;
				tabNo++;
				addNode(tabNo, "PON");
				i++;
				PON();
				tabNo--;
			}
		}
		else if (strcmp(stringParser[i], "print") == 0) // check if statemnet is of print
		{
			addNode(tabNo, "print");
			addNode(tabNo, "PRNT");
			i++;
			PRNT();
		}
		else if (strcmp(stringParser[i], "scan") == 0) // check if statemnet is of scan
		{
			addNode(tabNo, stringParser[i]);
			addNode(tabNo, "SCN");
			i++;
			SCN();
		}
		else if (strcmp(stringParser[i], "while") == 0) // check if statemnet is of scan
		{
			addNode(tabNo, "while");
			addNode(tabNo, "WHILE");
			i++;
			WHILE();
		}
		else if (strcmp(stringParser[i], "return") == 0) // check if statemnet is of scan
		{
			addNode(tabNo, "return");
			addNode(tabNo, "RTRN");
			i++;
			RTRN();
			tabNo--;
		}
		else if (strcmp(stringParser[i], "{") == 0) // check if statemnet is of scan
		{
			curlBrak[cbIndex] = 1;
			cbIndex++;
			addNode(tabNo, "{");
			tabNo++;
			i++;
		}
		else if (strcmp(stringParser[i], "}") == 0) // check if statemnet is of scan
		{
			if (curlBrak[cbIndex - 1] == 1)
			{
				cbIndex--;
				addNode(tabNo, "}");
				i++;
				tabNo--;
			}
		}
		else if (strcmp(stringParser[i], "$") == 0) // check for end of stringParser/code
		{
			flag = false;
			//printf(" Instruction Count\n");
			return;
		}
		else if (isIdentifier(stringParser[i]))
		{
			addNode(tabNo, Lexemes[i]);
			i++;
			if (strcmp(stringParser[i], "<-") == 0)
			{
				addNode(tabNo, stringParser[i]);
				i++;
				addNode(tabNo, "E");
				E();
				if (strcmp(stringParser[i], ";") == 0)
				{
					addNode(tabNo, stringParser[i]);
					i++;
				}
			}
		}
		else
		{
			// printf("ERROR"); printf("%s", stringParser[i]);
			i++;
			// exit(1);
		}
	}
	tabNo--;
}

void start()
{
	addNode(tabNo, "start");
	i = 1;
	statement();
}
int parser()
{

	char dollar[] = "$";
	int a;
	c++;
	r++;
	c = r;
	//printf("  r :%d\n" , r);
	//printf("  c :%d\n" , c);

	stringParser = (char **)malloc(r * sizeof(char *));
	for (a = 0; a < r; a++)
		stringParser[a] = (char *)malloc(c * sizeof(char));
	Lexemes = (char **)malloc(r * sizeof(char *));
	for (a = 0; a < r; a++)
		Lexemes[a] = (char *)malloc(c * sizeof(char));

	ExtractTokensAndLexemes(); //Display();

	Treefptr = fopen("parsetree.txt", "w");
	if (Treefptr == NULL)
	{
		printf("tree.txt FILE Error!");
		exit(1);
	}
	stringParser[toks][0] = dollar[0];

	start();

	return 0;
}

void tokenizing()
{
	int y = 0;
	int i, j, k;
	for (y = 0; y < strlen(buffer); y++)
	{
		if (buffer[y] == 60 && buffer[y + 1] == 45)
		{
			temp[0] = 60;
			temp[1] = 45;
			temp[2] = '\0';
			strcpy(string[count++], "<-");
			memset(temp, 0, strlen(temp));

			y++;
		}
		else if (buffer[y] == 39) //single quote
		{
			int o = 0;
			temp[0] = 39;
			y++;
			temp[1] = buffer[y++];
			temp[2] = 39;
			temp[3] = '\0';
			strcpy(string[count++], temp);
			memset(temp, 0, sizeof(temp));
		}
		else if (buffer[y] == 34)
		{
			int o = 0;
			temp[o++] = buffer[y++];

			for (y; !(buffer[y] == 34); y++)
			{
				temp[o++] = buffer[y];
			}

			temp[o++] = buffer[y];
			temp[o] = '\0';
			strcpy(string[count++], temp);
			memset(temp, 0, sizeof(temp));
		}
		else if (buffer[y] == '/' && buffer[y + 1] == '*')
		{
			for (y; !(buffer[y] == '*' && buffer[y + 1] == '/'); y++)
			{
			}
		}
		else
		{
			for (i = 0; i < 16; ++i)
			{

				if (buffer[y] == operators[i])
				{
					temp[0] = buffer[y];
					temp[1] = '\0';
					strcpy(string[count++], temp);
					memset(temp, 0, sizeof(temp));
				}
			}
		}
		if (isalnum(buffer[y]))
		{
			j = 0;

			for (j; isalnum(buffer[y]); j++)
			{
				buffer_[j] = buffer[y];
				y++;
			}
			buffer_[j] = '\0';
			y--;
			strcpy(string[count++], buffer_);
		}
		else if (buffer[y] == ' ' || buffer[y] == 9 || buffer[y] == 10 || buffer[y] == '\n')
		{
			//ki
		}
	}
}

void printTokenLexemePair(char *token, char *lexeme)
{
	fp1 = fopen("words.txt", "a");
	fprintf(fp1, "( %s , %s )\n", token, lexeme);
	fclose(fp1);
	r++;
}

void printTokLexemePair(char token, char *lexeme)
{
	fp1 = fopen("words.txt", "a");
	fprintf(fp1, "( %c , %s )\n", token, lexeme);
	fclose(fp1);
	r++;
}

bool isAssignemntOperator()
{
	if ((strcmp(string[index_], "<-") == 0))
	{
		return (true);
	}
	return (false);
}

bool isString(char ch)
{
	char t2[40];
	int i = 1;
	strcpy(t2, string[index_]);
	if (ch == 34)
	{

		for (i; i < strlen(string[index_]); i++)
		{
			if (t2[i] == 34)
				return true;
		}
	}
	return false;
}

bool isLtrl(char ch)
{
	char t1[10];
	if (ch == 39)
	{
		int i = 1;
		strcpy(t1, string[index_]);
		for (i; i < strlen(string[index_]); i++)
		{
			if (t1[i] == 39)
			{
				return true;
			}
		}
	}
	return false;
}

bool isComment(char ch)
{
	if (ch == '/' && buffer[right + 1] == '*')
	{
		int i;
		for (i = right + 2; i < len; i++)
		{
			if ((buffer[i] == '*' && buffer[i + 1] == '/'))
			{
				i++;
				i++;
				right = i;
				left = i;
				return true;
			}
		}
	}
	return false;
}

bool isRelationalOperator(char ch1, char ch2)
{
	//printf("isRelationalOperator %c%c   \n", ch1, ch2);

	if (
		(ch1 == '<' && ch2 == '=') ||
		(ch1 == '<' && ch2 == '=') ||
		(ch1 == '!' && ch2 == '=') ||
		(ch1 == '=' && ch2 == '=')

	)
	{
		//printf("isRelationalOperator %c%c   \n", ch1,ch2);
		return (true);
	}
	else if (ch1 == '<' || ch1 == '>')
	{
		//printf("isRelationalOperator %c  \n", ch1);
		return (true);
	}

	return (false);
}

bool isDelimiter(char ch)
{
	if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ',' || ch == ';' || ch == '>' || ch == '<' || ch == '=' || ch == '(' || ch == ')' || ch == ':' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '	')
		return (true);
	return (false);
}

bool isOperator(char ch) // Returns 'true' if the character is an OPERATOR.
{
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '=')
		return (true);
	return (false);
}

bool validIdentifier(char *str) // Returns 'true' if the string is a VALID IDENTIFIER.
{
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
		str[0] == '3' || str[0] == '4' || str[0] == '5' ||
		str[0] == '6' || str[0] == '7' || str[0] == '8' ||
		str[0] == '9' || (isDelimiter(str[0]) == true))
		return (false);
	return (true);
}

bool isKeyword(char *str) // Returns 'true' if the string is a KEYWORD.
{
	if (!strcmp(str, "if") || !strcmp(str, "else") ||
		!strcmp(str, "while") || !strcmp(str, "do") ||
		!strcmp(str, "break") || !strcmp(str, "bool") ||
		!strcmp(str, "continue") || !strcmp(str, "int") || !strcmp(str, "double") || !strcmp(str, "float") || !strcmp(str, "return") || !strcmp(str, "char") || !strcmp(str, "case") || !strcmp(str, "char") ||
		!strcmp(str, "sizeof") || !strcmp(str, "long") || !strcmp(str, "short") || !strcmp(str, "typedef") || !strcmp(str, "switch") || !strcmp(str, "unsigned") || !strcmp(str, "void") || !strcmp(str, "static") ||
		!strcmp(str, "struct") || !strcmp(str, "goto") || !strcmp(str, "scan") || !strcmp(str, "print"))
		return (true);
	return (false);
}

bool isInteger(char *str) // Returns 'true' if the string is an INTEGER.
{
	int i, len = strlen(str);

	if (len == 0)
		return (false);
	for (i = 0; i < len; i++)
	{
		if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
			return (false);
	}
	return (true);
}

bool isRealNumber(char *str) // Returns 'true' if the string is a REAL NUMBER.
{
	int i, len = strlen(str);
	bool hasDecimal = false;

	if (len == 0)
		return (false);
	for (i = 0; i < len; i++)
	{
		if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' && str[i] != '.' ||
			(str[i] == '-' && i > 0))
			return (false);
		if (str[i] == '.')
			hasDecimal = true;
	}
	return (hasDecimal);
}

bool isBrackets(char ch)
{
	bool flag = false;
	if (ch == '{' || ch == '}' || ch == '(' || ch == ')' || ch == ';' || ch == ':' || ch == ',')
	{
		flag = true;
	}
	return flag;
}

char *subString(char *str, int left, int right) // Extracts the SUBSTRING.
{
	int i;
	char *subStr = (char *)malloc(sizeof(char) * (right - left + 2));
	for (i = left; i <= right; i++)
		subStr[i - left] = str[i];
	subStr[right - left + 1] = '\0';
	return (subStr);
}

void updateSymbolTable(char *str, char *type)
{
	size_t len = 0;
	size_t read;
	char *line = NULL;
	int flag = 1;
	strcat(str, ",");
	strcat(str, type);
	idFptr = fopen("symboltable.txt", "r");
	if (!idFptr)
	{
		printf("Failed to open\n");
	}
	while ((read = getline(&line, &len, idFptr)) != -1)
	{
		line[strcspn(line, "\n")] = 0;

		if (strcmp(line, str) == 0)
			flag = 0;
	}
	fclose(idFptr);
	if (flag == 1 && (strcmp(type, " ") != 0))
	{
		idFptr = fopen("symboltable.txt", "a");
		fprintf(idFptr, "%s\n", str);
		fclose(idFptr);
	}
}

void LA() // Parsing the input STRING.
{
	idFptr = fopen("symboltable.txt", "w");
	fprintf(idFptr, "NAME,TYPE\n");
	fclose(idFptr);
	char temp[2];
	idFptr = fopen("symboltable.txt", "a");

	for (index_ = 0; index_ < count; index_++)
	{
		char ch = string[index_][0];
		char ch2 = string[index_ + 1][0];
		//printf(" ch1  %c || ch2 %c \n", ch, ch2);

		if (isBrackets(ch) == true)
		{
			printTokLexemePair(ch, "^");
			if (ch == ';')
			{
				memset(type, 0, sizeof(type[0]));
				strcpy(type, " ");
			}
		}
		else if (isLtrl(string[index_][0]) == true)
		{
			printTokenLexemePair("LTRL", string[index_]);
		}
		else if (isString(string[index_][0]) == true)
		{
			printTokenLexemePair("STR", string[index_]);
		}
		else if (isAssignemntOperator(string[index_]) == true)
		{
			printTokenLexemePair(string[index_], "^");
		}
		else if (isRelationalOperator(ch, ch2) == true)
		{

			if (ch == '<' && ch2 == '=')
			{
				printTokenLexemePair("RLOP", "<=");
				index_++;
			}
			else if (ch == '>' && ch2 == '=')
			{
				printTokenLexemePair("RLOP", ">=");
				index_++;
			}
			else if (ch == '=' && ch2 == '=')
			{
				printTokenLexemePair("RLOP", "==");
				index_++;
			}
			else if (ch == '!' && ch2 == '=')
			{
				printTokenLexemePair("RLOP", "!=");
				index_++;
			}
			else if (ch == '>')
			{
				printTokenLexemePair("RLOP", ">");
			}
			else if (ch == '<')
			{
				printTokenLexemePair("RLOP", "<");
			}
		}
		else if (isOperator(string[index_][0]) == true)
			printTokenLexemePair("AOP", string[index_]);

		else if (isKeyword(string[index_]))
		{
			if (!strcmp(string[index_], "int") || !strcmp(string[index_], "double") || !strcmp(string[index_], "float") || !strcmp(string[index_], "short") || !strcmp(string[index_], "char") || !strcmp(string[index_], "long") || !strcmp(string[index_], "void") || !strcmp(string[index_], "bool"))
				strcpy(type, string[index_]);
			printTokenLexemePair(string[index_], "^");
		}
		else if (isInteger(string[index_]))
			printTokenLexemePair("NUM", string[index_]);

		else if (isRealNumber(string[index_]))
			printTokenLexemePair("NUM", string[index_]);

		else if (validIdentifier(string[index_]) == true && isDelimiter(string[index_][0]) == false)
		{
			printTokenLexemePair("ID", string[index_]);
			updateSymbolTable(string[index_], type);
		}
		else
		{
			printf("'%s' IS NOT A VALID IDENTIFIER\n", string[index_]);
		}
	}

	//free(type);
	return;
}

void FillSymbolTable()
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *token;
	int index = 0;
	fp = fopen("translator-symboltable.txt", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1)
	{
		char *rest = line;
		char *tempS;
		char *type;
		int c = 0, tempIndex = 0;
		while ((token = strtok_r(rest, " , ", &rest)))
		{

			if (c == 0)
			{
				//tempS = token;
			}
			else if (c == 1)
			{

				//sprintf(type, "%s", token);
				type = token;
				//strcpy(type, token);
			}
			else if (c == 2)
			{
				tempIndex = atoi(token);
				c = -1;
			}
			c++;
		}
		if (strcmp(type, "int") == 0)
		{
			SymbolType[tempIndex][0] = INT;
		}
		else if (strcmp(type, "char") == 0)
		{
			SymbolType[tempIndex][0] = CHAR;
		}
		//AssignSymbolType(tempIndex, type);
		//sprintf(SymbolType[tempIndex], "%s", type);
		//strcpy(SymbolType[tempIndex], type);

		//printf("ooo\n");

		index++;
	}

	fclose(fp);
}
int getMachineCode()
{
	int instCount = 20;
	FILE *fptr;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int tIndex = 0, pc = 0, qColm = 0;
	// intitializing quadraple
	for (qColm = 0; qColm <= MaxMachineCodeLines; qColm++)
	{
		quadraple[qColm][0] = '\0';
	}
	FillSymbolTable();
	if ((fptr = fopen("machine-code.txt", "r")) == NULL)
	{
		printf("Error! machine-code.txt file cannot be opened");
		// Program exits if file pointer returns NULL.
		exit(1);
	}
	int i = 0;
	fscanf(fptr, "%d", &i);

	while ((read = getline(&line, &len, fptr)) != -1)
	{
		char *rest = line;
		char *token;
		int c = 0, tempIndex = 0;
		char *Index;
		while ((token = strtok_r(rest, " ", &rest)))
		{
			Index = token;
			read = strlen(token) - 1;
			Index[read] = '\0';
			sprintf(quadraple[atoi(Index)], "%s", rest);
			pc = atoi(Index);
			break;
		}
	}

	fclose(fptr);
	free(line);
	return pc;
}

void VM()
{
	PC = getMachineCode();
	//printf("\n Instruction Count %d \n", PC);
	//symbolTable has reference to identifiers
	//quadraple has machine code
	int tempIResult = 0;
	char *tempCResult;
	int src1, l;
	int src2;
	int dest;
	int opcode, srcN;
	char currInst[100];
	char src[3][100];
	int ip;
	for (ip = 0; ip <= PC; ip++)
	{ // printf("\n Instru %d \t", ip);

		if (quadraple[ip][0])
		{

			l = 0;
			// char *src1Str;
			// char *src2Str;
			// char *destStr;
			//memset(src1Str, '\0', 20);
			//memset(src2Str, '\0', 20);
			//memset(tempCResult, '\0', 20);
			//memset(destStr , '\0', 20);
			//printf("\n Instru %d \t", ip);
			sprintf(currInst, "%s", quadraple[ip]);
			//printf("%s", currInst);
			char *tok;
			char *rest = currInst;
			char *token = strtok(currInst, " ");
			opcode = atoi(token);
			srcN = 0;
			while (srcN < 3)
			{
				token = strtok(NULL, " ");
				strcpy(src[srcN], token);
				//RemoveChars( src[srcN] ,"'" );
				//printf(" srcArr  %s\t ", src[srcN]);
				srcN++;
			}
			dest = atoi(src[2]);

			if (opcode == IntermediateAssignment) //if assignment operator
			{
				memset(symbolTable[dest], '\0', 50 * sizeof(char));

				if (SymbolType[dest][0] == INT)
				{
					sprintf(symbolTable[dest], "%d", atoi(src[0]));

					//printf(" IntermediateAssignment val digit stored %d", atoi(symbolTable[dest]));
					//printf("digit stored %d", symbolTable[dest]);
				}
				else if (SymbolType[dest][0] == CHAR)
				{
				
				//printf(" IntermediateAssignment val stored %s", symbolTable[dest]);
				char toPrint='\0';
				toPrint =src[0][1] ;

				sprintf(symbolTable[dest], "%c", src[0][1]);
				}
			}
			else if (opcode == ADD) // if addition operator
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				tempIResult = atoi(symbolTable[src1]) + atoi(symbolTable[src2]);
				sprintf(symbolTable[dest], "%d", tempIResult);
				//printf(" ADD val stored %s", symbolTable[dest]);
			}
			else if (opcode == IADD) // if addition operator
			{						 //src1 is the var and src2 is the immidiate value to be added
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				tempIResult = atoi(symbolTable[src1]) + src2;
				sprintf(symbolTable[dest], "%d", tempIResult);
				//printf(" Intermediate ADD val stored %s", symbolTable[dest]);
			}
			else if (opcode == SUBTRACT) // if SUBTRACT operator
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				tempIResult = atoi(symbolTable[src1]) - atoi(symbolTable[src2]);
				sprintf(symbolTable[dest], "%d", tempIResult);
				//printf(" SUBTRACT val stored %s", symbolTable[dest]);
			}
			else if (opcode == ISUBTRACT) // if addition operator
			{							  //src1 is the var and src2 is the immidiate value to be added
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				tempIResult = atoi(symbolTable[src1]) - src2;
				sprintf(symbolTable[dest], "%d", tempIResult);
				//printf(" Intermediate ISUBTRACT val stored %s", symbolTable[dest]);
			}
			else if (opcode == MULTIPLY) // if MULTIPLY operator
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				tempIResult = atoi(symbolTable[src1]) * atoi(symbolTable[src2]);
				sprintf(symbolTable[dest], "%d", tempIResult);
				//printf(" MULTIPLY val stored %s", symbolTable[dest]);
			}
			else if (opcode == IMULTIPLY) // if addition operator
			{							  //src1 is the var and src2 is the immidiate value to be added
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				tempIResult = atoi(symbolTable[src1]) * src2;
				sprintf(symbolTable[dest], "%d", tempIResult);
				//printf(" Intermediate IMULTIPLY val stored %s", symbolTable[dest]);
			}
			else if (opcode == DIVIDE) // if DIVIDE operator
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				tempIResult = atoi(symbolTable[src1]) / atoi(symbolTable[src2]);
				sprintf(symbolTable[dest], "%d", tempIResult);
				//printf(" DIVIDE val stored %s", symbolTable[dest]);
			}
			else if (opcode == IDIVIDE) // if addition operator
			{							//src1 is the var and src2 is the immidiate value to be added
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				tempIResult = atoi(symbolTable[src1]) / src2;
				sprintf(symbolTable[dest], "%d", tempIResult);

				// printf("symbolTable[dest] bef %s \n", symbolTable[dest]);

				// // memset(symbolTable[dest], '\0', 50*sizeof(char));
				// printf("symbolTable[dest] after %s \n", symbolTable[dest]);
				// printf("symbolTable[dest] bef %s \n", symbolTable[dest]);

				//printf(" Intermediate IDIVIDE val stored %s\n", symbolTable[dest]);
				//printf(" Intermediate IDIVIDE val stored %d\n", tempIResult);
			}
			else if (opcode == IN)
			{ //char * strInput;
				gets(symbolTable[atoi(src[Src1])]);
				//printf(" cin>> %s \n", symbolTable[atoi(src[Src1])]);
			}
			else if (opcode == cout)
			{

				printf("%s \n", symbolTable[atoi(src[0])]);
			}
			else if (opcode == Icout)
			{
				char toPrint[50];
				int r = 0, c = 0;
				for (r = 0; src[0][r] != '\0'; r++)
				{
					if (src[0][r] != '"' && src[0][r] != 39)
					{
						toPrint[c] = src[0][r];
						c++;
					}
				}
				toPrint[c] = '\0';
				printf("%s \n", toPrint);
			}
			else if (opcode == ASSIGNMENT)
			{
				src1 = atoi(src[Src1]);
				int a;
				memset(symbolTable[dest], '\0', 50 * sizeof(char));

				for (a = 0; symbolTable[src1][a] != '\0'; ++a)
				{
					symbolTable[dest][a] = symbolTable[src1][a];
				}
				//printf("Assigned value %d", atoi( symbolTable[dest]) );
			}
			else if (opcode == IFG)
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) > atoi(symbolTable[src2]))
				{
					ip = dest;
				}
				//printf(" IFG ");
			}
			else if (opcode == IFGE) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) >= atoi(symbolTable[src2]))
				{
					ip = dest;
				}
				//printf(" IFGE ");
			}
			else if (opcode == IFL) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) < atoi(symbolTable[src2]))
				{
					ip = dest;
				}
				//printf(" IFL ");
			}
			else if (opcode == IFLTE) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) <= atoi(symbolTable[src2]))
				{
					ip = dest;
				}
				//printf(" IFLTE ");
			}
			else if (opcode == IIFG)
			{
				//printf("opcode %d IIFGE \n", opcode);
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) > src2)
				{
					ip = dest;
				}
			}
			else if (opcode == IIFGE) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) >= src2)
				{
					ip = dest;
				}
				//printf(" IIFGE ");
			}
			else if (opcode == IIFL) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) < src2)
				{
					ip = dest;
				}
			}
			else if (opcode == IIFLTE) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) <= src2)
				{
					ip = dest;
				}
				//printf(" IIFLTE ");
			}
			else if (opcode == IFEQ) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				//printf("src1 %d , src %d \n", atoi(symbolTable[src1]) ,atoi(symbolTable[src1]));
				if (atoi(symbolTable[src1]) == atoi(symbolTable[src1]))
				{
					ip = dest;
				}
				//	printf(" IFEQ ");
			}
			else if (opcode == IIFEQ) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) == src2)
				{
					ip = dest;
				}
				//printf(" IIFEQ ");
			}
			else if (opcode == IFNEQ) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				//printf("src1 %d , src %d \n", atoi(symbolTable[src1]) ,atoi(symbolTable[src1]));
				if (atoi(symbolTable[src1]) != atoi(symbolTable[src1]))
				{
					ip = dest;
				}
				//	printf(" IFEQ ");
			}
			else if (opcode == IIFNEQ) //
			{
				src1 = atoi(src[Src1]);
				src2 = atoi(src[Src2]);
				if (atoi(symbolTable[src1]) != src2)
				{
					ip = dest;
				}
				//printf(" IIFEQ ");
			}
			else if (opcode == GOTO)
			{
				src1 = atoi(src[Src1]);
				ip = src1;
				//printf(" GOTO ");
			}
		}
	}
}

int main(int argc, char **argv)
{
	fp1 = fopen("words.txt", "w");
	fprintf(fp1, "( Token , Lexeme )\n");
	fclose(fp1);
	FILE *fp;
	int i, j = 0;

	fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("error while opening words.txt file\n");
		exit(0);
	}

	type = malloc((size + 1) * sizeof(*type));
	fseek(fp, 0, SEEK_END); /* Go to end of file */
	size = ftell(fp);
	rewind(fp);
	buffer = malloc((size + 1) * sizeof(*buffer));
	fread(buffer, size, 1, fp); /* Read 1 chunk of size bytes from fp into buffer */
	buffer[size] = '\0';

	char *str;
	char *str2;
	tokenizing();

	LA(); // calling the LExical Analyzer
	fclose(fp);
	parser(); // calling Parser
	fclose(Treefptr);

	Translator(); // calling Translator
	GenerateMachineCode();
	VM();
	return 0;
}
