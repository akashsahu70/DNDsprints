/*********************************************************************************************************************************
* * FILE NAME: common.h
* *
* *DESCRIPTION: This file contains all the header files required for implementation.
* *
* *REVISION HISTORY:
* *DATE                 NAME                REFERENCE               REASON
-----------------------------------------------------------------------------------------------------------------------
* *                    Group-6              new file                Initial
***********************************************************************************************************************
* *                                     STANDARD HEADER FILES
***********************************************************************************************************************************/
#if !defined(HEADERS)
#define HEADERS -1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <locale.h>
#include <sys/time.h>

typedef struct user
{
    char uid[10];
    char name[10];
    char password[10];
    char mob[15];
    struct user *next;
} User;

typedef struct selective
{
    char uid[10];
    int status;
    char phones[256];
} Selective;

User *dndInit();
int updateGlobal(int status);
int updateSelective(char uid[], int status);
int connectUser(char f_uid[], char t_uid[], User *f);
User *registerUser(char name[], char mob[], char password[], User *f);
int loginUser(char uid[], char password[], User *f);
void showUsers(User *f);
void doFree(User *f);

int tokenize(User *usr, char *tmpBuff);
void removeLeading(char *str, char *str1);
void removeTrailing(char *str);
char *getMobileFromId(char *uid, User *f);
int checkPhone(char *phone);
#endif // HEADERS
