#ifndef ALCHEMY_SCENE_SCENE_H
#define ALCHEMY_SCENE_SCENE_H

// What the scenes share: the views of actors, sprites and state they read in their own way, and the
// constants of the mechanisms several scenes keep copies of (geysers, vents, smoke, sparks, cracked
// tiles).

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/field_coordinate.h"
#include "field/sprite.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid.h"
#include "graphics/render_input.h"
#include "scene/mars_lighthouse_aerie.h"
#include "field/scene_tables.h"
#include "system.h"

struct ACTOR;
struct FIELD_EFFECT;
struct POS;
struct FIELD_SPR;
struct PARTICLE;
struct PARTICLE_SPR;
struct POSITION;
struct VECTOR3;

// A map metatile, or a step between metatiles, kept as one 8-byte pair.
typedef struct CELL
{
	Sint32 x, z;
} CELL;

// A field sprite's byte 9: the top of its first tile and its priority (OAM attribute 2), read as
// bit-fields.
typedef struct SPR_ATTR2_HIGH
{
	Uint8 link[4];
	Uint8 y;         // attribute 0's y
	Uint8 attr0High; // attribute 0's high byte
	Uint8 attr1[2];
	Uint8 tileLow;   // attribute 2's low byte
	Uint8 tileHigh : 2;
	Uint8 priority : 2;
} SPR_ATTR2_HIGH;

// An actor spiralling round its anchor (syOrbiterSpiral): the circle it keeps in the actor's
// scene-owned words, and the ticks it has left.
typedef struct ORBITER
{
	const Sint32 *script;
	Sint16        scriptCursor;
	Uint16        facing;
	FIXED         x; // 16.16 fixed-point pixels
	FIXED         y; // 16.16 fixed-point pixels
	FIXED         z; // 16.16 fixed-point pixels
	FIXED         groundHeight;
	FIXED         scaleX;
	FIXED         scaleY;
	Uint16        radius;
	Uint8         layer;
	Uint8         priorityFlags;
	FIXED         velocityX;
	FIXED         velocityY;
	FIXED         velocityZ;
	FIXED         speedLimit;
	FIXED         acceleration;
	FIXED         targetX;
	FIXED         targetY;
	FIXED         targetZ;
	FIXED         bounce;
	FIXED         gravity;
	FIXED         orbitRadius; // 16.16 fixed-point pixels, in sceneWord
	void         *sprite;
	Uint8         spriteStorageKind;
	Uint8         motionFlags;
	Uint8         arrivalAxis;
	Uint8         conditionResult;
	Uint8         snapToTarget;
	Uint8         collisionFlags;
	Uint8         steeringFlags;
	Uint8         paused;
	Uint8         retainSpriteCache;
	Uint8         repeatCount;
	Sint16        wait;
	Uint8         refused;
	Uint8         frozen;
	Uint8         countdown; // ticks left of the spiral
	Uint8         sceneByte;
	Sint16        angle;     // its place on the circle, 1/65536 turns
	Uint16        speed;     // twice its turn a tick, 1/65536 turns
	struct ACTOR *anchor;    // the actor it circles, in watchedActor
} ORBITER;

// An actor circling a centre on a growing circle (syOrbitUpdate), its centre, radius and turn kept
// in the actor's scene-owned words.
typedef struct ORBIT
{
	const Sint32 *script;
	Sint16        scriptCursor;
	Uint16        facing;
	FIXED         x; // 16.16 fixed-point pixels
	FIXED         y; // 16.16 fixed-point pixels
	FIXED         z; // 16.16 fixed-point pixels
	FIXED         groundHeight;
	FIXED         scaleX;
	FIXED         scaleY;
	Uint16        radius;
	Uint8         layer;
	Uint8         priorityFlags;
	FIXED         velocityX;
	FIXED         velocityY;
	FIXED         velocityZ;
	FIXED         speedLimit;
	FIXED         acceleration;
	FIXED         targetX;
	FIXED         targetY;
	FIXED         targetZ;
	FIXED         centreX;     // 16.16 fixed-point pixels, in bounce
	FIXED         centreZ;     // 16.16 fixed-point pixels, in gravity
	FIXED         orbitRadius; // 16.16 fixed-point pixels, in sceneWord
	void         *sprite;
	Uint8         spriteStorageKind;
	Uint8         motionFlags;
	Uint8         arrivalAxis;
	Uint8         conditionResult;
	Uint8         snapToTarget;
	Uint8         collisionFlags;
	Uint8         steeringFlags;
	Uint8         paused;
	Uint8         retainSpriteCache;
	Uint8         repeatCount;
	Sint16        wait;
	Uint8         refused;
	Uint8         frozen;
	Uint8         phase;  // where the circle's growth starts, 1/64 turns
	Uint8         growth; // grows by 10 every other tick up to 141
	Sint16        angle;  // its place on the circle, 1/65536 turns
	Sint16        speed;  // its turn a tick, slowed by 128 a tick once grown
} ORBIT;

