/* @file */
/* Aqua Rock's inner rooms, inside the rock on the island south of the Apojii */
/* Islands, which Felix enters from the rock's outer slopes. The file holds ten
 */
/* rooms, most of them several screens, and their water puzzles. A grey Moai */
/* that Douse fills pours water and floods its room, lifting the yellow */
/* stepping blocks; a stone pillar Moved before a Moai's mouth stops it and the
 */
/* water sinks. Vents blow Felix off their ledge until Move sets a pillar in */
/* front of them. A Water Stone, the big blue drop, sends a stream along the */
/* blocks when Douse is cast on it. There are puddles to Frost, a tightrope, a
 */
/* Psynergy Stone, and in the Vial chest's room a waterfall that carries Felix
 */
/* back up to the slopes. Setting the Aquarius Stone in the empty column of the
 */
/* great pool's room opens a causeway and a gold door in the dais at its heart,
 */
/* which lead to the tablet where Piers learns Parch */
/* (scAquaRockInsideReadParchTablet). With Parch, Felix can dry the pool by the
 */
/* sunken Rusty Sword chest and stop the Moai whose stream hides Steam, the */
/* Mercury Djinni (scAquaRockInsideFightSteam). */

#include "macros.h"
#include "window/message_argument.h"
#include "item_ids.h"
#include "field/field_sequence.h"
#include "field/actor_script.h"
#include "scene/scene.h"
#include "graphics/palette_buffers.h"

/* A water pillar or spout drawn over its cell: its place, height and resting */
/* height, kind, state (1 a spout, 2 a pillar on an actor), the actor's */
/* position it follows, and its top's and column's sprite attributes. */
typedef struct PILLAR_ENTRY {
	s32 x;
	s32 height;
	s32 z;
	s32 base;
	u16 kind;
	u16 state;
	s32 *pos;
	u32 top[3];
	u32 column[3];
} PILLAR_ENTRY;

/* The most water pillars and spouts a room has. */
#define PILLAR_ENTRIES 32

/* The kinds of spout, each a run of collision codes from the first a room's */
/* spouts take, and of pillar top: AquaRockInside_PillarTiles holds a 16x16 */
/* top for each, then the 16x8 pieces of column under them, one for each step */
/* of height up to PILLAR_COLUMN_PIECES. */
#define SPOUT_KINDS			 8
#define PILLAR_COLUMN_PIECES 8
#define PILLAR_TOP_TILES	 SPRITE_TILES_8BPP(16, 16)
#define PILLAR_COLUMN_TILES	 SPRITE_TILES_8BPP(16, 8)

/* The depth the water pillars and the streams' splashes take past their screen
 */
/* row, behind an actor and the scene's other sprites (SCENE_SPRITE_DEPTH) at */
/* the same spot. */
#define WATER_SPRITE_DEPTH 50

/* The water pillars and spouts scAquaRockInsideDrawPillars draws, and their */
/* VRAM entry. */
typedef struct PILLAR_STATE {
	u16 vramEntry;
	u16 cnt;
	PILLAR_ENTRY entries[PILLAR_ENTRIES];
} PILLAR_STATE;

/* The head of one of the two streams a Water Stone sends: its place and the */
/* way it runs. */
typedef struct BEAM_ENTRY {
	s32 x;
	s32 y;
	s32 z;
	u16 unusedAfterZ; /* no code names it */
	u16 dir;
} BEAM_ENTRY;

/* A splash the streams leave on an open cell, and its sprite's attributes. */
typedef struct SPLASH {
	s32 x;
	s32 y;
	s32 z;
	u16 active;
	u16 dir;
	u32 spr[3];
} SPLASH;

/* The most splashes the streams leave at once. */
#define SPLASH_ENTRIES 32

/* The streams and their splashes, which scAquaRockInsideDrawSplashes draws. */
typedef struct BEAM_STATE {
	u16 vramEntry;
	u16 cnt;
	BEAM_ENTRY entries[2];
	SPLASH splashes[SPLASH_ENTRIES];
} BEAM_STATE;

void fdActorSpawnWadingSplash(ACTOR *actor);
s32 fdActorEmitMist(ACTOR *actor);

void scAquaRockInsideDrainStream(void);
s32 scAquaRockInsideBlockCellUnder(s32 id, s32 value);
void scAquaRockInsideSetPillarCellJumpableInWater(void);
void scAquaRockInsideAddMoaiWater(s32 id);
void scAquaRockInsideDrawPillars(void);
void scAquaRockInsideSetPillarsHeight(s32 value);
void scAquaRockInsideDrawSplashes(void);
void scAquaRockInsideStartSplashes(void);
s32 scAquaRockInsideAddSplash(s32 x, s32 y, s32 z, s32 kind);
s32 scAquaRockInsideAdvanceStreams(s32 first, s32 target);
/* The motion of a thing a vent throws out (field/actor_vent_bounce.c). */
void scVentBounce(ACTOR *actor);
void fdActorStartScripted(ACTOR *actor, s32 priority);
void scVentPuff(s32 *position, s32 base);
s32 scVentSetBlowing(s32 index, s32 blowing);
s32 scVentGetCounter(void);
s32 scVentAdvanceCounter(s32 limit);

extern s32 AquaRockInside_SpringLevel;
PILLAR_STATE AquaRockInside_Pillars BSS_DATA;
BEAM_STATE AquaRockInside_Splashes BSS_DATA;
extern u32 OrbitSpark_Script[];
extern u32 AquaRockInside_SplashScript[];
extern u32 WadingSplash_Script[];
extern u32 RippleScript[];
/* The water pillars' tiles, compressed (graphics/aqua_rock_inside/tiles.s). */
extern const u8 AquaRockInside_PillarTiles[];
/* The splashes' tiles, compressed. */
extern const u8 AquaRockInside_SplashTiles[];
extern u32 Dust_BurstScript[];

#include "field/map.h"
#include "scene/ground_effects.h"
#include "game/game.h"
#include "version.h"
#include "field/event_inline.h"
#include "field/call.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct ACTOR;

void scAquaRockInsideWaitToLand(ACTOR *actor);

void fdLeaderRiseFromWater(void);
void scAquaRockInsideStartPillars(void);
void scAquaRockInsideAddSpouts(u32 first, s32 level, s32 baseLevel);
void scAquaRockInsideShowAquariusStone(void);
void scAquaRockInsideAddPillar(s32 id, s32 kind, s32 height);

#include "text/msg_ids.h"

/* How far the water rises or falls when a Moai fills, in pixels. */
#define WATER_LEVEL_STEP 32

/* Aqua Rock's lasting flags. The Water of Life chest's room's west and east */
/* stone pillars, each Moved into its gap (scAquaRockInsideSetPillarInGap). */
#define FLAG_AQUA_ROCK_WEST_PILLAR_SET 0x95a
#define FLAG_AQUA_ROCK_EAST_PILLAR_SET 0x95b
/* The stone pillar of Steam's room, pushed off its ledge to the floor below. */
#define FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED 0x95c
/* The Aquarius Stone set in the column of the great pool's room. */
#define FLAG_AQUA_ROCK_AQUARIUS_STONE_SET 0x95d
/* Douse on the Water Stones: the streams they turn, as each room's */
/* scAquaRockInsideAdvanceStreams leaves them. */
#define FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED	 0xa64
#define FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED		 0xa65
#define FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED 0xa66
#define FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED 0xa67
/* The pool of the Rusty Sword's room, dried by Parch. */
#define FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED 0xa6a
/* Set once Piers has read the tablet and learned Parch. */
#define FLAG_PIERS_LEARNED_PARCH 0x9ae
/* This visit's flags, room by room. Douse on the Water of Life chest's room's
 */
/* east and west Moai: each has filled, and the water has risen. */
#define FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED FLAG_SCENE_TEMP_30
#define FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED FLAG_SCENE_TEMP_31
/* Frost on the Water of Life chest's room's puddle. */
#define FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN FLAG_SCENE_TEMP_50
/* Parch on the Moai of Steam's room: its stream has drained away. */
#define FLAG_STEAM_ROOM_STREAM_DRAINED FLAG_SCENE_TEMP_40
/* Douse on the Rusty Sword chest's room's Moai, and then on its Water Stone. */
#define FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED		  FLAG_SCENE_TEMP_49
#define FLAG_RUSTY_SWORD_CHEST_WATER_STONE_DOUSED FLAG_SCENE_TEMP_48
/* Move on the pillar before the Vial chest's room's Moai, which stops its */
/* water. */
#define FLAG_VIAL_CHEST_MOAI_BLOCKED FLAG_SCENE_TEMP_38
/* Frost on each of the Vial chest's room's three puddles. */
#define FLAG_VIAL_CHEST_PUDDLE_A_FROZEN FLAG_SCENE_TEMP_51
#define FLAG_VIAL_CHEST_PUDDLE_B_FROZEN FLAG_SCENE_TEMP_52
#define FLAG_VIAL_CHEST_PUDDLE_C_FROZEN FLAG_SCENE_TEMP_53
/* Douse on the Tear Stone's room's Moai, and then on its Water Stone. */
#define FLAG_TEAR_STONE_MOAI_FILLED		   FLAG_SCENE_TEMP_4B
#define FLAG_TEAR_STONE_WATER_STONE_DOUSED FLAG_SCENE_TEMP_4A
/* The Parch tablet's room's water has sunk and left its last pool, until */
/* Parch dries it. */
#define FLAG_PARCH_TABLET_POOL_LEFT FLAG_AREA_TEMP_49
/* The cell of the mouth of the Moai of Steam's room, cell (48, 15): Parch on */
/* it, then raised to drain the stream. */
#define TRIGGER_STEAM_ROOM_MOAI 88
/* The cells in front of the west and east vents of the Crystal Powder and */
/* Vial chests' rooms, where the gust blows Felix off the ledge */
/* (scAquaRockInsideStepIntoVentGust); a stone pillar Moved in front of a vent
 */
/* shields them. */
#define TRIGGER_WEST_VENT_GUST 77
#define TRIGGER_EAST_VENT_GUST 78

/* The ways between the rooms each of these maps holds, named by the room a */
/* way leaves and the room it leads to, each room by where it lies on its map;
 */
/* a way's exit and the entrance at its foot share its number, so the way back
 */
/* is the entrance the exit leads to. */
typedef enum AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_WAY {
	WAY_WATER_OF_LIFE_CHEST_NORTHWEST_TO_NORTHEAST = 2,
	WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_NORTHWEST,
	WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_WEST,
	WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_WEST,
	WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_EAST,
	WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_EAST,
	WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTH,
	WAY_WATER_OF_LIFE_CHEST_SOUTH_TO_NORTHEAST
} AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_WAY;
typedef enum AQUA_ROCK_INSIDE_STEAM_WAY {
	WAY_STEAM_NORTHWEST_TO_EAST = 2,
	WAY_STEAM_EAST_TO_NORTHWEST,
	WAY_STEAM_EAST_TO_SOUTHWEST = 7,
	WAY_STEAM_SOUTHWEST_TO_EAST
} AQUA_ROCK_INSIDE_STEAM_WAY;
typedef enum AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_WAY {
	WAY_RUSTY_SWORD_CHEST_SOUTHWEST_TO_SOUTHEAST = 7,
	WAY_RUSTY_SWORD_CHEST_SOUTHEAST_TO_SOUTHWEST
} AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_WAY;
typedef enum AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_WAY {
	WAY_CRYSTAL_POWDER_CHEST_EAST_TO_WEST = 4,
	WAY_CRYSTAL_POWDER_CHEST_WEST_TO_EAST
} AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_WAY;
/* The Vial chest's room: its lower floor in the south-east, the vents' room */
/* south-west of it and the upper passages north-east, whose doorways at their
 */
/* north end the party comes out of after leaving the lower floor by its south
 */
/* edge. */
typedef enum AQUA_ROCK_INSIDE_VIAL_CHEST_WAY {
	WAY_VIAL_CHEST_SOUTHEAST_TO_SOUTHWEST_A = 4,
	WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_A,
	WAY_VIAL_CHEST_SOUTHEAST_TO_SOUTHWEST_B,
	WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_B,
	WAY_VIAL_CHEST_SOUTHEAST_TO_NORTHEAST_WEST,
	ENTRANCE_VIAL_CHEST_WEST_DOORWAY,
	WAY_VIAL_CHEST_SOUTHEAST_TO_NORTHEAST_EAST,
	ENTRANCE_VIAL_CHEST_EAST_DOORWAY
} AQUA_ROCK_INSIDE_VIAL_CHEST_WAY;

/* The cells the rooms' step, examine and Parch triggers answer, by their */
/* collision code. */
typedef enum AQUA_ROCK_INSIDE_TRIGGER_CELL {
	/* A gap's edge Felix hops across the water from */
	/* (scAquaRockInsideJumpForward): */
	/* the Water of Life chest's room's two, walked into north or west and */
	/* south, the Rusty Sword chest's room's and the Crystal Powder chest's */
	/* room's. */
	TRIGGER_WATER_OF_LIFE_CHEST_GAP = 50,
	TRIGGER_WATER_OF_LIFE_CHEST_SOUTH_GAP = 51,
	TRIGGER_RUSTY_SWORD_CHEST_GAP = 55,
	TRIGGER_CRYSTAL_POWDER_CHEST_GAP = 70,
	/* A stream that carries Felix down it (fdLeaderFlowWithStream). */
	TRIGGER_STREAM = 40,
	/* The pools Parch dries: round the Rusty Sword chest, and on the Parch */
	/* tablet's steps. */
	TRIGGER_RUSTY_SWORD_CHEST_POOL = 50,
	TRIGGER_PARCH_TABLET_POOL = 40,
	/* The Psynergy Stone, and the Parch tablet, examined. */
	TRIGGER_PSYNERGY_STONE = 30,
	TRIGGER_PARCH_TABLET = 30
} AQUA_ROCK_INSIDE_TRIGGER_CELL;

/* The collision codes the rooms' water reads (MAP_CELL.collisionCode). */
typedef enum AQUA_ROCK_INSIDE_COLLISION_CODE {
	/* The first of the eight codes of the bends that turn a stream */
	/* (AquaRockInside_StreamTurns' columns); their cells raise spouts too */
	/* (scAquaRockInsideAddSpouts). */
	COLLISION_STREAM_BEND = 33,
	/* The first of the eight codes of a room's second set of spouts. */
	COLLISION_SECOND_SPOUTS = 40,
	/* A stream's goal: scAquaRockInsideAdvanceStreams counts the streams that
	 */
	/* reach it. */
	COLLISION_STREAM_GOAL = 55,
	/* An open cell: a stream runs on across it and leaves a splash. */
	COLLISION_STREAM_OPEN = 255
} AQUA_ROCK_INSIDE_COLLISION_CODE;

/* In AquaRockInside_StreamTurns, a bend a stream cannot run through: it */
/* stops there. */
#define STREAM_STOP 8

/* The actors of the inner rooms the set-up places, numbering each room's */
/* placement table, named by the Psynergy their triggers answer: a Moai */
/* (SPRITE_MOAI) that Douse fills, so that the water rises, or a */
/* Water Stone (SPRITE_WATER_DROP_LARGE, a tear-shaped rock) that Douse sets */
/* spouting, which turns the streams; a stone pillar (SPRITE_STONE_PILLAR) */
/* Moved in front of a vent or a Moai or into a gap, and the water pillars */
/* topped by yellow blocks (SPRITE_YELLOW_BLOCK) scAquaRockInsideAddPillar */
/* raises. */
typedef enum AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_ACTOR_ID {
	ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI = ACTOR_FIRST_PLACED,
	ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI,
	ACTOR_WATER_OF_LIFE_CHEST_WEST_PILLAR,
	ACTOR_WATER_OF_LIFE_CHEST_EAST_PILLAR,
	/* The small blue rock the water lifts (scAquaRockInsideSetPillarHeight). */
	ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK,
	ACTOR_WATER_OF_LIFE_CHEST_PUDDLE
} AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_ACTOR_ID;

/* Steam's room (AquaRockInside_SteamPlacements). */
typedef enum AQUA_ROCK_INSIDE_STEAM_ACTOR_ID {
	ACTOR_STEAM_ROOM_MOAI = ACTOR_FIRST_PLACED,
	/* The pillar pushed off its ledge (scAquaRockInsidePushPillarOffLedge). */
	ACTOR_STEAM_ROOM_PILLAR,
	/* The Mercury Djinni Steam (SPRITE_MERCURY_DJINNI). */
	ACTOR_STEAM
} AQUA_ROCK_INSIDE_STEAM_ACTOR_ID;

/* The Aquarius Stone's room (AquaRockInside_AquariusStonePlacements). */
typedef enum AQUA_ROCK_INSIDE_AQUARIUS_STONE_ACTOR_ID {
	ACTOR_AQUARIUS_STONE_WATER_STONE = ACTOR_FIRST_PLACED,
	/* Two hidden actors where the stone appears; the second shows it */
	/* (scAquaRockInsideShowAquariusStone). */
	ACTOR_AQUARIUS_STONE_BASE,
	ACTOR_AQUARIUS_STONE
} AQUA_ROCK_INSIDE_AQUARIUS_STONE_ACTOR_ID;

