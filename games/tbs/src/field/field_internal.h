#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

/* What every part of the field object shares: its includes and the few names
   more than one part uses. field.c includes it before the parts, and each part
   includes it too, so that a part read on its own still resolves. It places no
   bytes. */

#include "flags.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "battle/battle_camera.h"
#include "field/actor_builtin_scripts.h"
#include "field/actor_script.h"
#include "field/actor_script_operands.h"
#include "field/actor_update_arm.h"
#include "field/curve.h"
#include "field/script.h"
#include "field/facing.h"
#include "field/map_grid_data.h"
#include "field/map_grid_scroll.h"
#include "field/sprite_viewer_scale.h"
#include "field/terrain_tables.h"
#include "field/world_map_affine.h"
#include "field/world_map_lines.h"
#include "graphics/graphics.h"
#include "graphics/palette_cycles.h"
#include "system/sprite_metadata.h"

/* Animation script commands. */
typedef enum ANIMATION_COMMAND {
	ANIMCMD_END = 239,		/* blank the layer and stop its script */
	ANIMCMD_SET_KIND = 240, /* use facing table `arg` */
	ANIMCMD_HOLD = 241,		/* stay on this command and tick for good */
	ANIMCMD_WAIT = 245,		/* keep the current tick `arg` ticks longer */
	ANIMCMD_JUMP = 253,		/* continue at script position `arg` */
	ANIMCMD_PLAY = 254,		/* switch to animation `arg` */
	ANIMCMD_BLANK = 255		/* show nothing for `arg` */
} ANIMATION_COMMAND;

/* The field's pool sizes, and a sprite animation's timer step at normal
   speed. */
enum {
	RESOURCE_SLOT_COUNT = 8, /* ACTOR_SYSTEM_STATE.pictureCaches */
	ANIM_OBJECT_COUNT = 64,	 /* sprites and sprite animations */
	ANIM_LAYER_COUNT = 4,	 /* SPR.entries */
	/* Sixteenths of a tick taken off a sprite animation's timer each tick:
	   one tick. */
	SPRITE_ANIMATION_NORMAL_STEP = 16
};

/* Layered-map viewport distances in 16.16 pixels. The followed point is below
   the vertical centre; scroll bounds and camera following share this origin. */
enum {
	FIELD_VIEW_FOCUS_X = toFIXED(120), /* followed point from the left edge */
	FIELD_VIEW_FOCUS_Y = toFIXED(96),  /* followed point from the top edge */
	FIELD_VIEW_WIDTH = toFIXED(240),   /* the screen's width */
	FIELD_VIEW_HEIGHT = toFIXED(160)   /* the screen's height */
};

/* Sprite ids the field engine's own code creates actors of. */
enum {
	/* the ripple behind the leader in shallow water */
	FIELD_SPRITE_RIPPLE = 24,
	FIELD_SPRITE_FOOTPRINT = 25 /* a footprint, on maps that show them */
};

/* How a layout's metatile tiles are packed: the method byte
   fdTileMapDecodeStagedBuffer reads before the halfwords. */
typedef enum METATILE_TILES_PACKING {
	METATILE_TILES_PACKING_RAW = 0, /* copied as it is */
	/* All the high bytes, then all the low bytes; each value is XORed with the
	   one before. */
	METATILE_TILES_PACKING_SPLIT_DELTA = 1,
	/* each value is XORed with the one before */
	METATILE_TILES_PACKING_DELTA = 2
} METATILE_TILES_PACKING;

/* Where a layout's tileset animation and blend scripts are decoded, in the
   space after the shapes; the world map decodes its tileset animation there
   too. */
#define MAPGRID_ANIMATION_SCRIPT (RamMapGridShapes + 4096)
#define MAPGRID_BLEND_SCRIPT	 (RamMapGridShapes + 7680)

/* One of the world map area's four tile sets, half a character block; they are
   decoded one after another into BgTileBuffer. */
#define WORLD_MAP_TILE_SET_BYTES 8192

/* A row of SpritePictureCacheSources: the packed resource that holds an
   animation number's pictures, ending at number 0. */
typedef struct SPR_PICTURE_CACHE_SOURCE {
	Uint16 resource;
	Sint16 num;
} SPR_PICTURE_CACHE_SOURCE;

extern SPR_PICTURE_CACHE_SOURCE SpritePictureCacheSources[];

#endif