// Half a turn of an Orbit's phase, 64 to the turn: the sparks that rise round one who learns a
// Psynergy take every other phase short of it, one each 2 pixels up a column round him, so their
// circles widen and narrow again up the column.
#define ORBIT_HALF_TURN      32

// The sparks that rise from a Psynergy's tablet as it lifts, flashing.
#define TABLET_RISING_SPARKS 16

// The LEARN_SPARKS that circle the one who learns a Psynergy start LEARN_SPARK_INTERVAL ticks
// apart, 122 pixels out, and close 8 pixels a tick (syOrbiterSpiral) to 2 pixels in
// LEARN_SPIRAL_TICKS; each one's countdown ends with the last's.
#define LEARN_SPARKS         16
#define LEARN_SPARK_INTERVAL 8
#define LEARN_SPIRAL_TICKS   15

// How far a pillar sinks, its top then level with the floor: into the floor as Blaze at a torch
// lowers it in Magma Rock, and into its gap as Move drops it in Jupiter Lighthouse.
#define PILLAR_SINK_DEPTH    32

// How long such a particle lives before it starts again: a slow swell and the 12 ticks it stays
// gone after, or a quick one.
enum
{
	PARTICLE_SLOW_LIFETIME  = 212,
	PARTICLE_QUICK_LIFETIME = 116
};

// A glow sprite queued for the OAM: the link at its depth, attributes 0 and 1 as one word, and
// attribute 2's fields.
typedef struct LIGHT_OAM
{
	Uint32 link;
	Uint32 attr01;
	Uint16 tile     : 10;
	Uint16 priority : 2;
	Uint16 palette  : 4;
	Uint16 attr3; // no code names it
} LIGHT_OAM;

// One of the eight sprites the sand draws: where it starts, how the level moves it, and its OAM
// entry as syOAMAddSpriteAtDepthClamped takes it.
typedef struct ANKOHL_RUINS_SPR
{
	Sint32 x;
	Sint32 z;
	Sint32 y;
	Uint32 kind; // which way the level moves it
	Uint32 oam[3];
} ANKOHL_RUINS_SPR;

// Ankohl Ruins' sand (AnkohlRuinsSand): its sprites' VRAM cache entry, its level, map layer 1's
// offset before it rose, and its eight sprites.
typedef struct ANKOHL_RUINS_SPR_STATE
{
	// No code names it as the sand's; the entrance tests read through the type so that their load
	// stays behind a store to the sand.
	Sint16           unused;
	Sint16           tile;  // the VRAM cache entry of the sprites' tiles
	Sint32           level; // 16.16 pixels the sand has risen
	// Map layer 1's offset when the sand started, which the level is added to.
	FIXED            baseOffsetX;
	FIXED            baseOffsetY;
	ANKOHL_RUINS_SPR sprites[8];
} ANKOHL_RUINS_SPR_STATE;

// A block of metatiles the sand fills in Ankohl Ruins once its level reaches `level`: `width` by
// `height` metatiles from (`x`, `z`).
typedef struct ANKOHL_RUINS_SAND_ROW
{
	Uint8 level;
	Uint8 width;
	Uint8 height;
	Uint8 x;
	Uint8 z;
	Uint8 unused[3];
} ANKOHL_RUINS_SAND_ROW;

// The map state (MAP_STATE) with its ground marks as bit-fields, for rooms that set the kind of
// wading.
typedef struct MAP_BITS
{
	FIXED *origin;
	Sint32 shakeX, shakeY, shakeDecay;
	Uint8 *script;
	Uint16 dispcnt;
	Uint8  wideTiles;
	Uint8  footprints : 2;
	Uint8  wading     : 2; // the kind of wading; rooms with a second level set 1
} MAP_BITS;

