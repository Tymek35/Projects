/** @file
 * Implementacja interfejsu użytkownika.
 */

#include <stdio.h>
#include <stdbool.h>
#include "phone_forward.h"

/** Maksymalna liczba obsługiwanych baz przekierowań.
 */
#define NUMBER_OF_BASES 101

/** Makro sprawdzające czy znak jest cyfrą lub literą.
 */
#define ISLETTERORNUMBER(c) (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<=';')

/** Rozmiar linii, do której wczytuje się polecenie.
 */
#define BUFFERSIZE 200000

/** Zmienna zlicza ile znaków wczytał program.
 */
int charCounter=0;

/** Zmienna zapamietuje numer pierwszego znaku ostatniego operatora.
 */
int lastOperator=0;

/** @brief Mierzy długość napisu.
 * @param[in] line - wskażnik na napis zakończony znakiem '\0'.
 * @return Długość napisu bez końcowego znaku '\0'.
 */

int length(char *line)
{
  int i=0;
  while(*(line+i)!='\0') ++i;
  return i;
}

/** @brief Wyznacza maksimum z dwóch liczb.
 * @param[in] a - pierwsza liczba;
 * @param[in] b  - druga liczba.
 * @return Zwraca większą z liczb.
 */

int max(int a, int b)
{
  if(a>b) return a;
  return b;
}

/** @brief Kopiuje napis.
 * @param[in] line - wskażnik na napis.
 * @return Wskaźnik na kopię napisu num.
 */

char *copy(char *line)
{
  int i=0, l=length(line)+1;
  char *result=(char*)malloc(l*sizeof(char));
  while(i<l){
    *(result+i)=*(line+i);
    ++i;
  }
  return result;
}

/** @brief Sprawdza czy napisy są identyczne.
 * @param[in] c1 - wskaźnik na pierwszy napis;
 * @param[in] c2 - wskaźnik na drugi napis.
 * @return True jeżeli napisy są identyczne, false w przeciwnym wypadku.
 */

bool equals(char *c1, char *c2)
{
  int i=0;
  if(c1==NULL || c2==NULL) return false;
  while(*(c1+i)!='\0' || *(c2+i)!='\0'){
    if(*(c1+i)!=*(c2+i)) return false;
    ++i;
  }
  return (*(c1+i)==*(c2+i));
}

/** @brief Sprawdza czy baza o nazwie 'line' istnieje, jeżeli tak to zwraca jej indeks.
 * @param[in] basesIDs - tablica z nazwami baz;
 * @param[in] line - nazwa szukanej bazy.
 * @return Indeks szukanej bazy, jeżeli jej nie ma to -1.
 */

int findBase(char **basesIDs, char *line)
{
  int i=0;
  while(i<NUMBER_OF_BASES){
    if(equals(*(basesIDs+i), line)) return i;
    ++i;
  }
  return -1;
}

/** @brief Pomija komentarz.
 * @param[in, out] error - jeżeli w nieoczekiwanym miejscu będzie EOF ustawia na true.
 * @return Pierwszy znak po komentarzu.
 */

int skipComment(bool *error)
{
  int c=getchar();
  ++charCounter;
  while(c!='$' && c!=EOF){
    c=getchar();
    ++charCounter;
  }
  if(c==EOF){
    *error=true;
    printf("ERROR EOF\n");
    return EOF;
  }
  ++charCounter;
  c=getchar();
  if(c=='$'){
    ++charCounter;
    return getchar();
  }
  else if(c==EOF){
    *error=true;
    printf("ERROR EOF\n");
    return EOF;
  }
  return skipComment(error);
}

/** @brief Pomija białe znaki.
 * @return Pierwszy znak, który nie jest białym znakiem.
 */

int skipSpaces()
{
  int c=getchar();
  ++charCounter;
  while(c==' ' || c=='\t' || c==13 || c=='\n'){//13 to znak powrotu karetki.
    c=getchar();
    ++charCounter;
  }
  return c;
}

/** @brief Drukuje numer.
 * @param[in] number - drukowany numer.
 */

void printNumber(char const *number)
{
  int i=0;
  while(*(number+i)!='\0'){
    printf("%c", *(number+i));
    ++i;
  }
  printf("\n");
}

/** @brief Drukuje informację o błędnym znaku wraz z jego numerem.
 * @param[in, out] error - wskaźnik, który ustawia na true.
 * @return Zwraca 0.
 */

int normalError(bool *error)
{
  *error=true;
  printf("ERROR %d\n", charCounter);
  return 0;
}

