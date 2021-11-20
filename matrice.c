#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 int getNumberMatrice();

//  int getNumberMatrice(){
//    if(fp != NULL){
//      while(!feof(fp)){
//        fscanf(fp, "%s", bound);
//        if (!strcmp("--TYPE", bound))
//          nb_matrice++;
//      }
//    }
//   return nb_matrice;
// }

int main(int argc, char const *argv[]) {
  /* code */
  FILE *fp = NULL;
  int val[11] = {0};
  int nb_matrice = 0;
  char nmatrice[255], bound[10];
  fp = fopen("matrice.txt", "r");
  printf("%s\n", "\nProgramme de lecture fichier matrice.txt\n");
  rewind(fp);
  if (fp != NULL) {
    while (!feof(fp)) {
      fscanf(fp, "%s", bound);
      if (strstr(bound, "#"))
        continue;
      else {
        if (!strcmp("--BEGIN", bound)) {
          fscanf(fp, "%s %s", bound, nmatrice);
          if (!strcmp("--TYPE", bound))
            nb_matrice++;
          printf("%s\n", nmatrice);
          for (size_t i = 0; i < 11; i++) {
            fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d", &val[0], &val[1],
                   &val[2], &val[3], &val[4], &val[5], &val[6], &val[7],
                   &val[8], &val[9], &val[10]);
            printf("%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n", val[0],
                   val[1], val[2], val[3], val[4], val[5], val[6], val[7],
                   val[8], val[9], val[10]); /* code */
          }
          fscanf(fp, "%s", bound);
        }
      }
    }
    fclose(fp);
  }
  printf("\nNombre de matrice %d\n", nb_matrice);
  char **p = (char **)malloc(sizeof(*p) * nb_matrice);
  if (p != NULL) {
    for (int i = 0; i < nb_matrice; i++) {
      p[i] = (char *)malloc(sizeof(**p) * 255);
      if (p[i] == NULL)
        exit(EXIT_FAILURE);
    }
  } else
    exit(EXIT_FAILURE);
  strcpy(*p, "English");
  strcpy(*(p + 1), "German");
  printf("\nnom des matrices %s %s\n", p[0], p[1]);
  return 0;
}