/* The Rusty Sword chest's room (AquaRockInside_RustySwordChestPlacements). */
typedef enum AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_ACTOR_ID {
	ACTOR_RUSTY_SWORD_CHEST_WATER_STONE = ACTOR_FIRST_PLACED,
	ACTOR_RUSTY_SWORD_CHEST_MOAI,
	/* The two Moai of the upper ledge. */
	ACTOR_RUSTY_SWORD_CHEST_MOAI_A,
	ACTOR_RUSTY_SWORD_CHEST_MOAI_B,
	ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A,
	ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_B,
	ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C,
	/* The room's chest, the first actor its map's chests make. */
	ACTOR_RUSTY_SWORD_CHEST = 64
} AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_ACTOR_ID;

/* The Crystal Powder chest's room */
/* (AquaRockInside_CrystalPowderChestPlacements). */
typedef enum AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_ACTOR_ID {
	ACTOR_CRYSTAL_POWDER_CHEST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_CRYSTAL_POWDER_CHEST_WATER_STONE
} AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_ACTOR_ID;

/* The Vial chest's room (AquaRockInside_VialChestPlacements). */
typedef enum AQUA_ROCK_INSIDE_VIAL_CHEST_ACTOR_ID {
	/* The pillar Moved before the Moai (scAquaRockInsideBlockMoaiWithPillar).
	 */
	ACTOR_VIAL_CHEST_MOAI_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_VIAL_CHEST_MOAI,
	/* The two pillars beside the vents (scAquaRockInsideStepIntoVentGust). */
	ACTOR_VIAL_CHEST_WEST_VENT_PILLAR,
	ACTOR_VIAL_CHEST_EAST_VENT_PILLAR,
	/* A hidden actor that splashes at the foot of the waterfall */
	/* (scAquaRockInsideSplashBeneath). */
	ACTOR_VIAL_CHEST_SPLASH,
	/* The three puddles Frost freezes (scAquaRockInsideFreezeRoomPuddle). */
	ACTOR_VIAL_CHEST_PUDDLE_A,
	ACTOR_VIAL_CHEST_PUDDLE_B,
	ACTOR_VIAL_CHEST_PUDDLE_C
} AQUA_ROCK_INSIDE_VIAL_CHEST_ACTOR_ID;

/* The tightrope's room (AquaRockInside_TightropePlacements). */
typedef enum AQUA_ROCK_INSIDE_TIGHTROPE_ACTOR_ID {
	ACTOR_TIGHTROPE_WEST_POST = ACTOR_FIRST_PLACED,
	ACTOR_TIGHTROPE_EAST_POST
} AQUA_ROCK_INSIDE_TIGHTROPE_ACTOR_ID;

/* The Tear Stone's room (AquaRockInside_TearStonePlacements). */
typedef enum AQUA_ROCK_INSIDE_TEAR_STONE_ACTOR_ID {
	ACTOR_TEAR_STONE_WATER_STONE = ACTOR_FIRST_PLACED,
	ACTOR_TEAR_STONE_MOAI,
	/* The nine water pillars of the puzzle, lettered A to I. */
	ACTOR_TEAR_STONE_WATER_PILLAR_A,
	ACTOR_TEAR_STONE_WATER_PILLAR_B,
	ACTOR_TEAR_STONE_WATER_PILLAR_C,
	ACTOR_TEAR_STONE_WATER_PILLAR_D,
	ACTOR_TEAR_STONE_WATER_PILLAR_E,
	ACTOR_TEAR_STONE_WATER_PILLAR_F,
	ACTOR_TEAR_STONE_WATER_PILLAR_G,
	ACTOR_TEAR_STONE_WATER_PILLAR_H,
	ACTOR_TEAR_STONE_WATER_PILLAR_I,
	/* The stone pillars Moved into the puzzle */
	/* (scAquaRockInsideMovePuzzlePillar). */
	ACTOR_TEAR_STONE_PILLAR_A,
	ACTOR_TEAR_STONE_PILLAR_B
} AQUA_ROCK_INSIDE_TEAR_STONE_ACTOR_ID;

/* The Parch tablet's room (AquaRockInside_ParchTabletPlacements). */
typedef enum AQUA_ROCK_INSIDE_PARCH_TABLET_ACTOR_ID {
	/* The six water pillars. */
	ACTOR_PARCH_TABLET_FIRST_WATER_PILLAR = ACTOR_FIRST_PLACED,
	/* The tablet Piers reads to learn Parch. */
	ACTOR_PARCH_TABLET = ACTOR_PARCH_TABLET_FIRST_WATER_PILLAR + 6
} AQUA_ROCK_INSIDE_PARCH_TABLET_ACTOR_ID;

/* Talking to Steam, the Mercury Djinni standing at the head of the stream */
/* before the Moai of Steam's room, starts a battle with it. Felix reaches it */
/* once Parch has dried the stream (scAquaRockInsideSteamRoomStreamDrains). */
void scAquaRockInsideFightSteam(void) {
	fdRequestDjinnBattle(ACTOR_STEAM, DJINNI_STEAM);
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE AquaRockInside_Placements[];
extern ACTOR_PLACE AquaRockInside_WaterOfLifeChestPlacements[];
extern ACTOR_PLACE AquaRockInside_SteamPlacements[];
extern ACTOR_PLACE AquaRockInside_AquariusStonePlacements[];
extern ACTOR_PLACE AquaRockInside_RustySwordChestPlacements[];
extern ACTOR_PLACE AquaRockInside_CrystalPowderChestPlacements[];
extern ACTOR_PLACE AquaRockInside_VialChestPlacements[];
extern ACTOR_PLACE AquaRockInside_PsynergyStonePlacements[];
extern ACTOR_PLACE AquaRockInside_TightropePlacements[];
extern ACTOR_PLACE AquaRockInside_TearStonePlacements[];
extern ACTOR_PLACE AquaRockInside_ParchTabletPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST)
		return AquaRockInside_WaterOfLifeChestPlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_STEAM)
		return AquaRockInside_SteamPlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE)
		return AquaRockInside_AquariusStonePlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST)
		return AquaRockInside_RustySwordChestPlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST)
		return AquaRockInside_CrystalPowderChestPlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST)
		return AquaRockInside_VialChestPlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE)
		return AquaRockInside_PsynergyStonePlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_TIGHTROPE)
		return AquaRockInside_TightropePlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE)
		return AquaRockInside_TearStonePlacements;
	if(scene == SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET)
		return AquaRockInside_ParchTabletPlacements;
	return AquaRockInside_Placements;
}

/* Moving or pushing one of the two stone pillars of the Water of Life chest's
 */
/* room into its gap: the west pillar north along the dotted track to the round
 */
/* hole at its end (cell (9, 12)), the east one two squares east into its gap */
/* among the yellow stepping blocks (column 51). There it settles a pixel lower
 */
/* and stays for good, opening a shortcut across the room; with the */
/* west one in place, its square and cell (13, 12) become places Felix hops */
/* across (MAP_CELL_JUMP). */
void scAquaRockInsideSetPillarInGap(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(id == ACTOR_WATER_OF_LIFE_CHEST_WEST_PILLAR) {
		s32 row = FIELD_CELL(actor->z.fixed);

		if(row == 12) {
			actor->motionFlg = 0;
			{
				s32 step = -toFIXED(1);

				actor->y.fixed += step;
			}
			fdEventSetFlag(FLAG_AQUA_ROCK_WEST_PILLAR_SET);
			fdMapGridCopyCellAttributes(0, 12, 1, 1, 9, row);
			fdMapGridCopyCellAttributes(0, 12, 1, 1, 13, row);
		}
	}
	else if(id == ACTOR_WATER_OF_LIFE_CHEST_EAST_PILLAR) {
		if(FIELD_CELL(actor->x.fixed) == 51) {
			actor->motionFlg = 0;
			{
				FIXED step = -toFIXED(1);

				actor->y.fixed += step;
			}
			fdEventSetFlag(FLAG_AQUA_ROCK_EAST_PILLAR_SET);
		}
	}
}

/* Walking forward at the edge of a gap in the water hops Felix across onto */
/* the yellow block beyond: at the foot of the vents' ledge in the Crystal */
/* Powder chest's room (cells (17, 24) and (19, 24)), at the east stone */
/* pillar's squares in the Water of Life chest's room (cells (49, 13) and */
/* (50, 13)) and at two spots of the Rusty Sword chest's room (cells (12, 44) */
/* and (20, 44)). The field engine's fdTryJumpForward finds the block to */
/* land on. */
void scAquaRockInsideJumpForward(void) { fdTryJumpForward(); }

