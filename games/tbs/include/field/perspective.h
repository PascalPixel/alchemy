#ifndef ALCHEMY_PERSPECTIVE_H
#define ALCHEMY_PERSPECTIVE_H

/* The world map's perspective view state. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* The world map's perspective view: the followed position, screen shake, view
   position, window, pitch and yaw, tile ids and zoom, in HEAP_SLOT_MAP_GRID
   where a map keeps its MAP_GRID_SCROLL_STATE. */
typedef struct WORLD_MAP_VIEW {
	Sint32 *origin; /* the followed point, as in MAP_GRID_SCROLL_STATE */
	/* The screen shake, as in MAP_GRID_SCROLL_STATE. */
	Sint32 shakeX, shakeY, shakeDecay;
	Uint8 *script; /* NULL at load */
	/* MAP_GRID_SCROLL_STATE's flags: REG_DISPCNT bits, BG0 to BG3 and the
	   sprites at load. */
	Uint16 shownLayers;
	Uint8  wideTiles; /* MAP_GRID_SCROLL_STATE's wideTiles, 0x80 at load */
	/* As MAP_GRID_SCROLL_STATE: footprints, tileset animations and blend
	   script. */
	Uint8 anmsAndBlend[228 - 23];
	FIXED view[2]; /* the view's top left, x then y, 16.16 fixed-point pixels */
	FIXED scaleX;  /* 32 in 16.16 at load */
	FIXED scaleY;  /* 64 in 16.16 at load */
	FIXED limitX;  /* 8160 in 16.16 at load */
	FIXED limitY;  /* 8160 in 16.16 at load */
	/* Set by fdWorldMapStartStillImage while BG1 shows the drawn map; the
	   tileset animations are then neither suspended nor resumed. */
	Sint8 anmActive;
	Uint8 unused[3];
	/* Screen rows, 0 and 159 at load and 200 and 255 from
	   fdWorldMapStartStillImage; each tick copies them to shownTop and
	   shownBottom and picks BG mode 2 or 0 from them. */
	Uint16 winTop;
	Uint16 winBottom;
	Uint16 shownTop;	/* windowTop as the tick update last applied it */
	Uint16 shownBottom; /* winBottom as the tick update last applied it */
	Uint16 winPending;	/* cleared by the tick update; not set by name */
	Uint8  unusedAfterWin[6];
	/* The world map's tile offsets (ResourceId_WorldMapPerspectiveTiles), each
	   tile LZ-compressed. */
	void   *tiles;
	Uint8	unusedAfterTiles[4];
	Uint16	pitch;	   /* 1/65536 turns */
	Uint16	yaw;	   /* 1/65536 turns */
	Uint32 *resources; /* a row of WorldMapGraphicsResources, by terrain */
	Uint8	unusedAfterResources[24];
	Uint16	tileIds[256]; /* the world map tile in each of 16 by 16 metatiles */
	/* The tile each quarter of each layer's block buffer shows, so an unchanged
	   quarter is not decoded again. */
	Uint16 quarterTiles[8];
	Sint32 farPlane;		   /* 510 pixels at load */
	Sint32 distance;		   /* farPlane scaled by the zoom */
	Sint32 prevZoom;		   /* the zoom a transition starts from */
	FIXED  zoom;			   /* 16.16 fixed point, toFIXED(1) full size */
	Sint16 zoomTransitionFrm;  /* a zoom transition's length; 0 when none */
	Sint16 zoomTransitionStep; /* ticks of the transition done */
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

/* The first tile, in BG1's character base, of the 30 by 20 tile picture of 256
   colours that screen block 6 numbers: the still image's
   (fdWorldMapStartStillImage), and the map screen's (fdMapScreenShow). */
#define WORLD_MAP_STILL_FIRST_TILE 424

#endif
