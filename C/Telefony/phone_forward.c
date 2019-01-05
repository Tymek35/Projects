** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych.
 */

#include "phone_forward.h"
#include <stdio.h>
#include <string.h>

/**
 * Drzewo binarne sortowane leksykograficznie według prefiksów.
 */

struct tree{
  char *prefix;///<Prefiks numeru.
  char *forward;///<Przekierowanie.
  struct tree *rightSon;///<Prawy syn węzła.
  struct tree *leftSon;///<Lewy syn węzła.
};

/**
 * Nadanie nazwy strukturze drzewa.
 */
typedef struct tree Ttree;

/**
 * Struktura zawiera wskaźnik na korzeń drzewa przekierowań.
 */

struct PhoneForward{
  Ttree *root;///<Korzeń drzewa przekierowań.
};

/**
 * Struktura zawiera liczbę numerów zawartych w napisie wskazywanym
 * przez numbers.
 */

struct PhoneNumbers{
  unsigned int numbersCounter;///<Liczba numerów w tej strukturze.
  char const *numbers;///<Wskażnik na numery.
};


/** @brief Sprawdza czy napis jest poprawnym numerem.
 * @param[in] num - wskaźnik na napis.
 * @return True jeżeli napis składa się tylko z cyfr, false w przeciwnym przypadku.
 */

static bool checkNumber(char const *num)
{
  if(num==NULL || *num=='\0') return false;
  while(*num!='\0'){
    if(*num<'0' || *num>';') return false;
    ++num;
  }
  return true;
}

/** @brief Sprawdza czy napisy są identyczne.
 * @param[in] num1 - wskaźnik na pierwszy napis;
 * @param[in] num2 - wskaźnik na drugi napis.
 * @return True jeżeli napisy są identyczne, false w przeciwnym wypadku.
 */

static bool equals(char const *num1, char const *num2)
{
  int i=0;
  while(*(num1+i)!='\0' || *(num2+i)!='\0'){
    if(*(num1+i)!=*(num2+i)) return false;
    ++i;
  }
  return (*(num1+i)==*(num2+i));
}

/** @brief Porównuje napisy leksykograficznie.
 * @param[in] num1 - napis pierwszy;
 * @param[in] num2 - napis drugi.
 * @return 1 jeżeli num1 jest przed num2 w porządku leksykograficznym,
 *         -1 w przeciwnym razie, a 0 gdy są równe.
 */

static int compare(char const *num1, char const *num2)
{
  int i=0;
  while(*(num1+i)!='\0' || *(num2+i)!='\0'){
    if(*(num1+i)<*(num2+i)) return 1;
    if(*(num1+i)==*(num2+i)) ++i;
    else return -1;
  }
  return 0;
}

/** @brief Mierzy długość napisu.
 * @param[in] num - wskażnik na napis zakończony znakiem '\0'.
 * @return Długość napisu bez końcowego znaku '\0'.
 */

static unsigned int length(char const *num)
{
  int i=0;
  while(*(num+i)!='\0') ++i;
  return i;
}

/** @brief Kopiuje napis.
 * @param[in] num - wskażnik na napis.
 * @return Wskaźnik na kopię napisu num.
 */

static char *copy(char const *num)
{
  int i=0, l=length(num)+1;
  char *result=(char*)malloc(l*sizeof(char));
  if(result==NULL) return NULL;
  while(i<l){
    *(result+i)=*(num+i);
    ++i;
  }
  return result;
}

/** @brief Usuwa pojedynczy węzeł.
 * @param[in, out] node - wskaźnik na usuwany węzeł.
 */

static void delOneNode(Ttree *node)
{
  free((void*)node->prefix);
  free((void*)node->forward);
  free(node);
}

/** @brief Usuwa drzewo przekierowań.
 * @param[in, out] t - wskaźnik na usuwane drzewo.
 */

static void delTree(Ttree *t)
{
  if(t!=NULL){
    delTree(t->leftSon);
    delTree(t->rightSon);
    delOneNode(t);
  }
}

pfT *phfwdNew(void)
{
  pfT *pf=(pfT*)malloc(sizeof(pfT));
  if(pf==NULL) return NULL;
  pf->root=NULL;
  return pf;
}

