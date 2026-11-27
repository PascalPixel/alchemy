#ifndef ALCHEMY_MAP_GRID_H
#define ALCHEMY_MAP_GRID_H

/* The map grid: its layers and metatiles, the metatile behaviors and attributes
   the field reads, its tile animations and blend script, and the world map's
   collision and battle environments. */

#include "field/field_coordinate.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "system.h"

/* The map's running blend script, which sets the blend registers over time
   (fdBlendScriptRun, each frame from fdBlendScriptStart). */
typedef struct BLEND_SCRIPT_STATE {
	Uint16 *script;
	Uint16 *cursor;
	Uint16	delay;
	Uint16	paused;
} BLEND_SCRIPT_STATE;

/* One of a map's 16 tile animations (fdTilesetAnimStartAll): the start of its
   script, where it is, the frames left before its next step, and whether it is
   paused. */
typedef struct MAP_ANIMATION {
	const Uint16 *start;
	const Uint16 *cursor;
	Uint16		  tmr;
	Uint16		  paused;
} MAP_ANIMATION;

/* The map grid: 128 by 128 metatiles, each MAPGRID_METATILE_PIXELS square,
   wrapping at its edges. */
#define MAPGRID_SIZE					128
/* A layer window's row mask (MapLayer.maskY) over the whole grid, which wraps
   there rather than at a window of its own. */
#define MAPGRID_ROW_MASK				((MAPGRID_SIZE - 1) * MAPGRID_SIZE)
/* The bytes between one layer's BG map and the next, from screen block 5. */
#define MAPGRID_LAYER_SCREEN_BYTES		2048
/* The low four bits of a shape's first byte: its kind (TERRAIN_KIND), which
   height function reads its samples (MapGrid_TerrainHeightFunctions). */
#define MAPGRID_SHAPE_KIND_MASK			0xf
/* A 16.16 height in the 8-pixel steps fdMapGridSetMetatileHeight and the
   collision tags take. */
#define METATILE_HEIGHT_STEPS(height)	((height) >> 19)
/* The collision code in fdMapMetatileGetAttributes's halfword, above its
   attribute byte. */
#define METATILE_CODE(attributes)		((attributes) >> 8)
/* fdMapMetatileGetAttributes's halfword: the collision code over the second
   attribute byte (MAP_METATILE.attributeB), and the halfword built from the
   two. */
#define METATILE_CODE_BITS				0xff00
#define METATILE_ATTRIBUTE_BITS			0xff
#define METATILE_ATTRIBUTES(code, bits) (((code) << 8) | (bits))

/* Where a layout's blend script is decoded, in the map collision buffer after
   the collision and the tileset animations; a scene may copy its own there. */
#define MAPGRID_BLEND_SCRIPT (Ram_MapCollision + 7680)
/* A blend script's commands besides its BLDCNT values: the end, which starts it
   over, and a jump, its low byte the halfword pair to go on from. */
#define BLEND_SCRIPT_LOOP	 65535
#define BLEND_SCRIPT_JUMP	 0xfe00

/* A layout's three layers, by what they hold. Most layouts give them priorities
   3, 2 and 1. */
typedef enum MAP_GRID_LAYER_INDEX {
	MAPGRID_LAYER_GROUND = 0, /* `BG3`: the ground, at the back */
	MAPGRID_LAYER_MIDDLE = 1, /* `BG2`: walls and edges, under the actors */
	/* `BG1`: roofs and tree tops, over the actors */
	MAPGRID_LAYER_OVERHEAD = 2,
} MAP_GRID_LAYER_INDEX;

/* A metatile's collision code: the codes the field code reads by name. Cloak,
   Sand, the Cyclone and Hover read theirs on the overhead layer
   (script_commands_field_psynergy.c). */
