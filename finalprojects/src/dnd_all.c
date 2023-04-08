/********************************************************************************************************************************************************
* * FILE NAAME: dnd_all.c
* * 
* *DESCRIPTION: This file contains all the necessary requirements for dnd.
* *
* *REVISION HISTORY:
* *DATE					         NAME				REFERENCE 				REASON
---------------------------------------------------------------------------------------------------------------------------------------------------------
**10/04/2023 		                	Group-6				new file				Initial 
*********************************************************************************************************************************************************
* *								STANDARD HEADER FILES
************************************************************************************************************************************************************/

#include "../inc/common.h"
#include "../inc/log.h"

/************************************************************************************************************************************************************
**
** FUNCTION NAME: generateId
**
** DESCRIPTION:  It generates the ID for the user.
**
** RETURNS:  SUCCESS 
************************************************************************************************************************************************************/

static char *generateId(char *str, size_t size)
{
	const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ012456789";
	if (size)
	{
		--size;
		srand(time(0));
		for (size_t n = 0; n < size; n++)
		{
			int key = rand() % (int)(sizeof charset - 1);
			str[n] = charset[key];
		}
		str[size] = '\0';
	}
	return str;
}

/************************************************************************************************************************************************************
**
** FUNCTION NAME: registerUser
**
** DESCRIPTION:  It registers the details of user.
**
** RETURNS:  SUCCESS
************************************************************************************************************************************************************/

User *registerUser(char name[], char mob[], char password[], User *f)
{
	char *uidgen = malloc(sizeof(char) * 6);
	generateId(uidgen, 6);
    	printf("Your Generated USER-ID is : %s.( Please keep it copied )\n\n", uidgen);
    	User *current = (User *)malloc(sizeof(User));
    	User *previous = f;
    	strcpy(current->name, name);
    	strcpy(current->uid, uidgen);
    	strcpy(current->mob, mob);
    	strcpy(current->password, password);
    	FILE *file = fopen("./data/users.txt", "a+");
	if(file==NULL)
	{
		perror("file open error");
	    	fclose(file);
	    	exit(EXIT_FAILURE);
    	}
	if (file != NULL)
    	{
        	fprintf(file, "%s;%s;%s;%s", current->uid, current->name, current->password, current->mob);
    	}
	fclose(file);
	current->next = NULL;
	if (f == NULL)
		f = current;
	else
	{
		while (f->next != NULL)
		f = f->next;
		f->next = current;
    	}
	
	char path[25] = "";
    	char base[10] = "./status/";
    	strcpy(path, base);
    	strcat(path, uidgen);
    	FILE *fpdnd = fopen(path, "a+");
	if(fpdnd == NULL)
	{
		perror("file open error dnd:");
	    	fclose(fpdnd);
	    	exit(EXIT_FAILURE);
    	}
	
	
	Selective dnd;
       	dnd.status = 0;
	strcpy(dnd.uid, uidgen);
	memset(dnd.phones, '\0', 256);
	strcpy(dnd.phones, "");
    	fwrite(&dnd, sizeof(Selective), 1, fpdnd);

    	fclose(fpdnd);


    	return previous;
}

/************************************************************************************************************************************************************
**
** FUNCTION NAME: loginUser
**
** DESCRIPTION:  It will login based on user details.
**
** RETURNS:  SUCCESS 
************************************************************************************************************************************************************/


int loginUser(char uid[], char password[], User *f)
{
	User *usrs = f;
	while (usrs != NULL)
	{
        	if ((strcmp(usrs->uid, uid) == 0 && strcmp(usrs->password, password) == 0))
        	{
            	return 1;
        	}
		usrs = usrs->next;
	}
	
	return 0;
}

int globalDnd=0;


/************************************************************************************************************************************************************
**
** FUNCTION NAME: dndInit
**
** DESCRIPTION:  It does dnd function.
**
** RETURNS:  SUCCESS 
************************************************************************************************************************************************************/


