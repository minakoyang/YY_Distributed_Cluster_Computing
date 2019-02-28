/* A simple program that demonstrates printing colors to a pnm
   bitmap file.  The format P3 is used for simplicity since it
   uses ASCII format.  Here is a reasonable reference:
       http://local.wasp.uwa.edu.au/~pbourke/dataformats/ppm/
   Let me know if you find a better one.

   Author:  Amy Apon
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

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
const char * colors[10] = { RED, ORANGE, YELLOW, LTGREEN, GREEN, LTBLUE, BLUE, DARKTEAL, BROWN, BLACK };

/* The image will be 1000 pixels wide and 1000 pixels tall */
const int linelen = 1000; //row
const int numlines = 1000; //col

int i, j, p, k;


FILE * fp;

void CopyNewToOld(float newarr[][linelen], float oldarr[][linelen], int numrows) //numrows: rows each process will have
{
    for( i=0; i<numrows; i++){
    	for( j=0; j<linelen; j++)
    		oldarr[i][j]=newarr[i][j];
	}
}
void CalculateNew(float newarr[][linelen], float oldarr[][linelen], int numrows, int rank, int size){
    for (i = 2; i < numrows - 2; i++) {
        for (j = 1; j < linelen - 1; j++) {
            newarr[i][j] = 0.25 * (oldarr[i - 1][j] + oldarr[i + 1][j] + oldarr[i][j - 1] + oldarr[i][j + 1]);
        }
    }

    if (rank != 0) {
        for (j = 1; j < linelen - 1; j++) {
            newarr[1][j] = 0.25 * (oldarr[0][j] + oldarr[2][j] + oldarr[1][j - 1] + oldarr[1][j + 1]);
        }
    }

    if (rank != size  - 1) {
        for (j = 1; j < linelen - 1; j++) {
            newarr[numrows - 2][j] = 0.25 * (oldarr[numrows - 3][j] + oldarr[numrows - 1][j] + oldarr[numrows - 2][j - 1] + oldarr[numrows - 2][j + 1]);
        }
    }
}

void PrintGrid(float newarr[][linelen]){
    for (i=0; i<numlines; i++) {
        for (j=0; j<linelen; j++){
            color=(newarr[i][j]<=250)+(newarr[i][j]<=180)+(newarr[i][j]<=120)+(newarr[i][j]<=80)+(newarr[i][j]<=60)+(newarr[i][j]<=50)+(newarr[i][j]<=40)+(newarr[i][j]<=30)+(newarr[i][j]<=20);
            fprintf(fp, "%s ", colors[color] );
    }
        fprintf(fp, "\n");
    }
}


int main(int argc, char * argv[]) {
    int iteration=50000;
    int step=1000;
    double end=0.0, start=0.0;

    // initialize the MPI evironment

    MPI_Init(&argc, &argv);

    // Get the number of processes
    int numProcs;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    int extnumlines;

    if(numlines%numProcs==0)
        extnumlines=numlines;
    else
        extnumlines=((int)(numlines/numProcs)+1)*numProcs;

    float newarr[extnumlines][linelen];
    float oldarr[extnumlines][linelen];

    /* The pnm filename is hard-coded.  */

    char filename_format[] = "c_%d.pnm";
    char filename[sizeof(filename_format) + 7];  // for up to 4 digit number

    for (i=0; i<extnumlines; i++) {
        for (j=0; j<linelen; j++)
            newarr[i][j]=20;
    }

    int fire_s=(int)(0.3*linelen);
    int fire_e=(int)(0.7*linelen);

    for (j=fire_s; j<=fire_e; j++) {
        newarr[0][j]=300;
    }


    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();


    int rows_proc = (int)(extnumlines/numProcs)+2;


    float mpiarr[rows_proc][linelen];
    float oldmpiarr[rows_proc][linelen];

    for(j=0;j<linelen;j++){
    	if(rank==0){
        	mpiarr[0][j]=20;
        	for(k=1;k<rows_proc;k++){
        		mpiarr[k][j]=newarr[rank*extnumlines/numProcs+k-1][j];
        	}
    	}
    	else if(rank==numProcs-1){
			mpiarr[rows_proc-1][j]=20;
    	    for(k=0;k<rows_proc-1;k++){
    		    mpiarr[k][j]=newarr[rank*extnumlines/numProcs+k-1][j];
    		}
    	}
    	else
    	{
        	for(k=0;k<rows_proc;k++){
        		mpiarr[k][j]=newarr[rank*extnumlines/numProcs+k-1][j];
        	}
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int iter;
 	for(iter=0;iter<iteration;iter++){
    	CopyNewToOld(mpiarr, oldmpiarr, rows_proc);
    	CalculateNew(mpiarr, oldmpiarr, rows_proc, rank, numProcs);

        if ( rank>0 )
        {
          MPI_Send ( &mpiarr[1][0], linelen, MPI_FLOAT, rank-1, 2, MPI_COMM_WORLD );
    	    MPI_Recv ( &mpiarr[0][0], linelen,  MPI_FLOAT, rank-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if ( rank<numProcs-1 )
        {
          MPI_Recv ( &mpiarr[rows_proc-1][0], linelen,  MPI_FLOAT, rank+1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
          MPI_Send ( &mpiarr[rows_proc-2][0], linelen, MPI_FLOAT, rank+1, 1, MPI_COMM_WORLD );
        }


	}

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank==0){
        for (i=0; i<rows_proc-2;i++) {
                for (j=0; j<linelen; j++) {
                newarr[i][j]=mpiarr[i+1][j];
            }
        }
        for ( p=1; p<numProcs; p++){
            MPI_Recv(&mpiarr,rows_proc*linelen,MPI_FLOAT,p,MPI_ANY_TAG,MPI_COMM_WORLD,&status);

            for (i=0; i<rows_proc-2; i++) {
                for (j=0; j<linelen; j++)
                newarr[p*numlines/numProcs+i][j]=mpiarr[i+1][j];
            }
        }


        snprintf(filename, sizeof(filename), filename_format, iteration);
        fp = fopen(filename, "w");
        /* Print the P3 format header */
        fprintf(fp, "P3\n%d %d\n15\n", linelen, numlines);
        PrintGrid(newarr);
        fclose(fp);
        printf("Time=%fs\n",end-start);
    }
    else {
        MPI_Send(&mpiarr,rows_proc*linelen,MPI_FLOAT,0,0,MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
