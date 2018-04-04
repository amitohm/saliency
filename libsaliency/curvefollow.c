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

static curve_t * createcurve() {
    curve_t *ctemp;
    ctemp = (curve_t *)malloc(sizeof(curve_t));
    if(ctemp == NULL) {
        printf("Error allocting memory.");
        exit(0);
    }
    return(ctemp);
}

static position_t * createposition() {
    position_t *ptemp;
    ptemp = (position_t *)malloc(sizeof(position_t));
    if(ptemp == NULL) {
        printf("Error allocting memory.");
        exit(0);
    }
    return(ptemp);
}

static void destroycurve(curve_t *ccurrent) {
    position_t *pcurrent,*ptemp;

    pcurrent = ccurrent->head;
    while(pcurrent != NULL) {
        ptemp = pcurrent->next;
        free(pcurrent);
        pcurrent = ptemp;
    }
    free(ccurrent);
}

static void curveremove(UINT8 *result,position_t *pcurrent,INT32 cols) {
    INT32 k,l;
    while(pcurrent!=NULL) {
        for(k=0;k<7;k++) {
            for(l=0;l<7;l++) {
                result[pcurrent->pos+cols*(k-3)+(l-3)] = 0;
            }
        }
        pcurrent = pcurrent->next;
    }
}

curve_t * curvefollow(salinfo_t *salinfo) {
    INT32 cols,rows;
    INT32 i,j,i_dummy,j_dummy,height,width,x,y;
    UINT8 *result,*phi;
    curve_t *cbegin;
    curve_t *ccurrent,*ctemp;
    position_t *pcurrent,*ptemp;

    /********************************************************************************/
    result = salinfo->temp;
    phi = salinfo->phi;
    cols = salinfo->cols;
    rows = salinfo->rows;
    ccurrent = cbegin = NULL;
    for(i=6*(cols+1);i<((cols*rows)-7*(cols-1));i+=cols) {
        for(j=0;j<cols-12;j++) {
            if(result[i+j]>THRESHOLD_FOR_Y) {
                ctemp = createcurve();
                ctemp->next = NULL;
                ctemp->len = 0;
                ctemp->head = createposition();
                pcurrent = ctemp->head;
                pcurrent->next = NULL;
                seekbrightspot(result,phi,&i,&j,cols);
                i_dummy = i;
                j_dummy = j;
                ctemp->xmin = cols;
                ctemp->xmax = 0;
                ctemp->ymin = rows;
                ctemp->ymax = 0;

                do {
                    ctemp->len += 1;
                    pcurrent->pos = i+j;

                    x = pcurrent->pos%cols;
                    y = pcurrent->pos/cols;

                    ctemp->xmin = MIN(x,ctemp->xmin);
                    ctemp->xmax = MAX(x,ctemp->xmax);

                    ctemp->ymin = MIN(y,ctemp->ymin);
                    ctemp->ymax = MAX(y,ctemp->ymax);

                    result[i+j] = 0;//any number less than THRESHOLD_FOR_Y -> to prevent following the same curve again
                    pcurrent->prev = createposition();
                    ptemp = pcurrent;
                    pcurrent = pcurrent->prev;
                    pcurrent->next = ptemp;
                    backtrack(result,phi,&i,&j,cols);
                    bandremove(result,phi,pcurrent->next->pos,cols);
                } while(pcurrent->next->pos!=(i+j));
                ptemp = pcurrent->next;
                free(pcurrent);
                ptemp->prev = NULL;

                pcurrent = ctemp->head;
                ctemp->head = ptemp;
                i = i_dummy;
                j = j_dummy;
                ctemp->len--;

                do {
                    ctemp->len += 1;
                    pcurrent->pos = i+j;

                    x = pcurrent->pos%cols;
                    y = pcurrent->pos/cols;

                    ctemp->xmin = MIN(x,ctemp->xmin);
                    ctemp->xmax = MAX(x,ctemp->xmax);

                    ctemp->ymin = MIN(y,ctemp->ymin);
                    ctemp->ymax = MAX(y,ctemp->ymax);

                    result[i+j] = 0;//any number less than THRESHOLD_FOR_Y -> to prevent following the same curve again
                    pcurrent->next = createposition();
                    ptemp = pcurrent;
                    pcurrent = pcurrent->next;
                    pcurrent->prev = ptemp;
                    track(result,phi,&i,&j,cols);
                    bandremove(result,phi,pcurrent->prev->pos,cols);
                }while(pcurrent->prev->pos!=(i+j));
                ptemp = pcurrent->prev;
                free(pcurrent);
                ptemp->next = NULL;
                pcurrent = ctemp->head;
                curveremove(result,pcurrent,cols);
                ctemp->next = NULL;
                ctemp->xmin -= 4;
                ctemp->xmin = MAX(ctemp->xmin,OBJECT_VICINITY);
                ctemp->xmax += 4;
                ctemp->xmax = MIN(ctemp->xmax,cols - OBJECT_VICINITY);
                ctemp->ymin -= 4;
                ctemp->ymin = MAX(ctemp->ymin,OBJECT_VICINITY);
                ctemp->ymax += 4;
                ctemp->ymax = MIN(ctemp->ymax, rows - OBJECT_VICINITY);
                height = ctemp->ymax - ctemp->ymin + 1;
                width = ctemp->xmax - ctemp->xmin + 1;
                ctemp->area = height*width;
                if(ctemp->len>5) {
                    if(cbegin==NULL) {
                        cbegin = ctemp;
                    } else {
                        ccurrent = cbegin;
                        if(ctemp->area>ccurrent->area) {
                            cbegin = ctemp;
                            ctemp->next = ccurrent;
                        } else {
                            while(1) {
                                if(ccurrent->next!=NULL) {
                                    if(ctemp->area>ccurrent->next->area) {
                                        ctemp->next = ccurrent->next;
                                        ccurrent->next = ctemp;
                                        break;
                                    }
                                } else {
                                    ccurrent->next = ctemp;
                                    break;
                                }
                                ccurrent = ccurrent->next;
                            }
                        }
                    }
                } else {
                    destroycurve(ctemp);
                }
                i = i_dummy;
                j = j_dummy;
            }
        }
    }
    ccurrent = cbegin;
    while(ccurrent != NULL) {
        pcurrent = ccurrent->head;
        while(pcurrent != NULL) {
            result[pcurrent->pos] = 255;
            pcurrent = pcurrent->next;
        }
        ccurrent = ccurrent->next;
    }
    return(cbegin);
}