typedef enum METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_NORMAL = 0, /* open ground, where nothing happens */
	/* 1 to 230: the scene's own, each the step cue it raises as the leader
	   walks onto it (fdUpdateWalking), or as he climbs a wall drawn over it on
	   the overhead layer (scGaiaRockCueWallMetatile); those above are the
	   field's own. */
	METATILE_BEHAVIOR_CUE_FIRST = 1,
	METATILE_BEHAVIOR_CUE_LAST = 230,
	METATILE_BEHAVIOR_CLOAK = 231, /* Cloak hides the leader there */
	METATILE_BEHAVIOR_SAND = 232,  /* Sand lets the leader move there as sand */
	/* A metatile the Cyclone acts on (fdMapGridGetCycloneMetatileKind's kind
	   1). */
	METATILE_BEHAVIOR_CYCLONE = 233,
	/* A metatile the Cyclone carries the leader to, its centre kept in
	   FIELD_STATE.cycloneCarryX and Z (kind 2). */
	METATILE_BEHAVIOR_CYCLONE_CARRY = 234,
	/* A metatile the Cyclone lifts the leader onto, its centre kept in
	   FIELD_STATE.cycloneLiftX and Z (kind 3). */
	METATILE_BEHAVIOR_CYCLONE_LIFT = 235,
	METATILE_BEHAVIOR_HOVER = 236, /* Hover holds the leader up over it */
	/* 242 to 247: examine cues with lines of their own, the same on every map
	   (fdExamineMetatile, Field_ExamineCueMessageOffsets). */
	METATILE_BEHAVIOR_JAR = 242,
	METATILE_BEHAVIOR_BARREL = 243,
	METATILE_BEHAVIOR_WOODEN_BOX = 244,
	METATILE_BEHAVIOR_GRAVE = 245,
	METATILE_BEHAVIOR_BOOKS = 246,
	METATILE_BEHAVIOR_WELL = 247,
	METATILE_BEHAVIOR_JUMP_FORWARD = 254, /* jumped across */
	METATILE_BEHAVIOR_WALL = 255		  /* blocks whatever the heights */
} METATILE_BEHAVIOR;

/* MAP_METATILE.attributeB's bits. */
typedef enum MAP_METATILE_ATTR {
	/* Scoop digs here, or everywhere else where the scene's attributes turn it
	   round (FIELD_STATE.sceneAttributeLow, fdMapGridCanScoop). */
	MAP_METATILE_SCOOP = 0x01,
	/* Pushing against the metatile pushes the actor ahead
	   (fdRunPushMetatile). */
	MAP_METATILE_PUSH = 0x02,
	/* Random battles here roll on the scene's second encounter zone
	   (fdUpdateWalking). */
	MAP_METATILE_SECOND_ZONE = 0x04,
	/* Hazardous ground; with HAZARD_COUNT_ON_METATILES the hazard count rises
	   by the pixels walked on it. */
	MAP_METATILE_HAZARD = 0x08,
	MAP_METATILE_WATER = 0x10, /* water (fdMapGridIsWaterMetatile) */
	MAP_METATILE_LEDGE = 0x20, /* a ledge, jumped down by pushing KEY_DOWN */
	/* A wall the leader can climb (fdMapGridIsClimbableMetatile,
	   fdActorMoveByKeys). */
	MAP_METATILE_CLIMBABLE = 0x40,
	/* a gap the leader jumps across, pushing forward */
	MAP_METATILE_JUMP = 0x80
} MAP_METATILE_ATTR;

/* One metatile of a map layer: its metatile tiles and flags, its collision code
   and a second attribute byte. */
typedef struct MAP_METATILE {
	Uint16 picture : 12;   /* its metatile tiles */
	Uint16 actorLayer : 2; /* the map layer an actor here takes, plus one */
	Uint16 actorPri : 2;   /* the sprite priority an actor here is drawn at */
	Uint8  collisionCode;
	Uint8  attributeB;
} MAP_METATILE;

/* A MAP_METATILE read as one word, as code that ORs bits into a metatile writes
   it: its collision code (the metatile behavior), the attribute bits above it
   and the flags of its metatile tiles. */
#define MAP_METATILE_WORD(behavior, attributes, pictureFlags) \
	((Uint32)(attributes) << 24 | (behavior) << 16 | (pictureFlags))
/* The flags of a metatile's tiles for an actor standing on the metatile: the
   sprite priority it is drawn at and the map layer it takes, which the actors'
   placement reads (MAP_METATILE's actorPri and actorLayer). */
