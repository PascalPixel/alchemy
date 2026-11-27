#ifndef ALCHEMY_MAP_GRID_SCROLL_H
#define ALCHEMY_MAP_GRID_SCROLL_H

// The map layers' scroll state and the background scroll values.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/map_grid.h"
#include "system.h"

// One background layer of the field map: its scroll position, origin, parallax against the view,
// drift and wrap, and its first map metatile.
typedef struct MAP_GRID_LAYER_SCROLL
{
	FIXED  x, y;             // 16.16 fixed-point pixels
	FIXED  offsetX, offsetY; // 16.16 fixed-point pixels
	// 16.16 fixed point, toFIXED(1) moves with the view
	FIXED  scaleX, scaleY;
	FIXED  speedX, speedY; // 16.16 fixed-point pixels per tick
	FIXED  phaseX, phaseY; // 16.16 fixed-point pixels
	Uint16 maskX, maskY;   // a drifting layer's wrap mask, in 8-pixel tiles
	MAP_GRID_METATILE
	*metatiles;            // the layer's first metatile in the map metatile buffer
} MAP_GRID_LAYER_SCROLL;

// The field map's state: the followed position, screen shake, tileset animations, blend script,
// view and its bounds, and three scrolling layers.
typedef struct MAP_GRID_SCROLL_STATE
{
	// The followed point, read as x, height and z in 16.16 fixed-point pixels; fdCameraFollowActor
	// points it at the camera's position.
	FIXED                *origin;
	// Screen shake amplitudes in 16.16 fixed-point pixels, and the 16.16 factor that scales them
	// each tick.
	FIXED                 shakeX, shakeY, shakeDecay;
	Uint8                *markers;    // the layout's treasure spots, for fdPlaceTreasureSpots
	Uint16                flags;      // REG_DISPCNT bits: sprites and the BG1 to BG3 shown
	Uint8                 wideTiles;  // nonzero: tileset animations copy 64-byte tiles
	Uint8                 footprints; // nonzero: the leader leaves footprints
	TILESET_ANIM          animations[16];
	BLEND_SCRIPT_STATE    blend;
	FIXED                 view[2];    // the view's top left, x then y, 16.16 fixed-point pixels
	FIXED                 minX, minY, maxX, maxY; // 16.16 fixed-point pixels
	// Set while a BG1 animation runs; the tileset animations are then neither suspended nor
	// resumed.
	Sint8                 animationActive;
	Uint8                 unused[3];
	// Each layer's BG priority, written into its BGCNT; zero hides the layer.
	Uint8                 enabled[3];
	// The last BLDCNT the blend script set; with mode 1 in bits 7..6 its values go to BLDALPHA,
	// else to BLDY.
	Uint8                 blendControl;
	MAP_GRID_LAYER_SCROLL layers[3]; // BG3, BG2 and BG1, in that order
} MAP_GRID_SCROLL_STATE;

// The map's view position (MAP_GRID_SCROLL_STATE.view): its left and its top, x then y, in 16.16
// fixed-point pixels.
static inline FIXED *syMapGridScrollStateView(MAP_GRID_SCROLL_STATE *grid)
{
	return grid->view;
}

#include "field/perspective.h"

STATIC_ASSERT(sizeof(MAP_GRID_LAYER_SCROLL) == 48);
STATIC_ASSERT(offsetof(MAP_GRID_LAYER_SCROLL, metatiles) == 44);
STATIC_ASSERT(sizeof(MAP_GRID_SCROLL_STATE) == 404);
STATIC_ASSERT(offsetof(MAP_GRID_SCROLL_STATE, animations) == 24);
STATIC_ASSERT(offsetof(MAP_GRID_SCROLL_STATE, blend) == 216);
STATIC_ASSERT(offsetof(MAP_GRID_SCROLL_STATE, view) == 228);
STATIC_ASSERT(offsetof(MAP_GRID_SCROLL_STATE, animationActive) == 252);
STATIC_ASSERT(offsetof(MAP_GRID_SCROLL_STATE, layers) == 260);

// A background's hardware scroll offsets.
typedef struct BG_SCROLL
{
	Uint16 x, y; // pixels
} BG_SCROLL;

// BG0 to BG3's scroll offsets, which the VBlank handler copies to REG_BG0HOFS to REG_BG3VOFS in the
// VBlank after sySynch() sets TickRenderPending.
extern BG_SCROLL BgScrollWork[];

#endif