/* Douse on the grey east Moai of the Water of Life chest's room: it turns */
/* green and pours water from its mouth, the room floods, and the yellow */
/* stepping blocks and the small blue rock rise with the water. */
void scAquaRockInsideDouseWaterOfLifeChestEastMoai(s32 unused, s32 id) {
	MAP_LAYER *layer =
		&((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
	s32 i;
	s32 column;
	s32 moai = id;

	scFieldActorSetSprCol(fdActorGet(moai), SPRITE_COLOR_NORMAL);
	scAquaRockInsideAddMoaiWater(moai);
	fdActorSetAnimation(moai, 3);
	layer->ofsY -= toFIXED(WATER_LEVEL_STEP);
	fdMapLoadCellsAndShapes();
	fdMapGridCenterOnFollowedPoint();
	column =
		FIELD_CELL(fdActorGet(ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK)->x.fixed) -
		42;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_RISE_WITH_SPARKS_HIGH);
	/* scAquaRockInsideSetPillarHeight, defined below, is called with no
	 * prototype */
	/* in scope. */
	for(i = 0; i <= WATER_LEVEL_STEP - 1; i++) {
		scAquaRockInsideSetPillarsHeight(INTtoFIXED(i));
		scAquaRockInsideSetPillarHeight(column, 0);
		layer->ofsY += toFIXED(1);
		syWait(4);
	}
	if(fdEventGetFlag(FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN))
		fdMapGridCopyCellAttributes(41, 42, 1, 1, 43, 44);
	fdEventEnd();
	fdEventSetFlag(FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED);
}

/* Douse on the grey west Moai of the Water of Life chest's room: it pours */
/* water from its mouth, the room floods and the yellow stepping blocks rise. */
void scAquaRockInsideDouseWaterOfLifeChestWestMoai(s32 unused, s32 id) {
	MAP_LAYER *layer =
		&((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
	s32 i;

	scFieldActorSetSprCol(fdActorGet(id), SPRITE_COLOR_NORMAL);
	scAquaRockInsideAddMoaiWater(id);
	fdActorSetAnimation(id, 3);
	layer->ofsY -= toFIXED(WATER_LEVEL_STEP);
	fdMapLoadCellsAndShapes();
	fdMapGridCenterOnFollowedPoint();
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_RISE_WITH_SPARKS_HIGH);
	for(i = 0; i <= WATER_LEVEL_STEP - 1; i++) {
		scAquaRockInsideSetPillarsHeight(INTtoFIXED(i));
		layer->ofsY += toFIXED(1);
		syWait(4);
	}
	fdEventEnd();
	fdEventSetFlag(FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED);
}

/* Nothing more happens when Felix uses Move on the small blue rock of the */
/* Water of Life chest's room: Move slides it as it slides a pillar, and the */
/* trigger's function adds nothing. */
void scAquaRockInsideDoNothing(void) {}

/* Frost on the puddle of the Water of Life chest's room, at cell (43, 44), */
/* freezes it, and Felix can hop across it (MAP_CELL_JUMP): one way through */
/* the room's puzzle. */
void scAquaRockInsideFreezePuddle(void) {
	fdMapGridCopyCellAttributes(41, 42, 1, 1, 43, 44);
	fdEventSetFlag(FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN);
}

#if TLA_PATCH_1
/* From patch 1: lets the actor fall and waits until it lands with a thump. */
void scAquaRockInsideWaitToLand(ACTOR *actor) {
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	do {
		syWait(1);
	} while(actor->velY != 0);
	fdEventPlaySnd(SE_THUMP);
	syWait(10);
	actor->motionFlg = 0;
}
#endif

/* Moving or pushing the stone pillar of Steam's room east off the edge of its
 */
/* ledge (column 39) drops it to the floor below, onto the round mark there; */
/* in patch 1 it lands with a thump. Its square becomes one Felix hops across */
/* (MAP_CELL_JUMP). */
void scAquaRockInsidePushPillarOffLedge(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);
	s32 x;

	if(id == ACTOR_STEAM_ROOM_PILLAR) {
		x = FIELD_CELL(actor->x.fixed);
		if(x == 39) {
#if !TLA_PATCH_1
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			syWait(15);
			actor->motionFlg = 0;
			fdEventSetFlag(FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED);
			fdMapGridCopyCellAttributes(41, 9, 1, 1, x, id);
#else
			fdEventStart();
			syFieldEventEndLastingPsynergy(0);
			scAquaRockInsideWaitToLand(actor);
			fdEventSetFlag(FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED);
			fdMapGridCopyCellAttributes(41, 9, 1, 1, x, id);
			fdEventEnd();
#endif
		}
	}
}

/* Parch on the Moai of Steam's room, at its mouth (cell (48, 15)): the Moai */
/* turns grey and stops pouring, and the stream it fed starts to drain */
/* (scAquaRockInsideSteamRoomStreamDrains, which raising the trigger runs). */
void scAquaRockInsideParchSteamRoomMoai(void) {
	fdActorSetAnimation(ACTOR_STEAM_ROOM_MOAI, 4);
	scFieldActorSetSprCol(fdActorGet(ACTOR_STEAM_ROOM_MOAI), SPRITE_COLOR_GREY);
	fdMapGridCopyCells(123, 15, 5, 3, 110, 15);
	FieldStateWork->raisedTrigger = TRIGGER_STEAM_ROOM_MOAI;
}

/* Each frame while a stream drains: its level falls a step and the water's */
/* pictures follow it down from the Moai, until the bed is dry. */
void scAquaRockInsideDrainStream(void) {
	s32 count = AquaRockInside_SpringLevel - 1;
	s32 row;

	AquaRockInside_SpringLevel = count;
	row = count / 8;
	fdMapGridCopyCells((count & 1) * 5 + 118, 47 - row, 5, 2, 110, 47 - row);
	if(AquaRockInside_SpringLevel == 0)
		syFieldTaskRemove(scAquaRockInsideDrainStream);
}

/* After Parch on its Moai, the stream of Steam's room drains away: Felix */
/* faces south and the camera follows the water down the stream's bed to its */
/* far end, then the bed (cells (46, 16) to (50, 47)) becomes dry ground. */
void scAquaRockInsideSteamRoomStreamDrains(void) {
	s32 priority;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdEventPlaySnd(SE_RISE_WITH_SPARKS_LOW);
	priority = TASK_PASS_FRAME;
	syFieldTaskAddFrame(scAquaRockInsideDrainStream, priority);
	fdCameraSetSpeedAndAcceleration(2.5, 0.25);
	fdCameraMoveTo(776, CAMERA_KEEP, 648, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(30);
	fdEventEnd();
	fdEventSetFlag(FLAG_STEAM_ROOM_STREAM_DRAINED);
	fdMapGridCopyCellAttributes(59, 16, 5, 32, 46, 16);
}

/* The Aquarius Stone shown set in the column. */
void scAquaRockInsideShowAquariusStone(void) {
	ACTOR *actor;
	SPR *sprite;

	scFieldActorShowItemIconById(ACTOR_AQUARIUS_STONE, ITEM_AQUARIUS_STONE);
	/* UB: this room has no actor 20 (it places actors 8 to 10 and treasure */
	/* spots 64 and 65), so fdActorGet returns NULL and the stores below go */
	/* through it. */
	actor = fdActorGet(20);
#ifdef UBFIX
	if(actor == NULL)
		return;
#endif
	sprite = actor->spr;
	sprite->retainCacheEntry = 1;
	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(24);
}

/* The great pool's pictures as it drains, from full to dry. */
#define SPRING_LEVELS 16

/* Where on the map each of the pool's pictures is taken from: a column and a */
/* row for each. */
extern u8 AquaRockInside_SpringLevelCells[SPRING_LEVELS * 2];

/* Using the Aquarius Stone on the empty column at the south of the great */
/* pool's room sets it there. The music fades, the ground shakes harder and */
/* harder, and as the camera pans north to the dais in the middle of the pool */
/* the water falls away to leave a causeway and a gold door in the dais; a */
/* jingle plays once the shaking stops. */
void scAquaRockInsideUseAquariusStone(void) {
	s32 priority;
	s32 i;
	s32 j;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	scAquaRockInsideShowAquariusStone();
	fdEventPlaySnd(SOUND_FADE_OUT_BGM);
	syFieldEventWait(60);
	fdEventPlaySnd(SE_WAVE);
	fdMapGridSetShake(1, 1, 1);
	syFieldEventWait(20);
	fdMapGridSetShake(1, 2, 1);
	syFieldEventWait(20);
	fdMapGridSetShake(1, 3, 1);
	syFieldEventWait(40);
	/* scAquaRockInsideDrainStream runs here too, as in Steam's room, rewriting
	 */
	/* cells (110, 16) to (114, 47); in this room's map those cells lie far east
	 */
	/* of the room (x 1760 on, the room ending at x 704), so nothing the player
	 */
	/* sees is them. */
	priority = TASK_PASS_FRAME;
	syFieldTaskAddFrame(scAquaRockInsideDrainStream, priority);
	fdCameraSetSpeedAndAcceleration(2.5, 0.25);
	fdCameraMoveTo(312, CAMERA_KEEP, 504, 1);
	fdEventPlaySnd(SE_SCATTER);
	fdMapGridCopyCellsLayered(75, 96, 7, 13, 80, 27);
	fdMapGridCopyCellsLayered(91, 96, 7, 10, 16, 31);
	fdMapGridCopyCellsLayered(99, 96, 7, 14, 16, 91);
	fdMapGridCopyCellAttributes(43, 0, 5, 9, 17, 31);
	for(i = 0; i < SPRING_LEVELS - 1; i++) {
		for(j = 0; j <= 6; j++) {
			{
				s32 index = (i + (1 & ~j)) * 2;

				fdMapGridCopyCellsLayered(
					AquaRockInside_SpringLevelCells[index],
					AquaRockInside_SpringLevelCells[index + 1], 7, 13, 80, 27);
				syWait(1);
			}
		}
	}
	fdCameraWaitForArrival();
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	syFieldEventWait(30);
	fdEventPlaySnd(MUS_PUZZLE_SOLVED);
	sySndWaitForBgmRestore();
	fdEventEnd();
	scPlayMusic();
	fdEventSetFlag(FLAG_AQUA_ROCK_AQUARIUS_STONE_SET);
	fdEventSetFlag(FLAG_ITEM_USED_UP);
}

/* Walking into the gold door in the dais of the great pool's room, once the */
/* Aquarius Stone is set (cell (19, 31)): it opens with a thump and Felix walks
 */
/* through, by `exit`, to the Parch tablet's room. */
void scAquaRockInsideOpenDoorToTablet(s32 exit) {
	s32 width = 1;
	s32 height = 2;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_THUMP);
	fdMapGridCopyCellsTo(107, 96, 19, 29, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellsTo(108, 96, 19, 29, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellsTo(107, 98, 19, 29, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellsTo(108, 98, 19, 29, width, height);
	syFieldEventWait(15);
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	fdActorGet(MainRoutineStateWork.leaderActorId)->speedLimit =
		fdActorGet(MainRoutineStateWork.leaderActorId)->acceleration =
			toFIXED(1) / 2;
	fdEventPlaySnd(SE_MAP_EXIT);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 312, 496);
	syFieldEventWait(5);
	syFieldEventSetExitRequest(exit);
}

/* Checking the empty column at the south of the great pool's room: "It's a */
/* column with a spot where you can place something.", and the field menu */
/* opens if the party holds the Aquarius Stone. Once the stone is set, the */
/* message is "You placed the Aquarius Stone." */
void scAquaRockInsideCheckColumn(void) {
	s32 seen;
	const u8 *message;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	seen = fdEventGetFlag(FLAG_AQUA_ROCK_AQUARIUS_STONE_SET);
	message = MsgCheckedTheColumn;
	if(!seen) {
		mnMessageShowCentered(message, 1);
		mnMessageShowCentered(message + 1, 1);
		if(syFieldPartyInventoryFindMember(ITEM_AQUARIUS_STONE) != -1)
			FieldStateWork->fieldMenu = 1;
	}
	else {
		mnMessageShowCentered(message, 1);
		mnMessageArgumentPush(ITEM_AQUARIUS_STONE, MESSAGE_ARGUMENT_ITEM);
		mnMessageShowCentered(message + 2, 1);
	}
	fdEventEnd();
}

/* Douse on the Water Stone on the dais of the great pool's room: two streams */
/* of sparkles run east and west from it along the yellow blocks, six squares.
 */
void scAquaRockInsideDouseAquariusStoneWaterStone(void) {
	s32 i;

	scAquaRockInsideStartSplashes();
	scAquaRockInsideAddSplash(16, 4, 16, 2);
	scAquaRockInsideAddSplash(22, 4, 16, 0);
	for(i = 0; i < 6; i++) {
		scAquaRockInsideAdvanceStreams(COLLISION_STREAM_BEND,
									   COLLISION_STREAM_GOAL);
		syWait(15);
	}
	fdEventSetFlag(FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED);
}

/* Parch on the pool around the sunken Rusty Sword chest dries it for good, */
/* leaving the chest within reach. */
void scAquaRockInsideParchPool(void) {
	fdMapGridCopyCellsLayered(106, 14, 14, 13, 81, 14);
	fdMapGridCopyCellsLayered(44, 78, 14, 12, 17, 78);
	fdMapGridCopyCellAttributes(81, 14, 14, 13, 17, 14);
	fdEventSetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED);
}

/* Gives the cell under actor `id` the collision code `value`, so a stream */
/* turns there. */
s32 scAquaRockInsideBlockCellUnder(s32 id, s32 value) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *actor = fdActorGet(id);

	if(actor != 0) {
		MAP_CELL *cells = map->layers[MAPGRID_LAYER_GROUND].cells;
		MAP_CELL *cell = &cells[FIELD_CELL(actor->x.fixed) +
								FIELD_CELL(actor->z.fixed) * MAPGRID_SIZE];

		cell->collisionCode = value;
		cell->attributeB |= MAP_CELL_JUMP;
	}
}

/* Nothing visible happens when the Moai of the Rusty Sword chest's room's */
/* puzzle has filled: the square under the water pillar Felix can Move takes */
/* MAP_CELL_JUMP and MAP_CELL_WATER, a square to hop across in the water. */
void scAquaRockInsideSetPillarCellJumpableInWater(void) {
	ACTOR *actor = fdActorGet(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

	fdMapGridCopyCellAttributes(64, 43, 1, 1, FIELD_CELL(actor->x.fixed),
								FIELD_CELL(actor->z.fixed));
}

/* Nothing visible happens when the water pillar Felix can Move in the Rusty */
/* Sword chest's room's puzzle comes to rest: its new square becomes one he */
/* hops across (MAP_CELL_JUMP). */
void scAquaRockInsideSetPillarCellJumpable(void) {
	ACTOR *actor = fdActorGet(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

	fdMapGridCopyCellAttributes(64, 42, 1, 1, FIELD_CELL(actor->x.fixed),
								FIELD_CELL(actor->z.fixed));
}

/* Nothing visible happens as Felix starts to Move or push the water pillar of
 */
/* the Rusty Sword chest's room's puzzle: the square it leaves takes */
/* MAP_CELL_PUSH, as the ground a pushed thing slides over does. */
void scAquaRockInsideSetPillarCellPushable(void) {
	ACTOR *actor = fdActorGet(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

	fdMapGridCopyCellAttributes(64, 41, 1, 1, FIELD_CELL(actor->x.fixed),
								FIELD_CELL(actor->z.fixed));
}

/* Douse on the Water Stone of the Rusty Sword chest's room's puzzle, once its
 */
/* Moai has flooded the room: the stream runs along the water pillars, turned */
/* by the one Felix has Moved, and if it reaches its goal it stays turned for */
/* good. Before the Moai fills, nothing happens. */
void scAquaRockInsideDouseRustySwordChestWaterStone(void) {
	s32 i;
	s32 result;

	if(fdEventGetFlag(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED)) {
		scAquaRockInsideBlockCellUnder(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A,
									   COLLISION_STREAM_BEND + 5);
		scAquaRockInsideStartSplashes();
		scAquaRockInsideAddSplash(12, 2, 45, 1);
		for(i = 0; i < 4; i++)
			result = scAquaRockInsideAdvanceStreams(COLLISION_STREAM_BEND,
													COLLISION_STREAM_GOAL);
		fdEventSetFlag(FLAG_RUSTY_SWORD_CHEST_WATER_STONE_DOUSED);
		if(result == 1)
			fdEventSetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED);
	}
}

/* Douse on the grey Moai of the Rusty Sword chest's room's puzzle: it turns */
/* blue and pours water from its mouth, and the room floods around the water */
/* pillars, so that the Water Stone's stream can run. */
void scAquaRockInsideDouseRustySwordChestMoai(s32 unused, s32 id) {
	MAP_LAYER *layer =
		&((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
	s32 i;

	scFieldActorSetSprCol(fdActorGet(ACTOR_RUSTY_SWORD_CHEST_MOAI),
						  SPRITE_COLOR_BRG);
	scAquaRockInsideAddMoaiWater(id);
	fdActorSetAnimation(id, 3);
	layer->ofsY -= toFIXED(WATER_LEVEL_STEP);
	fdMapLoadCellsAndShapes();
	fdMapGridCenterOnFollowedPoint();
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_RISE_WITH_SPARKS_LOW);
	for(i = 0; i < WATER_LEVEL_STEP; i++) {
		layer->ofsY += toFIXED(1);
		syWait(4);
	}
	fdEventEnd();
	scAquaRockInsideSetPillarCellJumpableInWater();
	fdEventSetFlag(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED);
}

/* Felix jumps down the ledge he faces. */
void scAquaRockInsideJumpDownLedge() {
	fdActorJumpDownLedge(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SPRING,
						 0);
	fdEventEnd();
}

/* Stepping in front of a vent in the Vial or Crystal Powder chest's room */
/* (cells (9, 34) and (13, 34) in the first, (17, 22) and (19, 22) in the */
/* second, with the squares south of them): its gust blows Felix south, */
/* spinning, to the edge of the ledge and over it. A stone pillar Moved in */
/* front of the vent shields him, and nothing happens. */
void scAquaRockInsideStepIntoVentGust(s32 trigger) {
	s32 clear = 1;
	ACTOR *actor;
	VECTOR3 target;
	s32 result;

	if(MainRoutineStateWork.next ==
	   SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
		s32 column =
			FIELD_CELL(fdActorGet(ACTOR_CRYSTAL_POWDER_CHEST_PILLAR)->x.fixed);

		if(trigger == TRIGGER_WEST_VENT_GUST && column == 17)
			clear = 0;
		if(trigger == TRIGGER_EAST_VENT_GUST && column == 19)
			clear = 0;
	}
	if(MainRoutineStateWork.next == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
		if(trigger == TRIGGER_WEST_VENT_GUST &&
		   FIELD_CELL(fdActorGet(ACTOR_VIAL_CHEST_WEST_VENT_PILLAR)->x.fixed) ==
			   9)
			clear = 0;
		if(trigger == TRIGGER_EAST_VENT_GUST &&
		   FIELD_CELL(fdActorGet(ACTOR_VIAL_CHEST_EAST_VENT_PILLAR)->x.fixed) ==
			   13)
			clear = 0;
	}
	if(clear == 0)
		return;
	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	for(;;) {
		target.x = actor->x.fixed;
		target.y = actor->y.fixed;
		target.z = actor->z.fixed + toFIXED(8);
		result = fdMapGridCheckCollision(actor, &target);
		if(result > 0)
			break;
		if(result < 0) {
			actor->facing = FACING_SOUTH;
			scAquaRockInsideJumpDownLedge(trigger);
			return;
		}
		{
			s32 offset;
			FIXED x;

			actor->z.fixed += toFIXED(2);
			x = actor->x.fixed;
			offset = (x & (toFIXED(16) - 1)) - toFIXED(8);
			{
				s32 limit = toFIXED(1) + 1;

				if(offset >= limit)
					offset = toFIXED(1);
				limit = -(toFIXED(1) + 1);
				if(offset <= limit)
					offset = -toFIXED(1);
			}
			actor->x.fixed = x - offset;
		}
		actor->facing += TRIG_TURN / 16;
		syWait(1);
	}
}

/* Douse on the Water Stone of the Crystal Powder chest's room: a stream of */
/* sparkles runs west from it along the yellow blocks, five squares. */
void scAquaRockInsideDouseCrystalPowderChestWaterStone(void) {
	s32 i;

	scAquaRockInsideStartSplashes();
	scAquaRockInsideAddSplash(44, 0, 34, 2);
	for(i = 0; i < 5; i++) {
		scAquaRockInsideAdvanceStreams(COLLISION_STREAM_BEND,
									   COLLISION_STREAM_GOAL);
		syWait(15);
	}
	fdEventSetFlag(FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED);
}

/* Stepping into the foot of the waterfall in the Vial chest's room, at the */
/* north end of its pool (cell (10, 10)): Felix tumbles, turns blue with his */
/* arms raised and is carried up out of sight, and comes out by `exit` in the */
/* Mist Sabre chest's room on Aqua Rock's slopes. */
void scAquaRockInsideRideFalls(s32 exit) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIXED speed;
	s32 i;
	u8 *flags;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	flags = &actor->motionFlg;
	speed = 0;
	*flags = 0;
	actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSelectAnimation(actor, ANIM_LEADER_TUMBLE_IN_WATER);
	fdEventPlaySnd(SE_SPLASH_SMALL);
	syWait(13);
	fdActorSetSprColMode(actor, SPRITE_COLOR_BLUE_TINT);
	fdActorSelectAnimation(actor, ANIM_LEADER_ARMS_RAISED);
	fdEventPlaySnd(SE_SPIRAL);
	for(i = 0; i <= 59; i++) {
		if(i == 30)
			fdEventFinishScrTransition();
		speed += toFIXED(1) / 4;
		actor->y.fixed += speed;
		/* Every eighth frame a wading splash is made at the third puddle's
		 * place, */
		/* not the waterfall's: that puddle is not on stage until the Moai is */
		/* blocked, and then lies at (808, 744), in another camera box of the */
		/* room, so none shows on screen as he rises. */
		if((i & 7) == 0)
			fdActorSpawnWadingSplash(fdActorGet(ACTOR_VIAL_CHEST_PUDDLE_C));
		syWait(1);
	}
	fdEventEnd();
	syFieldEventSetExitRequest(exit);
}

/* Moving the stone pillar of the Vial chest's room in front of the green */
/* Moai's mouth (column 49) stops its water: the pool below sinks a level and */
/* three puddles are left on its floor for Frost. */
void scAquaRockInsideBlockMoaiWithPillar(s32 unused, s32 id) {
	ACTOR *actor = fdActorGetById(id);

	if(FIELD_CELL(actor->x.fixed) == 49) {
		MAP_LAYER *layer = &((MAP_STATE *)Ram_HeapSlots->mapGrid)
								->layers[MAPGRID_LAYER_MIDDLE];
		s32 i;
		s32 fade;

		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 0);
		fdEventPlaySnd(SE_RISE_WITH_SPARKS_LOW);
		fdActorSetAnimation(ACTOR_VIAL_CHEST_MOAI, 2);
		fdMapGridCopyCellsLayered(114, 39, 1, 2, 113, 39);
		for(i = 0; i < WATER_LEVEL_STEP; i++) {
			layer->ofsY -= toFIXED(1);
			syWait(4);
		}
		for(i = ACTOR_VIAL_CHEST_PUDDLE_A; i <= ACTOR_VIAL_CHEST_PUDDLE_C;
			i++) {
			actor = fdActorGetById(i);
			actor->x.fixed = INTtoFIXED(actor->tmr);
			actor->z.fixed = toFIXED(actor->userValue);
			actor->motionFlg = 0;
			actor->groundHeight = -toFIXED(WATER_LEVEL_STEP);
			actor->y.fixed = -toFIXED(WATER_LEVEL_STEP);
			scFieldActorSetSprPriority(i, 3);
		}
		fdBlendScriptSuspend();
		for(fade = 12; fade >= 0; fade--) {
			IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
								BLEND_ALPHA(fade, BLEND_WEIGHT_MAX),
								IO_WRITE_HALFWORD);
			syWait(4);
		}
		fdMapGridCopyCellAttributes(105, 41, 12, 13, 41, 41);
		fdMapGridCopyCellsLayered(105, 105, 12, 13, 105, 41);
		fdBlendScriptResume();
		fdEventEnd();
		fdEventSetFlag(FLAG_VIAL_CHEST_MOAI_BLOCKED);
	}
}

/* Frost on one of the three puddles left in the Vial chest's room's sunken */
/* pool freezes it, and Felix can hop across it (MAP_CELL_JUMP). */
void scAquaRockInsideFreezeRoomPuddle(s32 unused, s32 id) {
	if((u32)(id - ACTOR_VIAL_CHEST_PUDDLE_A) <= 2) {
		ACTOR *actor = fdActorGet(id);

		fdMapGridCopyCellAttributes(47, 45, 1, 1, FIELD_CELL(actor->x.fixed),
									FIELD_CELL(actor->z.fixed));
		fdEventSetFlag(FLAG_VIAL_CHEST_PUDDLE_A_FROZEN -
					   ACTOR_VIAL_CHEST_PUDDLE_A + id);
		actor->priFlg = 0;
	}
}

/* Nothing visible happens as Felix starts to Move or push one of the stone */
/* pillars beside the vents of the Vial chest's room: the ground along their */
/* row (row 34) takes MAP_CELL_PUSH, keeping the vents' gusts, so the pillar */
/* can slide in front of a vent. */
void scAquaRockInsideFreeVentPillars(void) {
	fdMapGridCopyCellAttributes(8, 63, 7, 1, 8, 34);
}

/* When either stone pillar beside the vents of the Vial chest's room comes to
 */
/* rest, nothing visible happens: both stop, and the square under each loses */
/* the gust and becomes one Felix hops across (MAP_CELL_JUMP). */
void scAquaRockInsideSettleVentPillars(s32 unused, s32 id) {
	if((u32)(id - ACTOR_VIAL_CHEST_WEST_VENT_PILLAR) <= 1) {
		ACTOR *actor = fdActorGet(ACTOR_VIAL_CHEST_WEST_VENT_PILLAR);

		actor->motionFlg = 0;
		{
			s32 row = 33;

			fdMapGridCopyCellAttributes(8, row, 1, 1,
										FIELD_CELL(actor->x.fixed), 34);
		}
		actor = fdActorGet(ACTOR_VIAL_CHEST_EAST_VENT_PILLAR);
		actor->motionFlg = 0;
		{
			s32 row = 33;

			fdMapGridCopyCellAttributes(8, row, 1, 1,
										FIELD_CELL(actor->x.fixed), 34);
		}
	}
}

/* Waits, for 60 frames at most, until the actor stops moving up or down. */
void scAquaRockInsideWaitToRise(ACTOR *actor) {
	s32 frames = 60;

	while(frames != 0) {
		syWait(1);
		frames--;
		if(actor->velY == 0)
			break;
	}
}

/* Checking the big Psynergy Stone in the middle of its cave (cell (12, 13)): */
/* "Everyone's PP was replenished!" */
void scAquaRockInsideCheckPsynergyStone(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	mnMessageShowCentered(MsgCheckedThePsynergy, 1);
	fdEventPlaySnd(SE_RECOVERY);
	fdPartyAddPp(PARTY_PP_REPLENISH, 0);
	syFieldEventWait(10);
	fdEventClearFlag(FLAG_NOTHING_HAPPENS);
	mnMessageShowCentered(MsgCheckedThePsynergy + 1, 1);
	fdEventEnd();
}

/* Move on the west of the two stone pillars at the south of the Tear Stone's */
/* room (row 39): the ground follows it, the two pillars' starting squares */
/* going back to floor and its new square taking the pillar's. */
void scAquaRockInsideMovePuzzlePillar(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	fdMapGridCopyCellAttributes(17, 39, 2, 1, 20, 39);
	fdMapGridCopyCellAttributes(23, 39, 1, 1, FIELD_CELL(actor->x.fixed), 39);
}

/* Douse on the Water Stone of the Tear Stone's room's puzzle, once its Moai */
/* has flooded the room: the stream runs along the water pillars, turned by */
/* the three Felix can Move, and if it reaches its goal it stays turned for */
/* good. Before the Moai fills, nothing happens. */
void scAquaRockInsideDouseTearStoneWaterStone(void) {
	s32 i;
	s32 result;

	if(fdEventGetFlag(FLAG_TEAR_STONE_MOAI_FILLED)) {
		Call2((void (*)())scAquaRockInsideBlockCellUnder,
			  ACTOR_TEAR_STONE_WATER_PILLAR_C, COLLISION_STREAM_BEND + 5);
		Call2((void (*)())scAquaRockInsideBlockCellUnder,
			  ACTOR_TEAR_STONE_WATER_PILLAR_D, COLLISION_STREAM_BEND + 6);
		scAquaRockInsideBlockCellUnder(ACTOR_TEAR_STONE_WATER_PILLAR_F,
									   COLLISION_STREAM_BEND + 4);
		scAquaRockInsideStartSplashes();
		scAquaRockInsideAddSplash(38, 2, 19, 1);
		for(i = 0; i < 9; i++)
			result = scAquaRockInsideAdvanceStreams(COLLISION_STREAM_BEND,
													COLLISION_STREAM_GOAL);
		fdEventSetFlag(FLAG_TEAR_STONE_WATER_STONE_DOUSED);
		if(result == 1)
			fdEventSetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED);
	}
}

