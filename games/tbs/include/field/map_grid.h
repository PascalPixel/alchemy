#ifndef ALCHEMY_MAP_GRID_H
#define ALCHEMY_MAP_GRID_H

/* @file */
/* Map grids: their cells, layers, animations and blend scripts, and the map */
/* grid state. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/field.h"
#include "system.h"

/* A map cell's cell behavior (`MAP_GRID_CELL.behavior`): what stepping */
/* on, pushing against or searching the cell does. */
typedef enum CELL_BEHAVIOR {
	CELL_BEHAVIOR_NORMAL = 0, /* open ground: nothing happens there */
	/* 1 to 239: the scene's own events, step trigger or exit when stepped on */
	/* (scRunStepTriggerOrExit), examine trigger when the player presses A */
	/* facing the cell. */
	CELL_BEHAVIOR_EVENT_FIRST = 1,
	CELL_BEHAVIOR_EVENT_LAST = 239,
	/* Ground with encounters of its own: grounds 1 and 2 of the map's zones. */
	CELL_BEHAVIOR_ENCOUNTER_GROUND_A = 240,
	CELL_BEHAVIOR_ENCOUNTER_GROUND_B = 241,
	/* 242 to 247: examine triggers with lines of their own, the same on every
	 */
	/* map (fdExamineCell, Field_ExamineTriggerMessageOffsets). */
	CELL_BEHAVIOR_JAR = 242,
	CELL_BEHAVIOR_BARREL = 243,
	CELL_BEHAVIOR_WOODEN_BOX = 244,
	CELL_BEHAVIOR_GRAVE = 245,
	CELL_BEHAVIOR_BOOKS = 246,
	CELL_BEHAVIOR_WELL = 247,
	/* Jumps forward like CELL_BEHAVIOR_JUMP_FORWARD, but fdUpdateWalking */
	/* never passes it to fdRunPushCell. */
	CELL_BEHAVIOR_JUMP_FORWARD_UNREACHED = 249,
	/* Ground that hurts the party over time: the Lamakan Desert's sand. */
	CELL_BEHAVIOR_DESERT_SAND = 250,
	/* Ground Cloak keeps the leader hidden on; stepping off it ends Cloak. */
	CELL_BEHAVIOR_CLOAK = 251,
	/* 252 to 254: cells the leader pushes against (fdRunPushCell). */
	CELL_BEHAVIOR_LEDGE = 252, /* jumped down with Down held */
	CELL_BEHAVIOR_CLIMB = 253, /* climbed up or down with Up or Down held */
	CELL_BEHAVIOR_JUMP_FORWARD = 254, /* jumped across */
	CELL_BEHAVIOR_WALL = 255		  /* blocks whatever the heights */
} CELL_BEHAVIOR;

/* One cell of a map layer: its cell tiles and flags, its cell behavior and */
/* its shape, an index into the shape table. */
typedef struct MAP_GRID_CELL {
	/* bits: 15..14 the priority an actor standing on it takes, if not 0 */
	/* (also read by fdMapGridGetCellHighFlags) | 13..12 the layer such an */
	/* actor takes, + 1, if not 0 | 11..0 cell tiles (ACTOR_CAMERA_TILE) */
	u16 cellTilesAndFlg;
	u8 behavior; /* an CELL_BEHAVIOR */
	/* Indexes the 4-byte shape table (MapGridShapes): the shape's kind in */
	/* the first byte's low four bits, then its height samples. */
	u8 shape;
} MAP_GRID_CELL;

/* The number of the cell tiles a map cell draws, in its low 12 bits. */
#define MAPGRID_CELL_TILES_ID_MASK 0x00000fff

/* The map's running blend script, which sets the blend registers over time */
/* (fdBlendScriptRun, each frame from fdBlendScriptStart). */
typedef struct BLEND_SCRIPT_STATE {
	u16 *script; /* where 0xffff loops back to; null for none */
	u16 *cursor; /* the next command */
	u16 delay;	 /* frames left before the next command */
	u16 paused;	 /* nonzero holds the script where it is */
} BLEND_SCRIPT_STATE;

