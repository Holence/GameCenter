#ifndef _MXYSB_
#define _MXYSB_

#define COL 83
#define ROW 41

int custommap[41][83];
int times;
FILE *fp;
int m, n, i, j;

void file_read();
void file_load();

void file_read(){
	if (!(fp = fopen ("custom.txt", "r"))) {
			printf ("Can't read custom.txt'");
			exit(0);
		}
		rewind(fp);
		for (i = 0; i < ROW; i++) {
			for (j = 0; j < COL; j++) {
				fscanf (fp, "%d,", &custommap[i][j]);
			}
			fgetc (fp);	
		}
	fclose (fp);
}

void file_load()
{
	fp = fopen ("custom.txt", "w");

	for (i = 0; i < ROW; i++)
	for (m = 0; m < 1; m++) {
		for (j = 0; j < COL; j++)
		for (n = 0; n < 1; n++)
			fprintf (fp, "%d,", custommap[i][j]);
		fputc ('\n', fp);
	}
	fclose (fp);
}

#endif