/** @brief Drukuje informację o EOF, który pojawił się niespodziewanie.
 * @param[in, out] error - wskaźnik, który ustawia na true.
 * @return Zwraca 0.
 */

int errorEOF(bool *error)
{
  *error=true;
  printf("ERROR EOF\n");
  return 0;
}

/** @brief Wczytuje do line numer.
 * @param[in, out] line - wskaźnik na linię, do której jest wczytywany numer.
 * @return Pierwszy znak po numerze.
 */

char getNumber(char *line)
{
  int i=0;
  char c=getchar();
  ++charCounter;
  while(c>='0' && c<=';'){
    *(line+i)=c;
    c=getchar();
    ++charCounter;
    ++i;
  }
  *(line+i)='\0';
  return c;
}

/** @brief Wczytuje dane potrzebne do operacji i w razie błędu sygnalizuje go.
 * @param[in, out] line - wskaźnik na miejsce do zapisania danych;
 * @param[in, out] error - wskaźnik, który w razie błędu jest zmieniany na true;
 * @param[in, out] temp - wskaźnik na znak, gdzie będzie zapisany pierwszy znak po 
                          potrzebnych danych.
 * @return Zwraca 0 w wypadku błędu lub 7 jeżeli dane są poprawne.
 */

int trivial(char *line, bool *error, char *temp)
{
  char c=skipSpaces();
  if(c=='$'){
    ++charCounter;
    c=getchar();
    if(c=='$') c=skipComment(error);
    else if(c==EOF) return errorEOF(error);
    else return normalError(error);
  }
  else if(c==EOF) return errorEOF(error);

  if(c==' ' || c=='\t' || c==13 || c=='\n') c=skipSpaces();

  if(c>='0' && c<=';'){
    *line=c;
    *temp=getNumber(line+1);
    return 7;
  }
  else if(c==EOF) errorEOF(error);
  return normalError(error);
}

/** @brief Wczytuje dane potrzebne do operacji i w razie błędu sygnalizuje go.
 * @param[in, out] line - wskaźnik na miejsce do zapisania danych;
 * @param[in, out] error - wskaźnik, który w razie błędu jest zmieniany na true;
 * @param[in, out] temp - wskaźnik na znak, gdzie będzie zapisany pierwszy znak po 
                          potrzebnych danych.
 * @return Zwraca 0 w wypadku błędu lub 3 albo 4 jeżeli dane są poprawne.
 */

int num(char *line, bool *error, char *temp)
{
  char c=getNumber(line+1);
  if(c==' ' || c=='\t' || c==13 || c=='\n') c=skipSpaces();

  if(c=='$'){
    ++charCounter;
    c=getchar();
    if(c=='$') c=skipComment(error);
    else if(c==EOF) return errorEOF(error);
    else return normalError(error);
  }
  else if(c==EOF) return errorEOF(error);

  if(c==' ' || c=='\t' || c==13 || c=='\n') c=skipSpaces();

  if(c=='>'){
    lastOperator=charCounter;
    c=skipSpaces();
    if(c=='$'){
      ++charCounter;
      c=getchar();
      if(c=='$') c=skipComment(error);
      else if(c==EOF) return errorEOF(error);
      else return normalError(error);
    }
    else if(c==EOF) return errorEOF(error);

    if(c==' ' || c=='\t' || c==13 || c=='\n') c=skipSpaces();

    if(c>='0' && c<=';'){
      *(line+length(line)+1)=c;
      *temp=getNumber(line+length(line)+2);
      return 3;
    }
    else if(c==EOF) errorEOF(error);
    else return normalError(error);
  }
  else if(c=='?'){
    lastOperator=charCounter;
    ++charCounter;
    *temp=getchar();
    return 4;
  }
  else if(c==EOF) return errorEOF(error);
  return normalError(error);
}

/** @brief Wczytuje dane potrzebne do operacji i w razie błędu sygnalizuje go.
 * @param[in, out] line - wskaźnik na miejsce do zapisania danych;
 * @param[in, out] error - wskaźnik, który w razie błędu jest zmieniany na true;
 * @param[in, out] temp - wskaźnik na znak, gdzie będzie zapisany pierwszy znak po 
                          potrzebnych danych.
 * @return Zwraca 0 w wypadku błędu lub 5 jeżeli dane są poprawne.
 */

int rev(char *line, bool *error, char *temp)
{
  char c=skipSpaces();
  if(c=='$'){
    ++charCounter;
    c=getchar();
    if(c=='$') c=skipComment(error);
    else if(c==EOF) return errorEOF(error);
    else return normalError(error);
  }
  else if(c==EOF) return errorEOF(error);

  if(c==' ' || c=='\t' || c==13 || c=='\n') c=skipSpaces();

  if(c>='0' && c<=';'){
    *line=c;
    *temp=getNumber(line+1);
    return 5;
  }
  else if(c==EOF) errorEOF(error);
  return normalError(error);
}

