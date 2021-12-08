#ifndef _MATRICE_H_
#define _MATRICE_H_

#define HOR_MAX 11 /* nb rows    */
#define VER_MAX 11 /* nb columns */

#define CALLBACK_MAT(pf) void (*pf)(int valPeg[][VER_MAX]))

typedef struct MATRICE {
  unsigned char id;
  char *name;
  int valPeg[HOR_MAX][VER_MAX];
} s_matrice;

typedef s_matrice *pmatrice;

typedef void (*FuncPtr)(); //  pointeur de function
// function de GSList
void printValue(pmatrice, void *);
void getNameList(pmatrice, char **);
char *getNameListMatrice(const unsigned int, char **);
void *getMatriceList(unsigned int, int[HOR_MAX][VER_MAX]);
#endif
