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

void objectrank(object_t *objbegin,salinfo_t *salinfo) {
    INT32 objcount,rank;
    DOUBLE max;
    object_t *objcurrent,*objsalient;
    /********************************************************************************/
    objcount = 0;
    brightnessrank(objbegin,salinfo);
    colourfulnessrank(objbegin,salinfo);
    positionrank(objbegin,salinfo);
    arearank(objbegin,salinfo);
    contrastrank(objbegin);
    objcurrent = objbegin;
    while(objcurrent != NULL) {
        objcurrent->saliency_val = 0.3*objcurrent->rel_area + 0.3*objcurrent->rel_brightess + 
                                                0.2*objcurrent->rel_colour + 0.1*objcurrent->rel_pos + 
                                                0.1*objcurrent->curve_count;
        objcurrent->rank = 0;
        objcount++;
        objcurrent = objcurrent->next;
    }
    rank = 1;
    while(objcount--) {
        max = 0;
        objsalient = NULL;
        objcurrent = objbegin;
        while(objcurrent != NULL) {
            if(objcurrent->rank==0) {
                max = MAX(max,objcurrent->saliency_val);		
                if(max==objcurrent->saliency_val) {
                    objsalient = objcurrent;
                }
            }
            objcurrent = objcurrent->next;
        }	
        objsalient->rank = rank;
        rank++;
    }
    return;
}

void contrastrank(object_t *objbegin) {
    DOUBLE max,min;
    object_t *objcurrent;
    /********************************************************************************/
    max = 0.0;
    min = (1.0)*(1<<30);
    objcurrent = objbegin;
    while(objcurrent != NULL) {
        max = MAX(max,objcurrent->curve_count);
        min = MIN(min,objcurrent->curve_count);
        objcurrent = objcurrent->next;
    }	
    objcurrent = objbegin;
    while(objcurrent != NULL) {
        /*check if max!=min to ensure there are more than 1 objects.
        In case there is only 1 object relative position makes no sense.*/
        if(max!=min) {
            objcurrent->curve_count = (100.0*(objcurrent->curve_count - min))/(max - min);
        }
        objcurrent = objcurrent->next;
    }
    return;
}

void arearank(object_t *objbegin,salinfo_t *salinfo) {
    INT32 cols,rows;
    INT32 new_cols,new_rows;
    INT32 total_area,area;
    object_t *objcurrent;

    /********************************************************************************/

    cols = salinfo->cols;
    rows = salinfo->rows;

    total_area = cols * rows;

    objcurrent = objbegin;
    while(objcurrent != NULL) {
        new_cols = objcurrent->xmax - objcurrent->xmin + 1;
        new_rows = objcurrent->ymax - objcurrent->ymin + 1;
        area = new_cols*new_rows;
        objcurrent->rel_area = (100.0*area)/total_area;
        objcurrent = objcurrent->next;
    }	
    return;
}

void brightnessrank(object_t *objcurrent,salinfo_t *salinfo) {
    UINT8 *rgb_input;
    INT32 count,i,j,y,cols;
    DOUBLE brightsum;
    /********************************************************************************/
    rgb_input = salinfo->rgb_input;
    cols = salinfo->cols;
    while(objcurrent != NULL) {
        count = 0;
        brightsum = 0.0;
        for(i=objcurrent->ymin;i<=objcurrent->ymax;i++) {
            for(j=objcurrent->xmin;j<=objcurrent->xmax;j++) {
                y = (INT32)((0.299 * rgb_input[3*((i*cols)+j)+0]) + 
                                        (0.587 * rgb_input[3*((i*cols)+j)+1]) + 
                                        (0.114 * rgb_input[3*((i*cols)+j)+2]));
                y = MAX(y,0);
                y = MIN(y,255);
                brightsum += (DOUBLE)y;
                count++;
            }
        }
        objcurrent->rel_brightess = (brightsum*100)/(count*255);
        objcurrent = objcurrent->next;
    }
}


void positionrank(object_t *objbegin,salinfo_t *salinfo) {
    INT32 cols,rows;
    INT32 xmod,ymod;
    DOUBLE max,min;
    object_t *objcurrent;
    /********************************************************************************/
    cols = salinfo->cols;
    rows = salinfo->rows;

    max = 0.0;
    min = (1.0)*(1<<30);

    objcurrent = objbegin;
    while(objcurrent != NULL) {
        xmod = cols/2 - (objcurrent->xmin+(objcurrent->xmax-objcurrent->xmin)/2);
        ymod = rows/2 - (objcurrent->ymin+(objcurrent->ymax-objcurrent->ymin)/2);
        objcurrent->rel_pos = sqrt(xmod*xmod + ymod*ymod);
        max = MAX(max,objcurrent->rel_pos);
        min = MIN(min,objcurrent->rel_pos);
        objcurrent = objcurrent->next;
    }
    objcurrent = objbegin;
    while(objcurrent != NULL) {
        /*check if max!=min to ensure there are more than 1 objects.
        In case there is only 1 object relative position makes no sense.*/
        if(max!=min) {
            objcurrent->rel_pos = (100.0*(objcurrent->rel_pos - min))/(max - min);
            objcurrent->rel_pos = 100.0 - objcurrent->rel_pos;
        }	
        objcurrent = objcurrent->next;
    }	
    return;
}
