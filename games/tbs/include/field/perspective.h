#ifndef ALCHEMY_PERSPECTIVE_H
#define ALCHEMY_PERSPECTIVE_H

/* @file */
/* The world map's perspective view state. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* The world map's perspective view: the followed position, screen shake, */
/* view position, window, pitch and yaw, tile ids and zoom, in */
/* HEAP_SLOT_MAP_GRID where a map keeps its MAP_GRID_SCROLL_STATE. */
typedef struct WORLD_MAP_VIEW {
	s32 *origin; /* the followed point, as in MAP_GRID_SCROLL_STATE */
	/* The screen shake, as in MAP_GRID_SCROLL_STATE. */
	s32 shakeX, shakeY, shakeDecay;
	u8 *script; /* NULL at load */
	/* MAP_GRID_SCROLL_STATE's flags: REG_DISPCNT bits, BG0 to BG3 and the
	 * sprites
	 */
	/* at load. */
	u16 shownLayers;
	u8 wideTiles; /* MAP_GRID_SCROLL_STATE's wideTiles, 0x80 at load */
	/* As MAP_GRID_SCROLL_STATE: footprints, tileset animations and blend
	 * script.
	 */
	u8 anmsAndBlend[228 - 23];
	FIXED view[2]; /* the view's top left, x then y, 16.16 fixed-point pixels */
	FIXED scaleX;  /* 32 in 16.16 at load */
	FIXED scaleY;  /* 64 in 16.16 at load */
	FIXED limitX;  /* 8160 in 16.16 at load */
	FIXED limitY;  /* 8160 in 16.16 at load */
	/* Set by fdWorldMapStartStillImage while BG1 shows the drawn map; the */
	/* tileset animations are then neither suspended nor resumed. */
	s8 anmActive;
	u8 unused[3];
	/* Screen rows, 0 and 159 at load and 200 and 255 from */
	/* fdWorldMapStartStillImage; each frame copies them to shownTop and */
	/* shownBottom and picks BG mode 2 or 0 from them. */
	/* @{ */
	u16 winTop;
	u16 winBottom;
	/* @} */
	u16 shownTop;	 /* windowTop as the frame update last applied it */
	u16 shownBottom; /* winBottom as the frame update last applied it */
	u16 winPending;	 /* cleared by the frame update; not set by name */
	u8 unusedAfterWin[6];
	/* The world map's tile offsets (ResourceId_WorldMapPerspectiveTiles), each
	 */
	/* tile LZ-compressed. */
	void *tiles;
	u8 unusedAfterTiles[4];
	u16 pitch;		/* 1/65536 turns */
	u16 yaw;		/* 1/65536 turns */
	u32 *resources; /* a row of WorldMap_GraphicsResources, by terrain */
	u8 unusedAfterResources[24];
	u16 tileIds[256]; /* the world map tile in each of 16 by 16 cells */
	/* The tile each quarter of each layer's block buffer shows, so an unchanged
	 */
	/* quarter is not decoded again. */
	u16 quarterTiles[8];
	s32 farPlane;			/* 510 pixels at load */
	s32 distance;			/* farPlane scaled by the zoom */
	s32 prevZoom;			/* the zoom a transition starts from */
	FIXED zoom;				/* 16.16 fixed point, FIXED_ONE full size */
	s16 zoomTransitionFrm;	/* a zoom transition's length; 0 when none */
	s16 zoomTransitionStep; /* frames of the transition done */
} WORLD_MAP_VIEW;

STATIC_ASSERT(sizeof(WORLD_MAP_VIEW) == 860);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, view) == 228);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, pitch) == 280);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, anmActive) == 252);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, resources) == 284);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, tileIds) == 312);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, quarterTiles) == 824);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, farPlane) == 840);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, zoom) == 852);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, prevZoom) == 848);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, zoomTransitionFrm) == 856);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, zoomTransitionStep) == 858);

/* The first tile, in BG1's character base, of the 30 by 20 tile picture of */
/* 256 colours that screen block 6 numbers: the still image's */
/* (fdWorldMapStartStillImage), and the map screen's (fdMapScrShow). */
#define WORLD_MAP_STILL_FIRST_TILE 424

#endif
