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

object_t * createobject() {
    object_t *objtemp;
    objtemp = (object_t *)malloc(sizeof(object_t));
    if(objtemp == NULL)	{
	printf("Error allocting memory.");
	exit(0);
    }
    return(objtemp);
}

INT32 checkoverlap(object_t *objcurrent,curve_t *ccurrent) {
    INT32 check;	
    if((ccurrent->xmin>objcurrent->xmin&&ccurrent->xmin<objcurrent->xmax)&&((ccurrent->ymin>objcurrent->ymin&&ccurrent->ymin<objcurrent->ymax)
            ||(ccurrent->ymax>objcurrent->ymin&&ccurrent->ymax<objcurrent->ymax))) {
	check = 1;
    } else if((ccurrent->xmax>objcurrent->xmin&&ccurrent->xmax<objcurrent->xmax)&&((ccurrent->ymin>objcurrent->ymin&&ccurrent->ymin<objcurrent->ymax)
            ||(ccurrent->ymax>objcurrent->ymin&&ccurrent->ymax<objcurrent->ymax))) {
	check = 1;
    } else if((ccurrent->xmin>objcurrent->xmin&&ccurrent->xmin<objcurrent->xmax)&&(ccurrent->ymin<objcurrent->ymin&&ccurrent->ymax>objcurrent->ymax)) {
	check = 1;
    } else if((ccurrent->xmax>objcurrent->xmin&&ccurrent->xmax<objcurrent->xmax)&&(ccurrent->ymin<objcurrent->ymin&&ccurrent->ymax>objcurrent->ymax)) {
	check = 1;
    } else if((ccurrent->ymin>objcurrent->ymin&&ccurrent->ymin<objcurrent->ymax)&&(ccurrent->xmin<objcurrent->xmin&&ccurrent->xmax>objcurrent->xmax)) {
	check = 1;
    } else if((ccurrent->ymax>objcurrent->ymin&&ccurrent->ymax<objcurrent->ymax)&&(ccurrent->xmin<objcurrent->xmin&&ccurrent->xmax>objcurrent->xmax)) {
        check = 1;
    } else if((ccurrent->xmin<objcurrent->xmin&&ccurrent->xmax>objcurrent->xmax)&&(ccurrent->ymin<objcurrent->ymin&&ccurrent->ymax>objcurrent->ymax)) {
	check = 1;
    } else {
	check = 0;
    }
    return(check);
}