User *dndInit()
{
	FILE *fp_ = fopen("./data/globaldnd.txt", "r+");
	if(fp_==NULL)
	{
		perror("file handled error\n");
	    	fclose(fp_);
	    	exit(EXIT_FAILURE);
	}
	
	fscanf(fp_, "%d",&globalDnd);
	fclose(fp_);
    	FILE *fp = NULL;
    	User *newNode = NULL;
    	User *head = NULL;
    	User *dd=NULL;
    	int _fSize = 0;
    	char tmpBuff[1024] = {'\0',};
	fp = fopen("./data/users.txt", "r");
	if (fp == NULL)
	{
		perror("\n\tfopen() ");
		fclose(fp);
		return NULL;
	}
	
	fseek(fp, 0L, SEEK_SET);
	fseek(fp, 0L, SEEK_END);
	
	_fSize = ftell(fp);
	if (_fSize == 0) /* No records */
	{
		head = NULL;
	}
	else
	{
		fseek(fp, 0L, SEEK_SET);
		memset(tmpBuff, '\0', 1024);
		
		while(fgets(tmpBuff, 1024, fp))
		{
			if (head == NULL) /* first record */
			{
				newNode = (User *)malloc(sizeof(User));
				newNode->next = NULL;
				head = newNode;
				dd = newNode;
				tokenize(newNode, tmpBuff);
            		}
			else /* rest of the records */
            		{
				newNode = (User *)malloc(sizeof(User));
				newNode->next = NULL;
                		dd->next = newNode;
                		tokenize(newNode, tmpBuff);
                		dd = dd->next;
            		}
        	}
    	}
	
	fclose(fp);
    	return head;
}

/************************************************************************************************************************************************************
**
** FUNCTION NAME: updateGlobal
**
** DESCRIPTION:  It checks dnd status.
**
** RETURNS:  SUCCESS
************************************************************************************************************************************************************/

int updateGlobal(int status)
{
	if (globalDnd == status)
    	{
       		printf("\tCouldn't change the status to itself\n");
        	sleep(4);
       		 return -1;
    	}
	FILE *fp = fopen("./data/globaldnd.txt", "w");
    	fprintf(fp, "%d", status);
    	fclose(fp);
    	globalDnd = status;
	
	if(status == 1)
    	printf("\tGlobal DND is Activated.\n");
    	else
    	printf("\tGlobal DND is De_activated.\n");	    
	
	sleep(4);
    	return 0;
}

/**************************************************************************
**
** FUNCTION NAME: updateSelective
**
** DESCRIPTION:  It selects the dnd status.
**
** RETURNS:  SUCCESS 
*****************************************************************************/



int updateSelective(char uid[], int status)
{
	int flag=1;
	if (globalDnd == 1)
    	{
		printf("\nGlobal DND is ON. \ncouldnt Change Anything.\n");
        	sleep(4);
        	return -1;
    	}
	char path[25] = "";
    	char base[10] = "./status/";
    	strcpy(path, base);
    	strcat(path, uid);
    	FILE *fp = fopen(path, "rb");
    	if(fp==NULL)
    	{
		perror("fileope error\n");
	    	fclose(fp);
	    	exit(EXIT_FAILURE);
	}
	Selective sel;
	fread(&sel, sizeof(Selective), 1, fp);
	if (sel.status == status)
	{
		printf("\nCouldn't change the status to itself\n");
        	sleep(4);
		fclose(fp);
        	return -1;
	}
	else
	{
		if (status == 1)
		{
			printf("\nEnter Phone numbers you want to add:");
			memset(sel.phones, '\0', 256);
			fgets(sel.phones, 255, stdin);
	    		flag=0;	
        	}
        	if(status==0)
        	{

	    		printf("selective dnd is off for %s :",sel.phones);
			memset(sel.phones, '\0', 256);
			sleep(2);
		}
    	}
	
	fclose(fp);
	sel.status = status;
	FILE *fpdnd = fopen(path, "wb");
	fwrite(&sel, sizeof(Selective), 1, fpdnd);
	fclose(fpdnd);
    	if(flag==0)
		printf("Selective dnd is on for %s:", sel.phones);
	sleep(2);
	return 0;
}

/************************************************************************************************************************************************************
**
** FUNCTION NAME: connectUser
**
** DESCRIPTION:  It checks if we want to connect one user to another.
**
** RETURNS:  SUCCESS
************************************************************************************************************************************************************/

