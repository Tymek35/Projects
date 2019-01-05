#include <limits.h>
#include "tree.h"

/*Długość wczytywanej linii z komendą, jeżeli linia bedzię dłuższa to komenda
  na pewno jest niepoprawna.*/
#define L 27

//Funkcja wczytująca polecenie.
bool getLine(char line[L])
{
    char c=getchar();
    line[0]=c;
    if(c!=EOF && c!='#' && c!='\n'){
        int i=1;
        while(i<L && c!='\n'){
            c=getchar();
            line[i]=c;
            ++i;
        }
        if(i==L){
            line[0]='E';
            while(c!='\n') c=getchar();
            return true;
        }
        return true;
    }
    else if(c=='#' || c=='\n'){
        while(c!='\n') c=getchar();
        return true;
    }
    else return false;
}

//Pięć funkcji pomocniczych, które wyciągają potrzebne dane z wczytanej linii z poleceniem.
bool addUser_H(char line[L], Ttree *users[MAX_USERID])
{
    if(line[7]==' ' && line[8]>='0' && line[8]<='9'){
        int parentUserId=atoi(&line[8]);
        int i=8;
        while(line[i]>='0' && line[i]<='9') ++i;
        if(line[i]==' ' && line[i+1]>='0' && line[i+1]<='9'){
            int userId=atoi(&line[i+1]);
            ++i;
            while(line[i]>='0' && line[i]<='9') ++i;
            if(userId>0 && userId<MAX_USERID && parentUserId>=0 && parentUserId<MAX_USERID && line[i]=='\n'){
                if(!addUser(parentUserId, userId, users)) return false;
                return true;
            }
            else{
                printf("ERROR\n");
                return true;
            }
        }
        else{
            printf("ERROR\n");
            return true;
        }
    }
    else{
        printf("ERROR\n");
        return true;
    }
}

bool addMovie_H(char line[L], Ttree *users[MAX_USERID])
{
    if(line[8]==' ' && line[9]>='0' && line[9]<='9'){
        int userId=atoi(&line[9]);
        int i=9;
        while(line[i]>='0' && line[i]<='9') ++i;
        if(line[i]==' ' && line[i+1]>='0' && line[i+1]<='9'){
            long long int movieRating=atoll(&line[i+1]);
            ++i;
            while(line[i]>='0' && line[i]<='9') ++i;
            if(userId>=0 && userId<MAX_USERID && movieRating>=0 && movieRating<=INT_MAX && line[i]=='\n'){
                if(!addMovie(userId, (int)movieRating, users)) return false;
                return true;
            }
            else{
                printf("ERROR\n");
                return true;
            }
        }
        else{
            printf("ERROR\n");
            return true;
        }
    }
    else{
        printf("ERROR\n");
        return true;
    }
}

void delMovie_H(char line[L], Ttree *users[MAX_USERID])
{
    if(line[8]==' ' && line[9]>='0' && line[9]<='9'){
        int userId=atoi(&line[9]);
        int i=9;
        while(line[i]>='0' && line[i]<='9') ++i;
        if(line[i]==' ' && line[i+1]>='0' && line[i+1]<='9'){
            long long int movieRating=atoll(&line[i+1]);
            ++i;
            while(line[i]>='0' && line[i]<='9') ++i;
            if(userId>=0 && userId<MAX_USERID && movieRating>=0 && movieRating<=INT_MAX && line[i]=='\n'){
                delMovie(userId, (int)movieRating, users);
            }
            else{
                printf("ERROR\n");
            }
        }
        else{
            printf("ERROR\n");
        }
    }
    else{
        printf("ERROR\n");
    }
}

void delUser_H(char line[L], Ttree *users[MAX_USERID])
{
     if(line[7]==' ' && line[8]>='0' && line[8]<='9'){
        int userId=atoi(&line[8]);
        int i=8;
        while(line[i]>='0' && line[i]<='9') ++i;
        if(line[i]=='\n' && userId>0 && userId<MAX_USERID){
            delUser(userId, users);
        }
        else{
            printf("ERROR\n");
        }
     }
    else{
        printf("ERROR\n");
    }
}

void marathon_H(char line[L], Ttree *users[MAX_USERID])
{
    if(line[8]==' ' && line[9]>='0' && line[9]<='9'){
        int userId=atoi(&line[9]);
        int i=9;
        while(line[i]>='0' && line[i]<='9') ++i;
        if(line[i]==' ' && line[i+1]>='0' && line[i+1]<='9'){
            long long int k=atoll(&line[i+1]);
            ++i;
            while(line[i]>='0' && line[i]<='9') ++i;
            if(userId>=0 && userId<MAX_USERID && k>=0 && k<=INT_MAX && line[i]=='\n'){
                marathon(userId, (int)k, users);
            }
            else{
                printf("ERROR\n");
            }
        }
        else{
            printf("ERROR\n");
        }
    }
    else{
        printf("ERROR\n");
    }
}

/*Funkcja okresla jakie polecenie zostalo wpisane. Jeżeli
  w wywołanej operacjii malloc nie zadziała to zwróci false.*/
bool whichOperation(char line[L], Ttree *users[MAX_USERID])
{
    if(line[0]=='a'){
        if(line[3]=='U'){
            if(!addUser_H(line, users)) return false;
            return true;
        }
        else if(line[3]=='M'){
            if(!addMovie_H(line, users)) return false;
            return true;
        }
        else{
            printf("ERROR\n");
            return true;
        }
    }
    else if(line[0]=='d'){
        if(line[3]=='M'){
            delMovie_H(line, users);
            return true;
        }
        else if(line[3]=='U'){
            delUser_H(line, users);
            return true;
        }
        else{
            printf("ERROR\n");
            return true;
        }
    }
    else if(line[0]=='m'){
        marathon_H(line, users);
        return true;
    }
    else if(line[0]=='#' || line[0]=='\n') return true;
    else{
        printf("ERROR\n");
        return true;
    }
}

int main(void)
{
    Ttree *users[MAX_USERID];
    setUsers(users);
    Ttree *root=(Ttree*)malloc(sizeof(Ttree));
    if(root==NULL) return 1;
    root->films=NULL;
    root->rightSon=NULL;
    root->leftSon=NULL;
    root->rightBrother=NULL;
    root->leftBrother=NULL;
    users[0]=root;

    char line[L];

    while(getLine(line)){
        //Gdy ktoryś malloc nie zadziała program jest przerywany.
        if(!whichOperation(line, users)){
            delAll(users);
            return 1;
        }
    }

    delAll(users);

    return 0;
}