/* One of the 16 tileset animations, which copy tiles into VRAM by */
/* script (fdTilesetAnimUpdate): groups of four halfwords, the source tile, */
/* the count, the destination tile and the frames to wait. */
typedef struct TILESET_ANIM {
	const u16 *start;  /* where 0xffff loops back to; null for none */
	const u16 *cursor; /* the next group */
	u16 tmr;		   /* frames left of the current wait */
	u16 paused;		   /* nonzero, set by a scene, holds the animation */
} TILESET_ANIM;

/* A layout's three layers, by what they hold. Most layouts give them */
/* priorities 3, 2 and 1. */
typedef enum MAP_GRID_LAYER_INDEX {
	MAPGRID_LAYER_GROUND = 0, /* `BG3`: the ground, at the back */
	MAPGRID_LAYER_MIDDLE = 1, /* `BG2`: walls and edges, under the actors */
	MAPGRID_LAYER_OVERHEAD =
		2, /* `BG1`: roofs and tree tops, over the actors */
	MAPGRID_LAYER_COUNT = 3
} MAP_GRID_LAYER_INDEX;

STATIC_ASSERT(sizeof(TILESET_ANIM) == 12);

/* A position on the world map, as its walkability check reads it. */
typedef struct WORLD_POS {
	FIXED x;	  /* 16.16 fixed-point pixels */
	FIXED height; /* 16.16 fixed-point pixels */
	FIXED y;	  /* 16.16 fixed-point pixels */
} WORLD_POS;

/* The layout's cell tiles, 32 KB of EWRAM at 0x02020000: each cell's tiles */
/* 8 bytes, its four tile entries. fdMapGridLoadMap decodes them in. */
extern u32 CellTiles[];
/* The World Map's 32-pixel blocks, 32 to a row of CellTiles, wrapping */
/* round: a 16.16 coordinate shifted down this far is a block. */
#define WORLD_MAP_BLOCK_SHIFT	21
#define WORLD_MAP_BLOCK_PIXELS	32
#define WORLD_MAP_BLOCKS_ACROSS 32

/* The tileset animations' tiles, 16 KB of EWRAM at 0x02028000. */
extern u8 MapGridAnimTiles[];

/* The map's shape table, 16 KB of EWRAM at 0x0202c000: 4-byte entries */
/* that MAP_GRID_CELL.shape indexes. */
extern u8 MapGridShapes[];

/* 32 KB of EWRAM at 0x02038000 holding background tiles, such as the world */
/* map's char blocks 2 and 3, which fdWorldMapEndStillImage copies back */
/* after the still image. */
extern u8 BgTileBuffer[];

/* The map grid: 128 by 128 cells, each 16 pixels square, wrapping at its */
/* edges. */
#define MAPGRID_SIZE 128
/* A map grid row, MAPGRID_SIZE cells, as a shift. */
#define MAPGRID_ROW_SHIFT	7
#define MAPGRID_CELL_PIXELS 16
/* The map grid's cells, row by row, in Ram_MapGridCellBuffer */
/* (system/definitions.h): the cell at column x, row y is */
/* MAPGRID_CELLS[y * MAPGRID_SIZE + x]. */
#define MAPGRID_CELLS ((MAP_GRID_CELL *)Ram_MapGridCellBuffer)
/* A whole pixel coordinate's map cell, in pixels. */
#define MAPGRID_CELL_PIXEL_MASK 0xfff0
/* The low four bits of a shape's first byte: its kind, which height */
/* function reads its samples (MapGrid_TerrainHeightFunctions). */
#define MAPGRID_SHAPE_KIND_MASK 0xf

/* HEAP_SLOT_WORLD_MAP_AFFINE under its own name, which no code reads; the */
/* world map keeps its WORLD_MAP_AFFINE_STATE in the slot. */
#define WorldMapAffineStateWork (*(u8 **)&HeapSlots[HEAP_SLOT_WORLD_MAP_AFFINE])

/* A row of MapsWork: a layout's resources, each counted from */
/* ResourceId_EmptyMap. */
typedef struct MAP_RESOURCES {
	u16 header;
	u16 pal;
	u16 tiles[3]; /* For char blocks 1, 2 and 3. */
	u16 anmTiles; /* Into MapGridAnimTiles. */
} MAP_RESOURCES;
/* Every layout's resources, by map number (src/map/resources.s). */
extern const MAP_RESOURCES MapsWork[];

#endif
