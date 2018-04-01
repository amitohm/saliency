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

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include "../saliency.h"

/* Debug dumps */
//#define DUMP_EDGEMAP_FRAME 1
//#define DUMP_CURVEFOLLOW_FRAME 1
//#define DUMP_CHROMA_FRAME 1

#define	 MIN(X,Y)	((X) < (Y) ? (X) : (Y))

#define	 MAX(X,Y)	((X) > (Y) ? (X) : (Y))

#define OBJ_WIDTH		100
#define OBJ_HEIGHT		75

#define THRESHOLD_FOR_Y		16

#define OBJECT_VICINITY		8

#define PI 3.14159265

typedef struct position_t	position_t;
struct position_t {
    INT32 pos;
    position_t *next;
    position_t *prev;
};

typedef struct curve_t	curve_t;
struct curve_t {
    INT32 len;
    INT32 area;
    INT32 xmin;
    INT32 xmax;
    INT32 ymin;
    INT32 ymax;

    position_t *head;
    curve_t *next;
};

void edgedetect(salinfo_t *salinfo);
void colourfulness(salinfo_t *salinfo);
curve_t * curvefollow(salinfo_t *salinfo);
position_t * createposition();
curve_t * createcurve();
void seekbrightspot(UINT8 *,UINT8 *,INT32 *,INT32 *,INT32);
void track(UINT8 *,UINT8 *,INT32 *,INT32 *,INT32);
void backtrack(UINT8 *,UINT8 *,INT32 *,INT32 *,INT32);
void bandremove(UINT8 *,UINT8 *,INT32,INT32);
void curveremove(UINT8 *,position_t *,INT32);
void destroycurve(curve_t *ccurrent);
void enclosecurve(curve_t *,INT32);
INT32 checkoverlap(object_t *objcurrent,curve_t *ccurrent);
INT32 checkrepeat(object_t *,object_t *);
object_t * createobject();
void brightnessrank(object_t *objcurrent,salinfo_t *salinfo);
void colourfulnessrank(object_t *objbegin,salinfo_t *salinfo);
void positionrank(object_t *objbegin,salinfo_t *salinfo);
void arearank(object_t *objbegin,salinfo_t *salinfo);
void contrastrank(object_t *);
void objectrank(object_t *objbegin,salinfo_t *salinfo);

#endif
