#include "tree.h"

void setUsers(Ttree *users[MAX_USERID])
{
    int i;
    for(i=1;i<MAX_USERID;++i){
        users[i]=NULL;
    }
}

//Usuwa całe stworzone podczas działania programu drzewo.
void delAll(Ttree *users[MAX_USERID])
{
    int i;
    for(i=0;i<MAX_USERID;++i){
        if(users[i]!=NULL){
            delList(users[i]->films);
            if(users[i]->leftSon!=users[i]->rightSon){
                free(users[i]->leftSon);
                free(users[i]->rightSon);
            }
            free(users[i]);
        }
    }
}

//Zwraca false tylko, gdy malloc nie zadziała.
bool addUser(int parentUserId, int userId, Ttree *users[MAX_USERID])
{
    if(users[userId]==NULL && users[parentUserId]!=NULL){
        if(users[parentUserId]->leftSon==users[parentUserId]->rightSon){
            (users[parentUserId]->leftSon)=(Ttree*)malloc(sizeof(Ttree));
            (users[parentUserId]->rightSon)=(Ttree*)malloc(sizeof(Ttree));
            users[userId]=(Ttree*)malloc(sizeof(Ttree));

            if(users[userId]==NULL || (users[parentUserId]->rightSon)==NULL ||
              (users[parentUserId]->leftSon)==NULL) return false;

            (users[parentUserId]->leftSon)->rightBrother=users[userId];
            (users[parentUserId]->rightSon)->leftBrother=users[userId];

            (users[parentUserId]->leftSon)->father=users[parentUserId];
            (users[parentUserId]->rightSon)->father=users[parentUserId];

            (users[userId]->leftBrother)=(users[parentUserId]->leftSon);
            (users[userId]->rightBrother)=(users[parentUserId]->rightSon);

            (users[parentUserId]->leftSon)->leftBrother=NULL;
            (users[parentUserId]->rightSon)->rightBrother=NULL;
        }
        else{
            users[userId]=(Ttree*)malloc(sizeof(Ttree));

            if(users[userId]==NULL) return false;

            Ttree *temp;
            temp=((users[parentUserId]->leftSon)->rightBrother);
            ((users[parentUserId]->leftSon)->rightBrother)=users[userId];

            (users[userId]->leftBrother)=(users[parentUserId]->leftSon);
            (users[userId]->rightBrother)=temp;
            (temp->leftBrother)=users[userId];
        }
        users[userId]->leftSon=NULL;
        users[userId]->rightSon=NULL;
        users[userId]->films=NULL;
        printf("OK\n");
        return true;
    }
    else{
        printf("ERROR\n");
        return true;
    }
}

//Trzy funkcje pomocnicze funkcji delUser obsługujące dane przypadki.
static void brothersNoSons(int userId, Ttree *users[MAX_USERID])
{
    (users[userId]->leftBrother)->rightBrother=(users[userId]->rightBrother);
    (users[userId]->rightBrother)->leftBrother=(users[userId]->leftBrother);

    free(users[userId]);
    users[userId]=NULL;
}

static void noBrothersNoSons(int userId, Ttree *users[MAX_USERID])
{
    ((users[userId]->leftBrother)->father)->leftSon=NULL;
    ((users[userId]->leftBrother)->father)->rightSon=NULL;

    free(users[userId]->leftBrother);
    free(users[userId]->rightBrother);
    free(users[userId]);

    users[userId]=NULL;
}

static void sons(int userId, Ttree *users[MAX_USERID])
{
    (users[userId]->leftBrother)->rightBrother=(users[userId]->leftSon)->rightBrother;
    ((users[userId]->leftSon)->rightBrother)->leftBrother=users[userId]->leftBrother;

    (users[userId]->rightBrother)->leftBrother=(users[userId]->rightSon)->leftBrother;
    ((users[userId]->rightSon)->leftBrother)->rightBrother=users[userId]->rightBrother;

    free(users[userId]->leftSon);
    free(users[userId]->rightSon);
    free(users[userId]);
    users[userId]=NULL;
}

