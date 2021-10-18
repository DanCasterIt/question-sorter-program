#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct tipo {
    long *offset;
    long *offset_save;
    struct tipo *punt;
} tipo;

int main(int argv, char *argc[]) {
    srand(time(NULL));
    int eof, i = 0, e, rnd, rb = 0;
    long buff, save_buff;
    tipo *pri_ind = NULL, *sco = NULL, *old = NULL, *buf = NULL;
    char str[100], ch = '\n', sw = 'h';
    FILE * fp, * fds;
    
    fds = fopen("save.txt", "r");
    rewind(fds);

    fp = fopen("test.txt", "r");
    rewind(fp);

	if(fds == NULL)	{
		fclose(fds);
    	fds = fopen("save.txt", "w+");
    	rewind(fds);
    	
	    pri_ind = (tipo*) malloc(sizeof (tipo));
	    pri_ind->offset = (long*) malloc(sizeof (long));
	    *pri_ind->offset = ftell(fp);
	    pri_ind->offset_save = (long*) malloc(sizeof (long));
	    *pri_ind->offset_save = ftell(fds);
	    fprintf(fds, "%016X\n", *pri_ind->offset);
	    sco = pri_ind;
	    sco->punt = pri_ind;
		i++;

	    while (!eof) {
	        fscanf(fp, "%s", str);
	        fseek(fp, 1, SEEK_CUR);
	        sw = fgetc(fp);
	        eof = feof(fp);
	        fseek(fp, -2, SEEK_CUR);
	        if (sw == '\n') {
	            sco->punt = (tipo*) malloc(sizeof (tipo));
	            sco = sco->punt;
	            sco->offset = (long*) malloc(sizeof (long));
	            *sco->offset = ftell(fp);
	            sco->offset_save = (long*) malloc(sizeof (long));
	            *sco->offset_save = ftell(fds);
	            fprintf(fds, "%016X\n", *sco->offset);
	            sco->punt = pri_ind;
	            old = sco;
	            i++;
	        }
    	}
	} else	{
		fclose(fds);
    	fds = fopen("save.txt", "r+");
    	rewind(fds);
    	printf("Trovato save.txt. Caricamento...\n\n");
    	
    	do	{
		    save_buff = ftell(fds);
		    rb = fscanf(fds, "%X\n", &buff);
		    if (rb > 0) {
			    pri_ind = (tipo*) malloc(sizeof (tipo));
			    pri_ind->offset = (long*) malloc(sizeof (long));
			    *pri_ind->offset = buff;
			    pri_ind->offset_save = (long*) malloc(sizeof (long));
			    *pri_ind->offset_save = save_buff;
			    sco = pri_ind;
			    sco->punt = pri_ind;
			    i++;
			} else	{
				e = fscanf(fds, "%s\n", str);
				if(e <= 0) return 0;
			}
		} while(rb <= 0);
	    
	    while (!eof) {
	    	save_buff = ftell(fds);
	        rb = fscanf(fds, "%X\n", &buff);
	        eof = feof(fds);
	        if (rb > 0) {
	            sco->punt = (tipo*) malloc(sizeof (tipo));
	            sco = sco->punt;
	            sco->offset = (long*) malloc(sizeof (long));
	            *sco->offset = buff;
	            sco->offset_save = (long*) malloc(sizeof (long));
	            *sco->offset_save = save_buff;
	            sco->punt = pri_ind;
	            old = sco;
	            i++;
	        } else	fscanf(fds, "%s\n", str);
    	}
	}

    sco = pri_ind;
    fseek(fp, *sco->offset, SEEK_SET);

    while (1) {
        fscanf(fp, "%s", str);
        printf("%s ", str);
        if (str[0] == '=') getch();
        else {
            fseek(fp, 1, SEEK_CUR);
            sw = fgetc(fp);
            fseek(fp, -2, SEEK_CUR);
            if (sw == '\n' || sw == EOF) {
                while (ch != 's' && ch != 'n') {
                    printf("\nCorretta? (s o n): ");
                    ch = getch();
                    printf("%c\n", ch);
                    if (ch != 's' && ch != 'n') printf("Digitare solo uno tra i caratteri ammessi.\n");
                }
                if (ch == 's') {
    				fseek(fds, *sco->offset_save, SEEK_SET);
                    fprintf(fds, "xxxxxxxxxxxxxxxx\n", *sco->offset_save);
                    if (sco == sco->punt) {
                        fclose(fp);
                        fclose(fds);
                        return 0;
                    } else {
                		i--;
                        buf = old->punt->punt;
                        free(old->punt->offset);
                        free(old->punt->offset_save);
                        free(old->punt);
                        old->punt = buf;
                        sco = old;
                    }
                }
                if(i > 1)	rnd = (rand() % (i - 1)) + 1;
                for(e = 0; e < rnd; e++)	{
                	old = sco;
					sco = sco->punt;
				}
                if (sco != NULL) fseek(fp, *sco->offset, SEEK_SET);
                ch = '\n';
                printf("-----------------------\n");
            }
        }
    }
}
