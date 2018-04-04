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

void track(UINT8 *result,UINT8 *phi,INT32 *k,INT32 *l,INT32 cols) {
    INT32 i,j;
    i = *k;
    j = *l;

    switch(phi[i+j]) {
        case 0:
        if(result[i+j+1]>30) {
            j++;
        } else if(result[i+j+cols+1]>30&&result[i+j+cols+1]>result[i+j-cols+1]) {
            i += cols;
            j++;
        }else if(result[i+j-cols+1]>30) {
            i -= cols;
            j++;
        }
        break;
        
        case 1*32:
        if(result[i+j-cols+1]>30) {
            i -= cols;
            j++;
        } else if(result[i+j-cols]>30&&result[i+j-cols]>result[i+j+1]) {
            i -= cols;
        } else if(result[i+j+1]>30) {
            j++;
        }
        break;
        
        case 2*32:
        if(result[i+j-cols]>30) {
            i -= cols;
        } else if(result[i+j-cols-1]>30&&result[i+j-cols-1]>result[i+j-cols+1]) {
            i -= cols;
            j--;
        } else if(result[i+j-cols+1]>30) {
            i -= cols;
        j++;
        }
        break;
        
        case 3*32:
        if(result[i+j-cols-1]>30) {
            i -= cols;
            j--;
        } else if(result[i+j-1]>30&&result[i+j-1]>result[i+j-cols]) {
            j--;
        } else if(result[i+j-cols]>30) {
            i -= cols;
        }
        break;
        
        case 4*32:
        if(result[i+j-1]>30) {
            j--;
        } else if(result[i+j-cols-1]>30&&result[i+j-cols-1]>result[i+j+cols-1]) {
            i -= cols;
            j--;
        } else if(result[i+j+cols-1]>30) {
            i += cols;
            j--;
        }
        break;
        
        case 5*32:
        if(result[i+j+cols-1]>30) {
            i += cols;
            j--;
        } else if(result[i+j+cols]>30&&result[i+j+cols]>result[i+j-1]) {
            i += cols;
        } else if(result[i+j-1]>30) {
            j--;
        }
        break;
        
        case 6*32:
        if(result[i+j+cols]>30) {
            i += cols;
        } else if(result[i+j+cols+1]>30&&result[i+j+cols+1]>result[i+j+cols-1]) {
            i += cols;
            j++;
        } else if(result[i+j+cols-1]>30) {
            i += cols;
            j--;
        }
        break;
        
        case 7*32:
        if(result[i+j+cols+1]>30) {
            i += cols;
            j++;
        } else if(result[i+j+1]>30&&result[i+j+1]>result[i+j+cols]) {
            j++;
        } else if(result[i+j+cols]>30) {
            i += cols;
        }
        break;
    }
    *k = i;
    *l = j;
}

void backtrack(UINT8 *result,UINT8 *phi,INT32 *k,INT32 *l,INT32 cols) {
    INT32 i,j;
    i = *k;
    j = *l;
    switch(phi[i+j])
    {
        case 4*32:
        if(result[i+j+1]>30) {
            j++;
        } else if(result[i+j+cols+1]>30&&result[i+j+cols+1]>result[i+j-cols+1]) {
            i += cols;
            j++;
        } else if(result[i+j-cols+1]>30) {
            i -= cols;
            j++;
        }
        break;

        case 5*32:
        if(result[i+j-cols+1]>30) {
            i -= cols;
            j++;
        } else if(result[i+j-cols]>30&&result[i+j-cols]>result[i+j+1]) {
            i -= cols;
        } else if(result[i+j+1]>30) {
            j++;
        }
        break;

        case 6*32:
        if(result[i+j-cols]>30) {
            i -= cols;
        } else if(result[i+j-cols-1]>30&&result[i+j-cols-1]>result[i+j-cols+1]) {
            i -= cols;
            j--;
        } else if(result[i+j-cols+1]>30) {
            i -= cols;
            j++;
        }
        break;

        case 7*32:
        if(result[i+j-cols-1]>30) {
            i -= cols;
            j--;
        } else if(result[i+j-1]>30&&result[i+j-1]>result[i+j-cols]) {
            j--;
        } else if(result[i+j-cols]>30) {
            i -= cols;
        }
        break;

        case 0*32:
        if(result[i+j-1]>30) {
            j--;
        } else if(result[i+j-cols-1]>30&&result[i+j-cols-1]>result[i+j+cols-1]) {
            i -= cols;
            j--;
        } else if(result[i+j+cols-1]>30) {
            i += cols;
            j--;
        }
        break;

        case 1*32:
        if(result[i+j+cols-1]>30) {
            i += cols;
            j--;
        } else if(result[i+j+cols]>30&&result[i+j+cols]>result[i+j-1]) {
            i += cols;
        } else if(result[i+j-1]>30) {
            j--;
        }
        break;

        case 2*32:
        if(result[i+j+cols]>30) {
            i += cols;
        } else if(result[i+j+cols+1]>30&&result[i+j+cols+1]>result[i+j+cols-1]) {
            i += cols;
            j++;
        } else if(result[i+j+cols-1]>30) {
            i += cols;
            j--;
        }
        break;

        case 3*32:
        if(result[i+j+cols+1]>30) {
            i += cols;
            j++;
        } else if(result[i+j+1]>30&&result[i+j+1]>result[i+j+cols]) {
            j++;
        } else if(result[i+j+cols]>30) {
            i += cols;
        }
        break;
    }
    *k = i;
    *l = j;
}