object_t * detectobject(salinfo_t *salinfo) {
    UINT8 *result;
    INT32 i,j,check,height,width, objcount,cols;
    UINT32 xmin, xmax, ymin, ymax;
    object_t *objbegin, *objcurrent,*objtemp;
    curve_t *cbegin ,*cprevious, *ccurrent;
    
    /********************************************************************************/
    result = salinfo->temp;
    cols = salinfo->cols;
    cprevious = ccurrent = cbegin = NULL;
	
    objbegin = objcurrent = NULL;
    objcount = 0;
    ////////////////////////////////////////////////////////////////////
    edgedetect(salinfo);
#if defined(DUMP_EDGEMAP_FRAME)
    write_pgm(salinfo,"edgemap");
#endif
    //////////////////
    // Curvefollow ///
    //////////////////	
    cbegin = curvefollow(salinfo);
#if defined(DUMP_CURVEFOLLOW_FRAME)
    write_pgm(salinfo,"curvefollow");
#endif
    ///////////////////////////////////
    // Object detection and ranking	///
    ///////////////////////////////////
    cprevious = ccurrent = cbegin;
    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    ///////					detect single objects				////////
    ////////////////////////////////////////////////////////////////////
    while(ccurrent != NULL) {
        height = ccurrent->ymax - ccurrent->ymin + 1;
        width = ccurrent->xmax - ccurrent->xmin + 1;
        check = 0;
        for(i=(ccurrent->ymin-OBJECT_VICINITY);i<=(ccurrent->ymax+OBJECT_VICINITY);i++) {
            for(j=(ccurrent->xmin-OBJECT_VICINITY);j<=(ccurrent->xmax+OBJECT_VICINITY);j++) {
                if((i<ccurrent->ymin) || (i>ccurrent->ymax) || 
                       (j<ccurrent->xmin) || (j>ccurrent->xmax)) {
                    if(result[(i*cols)+j] == 255) {
                        check = 1;
                        break;
                    }
                }
            }
            if(check) {
                break;
            }
        }	
        if((check == 0) && (((height>=75)&&(width>=100)) || ((width>=75)&&(height>=100)))) {
            objcount++;
            objtemp = createobject();
            objtemp->next = NULL;
            objtemp->curve_count = 0;
            if(objcurrent == NULL) {
                objbegin = objtemp;
            } else {
                objcurrent->next = objtemp;
            }
            objcurrent = objtemp;		
            objcurrent->xmax = ccurrent->xmax;
            objcurrent->xmin = ccurrent->xmin;
            objcurrent->ymax = ccurrent->ymax;
            objcurrent->ymin = ccurrent->ymin;
            if(cprevious != ccurrent) {
                cprevious->next = ccurrent->next;
                free(ccurrent);
                ccurrent = cprevious->next;
            } else {
                cbegin = ccurrent->next;
                free(ccurrent);
                cprevious = ccurrent = cbegin;
            }
        } else {
            cprevious = ccurrent;
            ccurrent = ccurrent->next;
        }
    }
    ////////////////////////////////////////////////////////////////////
    ///////		remove curves overlapping single objects		////////
    ////////////////////////////////////////////////////////////////////
    objcurrent = objbegin;
    while(objcurrent != NULL) {
        cprevious = ccurrent = cbegin;
        while(ccurrent != NULL) {
            check = checkoverlap(objcurrent, ccurrent);	
            if(check) {
                objtemp->curve_count++;		
                if(cprevious != ccurrent) {
                    cprevious->next = ccurrent->next;
                    free(ccurrent);
                    ccurrent = cprevious->next;
                } else {
                    cbegin = ccurrent->next;
                    free(ccurrent);
                    cprevious = ccurrent = cbegin;
                }
            } else {
                cprevious = ccurrent;
                ccurrent = ccurrent->next;
            }
        }	
        objcurrent = objcurrent->next;
    }
    ////////////////////////////////////////////////////////////////////
    //////			detect remaining objects					////////
    ////////////////////////////////////////////////////////////////////
    objcurrent = objbegin;
    if(objcurrent != NULL) {
        while(objcurrent->next != NULL) {
            objcurrent = objcurrent->next;
        }
    }
    cprevious = ccurrent = cbegin;
    while(ccurrent != NULL) {
        objcount++;
        objtemp = createobject();
        objtemp->next = NULL;	
        objtemp->curve_count = 0;
	
        objtemp->xmax = ccurrent->xmax;
	objtemp->xmin = ccurrent->xmin;
	objtemp->ymax = ccurrent->ymax;
        objtemp->ymin = ccurrent->ymin;
	cbegin = ccurrent->next;
    	free(ccurrent);
	cprevious = ccurrent = cbegin;
	while(ccurrent!=NULL) {
            check = checkoverlap(objtemp, ccurrent);		
            if(check) {
                xmin = MIN(objtemp->xmin,ccurrent->xmin);
                xmax = MAX(objtemp->xmax,ccurrent->xmax);
                ymin = MIN(objtemp->ymin,ccurrent->ymin);
                ymax = MAX(objtemp->ymax,ccurrent->ymax);
		
                width = xmax - xmin + 1;
                height = ymax - ymin + 1;

		if((height>=DISPLAY_HEIGHT) || (width>=DISPLAY_WIDTH)) 
                    break;
		
                objtemp->curve_count++;
		objtemp->xmin = xmin;
		objtemp->xmax = xmax;
		objtemp->ymin = ymin;
		objtemp->ymax = ymax;

		if(cprevious != ccurrent) {
                    cprevious->next = ccurrent->next;
                    free(ccurrent);
                    //ccurrent = cprevious->next;
                } else {
                    cbegin = ccurrent->next;
                    free(ccurrent);
                    //cprevious = ccurrent = cbegin;
                }
                cprevious = ccurrent = cbegin;//start search from beginning
            } else {
                cprevious = ccurrent;
                ccurrent = ccurrent->next;
            }
        }
        width = objtemp->xmax - objtemp->xmin + 1;
        height = objtemp->ymax - objtemp->ymin + 1;
        if(((height>=75)&&(width>=100)) || ((width>=75)&&(height>=100))) {
            if(objcurrent == NULL) {
                objbegin = objtemp;
            } else {
                objcurrent->next = objtemp;
            }
            objcurrent = objtemp;
        } else {
            free(objtemp);
        }
        cprevious = ccurrent = cbegin;
        ////////////////////////////////////////////////////////////////////
        ///////			remove curves inside current object			////////
        ////////////////////////////////////////////////////////////////////
        while(ccurrent!=NULL) {
            if((ccurrent->xmin>objtemp->xmin) && (ccurrent->xmax<objtemp->xmax) && 
               (ccurrent->ymin>objtemp->ymin) && (ccurrent->ymax<objtemp->ymax)) {
                objtemp->curve_count++;
                if(cprevious != ccurrent) {
                    cprevious->next = ccurrent->next;
                    free(ccurrent);
                    ccurrent = cprevious->next;
                } else {
                    cbegin = ccurrent->next;
                    free(ccurrent);
                    cprevious = ccurrent = cbegin;
                }
            } else {
                cprevious = ccurrent;
                ccurrent = ccurrent->next;
            }
        }
        ccurrent = cbegin;
    }
    colourfulness(salinfo);	
#if defined(DUMP_CHROMA_FRAME)
    write_pgm(salinfo,"chroma");
#endif
    objectrank(objbegin,salinfo);
    return(objbegin);
}
