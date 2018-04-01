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

void edgedetect(salinfo_t *salinfo) {
    UINT8 *image,*rgb_input,*result,*phi;
    INT32 gaussian[9]={1,4,1,4,10,4,1,4,1};
    INT32 kernel_edgex[9] = {-1,0,1,-2,0,2,-1,0,1};
    INT32 kernel_edgey[9] = {-1,-2,-1,0,0,0,1,2,1};
    INT32 i,j,k,l,y,cols,rows;
    DOUBLE ans0,ans1,ans2;

    /********************************************************************************/

    rgb_input = salinfo->rgb_input;
    phi = salinfo->phi;
    result = salinfo->temp;
    cols = salinfo->cols;
    rows = salinfo->rows;

    image = (UINT8 *)malloc(cols*rows*sizeof(UINT8));
    if(image==NULL) {
        printf("Error allocating memory for image in edgedetect.\n");
        exit(0);
    }
    //////////////////////
    // Image n Result   //
    //Dimension matching//
    //////////////////////
    for(i=0;i<(cols*rows);i++) {
        y = (INT32)((0.299 * rgb_input[(3*i)+0]) + 
                                (0.587 * rgb_input[(3*i)+1]) + 
                                (0.114 * rgb_input[(3*i)+2]));
        y = MAX(y,0);
        y = MIN(y,255);
        image[i] = (UINT8)y;
        result[i] = (UINT8)0;
    }
    ///////////////////////
    // Noise Elimination //
    ///////////////////////
    for(i=2*(cols+1);i<((cols*rows)-(3*(cols-1)));i+=cols) {
        for(j=0;j<cols-4;j++) {
            ans0 = 0;
            for(k=0;k<3;k++) {
                for(l=0;l<3;l++) {
                    ans0 += gaussian[3*k+l]*image[i+j+cols*(k-1)+(l-1)]/115;
                }
            }
            result[i+j] = (UINT8)MIN((UINT8)ans0,255);
        }
    }
    memcpy(image,result,cols*rows);
    memset(result,0,cols*rows);
    //////////////////
    //   Edge	 /////
    // Detection /////
    //////////////////
    for(i=6*(cols+1);i<rows*cols-7*(cols-1);i+=cols) {
        for(j=0;j<cols-12;j++) {
            ans1 = ans2 = 0;
            for(k=0;k<3;k++) {
                for(l=0;l<3;l++) {
                    ans1 += kernel_edgex[3*k+l]*image[i+j+cols*(k-1)+(l-1)];
                    ans2 += kernel_edgey[3*k+l]*image[i+j+cols*(k-1)+(l-1)];
                }
            }
            ans0 = sqrt(ans1*ans1 + ans2*ans2);
            result[i+j] = (UINT8)MIN((UINT8)ans0,255);
            if((result[i+j]>THRESHOLD_FOR_Y)&&(phi!=NULL)) {
                ans0 =(INT32)(atan2(ans1,ans2)*180/PI);
                if((ans0>=0&&ans0<=22.5)||(ans0>=-22.5&&ans0<=0))
                    phi[i+j] = (UINT8)(0*32);
                else if(ans0>22.5&&ans0<67.5)
                    phi[i+j] = (UINT8)(1*32);
                else if(ans0>=67.5&&ans0<=112.5)
                    phi[i+j] = (UINT8)(2*32);
                else if(ans0>112.5&&ans0<157.5)
                    phi[i+j] = (UINT8)(3*32);
                else if((ans0>=157.5&&ans0<=180)||(ans0<=-157.5&&ans0>=-180))
                    phi[i+j] = (UINT8)(4*32);
                else if((ans0>-157.5&&ans0<-112.5)||(ans0>22.5&&ans0<67.5))
                    phi[i+j] = (UINT8)(5*32);
                else if(ans0>=-112.5&&ans0<=-67.5)
                    phi[i+j] = (UINT8)(6*32);
                else if((ans0>-67.5&&ans0<-22.5)||(ans0>112.5&&ans0<157.5))
                    phi[i+j] = (UINT8)(7*32);
            } else {
                result[i+j] = (UINT8)0;
                if(phi!=NULL)
                    phi[i+j] = (UINT8)255;
            }
        }
    }
    free(image);
    return;
}