#define METATILE_DRAWS_ACTOR(priority, layer) \
	((priority) << 14 | ((layer) + 1) << 12)

/* A row of MapsWork: a layout's resources, each counted from
   ResourceId_EmptyMap. */
typedef struct MAP_RESOURCES {
	Uint16 header;
	Uint16 pal;
	Uint16 tiles[3]; /* for char blocks 1, 2 and 3 */
	Uint16 anmTiles; /* into MapGridAnimTiles */
} MAP_RESOURCES;

/* Every layout's resources, by map number (field/map_resources.s). */
extern const MAP_RESOURCES MapsWork[];

/* One background layer of the field map: its scroll position, origin, parallax
   against the view, drift and wrap, and its window of metatiles. */
typedef struct MAP_LAYER {
	FIXED x, y;		  /* 16.16 fixed-point pixels */
	FIXED ofsX, ofsY; /* 16.16 fixed-point pixels */
	/* 16.16 fixed point, toFIXED(1) moves with the view */
	FIXED scaleX, scaleY;
	FIXED speedX, speedY; /* 16.16 fixed-point pixels per frame */
	FIXED phaseX, phaseY; /* 16.16 fixed-point pixels */
	/* The window's wrap masks over its metatile columns and rows, maskY's a row
	   times MAPGRID_SIZE: MAPGRID_SIZE - 1 and MAPGRID_ROW_MASK for the whole
	   grid. */
	Uint16 maskX, maskY;
	/* the metatile column and row of the layer's window */
	Uint16 originX, originY;
	/* the layer's first metatile in the map metatile buffer */
	MAP_METATILE *metatiles;
	Uint8		 *terrainIds;
} MAP_LAYER;

/* The map grid's state (heap slot HEAP_SLOT_MAP_GRID): the point the view
   follows, the screen shake, the tile animations and blend script, and the
   view. */
typedef struct MAP_STATE {
	/* The followed point, read as x, height and z in 16.16 fixed-point
	   pixels. */
	FIXED *origin;
	/* Screen shake amplitudes and the factor that scales them each frame;
	   fdMapGridSetScreenShake keeps a component given as negative. */
	Sint32 shakeX, shakeY, shakeDecay;
	Uint8 *script;
	/* Display control bits ORed into DISPCNT; fdScreenTransitionHideBackgrounds
	   clears those of backgrounds 1 to 3. */
	Uint16 dispcnt;
	Uint8  wideTiles;
	/* The ground's marks: bits 1..0 footprints, 3..2 the kind of wading (rooms
	   with a second level set 1). */
	Uint8			   groundEffs;
	MAP_ANIMATION	   anim[16];
	BLEND_SCRIPT_STATE blend;
	FIXED view[2]; /* the view's top left, x then z, 16.16 fixed-point pixels */
	Sint32 minX;
	Sint32 minZ;
	Sint32 maxX;
	Sint32 maxZ;
	/* The camera boxes: a -1-ended list of Sint16 rectangles (x0, z0, x1, z1 in
	   pixels) that fdCameraFollowLeaderInBounds keeps the view out of; NULL for
	   none. */
	const Sint16 *cameraBoxes;
	/* Set while a BG1 animation runs; the tileset animations are then neither
	   suspended nor resumed. */
	Sint8 anmActive;
	/* The sprites' base priority; the scrolling camera draws one above it. */
	Sint8 basePri;
	Uint8 unusedAfterBasePri[2];
	/* Each layer's BG priority, written into its BGCNT; zero hides the
	   layer. */
	Uint8 enabled[3];
	/* The last BLDCNT the blend script wrote; with mode 1 in bits 7..6 its
	   values go to BLDALPHA, else to BLDY. */
	Uint8	  blendCtrl;
	MAP_LAYER layers[4];
	Uint8	  unusedAfterLayers[1908]; /* no code names it */
	Sint32	  farPlane;
	Sint32	  zoom;
	Sint32	  zoomFrom;
	Sint32	  zoomTo;
	Sint16	  zoomFrm;
	Sint16	  zoomStep;
} MAP_STATE;