/* Douse on the grey Moai of the Tear Stone's room's puzzle: it turns blue and
 */
/* pours water from its mouth, and the room floods around the water pillars. */
void scAquaRockInsideDouseTearStoneMoai(s32 unused, s32 id) {
	MAP_LAYER *layer =
		&((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
	s32 i;

	scFieldActorSetSprCol(fdActorGet(id), SPRITE_COLOR_BRG);
	scAquaRockInsideAddMoaiWater(id);
	fdActorSetAnimation(id, 3);
	layer->ofsY -= toFIXED(WATER_LEVEL_STEP);
	fdMapLoadCellsAndShapes();
	fdMapGridCenterOnFollowedPoint();
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_RISE_WITH_SPARKS_HIGH);
	for(i = 0; i < WATER_LEVEL_STEP; i++) {
		layer->ofsY += toFIXED(1);
		syWait(4);
	}
	fdEventEnd();
	fdEventSetFlag(FLAG_TEAR_STONE_MOAI_FILLED);
}

/* The times the last pool's cells flicker between two of its pictures, two */
/* frames each, as the water starts to sink. */
enum { POOL_FLICKERS = 15 };

/* Once Piers has learned Parch, the ground trembles and the water round the */
/* tablet's dais sinks away, but a pool is left on the steps before the */
/* tablet, which keeps Felix from leaving until Parch dries it. */
void scAquaRockInsideWaterSinks(void) {
	MAP_LAYER *layer =
		&((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
	s32 i;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	layer->ofsY -= toFIXED(WATER_LEVEL_STEP);
	fdMapGridCenterOnFollowedPoint();
	fdMapGridSetShake(1, 1, 1);
	for(i = 0; i < POOL_FLICKERS; i++) {
		{
			s32 column = 77;
			s32 row = 8;

			fdMapGridCopyCellsLayered(70, 8, 7, 6, column, row);
			syWait(1);
			fdMapGridCopyCellsLayered(50, 8, 7, 6, column, row);
			syWait(1);
		}
	}
	syWait(10);
	for(i = 0; i < WATER_LEVEL_STEP * 4; i++) {
		layer->ofsY += toFIXED(1) / 4;
		syWait(1);
	}
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventEnd();
	fdMapGridCopyCellAttributes(38, 9, 7, 6, 13, 9);
	fdEventSetFlag(FLAG_PARCH_TABLET_POOL_LEFT);
}

/* Parch on the pool left on the steps before the Parch tablet dries it, and */
/* the way out of the room is open. */
void scAquaRockInsideParchLastPool(void) {
	fdMapGridCopyCellsLayered(70, 8, 7, 6, 77, 8);
	fdMapGridCopyCellAttributes(38, 17, 7, 6, 13, 9);
	fdEventClearFlag(FLAG_PARCH_TABLET_POOL_LEFT);
}

#include "shared/orbit_spark_spawn.h"

#include "shared/orbiter_spiral.h"

/* Moves around in a circle. */
#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

#include "shared/actor_emit_sparkles.h"

extern u32 Lesson_TabletRiseScript[];
extern u32 AquaRockInside_TabletGlowScript[];
extern s32 AquaRockInside_SpiralSparkScript[];
extern u32 Lesson_AdeptRiseScript[];
extern s32 Lesson_SeekingSparkScript[];
extern s32 Lesson_TabletSettleScript[];
extern s32 Lesson_AdeptSettleScript[];

/* The tablet (`tabletId`) rises and glows, sparks gather on Piers (`adeptId`)
 */
/* and he learns Parch in a flash. */
void scAquaRockInsidePiersLearnsParch(s32 tabletId, s32 adeptId) {
	ACTOR *tablet = fdActorGetById(tabletId);
	ACTOR *adept = fdActorGetById(adeptId);
	ACTOR *actor;
	ACTOR *spark;
	ACTOR *swirl;
	s32 i;
	u16 noMotion;
	const u8 *script;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	s32 unused[3];

	script = Lesson_AdeptRiseScript;
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdEventWait(30);
	fdCameraSetSpeed(0.5);
	fdCameraMoveWithinMapBounds(INTtoFIXED(FIXEDtoINT(tablet->x.fixed)),
								INTtoFIXED(FIXEDtoINT(tablet->y.fixed)),
								INTtoFIXED(FIXEDtoINT(tablet->z.fixed)), 1);
	sySndPlay(SE_RUMBLE);
	fdMapGridSetShake(2, 2, 1);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	tablet->motionFlg = 0;
	tablet->acceleration = toFIXED(1) / 5;
	tablet->speedLimit = toFIXED(1) / 5;
	fdActorSetMovementTypeById(tabletId, Lesson_TabletRiseScript);
	sySndPlay(SE_TABLET_RISE);
	fdEventWait(45);
	fdActorConfigureSprCol(tabletId, SPRITE_COLOR_BLINK);
	REG_BLDALPHA = BLEND_ALPHA(8, BLEND_WEIGHT_MAX);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	for(i = 0; i != TABLET_RISING_SPARKS; i++) {
		actor = ((void *)syImportActorCreate(SPRITE_PARTICLE, tablet->x.fixed,
											 tablet->y.fixed, tablet->z.fixed));
		sySndPlay(SE_GLITTER);
		actor->x.fixed += INTtoFIXED((syRandom() & 31) - 16);
		actor->y.fixed += INTtoFIXED((syRandom() & 15) - 8);
		actor->speedLimit = toFIXED(8);
		actor->acceleration = toFIXED(1) / 5;
		actor->motionFlg = 0;
		fdActorSetSprCol(actor, SPRITE_COLOR_RBG);
		scSetActorSprPriority(actor, 1);
		fdActorSelectAnimation(actor, 7);
		fdActorSetSprFlags(actor, 0);
		fdActorSetBlendMode(actor, 1);
		fdActorStartScript(actor, AquaRockInside_TabletGlowScript);
		REG_BLDALPHA = BLEND_ALPHA(4, BLEND_WEIGHT_MAX);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(10, BLEND_WEIGHT_MAX);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(16, BLEND_WEIGHT_MAX);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(10, BLEND_WEIGHT_MAX);
		sySynch(2);
	}
	REG_BLDALPHA = BLEND_ALPHA(8, BLEND_WEIGHT_MAX);
	sySynch(30);
	for(i = 0; i != LEARN_SPARKS; i++) {
		spark = scFieldActorCreate(SPRITE_PARTICLE, adept->x.fixed,
								   adept->y.fixed, adept->z.fixed);
		sySndPlay(SE_SPARKS);
		spark->tmr = (syRandom() & 0x7fff) + DEGtoANG(180);
		spark->userValue = (((i & 1) << 1) - 1) << ((syRandom() & 3) + 9);
		noMotion = 0;
		spark->motionFlg = noMotion;
		spark->sceneWord = toFIXED(122);
		spark->watchedActor = (FIELD_OBJECT *)adept;
		spark->sceneCnt =
			(LEARN_SPARKS - i) * LEARN_SPARK_INTERVAL + LEARN_SPIRAL_FRAMES;
		fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
		scSetActorSprPriority(spark, 1);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSprFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		syFieldVectorAddPolarOffsetFX(spark->sceneWord, spark->tmr,
									  &spark->x.fixed);
		fdActorStartScript(spark, AquaRockInside_SpiralSparkScript);
		if(i == 3) {
			fdActorSetSprColById(adeptId, SPRITE_COLOR_BLINK);
			adept->motionFlg = noMotion;
			adept->acceleration = toFIXED(1) / 5;
			adept->speedLimit = toFIXED(1) / 5;
			fdActorSetMovementTypeById(adeptId, script);
		}
		sySynch(LEARN_SPARK_INTERVAL);
	}
	sySndPlay(SE_SHIMMER);
	sySynch(16);
	for(i = 2; i != ORBIT_HALF_TURN; i += 2) {
		swirl = scFieldActorCreate(SPRITE_PARTICLE, adept->x.fixed,
								   adept->y.fixed + i * toFIXED(1) - toFIXED(8),
								   adept->z.fixed);
		swirl->tmr = syRandom();
		swirl->userValue = TRIG_TURN / 32;
		swirl->sceneCnt = i;
		swirl->sceneByte = 1;
		swirl->bounce = swirl->x.fixed;
		swirl->sceneWord = 0;
		swirl->gravity = swirl->z.fixed;
		swirl->watchedActor = (FIELD_OBJECT *)adept;
		swirl->motionFlg = (u16)0;
		swirl->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		fdActorSetSprCol(swirl, SPRITE_COLOR_RBG);
		fdActorSelectAnimation(swirl, 7);
		fdActorSetSprFlags(swirl, 0);
		fdActorSetBlendMode(swirl, 1);
		fdActorStartScript(swirl, Lesson_SeekingSparkScript);
	}
	sySndPlay(SE_SHIMMER);
	sySynch(50);
	fdPaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(8);
	sySynch(16);
	fdMapGridSetShake(0, 0, 0);
	fdActorConfigureSprCol(tabletId, SPRITE_COLOR_NORMAL);
	fdActorConfigureSprCol(adeptId, SPRITE_COLOR_NORMAL);
	sySndPlay(SE_FLASH);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(80);
	fdActorSetMovementTypeById(tabletId, Lesson_TabletSettleScript);
	fdActorSetMovementTypeById(adeptId, Lesson_AdeptSettleScript);
	adept->update = fdActorEmitSparkles;
	sySynch(120);
	adept->update = 0;
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	scPlayMusic();
	sySynch(1);
}

/* Reading the tablet from its south side: "Wielder of Water's strength... Lay
 */
/* your hands upon this stone." Piers steps out of the party ("Water's */
/* strength, huh? That sounds like my sort of thing"), touches it, rises in a */
/* glow and learns Parch. The water then sinks, leaving a pool on the steps; */
/* Piers wonders what went wrong, decides their power is being tested, and */
/* walks back into Felix. Once Parch is learned only the tablet's words show. */
void scAquaRockInsideReadParchTablet(void) {
	ACTOR *actor;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	/* "Felix checked the tablet. It looks like there are strange characters..."
	 */
	mnMessageShowCentered((s32)&MsgCheckedTheTabletIt, 1);
	if(fdEventGetFlag(FLAG_PIERS_LEARNED_PARCH)) {
		fdEventEnd();
		return;
	}
	syFieldEventSetMessageId((s32)&MsgCheckedTheTabletIt);
	FieldStateWork->msg = (FieldStateWork->msg + 1);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	syFieldEventWait(1);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 256, 184);
	fdActorWalkTo(ACTOR_PIERS, 274, 184);
	scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	scFieldActorWaitForMoveById(ACTOR_PIERS);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	syFieldEventWait(5);
	/* "Water's strength, huh? That sounds like my sort of thing, doesn't it?"
	 */
	fdEventShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	syFieldEventWait(30);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 248, 184);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST,
						 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_PIERS, 1);
	fdActorWalkToAndWait(ACTOR_PIERS, 264, 168);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	syFieldEventWait(20);
	/* "{actor_name:8} tried to touch the stone slate." */
	mnMessageShowCentered(((s32)&MsgCheckedTheTabletIt + 2), 1);
	scAquaRockInsidePiersLearnsParch(ACTOR_PARCH_TABLET, ACTOR_PIERS);
	fdPartyLearnAbility(PIERS, ABILITY_PARCH);
	fdEventSetFlag(FLAG_PIERS_LEARNED_PARCH);
	fdEventPlaySnd(SE_TREMOR);
	syFieldEventWait(30);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(10);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scAquaRockInsideWaterSinks();
	fdEventPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	syFieldEventWait(30);
	syFieldEventSetMessageId((s32)&MsgWhatWentWrong);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	fdActorFaceEachOther(MainRoutineStateWork.leaderActorId, ACTOR_PIERS, 0);
	syFieldEventWait(10);
	/* "What went wrong? What are we going to do, Felix?" */
	fdEventShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Maybe our power is being tested." */
	fdEventShowMessage(ACTOR_PIERS, 0);
	syFieldEventWait(10);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(actor != 0) {
		fdActorSetDestination(ACTOR_PIERS, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdEventEnd();
}

/* Checking the Parch tablet from its north side: Felix walks round to the */
/* front and reads it (scAquaRockInsideReadParchTablet), or once Parch is */
/* learned only its words show. */
void scAquaRockInsideWalkToParchTablet(void) {
	if(fdEventGetFlag(FLAG_PIERS_LEARNED_PARCH)) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		mnMessageShowCentered(MsgCheckedTheTabletIt, 1);
		fdEventEnd();
	}
	else {
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 248, 152);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 248, 184);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 264, 184);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
		syFieldEventWait(10);
		scAquaRockInsideReadParchTablet();
	}
}