void phfwdDelete(pfT *pf)
{
  if(pf!=NULL){
    delTree(pf->root);
    free(pf);
  }
}
/** Opisana w phone_forward.h
 */
void phnumDelete(pnT const *pnum)
{
  if(pnum!=NULL){
    free((void*)pnum->numbers);
    free((void*)pnum);
  }
}

/** @brief Tworzy nowy węzeł drzewa przekierowań.
 * @param[in] num1 - wskaźnik na prefiks;
 * @param[in] num2 - wskaźnik na prefiks przekierowania.
 * @return Wskaźnik na nowo utworzony węzeł.
 */

static Ttree *addNode(char const *num1, char const *num2)
{
  Ttree *temp;
  temp=(Ttree*)malloc(sizeof(Ttree));
  if(temp==NULL) return NULL;
  temp->rightSon=NULL;
  temp->leftSon=NULL;
  temp->prefix=copy(num1);
  temp->forward=copy(num2);
  if(temp->prefix==NULL || temp->forward==NULL) return NULL;
  return temp;
}

bool phfwdAdd(pfT *pf, char const *num1, char const *num2)
{
  if(pf==NULL || !checkNumber(num1) || !checkNumber(num2) || equals(num1, num2)) return false;
  Ttree *temp1=NULL;
  Ttree *temp2=pf->root;
  int x;
  while(temp2!=NULL){
    temp1=temp2;
    x=compare(num1, temp2->prefix);
    if(x==1) temp2=temp2->leftSon;
    else if(x==-1)temp2=temp2->rightSon;
    else temp2=NULL;
  }
  if(temp1==NULL){
    pf->root=addNode(num1, num2);
    if(pf->root==NULL) return false;
  }
  else if(x==0){
    free((void*)temp1->forward);
    temp1->forward=copy(num2);
  }
  else if(x==1){
    temp1->leftSon=addNode(num1, num2);
    if(temp1->leftSon==NULL) return false;
  }
  else{
    temp1->rightSon=addNode(num1, num2);
    if(temp1->rightSon==NULL) return false;
  }

  return true;
}

/** @brief Sprawdza czy pierwszy napis jest prefiksem drugiego.
 * @param[in] num1 - wskaźnik na pierwszy napis;
 * @param[in] num2 - wskażnik na drugi napis;
 * @return True jeżeli num1 jest prefiksem num2, false w przeciwnym przypadku.
 */

static bool isPrefix(char const *num1, char const *num2)
{
  int i=0;
  while(*(num1+i)==*(num2+i) && *(num1+i)!='\0') ++i;
  if(*(num1+i)=='\0') return true;
  else return false;
}

/** @brief Znajduje najmniejszą wartość w drzewie i zapisuje ojca węzła, w której ona jest.
 * @param[in, out] father - wskaźnik na ojca węzła z najmniejszą wartością;
 * @param[in] node - wskażnik na węzeł, od którego zaczynamy szukać.
 * @return Wskaźnik na węzeł z najmniejszą wartością.
 */

static Ttree *minValueNode(Ttree **father, Ttree *node)
{
  Ttree *current=node, *currentFather=*father;
  while (current->leftSon!=NULL){
    currentFather=current;
    current = current->leftSon;
  }
  *father=currentFather;
  return current;
}

/** @brief Funkcja usuwa węzły z drzewa, w których num jest prefiksem prefixu.
 * @param[in, out] root - wskaźnik na węzeł, który aktualnie rozpatrujemy;
 * @param[in] num - numer, według którego przeprowadzamy usuwanie.
 * @return Wskaźnik na korzeń drzewa, który może ulec zmianie.
 */

static Ttree *delNodes(Ttree *root, char const *num)
{
  if(root==NULL) return root;
  int com=compare(num, root->prefix);

  if(com==1) root->leftSon=delNodes(root->leftSon, num);
  else if(com==-1) root->rightSon=delNodes(root->rightSon, num);

  if(com==0 || isPrefix(num, root->prefix)){
    if(root->leftSon==NULL){
      Ttree *temp=root->rightSon;
      delOneNode(root);
      return delNodes(temp, num);
    }
    else if(root->rightSon==NULL){
      Ttree *temp=root->leftSon;
      delOneNode(root);
      return delNodes(temp, num);
    }
    else{
      Ttree *father=root;
      Ttree *temp=minValueNode(&father, root->rightSon);
      root->prefix=copy(temp->prefix);
      root->forward=copy(temp->forward);

      if(father->leftSon==temp) father->leftSon=NULL;
      else father->rightSon=NULL;
      delOneNode(temp);

      if(com==1) root->leftSon=delNodes(root->leftSon, num);
      else if(com==-1 || com==0) root->rightSon=delNodes(root->rightSon, num);
    }
  }
  return root;
}

