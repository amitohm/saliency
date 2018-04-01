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

INT32 checkrepeat(object_t *objtemp,object_t *objcurrent) {
    INT32 poscheck=0,check=0;
    
    if((objtemp->xmin>objcurrent->xmin-0.25*objcurrent->xmin && objtemp->xmin<objcurrent->xmin+0.25*objcurrent->xmin)&&
            (objtemp->ymin>objcurrent->ymin-0.25*objcurrent->ymin && objtemp->ymin<objcurrent->ymin+0.25*objcurrent->ymin)) {
        poscheck = 1;
    } else if((objtemp->xmin>objcurrent->xmin-0.25*objcurrent->xmin && objtemp->xmin<objcurrent->xmin+0.25*objcurrent->xmin)&&
            (objtemp->ymax>objcurrent->ymax-0.25*objcurrent->ymax && objtemp->ymax<objcurrent->ymax+0.25*objcurrent->ymax)) {
        poscheck = 1;
    } else if((objtemp->xmax>objcurrent->xmax-0.25*objcurrent->xmax && objtemp->xmax<objcurrent->xmax+0.25*objcurrent->xmax)&&
            (objtemp->ymin>objcurrent->ymin-0.25*objcurrent->ymin && objtemp->ymin<objcurrent->ymin+0.25*objcurrent->ymin)) {
        poscheck = 1;
    } else if((objtemp->xmax>objcurrent->xmax-0.25*objcurrent->xmax && objtemp->xmax<objcurrent->xmax+0.25*objcurrent->xmax)&&
            (objtemp->ymax>objcurrent->ymax-0.25*objcurrent->ymax && objtemp->ymax<objcurrent->ymax+0.25*objcurrent->ymax)) {
        poscheck = 1;
    } else {
        poscheck = 0;
    }
    //htemp = objtemp->ymax - objtemp->ymin + 1;
    //wtemp = objtemp->xmax - objtemp->xmin + 1;
    //hcurrent = objcurrent->ymax-objcurrent->ymin+1;
    //wcurrent = objcurrent->xmax-objcurrent->xmin+1;

    if(poscheck == 1) {
        check = 1;
    }
    return(check);
}