/* Defines a function for checking the Parch tablet from one side: once Piers */
/* has learned Parch the game only says so; before, Felix walks from sideX */
/* round to its front, faces it, and reads it. */
#define DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(name, sideX)                       \
	void name(void) {                                                          \
		if(fdEventGetFlag(FLAG_PIERS_LEARNED_PARCH)) {                         \
			fdEventStart();                                                    \
			syFieldEventEndLastingPsynergy(0);                                 \
			mnMessageShowCentered(MsgCheckedTheTabletIt, 1);                   \
			fdEventEnd();                                                      \
		}                                                                      \
		else {                                                                 \
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, sideX,    \
								 184);                                         \
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 264,      \
								 184);                                         \
			fdActorFaceDirection(MainRoutineStateWork.leaderActorId,           \
								 FACING_NORTH, 0);                             \
			syFieldEventWait(10);                                              \
			scAquaRockInsideReadParchTablet();                                 \
		}                                                                      \
	}

/* Checking the Parch tablet from its east side: Felix steps round to the front
 */
/* and reads it. */
DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(AquaRockInside_ReadTabletFacingWest, 280)

/* Checking the Parch tablet from its west side: Felix steps round to the front
 */
/* and reads it. */
DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(AquaRockInside_ReadTabletFacingEast, 248)

extern TRIGGER AquaRockInside_Triggers[];
extern TRIGGER AquaRockInside_WaterOfLifeChestTriggers[];
extern TRIGGER AquaRockInside_SteamTriggers[];
extern TRIGGER AquaRockInside_AquariusStoneTriggers[];
extern TRIGGER AquaRockInside_RustySwordChestTriggers[];
extern TRIGGER AquaRockInside_CrystalPowderChestTriggers[];
extern TRIGGER AquaRockInside_VialChestTriggers[];
extern TRIGGER AquaRockInside_PsynergyStoneTriggers[];
extern TRIGGER AquaRockInside_TightropeTriggers[];
extern TRIGGER AquaRockInside_TearStoneTriggers[];
extern TRIGGER AquaRockInside_ParchTabletTriggers[];

const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST)
		return AquaRockInside_WaterOfLifeChestTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_STEAM)
		return AquaRockInside_SteamTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE)
		return AquaRockInside_AquariusStoneTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST)
		return AquaRockInside_RustySwordChestTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST)
		return AquaRockInside_CrystalPowderChestTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST)
		return AquaRockInside_VialChestTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE)
		return AquaRockInside_PsynergyStoneTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_TIGHTROPE)
		return AquaRockInside_TightropeTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE)
		return AquaRockInside_TearStoneTriggers;
	if(scene == SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET)
		return AquaRockInside_ParchTabletTriggers;
	return AquaRockInside_Triggers;
}

/* Gives a Moai its water, a sprite added to its own: the pool in the top of */
/* its head and the water pouring from its mouth. */
void scAquaRockInsideAddMoaiWater(s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(actor != 0) {
		SPR_ANIMATION *water =
			(SPR_ANIMATION *)fdSprAddAnimation(actor->spr, SPRITE_MOAI_WATER);

		water->colMode = SPRITE_COLOR_NORMAL;
		water->pri = 0;
		fdActorSelectAnimation(actor, 2);
	}
}

/* Every sixteenth frame, a splash under the actor. */
void scAquaRockInsideSplashBeneath(ACTOR *actor) {
	s32 phase = FrameCount & 15;

	if(phase == 0) {
		ACTOR *child = scFieldActorCreate(SPRITE_SPLASH_RING, actor->x.fixed,
										  actor->y.fixed, actor->z.fixed);

		if(child != 0) {
			FIELD_SPR *sprite = child->spr;

			fdActorStartScript(child, AquaRockInside_SplashScript);
			child->motionFlg = 0;
			child->layer = 1;
			if(sprite != 0) {
				fdSprStartAnimation(sprite, 2);
				sprite->flg = 0;
				sprite->rotation = DEGtoANG(90);
				sprite->pri = 2;
			}
		}
	}
}

/* Arriving in the Vial chest's room down its waterfall from the Mist Sabre */
/* chest's room: Felix, tinted blue, drops spinning into the pool at the */
/* waterfall's foot, splashes and comes to stand on the yellow block there. */
void scAquaRockInsideFallThroughWater(void) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	u8 *flags = &actor->motionFlg;
	s32 saved = *flags;
	FIXED speed;
	s32 i;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventStartScrTransition();
	fdActorSetSprColMode(actor, SPRITE_COLOR_BLUE_TINT);
	*flags = 0;
	actor->y.fixed = toFIXED(80);
	speed = toFIXED(3);
	for(i = 0; i < 32; i++) {
		actor->y.fixed -= speed;
		actor->facing += DEGtoANG(45);
		speed += -(toFIXED(1) / 20);
		fdActorSetSprFlags(actor, 0);
		syWait(1);
	}
	fdActorSpawnWadingSplash(actor);
	fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2);
	*flags = saved;
	actor->velY = toFIXED(8);
	fdEventPlaySnd(SE_SPLASH_SMALL);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 168, 200);
	fdEventEnd();
}

/* How long the vents blow: an hour of frames. */
#define VENT_BLOW_FRAMES (60 * 60 * 60)

/* The vents' hook: they blow for VENT_BLOW_FRAMES, with their sound, then */
/* stop. */
void scAquaRockInsideUpdateVents(void) {
	s32 counter = scVentGetCounter();

	switch(counter) {
	case 0:
		scVentSetBlowing(0, 1);
		scVentSetBlowing(1, 1);
		scQueueMusic(SE_AMBIENT_WATER);
		break;
	case VENT_BLOW_FRAMES - 1:
		scVentSetBlowing(0, 0);
		scVentSetBlowing(1, 0);
		scQueueMusic(-1);
		break;
	}
	scVentAdvanceCounter(VENT_BLOW_FRAMES);
}

extern u16 AquaRockInside_CrystalPowderChestVents[];
extern u16 AquaRockInside_CrystalPowderChestVentPillars[];
extern u16 AquaRockInside_VialChestVents[];
extern u16 AquaRockInside_VialChestVentPillars[];

/* Aqua Rock's entry setup: in each room it shows the statues already filled by
 */
/* their flags (or dims those still empty), opens what the story has opened and
 */
/* arms the room's water and cell handlers by entrance. */
s32 scInit(void) {
	s32 scene = MainRoutineStateWork.next;
	s32 entrance = MainRoutineStateWork.entrance;
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST) {
		if(syGameFlagGet(FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED)) {
			scAquaRockInsideAddMoaiWater(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI);
			fdActorSetAnimation(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI, 3);
		}
		else {
			scFieldActorSetSprCol(
				fdActorGetById(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI),
				SPRITE_COLOR_GREY);
		}
		if(syGameFlagGet(FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED)) {
			scAquaRockInsideAddMoaiWater(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI);
			fdActorSetAnimation(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI, 3);
		}
		else {
			scFieldActorSetSprCol(
				fdActorGetById(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI),
				SPRITE_COLOR_GREY);
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_STEAM) {
		if(syGameFlagGet(FLAG_STEAM_ROOM_STREAM_DRAINED)) {
			fdMapGridCopyCellsLayered(118, 15, 5, 32, 110, 15);
			fdMapGridCopyCellAttributes(59, 16, 5, 32, 46, 16);
			scFieldActorSetSprCol(fdActorGetById(ACTOR_STEAM_ROOM_MOAI),
								  SPRITE_COLOR_GREY);
		}
		else {
			scAquaRockInsideAddMoaiWater(ACTOR_STEAM_ROOM_MOAI);
			fdActorSetAnimation(ACTOR_STEAM_ROOM_MOAI, 3);
		}
		actor = fdActorGetById(ACTOR_STEAM_ROOM_PILLAR);
		actor->motionFlg = 0;
		if(syGameFlagGet(FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED)) {
			fdActorSetPos(ACTOR_STEAM_ROOM_PILLAR, 632, 152);
			fdMapGridCopyCellAttributes(41, 9, 1, 1, 39, 9);
			actor->y.fixed = 0;
			actor->groundHeight = 0;
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE) {
		if(entrance ==
		   AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE)
			fdLeaderRiseFromWater();
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST) {
		scAquaRockInsideAddMoaiWater(ACTOR_RUSTY_SWORD_CHEST_MOAI_A);
		fdActorSetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI_A, 3);
		scAquaRockInsideAddMoaiWater(ACTOR_RUSTY_SWORD_CHEST_MOAI_B);
		fdActorSetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI_B, 3);
		/* In the south-west room, from the Crystal Powder chest's room or the
		 */
		/* south-east one. */
		if((u32)(entrance -
				 AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) <=
		   WAY_RUSTY_SWORD_CHEST_SOUTHWEST_TO_SOUTHEAST -
			   AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
			if(syGameFlagGet(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED)) {
				scFieldActorSetSprCol(
					fdActorGetById(ACTOR_RUSTY_SWORD_CHEST_MOAI),
					SPRITE_COLOR_BRG);
				scAquaRockInsideAddMoaiWater(ACTOR_RUSTY_SWORD_CHEST_MOAI);
				fdActorSetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI, 3);
				scAquaRockInsideSetPillarCellJumpableInWater();
			}
			else {
				scFieldActorSetSprCol(
					fdActorGetById(ACTOR_RUSTY_SWORD_CHEST_MOAI),
					SPRITE_COLOR_GREY);
			}
		}
		fdActorGetById(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_B)->sceneCnt = 1;
		fdActorGetById(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C)->sceneCnt = 1;
		actor = fdActorGetById(ACTOR_RUSTY_SWORD_CHEST);
		if(actor != 0) {
			actor->motionFlg = 0;
			actor->groundHeight = -toFIXED(WATER_LEVEL_STEP);
			actor->y.fixed = -toFIXED(WATER_LEVEL_STEP);
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
		/* In the west room, the vents', from the east one or the Rusty Sword */
		/* chest's room. */
		if((u32)(entrance - WAY_CRYSTAL_POWDER_CHEST_WEST_TO_EAST) <=
		   AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST -
			   WAY_CRYSTAL_POWDER_CHEST_WEST_TO_EAST) {
			/* syVentsStart, defined below, is called with no prototype */
			/* in scope. */
			syVentsStart(AquaRockInside_CrystalPowderChestVents,
						 AquaRockInside_CrystalPowderChestVentPillars,
						 scAquaRockInsideUpdateVents);
			scQueueMusic(SE_AMBIENT_WATER);
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
		/* In the south-west room, the vents'. */
		if(entrance == WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_A ||
		   entrance == WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_B) {
			syVentsStart(AquaRockInside_VialChestVents,
						 AquaRockInside_VialChestVentPillars,
						 scAquaRockInsideUpdateVents);
			scQueueMusic(SE_AMBIENT_WATER);
			scAquaRockInsideSettleVentPillars(
				0, ACTOR_VIAL_CHEST_WEST_VENT_PILLAR);
		}
		if(!syGameFlagGet(FLAG_VIAL_CHEST_MOAI_BLOCKED)) {
			scAquaRockInsideAddMoaiWater(ACTOR_VIAL_CHEST_MOAI);
			fdActorSetAnimation(ACTOR_VIAL_CHEST_MOAI, 3);
		}
		actor = fdActorGetById(ACTOR_VIAL_CHEST_SPLASH);
		fdActorSetSprFlags(actor, 0);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		actor->update = (OBJECT_UPDATE)scAquaRockInsideSplashBeneath;
		if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION) &&
		   entrance ==
			   AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_MIST_SABRE_CHEST)
			scAquaRockInsideFallThroughWater();
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE) {
		if(syGameFlagGet(FLAG_TEAR_STONE_MOAI_FILLED)) {
			scFieldActorSetSprCol(fdActorGetById(ACTOR_TEAR_STONE_MOAI),
								  SPRITE_COLOR_BRG);
			scAquaRockInsideAddMoaiWater(ACTOR_TEAR_STONE_MOAI);
			fdActorSetAnimation(ACTOR_TEAR_STONE_MOAI, 3);
		}
		else {
			scFieldActorSetSprCol(fdActorGetById(ACTOR_TEAR_STONE_MOAI),
								  SPRITE_COLOR_GREY);
		}
		fdActorGetById(ACTOR_TEAR_STONE_WATER_PILLAR_A)->sceneCnt = 1;
		fdActorGetById(ACTOR_TEAR_STONE_WATER_PILLAR_B)->sceneCnt = 1;
		fdActorGetById(ACTOR_TEAR_STONE_WATER_PILLAR_E)->sceneCnt = 1;
		fdActorGetById(ACTOR_TEAR_STONE_WATER_PILLAR_G)->sceneCnt = 1;
		fdActorGetById(ACTOR_TEAR_STONE_WATER_PILLAR_H)->sceneCnt = 1;
		fdActorGetById(ACTOR_TEAR_STONE_WATER_PILLAR_I)->sceneCnt = 1;
	}
	return 0;
}

/* Sets BG1CNT's priority bits, through the volatile copy the function keeps. */
#define SET_BG1CNT_PRIORITY(priority)                                          \
	{                                                                          \
		u16 value;                                                             \
                                                                               \
		value = (control = REG_BG1CNT & ~BGCNT_PRIORITY(3));                   \
		do {                                                                   \
			REG_BG1CNT = value | (priority);                                   \
		} while(0);                                                            \
	}

/* Aqua Rock's map preparation: for each room and entrance it sets the water */
/* level, places the statues and boulders the flags have moved, opens or */
/* closes the cells around them, and in the rooms whose water drains it shows */
/* the drained state once its flag is set. The Vial chest's room puts its */
/* puddles out of sight (x and z cleared) until a pillar has stopped its Moai.
 */