/** @brief Wczytuje dane potrzebne do operacji i w razie błędu sygnalizuje go.
 * @param[in, out] line - wskaźnik na miejsce do zapisania danych;
 * @param[in, out] error - wskaźnik, który w razie błędu jest zmieniany na true;
 * @param[in, out] temp - wskaźnik na znak, gdzie będzie zapisany pierwszy znak po 
                          potrzebnych danych.
 * @return Zwraca 0 w wypadku błędu lub 2 albo 6 jeżeli dane są poprawne.
 */

int del(char *line, bool *error, char *temp)
{
  ++charCounter;
  char c=getchar();
  if(c!='E'){
    if(c==EOF) return errorEOF(error);
    return normalError(error);
  }
  ++charCounter;
  c=getchar();
  if(c!='L'){
    if(c==EOF) return errorEOF(error);
    return normalError(error);
  }
  c=skipSpaces();
  if(c=='$'){
    ++charCounter;
    c=getchar();
    if(c=='$') c=skipComment(error);
    else if(c==EOF) return errorEOF(error);
    else return normalError(error);
  }
  else if(c==EOF) return errorEOF(error);

  if(c==' ' || c=='\t' || c==13 || c=='\n') c=skipSpaces();

  if((c>='a' && c<='z') || (c>='A' && c<='Z')){
    int i=0;
    while(ISLETTERORNUMBER(c)){
      *(line+i)=c;
      c=getchar();
      ++charCounter;
      ++i;
    }
    *temp=c;
    *(line+i)='\0';
    return 2;
  }
  else if(c>='0' && c<=';'){
    int i=0;
    while(c>='0' && c<=';'){
      *(line+i)=c;
      c=getchar();
      ++charCounter;
      ++i;
    }
    *temp=c;
    *(line+i)='\0';
    return 6;
  }
  else if(c==EOF) return errorEOF(error);
  else return normalError(error);
}

/** @brief Wczytuje dane potrzebne do operacji i w razie błędu sygnalizuje go.
 * @param[in, out] line - wskaźnik na miejsce do zapisania danych;
 * @param[in, out] error - wskaźnik, który w razie błędu jest zmieniany na true;
 * @param[in, out] temp - wskaźnik na znak, gdzie będzie zapisany pierwszy znak po 
                          potrzebnych danych.
 * @return Zwraca 0 w wypadku błędu lub 1 jeżeli dane są poprawne.
 */

int new(char *line, bool *error, char *temp)
{
  ++charCounter;
  char c=getchar();
  if(c!='E'){
    if(c==EOF) return errorEOF(error);
    return normalError(error);
  }
  ++charCounter;
  c=getchar();
  if(c!='W'){
    if(c==EOF) return errorEOF(error);
    return normalError(error);
  }
  c=skipSpaces();
  if(c=='$'){
    ++charCounter;
    c=getchar();
    if(c=='$') c=skipComment(error);
    else if(c==EOF) return errorEOF(error);
    else return normalError(error);
  }
  else if(c==EOF) return errorEOF(error);

  if(c==' ' || c=='\t' || c==13 || c=='\n') c=skipSpaces();

  if((c>='a' && c<='z') || (c>='A' && c<='Z')){
    int i=0;
    while(ISLETTERORNUMBER(c)){
      *(line+i)=c;
      c=getchar();
      ++charCounter;
      ++i;
    }
    *temp=c;
    *(line+i)='\0';
    if(equals(line, "NEW") || equals(line, "DEL")){
      printf("ERROR %d\n", charCounter-3);
      *error=true;
      return 0;
    }
    return 1;
  }
  else if(c==EOF) return errorEOF(error);
  else return normalError(error);
}

/** @brief Na podstawie wczytanych danych określa, która instrukcja została podana.
    W wypadku błędu lub poprawnego końca pliku sygnalizuje to przez wskaźnik.
 * @param[in, out] line - wskaźnik na miejsce do zapisania danych;
 * @param[in, out] error - wskaźnik, który w razie błędu jest zmieniany na true;
 * @param[in, out] end - wskaźnik, który w razie poprawnego końca danych zmieniany jest na true.
 * @return Zwraca 0 w wypadku błędu lub wartość z przedziału od 1 do 7 jeżeli dane są poprawne.
 */