// A script's message counter, read as a word or as its halfword (Gabomba Catacombs' centred lines).
typedef union SCRIPT_LINE
{
	Sint32 word;
	Uint16 line;
} SCRIPT_LINE;

// A metatile of Gabomba Statue's channel puzzle: its kind, whether it is blocked, its shape and
// what stands on it.
typedef struct PUZZLE_METATILE
{
	Uint8 kind;
	Uint8 blocked;
	Uint8 state;
	Uint8 occupied;
} PUZZLE_METATILE;

// The palette fade's state (heap slot paletteFade) as 256 colours.
typedef struct FADE_PALETTE
{
	Uint16 colors[256];
} FADE_PALETTE;

// A row of a scene's trap table, ended by an actor of 0xffff: an actor and its flag.
typedef struct TRAP_ENTRY
{
	Uint16 actor;
	Uint16 flags;
} TRAP_ENTRY;

// An actor's coordinate read as its word or its low halfword, through a union so that a store to it
// may alias a read after it.
typedef union ACTOR_COORDINATE
{
	Sint32 value;
	Uint16 half;
} ACTOR_COORDINATE;

// A coordinate as the scenes' actor views read it: FIELD_COORDINATE's layout as a union, whose
// members may alias anything.
typedef union VIEW_COORDINATE
{
	FIXED fixed;

	struct
	{
		Uint16 decimal;
		Sint16 integer;
	} part[1];
} VIEW_COORDINATE;

// A spark of the aerie's beam: where it is, how fast it drifts up and its age in ticks.
typedef struct BEAM_SPARK
{
	Sint32 pos[3];
	Sint32 unusedBeforeDrift; // no code names it
	Sint32 drift;             // taken off its height each tick
	Sint32 unusedAfterDrift;  // no code names it
	Uint32 age;
} BEAM_SPARK;

// Lemuria spring's toss (LemuriaSpringToss): the tick the coin is let go, its trail of positions
// and its shadow, its velocity, and the ticks the spring still draws it in.
typedef struct SPRING_TOSS_MOTION
{
	Uint8   unused[2];
	Sint16  launchFrame;
	VECTOR3 trail[4];
	VECTOR3 shadow;
	VECTOR3 velocity;
	Sint32  attractionFrame;
} SPRING_TOSS_MOTION;

// One of the spring's moving obstacles: its position and heading, and the timers its motion and its
// hits keep.
typedef struct SPRING_OBSTACLE
{
	VECTOR3 pos;
	Sint16  heading;
	Sint16  pauseOrPhase;
	Sint16  speedMode;
	Sint16  reactionFrame;
	Sint16  hitCooldownFrame;
} SPRING_OBSTACLE;

// A door of the Lemurian ship: the step cue it answers, its opening's metatile animation and the
// metatile it opens at.
typedef struct LEMURIAN_SHIP_WARP
{
	Uint16 exit;
	Sint32 value;
	Uint16 x;
	Uint16 z;
} LEMURIAN_SHIP_WARP;

// The sprite words as one stored pair, or the attributes field by field.
typedef union SPARK_OAM
{
	struct
	{
		Uint32 first;
		Uint32 second;
	} words;

	OAM_DATA oam;
} SPARK_OAM;

// A place on the Sea of Time's lap, in 16.16 pixels.
typedef struct SPOT
{
	Sint32 x;
	Sint32 z;
} SPOT;

// The six places of the Sea of Time's lap where spray rises once the lap has been run more than
// three times.
typedef struct SPOTS
{
	SPOT spot[6];
} SPOTS;

// The 14 colours of BG palette 3 (entries 1 to 14) that the fade returns to, copied from palette
// RAM by scInit and rotated by scSeaOfTimeAnimateSea in step with BG palette entries 49 to 62.
typedef struct SAVED_COL
{
	Uint16 colors[14];
} SAVED_COL;

// The field effect state as the Golden Rings read it: the VRAM cache entry their sprites share, in
// bytes FIELD_EFFECT_STATE leaves unnamed.
typedef struct LINKED_EFFECT_STATE
{
	Uint8  beforeVramBlock[1984];
	Uint16 vramBlock;
} LINKED_EFFECT_STATE;

