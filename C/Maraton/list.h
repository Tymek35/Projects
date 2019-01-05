#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>

struct list{
    int movie_rating;
    struct list *next;
};

typedef struct list Tlist;

//Usuwa listę, na której początek wskazuje l.
void delList(Tlist *l);

#endif
