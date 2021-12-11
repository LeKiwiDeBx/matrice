/**
 * @author LeKiwiDeBx (c) [°}<couak!>
 * Friday, December 3, 2021 7:20 PM
 * @version alpha 0.1
 * @license GNU GPL v3 License
 */
#include <assert.h>

#include "matrice.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *fp = NULL;
static unsigned int nb_matrice = 0;
static char nmatrice[255], bound[10];

GSList *listMatrice = NULL;

pmatrice pcurrentMatrice;

static int _getNumberMatrice();
static char *_getNameMatrice();
static void _setValueMatrice(int[][VER_MAX]);
// static void _serializeMatrice(const char *name, void (*pf)(int[][VER_MAX]));
static void _serializeMatrice(const char *name, FuncPtr pf);

static FILE *openFileMatrice(const char *filename);
static void readFileMatrice();
static void closeFileMatrice();

/**
 * @brief recupere la matrice nieme de la GList (matrice)
 * @param index numero de la matrice
 * @param m[][] tableau pour les valeurs de la matrice choisie
 * @return la structure de la matrice nieme (index)
 */
void *getMatriceList(unsigned int index, int m[HOR_MAX][VER_MAX]) {
  pmatrice pm = g_slist_nth_data(listMatrice, index - 1);
  for (size_t i = 0; i < HOR_MAX; i++) {
    for (size_t j = 0; j < VER_MAX; j++) {
      m[i][j] = pm->valPeg[i][j];
    }
  }
  return pm;
}

/**
 * @brief renseigne pour serializer la matrice en cours du fichier
 * @param valPeg[][] tableau de la matrice à serializer
 * @return
 */
static void _setValueMatrice(int valPeg[][VER_MAX]) {
  for (size_t i = 0; i < HOR_MAX; i++) {
    fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d", &valPeg[i][0], &valPeg[i][1],
           &valPeg[i][2], &valPeg[i][3], &valPeg[i][4], &valPeg[i][5],
           &valPeg[i][6], &valPeg[i][7], &valPeg[i][8], &valPeg[i][9],
           &valPeg[i][10]);
  }
}

/**
 * @brief pour des facilités de DEBUG sur la listMatrice (fichier serializé)
 * appelé par g_slist_foreach(...)
 * @param pm structure de matrice à afficher
 * @param void
 * @note provisoire
 */
void printValue(pmatrice pm, void *p) {
  printf("\nFrom listMatrice\n");
  printf("\n_serialize : name %s indice %d\n", pm->name, pm->id);
  for (size_t i = 0; i < HOR_MAX; i++) {
    printf("%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n", pm->valPeg[i][0],
           pm->valPeg[i][1], pm->valPeg[i][2], pm->valPeg[i][3],
           pm->valPeg[i][4], pm->valPeg[i][5], pm->valPeg[i][6],
           pm->valPeg[i][7], pm->valPeg[i][8], pm->valPeg[i][9],
           pm->valPeg[i][10]);
  }
}

/**
 * @brief enregistre la matrice dans une structure
 * conversion matrice.txt -> liste de structures de matrices
 * @param name nom du type de matrice
 * @param pointeur de fonction pour la methode de serialization de la structure
 * matrice
 * void (*pfSetMatrice)(int valPeg[][VER_MAX])
 */