bool delUser(int userId, Ttree *users[MAX_USERID])
{
    if(users[userId]!=NULL){
        delList(users[userId]->films);
        if(users[userId]->leftSon==users[userId]->rightSon){
            if(((users[userId]->leftBrother)->leftBrother)==((users[userId]->rightBrother)->rightBrother)){
                noBrothersNoSons(userId, users);
            }
            else{
                brothersNoSons(userId, users);
            }
        }
        else{
            sons(userId, users);
        }
        printf("OK\n");
        return true;
    }
    else{
        printf("ERROR\n");
        return true;
    }
}

bool addMovie(int userId, int movieRating, Ttree *users[MAX_USERID])
{
    if(users[userId]!=NULL){
        Tlist *act,*nx;
        if (users[userId]->films==NULL || movieRating>(users[userId]->films)->movie_rating){
            act=users[userId]->films;
            users[userId]->films=(Tlist*)malloc(sizeof(Tlist));
            if(users[userId]->films==NULL) return false;
            users[userId]->films->next=act;
            users[userId]->films->movie_rating=movieRating;
        }
        else{
            act=users[userId]->films;
            nx=act->next;
            while(nx!=NULL && nx->movie_rating>movieRating){
                act=nx;
                nx=act->next;
            }
            if(act->movie_rating==movieRating){
                printf("ERROR\n");
                return true;
            }
            nx=(Tlist*)malloc(sizeof(Tlist));
            if(nx==NULL) return false;
            nx->next=act->next;
            nx->movie_rating=movieRating;
            act->next=nx;
        }
        printf("OK\n");
        return true;
    }
    else{
        printf("ERROR\n");
        return true;
    }
}

bool delMovie(int userId, int movieRating, Ttree *users[MAX_USERID])
{
    if(users[userId]!=NULL){
        Tlist *temp, *nx;
        temp=users[userId]->films;
        nx=temp;
        while(temp!=NULL && temp->movie_rating!=movieRating){
            nx=temp;
            temp=temp->next;
        }
        if(temp==NULL){
            printf("ERROR\n");
            return true;
        }
        nx->next=temp->next;
        if(users[userId]->films==temp) users[userId]->films=temp->next;
        free(temp);

        printf("OK\n");
        return true;
    }
    else{
        printf("ERROR\n");
        return true;
    }
}

static void setArray(int k, int array[])
{
    int i;
    for(i=0;i<k;++i) array[i]=-1;
}

//Przesuwa wszystkie elementy tablicy od danego indeksu x w prawo o jeden.
static void pushRightFrom(int k, int x, int array[k])
{
    int i;
    int temp[k];
    for(i=0;i<k;++i){
        temp[i]=array[i];
    }
    const int c=k-1;
    while(x<c){
        array[x+1]=temp[x];
        ++x;
    }
}

//Znajduje indeks pierwszej wartości w tablicy mniejszej niż value.
static int findFirstSmaller(int value, int k, int array[k])
{
    int i=0;
    while(i<k && array[i]>value) ++i;

    return i;
}

static void marathon_rec(int k, int movies[k], Ttree *start, int max)
{
    if(start!=NULL){
        Tlist *temp=start->films;
        int i=0;
        while(temp!=NULL && temp->movie_rating>max && i<k){
            i=findFirstSmaller(temp->movie_rating, k, movies);
            if(i<k) pushRightFrom(k, i, movies);
            movies[i]=temp->movie_rating;
            temp=temp->next;
        }
        Ttree *pointer;
        int max_rec;
        if(start->films==NULL) max_rec=max;
        else if(max<start->films->movie_rating) max_rec=start->films->movie_rating;
        else max_rec=max;

        if(start->leftSon!=start->rightSon){
            pointer=((start->leftSon)->rightBrother);
            while(pointer->rightBrother!=NULL){
                marathon_rec(k, movies, pointer, max_rec);
                pointer=pointer->rightBrother;
            }
        }
    }
}

void marathon(int userId, int k, Ttree *users[MAX_USERID])
{
    if(k==0 && users[userId]!=NULL) printf("NONE\n");
    else if(users[userId]!=NULL){
        if(k>10000) k=10000;
        int movies[k];
        setArray(k, movies);
        marathon_rec(k, movies, users[userId], -1);
        if(movies[0]!=-1){
            int i;
            for(i=0;i<k && movies[i]!=-1;++i){
                if(i<k-1 && movies[i+1]!=-1) printf("%d ", movies[i]);
                else printf("%d\n", movies[i]);
            }
        }
        else{
            printf("NONE\n");
        }
    }
    else{
        printf("ERROR\n");
    }
}