// A spiky rock's metatile on Treasure Isle (scTreasureIsleRaiseSpikyRock).
typedef struct TREASURE_ISLE_METATILE
{
	Sint32 x;                // in 16-pixel metatiles
	Sint32 z;                // in 16-pixel metatiles
	Sint32 unusedAfterZ;     // no code names it; 12 onwards, one a rock
	Uint16 flags;            // set once the rock has risen (scTreasureIsleSetSpikyRock)
	Uint16 unusedAfterFlags; // no code names it
} TREASURE_ISLE_METATILE;

// A spark of Venus Lighthouse's entrance: its position and velocity in 16.16 pixels, and its tick.
typedef struct SPARK_STATE
{
	Sint32 x;
	Sint32 y;
	Sint32 z;
	Sint32 velocityX;
	Sint32 velocityY;
	Sint32 velocityZ;
	Sint32 frame;
} SPARK_STATE;

// The World Map's Echo (scWorldMapCircleEcho), which circles and bobs where it waits: ACTOR with
// its timer and userValue words named.
typedef struct BOBBING_MARKER
{
	const Sint32   *script;
	Sint16          scriptCursor;
	Uint16          facing;
	VIEW_COORDINATE x;
	VIEW_COORDINATE y;
	VIEW_COORDINATE z;
	FIXED           groundHeight;
	FIXED           scaleX;
	FIXED           scaleY;
	Uint16          radius;
	Uint8           layer;
	Uint8           priorityFlags;
	FIXED           velocityX;
	FIXED           velocityY;
	FIXED           velocityZ;
	FIXED           speedLimit;
	FIXED           acceleration;
	FIXED           targetX;
	FIXED           targetY;
	FIXED           targetZ;
	FIXED           bounce;
	FIXED           gravity;
	Sint32          sceneWord;
	void           *sprite;
	Uint8           spriteStorageKind;
	Uint8           motionFlags;
	Uint8           arrivalAxis;
	Uint8           conditionResult;
	Uint8           snapToTarget;
	Uint8           collisionFlags;
	Uint8           steeringFlags;
	Uint8           paused;
	Uint8           retainSpriteCache;
	Uint8           repeatCount;
	Sint16          wait;
	Uint8           refused;
	Uint8           frozen;
	Uint8           sceneCount;
	Uint8           sceneByte;
	Sint16          angle;   // its place on the circle, 1/65536 turns, in timer
	Sint16          stopped; // nonzero holds it still, in userValue
} BOBBING_MARKER;

// A field sprite (FIELD_SPR) read a byte at a time, as Yampi Desert's scorpion sets its colour mode
// and priority and loads its tiles.
typedef struct YAMPI_DESERT_VISUAL
{
	Uint8 link[4];
	Uint8 y;         // attribute 0's y
	Uint8 attr0High; // attribute 0's high byte: bit 5 is full colour
	Uint8 attr1[2];
	Uint8 tileLow;   // attribute 2's low byte
	// Attribute 2's high byte. bits: 7..4 palette | 3..2 priority | 1..0 tile bits 9..8
	Uint8 attr2High;
	Uint8 attr3[2];
	Uint8 scale[4];
	Uint8 vramBlock;     // VRAM cache entry holding its tiles
	Uint8 cacheFlags;
	Uint8 rotation[2];
	Uint8 size[4];       // width, height and the x and y offsets
	Uint8 lastAnimation; // the animation number last selected
	Uint8 dirty;         // nonzero redraws its pixels
	Uint8 flags;         // the sprite flags (SPR_FLAG)
	Uint8 layerCount;
} YAMPI_DESERT_VISUAL;

// Compressed tiles the scenes unpack into the VRAM cache (graphics/field/cached_a.png to
// cached_d.png).
extern Uint8        FieldCachedTilesA[];

// See FieldCachedTilesA (graphics/field/cached_d.png).
extern Uint8        FieldCachedTilesD[];

// Compressed ground tiles (graphics/field/ground_tiles.png).
extern const Uint8  FieldGroundTiles[];

// An explosion's 16 sprite colours (graphics/field/explosion_palette.png).
extern const Uint16 FieldExplosionPalette[];
// The sparks of the explosion that Loho's cannon blast and the Northern Reaches' shattered ice wall
// throw over the screen: how many, a spark's life in ticks (the explosion sprite sheet's eight
// pictures, four ticks each), the sprite palette their colours are loaded into
// (FieldExplosionPalette), and their depth slot among the field engine's sprites.
#define EXPLOSION_SPARK_COUNT   96
#define EXPLOSION_SPARK_TICKS   32
#define EXPLOSION_SPARK_PALETTE 15
#define EXPLOSION_SPARK_DEPTH   100

