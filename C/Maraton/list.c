#include "list.h"

void delList(Tlist *l)
{
    Tlist *nx;
    while(l!=NULL){
        nx=l->next;
        free(l);
        l=nx;
    }
}
