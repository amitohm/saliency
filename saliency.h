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

#ifndef _SALIENCY_H_
#define _SALIENCY_H_

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>

typedef char		INT8;
typedef unsigned char	UINT8;

typedef short		INT16;
typedef unsigned short	UINT16;

typedef int		INT32;
typedef unsigned int	UINT32;

typedef double		DOUBLE;

#define DISPLAY_WIDTH	720
#define DISPLAY_HEIGHT	480

typedef struct salinfo_t	salinfo_t;
struct salinfo_t {
	INT32 cols;
	INT32 rows;
	INT32 depth;
	
	INT8 *fname;
	
	UINT8 *rgb_input;
	UINT8 *temp;
	UINT8 *phi;
};

typedef struct object_t		object_t;
struct object_t {
	UINT8 rank;
	INT32 xmin;
	INT32 xmax;
	INT32 ymin;
	INT32 ymax;
	DOUBLE rel_pos;
	DOUBLE curve_count;
	DOUBLE rel_brightess;
	DOUBLE rel_colour;
	DOUBLE rel_area;
	DOUBLE saliency_val;
	
	object_t *next;
};

object_t * detectobject(salinfo_t *salinfo);
void write_pgm(salinfo_t *salinfo,INT8 *fname);

#endif