/* The map's view position (MAP_STATE.view): its left and its top, x then z, in
   16.16 fixed-point pixels. */
static inline FIXED *syMapStateView(MAP_STATE *map)
{
	return map->view;
}

/* The map grid's background layer `layer` (MAP_STATE.layers), in the map state
   of heap slot HEAP_SLOT_MAP_GRID. */
static inline MAP_LAYER *fdMapGridGetLayer(Sint32 layer)
{
	return &((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[layer];
}

/* The bytes of MAP_STATE a field map uses: up to its third layer, the last the
   field scrolls. */
#define MAPGRID_FIELD_STATE_SIZE offsetof(MAP_STATE, layers[3])

/* MAP_STATE.groundEffs' kind of wading, its bits 3..2; none where nothing
   wades. */
#define GROUND_EFFECTS_WADING_MASK	   (3 << 2)
#define GROUND_EFFECTS_WADING(effects) ((Uint32)((effects) << 28) >> 30)

/* A view of the map state as the screen transitions read it: the display
   control bits, the window edges and interrupt lines of a transition, and the
   map's resources. */
typedef struct MAP_VIEW_STATE {
	Uint32	unused[5]; /* map state fields no window view names */
	Uint16	dispcnt;   /* display control bits ORed into DISPCNT */
	Uint8	unusedAfterDispcnt[234]; /* no code names it */
	Sint8	anmActive;
	Uint8	unusedAfterAnmActive[3]; /* no code names it */
	Uint16	winTop;
	Uint16	winBottom;
	Uint16	irqFirstLine;
	Uint16	irqSecondLine;
	Uint16	irqPhase;
	Uint8	unusedAfterIrqPhase[18]; /* no code names it */
	Uint32 *resources;
	Uint8	unusedAfterResources[2124]; /* no code names it */
	Sint8	extraTiles;
	Uint8	pictureShown;
	Uint8	picturePending;
	Sint8	area;
} MAP_VIEW_STATE;

/* A position on the world map, as its walkability check reads it. */
typedef struct WORLD_POS {
	FIXED x;			   /* 16.16 fixed-point pixels */
	Uint8 unusedAfterX[4]; /* no code names it */
	FIXED y;
} WORLD_POS;

/* The world map's collision values (fdWorldMapGetCollision), 1 to 15, the sea
   the ship sails among them. */
typedef enum WORLD_MAP_COLLISION {
	/* The ship sails on WORLD_MAP_COLLISION_SEA_FIRST to
	   WORLD_MAP_COLLISION_SEA_LAST and on shallow water
	   (fdWorldMapIsSailable). */
	WORLD_MAP_COLLISION_SEA_FIRST = 1,
	/* The first collision that is water to Scoop and Parch: the sea from here
	   to WORLD_MAP_COLLISION_SEA_LAST, all but
	   WORLD_MAP_COLLISION_SEA_FIRST. */
	WORLD_MAP_COLLISION_WATER_FIRST = 2,
	WORLD_MAP_COLLISION_SEA_LAST = 4,
	/* The party walks only on WORLD_MAP_COLLISION_WALKABLE_FIRST to
	   WORLD_MAP_COLLISION_WALKABLE_LAST (fdWorldMapIsWalkable). */
	WORLD_MAP_COLLISION_WALKABLE_FIRST = 5,
	/* Shallow water: a ripple follows the leader as it walks, and the ship
	   sails there too. */
	WORLD_MAP_COLLISION_SHALLOW_WATER = 6,
	/* Where neither collision layer has a value. */
	WORLD_MAP_COLLISION_DEFAULT = 7,
	/* Forest: the leader's sprite shows its second layer and no shadow. */
	WORLD_MAP_COLLISION_FOREST = 9,
	WORLD_MAP_COLLISION_WALKABLE_LAST = 12
} WORLD_MAP_COLLISION;

/* A world map battle environment (WorldMap_TerrainBehaviorTable,
   fdWorldMapGetBattleEnvironment): what its ground means for battles, named by
   the battle background fdBattleBackgroundSelectForEnvironment gives it. */
typedef enum BATTLE_ENVIRONMENT {
	BATTLE_ENVIRONMENT_NONE = 0,
	BATTLE_ENVIRONMENT_GRASSLAND = 1, /* ResourceId_GrasslandBBackdrop */
	/* ResourceId_DenseForestBackdrop; encounters take a region's forest
	   zone. */
	BATTLE_ENVIRONMENT_FOREST = 2,
	BATTLE_ENVIRONMENT_DESERT = 3,	  /* ResourceId_DesertBackdrop */
	BATTLE_ENVIRONMENT_BEACH = 4,	  /* ResourceId_RockyBeachBackdrop */
	BATTLE_ENVIRONMENT_SNOWFIELD = 5, /* ResourceId_SnowfieldBBackdrop */
	/* ResourceId_SnowfieldBBackdrop, for the snowfield's forest. */
	BATTLE_ENVIRONMENT_SNOW_FOREST = 6,
	/* ResourceId_RockyBeachBackdrop, for shallow water
	   (WORLD_MAP_COLLISION_SHALLOW_WATER). */
	BATTLE_ENVIRONMENT_SHALLOWS = 7,
	/* The sea the ship sails, collisions 1 to 3 outside the snowfields, where
	   its encounters are (WorldMap_ShipEncounterRules);
	   ResourceId_ShipDeckBackdrop. */
	BATTLE_ENVIRONMENT_SEA = 8,
	/* All the sea of the snowfield regions, where no rule gives encounters. */
	BATTLE_ENVIRONMENT_SNOWFIELD_SEA = 9,
	/* WORLD_MAP_COLLISION_SEA_LAST outside the snowfields, where no rule gives
	   encounters; a battle there would take the ship deck's backdrop. */
	/* XXX: which waters BATTLE_ENVIRONMENT_SEA_LAST covers; the World Map
	   metatiles whose collision is WORLD_MAP_COLLISION_SEA_LAST outside the
	   snowfield regions, read from its collision tiles
	   (fdWorldMapGetCollision), would settle it. */
	BATTLE_ENVIRONMENT_SEA_LAST = 10,
	BATTLE_ENVIRONMENT_BLIZZARD = 11, /* ResourceId_BlizzardBackdrop */
	/* ResourceId_BlizzardBackdrop, for the blizzard's forest. */
	BATTLE_ENVIRONMENT_BLIZZARD_FOREST = 12
} BATTLE_ENVIRONMENT;

void fdMapGridFillCollisionRectangle(Sint32 layer, Sint32 x, Sint32 y,
	Uint32 width, Uint32 height, Sint32 value);

STATIC_ASSERT(sizeof(MAP_LAYER) == 56);
STATIC_ASSERT(offsetof(MAP_STATE, layers) == 264);

STATIC_ASSERT(sizeof(MAP_ANIMATION) == 12);
STATIC_ASSERT(offsetof(MAP_STATE, anim) == 24);
STATIC_ASSERT(offsetof(MAP_STATE, blend) == 216);
STATIC_ASSERT(offsetof(MAP_STATE, blendCtrl) == 263);
STATIC_ASSERT(offsetof(MAP_STATE, minX) == 236);
STATIC_ASSERT(offsetof(MAP_STATE, farPlane) == 2396);
STATIC_ASSERT(offsetof(MAP_STATE, zoomStep) == 2414);
STATIC_ASSERT(offsetof(MAP_VIEW_STATE, dispcnt) == 20);
STATIC_ASSERT(offsetof(MAP_VIEW_STATE, winTop) == 260);
STATIC_ASSERT(offsetof(MAP_VIEW_STATE, irqPhase) == 268);

/* A metatile animation step's source column that ends the steps. */
#define MAPGRID_ANIMATION_STEPS_END 65535

/* A metatile of the map's shape grid: one byte, the tag of the metatile's
   collision shape. */
typedef struct SHAPE_METATILE {
	Uint8 shape;
} __attribute__((packed)) SHAPE_METATILE;

/* The ground height of a layer at (x, z), 16.16 pixels
   (fdMapGridGetTerrainHeight, through the overlay's own import row). */
Sint32 fdMapGetTerrainHeight(Sint32 terrain, Sint32 x, Sint32 z);

#endif
