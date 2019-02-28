/* A simple program that demonstrates printing colors to a pnm
   bitmap file.  The format P3 is used for simplicity since it
   uses ASCII format.  Here is a reasonable reference:
       http://local.wasp.uwa.edu.au/~pbourke/dataformats/ppm/
   Let me know if you find a better one.

   Author:  Amy Apon
*/

#include <stdio.h>

/* 10 colors plus white are defined.  Many more are possible. */
#define WHITE    "15 15 15 "
#define RED      "15 00 00 "
#define ORANGE   "15 05 00 "
#define YELLOW   "15 10 00 "
#define LTGREEN  "00 13 00 "
#define GREEN    "05 10 00 "
#define LTBLUE   "00 05 10 "
#define BLUE     "00 00 10 "
#define DARKTEAL "00 05 05 "
#define BROWN    "03 03 00 "
#define BLACK    "00 00 00 "

  int numcolors = 10;
  int color;

  /* Colors are list in order of intensity */
  const char * colors[10] = { RED, ORANGE, YELLOW, LTGREEN, GREEN,
                         LTBLUE, BLUE, DARKTEAL, BROWN, BLACK };
  /* The image will be 1000 pixels wide and 1000 pixels tall */
  const int linelen = 1000;
  const int numlines = 1000;
  int i, j;
  float newarr[numlines][linelen];
  float oldarr[numlines][linelen];

  FILE * fp;

void CopyNewToOld(float newarr[][linelen], float oldarr[][linelen]){
  for(int i = 0; i < numlines; i++){
    for(int j = 0; j < linelen; j++)
      oldarr[i][j] = newarr[i][j];
  }
}

void CalculateNew(float newarr[][linelen], float oldarr[][linelen]) {
  for(int i = 1; i < numlines - 1; i++)
    for(int j = 1; j < linelen - 1; j++)
    newarr[i][j] = 0.25*(oldarr[i - 1][j] + oldarr[i + 1][j] + oldarr[i][j - 1]+ oldarr[i][j+1]);
}

void PrintGrid(float newarr[][linelen]){
  for(i = 0; i < numlines; i++) {
    for( j = 0; j < linelen; j++) {
      color = (newarr[i][j]<=250) + (newarr[i][j]<=180) + (newarr[i][j]<=120) + (newarr[i][j]<=80) + (newarr[i][j]<=60) + (newarr[i][j]<=50) + (newarr[i][j]<=40) + (newarr[i][j]<=30) + (newarr[i][j]<=20);
      fprintf(fp, "%s ", colors[color]);
    }
    fprintf(fp, "\n");
  }
}

int main(int argc, char * argv[]) {
  int iteration = 50000;
  int step = 1000;

  /* The pnm filename is hard-coded.  */

  for (i = 0; i<numlines; i++) {
    for (j = 0; j < linelen; j++)
    newarr[i][j]=20;
  }
  int fire_s=(int)(0.3*linelen); //fire_s fp start position
  int fire_e=(int)(0.7*linelen); //fire_e fp end position

  for (j = fire_s; j<=fire_e; j++)
    newarr[0][j]=300;

    char filename_format[] = "c_%d.pnm";
    char filename[sizeof(filename_format) + 7];
  for(int i=0; i<iteration; i++) {
    CopyNewToOld(newarr, oldarr);
    CalculateNew(newarr, oldarr);
    if(i%step==0) {
      snprintf(filename, sizeof(filename), filename_format, i);
      fp = fopen(filename, "w");
  /* Print the P3 format header */
      fprintf(fp, "P3\n%d %d\n15\n", linelen, numlines);
      PrintGrid(newarr);
      fclose(fp);
    }
  }
  /* Print 300 lines of colors. ASCII makes this easy.           */
   /* Each %s (color string) is a single pixel in the final image */



   /* Convert the pnm file to a format that is more easily viewed
      in a web browser. */
   /*   system("convert c.pnm c.png"); */
   //system("convert c.pnm c.gif");   /* png not supported on comp */

   /* This copies the file to a directory that allows it to be
      opened in a web browser directly.  Browse to
      http://prospero.uark.edu/gifs/c.png */

   /*  CHANGE THE DESTINATION FILE TO A UNIQUE NAME */
   /* This will overwrite an existing file!        */
   /*   system("/bin/cp c.png /var/www/html/gifs/aapon-c.png");*/
   return 0;
}