// Counts one more weird answer Felix gives (fdScriptCountAnswer, through the overlay's own import
// row).
void               scEasternAlhafraShipCountAnswer();

// A geyser's compressed tiles (graphics/field/geyser.png).
extern const Uint8 FieldGeyserTiles[];

// The geysers Scoop digs up in Gondowan Cliffs and in Yampi Desert's scorpion pit (each scene's
// GEYSER_WORK): the most sprites btGeyserDraw queues in a tick, and the geysers a room can have.
#define GEYSER_SPRITES       80
#define GEYSER_ENTRIES       16
// A geyser's six-second round, and the ticks of it at which it starts to rise and to fall.
#define GEYSER_PERIOD_TICKS  360
#define GEYSER_RISE_TICK     60
#define GEYSER_FALL_TICK     240
// The pixels a geyser rises to.
#define GEYSER_MAX_HEIGHT    64
// The terrain id and collision slot of the first geyser: the geysers take the last GEYSER_ENTRIES
// of the 256 terrain ids.
#define GEYSER_FIRST_TERRAIN (256 - GEYSER_ENTRIES)
// The geysers' state (each scene's GEYSER_WORK), kept in the inventory snapshot's spare space.
#define GEYSER_STATE         ((struct GEYSER_WORK *)(InventorySnapshot + INVENTORY_SNAPSHOT_SPARE))

// See FieldCachedTilesA (graphics/field/cached_b.png).
extern Uint8 FieldCachedTilesB[];

// See FieldCachedTilesA (graphics/field/cached_c.png).
extern Uint8 FieldCachedTilesC[];

// The metatile stampers of Air's Rock's and Magma Rock's insides keep the last metatile they
// stamped under the leader in one word, x in the high half and z in the low
// (scAirsRockInsideStampMetatileUnderLeader, scMagmaRockInsideStampStepMetatile): the word for
// metatile (x, z), and the x and z a word holds.
#define METATILE_KEY(x, z)  (((x) << 16) + (z))
#define METATILE_KEY_X(key) ((key) >> 16)
#define METATILE_KEY_Z(key) ((key) & 0xffff)

// The scene's cue of a kind for a target whose condition holds, or NULL (fdFindCue, through the
// overlay's own import row).
Sint32             sySceneCueFindActiveRecord(Sint32 mode, Sint32 value);

// The map screen's third set of compressed tiles (graphics/world_map/storyc.png).
extern const Uint8 WorldMapPackedTilesC[];

struct VECTOR3;
struct WINDOW;

// The lightning bolt's picture (RESOURCE_LIGHTNING_TILES and RESOURCE_LIGHTNING_MAP), shown on BG0
// for the strike: its tiles at the start of character block 0, and its map, LIGHTNING_MAP_ROWS rows
// of LIGHTNING_MAP_COLUMNS numbered from tile 512, on screen block LIGHTNING_SCREEN_BLOCK.
#define LIGHTNING_TILES        64
#define LIGHTNING_MAP_ROWS     16
#define LIGHTNING_MAP_COLUMNS  4
#define LIGHTNING_SCREEN_BLOCK 4

// Plays an actor's animation and lets ticks pass before the script goes on.
#define fdActorAnimateThenWait(id, animation, ticks)                                               \
	do                                                                                             \
	{                                                                                              \
		fdActorSetAnimation(id, animation);                                                        \
		syFieldScriptWait(ticks);                                                                  \
	} while (0)

// MapsWork holds Mars Lighthouse's nineteen maps twice, from its outside to its Venus ledge:
// frozen, then again in the colours the lighthouse takes once it is thawed
// (FLAG_MARS_LIGHTHOUSE_THAWED). A frozen map's thawed copy is MARS_LIGHTHOUSE_THAWED_MAPS rows on,
// the distance the linker gives between the two first rows (field/map_resources.s).
extern char MapIndexMarsLighthouseOutside;
extern char MapIndexMarsLighthouseOutsideThawed;
#define MARS_LIGHTHOUSE_THAWED_MAPS                                                                \
	((Sint32)&MapIndexMarsLighthouseOutsideThawed - (Sint32)&MapIndexMarsLighthouseOutside)