int connectUser(char f_uid[], char t_uid[], User *head)
{
	int flag2=0;
	int flag=0;
	if (globalDnd == 1)
    	{
        	printf("DND on Globally. Couldn't connect\n");
		flag=1;
        	sleep(4);
        	return -1;
    	}
	else if(flag==0);
    	{
		
		Selective f, t;
		char path[25] = "";
       		char base[10] = "./status/";
        	strcpy(path, base);
        	strcat(path, f_uid);
		
		FILE *fp = fopen(path, "r");
        	fread(&f, sizeof(Selective), 1, fp);
        	fclose(fp);

        	strcpy(path, "");
        	strcpy(path, base);
        	strcat(path, t_uid);
        	FILE *fp_ = fopen(path, "r");
        	fread(&t, sizeof(Selective), 1, fp_);
        	fclose(fp_);
	
        	char *me = getMobileFromId(f_uid, head);
		
		if (f.status == 1)
        	{
			printf("Your DND is ON. Couldn't connect.\n");
            		sleep(4);
            		return -1;
        	}
        	else if (t.status == 1)
        	{
			if (strcmp(t.phones, me) ==0)
            		{
                		printf("You are blocked by the receiver. Couldn't connect.\n");
                		sleep(4);
                		return -1;
           	 	}	
            		else
            		{
                		printf("you are free to call\n");
                		sleep(4);
                		return 0;
            		}
        	}
        	else
       	 	{
            		printf("Connection Estblished\n");
            		sleep(4);
        	}
	}
	
	return 0;
}

/************************************************************************************************************************************************************
**
** FUNCTION NAME: showUser
**
** DESCRIPTION:  It displays all the user details registered.
**
** RETURNS:  SUCCESS 
************************************************************************************************************************************************************/

void showUsers(User *f)
{
	User *user = f;
    	while (user != NULL)
    	{
        	printf("\t%s\t%s\t%s\n", user->uid, user->name, user->mob);
        	user = user->next;
    	}
}



/************************************************************************************************************************************************************
**
** FUNCTION NAME: tokenize
**
** DESCRIPTION:  It tokenizes the details.
**
** RETURNS:  SUCCESS 
************************************************************************************************************************************************************/

int tokenize(User *usr, char *tmpBuff)
{
	char *tokens;
	tokens = strtok(tmpBuff, ";");
    	removeLeading(tokens, usr->uid);

    	tokens = strtok(NULL, ";");
    	removeLeading(tokens, usr->name);

    	tokens = strtok(NULL, ";");
    	removeLeading(tokens, usr->password);
    	removeTrailing(usr->password);
    	tokens = strtok(NULL, ";");

    	removeLeading(tokens, usr->mob);
    	removeTrailing(usr->mob);
    	usr->mob[9] = '\0';
	return 0;
}

/************************************************************************************************************************************************************
**
** FUNCTION NAME: removeTrailing
**
** DESCRIPTION:  It removes spaces.
**
** RETURNS:  SUCCESS 
************************************************************************************************************************************************************/

void removeTrailing(char *str)
{
	if ((str[strlen(str) - 1] == ' ' || str[strlen(str) - 1] == '\t' || str[strlen(str) - 1] == '\n'))
    	{
        	str[strlen(str) - 1] = '\0';
    	}
}
	
void removeLeading(char *str, char *str1)
{
	int idx = 0, j, k = 0;
	while (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\n')
    	{
        	idx++;
    	}
	for (j = idx; str[j] != '\0'; j++)
    	{
        	str1[k] = str[j];
        	k++;
    	}
	
	str1[k] = '\0';
}

char *getMobileFromId(char *uid, User *f)
{
    	User *t = f;
    	while (t != NULL)
    	{
        	if (strcmp(t->uid, uid) == 0)
        	{
            	return t->mob;
        	}
		t = t->next;
    	}
	
	return "";
}

/************************************************************************************************************************************************************
**
** FUNCTION NAME: checkPhone
**
** DESCRIPTION:  It work for Number Validation.
**
** RETURNS:  SUCCESS
************************************************************************************************************************************************************/

int checkPhone(char *phone) {
	if(strlen(phone) != 11){
		printf("Phone number should be 10 digits only\n");
		return -1;

	}
	else
	{
		if(phone[0]=='0')
			return -1;
	}
	return 1;
}
