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

#include "definitions.h"

/************************************************************
For more info on color space convertions,
refer lindbloom.org
************************************************************/
void colourfulness(salinfo_t *salinfo) {
    UINT8 val;
    INT32 i,j,cols,rows;
    UINT8 *rgb_input,*result;
    DOUBLE R,G,B;
    DOUBLE a,b,c;
    DOUBLE X,Y,Z;
    DOUBLE Xr,Yr,Zr,temp;
    DOUBLE u_r,v_r;
    DOUBLE Ls,us,vs,yr,Suv;
    DOUBLE ep,k,maxv,minv;
    DOUBLE *tout;
    /********************************************************************************/
    cols = salinfo->cols;
    rows = salinfo->rows;
    rgb_input = salinfo->rgb_input;
    result = salinfo->temp;

    maxv = -1<<20;
    minv = 1<<20;

    ep = (DOUBLE)(0.0088564516790356308171716757554635);
    k = (DOUBLE)(903.2962962962962962962962962963);

    Xr = (DOUBLE)(0.980670);
    Yr = (DOUBLE)(0.999998);
    Zr = (DOUBLE)(1.182287);

    u_r = ((4*Xr)/((Xr) + (15*Yr) + (3*Zr)));
    v_r = ((9*Yr)/((Xr) + (15*Yr) + (3*Zr)));

    tout = (DOUBLE *)malloc(sizeof(DOUBLE)*cols*rows);
    if(tout==NULL) {
        printf("Error allocating memory for tout.\n");
        exit(0);
    }

    for(i=0;i<rows;i++) {
        for(j=0;j<cols;j++) {
            R = (DOUBLE)(rgb_input[3*((i*cols)+j)+0]);
            R /= 255;
            G = (DOUBLE)(rgb_input[3*((i*cols)+j)+1]);
            G /= 255;
            B = (DOUBLE)(rgb_input[3*((i*cols)+j)+2]);
            B /= 255;

            a = (DOUBLE)(0.6068909);
            b = (DOUBLE)(0.1735011);
            c = (DOUBLE)(0.2003480);

            X = R*a + G*b + B*c;

            a = (DOUBLE)(0.2989164);
            b = (DOUBLE)(0.5865990);
            c = (DOUBLE)(0.1144845);

            Y = R*a + G*b + B*c;

            a = (DOUBLE)(-0.0000000);
            b = (DOUBLE)(0.0660957);
            c = (DOUBLE)(1.1162243);

            Z = R*a + G*b + B*c;

            yr = Y/Yr;

            if(yr>ep)
                Ls = ((116*pow(yr,(1.0/3))) - 16);
            else
                Ls = k*yr;

            if((X!=0)&&(Y!=0)&&(Z!=0)) {
                us = ((4*X)/((X) + (15*Y) + (3*Z)));
                vs = ((9*Y)/((X) + (15*Y) + (3*Z)));
            } else {
                us = 0;
                vs = 0;
            }
            us = (us - u_r);
            vs = (vs - v_r);
            temp = (us*us) + (vs*vs);
            temp = pow(temp,(1.0/2));
            Suv = 13 * temp;
            temp = Suv * Ls;//chroma
            //temp = Suv;//saturation

            maxv = MAX(temp,maxv);
            minv = MIN(temp,minv);

            tout[(i*cols)+j] = temp;
        }
    }
    a = 255.0/(maxv-minv);
    b = -a * minv;
    
    for(i=0;i<rows;i++) {
        for(j=0;j<cols;j++) {
            temp = a*tout[(i*cols)+j] + b;
            val = (UINT8)temp;
            val = MIN(val,255);
            val = MAX(val,0);
            result[(i*cols)+j] = val;
        }
    }
    free(tout);
    return;
}

void colourfulnessrank(object_t *objcurrent,salinfo_t *salinfo) {
    UINT8 *chroma;
    INT32 count,i,j,cols;
    DOUBLE colourfulness;
    /********************************************************************************/
    chroma = salinfo->temp;
    cols = salinfo->cols;

    while(objcurrent != NULL) {
        count = 0;
        colourfulness = 0.0;

        for(i=objcurrent->ymin;i<=objcurrent->ymax;i++) {
            for(j=objcurrent->xmin;j<=objcurrent->xmax;j++) {
                colourfulness += (DOUBLE)chroma[i*cols+j];
                count++;
            }
        }
        objcurrent->rel_colour = (colourfulness*100)/(count*255);
        objcurrent = objcurrent->next;
    }
}
