#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include <stdbool.h>
#include "list.h"

#define MAX_USERID 65536

/*Każdy węzeł posiada listę filmów posortowaną malejąco,
  jego synowie tworzą listę dwukierunkową, która zaczyna
  się i kończy atrapą (leftSon i rightSon) i tylko atrapy
  mają ustawiony wskaźnik na ojca. */
struct tree{
    Tlist *films;
    struct tree *leftBrother;
    struct tree *rightBrother;
    struct tree *leftSon;
    struct tree *rightSon;
    struct tree *father;
};
typedef struct tree Ttree;

//Ustawia wszystkie elementy tablicy users na NULL.
void setUsers(Ttree *users[MAX_USERID]);

void delAll(Ttree *users[MAX_USERID]);

bool delMovie(int userId, int movieRating, Ttree *users[MAX_USERID]);

bool addMovie(int userId, int movieRating, Ttree *users[MAX_USERID]);

bool delUser(int userId, Ttree *users[MAX_USERID]);

bool addUser(int parentUserId, int userId, Ttree *users[MAX_USERID]);

void marathon(int userId, int k, Ttree *users[MAX_USERID]);

#endif