void phfwdRemove(pfT *pf, char const *num)
{
  if(pf!=NULL && checkNumber(num)){
    pf->root=delNodes(pf->root, num);
  }
}

/** @brief Tworzy pustę strukturę PhoneNumbers.
 * @return Wskaźnik na stworzoną strukturę.
 */

static pnT *empty()
{
  pnT *temp=(pnT*)malloc(sizeof(pnT));
  temp->numbersCounter=0;
  temp->numbers=NULL;
  return temp;
}

/** Opisana w phone_forward.h
 */
char const *phnumGet(pnT const *pnum, size_t idx)
{
  if(pnum==NULL || pnum->numbers==NULL) return NULL;
  unsigned int i=0;
  char *temp=(char*)pnum->numbers;
  while(i<idx && i<pnum->numbersCounter){
    if(*temp=='\0') ++i;
    temp++;
  }
  if(i>=pnum->numbersCounter) return NULL;
  else return temp;
}

pnT const *phfwdGet(pfT *pf, char const *num)
{
  if(pf==NULL || checkNumber(num)!=true){
    return empty();
  }
  pnT *result=(pnT*)malloc(sizeof(pnT));
  if(result==NULL) return NULL;
  result->numbersCounter=1;
  int l=length(num), i=1, bestlen=0, com, j;
  Ttree *temp=pf->root, *best=NULL;
  char *pref=(char*)malloc((l+1)*sizeof(char));
  if(temp==NULL){
    char *forward=copy(num);
    result->numbers=forward;
    free((void*)pref);
    return result;
  }
  while(i<=l){
    for(j=0;j<i;++j) *(pref+j)=*(num+j);
    *(pref+j)='\0';
    com=compare(pref, temp->prefix);
    while(com!=0 && temp!=NULL){
      if(com==1) temp=temp->leftSon;
      else temp=temp->rightSon;
      if(temp!=NULL) com=compare(pref, temp->prefix);
    }
    if(temp!=NULL){
      best=temp;
      bestlen=i;
    }
    temp=pf->root;
    ++i;
  }
  free((void*)pref);
  if(best==NULL){
    char *forward=copy(num);
    result->numbers=forward;
    return result;
  }
  else{
    int forlen=length(best->forward), k=0;
    char *forward=(char*)malloc((l-bestlen+1+forlen)*sizeof(char)), *chptr=best->forward;
    for(i=0;i<forlen;++i, ++k) *(forward+k)=*(chptr+i);
    for(i=bestlen;i<=l;++i, ++k) *(forward+k)=*(num+i);
    result->numbers=forward;
    return result;
  }
}

/** @brief Ustawia elementy tablicy na NULL zaczynając od indeksu start i kończąc na end.
 * @param[in, out] array - wskaźnik na tablicę wskaźników;
 * @param[in] start - indeks od którego zacząć ustawianie;
 * @param[in] end - indeks, na którym skończyć.
 */
static void setArray(Ttree **array, int start, int end)
{
  for(int i=start;i<end;++i) *(array+i)=NULL;
}

/** @brief Znajduje węzły w drzewie, których prefiksem jest num.
 * @param[in] t - wskaźnik na aktualnie sprawdzany węzeł;
 * @param[in, out] nodes - tablica węzłów;
 * @param[in, out] S - wskaźnik rozmiar tablicy węzłów;
 * @param[in, out] index - wskaźnik na aktualny indeks;
 * @param[in] num - numer, według którego szukamy węzłów.
 */
static void findNodes(Ttree *t, Ttree **nodes, int *S, int *index, char const *num)
{
  if(t!=NULL){
    findNodes(t->leftSon, nodes, S, index, num);
    if(isPrefix(t->forward, num)){
      *(nodes+(*index))=t;
      (*index)++;
      if((*index)==(*S)){
        (*S)=(*S)*2;
        nodes=(Ttree**)realloc(nodes ,(*S)*sizeof(Ttree*));
      }
    }
    findNodes(t->rightSon, nodes, S, index, num);
  }
}