int getInstruction(char *line, bool *end, bool *error)
{
  static char temp='\n';
  int c=temp;
  if(c==' ' || c=='\t' || c=='\n' || c==13) c=skipSpaces();
  if(c=='$'){
    c=getchar();
    ++charCounter;
    if(c=='$') c=skipComment(error);
  }

  while(c==' ' || c=='\t' || c=='\n' || c==13){
    c=getchar();
    if(c=='$'){
      c=getchar();
      ++charCounter;
      if(c=='$') c=skipComment(error);
    }
    ++charCounter;
  }
  if(c=='N'){
    lastOperator=charCounter;
    return new(line, error, &temp);
  }
  else if(c=='D'){
    lastOperator=charCounter;
    return del(line, error, &temp);
  }
  else if(c=='?'){
    lastOperator=charCounter;
    return rev(line, error, &temp);
  }
  else if(c>='0' && c<=';'){
    *line=c;
    return num(line, error, &temp);
  }
  else if(c=='@'){
    lastOperator=charCounter;
    return trivial(line, error, &temp);
  }
  else if(c==EOF){
    *end=true;
    return 0;
  }
  else return normalError(error);
}

/** @brief Wkonuje operację phfwdNonTrivialCount, w razie błędu sygnalizuje go.
 * @param[in] bases - tablica wskaźników na bazy;
 * @param[in] actBase - indeks w tablicy aktualnej bazy;
 * @param[in] line - dane dla funkcji phfwdNonTrivialCount;
 * @param[in, out] error - wskaźnik w razie błędu zmieniany na true.
 */

void nonTrival(pfT *bases[NUMBER_OF_BASES], int actBase, char *line, bool *error)
{
  if(actBase==-1){
    printf("ERROR @ %d\n", lastOperator);
    *error=true;
  }
  else{
    size_t len=max(0, length(line)-12);
    printf("%ld\n", phfwdNonTrivialCount(bases[actBase], line, len));
  }
}

/** @brief Wkonuje operację phfwdRemove, w razie błędu sygnalizuje go.
 * @param[in] bases - tablica wskaźników na bazy;
 * @param[in] actBase - indeks w tablicy aktualnej bazy;
 * @param[in] line - dane dla funkcji phfwdRemove;
 * @param[in, out] error - wskaźnik w razie błędu zmieniany na true.
 */

void rem(pfT *bases[NUMBER_OF_BASES], int actBase, char *line, bool *error)
{
  if(actBase==-1){
    printf("ERROR DEL %d\n", lastOperator);
    *error=true;
  }
  else{
    phfwdRemove(bases[actBase], line);
  }
}

/** @brief Wkonuje operację phfwdReverse, w razie błędu sygnalizuje go.
 * @param[in] bases - tablica wskaźników na bazy;
 * @param[in] actBase - indeks w tablicy aktualnej bazy;
 * @param[in] line - dane dla funkcji phfwdReverse;
 * @param[in, out] error - wskaźnik w razie błędu zmieniany na true.
 */

void printReverse(pfT *bases[NUMBER_OF_BASES], int actBase, char *line, bool *error)
{
  if(actBase==-1){
    printf("ERROR ? %d\n", lastOperator);
    *error=true;
  }
  else{
    const pnT *result=phfwdReverse(bases[actBase], line);
    size_t i=0;
    char const *temp=phnumGet(result, i);
    while(temp!=NULL){
      printNumber(temp);
      ++i;
      temp=phnumGet(result, i);
    }
    phnumDelete(result);
  }
}

/** @brief Wkonuje operację phfwdGet, w razie błędu sygnalizuje go.
 * @param[in] bases - tablica wskaźników na bazy;
 * @param[in] actBase - indeks w tablicy aktualnej bazy;
 * @param[in] line - dane dla funkcji phfwdGet;
 * @param[in, out] error - wskaźnik w razie błędu zmieniany na true.
 */

void printForward(pfT *bases[NUMBER_OF_BASES], int actBase, char *line, bool *error)
{
  if(actBase==-1){
    printf("ERROR ? %d\n", lastOperator);
    *error=true;
  }
  else{
    const pnT *result=phfwdGet(bases[actBase], line);
    char const *temp=phnumGet(result, 0);
    printNumber(temp);
    phnumDelete(result);
  }
}

/** @brief Wkonuje operację phfwdAdd, w razie błędu sygnalizuje go.
 * @param[in] bases - tablica wskaźników na bazy;
 * @param[in] actBase - indeks w tablicy aktualnej bazy;
 * @param[in] line - dane dla funkcji phfwdAdd;
 * @param[in, out] error - wskaźnik w razie błędu zmieniany na true.
 */

