#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static FILE *fp = NULL;
//  static int val[11] = {0};
static int nb_matrice = 0;
static char nmatrice[255], bound[10];

GSList *listMatrice = NULL;

typedef struct MATRICE {
  unsigned char id;
  char *name;
  int valPeg[11][11];
} s_matrice;

s_matrice currentMatrice;

typedef s_matrice *pmatrice;

static int _getNumberMatrice();
static char *_getNameMatrice();
static void _setValueMatrice(int[][11]);
static void _serializeMatrice(const char *name, void (*pf)(int[][11]));

static FILE *openFileMatrice();
static void readFileMatrice();
static void closeFileMatrice();

// function de GSList
void printValue(pmatrice, void *);
void getNameList(pmatrice, char **);
void getNameListMatrice();
void getMatriceList(guint, int [][11]);

/**
*
*/
void getMatriceList(guint index, int m[][11]){
  gpointer* pm = g_slist_nth_data(listMatrice, index);
}

/**
 *
 */
static void _setValueMatrice(int valPeg[][11]) {
  for (size_t i = 0; i < 11; i++) {
    fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d", &valPeg[i][0], &valPeg[i][1],
           &valPeg[i][2], &valPeg[i][3], &valPeg[i][4], &valPeg[i][5],
           &valPeg[i][6], &valPeg[i][7], &valPeg[i][8], &valPeg[i][9],
           &valPeg[i][10]);
  }
}

/**
 * pour des facilités de DEBUG sur la listMatrice (fichier serializé)
 * appelé par g_slist_foreach(...)
 */
void printValue(pmatrice pm, void *p) {
  printf("\nFrom listMatrice\n");
  printf("\n_serialize : name %s indice %d\n", pm->name, pm->id);
  for (size_t i = 0; i < 11; i++) {
    printf("%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n", pm->valPeg[i][0],
           pm->valPeg[i][1], pm->valPeg[i][2], pm->valPeg[i][3],
           pm->valPeg[i][4], pm->valPeg[i][5], pm->valPeg[i][6],
           pm->valPeg[i][7], pm->valPeg[i][8], pm->valPeg[i][9],
           pm->valPeg[i][10]);
  }
}
/**
 * enregistre la matrice dans une structure
 * conversion matrice.txt -> liste de structures de matrices
 */
static void _serializeMatrice(const char *name,
                              void (*pfSetMatrice)(int valPeg[][11])) {
  static unsigned char ind = 1;
  pmatrice pm = g_new(s_matrice, 1);
  pm->id = ind++;
  pm->name = g_strdup(name);
  pfSetMatrice(pm->valPeg);
  listMatrice = g_slist_append(listMatrice, pm);
  // DEBUG
  g_slist_foreach(listMatrice, (GFunc)printValue, NULL);

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
 *
 */
static int _getNumberMatrice() { return nb_matrice; }

/**
 *
 */
static char *_getNameMatrice() {
  char *name = "Unknown";
  fscanf(fp, "%s %s", bound, nmatrice);
  if (!strcmp("--TYPE", bound))
    return nmatrice;
  return name;
}

/**
 *
 */
FILE *openFileMatrice() {
  fp = fopen("matrice.txt", "r");
  printf("%s\n", "\n\n\n\nProgramme de Lecture Fichier Matrice.txt\n");
  rewind(fp);
  return (fp);
}

/**
 *
 */
void readFileMatrice() {
  void (*pf)();
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
 * p est un tableau dynamique de char qui contient les noms des matrices
 */
void getNameList(pmatrice pm, char **p) {
  static size_t i = 0;
  p[i] = g_strdup(pm->name);
  i++;
}

/**
 *
 */
void getNameListMatrice() {
  char **p = (char **)malloc(sizeof(*p) * nb_matrice);
  if (p != NULL) {
    for (int i = 0; i < nb_matrice; i++) {
      p[i] = (char *)malloc(sizeof(**p) * 255);
      if (p[i] == NULL)
        exit(EXIT_FAILURE);
    }
  } else
    exit(EXIT_FAILURE);
  g_slist_foreach(listMatrice, (GFunc)getNameList, p);
  printf("\nDEBUG nom des matrices %s %s\n", p[0], p[1]);
}

/**
 *
 */
void closeFileMatrice() {
  if (fp != NULL)
    fclose(fp);
}

/**
 *
 */
int main(int argc, char const *argv[]) {
  if (openFileMatrice() != NULL) {
    readFileMatrice();
    closeFileMatrice();
  }
  // on travaille sur la GSList (fichier serializé)
  getNameListMatrice();
  return 0;
}