/** @brief Usuwa powtórzenia z tablicy i zminiejsza całkowitą długość.
 * @param[in, out] array - tablica, z której usuwamy powtórzenia;
 * @param[in, out] totalLength - wskaźnik na całkowitą długość; 
 * @param[in] size - rozmiar tablicy.
 */
static void deleteRepeats(char **array, int *totalLength, int size)
{
  for(int i=0;i<=size;++i){
    for(int j=i+1;j<=size;++j){
      if(*(array+i)!=NULL && *(array+j)!=NULL && equals(*(array+i), *(array+j))){
        *totalLength-=(length(*(array+j))+1);
        free((void*)*(array+j));
        *(array+j)=NULL;
      }
    }
  }
}

/** @brief Sortuje tablicę leksykograficznie.
 * @param[in, out] array - sortowana tablica;
 * @param[in] size - rozmiar tablicy.
 */
static void sortArray(char **array, int size)
{
  int i=0, j;
  char *chptr;
  while(i<size){
    j=0;
    while(j<size-i){
      if(compare(*(array+j), *(array+j+1))!=1){
        chptr=*(array+j);
        *(array+j)=*(array+j+1);
        *(array+j+1)=chptr;
      }
      ++j;
    }
    ++i;
  }
}

pnT const *phfwdReverse(pfT *pf, char const *num)
{
  if(checkNumber(num)!=true || pf==NULL){
    return empty();
  }

  pnT *result=(pnT*)malloc(sizeof(pnT));
  if(result==NULL) return NULL;
  int S=10000, index=0, totalLength=0, i=0, lnum=length(num);
  int temp, lpref, lfor, j, u, place=0;
  Ttree **nodes=(Ttree**)malloc(S*sizeof(Ttree*));
  setArray(nodes, 0, S);
  findNodes(pf->root, nodes, &S, &index, num);
  result->numbersCounter=0;
  char **results=(char**)malloc((index+1)*sizeof(char*));

  while(i<index){
    Ttree *t=*(nodes+i);
    char *chptrpref=t->prefix, *chptrfor=t->forward;
    lpref=length(chptrpref);
    lfor=length(chptrfor);
    temp=lnum-lfor+lpref+1;
    *(results+i)=(char*)malloc(temp*sizeof(char));
    char *chptrres=*(results+i); 

    for(j=0, u=0;j<lpref;++j, ++u) *(chptrres+u)=*(chptrpref+j);
    for(j=lfor;j<=lnum;++j, ++u) *(chptrres+u)=*(num+j);

    totalLength=totalLength+temp;
    ++i;
  }

  while(place<index){
    if(compare(num, *(results+place))==1) break;
    else ++place;
  }

  char *charpointer=*(results+place);
  *(results+place)=copy(num);
  if(place==0 && index!=0){
    while(i>1){
      *(results+i)=*(results+i-1);
      --i;
    }
    *(results+1)=charpointer;
  }
  else if(place!=i){
    ++place;
    while(i>place){
      *(results+i)=*(results+i-1);
      --i;
    }
    *(results+i)=charpointer;
  }
  totalLength=totalLength+lnum+1;
  sortArray(results, index);
  deleteRepeats(results, &totalLength, index);

  char *numbrs=(char*)malloc(totalLength*sizeof(char));
  int count=0, k=0;
  i=0;
  while(i<=index){
    if(*(results+i)!=NULL){
      ++count;
      for(j=0;*(*(results+i)+j)!='\0';++j, ++k) *(numbrs+k)=*(*(results+i)+j);
      *(numbrs+k)='\0';
      ++k;
    }
    ++i;
  }
  result->numbersCounter=count;
  result->numbers=numbrs;

  for(int d=0;d<=index;++d) free((void*)*(results+d));
  free((void*)results);
  free((void*)nodes);

  return result;
}

/** @brief Liczy cyfry w zbiorze set.
 * @param[in] set - zbiór znaków.
 * @return Liczba cyfr w zbiorze set.
 */