void addForward(pfT *bases[NUMBER_OF_BASES], int actBase, char *line, bool *error)
{
  if(actBase==-1){
    printf("ERROR > %d\n", lastOperator);
    *error=true;
  }
  else{
    int secondNumber=0;
    while(*(line+secondNumber)!='\0') ++secondNumber;
    ++secondNumber;
    if(!phfwdAdd(bases[actBase], line, (line+secondNumber))){
      printf("ERROR > %d\n", lastOperator);
      *error=true;
    }
  }
}

/** @brief Wkonuje operację phfwdDelete, w razie błędu sygnalizuje go.
 * @param[in, out] bases - tablica wskaźników na bazy;
 * @param[in, out] actBase - wskaźnik na indeks w tablicy aktualnej bazy;
 * @param[in] line - nazwa bazy do usunięcia;
 * @param[in, out] basesIDs - tablica nazw baz;
 * @param[in, out] error - wskaźnik w razie błędu zmieniany na true.
 */

void delBase(pfT *bases[NUMBER_OF_BASES], int *actBase, char *line, char **basesIDs, bool *error)
{
  int i=findBase(basesIDs, line);
  if(i==-1){
    printf("ERROR DEL %d\n", lastOperator);
    *error=true;
  }
  else{
    phfwdDelete(bases[i]);
    bases[i]=NULL;
    free((void*)*(basesIDs+i));
    *(basesIDs+i)=NULL;
    if(*actBase==i) *actBase=-1;
  }
}

/** @brief Wkonuje operację phfwdNew lub zmienia aktualną bazę.
 * @param[in, out] bases - tablica wskaźników na bazy;
 * @param[in, out] actBase - wskaźnik na indeks w tablicy aktualnej bazy;
 * @param[in] line - nazwa bazy do stworzenia lub ustawienia;
 * @param[in, out] basesIDs - tablica nazw baz;
 */

void newBase(pfT *bases[NUMBER_OF_BASES], int *actBase, char *line, char **basesIDs)
{
  int i=findBase(basesIDs, line);
  if(i==-1){
    int firstNULL=0;
    while(bases[firstNULL]!=NULL) ++firstNULL;
    *actBase=firstNULL;
    *(basesIDs+firstNULL)=copy(line);
    bases[firstNULL]=phfwdNew();
  }
  else{
    *actBase=i;
  }
}

/** @brief Główna pętla programu.
    Wykonuje operacje, aż wystąpi błąd lub się skończą.
 * @param[in, out] bases - tablica wskaźników na bazy.
 * @return True jeżeli podczas wykonywania operacji wystąpił błąd, false w
   przeciwnym wypadku.
 */

bool doInstructions(pfT *bases[NUMBER_OF_BASES])
{
  int actBase=-1;
  char *line=(char*)malloc(BUFFERSIZE*sizeof(char));
  char **basesIDs=(char**)malloc(NUMBER_OF_BASES*sizeof(char*));

  for(int i=0;i<NUMBER_OF_BASES;++i) *(basesIDs+i)=NULL;

  bool end=false, error=false;
  int instruction=getInstruction(line, &end, &error);

  while(end==false && error==false){
    switch(instruction){
      case 1:
      newBase(bases, &actBase, line, basesIDs);
      break;
      case 2:
      delBase(bases, &actBase, line, basesIDs, &error);
      break;
      case 3:
      addForward(bases, actBase, line, &error);
      break;
      case 4:
      printForward(bases, actBase, line, &error);
      break;
      case 5:
      printReverse(bases, actBase, line, &error);
      break;
      case 6:
      rem(bases, actBase, line, &error);
      break;
      case 7:
      nonTrival(bases, actBase, line, &error);
    }
    if(end==false  && error==false) instruction=getInstruction(line, &end, &error);
  }

  free((void*)line);
  for(int i=0;i<NUMBER_OF_BASES;++i) free((void*)*(basesIDs+i));
  free((void*)basesIDs);

  if(error==true) return true;
  else return false;
}

/** @brief Główna funkcja programu.
 * @return 0 gdy nie wystąpił błąd, 1 w przeciwnym wypadku.
 */

int main(void)
{
  pfT *bases[NUMBER_OF_BASES];

  for(int i=0;i<NUMBER_OF_BASES;++i){
    bases[i]=NULL;
  }

  bool error=doInstructions(bases);

  for(int i=0;i<NUMBER_OF_BASES;++i){
    phfwdDelete(bases[i]);
  }

  if(error==true) return 1;
  else return 0;
}
