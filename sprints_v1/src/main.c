
#include "../inc/commonheaders.h"
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
void clrscr(void)
{
    system("clear");
}
int main(int argc, char const *argv[])
{
    User *f = NULL;
    f = (User *)malloc(sizeof(User));
    int ch;
    clrscr();
    f = dndInit();
    printf("\t******Welcome TO The DND Implementation*****\n");
    printf("<><><><><><><><> DO NOT DISTURB <><><><><><><><>\n"); 
    printf("\tLogin \t\t(0)\n");
    printf("\tRegister\t(1)\n");
    printf("\tShow Users\t(2)\n\n");
    printf("\tEnter Your Choice:");
    scanf("%d", &ch);
    getchar();
   // printf("hi\n");
    int status = 0;
    char loggedInUserId[10]; 
    if (ch == 0)
    {
        char uid[10];
        char pwd[10];
        printf("\tEnter Your  USER-ID:");
        fgets(uid, sizeof(uid), stdin);
        uid[strlen(uid) - 1] = '\0';

        printf("\tEnter Your  PASSWORD:");
        fgets(pwd, sizeof(pwd), stdin);
        pwd[strlen(pwd) - 1] = '\0';

        status = loginUser(uid, pwd, f);
        if (status == 1)
        {
            printf("\tLogged In\n");
            strcpy(loggedInUserId, uid);
        }
        else
            printf("\tIncorrect User-id or Password\n");
    }
    else if (ch == 1)
    {
        char uname[10];
        char pwd[10];
        char phone[14];
        memset(phone, '\0', 10);
        memset(pwd, '\0', 10);
        memset(uname, '\0', 10);

        printf("\tEnter your NAME:");
        fgets(uname, sizeof(uname), stdin);
        uname[strlen(uname) - 1] = '\0';

        printf("\tEnter your PASSWORD:");
        fgets(pwd, sizeof(pwd), stdin);
        pwd[strlen(pwd) - 1] = '\0';

        printf("\tEnter your PHONE NO:");
        fgets(phone, sizeof(phone), stdin);
        phone[strlen(phone)] = '\0';

        f = registerUser(uname, phone, pwd, f);
          
        exit(0);
    }
    else if (ch == 2)
    {
        clrscr();
        sleep(1);
        printf("\tU-ID\tName\tMobile No\n");
        printf("\t-------------------------------------\n");
        showUsers(f);
    }
    else
    {
        printf("\tWrong Option!!");
        exit(0);
    }
  
    while (status == 1)
    {
        clrscr();
        int c, sc;
        printf("\n\n\t**********main-menu***********************\n");
       
        printf("\tSelect Global DND\t(1)\n");
        printf("\tSelect Selective DND\t(2)\n");
        printf("\tConnect User\t\t(3)\n");
        printf("\tQuit\t\t\t(4)\n");
        printf("\tChoosen Option:");
        scanf("%d", &c);
        getchar();

        // printf("\n\n");
        switch (c)
        {
        case 1:
            printf("\t========== GLOBAL =================\n");
            printf("\tGlobal DND\n\n");
            printf("\t\tTo turn ON\t(1)\n");
            printf("\t\tTo turn OFF\t(0)\n");
            printf("\tYour Choice:");
            scanf("%d", &sc);
            getchar();
            updateGlobal(sc);
            break;
        case 2:
            printf("\t========== SELECTIVE =============\n");
            printf("\tSelective DND\n\n");
            printf("\t\tTo turn ON\t(1)\n");
            printf("\t\tTo turn OFF\t(0)\n");
            printf("\tYour Choice:");
            scanf("%d", &sc);
            getchar();
            updateSelective(loggedInUserId, sc);
            break;
        case 3:
            printf("\tConnect User\n");
            printf("\t\tEnter your  USER-ID:");
            char uid[10];
            fgets(uid, sizeof(uid), stdin);
            uid[strlen(uid) - 1] = '\0';
            connectUser(loggedInUserId, uid, f);
            break;

        default:
            if (c != 4)
                printf("Invalid Option!!\n");
            exit(0);
        }
    }

    free(f);
    return 0;
}
