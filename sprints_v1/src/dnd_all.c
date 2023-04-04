
#include "../headers/commonheaders.h"


//Generate Id

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

//register the user details 

User *registerUser(char name[], char mob[], char password[], User *f)
{
    char *uidgen = malloc(sizeof(char) * 6);
    generateId(uidgen, 6);
    printf("Your U-ID is : %s.( Please keep it copied )\n\n", uidgen);
    User *current = (User *)malloc(sizeof(User));
    User *previous = f;
    strcpy(current->name, name);
    strcpy(current->uid, uidgen);
    strcpy(current->mob, mob);
    strcpy(current->password, password);
    FILE *file = fopen("users.txt", "a+");
    if(file==NULL)
    {
	    perror("file open error");
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

    // User *temp;

    char path[25] = "";
    char base[10] = "./status/";
    strcpy(path, base);
    strcat(path, uidgen);
    FILE *fpdnd = fopen("path.txt", "a+");
    if(fpdnd == NULL)
    {
	    perror("file open error dnd:");
	    exit(EXIT_FAILURE);
    }
    Selective dnd;

    dnd.status = 0;
    strcpy(dnd.uid, uidgen);
    memset(dnd.phones, '\0', 256);
    strcpy(dnd.phones, "");
    fwrite(&dnd, sizeof(Selective), 1, fpdnd);

    fclose(fpdnd);

    // printf("%s", path);

    return previous;
}


//Login the user

int loginUser(char uid[], char password[], User *f)
{
    User *usrs = f;
    while (usrs != NULL)
    {
        if ((strcmp(usrs->uid, uid) == 0 && strcmp(usrs->password, password) == 0))
        {
            // printf("Logged In!!");
            return 1;
        }
        usrs = usrs->next;
    }
    return 0;
}

int globalDnd=0;


//DND Function


User *dndInit()
{
    FILE *fp_ = fopen("globaldnd.txt", "r+");
 
    if(fp_==NULL)
    {
	    perror("file handled error\n");
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

    fp = fopen("users.txt", "r");
    if (fp == NULL)
    {
        perror("\n\tfopen() ");
        return NULL;
    }
       
                printf("\nok\n");
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
        // head = newNode;
	
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

//Global DND

int updateGlobal(int status)
{
    if (globalDnd == status)
    {
        printf("\tCouldn't change the status to itself\n");
        sleep(4);
        return -1;
    }
    FILE *fp = fopen("globaldnd", "w");
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

//Selective DND


int updateSelective(char uid[], int status)
{
    if (globalDnd == 1)
    {
        printf("\nGlobal DND is ON. \ncouldnt Change Anything.\n");
        sleep(4);
        return -1;
    }
    char path[25] = "";
    char base[10] = "status.txt";
    strcpy(path, base);
    strcat(path, uid);
    FILE *fp = fopen("path.txt", "r");
    if(fp==NULL)
    {
	    perror("fileope error\n");
	    exit(EXIT_FAILURE);
    }

    Selective sel;
    fread(&sel, sizeof(Selective), 1, fp);
    if (sel.status == status)
    {
        printf("\nCouldn't change the status to itself\n");
        sleep(4);
        return -1;
    }
    else
    {
        if (status == 1)
        {
            printf("\nEnter Phone numbers you want to add:");
            memset(sel.phones, '\0', 256);
            fgets(sel.phones, 255, stdin);
        }
        else
        {
            memset(sel.phones, '\0', 256);
        }
    }
    fclose(fp);

    sel.status = status;
    FILE *fpdnd = fopen(path, "wb");
    fwrite(&sel, sizeof(Selective), 1, fpdnd);
    fclose(fpdnd);
    printf("Selective dnd is on for %s:", sel.phones);
    sleep(2);
    return 0;
}

//It checks the connection of user

int connectUser(char f_uid[], char t_uid[], User *head)
{
    if (globalDnd == 1)
    {
        printf("DND on Globally. Couldn't connect\n");
        sleep(4);
        return -1;
    }
    else if(globalDnd == 0)
    {
	    printf("DND has not been activated\n");
	    sleep(3);
	    return -1;
    }

    else
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
            if (strstr(t.phones, me) != NULL)
            {
                printf("You are blocked by the receiver. Couldn't connect.\n");
                sleep(4);
                return -1;
            }
            else
            {
                printf("Connection Estblished\n");
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

//Display the registered user

void showUsers(User *f)
{
    User *user = f;
    while (user != NULL)
    {
        printf("\t%s\t%s\t%s\n", user->uid, user->name, user->mob);
        user = user->next;
    }
}

// void doFree(Users *f)
// {
//     free(f);
// }


//Tokenizes details

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

//Remove Spaces 

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
