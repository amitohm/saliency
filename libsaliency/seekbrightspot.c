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

void seekbrightspot(UINT8 *result,UINT8 *phi,INT32 *i,INT32 *j,INT32 cols) {
    switch(phi[*i+*j])
    {
        case 0*32:
        while(result[*i+*j+cols]>result[*i+*j]) {
                *i += cols;
        }
        break;
        
        case 1*32:
        while(result[*i+*j+cols+1]>result[*i+*j]) {
            *i += cols;
            (*j)++;
        }
        break;
        
        case 2*32:
        while(result[*i+*j+1]>result[*i+*j]) {
            (*j)++;
        }
        break;
    
        case 3*32:
        while(result[*i+*j+cols-1]>result[*i+*j]) {
            *i += cols;
            (*j)--;
        }
        break;
        
        case 4*32:
        while(result[*i+*j+cols]>result[*i+*j]) {
            *i += cols;
        }
        break;
        
        case 5*32:
        while(result[*i+*j+cols+1]>result[*i+*j]) {
            *i += cols;
            (*j)++;
        }
        break;
    
        case 6*32:
        while(result[*i+*j+1]>result[*i+*j]) {
            (*j)++;
        }
        break;
    
        case 7*32:
        while(result[*i+*j+cols-1]>result[*i+*j]) {
            *i += cols;
            (*j)--;
        }
        break;
    }
}
