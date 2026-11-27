#ifndef ALCHEMY_RENDER_MODEL_H
#define ALCHEMY_RENDER_MODEL_H

#include "types.h"

/* The model records the battle effects hand to the ARM model renderer. */

/* A corner of a model, as btRenderTransformVertices and btRenderProjectVertices
   read it: signed whole pixels, padded to a word. */
typedef struct MODEL_VERTEX {
	Sint8 x;
	Sint8 y;
	Sint8 z;
	Sint8 unused; /* no code names it */
} MODEL_VERTEX;

/* One projected vertex, as syConvert3Dto2D leaves it. */
typedef struct PROJECTED_VERTEX {
	Sint16 x;	   /* screen pixels */
	Sint16 y;	   /* screen pixels */
	Sint16 depth;  /* projected depth */
	Sint16 unused; /* no code names it */
} PROJECTED_VERTEX;

/* A texture for the ARM model renderer: its size as powers of two and the
   address of its pixels. */
typedef struct MODEL_TEXTURE {
	Uint32		 logWidth : 8;	/* the width is 1 << logWidth pixels */
	Uint32		 logHeight : 8; /* the height is 1 << logHeight pixels */
	Uint32		 unused : 16;	/* no code names it */
	const Uint8 *pixels;
} MODEL_TEXTURE;

/* One entry of the list btRenderDrawModelList draws: which rasterizer draws it,
   its faces and their transformed corners, its texture and its shade.
   btRenderAllocateModelList ends the list with a cleared entry. */
typedef struct RENDER_MODEL {
	long   kind; /* the rasterizer kernel that draws it; 0 ends the list */
	Sint32 flg;	 /* MODEL_TEXTURE_IN_PLACE keeps the texture where it is */
	const Uint16  *mesh;	 /* the faces, as the effect tables write them */
	Sint16		  *vertices; /* the corners btRenderTransformVertices wrote */
	MODEL_TEXTURE *texture;
	/* a textured model's brightness; a solid model's face colours, by
	   address */
	long  shade;
	Uint8 scrollU; /* how far the texture is scrolled across */
	Uint8 scrollV; /* how far the texture is scrolled down */
} RENDER_MODEL;

/* A texture's scroll wraps within 128 texels. */
#define TEXTURE_SCROLL_MASK 0x7f

/* Draws a list of models, ended by a cleared entry: each model's rasterizer is
   copied into IWRAM for a run of its kind, and its texture with it where there
   is room. */
void btRenderDrawModelList(RENDER_MODEL *model);
/* Projects count model vertices through syConvert3Dto2D into screen
   coordinates. */
void btRenderProjectVertices(
	MODEL_VERTEX *vertices, PROJECTED_VERTEX *screen, Sint32 cnt);

#endif