s32 scPrepareMap(void) {
	s32 scene = MainRoutineStateWork.next;
	s32 entrance = MainRoutineStateWork.entrance;
	volatile u16 control;
	s32 streamStep, rustySwordStep, rustySwordPillar, crystalPowderStep,
		hiddenPuddle, vialPuddle, tearStoneStep, tearStonePillar;
	ACTOR *actor;
	MAP_STATE *map;

	fdMapSetUpperLevelWading(Ram_HeapSlots->mapGrid);
	scAquaRockInsideStartPillars();
	if(scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST) {
		/* By the room the party is in: the north-east, the south-east, the
		 * south. */
		if((u32)(entrance - WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_NORTHWEST) <=
			   WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_WEST -
				   WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_NORTHWEST ||
		   entrance == WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_EAST ||
		   entrance == WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTH) {
			scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -2);
		}
		else if(entrance ==
					WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_WEST ||
				entrance ==
					WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_EAST) {
			actor = fdActorGetById(ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK);
			actor->motionFlg = 0;
			actor->groundHeight = 0;
			actor->y.fixed = 0;
			fdActorSelectAnimation((s32)actor, 0);
			actor = fdActorGetById(ACTOR_WATER_OF_LIFE_CHEST_PUDDLE);
			actor->motionFlg = 0;
			actor->groundHeight = 0;
			actor->y.fixed = 0;
			if(!syGameFlagGet(FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED)) {
				fdMapGridCopyCellsLayered(104, 80, 16, 10, 104, 40);
				fdMapGridCopyCellsLayered(110, 116, 18, 12, 39, 105);
				fdMapGridCopyCellAttributes(64, 32, 64, 32, 0, 32);
				scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, 0);
			}
			else {
				scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 4, 0);
				actor = fdActorGetById(ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK);
				scAquaRockInsideSetPillarHeight(FIELD_CELL(actor->x.fixed) - 42,
												0);
			}
			if(syGameFlagGet(FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN)) {
				fdMapGridCopyCellAttributes(41, 42, 1, 1, 43, 44);
			}
		}
		else if(
			(u32)(entrance - WAY_WATER_OF_LIFE_CHEST_SOUTH_TO_NORTHEAST) <=
			AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST -
				WAY_WATER_OF_LIFE_CHEST_SOUTH_TO_NORTHEAST) {
			if(!syGameFlagGet(FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED)) {
				fdMapGridCopyCellsLayered(77, 80, 23, 20, 77, 32);
				fdMapGridCopyCellsLayered(112, 96, 16, 20, 12, 98);
				fdMapGridCopyCellAttributes(64, 32, 64, 32, 0, 32);
				scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, 0);
			}
			else {
				scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 4, 0);
			}
		}
		if(syGameFlagGet(FLAG_AQUA_ROCK_WEST_PILLAR_SET)) {
			fdActorSetPos(ACTOR_WATER_OF_LIFE_CHEST_WEST_PILLAR, 184, 200);
			fdMapGridCopyCellAttributes(0, 12, 1, 1, 9, 12);
			fdMapGridCopyCellAttributes(0, 12, 1, 1, 13, 12);
		}
		if(syGameFlagGet(FLAG_AQUA_ROCK_EAST_PILLAR_SET)) {
			fdActorSetPos(ACTOR_WATER_OF_LIFE_CHEST_EAST_PILLAR, 824, 216);
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_STEAM) {
		/* In the north-west room. */
		if((u32)(entrance -
				 AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE) <=
		   WAY_STEAM_NORTHWEST_TO_EAST -
			   AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE) {
			scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -2);
		}
		actor = fdActorGetById(ACTOR_STEAM_ROOM_MOAI);
		actor->motionFlg = 0;
		actor->groundHeight = 0;
		actor->y.fixed = 0;
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE) {
		fdEffectStartBlankOverlay(0);
		fdActorSetSprFlags((s32)fdActorGetById(ACTOR_AQUARIUS_STONE_BASE), 0);
		fdActorSetSprFlags((s32)fdActorGetById(ACTOR_AQUARIUS_STONE), 0);
		scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 8, -2);
		scAquaRockInsideAddSpouts(COLLISION_SECOND_SPOUTS, 8, 8);
		if(syGameFlagGet(FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED)) {
			scAquaRockInsideStartSplashes();
			scAquaRockInsideAddSplash(16, 4, 16, 2);
			scAquaRockInsideAddSplash(22, 4, 16, 0);
			for(streamStep = 0; streamStep < 6; streamStep++)
				scAquaRockInsideAdvanceStreams(COLLISION_STREAM_BEND,
											   COLLISION_STREAM_GOAL);
		}
		if(syGameFlagGet(FLAG_AQUA_ROCK_AQUARIUS_STONE_SET)) {
			fdMapGridCopyCellsLayered(75, 96, 7, 13, 80, 27);
			fdMapGridCopyCellsLayered(91, 96, 7, 10, 16, 31);
			fdMapGridCopyCellsLayered(99, 96, 7, 14, 16, 91);
			fdMapGridCopyCellAttributes(43, 0, 5, 9, 17, 31);
			scAquaRockInsideShowAquariusStone();
		}
		else {
			fdMapGridCopyCellAttributes(17, 32, 5, 1, 17, 31);
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST) {
		fdEffectStartBlankOverlay(0);
		/* In the south-west room. */
		if((u32)(entrance -
				 AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) <=
		   WAY_RUSTY_SWORD_CHEST_SOUTHWEST_TO_SOUTHEAST -
			   AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
			fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, 5, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_B, 3, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C, 4, 4);
			scAquaRockInsideAddSpouts(COLLISION_SECOND_SPOUTS, 4, 4);
			if(!syGameFlagGet(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED) &&
			   !fdEventGetFlag(
				   FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED)) {
				map = Ram_HeapSlots->mapGrid;
				fdMapGridCopyCellsLayered(64, 64, 16, 13, 71, 39);
				fdMapGridCopyCellAttributes(71, 39, 16, 15, 7, 39);
				scAquaRockInsideSetPillarCellJumpable();
				map->enabled[2] = 1;
				SET_BG1CNT_PRIORITY(1);
			}
			else if(fdEventGetFlag(
						FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED) ||
					syGameFlagGet(FLAG_RUSTY_SWORD_CHEST_WATER_STONE_DOUSED)) {
				if(fdEventGetFlag(
					   FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED)) {
					fdActorSetPos(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, 280,
								  712);
				}
				scAquaRockInsideBlockCellUnder(
					ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A,
					COLLISION_STREAM_BEND + 5);
				scAquaRockInsideStartSplashes();
				scAquaRockInsideAddSplash(12, 2, 45, 1);
				for(rustySwordStep = 0; rustySwordStep < 8; rustySwordStep++)
					scAquaRockInsideAdvanceStreams(COLLISION_STREAM_BEND,
												   COLLISION_STREAM_GOAL);
				fdEventSetFlag(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED);
			}
			for(rustySwordPillar = ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A;
				rustySwordPillar <= ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C;
				rustySwordPillar++) {
				actor = fdActorGetById(rustySwordPillar);
				actor->motionFlg = 0;
				actor->groundHeight = 0;
				actor->y.fixed = 0;
			}
		}
		else {
			scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -4);
		}
		if(syGameFlagGet(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED)) {
			scAquaRockInsideParchPool();
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
		scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -2);
		scAquaRockInsideAddSpouts(COLLISION_SECOND_SPOUTS, 0, 0);
		fdEffectStartBlankOverlay(0);
		/* In the east room. */
		if(((u32)(entrance -
				  AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTHWEST) <=
				WAY_CRYSTAL_POWDER_CHEST_EAST_TO_WEST -
					AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTHWEST ||
			entrance ==
				AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_SOUTH) &&
		   syGameFlagGet(FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED)) {
			scAquaRockInsideStartSplashes();
			scAquaRockInsideAddSplash(44, 0, 34, 2);
			for(crystalPowderStep = 0; crystalPowderStep <= 4;
				crystalPowderStep++)
				scAquaRockInsideAdvanceStreams(COLLISION_STREAM_BEND,
											   COLLISION_STREAM_GOAL);
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
		fdEffectStartBlankOverlay(0);
		if(entrance ==
			   AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTHWEST ||
		   entrance ==
			   AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_MIST_SABRE_CHEST) {
			scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -2);
		}
		else if(
			entrance == ENTRANCE_VIAL_CHEST_WEST_DOORWAY ||
			entrance == ENTRANCE_VIAL_CHEST_EAST_DOORWAY ||
			entrance ==
				AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_SOUTH) {
			scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -2);
		}
		if(!syGameFlagGet(FLAG_VIAL_CHEST_MOAI_BLOCKED)) {
			for(hiddenPuddle = ACTOR_VIAL_CHEST_PUDDLE_A;
				hiddenPuddle <= ACTOR_VIAL_CHEST_PUDDLE_C; hiddenPuddle++) {
				actor = fdActorGetById(hiddenPuddle);
				actor->motionFlg = 0;
				actor->x.fixed = 0;
				actor->z.fixed = 0;
			}
			return 0;
		}
		else {
			fdMapGridCopyCellsLayered(114, 39, 1, 2, 113, 39);
			fdMapGridCopyCellAttributes(105, 41, 12, 13, 41, 41);
			fdMapGridCopyCellsLayered(105, 105, 12, 13, 105, 41);
			for(vialPuddle = ACTOR_VIAL_CHEST_PUDDLE_A;
				vialPuddle <= ACTOR_VIAL_CHEST_PUDDLE_C; vialPuddle++) {
				actor = fdActorGetById(vialPuddle);
				if(syGameFlagGet(FLAG_VIAL_CHEST_PUDDLE_A_FROZEN -
								 ACTOR_VIAL_CHEST_PUDDLE_A + vialPuddle)) {
					fdMapGridCopyCellAttributes(47, 45, 1, 1,
												FIELD_CELL(actor->x.fixed),
												FIELD_CELL(actor->z.fixed));
				}
			}
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE) {
		scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -2);
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_TIGHTROPE) {
		fdEffectStartBlankOverlay(0);
		scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -2);
		fdTightropeInit();
		fdTightropePlace(0, ACTOR_TIGHTROPE_WEST_POST,
						 ACTOR_TIGHTROPE_EAST_POST);
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE) {
		/* In the east room. */
		if((u32)(entrance -
				 AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_NORTHEAST) <=
		   AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_EAST -
			   AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_NORTHEAST) {
			fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_A, 6, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_B, 3, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_C, 5, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_D, 6, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_E, 5, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_F, 4, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_G, 4, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_H, 3, 4);
			Value3((s32 (*)())scAquaRockInsideAddPillar,
				   ACTOR_TEAR_STONE_WATER_PILLAR_I, 6, 4);
			scAquaRockInsideAddSpouts(COLLISION_SECOND_SPOUTS, 4, 4);
			if(!syGameFlagGet(FLAG_TEAR_STONE_MOAI_FILLED) &&
			   !fdEventGetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED)) {
				map = Ram_HeapSlots->mapGrid;
				fdMapGridCopyCellsLayered(64, 64, 19, 17, 100, 13);
				fdMapGridCopyCellsLayered(35, 105, 23, 24, 35, 73);
				fdMapGridCopyCellAttributes(100, 13, 19, 19, 36, 13);
				map->enabled[2] = 1;
				SET_BG1CNT_PRIORITY(1);
			}
			else if(fdEventGetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED) ||
					syGameFlagGet(FLAG_TEAR_STONE_WATER_STONE_DOUSED)) {
				if(fdEventGetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED)) {
					fdActorSetPos(ACTOR_TEAR_STONE_WATER_PILLAR_C, 808, 296);
					fdActorSetPos(ACTOR_TEAR_STONE_WATER_PILLAR_D, 680, 360);
					fdActorSetPos(ACTOR_TEAR_STONE_WATER_PILLAR_F, 744, 424);
				}
				Call2((void (*)())scAquaRockInsideBlockCellUnder,
					  ACTOR_TEAR_STONE_WATER_PILLAR_C,
					  COLLISION_STREAM_BEND + 5);
				Call2((void (*)())scAquaRockInsideBlockCellUnder,
					  ACTOR_TEAR_STONE_WATER_PILLAR_D,
					  COLLISION_STREAM_BEND + 6);
				scAquaRockInsideBlockCellUnder(ACTOR_TEAR_STONE_WATER_PILLAR_F,
											   COLLISION_STREAM_BEND + 4);
				scAquaRockInsideStartSplashes();
				scAquaRockInsideAddSplash(38, 2, 19, 1);
				for(tearStoneStep = 0; tearStoneStep < 8; tearStoneStep++)
					scAquaRockInsideAdvanceStreams(COLLISION_STREAM_BEND,
												   COLLISION_STREAM_GOAL);
				fdEventSetFlag(FLAG_TEAR_STONE_MOAI_FILLED);
			}
		}
		for(tearStonePillar = ACTOR_TEAR_STONE_WATER_PILLAR_A;
			tearStonePillar <= ACTOR_TEAR_STONE_PILLAR_B; tearStonePillar++) {
			actor = fdActorGetById(tearStonePillar);
			actor->motionFlg = 0;
			actor->groundHeight = 0;
			actor->y.fixed = 0;
		}
		actor = fdActorGetById(ACTOR_TEAR_STONE_PILLAR_A);
		fdMapGridCopyCellAttributes(23, 39, 1, 1, FIELD_CELL(actor->x.fixed),
									39);
		/* In the west room. */
		if((u32)(entrance -
				 AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_WEST) <=
		   AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST -
			   AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_WEST) {
			fdEffectStartBlankOverlay(0);
		}
	}
	else if(scene == SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET) {
		fdEffectStartBlankOverlay(0);
		scAquaRockInsideAddSpouts(COLLISION_STREAM_BEND, 0, -2);
		fdEventSetFlag(FLAG_RETREAT_BLOCKED);
		if(syGameFlagGet(FLAG_PARCH_TABLET_POOL_LEFT)) {
			fdMapGridCopyCellAttributes(38, 9, 7, 6, 13, 9);
		}
		else {
			fdMapGridCopyCellsLayered(77, 8, 7, 6, 50, 8);
			fdMapGridCopyCellsLayered(70, 8, 7, 6, 77, 8);
			fdMapGridCopyCellAttributes(38, 17, 7, 6, 13, 9);
		}
	}
	return 0;
}

#include "shared/actor_spawn_wading_splash.h"

#include "shared/actor_emit_ripples.h"

#include "shared/leader_flow_with_stream.h"

#include "shared/moai_add_water.h"

#include "shared/leader_spin_away.h"

/* On odd frames, spawns a drifting effect above the object, swaying with the */
/* frame count. */
#include "shared/actor_emit_mist.h"

#include "shared/leader_sink_away.h"

#include "shared/leader_rise_from_water.h"

/* Draws the scene's pillars: a top sprite for each active one and, when it */
/* stands above its base, a column sprite as tall as its height. */
void scAquaRockInsideDrawPillars(void) {
	PILLAR_STATE *state = &AquaRockInside_Pillars;
	s32 viewX = syMapStateView(Ram_HeapSlots->mapGrid)[0];
	PILLAR_ENTRY *entry = state->entries;
	s32 viewZ;
	s32 originY;
	s32 depth;
	s32 i;
	u32 tile;

	viewX &= FIELD_PIXEL_MASK;
	viewZ = syMapStateView(Ram_HeapSlots->mapGrid)[1] & FIELD_PIXEL_MASK;
	originY = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->origin[1];
	tile = VramCacheEntries[state->vramEntry].ofs >> 5;

	for(i = 0; i < PILLAR_ENTRIES; i++) {
		if(entry->state != 0) {
			s32 *position = entry->pos;
			s32 x;
			s32 y;
			s32 z;
			s32 screenY;
			s32 attributes;

			if(position != 0) {
				x = position[0] - viewX;
				y = position[1] + entry->height - originY;
				z = position[2] - viewZ - originY;
				screenY = z - y;
			}
			else {
				x = entry->x - viewX;
				y = entry->height - originY;
				z = entry->z - viewZ - originY;
				screenY = z - y;
			}
			if(entry->height != entry->base || entry->state == 2)
				attributes = OBJ_PRIORITY(2);
			else
				attributes = OBJ_PRIORITY(3);
			x = FIXEDtoINT(x) - 8;
			screenY = FIXEDtoINT(screenY) - 8;
			depth = FIXEDtoINT(y + z) + WATER_SPRITE_DEPTH;
			if((u32)(x + 15) <= 270) {
				s32 rise;

				if((u32)(screenY + 47) <= 238) {
					x &= 511;
					screenY &= 255;
					rise = entry->kind * PILLAR_TOP_TILES;
					entry->top[0] = 0;
					screenY |= OBJ_X(x);
					screenY |= OBJ_SIZE_16X16 | OBJ_256_COLORS;
					entry->top[1] = screenY;
					entry->top[2] = (tile + rise) | attributes;
					syOamAddSprAtDepthClamped(entry->top, depth);
				}
				screenY = z - y;
				screenY = FIXEDtoINT(screenY) + 8;
				if((u32)(screenY + 47) <= 238) {
					rise = FIXEDtoINT(entry->height - entry->base);
					if(rise != 0) {
						u32 *column;
						rise--;
						screenY &= 255;
						column = entry->column;
						if((u32)rise > PILLAR_COLUMN_PIECES - 1)
							rise = PILLAR_COLUMN_PIECES - 1;
						rise = rise * PILLAR_COLUMN_TILES +
							   SPOUT_KINDS * PILLAR_TOP_TILES;
						column[0] = 0;
						screenY |= OBJ_X(x);
						screenY |= OBJ_SIZE_16X8 | OBJ_256_COLORS;
						entry->column[1] = screenY;
						entry->column[2] = attributes | (tile + rise);
						syOamAddSprAtDepthClamped(column, depth);
					}
				}
			}
		}
		entry++;
	}
}

/* Clears the water pillars, loads their tiles into VRAM and starts their */
/* drawing. */
void scAquaRockInsideStartPillars(void) {
	void *buffer;
	s32 priority;

	syWordsClearEntry(&AquaRockInside_Pillars, sizeof(PILLAR_STATE));
	buffer = syHeapCallAlternatePool(syHeapAlloc, 3200);
	syDecompressTaggedLz(AquaRockInside_PillarTiles, buffer);
	AquaRockInside_Pillars.vramEntry = syVramCacheFindFreeEntry();
	mnVramLoad(AquaRockInside_Pillars.vramEntry, 3200, buffer);
	syHeapRelease(buffer);
	priority = TASK_PASS_FRAME - 100;
	syFieldTaskAddFrame(scAquaRockInsideDrawPillars, priority);
}

/* Adds a water spout on every map cell of the given layer whose spout kind */
/* lies in [first, first + SPOUT_KINDS), at the given heights, and gives those
 */