static int countDigits(char const *set)
{
  int i=0, count=0;
  while(*(set+i)!='\0'){
    if(*(set+i)>='0' && *(set+i)<=';') ++count;
    ++i;
  }
  return count;
}

/** @brief Kopiuje węzły drzewa do tablicy.
 * @param[in] t - wskaźnik na korzeń drzewa;
 * @param[in, out] nodes - tablica węzłów;
 * @param[in, out] S - wskaźnik na rozmiar tablicy;
 * @param[in, out] numberOfNodes - liczba węzłów;
 * @param[in, out] ptr - wskaźnik na tablicę by w razie czego powiększyć jej rozmiar.
 */
static void copyNodes(Ttree *t, Ttree **nodes, int *S, int *numberOfNodes, Ttree ***ptr)
{
  if(t!=NULL){
    copyNodes(t->leftSon, nodes, S, numberOfNodes, ptr);
    *(nodes+(*numberOfNodes))=t;
    (*numberOfNodes)++;
    if((*numberOfNodes)==(*S)){
      (*S)=(*S)*2;
      *ptr=(Ttree**)realloc(nodes ,(*S)*sizeof(Ttree*));
      setArray(nodes, *(S)/2, *(S));
    }
    copyNodes(t->rightSon, nodes, S, numberOfNodes, ptr);
  }
}

/** @brief Po działani funkcji żaden 'forward' z węzła nie jest prefiksem innego.
 * @param[in, out] nodes - tablica węzłów;
 * @param[in] numberOfNodes - rozmiar tablicy.
 */
static void deletePrefix(Ttree **nodes, int numberOfNodes)
{
  Ttree *treptr1, *treptr2;
  for(int i=0;i<numberOfNodes;++i){
    treptr1=*(nodes+i);
    if(treptr1!=NULL){
      for(int j=0;j<numberOfNodes;++j){
        treptr2=*(nodes+j);
        if(j!=i && isPrefix(treptr2->forward, treptr1->forward)){
          *(nodes+j)=NULL;
        }
      }
    }
  }
}

/** @brief Sprawdza czy każda cyfra z 'forward' jest obecna w tablicy.
 * @param[in] forward - wskaźnik na numer;
 * @param[in] presentDigits - tablica określająca, które cyfry są dostępne.
 */
static bool everyDigitPresent(char *forward, bool presentDigits[])
{
  for(int i=0;*(forward+i)!='\0';++i){
    if(presentDigits[(int)*(forward+i)]!=true) return false;
  }
  return true;
}

/** @brief Liczy base do potęgi exponent.
 * @param[in] base - baza potęgi;
 * @param[in] exponent - wykładnik potęgi.
 */
static size_t power(int base, int exponent)
{
  size_t result=1;
  for(int i=0;i<exponent;++i) result=result*base;
  return result;
}

size_t phfwdNonTrivialCount(pfT *pf, char const *set, size_t len)
{
  if(pf==NULL || set==NULL || *set=='\0' || len==0 || pf->root==NULL){
    return 0;
  }
  int digitsNumber=countDigits(set), S=10000, numberOfNodes=0;
  int diffrentDigits=0;
  const int size=';'+1;
  if(digitsNumber==0) return 0;
  bool presentDigits[size];
  for(int i=0;i<size;++i) presentDigits[i]=false;
  for(int i=0;*(set+i)!='\0';++i){
    if(*(set+i)>='0' && *(set+i)<=';'){
      presentDigits[(int)*(set+i)]=true;
    }
  }
  for(int i='0';i<size;++i){
    if(presentDigits[i]==true) ++diffrentDigits;
  }
  Ttree **nodes=(Ttree**)malloc(S*sizeof(Ttree*));
  setArray(nodes, 0, S);
  copyNodes(pf->root, nodes, &S, &numberOfNodes, &nodes);
  deletePrefix(nodes, numberOfNodes);
  size_t result=0;
  Ttree *ptr;
  for(int i=0;i<numberOfNodes;++i){
    ptr=*(nodes+i);
    if(ptr!=NULL && length(ptr->forward)<=len &&
       everyDigitPresent(ptr->forward, presentDigits)){
      result=result+power(diffrentDigits, len-length(ptr->forward));
    }
  }
  free((void*)nodes);
  return result;
}