static void _serializeMatrice(const char *name, CALLBACK_MAT(pfSetMatrice) {
  static unsigned char index = 1;
  pmatrice pm = g_new(s_matrice, 1);
  pm->id = index++;
  pm->name = g_strdup(name);
  pfSetMatrice(pm->valPeg);
  listMatrice = g_slist_append(listMatrice, pm);
  // DEBUG
  // g_slist_foreach(listMatrice, (GFunc)printValue, NULL);

  // remplir un tableau de pointeur de structures
  // IDEES GENERALES
  // GSList∗ listMatrice = NULL;
  // pmatrice pm = (pmatrice)malloc(sizeof(s_matrice));
  // OR
  // pmatrice pm = g_new(s_matrice,1); //cleanest ::bit cleaner than
  // malloc(sizeof) affecte les valeurs à pm listMatrice =
  // g_slist_append(listMatrice, pm); g_slist_free(listMatrice); g_free(pm);
  // SNIPPETS:
  // g_slist_foreach(list, (GFunc)print_name, NULL);
  // g_slist_foreach(list, (GFunc)g_free, NULL);
  // g_slist_free(list);
  // ref: https://developer.ibm.com/tutorials/l-glib/
}

/**
 * @brief
 * @note provisoire DEBUG today unused function Friday, December 3, 2021 4:05 PM
 */
static int _getNumberMatrice() {
  return nb_matrice; }

/**
 * @brief retourne le nom du type de matrice lu du fichier
 * @return nom du type dela matrice
 */
static char *_getNameMatrice() {
  char *name = "Unknown";
  fscanf(fp, "%s %s", bound, nmatrice);
  if (!strcmp("--TYPE", bound))
    return nmatrice;
  return name;
}

/**
 * @brief ouvre le fichier texte parametrage des matrices du jeu
 * @return pointeur de fichier
 * @todo passer le nom du fichier ne parametre
 */
FILE *openFileMatrice(const char *nf) {
  fp = fopen(nf, "r");
  printf("\n\n\nProgramme de Lecture Fichier %s\n", nf);
  rewind(fp);
  return (fp);
}

/**
 * @brief algo de lecture du fichier pour sa serialization
 * @return
 */
void readFileMatrice() {
  FuncPtr pf;
  while (!feof(fp)) {
    fscanf(fp, "%s", bound); // read current lines
    if (strstr(bound, "#"))  //# except for comment
      continue;
    else {
      if (!strcmp("--BEGIN", bound)) { //--BEGIN
        nb_matrice++;                  //
        char *name = _getNameMatrice();
        pf = _setValueMatrice;
        _serializeMatrice(name, pf); //--TYPE
      }
    }
  }
}

/**
 * @brief
 * p est un tableau dynamique de char qui contient les noms des matrices
 * appelé par getNameListMatrice
 */
void getNameList(pmatrice pm, char **p) {
  static size_t i = 0;
  p[i] = g_strdup(pm->name);
  i++;
}

/**
 * @brief donne dans un tableau dynamique de char le nom de
 * toutes les matrices
 * @param index
 * @todo
 * [X] passer en parametre un tableau pour les noms ou bien
 *     un @return de tableau PASSAGE TABLEAU DE CHAR**
 * @note
 * [X] nb_matrice est une variable globale et static et crée une dependance
 *     pour le tableau dynamique dont l'allocation memoire depends.
 *     Est-ce que l'allocation doit se faire dans cette fonction? NON
 */
char *getNameListMatrice(const unsigned int index, char **p) {
  g_slist_foreach(listMatrice, (GFunc)getNameList, p);
  /* DEBUG */
  printf("\nDEBUG nom de la matrice index %d est %s\n", index, p[index - 1]);
  /* FIN DEBUG */
  return (index > 0 && index <= nb_matrice) ? p[index - 1] : NULL;
}

/**
 * @brief  clos le fichier texte
 * @return
 */
void closeFileMatrice() {
  if (fp != NULL)
    fclose(fp);
}

/**
 * @brief algo general pour faire tourner les fonctions et tester
 */
int main(int argc, char const *argv[]) {
  const char *nf = "matrice.txt";
  if (openFileMatrice(nf) != NULL) {
    readFileMatrice();
    closeFileMatrice();
  }
  // DRAFT SIMULATION DE LA METHODE matrixLoad(int choice) de matrix.c
  // --------------------------------  VARIANTS  -------------------------
  // int choice = 0;
  // int m[HOR_MAX][VER_MAX];
  // typedef char Matrix[HOR_MAX][VER_MAX];
  // typedef struct s_matrixOfBoard {
  //     int id;
  //     char *name;
  //     Matrix *pShape;
  // } matrixOfBoard;
  // matrixOfBoard currentMatrixOfBoard;
  // Matrix matrixCopy, pMatrixLoad;
  // --------------------------------- FUNCTION  -------------------------
  // if(choice >= 0 && choice < nb_matrice){
  // int index = choice + 1;
  // pcurrentMatrice = getMatriceList(index, m) ;
  // currentMatrixOfBoard.pShape = pcurrentMatrice->valPeg;
  // currentMatrixOfBoard.name = getNameListMatrice(index,p);
  // currentMatrixOfBoard.id = choice;
  // memcpy(matrixCopy, currentMatrixOfBoard.pShape, HOR_MAX * VER_MAX *
  // sizeof(char)) ;
  // pMatrixLoad = matrixCopy;
  // -------------------------------  GARBAGE  ----------------------------
  // return 1;
  //} else {
  // return EXIT_FAILURE ;
  // }
  // ------------------------------ EOF GHOST FUNCTION --------------------

  // on travaille sur la GSList (fichier serializé)
  int index = 1; // DEBUG
  assert(index == 1);
  int n = _getNumberMatrice();
  char **p = (char **)malloc(sizeof(*p) * n);
  if (p != NULL) {
    for (int i = 0; i < n; i++) {
      p[i] = (char *)malloc(sizeof(**p) * 255);
      if (p[i] == NULL)
        exit(EXIT_FAILURE);
    }
  } else
    exit(EXIT_FAILURE);
  getNameListMatrice(index, p);
  /*
   * 1: english 2: german
   * zone test
   **/
  int m[HOR_MAX][VER_MAX];
  pcurrentMatrice = getMatriceList(index, m);
  printf("\nDEBUG:: Nom de la matrice courante %s\n", pcurrentMatrice->name);
  for (size_t i = 0; i < HOR_MAX; i++) {
    for (size_t j = 0; j < VER_MAX; j++) {
      printf("%d ", m[i][j]);
    }
    printf("\n");
  }
  return 0;
}