/* cells MAP_SHAPE_MARKED, its entry set to `level`. */
void scAquaRockInsideAddSpouts(u32 first, s32 level, s32 baseLevel) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *layer = map->layers;
	MAP_CELL *cells = layer->cells;
	s32 originX = layer->ofsX + map->minX;
	s32 originZ = layer->ofsY + map->minZ;
	PILLAR_STATE *state = &AquaRockInside_Pillars;
	s32 width = FIELD_CELL(map->maxX - map->minX);
	s32 depth = FIELD_CELL(map->maxZ - map->minZ);
	PILLAR_ENTRY *spout = &state->entries[state->cnt];
	s32 corner = (FIELD_CELL(map->minZ) << 7) + FIELD_CELL(map->minX);
	s16 row;

	*(s32 *)(MapGridShapes + MAP_SHAPE_MARKED * 4) = MAP_SHAPE(0, level, 0, 0);
	cells += corner;
	row = 0;
	if(row < depth) {
		MAP_CELL *cell;
		s16 column;
		FIXED nextRow;

		do {
			cell = cells + ((u16)row << 7);
			column = 0;
			if(column < width) {
				s32 nextColumn;

				do {
					{
						u32 kind = cell->collisionCode;

						if(kind != 0 && kind >= first &&
						   kind < first + SPOUT_KINDS) {
							spout->x = FIELD_CELL_ORIGIN((u16)column) +
									   originX + toFIXED(8);
							spout->z = FIELD_CELL_ORIGIN((u16)row) + originZ +
									   toFIXED(8);
							spout->base = baseLevel * toFIXED(8);
							spout->height = level * toFIXED(8);
							spout->kind = kind - first;
							spout->pos = 0;
							spout->state = 1;
							spout++;
							state->cnt++;
							{
								u8 *terrain = map->layers[MAPGRID_LAYER_GROUND]
												  .terrainIds +
											  corner;

								terrain[(u16)column + ((u16)row << 7)] =
									MAP_SHAPE_MARKED;
							}
						}
					}
					nextColumn = INTtoFIXED(column) + toFIXED(1);
					column = FIXEDtoINT(nextColumn);
					cell++;
				} while((s32)FIXEDtoINT((u32)nextColumn) < width);
			}
			nextRow = INTtoFIXED(row) + toFIXED(1);
			row = FIXEDtoINT(nextRow);
		} while((s32)FIXEDtoINT((u32)nextRow) < depth);
	}
}

/* Adds a water pillar on actor `id`, of `kind`, risen to `height` steps. */
void scAquaRockInsideAddPillar(s32 id, s32 kind, s32 height) {
	ACTOR *actor = fdActorGet(id);
	PILLAR_STATE *state = &AquaRockInside_Pillars;
	PILLAR_ENTRY *entry = &state->entries[state->cnt];

	if(actor != 0) {
		entry->x = 0;
		entry->z = 0;
		entry->base = height * toFIXED(8);
		entry->height = height * toFIXED(8);
		entry->state = 2;
		entry->kind = kind;
		entry->pos = &actor->x.fixed;
		state->cnt++;
	}
}

/* Raises every water pillar to `height`, MAP_SHAPE_MARKED taking it too. */
void scAquaRockInsideSetPillarsHeight(s32 height) {
	PILLAR_STATE *state = &AquaRockInside_Pillars;
	PILLAR_ENTRY *entry;
	s32 i;

	*(s32 *)(Ram_MapCollision + MAP_SHAPE_MARKED * 4) =
		MAP_SHAPE(0, (height / toFIXED(8)) & 0xff, 0, 0);
	entry = state->entries;
	for(i = 0; i < PILLAR_ENTRIES; i++) {
		if(entry->x != 0)
			entry->height = height;
		entry++;
	}
}

/* Raises water pillar `index` alone to `height`, its cell taking */
/* MAP_SHAPE_RAISED_ALONE at that height. */
void scAquaRockInsideSetPillarHeight(u32 index, s32 height) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	PILLAR_STATE *state = &AquaRockInside_Pillars;

	if(index < PILLAR_ENTRIES) {
		PILLAR_ENTRY *entry = &state->entries[index];

		if(entry->x != 0) {
			*(s32 *)(Ram_MapCollision + MAP_SHAPE_RAISED_ALONE * 4) =
				MAP_SHAPE(0, (height / toFIXED(8)) & 0xff, 0, 0);
			{
				u8 *ids = map->layers[MAPGRID_LAYER_GROUND].terrainIds;
				s32 at =
					FIELD_CELL(entry->x) + FIELD_CELL(entry->z) * MAPGRID_SIZE;
				s32 v = MAP_SHAPE_RAISED_ALONE;
				ids[at] = v;
			}
			entry->height = height;
		}
	}
}

/* Draws thirty-two sprites that each shake by a random few pixels a frame. */
void scAquaRockInsideDrawSplashes(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	BEAM_STATE *splashes = &AquaRockInside_Splashes;
	s32 viewX = syMapStateView(map)[0];
	s32 viewZ;
	s32 originY;
	SPLASH *splash;
	s32 i;
	u32 tile;
	s32 priority = OBJ_PRIORITY(2);

	viewX &= FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	originY = map->origin[1];
	tile = VramCacheEntries[splashes->vramEntry].ofs >> 5;
	splash = splashes->splashes;
	i = SPLASH_ENTRIES - 1;

	do {
		if(splash->active != 0) {
			s32 x = splash->x - viewX;
			s32 y = splash->y - originY;
			s32 z = splash->z;
			s32 screenY, rawX, rawY;
			z -= viewZ;
			z -= originY;
			screenY = z - y;
			rawX = FIXEDtoINT(x) + (syRandomBelow(10));

			x = rawX - 10;
			rawY = FIXEDtoINT(screenY) + (syRandomBelow(10));
			screenY = rawY - 10;
			z = FIXEDtoINT(y + z) + WATER_SPRITE_DEPTH;

			if((u32)(x + 15) <= 270 && (u32)(screenY + 47) <= 238) {
				u32 *frame = &splash->spr[2];
				s32 column = x & 511;

				screenY &= 255;
				splash->spr[0] = 0;
				splash->spr[1] =
					screenY | OBJ_X(column) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
				*frame = (tile + ((syRandomBelow(3)) << 3)) | priority;
				syOamAddSprAtDepthClamped(splash->spr, z);
			}
		}
		i--;
		splash++;
	} while(i >= 0);
}

/* Clears the streams' splashes, loads their tiles into VRAM and starts their */
/* drawing. */
void scAquaRockInsideStartSplashes(void) {
	void *buffer;
	s32 priority;

	syWordsClearEntry(&AquaRockInside_Splashes, sizeof(BEAM_STATE));
	buffer = syHeapCallAlternatePool(syHeapAlloc, 1024);
	syDecompressTaggedLz(AquaRockInside_SplashTiles, buffer);
	AquaRockInside_Splashes.vramEntry = syVramCacheFindFreeEntry();
	mnVramLoad(AquaRockInside_Splashes.vramEntry, 1024, buffer);
	syHeapRelease(buffer);
	priority = TASK_PASS_FRAME - 100;
	syFieldTaskAddFrame(scAquaRockInsideDrawSplashes, priority);
	fdEventPlaySnd(SE_SHIMMER);
}

/* Starts a stream from cell (x, z), at height y, running in direction `kind`;
 */
/* the second call starts the second stream. */
s32 scAquaRockInsideAddSplash(s32 x, s32 y, s32 z, s32 kind) {
	BEAM_STATE *state = &AquaRockInside_Splashes;
	BEAM_ENTRY *entry = state->entries;

	if(entry->x != 0)
		entry++;
	SET_POSITION(entry, FIELD_CELL_CENTER(x), FIELD_CELL_ORIGIN(y),
				 FIELD_CELL_CENTER(z));
	entry->dir = kind;
}

extern s8 AquaRockInside_StreamTurns[];

/* Advances Aqua Rock's two water streams one cell along the map: each stream */
/* follows the turn its cell's kind gives, leaves a splash on every open cell */
/* it crosses, stops on a wall kind and counts the cells of the given kind it */
/* reaches; then it clears the splashes past the new count and opens */
/* the cells under those left. */
s32 scAquaRockInsideAdvanceStreams(s32 first, s32 target) {
	s32 reached = 0;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	BEAM_STATE *state = &AquaRockInside_Splashes;
	s32 count = state->cnt;
	s32 turn;
	s32 again = 0;
	s32 i;
	BEAM_ENTRY *stream = state->entries;
	SPLASH *splash;

	for(i = 0; i < 2; i++) {
		if(stream->x != 0) {
			splash = &state->splashes[state->cnt];
			do {
				MAP_CELL *cells = map->layers[MAPGRID_LAYER_GROUND].cells;
				MAP_CELL *cell = &cells[FIELD_CELL(stream->x) +
										FIELD_CELL(stream->z) * MAPGRID_SIZE];
				s32 kind = cell->collisionCode;

				if(kind == COLLISION_STREAM_OPEN) {
					splash->x = stream->x;
					splash->z = stream->z;
					splash->y = stream->y;
					splash->active = 1;
					splash++;
					state->cnt++;
					turn = 0;
					again = 1;
				}
				else if(kind >= first && kind <= first + 7) {
					turn = AquaRockInside_StreamTurns[(stream->dir << 3) +
													  kind - first];
					if(turn != STREAM_STOP)
						count = state->cnt;
					again = 0;
				}
				else if(kind == target) {
					turn = STREAM_STOP;
					count = state->cnt;
					reached++;
					again = 0;
				}
				else if(kind == 0) {
					turn = STREAM_STOP;
					again = 0;
				}
				if(turn != STREAM_STOP) {
					/* Each step of `turn` is a quarter turn, one way or the
					 * other. */
					stream->dir = (u32)(stream->dir + turn) % 4;
					syFieldVectorAddPolarOffset(16, stream->dir * DEGtoANG(90),
												&stream->x);
				}
			} while(again != 0);
			state->cnt = count;
		}
		stream++;
	}
	splash = &state->splashes[count];
	if(count < SPLASH_ENTRIES) {
		i = SPLASH_ENTRIES - count;
		do {
			if(splash->active != 0)
				splash->active = 0;
			splash++;
		} while(--i != 0);
	}
	splash = state->splashes;
	if(count != 0) {
		i = count;
		do {
			if(splash->active != 0) {
				MAP_CELL *cells = map->layers[MAPGRID_LAYER_GROUND].cells;
				MAP_CELL *cell = &cells[FIELD_CELL(splash->x) +
										FIELD_CELL(splash->z) * MAPGRID_SIZE];

				cell->collisionCode = 0;
				cell->attributeB |= MAP_CELL_JUMP;
			}
			splash++;
		} while(--i != 0);
	}
	return reached;
}
#undef SET_BG1CNT_PRIORITY

#include "../field/actor_vent_bounce.c"

#include "shared/actor_start_scripted.h"

/* A puff of mist from a vent at the position, rising from `base`. */
void scVentPuff(s32 *position, s32 base) {
	ACTOR *actor = scFieldActorCreate(SPRITE_DUST_CLOUD, position[0],
									  position[1], position[2] - toFIXED(2));
	s32 angle;

	if(actor != 0) {
		actor->bounce = syCos(((u32)(syRandom() * (FIXED_ONE * 3 / 16)) >> 16) +
							  (FIXED_ONE * 3 / 16));
		actor->gravity = (u16)syRandom();
		actor->sceneWord = (u16)(syRandom() << 1) + base;
		angle = (u16)syRandom() - DEGtoANG(180);
		actor->tmr = angle;
		fdActorStartScripted(actor, 3);
		actor->update = (OBJECT_UPDATE)scVentBounce;
		fdActorSetBlendMode(actor, 1);
	}
}

#include "shared/vent_update.h"

#include "shared/vents_start.h"

#include "shared/vent_is_blowing.h"

/* Starts or stops vent index's gust. */
s32 scVentSetBlowing(s32 index, s32 blowing) {
	VENT_WORK *state;

	state = VENT_STATE;
	VENT_AT(state, index)->entries[0].active = blowing;
}

/* The vents' frame counter, which their cycles time their gusts by. */
s32 scVentGetCounter(void) { return VENT_STATE->cnt; }

#include "shared/vent_advance_counter.h"

#include "shared/vent_set_gust.h"

#include "shared/vent_set_id.h"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* Where each of the great pool's pictures lies in the map, x then z. */
u8 AquaRockInside_SpringLevelCells[SPRING_LEVELS * 2] = {67, 66, 75, 66, 83, 66, 91, 66, 99, 66, 107, 66, 115, 66, 67, 81, 75, 81, 83, 81, 91, 81, 99, 81, 107, 81, 115, 81, 67, 96, 75, 96};

/* A spark's script. */
u32 OrbitSpark_Script[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

/* A glow rising from the Parch tablet. */
u32 AquaRockInside_TabletGlowScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

/* A spark spiralling in to Piers, leaving a trail of small ones. */
s32 AquaRockInside_SpiralSparkScript[] = {
    SC_CALL(syOrbiterSpiral),
    SC_RELEASE,
    SC_END,
};

#include "shared/lesson_seeking_spark_script.c"

#include "shared/lesson_scripts.c"

/* A splash's script at the foot of the waterfall. */
u32 AquaRockInside_SplashScript[] = {
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};

/* The Crystal Powder chest's room's vents: x, z and the speed of each gust, */
/* ended by 0; and the pillar their puffs land on. */
u16 AquaRockInside_CrystalPowderChestVents[] = {
    280, 344, 2, 312, 344, 2, 0,
};
u16 AquaRockInside_CrystalPowderChestVentPillars[] = {
    ACTOR_CRYSTAL_POWDER_CHEST_PILLAR, 0,
};

/* The Vial chest's room's vents, and the two pillars their puffs land on. */
u16 AquaRockInside_VialChestVents[] = {
    152, 536, 2, 216, 536, 2, 0,
};
u16 AquaRockInside_VialChestVentPillars[] ALIGNED(4) = {
    ACTOR_VIAL_CHEST_WEST_VENT_PILLAR, ACTOR_VIAL_CHEST_EAST_VENT_PILLAR, 0, 0,
};

/* A splash's script: it shows for 20 frames, then is released. */
u32 WadingSplash_Script[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

u32 RippleScript[] = {
    SC_ADD(SW_Y, toFIXED(0.25)),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};
/* clang-format on */

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_AQUA_ROCK_INSIDE_READ_TABLET
#undef SET_BG1CNT_PRIORITY

/* clang-format off */
#define DUST_SCRIPT_SECTION LATE_DATA
#include "shared/dust_burst_script.c"

LATE_DATA const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

LATE_DATA SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 320, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 SceneExits[] = {
    SCENE_EXITS(AquaRockInsideWaterOfLifeChest),
    SCENE_EXIT(AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_TIGHTROPE, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST),
    SCENE_EXIT(WAY_WATER_OF_LIFE_CHEST_NORTHWEST_TO_NORTHEAST, AquaRockInsideWaterOfLifeChest, WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_NORTHWEST),
    SCENE_EXIT(WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_NORTHWEST, AquaRockInsideWaterOfLifeChest, WAY_WATER_OF_LIFE_CHEST_NORTHWEST_TO_NORTHEAST),
    SCENE_EXIT(WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_WEST, AquaRockInsideWaterOfLifeChest, WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_WEST),
    SCENE_EXIT(WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_WEST, AquaRockInsideWaterOfLifeChest, WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_WEST),
    SCENE_EXIT(WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_EAST, AquaRockInsideWaterOfLifeChest, WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_EAST),
    SCENE_EXIT(WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_EAST, AquaRockInsideWaterOfLifeChest, WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_EAST),
    SCENE_EXIT(WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTH, AquaRockInsideWaterOfLifeChest, WAY_WATER_OF_LIFE_CHEST_SOUTH_TO_NORTHEAST),
    SCENE_EXIT(WAY_WATER_OF_LIFE_CHEST_SOUTH_TO_NORTHEAST, AquaRockInsideWaterOfLifeChest, WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST),
    SCENE_EXITS(AquaRockInsideSteam),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE, AquaRockInsidePsynergyStone, AQUA_ROCK_INSIDE_PSYNERGY_STONE_FROM_AQUA_ROCK_INSIDE_STEAM),
    SCENE_EXIT(WAY_STEAM_NORTHWEST_TO_EAST, AquaRockInsideSteam, WAY_STEAM_EAST_TO_NORTHWEST),
    SCENE_EXIT(WAY_STEAM_EAST_TO_NORTHWEST, AquaRockInsideSteam, WAY_STEAM_NORTHWEST_TO_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_TIGHTROPE, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_STEAM),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_A, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_STEAM_NORTH_A),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_B, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_STEAM_NORTH_B),
    SCENE_EXIT(WAY_STEAM_EAST_TO_SOUTHWEST, AquaRockInsideSteam, WAY_STEAM_SOUTHWEST_TO_EAST),
    SCENE_EXIT(WAY_STEAM_SOUTHWEST_TO_EAST, AquaRockInsideSteam, WAY_STEAM_EAST_TO_SOUTHWEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_SOUTH, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_STEAM_SOUTH),
    SCENE_EXITS(AquaRockInsideAquariusStone),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST, AquaRockInsideWaterOfLifeChest, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_NORTH_A, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_A),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_PARCH_TABLET, AquaRockInsideParchTablet, AQUA_ROCK_INSIDE_PARCH_TABLET_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_WEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_EAST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_SOUTH, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_SOUTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE, AquaRockInsidePsynergyStone, AQUA_ROCK_INSIDE_PSYNERGY_STONE_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_NORTH_B, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_B),
    SCENE_EXITS(AquaRockInsideRustySwordChest),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_MOAI, AquaRockMoai, AQUA_ROCK_MOAI_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST, AquaRockInsideWaterOfLifeChest, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_TEAR_STONE, AquaRockInsideTearStone, AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_WEST, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_EAST, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXIT(WAY_RUSTY_SWORD_CHEST_SOUTHWEST_TO_SOUTHEAST, AquaRockInsideRustySwordChest, WAY_RUSTY_SWORD_CHEST_SOUTHEAST_TO_SOUTHWEST),
    SCENE_EXIT(WAY_RUSTY_SWORD_CHEST_SOUTHEAST_TO_SOUTHWEST, AquaRockInsideRustySwordChest, WAY_RUSTY_SWORD_CHEST_SOUTHWEST_TO_SOUTHEAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_TIGHTROPE, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXITS(AquaRockInsideCrystalPowderChest),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTHWEST, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTHWEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTH, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_EAST, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_EAST),
    SCENE_EXIT(WAY_CRYSTAL_POWDER_CHEST_EAST_TO_WEST, AquaRockInsideCrystalPowderChest, WAY_CRYSTAL_POWDER_CHEST_WEST_TO_EAST),
    SCENE_EXIT(WAY_CRYSTAL_POWDER_CHEST_WEST_TO_EAST, AquaRockInsideCrystalPowderChest, WAY_CRYSTAL_POWDER_CHEST_EAST_TO_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_SOUTH, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_SOUTH),
    SCENE_EXITS(AquaRockInsideVialChest),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTHWEST, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTHWEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTH, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_EAST, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_EAST),
    SCENE_EXIT(WAY_VIAL_CHEST_SOUTHEAST_TO_SOUTHWEST_A, AquaRockInsideVialChest, WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_A),
    SCENE_EXIT(WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_A, AquaRockInsideVialChest, WAY_VIAL_CHEST_SOUTHEAST_TO_SOUTHWEST_A),
    SCENE_EXIT(WAY_VIAL_CHEST_SOUTHEAST_TO_SOUTHWEST_B, AquaRockInsideVialChest, WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_B),
    SCENE_EXIT(WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_B, AquaRockInsideVialChest, WAY_VIAL_CHEST_SOUTHEAST_TO_SOUTHWEST_B),
    SCENE_EXIT(WAY_VIAL_CHEST_SOUTHEAST_TO_NORTHEAST_WEST, AquaRockInsideVialChest, ENTRANCE_VIAL_CHEST_WEST_DOORWAY),
    SCENE_EXIT(ENTRANCE_VIAL_CHEST_WEST_DOORWAY, AquaRockInsideVialChest, WAY_VIAL_CHEST_SOUTHEAST_TO_NORTHEAST_WEST),
    SCENE_EXIT(WAY_VIAL_CHEST_SOUTHEAST_TO_NORTHEAST_EAST, AquaRockInsideVialChest, ENTRANCE_VIAL_CHEST_EAST_DOORWAY),
    SCENE_EXIT(ENTRANCE_VIAL_CHEST_EAST_DOORWAY, AquaRockInsideVialChest, WAY_VIAL_CHEST_SOUTHEAST_TO_NORTHEAST_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_SOUTH, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_SOUTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_MIST_SABRE_CHEST, AquaRockMistSabreChest, AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST),
    SCENE_EXITS(AquaRockInsidePsynergyStone),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_STEAM, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_EAST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_WEST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE),
    SCENE_EXITS(AquaRockInsideTightrope),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST, AquaRockInsideWaterOfLifeChest, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_FROM_AQUA_ROCK_INSIDE_TIGHTROPE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE_WEST, AquaRockInsidePsynergyStone, AQUA_ROCK_INSIDE_PSYNERGY_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE_EAST, AquaRockInsidePsynergyStone, AQUA_ROCK_INSIDE_PSYNERGY_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_TIGHTROPE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_STEAM, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_TIGHTROPE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_EAST, AquaRockInsideTearStone, AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_NORTHEAST, AquaRockInsideTearStone, AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_NORTHEAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_WEST, AquaRockInsideTearStone, AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_WEST),
    SCENE_EXITS(AquaRockInsideTearStone),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_WEST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_TEAR_STONE_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_TEAR_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_NORTHEAST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_TEAR_STONE_NORTHEAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_EAST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_TEAR_STONE_EAST),
    SCENE_EXITS(AquaRockInsideParchTablet),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PARCH_TABLET_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_PARCH_TABLET),
    SCENE_EXITS_END,
};

