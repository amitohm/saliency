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

void bandremove(UINT8 *result,UINT8 *phi,INT32 pos,INT32 cols) {
    INT32 k,count;

    switch(phi[pos]) {
        case 0*32:
        k = cols;
        break;

        case 1*32:
        k = (cols+1);
        break;

        case 2*32:
        k = 1;
        break;

        case 3*32:
        k = (cols-1);
        break;

        case 4*32:
        k = cols;
        break;

        case 5*32:
        k = (cols+1);
        break;

        case 6*32:
        k = 1;
        break;

        case 7*32:
        k = (cols-1);
        break;

        default:
        k = 0;
        break;
    }
    count = 0;
    while(count<2) {
        result[pos-k] = 0;
        result[pos+k] = 0;
        k += k;
        count++;
    }
}
