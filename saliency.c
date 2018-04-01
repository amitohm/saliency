/******************************************************************************
 *
 * Copyright(c) 2010 Amit Sivaprasad <mail.me.a.myth@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#include "saliency.h"

////////////
//main
//////////
INT32 main(INT32 argc, char *argv[]) {
    FILE *fp;

    UINT8 byte;
    INT8 out_fname[100];

    INT32 rows, cols, depth, i, j;
    INT32 count;
    UINT8 *rgb_input,*temp,*phi;

    static salinfo_t salinfo;
    object_t *objbegin = NULL,*objcurrent,*objtemp;

    ////////////////
    // image read //
    ////////////////
    if(argc<2) {
            printf("Enter Filenames.");
            exit(0);
    }

    fp = fopen(argv[1],"rb");
    if(fp==NULL) {
            printf("Error accessing %s\n",argv[1]);
            exit(0);
    }
    byte = fgetc(fp);
    if(byte != 'P') {
            printf("Fatal error: Image starts not with P.\n");
            exit(0);
    }
    byte = fgetc(fp);
    if(byte != '6') {
            printf("Fatal error: Not a valid ppm image.\n");
            exit(0);
    }
    byte = fgetc(fp);//newline
    // get rid of those damn comments!
    byte = fgetc(fp);
    if (byte == '#') {
        do {
            byte = fgetc(fp);
        } while(byte != '\n');//tolerate crap
    }
    else {
        ungetc((INT32)byte, fp);//wow! no crap!
    }

    fscanf(fp,"%d %d\n%d",&cols,&rows,&depth);
    byte = fgetc(fp);//the damn '\n' after depth!

    /*******************************************************************************************/

    j = 0;
    for(i=0; argv[1][i] != '\0';i++) {
        if(argv[1][i] == '/')
            j = i + 1;
    }

    i = 0;
    while(argv[1][j] != '.') {
        argv[1][i] = argv[1][j];
        i++;
        j++;
    }
    argv[1][i] = '\0';

    /*******************************************************************************************/

    /////////////
    // mallocs //
    /////////////

    phi = (UINT8 *)malloc(cols*rows*sizeof(UINT8));
    if(phi==NULL) {
            printf("Error allocating memory for phi.\n");
            exit(0);
    }

    rgb_input = (UINT8 *)malloc(3*cols*rows*sizeof(UINT8));
    if(rgb_input==NULL) {
        printf("Error allocating memory for rgb_input.\n");
        free(phi);
        exit(0);
    }

    fread(rgb_input,sizeof(UINT8),(3*cols*rows),fp);
    fclose(fp);

    ////////////////
    // edgedetect //
    ////////////////
    temp = (UINT8 *)malloc(cols*rows*sizeof(UINT8));
    if(temp==NULL) {
        printf("Error allocating memory for temp.\n");
        free(phi);
        free(rgb_input);
        exit(0);
    }

    salinfo.cols = cols;
    salinfo.rows = rows;
    salinfo.depth = depth;

    salinfo.fname = argv[1];
    salinfo.rgb_input = rgb_input;
    salinfo.temp = temp;
    salinfo.phi = phi;

    objbegin = detectobject(&salinfo);

    free(temp);
    free(phi);

    /////////////////
    // image write //
    /////////////////
    objcurrent = objbegin;

    count = 0;

    printf("#\tArea\t\tAvgBrightness\tAvgColour\tPosition\tContrast\n");

    while(objcurrent != NULL) {
        if(objcurrent->rank<=9) {
            for(i=objcurrent->ymin;i<=objcurrent->ymax;i++) {
                if((i==objcurrent->ymin)||(i==objcurrent->ymax)) {
                    for(j=objcurrent->xmin;j<=objcurrent->xmax;j++) {
                        rgb_input[3*((i*cols)+j)+0] = 0;
                        rgb_input[3*((i*cols)+j)+1] = 0;
                        rgb_input[3*((i*cols)+j)+2] = 0;
                    }
                }
                else {
                    j = objcurrent->xmin;
                    rgb_input[3*((i*cols)+j)+0] = 0;
                    rgb_input[3*((i*cols)+j)+1] = 0;
                    rgb_input[3*((i*cols)+j)+2] = 0;

                    j = objcurrent->xmax;
                    rgb_input[3*((i*cols)+j)+0] = 0;
                    rgb_input[3*((i*cols)+j)+1] = 0;
                    rgb_input[3*((i*cols)+j)+2] = 0;
                }
            }

            count++;

            //stick numbers
            sprintf(out_fname,"numbers/%d.raw",objcurrent->rank);
            fp = fopen(out_fname,"rb");
            if (fp==NULL) {
                printf("Error accessing %s\n",out_fname);
                free(rgb_input);
                exit(0);
            }

            for(i=objcurrent->ymin;i<(objcurrent->ymin+16);i++) {
                for(j=objcurrent->xmin;j<(objcurrent->xmin+16);j++) {
                    rgb_input[3*((i*cols)+j)+0] = fgetc(fp);
                    rgb_input[3*((i*cols)+j)+1] = fgetc(fp);
                    rgb_input[3*((i*cols)+j)+2] = fgetc(fp);
                }
            }
            fclose(fp);
            //stick numbers

            printf("%d\t%0.1f\t\t%0.1f\t\t%0.1f\t\t%0.1f\t\t%0.1f\n",objcurrent->rank,
                    objcurrent->rel_area,objcurrent->rel_brightess,
                    objcurrent->rel_colour,objcurrent->rel_pos,
                    objcurrent->curve_count);			
        }
        objtemp = objcurrent->next;
        free(objcurrent);
        objcurrent = objtemp;
    }

    printf("\nNo. of objects = %d\n",count);

    sprintf(out_fname,"%s_output.ppm",argv[1]);
    fp = fopen(out_fname,"wb");
    if (fp==NULL) {
        printf("Error accessing %s\n",out_fname);
        free(rgb_input);
        exit(0);
    }
    fprintf (fp,"P6\n%d %d\n%d\n",cols,rows,depth);
    fwrite(rgb_input,sizeof(UINT8),(3*cols*rows),fp);
    fclose(fp);

    free(rgb_input);
    //getchar();

    return 0;
}

void write_pgm(salinfo_t *salinfo,INT8 *fname) {
    FILE *fp;
    INT8 out_fname[100];

    sprintf(out_fname,"%s_%s.pgm",salinfo->fname,fname);
    fp = fopen(out_fname,"wb");
    if (fp==NULL) {
        printf("Error accessing %s.\n",out_fname);
        free(salinfo->phi);
        free(salinfo->rgb_input);
        free(salinfo->temp);
        exit(0);
    }
    fprintf (fp,"P5\n%d %d\n%d\n",salinfo->cols,salinfo->rows,salinfo->depth);
    fwrite(salinfo->temp,sizeof(UINT8),(salinfo->cols*salinfo->rows),fp);
    fclose(fp);

    return;
}