// The Lemurian Ship's background palette, the blues of the band along its hull and the surf, in the
// scenes that show the ship: Atteka Inlet, Lemuria's dock and the ship's deck.
#define LEMURIAN_SHIP_PALETTE 11
// The runs of a background palette's colours the shore scenes turn a step at a time: 12 to 15, the
// surf, and on LEMURIAN_SHIP_PALETTE also 4 to 9, the blues of the band along its hull.
#define HULL_BAND_FIRST_COLOR 4
#define HULL_BAND_LAST_COLOR  9
#define SURF_FIRST_COLOR      12
#define SURF_LAST_COLOR       15
// The same run of four as Gondowan Cliffs (background palette 0) and SW Atteka Islet (palette 3)
// turn their water's colours in place (scGondowanCliffsCycleWaterColors,
// scSettlementsCycleWaterColors): its first colour and how many.
#define FIRST_WATER_COLOR     12
#define WATER_COLORS          4

// The ticks a puff of smoke from a chimney or a cooking fire rises and swells before it is
// released: Prox's, Kalt Island's (the Settlements), Shaman Village's and Kibombo's, each scene's
// own copy of the same drift.
#define SMOKE_TICKS           60

// The raised cue a whirlwind raises as it catches the leader, which each scene with whirlwinds
// answers by carrying him off on it: the Jupiter Lighthouse rooms with fdWhirlwindCarryLeader
// (shared/whirlwind_update.h raises it), Air's Rock with scAirsRockRideWhirlwind and its inside
// with scAirsRockInsideRideWhirlwind.
#define RAISED_RIDE_WHIRLWIND 200

// The metatile behavior of a cracked tile in the Jupiter Lighthouse rooms with cracked floors (the
// blue key's, the cracked floor's and the Phaeton's Blade chest's), the kind scCrackedTileFind
// looks for; a tile takes the next one up as it cracks.
enum
{
	METATILE_BEHAVIOR_CRACKED_TILE = 33
};

// The fall through such a tile to the floor below, on the room's own map: the exit it takes and the
// entrance it lands at share the number.
enum
{
	THROUGH_CRACKED_TILE = 13
};

// How near, in pixels, the two walking children of Lemuria and of Contigo heed: the other child
// they face, the leader they face, and the leader come so near that they walk on
// (scLemuriaNoticeLeader, scContigoPauseWhenFaced).
enum
{
	CHILD_SEES_CHILD_RANGE  = 18,
	CHILD_SEES_LEADER_RANGE = 26,
	CHILD_WALKS_ON_RANGE    = 11
};

// The vents of Aqua Rock and Taopo Swamp (VENT_WORK, shared/vent_*.h): the most vents a room has,
// and the most actors besides the leader their puffs land on.
#define VENT_MAX     16
#define VENT_IDS_MAX 4

// One vent: whether it blows, the ticks to its next puff, its position and its gust's speed,
// in 16.16 pixels a tick.
typedef struct VENT_ENTRY
{
	Sint16 active;
	Sint16 timer;
	FIXED  x;
	FIXED  y;
	FIXED  z;
	FIXED  value;
} VENT_ENTRY;

// The vents of the room, in the inventory snapshot's spare space (VENT_STATE): their tick counter,
// the room's hook run each tick, its vents and the actors their puffs land on, besides the leader.
typedef struct VENT_WORK
{
	Sint32     count;
	Sint32     (*hook)(void);
	VENT_ENTRY entries[VENT_MAX];
	Uint16     ids[VENT_IDS_MAX];
} VENT_WORK;

// The vents' state, kept in the inventory snapshot's spare space.
#define VENT_STATE            ((VENT_WORK *)(InventorySnapshot + INVENTORY_SNAPSHOT_SPARE))
// How far apart a and b are, never negative, as a vent's thrown object tests whether it rests on
// another in x and z (field/actor_vent_bounce.c). Each is read twice.
#define DIFF(a, b)            ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))
// The names field/actor_vent_bounce.c's two functions take in both rooms' copies: whether a thrown
// object rests on another, and its motion.
#define VENT_TOUCHING         VentIsTouching
#define VENT_BOUNCE           scVentBounce
// Vent index's entry: `state` moved on by index entries and read as the first, as the vents'
// accessors (scVentSetBlowing, scVentSetGust) read it.
#define VENT_AT(state, index) ((VENT_WORK *)((Uint8 *)(state) + (index) * sizeof(VENT_ENTRY)))

#endif