/* The scene's other rooms' actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_Placements[] = {
    {SCENE_TABLE_END},
};

/* The Water of Life chest's room's actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_WaterOfLifeChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 679, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 551, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* Steam's room's actors, in AQUA_ROCK_INSIDE_STEAM_ACTOR_ID's order. */
LATE_DATA ACTOR_PLACE AquaRockInside_SteamPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 247, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_STEAM), MOVEMENT_TYPE_STAND, 776, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/* The Aquarius Stone's room's actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_AquariusStonePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The Rusty Sword chest's room's actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_RustySwordChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 647, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 87, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 103, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

/* The Crystal Powder chest's room's actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_CrystalPowderChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The Vial chest's room's actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_VialChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 631, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The Psynergy Stone's room's actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_PsynergyStonePlacements[] = {
    {SCENE_TABLE_END},
};

/* The tightrope's room's actors: the tightrope's posts. */
LATE_DATA ACTOR_PLACE AquaRockInside_TightropePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The Tear Stone's room's actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_TearStonePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 231, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

/* The Parch tablet's room's actors. */
LATE_DATA ACTOR_PLACE AquaRockInside_ParchTabletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SPRITE_SLATE, CONDITION_ALWAYS, 1, 100, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The scene's other rooms' triggers. */
LATE_DATA TRIGGER AquaRockInside_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/* The Water of Life chest's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_WaterOfLifeChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_TIGHTROPE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, WAY_WATER_OF_LIFE_CHEST_NORTHWEST_TO_NORTHEAST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_NORTHWEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 5, CONDITION_ALWAYS, WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_WEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTHEAST_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 7, CONDITION_ALWAYS, WAY_WATER_OF_LIFE_CHEST_SOUTHEAST_TO_NORTHEAST_EAST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, WAY_WATER_OF_LIFE_CHEST_NORTHEAST_TO_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, WAY_WATER_OF_LIFE_CHEST_SOUTH_TO_NORTHEAST},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TRIGGER_WATER_OF_LIFE_CHEST_GAP, CONDITION_ALWAYS, EVENT(scAquaRockInsideJumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_WATER_OF_LIFE_CHEST_GAP, CONDITION_ALWAYS, EVENT(scAquaRockInsideJumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_WATER_OF_LIFE_CHEST_SOUTH_GAP, CONDITION_ALWAYS, EVENT(scAquaRockInsideJumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_WEST_PILLAR, FLAG_AQUA_ROCK_WEST_PILLAR_SET, EVENT(scAquaRockInsideSetPillarInGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_EAST_PILLAR, FLAG_AQUA_ROCK_EAST_PILLAR_SET, EVENT(scAquaRockInsideSetPillarInGap)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAquaRockInsideSetPillarInGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI, FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED, EVENT(scAquaRockInsideDouseWaterOfLifeChestEastMoai)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI, FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED, EVENT(scAquaRockInsideDouseWaterOfLifeChestWestMoai)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK, CONDITION_ALWAYS, (s32)((u8 *)scAquaRockInsideDoNothing + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WATER_OF_LIFE_CHEST_PUDDLE, FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN, EVENT(scAquaRockInsideFreezePuddle)},
    {TRIGGER_TABLE_END},
};

/* The level of a draining stream (scAquaRockInsideDrainStream), 256 when full. */
LATE_DATA s32 AquaRockInside_SpringLevel = 256;

/* Steam's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_SteamTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, WAY_STEAM_NORTHWEST_TO_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, WAY_STEAM_EAST_TO_NORTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_TIGHTROPE},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_A},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 6, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_B},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, WAY_STEAM_EAST_TO_SOUTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 8, CONDITION_ALWAYS, WAY_STEAM_SOUTHWEST_TO_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_SOUTH},
    {TRIGGER_STEP, TRIGGER_STREAM, CONDITION_ALWAYS, EVENT(fdLeaderFlowWithStream)},
    {TRIGGER_TALK, ACTOR_STEAM, CONDITION_ALWAYS, EVENT(scAquaRockInsideFightSteam)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STEAM_ROOM_PILLAR, FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED, EVENT(scAquaRockInsidePushPillarOffLedge)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAquaRockInsidePushPillarOffLedge)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), TRIGGER_STEAM_ROOM_MOAI, FLAG_STEAM_ROOM_STREAM_DRAINED, EVENT(scAquaRockInsideParchSteamRoomMoai)},
    {TRIGGER_RAISED, TRIGGER_STEAM_ROOM_MOAI, FLAG_STEAM_ROOM_STREAM_DRAINED, EVENT(scAquaRockInsideSteamRoomStreamDrains)},
    {TRIGGER_TABLE_END},
};

/* The Aquarius Stone's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_AquariusStoneTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_NORTH_A},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_PARCH_TABLET, CONDITION_ALWAYS, EVENT(scAquaRockInsideOpenDoorToTablet)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_WEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_EAST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_SOUTH},
    {STEP_TRIGGER_FACING(FACING_NORTH), AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE, CONDITION_ALWAYS, EVENT(fdLeaderSinkAway)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_NORTH_B},
    {TRIGGER_TALK, ACTOR_AQUARIUS_STONE_BASE, CONDITION_ALWAYS, EVENT(scAquaRockInsideCheckColumn)},
    {USE_ITEM_ON_ACTOR(ITEM_AQUARIUS_STONE), ACTOR_AQUARIUS_STONE_BASE, FLAG_AQUA_ROCK_AQUARIUS_STONE_SET, EVENT(scAquaRockInsideUseAquariusStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_AQUARIUS_STONE_WATER_STONE, FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED, EVENT(scAquaRockInsideDouseAquariusStoneWaterStone)},
    {TRIGGER_TABLE_END},
};

/* The Rusty Sword chest's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_RustySwordChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_MOAI},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_TEAR_STONE},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 5, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_EAST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, WAY_RUSTY_SWORD_CHEST_SOUTHWEST_TO_SOUTHEAST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, WAY_RUSTY_SWORD_CHEST_SOUTHEAST_TO_SOUTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_TIGHTROPE},
    {STEP_TRIGGER_WALKING, TRIGGER_RUSTY_SWORD_CHEST_GAP, CONDITION_ALWAYS, EVENT(scAquaRockInsideJumpForward)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, CONDITION_ALWAYS, EVENT(scAquaRockInsideSetPillarCellPushable)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAquaRockInsideSetPillarCellPushable)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, CONDITION_ALWAYS, EVENT(scAquaRockInsideSetPillarCellJumpable)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAquaRockInsideSetPillarCellJumpable)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), TRIGGER_RUSTY_SWORD_CHEST_POOL, FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED, EVENT(scAquaRockInsideParchPool)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RUSTY_SWORD_CHEST_WATER_STONE, FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED, EVENT(scAquaRockInsideDouseRustySwordChestWaterStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RUSTY_SWORD_CHEST_MOAI, FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED, EVENT(scAquaRockInsideDouseRustySwordChestMoai)},
    {TRIGGER_TABLE_END},
};

/* The Crystal Powder chest's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_CrystalPowderChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, WAY_CRYSTAL_POWDER_CHEST_EAST_TO_WEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, WAY_CRYSTAL_POWDER_CHEST_WEST_TO_EAST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_SOUTH},
    /* No row of SceneExits takes this room's exits 8 to 11. */
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, TRIGGER_WEST_VENT_GUST, CONDITION_ALWAYS, EVENT(scAquaRockInsideStepIntoVentGust)},
    {TRIGGER_STEP, TRIGGER_EAST_VENT_GUST, CONDITION_ALWAYS, EVENT(scAquaRockInsideStepIntoVentGust)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_CRYSTAL_POWDER_CHEST_GAP, CONDITION_ALWAYS, EVENT(scAquaRockInsideJumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRYSTAL_POWDER_CHEST_PILLAR, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_CRYSTAL_POWDER_CHEST_WATER_STONE, FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED, EVENT(scAquaRockInsideDouseCrystalPowderChestWaterStone)},
    {TRIGGER_TABLE_END},
};

/* The Vial chest's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_VialChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTHWEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTH},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, WAY_VIAL_CHEST_SOUTHEAST_TO_SOUTHWEST_A},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_A},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 6, CONDITION_ALWAYS, WAY_VIAL_CHEST_SOUTHEAST_TO_SOUTHWEST_B},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, WAY_VIAL_CHEST_SOUTHWEST_TO_SOUTHEAST_B},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, WAY_VIAL_CHEST_SOUTHEAST_TO_NORTHEAST_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, ENTRANCE_VIAL_CHEST_WEST_DOORWAY},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, WAY_VIAL_CHEST_SOUTHEAST_TO_NORTHEAST_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 11, CONDITION_ALWAYS, ENTRANCE_VIAL_CHEST_EAST_DOORWAY},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 12, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_SOUTH},
    {TRIGGER_STEP, AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_MIST_SABRE_CHEST, CONDITION_ALWAYS, EVENT(scAquaRockInsideRideFalls)},
    {TRIGGER_STEP, TRIGGER_WEST_VENT_GUST, CONDITION_ALWAYS, EVENT(scAquaRockInsideStepIntoVentGust)},
    {TRIGGER_STEP, TRIGGER_EAST_VENT_GUST, CONDITION_ALWAYS, EVENT(scAquaRockInsideStepIntoVentGust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_MOAI_PILLAR, FLAG_VIAL_CHEST_MOAI_BLOCKED, EVENT(scAquaRockInsideBlockMoaiWithPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_A, FLAG_VIAL_CHEST_PUDDLE_A_FROZEN, EVENT(scAquaRockInsideFreezeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_B, FLAG_VIAL_CHEST_PUDDLE_B_FROZEN, EVENT(scAquaRockInsideFreezeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_C, FLAG_VIAL_CHEST_PUDDLE_C_FROZEN, EVENT(scAquaRockInsideFreezeRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_WEST_VENT_PILLAR, CONDITION_ALWAYS, EVENT(scAquaRockInsideFreeVentPillars)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_EAST_VENT_PILLAR, CONDITION_ALWAYS, EVENT(scAquaRockInsideFreeVentPillars)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAquaRockInsideFreeVentPillars)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_WEST_VENT_PILLAR, CONDITION_ALWAYS, EVENT(scAquaRockInsideSettleVentPillars)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_EAST_VENT_PILLAR, CONDITION_ALWAYS, EVENT(scAquaRockInsideSettleVentPillars)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAquaRockInsideSettleVentPillars)},
    {TRIGGER_TABLE_END},
};

/* The Psynergy Stone's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_PsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_STEAM},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_WEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE},
    {TRIGGER_EXAMINE, TRIGGER_PSYNERGY_STONE, CONDITION_ALWAYS, EVENT(scAquaRockInsideCheckPsynergyStone)},
    {TRIGGER_TABLE_END},
};

/* The tightrope's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_TightropeTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_STEAM},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_EAST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_NORTHEAST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_WEST},
    {TRIGGER_TABLE_END},
};

/* The Tear Stone's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_TearStoneTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_NORTHEAST},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_EAST},
    {TRIGGER_STEP, TRIGGER_STREAM, CONDITION_ALWAYS, EVENT(fdLeaderFlowWithStream)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_PILLAR_A, CONDITION_ALWAYS, EVENT(scAquaRockInsideMovePuzzlePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_C, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_D, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_F, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TEAR_STONE_WATER_STONE, FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED, EVENT(scAquaRockInsideDouseTearStoneWaterStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TEAR_STONE_MOAI, FLAG_TEAR_STONE_MOAI_FILLED, EVENT(scAquaRockInsideDouseTearStoneMoai)},
    {TRIGGER_TABLE_END},
};

/* The Parch tablet's room's triggers. */
LATE_DATA TRIGGER AquaRockInside_ParchTabletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, AQUA_ROCK_INSIDE_PARCH_TABLET_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), TRIGGER_PARCH_TABLET, CONDITION_ALWAYS, EVENT(scAquaRockInsideReadParchTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), TRIGGER_PARCH_TABLET, CONDITION_ALWAYS, EVENT(scAquaRockInsideWalkToParchTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), TRIGGER_PARCH_TABLET, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadTabletFacingWest)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), TRIGGER_PARCH_TABLET, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadTabletFacingEast)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), TRIGGER_PARCH_TABLET_POOL, IF_FLAG_SET(FLAG_PARCH_TABLET_POOL_LEFT), EVENT(scAquaRockInsideParchLastPool)},
    {TRIGGER_TABLE_END},
};

/* How a stream turns on each bend's cell, by its direction and the cell's kind; */
/* 8 stops it. */
LATE_DATA s8 AquaRockInside_StreamTurns[] = {
    8, 8, 0, 8, -1, 8, 1, 8,
    8, 0, 8, -1, 1, 8, 8, 8,
    8, 8, 0, 1, 8, -1, 8, 8,
    8, 0, 8, 8, 8, 1, -1, 8,
};
/* clang-format on */
