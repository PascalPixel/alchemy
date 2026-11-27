/* @file */
/* Taopo Swamp, over the bridge west of Yallam: an optional dungeon of eleven */
/* rooms that share this overlay. */
/* */
/* In the swamp outside, Move pulls the stump at the end of the broken plank */
/* bridge into the mud so Felix can hop the gap, and each step through the mud
 */
/* sinks him deeper until he goes under and comes up on firm ground, unless a */
/* bubble lifts him out first. Whirlwind blows away the weeds over three cave */
/* mouths at the foot of the cliffs, and Growth grows the sprout beside the */
/* eastern cave into a vine. Behind the westernmost clump of weeds, a small */
/* cave holds a buried Tear Stone. Behind the eastern clump of weeds, stairs */
/* lead down to the vents room, where vents in the north wall puff gusts that */
/* blow Felix off the ledge into the room below; there Cyclone scatters the */
/* weeds hiding the Bramble Seed and a Mad Plant. Moving the vents room's east
 */
/* pillar over its vent sends the next gust onto Flower, the Venus Djinni, */
/* which falls to that same room below. A passage north from the vents room */
/* ends at the second Tear Stone. Behind the third clump of weeds lie the pool,
 */
/* a round spot in a gust that Douse wets and Frost freezes into an ice pillar,
 */
/* and the drips room, where drops from the ceiling fill a second spot. Below */
/* them, glowing red rocks cooled with Douse and pushed into the lava make */
/* stepping stones (in the Tremor room, and in the lava rocks' room with the */
/* Vial's chest); Tremor on the pillar under a rock pile dries a lava stream */
/* into a passage; and in the dark rooms beyond, cooling the last glowing rock
 */
/* shows where the Star Dust is buried. */
#include "version.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "game/game.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"

s32 fdMapGetTerrainHeight(s32 terrain, s32 x, s32 z);

/* What the rooms' map cells do when Felix steps on them or casts at them, */
/* beside their exits, which scene_exits.h numbers, and the vents' gusts */
/* (TRIGGER_FIRST_GUST). */
enum {
	/* Outside: the swamp's mud, its bubbles, and the four cells at its edges */
	/* that count as firm ground. */
	TRIGGER_MUD = 60,
	TRIGGER_BUBBLE = 61,
	TRIGGER_FIRM_GROUND = 62,
	/* The buried Tear Stones' spots, in the cave behind the westernmost clump
	   of */
	/* weeds and in the cave north of the vents room. */
	TRIGGER_FIRST_TEAR_STONE = 50,
	TRIGGER_SECOND_TEAR_STONE = 51,
	/* In the room below the vents: weeds Cyclone scatters, the weeds over the
	 */
	/* Bramble Seed and those the Mad Plant hides in. */
	TRIGGER_WEEDS = 30,
	TRIGGER_BRAMBLE_SEED_WEEDS = 31,
	TRIGGER_MAD_PLANT_WEEDS = 32,
	/* In the lava rocks' room, the cell Felix jumps from onto the sunken rock.
	 */
	TRIGGER_JUMP_TO_ROCK = 64,
	/* The Star Dust's spot in the last dark chamber. */
	TRIGGER_STAR_DUST = 50
};

/* The doors between rooms that share one map, each numbered as the exit at */
/* its side and as the entrance the other side's exit leads to. */
enum {
	/* The stairs down from the passage north of the vents room into the cave */
	/* of the second Tear Stone, and back up. */
	TEAR_STONES_PASSAGE_TO_CAVE = 8,
	TEAR_STONES_CAVE_TO_PASSAGE = 9,
	/* Past the pool, the way from the pillar's south room to the round spot's
	 */
	/* north room, and back. */
	PILLAR_GAP_SOUTH_TO_NORTH_ROOM = 4,
	PILLAR_GAP_NORTH_TO_SOUTH_ROOM = 5,
	/* The drips room's passage: the doorway in the north wall of the round */
	/* spot's part, and the south end of the rope posts' part. */
	DRIPS_ROUND_SPOT_TO_ROPE_POSTS = 4,
	DRIPS_ROPE_POSTS_TO_ROUND_SPOT = 5,
	/* The Star Dust's three dark rooms: the first, below the Tremor room, the
	 */
	/* middle one, and the last chamber. */
	STAR_DUST_FIRST_TO_MIDDLE_ROOM = 3,
	STAR_DUST_MIDDLE_TO_FIRST_ROOM = 4,
	STAR_DUST_MIDDLE_TO_LAST_CHAMBER = 5,
	STAR_DUST_LAST_CHAMBER_TO_MIDDLE = 6
};

/* The swamp's actors outside (SceneId_TaopoSwampMud). */
typedef enum TAOPO_SWAMP_MUD_ACTOR_ID {
	/* The five bushes of weeds Whirlwind blows away (SPRITE_WEEDS), each kept
	 */
	/* gone by its flag, FLAG_TAOPO_SWAMP_BUSH_BLOWN plus its id. Three of them
	 */
	/* cover */
	/* cave mouths at the foot of the cliffs, which open once they are gone. */
	ACTOR_FIRST_BUSH = ACTOR_FIRST_PLACED,
	/* The westernmost bush, at cell (13, 8), over the cave of the first Tear */
	/* Stone; the next two, east of it, cover nothing. */
	ACTOR_TEAR_STONES_WEST_BUSH = ACTOR_FIRST_BUSH,
	/* The bush at cell (37, 11), over the way to the pool. */
	ACTOR_POOL_BUSH = ACTOR_FIRST_BUSH + 3,
	/* The bush at cell (44, 16), over the stairs down to the vents room. */
	ACTOR_TEAR_STONES_EAST_BUSH,
	ACTOR_LAST_BUSH = ACTOR_TEAR_STONES_EAST_BUSH,
	/* The sprout at cell (47, 17), east of the eastern cave, which Growth grows
	 */
	/* into a vine (SPRITE_SPROUT). */
	ACTOR_MUD_SPROUT,
	/* The stump at the east end of the broken plank bridge, cell (13, 16), */
	/* which Move pulls into the mud (SPRITE_STUMP). */
	ACTOR_MUD_STUMP
} TAOPO_SWAMP_MUD_ACTOR_ID;

/* Flower, the Venus Djinni: on its ledge in the vents room, then, blown off */
/* it, in the room below (SceneId_TaopoSwampFlower). */
#define ACTOR_FLOWER ACTOR_FIRST_PLACED

/* The vents room's pillars Move pushes over a vent (SceneId_TaopoSwampVents).
 */
typedef enum TAOPO_SWAMP_VENTS_ACTOR_ID {
	ACTOR_WEST_VENT_PILLAR = ACTOR_FLOWER + 1,
	ACTOR_EAST_VENT_PILLAR
} TAOPO_SWAMP_VENTS_ACTOR_ID;

/* The actors of the rooms past the pool (SceneId_TaopoSwampPillarGap): the */
/* round spot in the north room, and the pillar by the gap in the south one. */
typedef enum TAOPO_SWAMP_PILLAR_GAP_ACTOR_ID {
	/* The round spot in a vent's gust that Douse wets and Frost freezes */
	/* (SPRITE_PUDDLE). */
	ACTOR_GAP_PUDDLE = ACTOR_FIRST_PLACED,
	/* The pillar Move drops into the gap. */
	ACTOR_GAP_PILLAR
} TAOPO_SWAMP_PILLAR_GAP_ACTOR_ID;

/* The drips room's actors (SceneId_TaopoSwampDrips). */
typedef enum TAOPO_SWAMP_DRIPS_ACTOR_ID {
	/* The two posts Lash ties a rope between (SPRITE_TIGHTROPE_POST). */
	ACTOR_WEST_ROPE_POST = ACTOR_FIRST_PLACED,
	ACTOR_EAST_ROPE_POST,
	/* The round spot the ceiling drips onto, in a vent's gust (SPRITE_PUDDLE).
	 */
	ACTOR_DRIP_PUDDLE
} TAOPO_SWAMP_DRIPS_ACTOR_ID;

/* The lava rocks' room's two glowing rocks (SceneId_TaopoSwampLavaRocks, */
/* SPRITE_EMBER_STONE): on the way to the chest and by the way out. */
typedef enum TAOPO_SWAMP_LAVA_ROCKS_ACTOR_ID {
	ACTOR_CHEST_ROCK = ACTOR_FIRST_PLACED,
	ACTOR_EXIT_ROCK
} TAOPO_SWAMP_LAVA_ROCKS_ACTOR_ID;

/* The Tremor room's actors (SceneId_TaopoSwampTremor). */
typedef enum TAOPO_SWAMP_TREMOR_ACTOR_ID {
	/* The glowing rock by the lava, and the one Tremor knocks out of the pillar
	 */
	/* (SPRITE_EMBER_STONE). */
	ACTOR_TREMOR_LAVA_ROCK = ACTOR_FIRST_PLACED + 1,
	ACTOR_TREMOR_PILLAR_ROCK,
	/* The pillar Tremor shakes, and the rock pile on it that rolls off. */
	ACTOR_TREMOR_PILLAR,
	ACTOR_TREMOR_ROCK_PILE = ACTOR_FIRST_PLACED + 11
} TAOPO_SWAMP_TREMOR_ACTOR_ID;

/* The clods Tremor bursts out of the pillar. */
#define TREMOR_CLODS 16

/* The pictures of the Tremor room's pool cooling, 13 by 5 cells each, stacked
 */
/* up the map from row 114 to row 64 at x 50. */
#define POOL_COOLING_PICTURES 11

/* The glowing rocks that light the Star Dust's rooms, the first room's and the
 */
/* last chamber's (SceneId_TaopoSwampStarDust, SPRITE_EMBER_STONE). */
typedef enum TAOPO_SWAMP_STAR_DUST_ACTOR_ID {
	ACTOR_DARK_ROCK = ACTOR_FIRST_PLACED,
	ACTOR_LAST_ROCK
} TAOPO_SWAMP_STAR_DUST_ACTOR_ID;

/* Whirlwind's flags for the swamp's bushes, one per actor id from */
/* ACTOR_FIRST_BUSH. */
#define FLAG_TAOPO_SWAMP_BUSH_BLOWN 0x8d8
/* Set once the swamp's stump has sunk. */
#define FLAG_TAOPO_SWAMP_STUMP_SUNK 0x8e5
/* Set once Move has left the vents room's west or east pillar over its vent. */
#define FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT 0xa6d
#define FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT 0xa6e
/* Set once the pillar has dropped into the gap. */
#define FLAG_TAOPO_SWAMP_PILLAR_IN_GAP 0xa6f
/* Set once the party has taken the Bramble Seed buried under the Flower */
/* room's weeds, at (BRAMBLE_SEED_X, BRAMBLE_SEED_Z). */
#define FLAG_TAOPO_SWAMP_BRAMBLE_SEED_TAKEN 0xf99
#define BRAMBLE_SEED_X						264
#define BRAMBLE_SEED_Z						664
/* Set once Scoop has dug up the Star Dust. */
#define FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP FLAG_AREA_TEMP_48
/* Set once the Mad Plant hiding in the Flower room's weeds has been beaten. */
#define FLAG_TAOPO_SWAMP_MAD_PLANT_BEATEN 0x9b6
/* Set once Growth has grown the swamp's sprout into a vine. */
#define FLAG_TAOPO_SWAMP_VINE_GROWN FLAG_SCENE_TEMP_20
/* The round spot in a vent's gust (ACTOR_GAP_PUDDLE, ACTOR_DRIP_PUDDLE): set */
/* while it holds water, a puddle or an ice pillar. */
#define FLAG_TAOPO_SWAMP_SPOT_WET FLAG_SCENE_TEMP_20
/* Set while Frost has turned the spot's water into an ice pillar. */
#define FLAG_TAOPO_SWAMP_SPOT_FROZEN FLAG_SCENE_TEMP_21
/* Set while the spot holds a puddle Frost can freeze. */
#define FLAG_TAOPO_SWAMP_SPOT_PUDDLE FLAG_SCENE_TEMP_22
/* The drops the drips room's spot has caught, up to four: a byte of flags. */
#define FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT FLAG_SCENE_TEMP_30
/* Set once Scoop has dug up the tear stones' room's buried Tear Stone n (0 or
 */
/* 1), under treasure actor n. */
#define FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(n) (FLAG_AREA_TEMP_00 + (n))
/* Set once Flower has been blown off its ledge into its own room. */
#define FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE FLAG_AREA_TEMP_10
/* Set once Tremor has dried the lava stream north of the pillar into a */
/* passage, and cooled the pool below. */
#define FLAG_TAOPO_SWAMP_PASSAGE_OPEN FLAG_AREA_TEMP_30
/* Set once Lash has tied the rope between the drips room's two posts. */
#define FLAG_TAOPO_SWAMP_ROPE_TIED FLAG_AREA_TEMP_08
/* Each glowing rock's flags: _COOLED once Douse has cooled it, _UNMOVABLE */
/* while Move cannot push it (until it is cooled, and once it has sunk), _SUNK
 */
/* once it has sunk into the lava as a stepping stone. */
#define FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED	FLAG_AREA_TEMP_31
#define FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE FLAG_AREA_TEMP_32
#define FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_SUNK		FLAG_AREA_TEMP_33
#define FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED			FLAG_AREA_TEMP_34
#define FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE		FLAG_AREA_TEMP_35
#define FLAG_TAOPO_SWAMP_PILLAR_ROCK_SUNK			FLAG_AREA_TEMP_36
#define FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED			FLAG_AREA_TEMP_38
#define FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE		FLAG_AREA_TEMP_39
#define FLAG_TAOPO_SWAMP_CHEST_ROCK_SUNK			FLAG_AREA_TEMP_3A
#define FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED			FLAG_AREA_TEMP_3B
#define FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE		FLAG_AREA_TEMP_3C
#define FLAG_TAOPO_SWAMP_EXIT_ROCK_SUNK				FLAG_AREA_TEMP_3D
#define FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED			FLAG_AREA_TEMP_40
#define FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE		FLAG_AREA_TEMP_41
#define FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED			FLAG_AREA_TEMP_42
#define FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE		FLAG_AREA_TEMP_43

void syGameFlagSetByte(s32 flag, s32 value);
void scVentSetGust(s32 index, FIXED speed);
void scVentSetId(s32 slot, s32 id);

extern s32 TaopoSwamp_SteamPuffs;
extern s32 TaopoSwamp_PassageFrames;
extern s32 TaopoSwamp_PoolFrames;
extern u32 TaopoSwamp_PuffSteamScript[];
void scTaopoSwampDarkenRoom(s32 fade);
/* Shows message in an event; when someone in the party has Scoop, the field */
/* menu then opens (fdEventShowMessageAndOfferScoop, through the overlay's own
 */
/* import row). */
void scTaopoSwampShowMessage(s32 message);
s32 scVentGetCounter(void);
#ifdef MODERN
void scVentSetBlowing(s32 index, s32 blowing);
#endif
s32 scVentAdvanceCounter(s32 limit);

/* Where Felix stands in the swamp outside (SCENE_STATE.footing). */
typedef enum MUD_FOOTING {
	FOOTING_FIRM_GROUND = 0,
	FOOTING_MUD = 1,   /* sinking a little with each step */
	FOOTING_BUBBLE = 2 /* lifted out of the mud by a bubble */
} MUD_FOOTING;

/* SCENE_STATE.depth for one row of the leader sunk. */
#define MUD_DEPTH_ROW 256

/* The swamp's state outside: where Felix stands, how deep he has sunk */
/* (MUD_DEPTH_ROW to a row of the sprite) and the middle of the last firm cell
 */
/* he stood on. */
typedef struct SCENE_STATE {
	s16 footing; /* MUD_FOOTING */
	s16 depth;
	FIXED x;
	FIXED z;
} SCENE_STATE;

/* The scene's view of SceneStateWork (system/globals.h). */
#define SceneStateWork (*(SCENE_STATE *)SceneStateWork)

/* The colour of the sprite palette the mud's splashes are drawn in */
/* (SPRITE_COLOR_FILL reads it from DecodeFillByte). */
#define MUD_COLOR 93

/* How many rows the leader sinks into the mud before it pulls them back out, */
/* and how many the stump sinks before it is gone. */
#define LEADER_SINK_ROWS 20
#define STUMP_SINK_ROWS	 24
/* Each frame the leader walks on the mud sinks them by this share of the full
 */
/* depth, and 1 more. */
#define MUD_SINK_FRAMES 96

/* The vents' gusts: each vent blows south along a column of cells whose step */
/* trigger is TRIGGER_FIRST_GUST plus its place in its room's list */
/* (TaopoSwampVentsGustVents, TaopoSwampPillarGapGustVents), and walking */
/* south off the end of one of the vents room's first seven, or the round */
/* spot's, drops the leader to the room below by the exit of the same */
/* number. */
enum {
	TRIGGER_FIRST_GUST = 10,
	/* Past the pool, the gust's cell beyond the round spot, and its cells */
	/* from the vent to the spot, which an ice pillar on the spot stops. */
	TRIGGER_PAST_SPOT_GUST = TRIGGER_FIRST_GUST,
	TRIGGER_SPOT_GUST = TRIGGER_FIRST_GUST + 2,
	/* In the vents room, the gusts of the vents the east and west pillars can
	 */
	/* be moved over. */
	TRIGGER_EAST_PILLAR_GUST = TRIGGER_FIRST_GUST + 2,
	TRIGGER_WEST_PILLAR_GUST = TRIGGER_FIRST_GUST + 8,
	/* Raised while the vent beside the covered one blows on Flower's ledge. */
	TRIGGER_FLOWER_IN_GUST = 100
};

/* The vents room's vents beside the two its pillars can cover, as places in */
/* TaopoSwampVentsVents: covering one makes the one beside it blow twice as */
/* far, and the one beside the east pillar's then reaches Flower's ledge. */
enum { VENT_BESIDE_WEST_PILLAR = 2, VENT_BESIDE_EAST_PILLAR = 8 };

void scVentBounce(ACTOR *actor);
extern u32 Dust_BurstScript[];
extern s32 TaopoSwamp_SparkleFrame;
void scTaopoSwampSparkleStarDust(void);
void scTaopoSwampFocusMapOn(s32 x, s32 z);
void scTaopoSwampSaveFirmGround(void);
void scTaopoSwampFillDripSpot(s32 caster, s32 id);
extern s32 TaopoSwamp_DripFromCeilingScript[];
extern s32 TaopoSwamp_DropFalls[];
void scTaopoSwampShatterIcePillar(s32 id);
void scTaopoSwampBlowAwayPuddle(s32 id);
extern u32 TaopoSwamp_SparkFades[];
extern s32 TaopoSwamp_DripAtRandomScript[];
extern u8 MsgIThoughtI;
void scTaopoSwampPuffSteam(void);
void scTaopoSwampOpenPassage(void);
void scTaopoSwampFlickerPool(void);

/* Copies what cells do and their flags, as the frozen spots do */
/* (fdMapGridCopyCellAttributes). */
static __inline__ void scTaopoSwampCopyCellAttributes(s32 sourceX, s32 sourceZ,
													  s32 width, s32 height,
													  s32 x, s32 z) {
	fdMapGridCopyCellAttributes(sourceX, sourceZ, width, height, x, z);
}

/* Actor id jumps down off the ledge before it (fdActorJumpDownLedge). */
static __inline__ void scTaopoSwampJumpDownLedge(s32 id, s32 pose, s32 param) {
	fdActorJumpDownLedge(id, pose, param);
}

/* Copies the map cells the leader's Psynergy touches */
/* (fdTileCopiesCopyTouchedCells). */
static __inline__ void scTaopoSwampCopyTouchedCells(s32 sourceX, s32 sourceZ,
													s32 width, s32 height,
													s32 x, s32 z, s32 layer) {
	fdTileCopiesCopyTouchedCells(sourceX, sourceZ, width, height, x, z, layer);
}

/* Cyclone scatters the Flower room's weeds where it touches them: the weeds' */
/* 14 by 8 cells from (14, 37) take the cells of the bare block at (76, 70). */
#define scTaopoSwampScatterWeedCells()                                         \
	scTaopoSwampCopyTouchedCells(76, 70, 14, 8, 14, 37, 0)

/* Creates an actor of sprite type at x, y, z. */
static __inline__ ACTOR *scTaopoSwampCreateActor(s32 type, s32 x, s32 y,
												 s32 z) {
	return syImportActorCreate(type, x, y, z);
}

/* The Mad Plant leaps out of the weeds at the leader (fdMadPlantLeapAtLeader).
 */
static __inline__ void scTaopoSwampLeapAtLeader(s32 sprite, s32 x, s32 z) {
	fdMadPlantLeapAtLeader(sprite, x, z);
}

s32 scVentIsBlowing(s32 index);

void scTaopoSwampFocusMapMiddle();
void scTaopoSwampJumpForward(void);

void scTaopoSwampGrowVine(void);

void scTaopoSwampUncoverBrambleSeed(void);

void scTaopoSwampStartMadPlantBattle(void);

void scTaopoSwampTremorPillar(void);

void scTaopoSwampSinkInMud(void);

void scTaopoSwampRiseOnBubble(void);

void scTaopoSwampStepOnFirmGround(void);

/* Talking to Flower, the Venus Djinni, starts its battle. */
void scTaopoSwampFightFlower(void) {
	fdRequestDjinnBattle(ACTOR_FLOWER, DJINNI_FLOWER);
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE TaopoSwamp_Placements[];
extern ACTOR_PLACE TaopoSwampMudActorPlacements[];
extern ACTOR_PLACE TaopoSwampFlowerActorPlacements[];
extern ACTOR_PLACE TaopoSwampVentsActorPlacements[];
extern ACTOR_PLACE TaopoSwampPillarGapActorPlacements[];
extern ACTOR_PLACE TaopoSwampDripsActorPlacements[];
extern ACTOR_PLACE TaopoSwampTremorActorPlacements[];
extern ACTOR_PLACE TaopoSwampLavaRocksActorPlacements[];
extern ACTOR_PLACE TaopoSwampStarDustActorPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_TAOPO_SWAMP_MUD)
		return TaopoSwampMudActorPlacements;
	if(scene == SCENE_TAOPO_SWAMP_FLOWER)
		return TaopoSwampFlowerActorPlacements;
	if(scene == SCENE_TAOPO_SWAMP_VENTS)
		return TaopoSwampVentsActorPlacements;
	if(scene == SCENE_TAOPO_SWAMP_PILLAR_GAP)
		return TaopoSwampPillarGapActorPlacements;
	if(scene == SCENE_TAOPO_SWAMP_DRIPS)
		return TaopoSwampDripsActorPlacements;
	if(scene == SCENE_TAOPO_SWAMP_LAVA_ROCKS)
		return TaopoSwampLavaRocksActorPlacements;
	if(scene == SCENE_TAOPO_SWAMP_TREMOR)
		return TaopoSwampTremorActorPlacements;
	if(scene == SCENE_TAOPO_SWAMP_STAR_DUST)
		return TaopoSwampStarDustActorPlacements;
	return TaopoSwamp_Placements;
}

/* Whirlwind on one of the five bushes of weeds at the foot of the swamp's */
/* cliffs blows it away for good. Three hide cave mouths: the westernmost, at */
/* cell (13, 8), the way to the first Tear Stone; the one at cell (37, 11), the
 */
/* way to the pool; and the one at cell (44, 16), the stairs down to the vents
 */
/* room. The two just east of the first, at (16, 7) and (18, 7), hide nothing.
 */
/* The bush's flag, FLAG_TAOPO_SWAMP_BUSH_BLOWN plus its actor, keeps it gone */
/* and opens the exit behind it. */
void scTaopoSwampBlowAwayBush(s32 unused, s32 id) {
	syGameFlagSet(FLAG_TAOPO_SWAMP_BUSH_BLOWN + id);
}

/* Growth on the sprout beside the eastern cave, cell (47, 17), grows it into */
/* a vine up the cliff, which Felix can then climb. Its two cells take the */
/* climbable cells kept at (50, 16). */
void scTaopoSwampGrowVine(void) {
	fdMapGridCopyCellAttributes(50, 16, 1, 2, 47, 16);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_VINE_GROWN);
}

struct CHILD_GROUP;

extern u32 TaopoSwamp_SplashFades[];

/* Move pulling the stump at the east end of the broken plank bridge west into
 */
/* the gap at cell (12, 16) drops it into the mud there: it slowly sinks, */
/* throwing up mud, two rings spread where it went under, and it stays gone, */
/* so Felix can hop across the gap. It sinks a row of its picture every three */
/* frames, and FLAG_TAOPO_SWAMP_STUMP_SUNK keeps it from being placed again. */
void scTaopoSwampSinkStump(s32 unused, s32 id) {
	ACTOR *actor = fdActorGetById(id);
	ACTOR *splash, *spark;
	FIELD_SPR *sprite, *splashSprite, *sparkSprite;
	s32 i;
	FIXED x, z;
	s32 k;

	if(FIELD_CELL(actor->x.fixed) != 12)
		return;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syWait(5);
	actor->motionFlg = 0;
	actor->velY = 0;
	actor->y.fixed = toFIXED(4);
	sprite = actor->spr;
	for(i = 0; i <= STUMP_SINK_ROWS; i++) {
		((SPR *)sprite)->sunkRows = i;
		sprite->flg |= SPRITE_FLAG_SUNK;
		sprite->flg &= ~SPRITE_FLAG_SHADOW;
		k = i;
		sprite->dirty = 1;
		k &= 3;
		if(k == 0) {
			x = actor->x.fixed + syRandom() * 2 - syRandom() * 2;
			z = actor->z.fixed + syRandom() * 2 - syRandom() * 2;
			splash = (scFieldActorCreate(SPRITE_WADING_SPLASH, x,
										 actor->y.fixed, z));
			if(splash != 0) {
				splashSprite = splash->spr;
				splash->groundHeight = actor->groundHeight;
				fdActorStartScript(splash, TaopoSwamp_SplashFades);
				splash->motionFlg = 0;
				if(splashSprite != 0) {
					fdSprStartAnimation(splashSprite, 1);
					splashSprite->flg = k;
					splashSprite->pri = sprite->pri;
					fdSprSetAnimationColModes(splashSprite, SPRITE_COLOR_FILL);
					fdSprSetAnimationSpeed(splashSprite, ANIM_STEP_NORMAL / 2);
				}
			}
		}
		syWait(3);
	}

	actor->sprStorageKind = 0;
	actor->y.fixed = 0;
	for(i = 0; i < 2; i++) {
		spark = (scFieldActorCreate(SPRITE_SPLASH_RING, actor->x.fixed,
									actor->y.fixed, actor->z.fixed));
		if(spark != 0) {
			sparkSprite = spark->spr;
			fdActorStartScript(spark, TaopoSwamp_SplashFades);
			spark->motionFlg = 0;
			spark->layer = 1;
			if(sparkSprite != 0) {
				fdSprStartAnimation(sparkSprite, 2);
				sparkSprite->flg = 0;
				sparkSprite->pri = 2;
			}
		}
		syWait(15);
	}
	syFieldEventWait(30);
	actor->sprStorageKind = 1;
	fdActorSetPos(id, 0, 0);
	fdEventEnd();
	fdEventSetFlag(FLAG_TAOPO_SWAMP_STUMP_SUNK);
}

/* Scoop on the ring of small stones in the middle of the cave at the end of */
/* the passage north of the vents room, cell (25, 43), digs up a Tear Stone. */
void scTaopoSwampScoopSecondTearStone(s32 mode) {
	fdEventDigUpTreasure(mode, FIELD_TREASURE_ACTOR + 1,
						 FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(1));
}

/* Scoop on the ring of small stones in the middle of the small cave behind */
/* the westernmost clump of weeds, cell (25, 7), digs up a Tear Stone. */
void scTaopoSwampScoopFirstTearStone(s32 mode) {
	fdEventDigUpTreasure(mode, FIELD_TREASURE_ACTOR,
						 FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(0));
}

/* Cyclone on the green weeds dotting the west of the room below the vents */
/* scatters their leaves and clears the weeds it touched. */
void scTaopoSwampScatterWeeds(void) { scTaopoSwampScatterWeedCells(); }

/* Cyclone on the weeds at cell (16, 41), in the room below the vents, scatters
 */
/* them and uncovers the Bramble Seed there, until it has been taken. Felix, */
/* if he stands on that cell, hops a step north off it. */
void scTaopoSwampUncoverBrambleSeed(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	scTaopoSwampScatterWeedCells();
	if(fdEventGetFlag(FLAG_TAOPO_SWAMP_BRAMBLE_SEED_TAKEN) == 0) {
		fdActorSetPos(FIELD_TREASURE_ACTOR, BRAMBLE_SEED_X, BRAMBLE_SEED_Z);
		if(FIELD_CELL(leader->x.fixed) == FIELD_CELL(toFIXED(BRAMBLE_SEED_X)) &&
		   FIELD_CELL(leader->z.fixed) == FIELD_CELL(toFIXED(BRAMBLE_SEED_Z))) {
			fdEventStart();
			syFieldEventEndLastingPsynergy(0);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
									 ATTACHED_EFFECT_IN_FRONT |
										 ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0,
										-16);
			leader->velY = toFIXED(6);
			scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdEventEnd();
		}
	}
}

/* Cyclone on the weeds at cell (21, 40), in the room below the vents, scatters
 */
/* them, and until it is beaten a Mad Plant leaps out of them at Felix and the
 */
/* party must fight it, with no escape. */
void scTaopoSwampStartMadPlantBattle(void) {
	scTaopoSwampScatterWeedCells();
	if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_MAD_PLANT_BEATEN)) {
		scTaopoSwampLeapAtLeader(SPRITE_MAD_PLANT, 344, 648);
		MainRoutineStateWork.pendingDjinni =
			IF_FLAG_SET(FLAG_TAOPO_SWAMP_MAD_PLANT_BEATEN);
		btSetType(BATTLE_TYPE_NO_ESCAPE);
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_MAD_PLANTS, 1);
	}
}

/* Walking south off the ledge at the foot of a vent's gust, or blown off it, */
/* Felix springs down and lands in the room below: from the vents room's seven
 */
/* gusts (their ends from cell (12, 16) in the west to (39, 16) in the east) */
/* into the room of the weeds below the vents, and from the round spot's gust,
 */
/* cell (26, 10), into the drips room. Each gust's cells and the exit it */
/* leaves by share their number. */
void scTaopoSwampDropToLowerFloor(s32 exit) {
	scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 3);
	scTaopoSwampJumpDownLedge(MainRoutineStateWork.leaderActorId,
							  ANIM_LEADER_SPRING, LEDGE_JUMP_NO_LANDING_WAIT);
	fdEventPlaySnd(SE_FALL);
	fdEventSetFlag(FLAG_ARRIVE_FALLING);
	syFieldEventSetExitRequest(exit);
}

/* Move pushing one of the vents room's two stone pillars in front of the vent
 */
/* beside it, the west one to cell (17, 14) or the east one to cell (29, 15), */
/* blocks that vent's gust and makes the next vent blow twice as hard; from */
/* the east pillar's, the gust then reaches Flower's ledge. The pillar's flag */
/* keeps it there. */
void scTaopoSwampPushPillarOverVent(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(id == ACTOR_WEST_VENT_PILLAR) {
		if(FIELD_CELL(actor->x.fixed) == 17) {
			actor->z.fixed += toFIXED(1);
			scVentSetGust(VENT_BESIDE_WEST_PILLAR, toFIXED(8));
			fdEventSetFlag(FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT);
		}
	}
	else if(id == ACTOR_EAST_VENT_PILLAR) {
		if(FIELD_CELL(actor->x.fixed) == 29) {
			actor->z.fixed += toFIXED(1);
			scVentSetGust(VENT_BESIDE_EAST_PILLAR, toFIXED(8));
			fdEventSetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT);
		}
	}
}

extern u8 TaopoSwampVentsGustVents[];
extern u8 TaopoSwampPillarGapGustVents[];

/* Stepping into the gust in front of a vent while it puffs (in the vents room,
 */
/* the round spot's room, the drips room or the lava rocks' room), Felix is */
/* blown south, spinning, until a wall stops him or off the ledge into the */
/* room below. An ice pillar on a round spot, or a pillar Moved before a vent,
 */
/* stops the gust. */
void scTaopoSwampStepIntoGust(s32 gust) {
	ACTOR *leader;
	VECTOR3 position;
	s32 found, result, slip;

	if(MainRoutineStateWork.next == SCENE_TAOPO_SWAMP_VENTS) {
		found = scVentIsBlowing(
			TaopoSwampVentsGustVents[gust - TRIGGER_FIRST_GUST]);
		if(gust == TRIGGER_EAST_PILLAR_GUST &&
		   fdEventGetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT))
			return;
		if(gust == TRIGGER_WEST_PILLAR_GUST &&
		   fdEventGetFlag(FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT))
			return;
	}
	if(MainRoutineStateWork.next == SCENE_TAOPO_SWAMP_PILLAR_GAP) {
		found = scVentIsBlowing(
			TaopoSwampPillarGapGustVents[gust - TRIGGER_FIRST_GUST]);
		if(gust == TRIGGER_PAST_SPOT_GUST &&
		   fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
			return;
		if(gust == TRIGGER_SPOT_GUST &&
		   fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
			return;
	}
	if(MainRoutineStateWork.next == SCENE_TAOPO_SWAMP_DRIPS) {
		found = scVentIsBlowing(gust - TRIGGER_FIRST_GUST);
		if(gust == TRIGGER_FIRST_GUST &&
		   fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
			return;
	}
	if(MainRoutineStateWork.next == SCENE_TAOPO_SWAMP_LAVA_ROCKS)
		found = scVentIsBlowing(gust - TRIGGER_FIRST_GUST);
	if(found == 0)
		return;
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	/* The gust carries the leader south 2 pixels a frame, spinning, and draws
	 */
	/* them to the middle of their column of cells. */
	for(;;) {
		position.x = leader->x.fixed;
		position.y = leader->y.fixed;
		position.z = leader->z.fixed + toFIXED(8);
		result = fdMapGridCheckCollision(leader, &position);
		if(result > 0)
			break;
		if(result < 0) {
			leader->facing = FACING_SOUTH;
			scTaopoSwampDropToLowerFloor(gust);
			return;
		}
		leader->z.fixed += toFIXED(2);
		slip = (leader->x.fixed & ~FIELD_CELL_MASK) - toFIXED(8);
		if(slip > toFIXED(1))
			slip = toFIXED(1);
		if(slip < -toFIXED(1))
			slip = -toFIXED(1);
		leader->x.fixed -= slip;
		leader->facing += TRIG_TURN / 16;
		syWait(1);
	}
}

/* Once the east pillar covers its vent, the next gust of the vent beside it */
/* reaches Flower, the Venus Djinni, on its ledge at cell (25, 19): Felix turns
 */
/* to it with a start, the camera moves to Flower, and it hops three times and
 */
/* falls off the ledge into the room below, where it waits */
/* (FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE). */
void scTaopoSwampBlowFlowerOffLedge(void) {
	ACTOR *actor = fdActorGet(ACTOR_FLOWER);
	FIXED speed;
	s32 i;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_FLOWER, 0);
	scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdCameraMoveTo(408, CAMERA_KEEP, 312, 1);
	fdCameraWaitForArrival();
	fdActorSetAttachedEffect(ACTOR_FLOWER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	speed = toFIXED(1) / 2;
	for(i = 0; i < 3; i++) {
		actor->velY = toFIXED(2);
		fdEventPlaySnd(SE_JUMP);
		actor->speedLimit = speed;
		actor->acceleration = speed / 2;
		fdActorWalkByAndWait(ACTOR_FLOWER, 0, 8);
		speed += toFIXED(0.25);
	}
	fdEventPlaySnd(SE_FALL);
	actor->velY = toFIXED(4);
	fdActorWalkByAndWait(ACTOR_FLOWER, 0, 8);
	syFieldEventWait(60);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE);
	fdEventEnd();
}

#include "shared/actor_wait_to_land.h"

/* Move pushing the stone pillar on the ledge in the south room past the pool,
 */
/* cell (15, 26), east off it drops the pillar into the gap at cell (16, 26), */
/* and its top becomes a stepping stone Felix hops across the gap on */
/* (FLAG_TAOPO_SWAMP_PILLAR_IN_GAP). The gap's cell takes the hop of the */
/* pillar's old one. */
void scTaopoSwampDropPillarIntoGap(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(id == ACTOR_GAP_PILLAR && FIELD_CELL(actor->x.fixed) == 16) {
		fdActorWaitToLand(ACTOR_GAP_PILLAR);
		fdMapGridCopyCellAttributes(15, 26, 1, 1, 16, 26);
		fdEventSetFlag(FLAG_TAOPO_SWAMP_PILLAR_IN_GAP);
	}
}

/* Douse on the round spot at the dead end in the north room past the pool, */
/* cell (26, 8), in the gust of the vent above it, leaves a puddle there, */
/* which the next gust blows away unless Frost turns it to an ice pillar */
/* first. */
void scTaopoSwampDouseVentSpot(s32 unused, s32 id) {
	scFieldActorSetSprPriority(id, 3);
	fdActorGet(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetAnimation(id, 1);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
}

/* Frost on a round spot's puddle freezes it into a pillar of ice, which stops
 */
/* the gust, and Felix can hop onto it from the stepping stones beside it to */
/* cross the gust. The spot's cell (spotX, z) takes the hop of the stepping */
/* stone at (pillarX, z): (26, 8) past the pool, (25, 27) in the drips */
/* room. */
#define DEFINE_TAOPO_SWAMP_FREEZE_SPOT(name, pillarX, z, spotX)                \
	void name(s32 unused, s32 id) {                                            \
		scTaopoSwampCopyCellAttributes(pillarX, z, 1, 1, spotX, z);            \
		scFieldActorSetSprPriority(id, 2);                                     \
		fdActorGet(id)->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;                   \
		scVentSetId(0, id);                                                    \
		fdEventSetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN);                          \
		fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);                        \
	}

/* Frost on the puddle on the round spot past the pool, cell (26, 8). */
DEFINE_TAOPO_SWAMP_FREEZE_SPOT(TaopoSwamp_FreezeVentSpot, 28, 8, 26)

/* The next gust shatters the ice pillar on the round spot past the pool, and */
/* Felix, if he stands on it, hops off south and turns to face it. */
void scTaopoSwampShatterIcePillar(s32 id) {
	ACTOR *leader;
	ACTOR *actor;

	fdActorSetAnimation(id, 8);
	scFieldActorSetSprPriority(id, 3);
	fdActorGet(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(id)->update = 0;
	scVentSetId(0, 0);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor = fdActorGet(id);
	if(FIELD_CELL(actor->x.fixed) == FIELD_CELL(leader->x.fixed) &&
	   FIELD_CELL(actor->z.fixed) == FIELD_CELL(leader->z.fixed)) {
		fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
								 ATTACHED_EFFECT_IN_FRONT |
									 ATTACHED_EFFECT_ONCE);
		fdActorWalkByAndWait(MainRoutineStateWork.leaderActorId, 0, 16);
		syFieldEventWait(2);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
		syFieldEventWait(10);
	}
	fdMapGridCopyCellAttributes(22, 8, 1, 1, 26, 8);
}

/* The next gust blows the puddle off the round spot past the pool. */
void scTaopoSwampBlowAwayPuddle(s32 id) {
	fdActorSetAnimation(id, 7);
	scFieldActorSetSprPriority(id, 3);
	fdActorGet(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(id)->update = 0;
	scVentSetId(0, 0);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
}

/* A drop from the ceiling, or Douse, on the round spot in the drips room, cell
 */
/* (25, 27), shows a puddle there, which Frost can freeze once it has caught */
/* four drops. Douse alone does not do it with Felix leading: cast from beside
 */
/* the spot it rains on it and leaves the puddle, and Frost cast on that */
/* puddle then only scatters its snowflakes, the puddle staying as it was. */
/* Douse runs this with the caster's actor (fdPsynergyDouse passes */
/* FIELD_EFFECT_STATE.casterId) and a drop with 0, and of the casters only */
/* Jenna's actor counts. */
void scTaopoSwampFillDripSpot(s32 caster, s32 id) {
	scFieldActorSetSprPriority(id, 3);
	fdActorGet(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetAnimation(id, 1);
	if(caster == ACTOR_JENNA ||
	   scFieldGameFlagGetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT) > 3) {
		fdEventSetFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
		fdEventSetFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
	}
}

/* Frost on the puddle on the drips room's round spot, cell (25, 27). */
DEFINE_TAOPO_SWAMP_FREEZE_SPOT(TaopoSwamp_FreezeDripSpot, 23, 27, 25)

/* The next gust from the vent shatters the ice pillar on the drips room's */
/* round spot: the tall pillar Frost froze from the puddle there breaks into a
 */
/* lump of ice that is gone a moment later, leaving the spot bare for the */
/* drops to fill again, and the drops it has caught start again from none */
/* (FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT). The puddle freezes only once it has */
/* caught four drops or Douse was cast by a caster whose actor is 5 */
/* (scTaopoSwampFillDripSpot). The vents blow and the ceiling drips only when */
/* the room is entered by entrance 2, 4 or 9 (scInit). */
void scTaopoSwampShatterDripIcePillar(s32 id) {
	ACTOR *leader;
	ACTOR *actor;

	fdActorSetAnimation(id, 8);
	scFieldActorSetSprPriority(id, 3);
	fdActorGet(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(id)->update = 0;
	scVentSetId(0, 0);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
	syGameFlagSetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT, 0);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor = fdActorGet(id);
	if(FIELD_CELL(actor->x.fixed) == FIELD_CELL(leader->x.fixed) &&
	   FIELD_CELL(actor->z.fixed) == FIELD_CELL(leader->z.fixed)) {
		fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
								 ATTACHED_EFFECT_IN_FRONT |
									 ATTACHED_EFFECT_ONCE);
		fdActorWalkByAndWait(MainRoutineStateWork.leaderActorId, 0, 16);
		syFieldEventWait(2);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
		syFieldEventWait(10);
	}
	fdMapGridCopyCellAttributes(1, 1, 1, 1, 25, 27);
}

/* The next gust blows the puddle off the drips room's round spot, and the */
/* drops it has caught start again from none. */
void scTaopoSwampBlowAwayDripPuddle(s32 id) {
	fdActorSetAnimation(id, 7);
	scFieldActorSetSprPriority(id, 3);
	fdActorGet(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(id)->update = 0;
	scVentSetId(0, 0);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
	syGameFlagSetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT, 0);
}

/* Douse on the glowing red rock at the edge of the lava in the Tremor room's */
/* west, cell (8, 15), cools it, and Move can push it. */
void scTaopoSwampCoolLavaRock(s32 unused, s32 id) {
	fdActorSetAnimation(id, 2);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE);
}

/* Move pushing the cooled rock in the Tremor room's west into the lava at cell
 */
/* (7, 15) sinks it there as a stepping stone, which Felix hops across on to */
/* reach the bottom of the room. */
void scTaopoSwampPushLavaRockIn(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(FIELD_CELL(actor->x.fixed) == 7) {
		fdActorSetAnimation(id, 3);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdEventSetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE);
		fdEventSetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_SUNK);
		fdMapGridCopyCellAttributes(7, 17, 1, 1, 7, 15);
	}
}

/* Douse on the red rock Tremor knocked out of the pillar, where it landed west
 */
/* of it at cell (27, 14), cools it, and Move can push it. */
void scTaopoSwampCoolPillarRock(s32 unused, s32 id) {
	fdActorSetAnimation(id, 2);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE);
	fdMapGridCopyCellAttributes(27, 14, 1, 1, 27, 13);
}

/* Move pushing the cooled rock from the pillar south to cell (27, 16) sends it
 */
/* sliding two cells west, to cell (25, 16), where it sinks as a stepping */
/* stone. */
void scTaopoSwampPushPillarRockIn(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(FIELD_CELL(actor->z.fixed) == 16) {
		fdActorSetAnimation(id, 3);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdEventSetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE);
		fdEventSetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_SUNK);
		fdActorSetSpeed(id, 0.5);
		fdActorSetDestinationOffset(id, -32, 0);
		scFieldActorWaitForMoveById(id);
		fdActorSetAnimation(id, 3);
		fdMapGridCopyCellAttributes(25, 18, 1, 1, 25, 16);
	}
}

/* The update of the rock pile Tremor knocks off the pillar: it turns back a */
/* thirty-second of a turn each frame as it rolls. */
void scTaopoSwampSpinRockPile(ACTOR *actor) {
	((FIELD_SPR *)actor->spr)->rotation -= TRIG_TURN / 32;
}

/* A task: one puff of steam from the cooling pool each frame, until the count
 */
/* runs out. */
void scTaopoSwampPuffSteam(void) {
	s32 a;
	s32 x;
	s32 z;
	ACTOR *actor;

	TaopoSwamp_SteamPuffs--;
	a = syRandom() * 32;
	x = syRandom() * 16 + a + toFIXED(300);
	z = syRandom() * 16 + a + toFIXED(260);
	actor = scFieldActorCreate(SPRITE_DUST_CLOUD, x, 0, z);
	fdActorStartScript(actor, TaopoSwamp_PuffSteamScript);
	fdActorSetSprFlags(actor, 0);
	fdActorSetBlendMode(actor, 1);
	if(TaopoSwamp_SteamPuffs == 0)
		syFieldTaskRemove(scTaopoSwampPuffSteam);
}

/* A task: the lava stream north of the pillar dries into a passage, from its */
/* south end up, a row of cells every eight frames. */
void scTaopoSwampOpenPassage(void) {
	s32 timer = --TaopoSwamp_PassageFrames;
	s32 step = timer / 8;

	fdMapGridCopyCells(47 - (timer & 1) * 2, 64 - step, 2, step, 29, 15 - step);
	if(TaopoSwamp_PassageFrames == 0)
		syFieldTaskRemove(scTaopoSwampOpenPassage);
}

/* A task: the cooled pool's two pictures in turn, until the count runs out. */
void scTaopoSwampFlickerPool(void) {
	s32 n = --TaopoSwamp_PoolFrames;

	fdMapGridCopyCells(50, (n & 1) * 5 + 59, 13, 5, 18, 15);
	if(TaopoSwamp_PoolFrames == 0)
		syFieldTaskRemove(scTaopoSwampFlickerPool);
}

extern u32 Debris_Script[];

/* Tremor on the pillar with the huge rock pile on top, in the middle of the */
/* Tremor room: the pile rolls off and lands with a thump, a glowing red rock */
/* pops out of the pillar, clods burst out, the lava stream north of it dries */
/* into a passage from its south end up, and the camera turns to the lava pool
 */
/* to the south-west, which cools, steaming */
/* (FLAG_TAOPO_SWAMP_PASSAGE_OPEN). */
void scTaopoSwampTremorPillar(void) {
	ACTOR *rock = fdActorGet(ACTOR_TREMOR_ROCK_PILE);
	ACTOR *clod;
	s32 i, row;
	FIXED cosine;
	u32 angle, speed, scale;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	for(i = 0; i < 10; i++) {
		syWait(1);
		((FIELD_SPR *)rock->spr)->rotation -= TRIG_TURN / 256;
		rock->x.fixed -= syCos(((FIELD_SPR *)rock->spr)->rotation) / 2;
		rock->tgtX = ACTOR_NO_TARGET;
	}
	rock->update = scTaopoSwampSpinRockPile;
	fdActorSetSpeed(ACTOR_TREMOR_ROCK_PILE, 2);
	fdActorSetDestination(ACTOR_TREMOR_ROCK_PILE, 492, 246);
	rock->gravity = toFIXED(4) / 5;
	rock->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	rock->layer = 0;
	scFieldActorWaitForMoveById(ACTOR_TREMOR_ROCK_PILE);
	rock->velY = 0;
	rock->y.fixed = rock->groundHeight;
	fdActorSetPos(ACTOR_TREMOR_ROCK_PILE, 480, 248);
	((FIELD_SPR *)rock->spr)->rotation = 0;
	fdEventPlaySnd(SE_THUMP);
	fdMapGridSetShake(5, 5, 1);
	fdEventPlaySnd(SE_RUMBLE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdActorSetPos(ACTOR_TREMOR_PILLAR_ROCK, 480, 248);
	fdActorSetDestination(ACTOR_TREMOR_PILLAR_ROCK, 440, 232);
	fdActorGet(ACTOR_TREMOR_PILLAR_ROCK)->velY = toFIXED(10);
	rock->update = 0;
	for(i = 0; i < TREMOR_CLODS; i++) {
		clod = (scFieldActorCreate(SPRITE_ROCK, rock->x.fixed,
								   rock->y.fixed + toFIXED(16), rock->z.fixed));
		if(clod != 0) {

			angle = syRandom();
			speed = syRandom();
			scale = (syRandom() >> 2) + FIXED_ONE / 16;
			fdActorStartScript(clod, Debris_Script);
			fdActorSetSprFlags(clod, 0);
			clod->velY = (angle << 2) + toFIXED(2);
			cosine = syCos(angle);
			speed <<= 3;
			clod->velZ = syMulFX(speed, cosine);
			clod->velX = syMulFX(speed, syMathSin(angle));
			clod->acceleration = 0;
			clod->gravity = toFIXED(0.33);
			clod->scaleX = scale;
			clod->scaleY = scale;
			clod->bounce = FIXED_ONE / 2;
		}
	}
	syFieldMapGridWaitForShakeToSettle();
	syTaskAddFrame(scTaopoSwampOpenPassage, TASK_PASS_FRAME);
	syWait(40);
	fdCameraSetSpeed(0.9);
	fdCameraMoveTo(336, CAMERA_KEEP, 280, 1);
	i = POOL_COOLING_PICTURES;
	row = 114;
	do {
		fdMapGridCopyCells(50, row, 13, 5, 18, 15);
		syWait(8);
		row -= 5;
	} while(--i > 0);
	syTaskAddFrame(scTaopoSwampPuffSteam, TASK_PASS_FRAME);
	syWait(60);
	syTaskAddFrame(scTaopoSwampFlickerPool, TASK_PASS_FRAME);
	syWait(24);
	syFieldEventWait(120);
	fdEventEnd();
	fdEventSetFlag(FLAG_TAOPO_SWAMP_PASSAGE_OPEN);
}

/* Douse on the glowing red rock in the lava rocks' room's lower left corner, */
/* cell (11, 22), on the way to the Vial's chest, cools it, and Move can push */
/* it. */
void scTaopoSwampCoolChestRock(s32 unused, s32 id) {
	fdActorSetAnimation(id, 2);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE);
}

/* Move pushing the cooled rock in the lava rocks' room's lower left corner */
/* north into the lava at cell (11, 21) sinks it there as a stepping stone on */
/* the way to the Vial's chest. */
void scTaopoSwampPushChestRockIn(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(FIELD_CELL(actor->z.fixed) == 21) {
		fdActorSetAnimation(id, 3);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdEventSetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE);
		fdEventSetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_SUNK);
		fdMapGridCopyCellAttributes(11, 19, 1, 1, 11, 21);
	}
}

/* Douse on the glowing red rock in the lava rocks' room's upper right corner,
 */
/* cell (32, 13), by the way back to the Tremor room, cools it, and Move can */
/* push it. */
void scTaopoSwampCoolExitRock(s32 unused, s32 id) {
	fdActorSetAnimation(id, 2);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE);
}

/* Move pushing the cooled rock in the lava rocks' room's upper right corner */
/* west into the lava stream at cell (31, 13) sends it sliding two cells north
 */
/* to cell (31, 11), where it sinks as a stepping stone across the stream, the
 */
/* way back to the Tremor room. */
void scTaopoSwampPushExitRockIn(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(FIELD_CELL(actor->x.fixed) == 31) {
		fdActorSetAnimation(id, 3);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdEventSetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE);
		fdEventSetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_SUNK);
		fdActorSetSpeed(id, 0.5);
		fdActorSetDestinationOffset(id, 0, -32);
		scFieldActorWaitForMoveById(id);
		fdActorSetAnimation(id, 3);
		fdMapGridCopyCellAttributes(30, 13, 1, 1, 31, 11);
	}
}

#if TLA_PATCH_1 /* Absent from the Japanese edition. */
/* Walking east from cell (9, 21) in the lava rocks' room, Felix jumps two */
/* cells over the lava onto the rock sunk at cell (11, 21), once it is there. */
/* The Japanese release has no such trigger. */
void scTaopoSwampJumpForward(void) { fdTryJumpForward(); }
#endif

/* Douse on the glowing rock lighting the first dark room below the Tremor */
/* room, cell (7, 39), puts its light out, the room going dark, and Move can */
/* push it aside. */
void scTaopoSwampCoolDarkRock(s32 unused, s32 id) {
	fdActorSetAnimation(id, 2);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE);
	fdMapGridCopyCellAttributes(7, 39, 1, 1, 7, 38);
	scTaopoSwampDarkenRoom(1);
}

/* Douse on the glowing rock in the last chamber, cell (32, 39), puts its */
/* light out, and in the dark a light shows on the ground where the Star Dust */
/* is buried. */
void scTaopoSwampCoolLastRock(s32 unused, s32 id) {
	fdActorSetAnimation(id, 2);
	fdEventSetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED);
	fdEventClearFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE);
	scTaopoSwampDarkenRoom(1);
}

/* Scoop on the light on the ground in the last chamber, cell (27, 37), digs */
/* up the Star Dust (FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP). */
void scTaopoSwampScoopStarDust(s32 mode) {
	fdEventDigUpTreasure(mode, FIELD_TREASURE_ACTOR,
						 FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP);
}

/* Searching the ground at the Star Dust's spot in the last chamber, cell */
/* (27, 37), before it is dug up: "I thought I saw something sparkling in the */
/* ground!" */
void scTaopoSwampShowSparkleMessage(void) {
	scTaopoSwampShowMessage((s32)&MsgIThoughtI);
}

extern TRIGGER TaopoSwamp_Triggers[];
extern TRIGGER TaopoSwampEntranceTriggers[];
extern TRIGGER TaopoSwampMudTriggers[];
extern TRIGGER TaopoSwampTearStonesTriggers[];
extern TRIGGER TaopoSwampFlowerTriggers[];
extern TRIGGER TaopoSwampVentsTriggers[];
extern TRIGGER TaopoSwampPoolTriggers[];
extern TRIGGER TaopoSwampPillarGapTriggers[];
extern TRIGGER TaopoSwampDripsTriggers[];
extern TRIGGER TaopoSwampTremorTriggers[];
extern TRIGGER TaopoSwampLavaRocksTriggers[];
extern TRIGGER TaopoSwampStarDustTriggers[];

const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_TAOPO_SWAMP_ENTRANCE)
		return (const TRIGGER *)TaopoSwampEntranceTriggers;
	if(scene == SCENE_TAOPO_SWAMP_MUD)
		return (const TRIGGER *)TaopoSwampMudTriggers;
	if(scene == SCENE_TAOPO_SWAMP_TEAR_STONES)
		return (const TRIGGER *)TaopoSwampTearStonesTriggers;
	if(scene == SCENE_TAOPO_SWAMP_FLOWER)
		return (const TRIGGER *)TaopoSwampFlowerTriggers;
	if(scene == SCENE_TAOPO_SWAMP_VENTS)
		return (const TRIGGER *)TaopoSwampVentsTriggers;
	if(scene == SCENE_TAOPO_SWAMP_POOL)
		return (const TRIGGER *)TaopoSwampPoolTriggers;
	if(scene == SCENE_TAOPO_SWAMP_PILLAR_GAP)
		return (const TRIGGER *)TaopoSwampPillarGapTriggers;
	if(scene == SCENE_TAOPO_SWAMP_DRIPS)
		return (const TRIGGER *)TaopoSwampDripsTriggers;
	if(scene == SCENE_TAOPO_SWAMP_LAVA_ROCKS)
		return TaopoSwampLavaRocksTriggers;
	if(scene == SCENE_TAOPO_SWAMP_TREMOR)
		return (const TRIGGER *)TaopoSwampTremorTriggers;
	if(scene == SCENE_TAOPO_SWAMP_STAR_DUST)
		return (const TRIGGER *)TaopoSwampStarDustTriggers;
	return (const TRIGGER *)TaopoSwamp_Triggers;
}

/* A falling drop splashes where it lands. */
s32 scTaopoSwampLandDrop(ACTOR *actor) {
	FIXED y = actor->y.fixed;
	ACTOR *spark;
	FIXED scale;

	if(y + actor->velY < actor->groundHeight) {
		spark = scTaopoSwampCreateActor(SPRITE_SPLASH_RING, actor->x.fixed, y,
										actor->z.fixed);
		scale = FIXED_ONE * 55 / 100;
		if(spark) {
			spark->scaleX = scale;
			spark->scaleY = scale;
			fdActorStartScript(spark, TaopoSwamp_SparkFades);
			fdActorSetSprFlags(spark, 0);
			fdActorSelectAnimation(spark, 2);
		}
		fdEventPlaySnd(SE_DRIP);
		actor->y.fixed = actor->groundHeight;
		actor->velY = 0;
		return 0;
	}
	return 1;
}

/* A task: on one in four of every eighth frame, a drop falls into the */
/* swamp's water at a random place near its east end. */
void scTaopoSwampDripAtRandom(void) {
	if((FrameCount & 7) == 0 && (u32)syRandom() < 16384) {
		s32 x = (syRandom() << 5) + toFIXED(520);
		s32 z = toFIXED(176) - (syRandom() << 3);
		FIXED y = toFIXED(160);
		ACTOR *actor = scFieldActorCreate(SPRITE_DROPLET, x, y, z);
		FIXED scale = FIXED_ONE * 33 / 100;

		if(actor != NULL) {
			actor->scaleX = scale;
			actor->scaleY = scale;
			actor->gravity = toFIXED(0.45);
			fdActorStartScript(actor, TaopoSwamp_DripAtRandomScript);
			fdActorSetSprFlags(actor, 0);
			fdActorSetSprPriorityDirect(actor, 3);
		}
	}
}

/* The vents' cycle in the vents room, the round spot's room and the drips */
/* room. */
#define VENT_CYCLE_FRAMES 480
/* The lava rocks' room's vents blow for ROCK_ROOM_VENTS_BLOW_FRAMES of every */
/* ROCK_ROOM_VENT_CYCLE_FRAMES. */
#define ROCK_ROOM_VENTS_BLOW_FRAMES 180
#define ROCK_ROOM_VENT_CYCLE_FRAMES 240

/* The vents room's cycle of VENT_CYCLE_FRAMES: two sets of vents blow in turn,
 */
/* and while the second blows with the east pillar over its vent, Flower is */
/* blown off its ledge. */
void scTaopoSwampCycleDjinniVents(void) {
	s32 timer = scVentGetCounter();

	switch(timer) {
	case 0:
		scVentSetBlowing(0, 1);
		scVentSetBlowing(1, 1);
		scVentSetBlowing(4, 1);
		scVentSetBlowing(5, 1);
		scQueueMusic(SE_AMBIENT_WATER);
		break;
	case 180:
		scVentSetBlowing(0, 0);
		scVentSetBlowing(1, 0);
		scVentSetBlowing(4, 0);
		scVentSetBlowing(5, 0);
		scQueueMusic(-1);
		break;
	case 240:
		scVentSetBlowing(2, 1);
		scVentSetBlowing(3, 1);
		scVentSetBlowing(6, 1);
		scVentSetBlowing(7, 1);
		scVentSetBlowing(8, 1);
		scVentSetBlowing(9, 1);
		scQueueMusic(SE_AMBIENT_WATER);
		break;
	case 420:
		scVentSetBlowing(2, 0);
		scVentSetBlowing(3, 0);
		scVentSetBlowing(6, 0);
		scVentSetBlowing(7, 0);
		scVentSetBlowing(8, 0);
		scVentSetBlowing(9, 0);
		scQueueMusic(-1);
		break;
	}
	/* Frames 251 to 429: from 11 frames after the second set starts blowing to
	 */
	/* 9 after it stops. */
	if((u32)(timer - 251) <= 178 &&
	   fdEventGetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT) &&
	   !fdEventGetFlag(FLAG_DJINNI(DJINNI_FLOWER)) &&
	   !fdEventGetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
		FieldStateWork->raisedTrigger = TRIGGER_FLOWER_IN_GUST;
	scVentAdvanceCounter(VENT_CYCLE_FRAMES);
}

/* The cycle of the vents by the round spot past the pool: they blow for 120 */
/* frames in VENT_CYCLE_FRAMES, and while they blow they break the spot's ice */
/* pillar or blow its puddle away. */
void scTaopoSwampCycleSpotVent(void) {
	s32 timer = scVentGetCounter();

	switch(timer) {
	case 0:
		scVentSetBlowing(0, 1);
		scVentSetBlowing(1, 1);
		scQueueMusic(SE_AMBIENT_WATER);
		break;
	case 120:
		scVentSetBlowing(0, 0);
		scVentSetBlowing(1, 0);
		scQueueMusic(-1);
		break;
	}
	if((u32)(timer - 11) <= 118) {
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
			scTaopoSwampShatterIcePillar(ACTOR_GAP_PUDDLE);
		else if(fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
			scTaopoSwampBlowAwayPuddle(ACTOR_GAP_PUDDLE);
	}
	scVentAdvanceCounter(VENT_CYCLE_FRAMES);
}

/* As scTaopoSwampCycleSpotVent, for the spot the ceiling drips onto. */
void scTaopoSwampCycleDripVent(void) {
	s32 timer = scVentGetCounter();

	switch(timer) {
	case 0:
		scVentSetBlowing(0, 1);
		scVentSetBlowing(1, 1);
		scQueueMusic(SE_AMBIENT_WATER);
		break;
	case 120:
		scVentSetBlowing(0, 0);
		scVentSetBlowing(1, 0);
		scQueueMusic(-1);
		break;
	}
	if((u32)(timer - 11) <= 118) {
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
			scTaopoSwampShatterDripIcePillar(ACTOR_DRIP_PUDDLE);
		else if(fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
			scTaopoSwampBlowAwayDripPuddle(ACTOR_DRIP_PUDDLE);
	}
	scVentAdvanceCounter(VENT_CYCLE_FRAMES);
}

/* The lava rocks' room's vents' cycle. */
void scTaopoSwampCycleRockRoomVents(void) {
	switch(scVentGetCounter()) {
	case 0:
		scVentSetBlowing(0, 1);
		scVentSetBlowing(1, 1);
		scQueueMusic(SE_AMBIENT_WATER);
		break;
	case ROCK_ROOM_VENTS_BLOW_FRAMES:
		scVentSetBlowing(0, 0);
		scVentSetBlowing(1, 0);
		scQueueMusic(-1);
		break;
	}
	scVentAdvanceCounter(ROCK_ROOM_VENT_CYCLE_FRAMES);
}

/* A drop from the ceiling lands: it splashes and grows the puddle below by */
/* one, up to four. */
s32 scTaopoSwampLandDrip(ACTOR *actor) {
	s32 flag;
	s32 count;
	ACTOR *spark;
	FIXED scale;

	if(actor->y.fixed + actor->velY < actor->groundHeight) {
		flag = fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN);
		if(flag) {
			fdEventPlaySnd(SE_DRIP);
			return 0;
		}
		spark = scTaopoSwampCreateActor(SPRITE_SPLASH_RING, actor->x.fixed,
										actor->y.fixed, actor->z.fixed);
		scale = FIXED_ONE * 55 / 100;
		if(spark) {
			spark->scaleX = scale;
			spark->scaleY = scale;
			fdActorStartScript(spark, TaopoSwamp_SparkFades);
			fdActorSetSprFlags(spark, 0);
			fdActorSelectAnimation(spark, 2);
		}
		fdEventPlaySnd(SE_DRIP);
		actor->y.fixed = actor->groundHeight;
		actor->velY = 0;
		if(fdEventGetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
			count = scFieldGameFlagGetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT);
			if(count <= 3) {
				count++;
				syGameFlagSetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT, count);
			}
			if(count != 0)
				scTaopoSwampFillDripSpot(0, ACTOR_DRIP_PUDDLE);
			actor = fdActorGet(ACTOR_DRIP_PUDDLE);
			actor->scaleX = count * (FIXED_ONE / 4);
			actor->scaleY = count * (FIXED_ONE / 4);
		}
		return 0;
	}
	return 1;
}

/* A task: every 64 frames a drop falls from the ceiling onto the drips room's
 */
/* spot. */
void scTaopoSwampDripFromCeiling(void) {
	if((FrameCount & 63) == 0) {
		FIXED x = toFIXED(408);
		FIXED z = toFIXED(440);
		FIXED y = toFIXED(160);
		ACTOR *actor = scFieldActorCreate(SPRITE_DROPLET, x, y, z);
		FIXED scale = FIXED_ONE * 33 / 100;

		if(actor != NULL) {
			actor->scaleX = scale;
			actor->scaleY = scale;
			actor->gravity = toFIXED(0.45);
			fdActorStartScript(actor, TaopoSwamp_DripFromCeilingScript);
			fdActorSetSprFlags(actor, 0);
		}
	}
}

/* A drop's script test: whether it is still above the floor. */
s32 scTaopoSwampIsDropFalling(ACTOR *actor) {
	if(actor->y.fixed + actor->velY < -toFIXED(48))
		return 0;
	return 1;
}

/* A task: on one in four of every eighth frame, a drop falls from the ceiling
 */
/* at a random place on the 32 pixels running east and north from (x0, z0). */
#define DEFINE_DRIP_AT_RANDOM(name, x0, z0)                                    \
	void name(void) {                                                          \
		if((FrameCount & 7) == 0 && (u32)syRandom() < 16384) {                 \
			s32 offset = syRandom() << 5;                                      \
			s32 x = offset + x0;                                               \
			s32 z = z0 - offset;                                               \
			s32 y = toFIXED(160);                                              \
			ACTOR *actor = scFieldActorCreate(SPRITE_DROPLET, x, y, z);        \
			s32 scale = FIXED_ONE * 33 / 100;                                  \
                                                                               \
			if(actor != NULL) {                                                \
				actor->scaleX = scale;                                         \
				actor->scaleY = scale;                                         \
				actor->motionFlg = ACTOR_FALLS;                                \
				actor->groundHeight = -toFIXED(64);                            \
				actor->gravity = toFIXED(0.45);                                \
				fdActorStartScript(actor, TaopoSwamp_DropFalls);               \
				fdActorSetSprFlags(actor, 0);                                  \
			}                                                                  \
		}                                                                      \
	}

/* The drops falling in the room of the pillar gap, from (384, 400). */
DEFINE_DRIP_AT_RANDOM(TaopoSwamp_DripInPillarGap, toFIXED(384), toFIXED(400))

/* The drops falling in the drips room, from (416, 352). */
DEFINE_DRIP_AT_RANDOM(TaopoSwamp_DripInDripRoom, toFIXED(416), toFIXED(352))

extern u16 TaopoSwampVentsVents[];
extern u16 TaopoSwampVentsPillars[];
extern u16 TaopoSwampPillarGapVents[];
extern u16 TaopoSwampDripsVents[];
extern u16 TaopoSwamp_RockRoomVents[];
void scTaopoSwampResetMud(void);
void syVentsStart(u16 *points, u16 *ids, void (*cycle)(void));

#if !TLA_PATCH_1
/* Sets up the room the party arrives in as its flags left it: the mud's */
/* bushes, vine and stump, the vents and their cycles, the spots, the rope, the
 */
/* glowing rocks and the passage. */
s32 scInit(void) {
	s32 scene = MainRoutineStateWork.next;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *actor, *stump, *piece;
	s32 id, entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	if(scene == SCENE_TAOPO_SWAMP_MUD) {
		MainRoutineStateWork.retreatScene = SCENE_TAOPO_SWAMP_ENTRANCE;
		MainRoutineStateWork.retreatEntrance =
			TAOPO_SWAMP_ENTRANCE_FROM_WORLD_MAP;
		scTaopoSwampResetMud();
		for(id = ACTOR_FIRST_BUSH; id <= ACTOR_LAST_BUSH; id++) {
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_BUSH_BLOWN + id))
				fdActorSetPos(id, 0, 0);
			else
				fdActorGetById(id)->scaleY = FIXED_ONE * 5 / 4;
		}
		fdActorSetSprPriority(ACTOR_MUD_SPROUT, 3);
		fdActorGetById(ACTOR_MUD_SPROUT)->collisionFlg = 0;
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_VINE_GROWN))
			fdMapGridCopyCellAttributes(50, 16, 1, 2, 47, 16);
		stump = fdActorGetById(ACTOR_MUD_STUMP);
		if(stump != 0) {
			stump->scaleX = FIXED_ONE * 3 / 4;
			stump->scaleY = FIXED_ONE * 3 / 4;
			stump->motionFlg = 0;
		}
		sySetEvent(scTaopoSwampDripAtRandom, TASK_PASS_FRAME);
		((MAP_STATE_GROUND_EFFECTS *)map)->wading = MAP_WADING_MUD;
		DecodeFillByte = MUD_COLOR;
	}
	else if(scene == SCENE_TAOPO_SWAMP_TEAR_STONES) {
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(0)))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR, TREASURE_SPOT_WIDE);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(1)))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR + 1, TREASURE_SPOT_WIDE);
	}
	else if(scene == SCENE_TAOPO_SWAMP_FLOWER) {
		fdEffectStartBlankOverlay(0);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
			fdActorSetPos(ACTOR_FLOWER, 0, 0);
	}
	else if(scene == SCENE_TAOPO_SWAMP_VENTS) {
		fdEffectStartBlankOverlay(0);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
			fdActorSetPos(ACTOR_FLOWER, 0, 0);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT))
			fdActorSetPos(ACTOR_WEST_VENT_PILLAR, 280, 232);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT))
			fdActorSetPos(ACTOR_EAST_VENT_PILLAR, 472, 248);
		syVentsStart(TaopoSwampVentsVents, TaopoSwampVentsPillars,
					 scTaopoSwampCycleDjinniVents);
	}
	else if(scene == SCENE_TAOPO_SWAMP_PILLAR_GAP) {
		fdEffectStartBlankOverlay(0);
		sySetEvent(TaopoSwamp_DripInPillarGap, TASK_PASS_FRAME);
		/* Only the north room has the round spot and its vents. */
		if(MainRoutineStateWork.entrance ==
			   TAOPO_SWAMP_PILLAR_GAP_FROM_TAOPO_SWAMP_DRIPS_NORTHWEST ||
		   MainRoutineStateWork.entrance == PILLAR_GAP_NORTH_TO_SOUTH_ROOM)
			syVentsStart(TaopoSwampPillarGapVents, 0,
						 scTaopoSwampCycleSpotVent);
		piece = fdActorGetById(ACTOR_GAP_PUDDLE);
		piece->motionFlg = 0;
		fdActorSetSprFlags(piece, 0);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN)) {
			fdMapGridCopyCellAttributes(28, 8, 1, 1, 26, 8);
			fdActorSelectAnimation(piece, 0);
		}
		else {
			fdMapGridCopyCellAttributes(22, 8, 1, 1, 26, 8);
			fdActorSetSprPriority(ACTOR_GAP_PUDDLE, 3);
			piece->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
				fdActorSelectAnimation(piece, 7);
		}
		piece = fdActorGetById(ACTOR_GAP_PILLAR);
		piece->motionFlg = 0;
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_PILLAR_IN_GAP)) {
			fdActorSetPos(ACTOR_GAP_PILLAR, 264, 424);
			piece->groundHeight = 0;
			piece->y.fixed = 0;
			fdMapGridCopyCellAttributes(15, 26, 1, 1, 16, 26);
		}
	}
	else if(scene == SCENE_TAOPO_SWAMP_DRIPS) {
		/* Arriving from the Tremor room, the pillar gap's northwest door or the
		 */
		/* round spot, the party is by the rope posts; otherwise, by the round
		 * spot */
		/* with its vents and drips. */
		if(MainRoutineStateWork.entrance !=
			   TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_SOUTH &&
		   MainRoutineStateWork.entrance != DRIPS_ROUND_SPOT_TO_ROPE_POSTS &&
		   MainRoutineStateWork.entrance !=
			   TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_EAST) {
			fdRopeInit();
			Call4(fdRopePlace, 0, FLAG_TAOPO_SWAMP_ROPE_TIED,
				  ACTOR_WEST_ROPE_POST, ACTOR_EAST_ROPE_POST);
		}
		else {
			Value3((s32 (*)())syVentsStart, (s32)TaopoSwampDripsVents, 0,
				   (s32)scTaopoSwampCycleDripVent);
			fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
			syFieldTaskAddFrame(TaopoSwamp_DripInDripRoom, TASK_PASS_FRAME);
			syFieldTaskAddFrame(scTaopoSwampDripFromCeiling, TASK_PASS_FRAME);
			actor = fdActorGetById(ACTOR_DRIP_PUDDLE);
			actor->motionFlg = 0;
			fdActorSetSprFlags(actor, 0);
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN)) {
				fdActorSelectAnimation(actor, 0);
			}
			else {
				fdActorSetSprPriority(ACTOR_DRIP_PUDDLE, 3);
				actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
				if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
					fdActorSelectAnimation(actor, 7);
			}
			fdMapGridCopyCells(50, 36, 13, 13, 60, 5);
		}
	}
	else if(scene == SCENE_TAOPO_SWAMP_LAVA_ROCKS) {
		syVentsStart(TaopoSwamp_RockRoomVents, 0,
					 scTaopoSwampCycleRockRoomVents);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE);
		}
		else if(fdEventGetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_SUNK)) {
			fdActorSetAnimation(ACTOR_CHEST_ROCK, 3);
			fdActorSetPos(ACTOR_CHEST_ROCK, 184, 344);
			fdActorGetById(ACTOR_CHEST_ROCK)->collisionFlg = 0;
			fdActorGetById(ACTOR_CHEST_ROCK)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdActorSetSprPriority(ACTOR_CHEST_ROCK, 3);
			fdMapGridCopyCellAttributes(11, 19, 1, 1, 11, 21);
		}
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE);
		}
		else if(fdEventGetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_SUNK)) {
			fdActorSetAnimation(ACTOR_EXIT_ROCK, 3);
			fdActorSetPos(ACTOR_EXIT_ROCK, 504, 184);
			fdActorGetById(ACTOR_EXIT_ROCK)->collisionFlg = 0;
			fdActorGetById(ACTOR_EXIT_ROCK)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			fdMapGridCopyCellAttributes(30, 13, 1, 1, 31, 11);
		}
	}
	else if(scene == SCENE_TAOPO_SWAMP_TREMOR) {
		piece = fdActorGetById(ACTOR_TREMOR_ROCK_PILE);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_PASSAGE_OPEN)) {
			fdActorSetPos(ACTOR_TREMOR_ROCK_PILE, 480, 248);
			if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION))
				fdActorSetPos(ACTOR_TREMOR_PILLAR_ROCK, 440, 232);
			fdMapGridCopyCells(47, 50, 2, 14, 29, 1);
			fdMapGridCopyCells(50, 59, 13, 5, 18, 15);
		}
		else {
			piece->motionFlg = 0;
			piece->y.fixed = toFIXED(32);
		}
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE);
		}
		else if(fdEventGetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_SUNK)) {
			fdActorSetAnimation(ACTOR_TREMOR_LAVA_ROCK, 3);
			fdActorSetPos(ACTOR_TREMOR_LAVA_ROCK, 120, 248);
			fdActorGetById(ACTOR_TREMOR_LAVA_ROCK)->collisionFlg = 0;
			fdActorGetById(ACTOR_TREMOR_LAVA_ROCK)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdMapGridCopyCellAttributes(7, 17, 1, 1, 7, 15);
		}
		else {
			fdActorSetAnimation(ACTOR_TREMOR_LAVA_ROCK, 0);
		}
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE);
		}
		else {
			fdMapGridCopyCellAttributes(27, 14, 1, 1, 27, 13);
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_SUNK)) {
				fdActorSetAnimation(ACTOR_TREMOR_PILLAR_ROCK, 3);
				fdActorSetPos(ACTOR_TREMOR_PILLAR_ROCK, 408, 264);
				fdActorGetById(ACTOR_TREMOR_PILLAR_ROCK)->collisionFlg = 0;
				fdActorGetById(ACTOR_TREMOR_PILLAR_ROCK)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdMapGridCopyCellAttributes(25, 18, 1, 1, 25, 16);
			}
			else {
				fdActorSetAnimation(ACTOR_TREMOR_PILLAR_ROCK, 0);
			}
		}
	}
	else if(scene == SCENE_TAOPO_SWAMP_STAR_DUST) {
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE);
		}
		else {
			fdActorSetAnimation(ACTOR_DARK_ROCK, 0);
			fdMapGridCopyCellAttributes(7, 39, 1, 1, 7, 38);
		}
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED))
			fdEventSetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE);
		else
			fdActorSetAnimation(ACTOR_LAST_ROCK, 0);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR, TREASURE_SPOT_PUT_AWAY);
		entrance = MainRoutineStateWork.entrance;
		if(entrance == TAOPO_SWAMP_STAR_DUST_FROM_TAOPO_SWAMP_TREMOR ||
		   entrance == STAR_DUST_FIRST_TO_MIDDLE_ROOM) {
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED))
				scTaopoSwampDarkenRoom(0);
			else
				scTaopoSwampFocusMapMiddle(0);
		}
		if(entrance == STAR_DUST_LAST_CHAMBER_TO_MIDDLE) {
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED))
				scTaopoSwampDarkenRoom(0);
			else
				scTaopoSwampFocusMapMiddle(0);
		}
		if(entrance == STAR_DUST_MIDDLE_TO_FIRST_ROOM ||
		   entrance == STAR_DUST_MIDDLE_TO_LAST_CHAMBER)
			fdBlendScriptSuspend();
	}
	return 0;
}
#else
/* As above; from the first patch on, the entrance and the swamp outside fade */
/* in from black and the other rooms open with the interlaced wipe, where the */
/* Japanese edition opens every room with a window shape. */
s32 scInit(void) {
	s32 scene = MainRoutineStateWork.next;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 *effect = &FieldStateWork->transition;
	ACTOR *actor, *stump, *piece;
	s32 id, entrance;

	*effect = SCENE_TRANSITION(TRANSITION_FADE, 0);
	if(scene == SCENE_TAOPO_SWAMP_MUD) {
		MainRoutineStateWork.retreatScene = SCENE_TAOPO_SWAMP_ENTRANCE;
		MainRoutineStateWork.retreatEntrance =
			TAOPO_SWAMP_ENTRANCE_FROM_WORLD_MAP;
		scTaopoSwampResetMud();
		for(id = ACTOR_FIRST_BUSH; id <= ACTOR_LAST_BUSH; id++) {
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_BUSH_BLOWN + id))
				fdActorSetPos(id, 0, 0);
			else
				fdActorGetById(id)->scaleY = FIXED_ONE * 5 / 4;
		}
		fdActorSetSprPriority(ACTOR_MUD_SPROUT, 3);
		fdActorGetById(ACTOR_MUD_SPROUT)->collisionFlg = 0;
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_VINE_GROWN))
			fdMapGridCopyCellAttributes(50, 16, 1, 2, 47, 16);
		stump = fdActorGetById(ACTOR_MUD_STUMP);
		if(stump != 0) {
			stump->scaleX = FIXED_ONE * 3 / 4;
			stump->scaleY = FIXED_ONE * 3 / 4;
			stump->motionFlg = 0;
		}
		sySetEvent(scTaopoSwampDripAtRandom, TASK_PASS_FRAME);
		((MAP_STATE_GROUND_EFFECTS *)map)->wading = MAP_WADING_MUD;
		DecodeFillByte = MUD_COLOR;
	}
	else if(scene == SCENE_TAOPO_SWAMP_TEAR_STONES) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(0)))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR, TREASURE_SPOT_WIDE);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(1)))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR + 1, TREASURE_SPOT_WIDE);
	}
	else if(scene == SCENE_TAOPO_SWAMP_FLOWER) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
		fdEffectStartBlankOverlay(0);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
			fdActorSetPos(ACTOR_FLOWER, 0, 0);
	}
	else if(scene == SCENE_TAOPO_SWAMP_VENTS) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
		fdEffectStartBlankOverlay(0);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
			fdActorSetPos(ACTOR_FLOWER, 0, 0);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT))
			fdActorSetPos(ACTOR_WEST_VENT_PILLAR, 280, 232);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT))
			fdActorSetPos(ACTOR_EAST_VENT_PILLAR, 472, 248);
		syVentsStart(TaopoSwampVentsVents, TaopoSwampVentsPillars,
					 scTaopoSwampCycleDjinniVents);
	}
	else if(scene == SCENE_TAOPO_SWAMP_POOL) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
	}
	else if(scene == SCENE_TAOPO_SWAMP_PILLAR_GAP) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
		fdEffectStartBlankOverlay(0);
		sySetEvent(TaopoSwamp_DripInPillarGap, TASK_PASS_FRAME);
		/* Only the north room has the round spot and its vents. */
		if(MainRoutineStateWork.entrance ==
			   TAOPO_SWAMP_PILLAR_GAP_FROM_TAOPO_SWAMP_DRIPS_NORTHWEST ||
		   MainRoutineStateWork.entrance == PILLAR_GAP_NORTH_TO_SOUTH_ROOM)
			syVentsStart(TaopoSwampPillarGapVents, 0,
						 scTaopoSwampCycleSpotVent);
		piece = fdActorGetById(ACTOR_GAP_PUDDLE);
		piece->motionFlg = 0;
		fdActorSetSprFlags(piece, 0);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN)) {
			fdMapGridCopyCellAttributes(28, 8, 1, 1, 26, 8);
			fdActorSelectAnimation(piece, 0);
		}
		else {
			fdMapGridCopyCellAttributes(22, 8, 1, 1, 26, 8);
			fdActorSetSprPriority(ACTOR_GAP_PUDDLE, 3);
			piece->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
				fdActorSelectAnimation(piece, 7);
		}
		piece = fdActorGetById(ACTOR_GAP_PILLAR);
		piece->motionFlg = 0;
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_PILLAR_IN_GAP)) {
			fdActorSetPos(ACTOR_GAP_PILLAR, 264, 424);
			piece->groundHeight = 0;
			piece->y.fixed = 0;
			fdMapGridCopyCellAttributes(15, 26, 1, 1, 16, 26);
		}
	}
	else if(scene == SCENE_TAOPO_SWAMP_DRIPS) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
		/* Arriving from the Tremor room, the pillar gap's northwest door or the
		 */
		/* round spot, the party is by the rope posts; otherwise, by the round
		 * spot */
		/* with its vents and drips. */
		if(MainRoutineStateWork.entrance !=
			   TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_SOUTH &&
		   MainRoutineStateWork.entrance != DRIPS_ROUND_SPOT_TO_ROPE_POSTS &&
		   MainRoutineStateWork.entrance !=
			   TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_EAST) {
			fdRopeInit();
			Call4(fdRopePlace, 0, FLAG_TAOPO_SWAMP_ROPE_TIED,
				  ACTOR_WEST_ROPE_POST, ACTOR_EAST_ROPE_POST);
		}
		else {
			Value3((s32 (*)())syVentsStart, (s32)TaopoSwampDripsVents, 0,
				   (s32)scTaopoSwampCycleDripVent);
			fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
			syFieldTaskAddFrame(TaopoSwamp_DripInDripRoom, TASK_PASS_FRAME);
			syFieldTaskAddFrame(scTaopoSwampDripFromCeiling, TASK_PASS_FRAME);
			actor = fdActorGetById(ACTOR_DRIP_PUDDLE);
			actor->motionFlg = 0;
			fdActorSetSprFlags(actor, 0);
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN)) {
				fdActorSelectAnimation(actor, 0);
			}
			else {
				fdActorSetSprPriority(ACTOR_DRIP_PUDDLE, 3);
				actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
				if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
					fdActorSelectAnimation(actor, 7);
			}
			fdMapGridCopyCells(50, 36, 13, 13, 60, 5);
		}
	}
	else if(scene == SCENE_TAOPO_SWAMP_LAVA_ROCKS) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
		syVentsStart(TaopoSwamp_RockRoomVents, 0,
					 scTaopoSwampCycleRockRoomVents);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE);
		}
		else if(fdEventGetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_SUNK)) {
			fdActorSetAnimation(ACTOR_CHEST_ROCK, 3);
			fdActorSetPos(ACTOR_CHEST_ROCK, 184, 344);
			fdActorGetById(ACTOR_CHEST_ROCK)->collisionFlg = 0;
			fdActorGetById(ACTOR_CHEST_ROCK)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdActorSetSprPriority(ACTOR_CHEST_ROCK, 3);
			fdMapGridCopyCellAttributes(11, 19, 1, 1, 11, 21);
		}
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE);
		}
		else if(fdEventGetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_SUNK)) {
			fdActorSetAnimation(ACTOR_EXIT_ROCK, 3);
			fdActorSetPos(ACTOR_EXIT_ROCK, 504, 184);
			fdActorGetById(ACTOR_EXIT_ROCK)->collisionFlg = 0;
			fdActorGetById(ACTOR_EXIT_ROCK)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			fdMapGridCopyCellAttributes(30, 13, 1, 1, 31, 11);
		}
	}
	else if(scene == SCENE_TAOPO_SWAMP_TREMOR) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
		piece = fdActorGetById(ACTOR_TREMOR_ROCK_PILE);
		if(fdEventGetFlag(FLAG_TAOPO_SWAMP_PASSAGE_OPEN)) {
			fdActorSetPos(ACTOR_TREMOR_ROCK_PILE, 480, 248);
			if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION))
				fdActorSetPos(ACTOR_TREMOR_PILLAR_ROCK, 440, 232);
			fdMapGridCopyCells(47, 50, 2, 14, 29, 1);
			fdMapGridCopyCells(50, 59, 13, 5, 18, 15);
		}
		else {
			piece->motionFlg = 0;
			piece->y.fixed = toFIXED(32);
		}
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE);
		}
		else if(fdEventGetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_SUNK)) {
			fdActorSetAnimation(ACTOR_TREMOR_LAVA_ROCK, 3);
			fdActorSetPos(ACTOR_TREMOR_LAVA_ROCK, 120, 248);
			fdActorGetById(ACTOR_TREMOR_LAVA_ROCK)->collisionFlg = 0;
			fdActorGetById(ACTOR_TREMOR_LAVA_ROCK)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdMapGridCopyCellAttributes(7, 17, 1, 1, 7, 15);
		}
		else {
			fdActorSetAnimation(ACTOR_TREMOR_LAVA_ROCK, 0);
		}
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE);
		}
		else {
			fdMapGridCopyCellAttributes(27, 14, 1, 1, 27, 13);
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_SUNK)) {
				fdActorSetAnimation(ACTOR_TREMOR_PILLAR_ROCK, 3);
				fdActorSetPos(ACTOR_TREMOR_PILLAR_ROCK, 408, 264);
				fdActorGetById(ACTOR_TREMOR_PILLAR_ROCK)->collisionFlg = 0;
				fdActorGetById(ACTOR_TREMOR_PILLAR_ROCK)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdMapGridCopyCellAttributes(25, 18, 1, 1, 25, 16);
			}
			else {
				fdActorSetAnimation(ACTOR_TREMOR_PILLAR_ROCK, 0);
			}
		}
	}
	else if(scene == SCENE_TAOPO_SWAMP_STAR_DUST) {
		*effect = SCENE_TRANSITION(TRANSITION_WINDOW,
								   WINDOW_TRANSITION_INTERLACED_WIPE);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED)) {
			fdEventSetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE);
		}
		else {
			fdActorSetAnimation(ACTOR_DARK_ROCK, 0);
			fdMapGridCopyCellAttributes(7, 39, 1, 1, 7, 38);
		}
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED))
			fdEventSetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE);
		else
			fdActorSetAnimation(ACTOR_LAST_ROCK, 0);
		if(!fdEventGetFlag(FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR, TREASURE_SPOT_PUT_AWAY);
		entrance = MainRoutineStateWork.entrance;
		if(entrance == TAOPO_SWAMP_STAR_DUST_FROM_TAOPO_SWAMP_TREMOR ||
		   entrance == STAR_DUST_FIRST_TO_MIDDLE_ROOM) {
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED))
				scTaopoSwampDarkenRoom(0);
			else
				scTaopoSwampFocusMapMiddle(0);
		}
		if(entrance == STAR_DUST_LAST_CHAMBER_TO_MIDDLE) {
			if(fdEventGetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED))
				scTaopoSwampDarkenRoom(0);
			else
				scTaopoSwampFocusMapMiddle(0);
		}
		if(entrance == STAR_DUST_MIDDLE_TO_FIRST_ROOM ||
		   entrance == STAR_DUST_MIDDLE_TO_LAST_CHAMBER)
			fdBlendScriptSuspend();
	}
	return 0;
}
#endif

s32 scPrepareMap(void) { return 0; }

/* A task: the Star Dust's sparkle in the dark, a 3 by 3 picture of cells that
 */
/* steps through four pictures and back every 60 frames, until it is dug up */
/* and the last picture stays (FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP). The */
/* pictures are stacked at x 45 of the map from row 50. */
void scTaopoSwampSparkleStarDust(void) {
	s32 row = -1;

	if(fdEventGetFlag(FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP)) {
		row = 62;
	}
	else {
		switch(TaopoSwamp_SparkleFrame) {
		case 0:
			row = 59;
			break;
		case 10:
		case 50:
			row = 56;
			break;
		case 20:
		case 40:
			row = 53;
			break;
		case 30:
			row = 50;
			break;
		}
		if(++TaopoSwamp_SparkleFrame > 59)
			TaopoSwamp_SparkleFrame = 0;
	}
	if(row != -1)
		fdMapGridCopyCellsLayered(45, row, 3, 3, 76, 61);
}

extern u8 MapGridCellBuffer[];

/* Shows the Star Dust rooms' light, the map's layer 2, from its 8-pixel tile */
/* (x, z) on, and the screen follows. */
void scTaopoSwampFocusMapOn(s32 x, s32 z) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *layer;
	s32 index, fx = FIELD_HALF_CELL_ORIGIN(x);

	layer = &map->layers[2];
	layer->ofsX = fx;
	layer->ofsY = FIELD_HALF_CELL_ORIGIN(z);
	layer->maskX = MAPGRID_SIZE - 1;
	layer->maskY = MAPGRID_ROW_MASK;
	layer->originY = z << 7;
	layer->originX = x;
	layer->speedX = 0;
	layer->speedY = 0;
	layer->phaseX = 0;
	layer->phaseY = 0;
	index = (z / 2 << 7) + x / 2;
	layer->scaleY = layer->scaleX = FIXED_ONE;
	layer->cells = (MAP_CELL *)(MapGridCellBuffer + index * 4);
	layer->terrainIds = MapShapeGrid + index;
	layer->x = syMulFX(map->view[0], FIXED_ONE) + layer->ofsX;
	layer->y = syMulFX(map->view[1], layer->scaleY) + layer->ofsY;
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
}

/* The light goes out, over 50 frames when fade is set, from tile (100, 50) on,
 */
/* and in the last chamber the Star Dust's sparkle starts. */
void scTaopoSwampDarkenRoom(s32 fade) {
	fdBlendScriptSuspend();
	if(fade) {
		syIoWrite16(BLEND_ALPHA(6, 14), (u16 *)&REG_BLDALPHA);
		syWait(10);
		syIoWrite16(BLEND_ALPHA(4, 13), (u16 *)&REG_BLDALPHA);
		syWait(10);
		syIoWrite16(BLEND_ALPHA(2, 13), (u16 *)&REG_BLDALPHA);
		syWait(10);
		syIoWrite16(BLEND_ALPHA(1, 12), (u16 *)&REG_BLDALPHA);
		syWait(10);
		syIoWrite16(BLEND_ALPHA(0, 12), (u16 *)&REG_BLDALPHA);
		syWait(10);
	}
	scTaopoSwampFocusMapOn(100, 50);
	syIoWrite16(BLEND_ALPHA(7, 12), (u16 *)&REG_BLDALPHA);
	if(MainRoutineStateWork.entrance == STAR_DUST_LAST_CHAMBER_TO_MIDDLE)
		syFieldTaskAddFrame(scTaopoSwampSparkleStarDust, TASK_PASS_FRAME);
}

/* Shows the Star Dust rooms' light lit, from tile (100, 100). */
void scTaopoSwampFocusMapMiddle(void) { scTaopoSwampFocusMapOn(100, 100); }

/* Entering the swamp outside afresh (not back from a menu or battle), Felix */
/* starts out of the mud. */
void scTaopoSwampResetMud(void) {
	SCENE_STATE *state = &SceneStateWork;

	if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
		syWordsClearEntry(state, sizeof(SCENE_STATE));
}

/* Each step Felix takes through the swamp's mud sinks him a little deeper. */
/* Sunk all the way, he goes under, two rings spread on the mud, and he comes */
/* back up on the last of the mud's firm cells he crossed. */
void scTaopoSwampSinkInMud(void) {
	SCENE_STATE *state = &SceneStateWork;
	ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
	SPR *sprite = actor->spr;
	ACTOR *spark;
	FIELD_SPR *sparkSprite;
	s32 step, i;

	if(sprite->entries[1] != 0)
		sprite->entries[1]->pri = 1;
	sprite->flg |= SPRITE_FLAG_SUNK;
	sprite->flg &= ~SPRITE_FLAG_SHADOW;
	syIoWrite8(1, &sprite->dirty);
	/* Walking, the leader sinks all the way in about 95 frames; running, in 64.
	 */
	if(InputWork.held & KEYS_DPAD) {
		step = LEADER_SINK_ROWS * MUD_DEPTH_ROW;
		if(InputWork.held & KEY_B)
			step = step * 3 >> 1;
		state->depth += step / MUD_SINK_FRAMES + 1;
	}
	sprite->sunkRows = state->depth / MUD_DEPTH_ROW;
	state->footing = FOOTING_MUD;
	if(state->depth < LEADER_SINK_ROWS * MUD_DEPTH_ROW)
		return;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	actor->sprStorageKind = 0;
	for(i = 0; i < 2; i++) {
		spark = (scFieldActorCreate(SPRITE_SPLASH_RING, actor->x.fixed,
									actor->y.fixed, actor->z.fixed));
		if(spark != 0) {
			sparkSprite = spark->spr;
			fdActorStartScript(spark, TaopoSwamp_SplashFades);
			spark->motionFlg = 0;
			spark->layer = 1;
			if(sparkSprite != 0) {
				fdSprStartAnimation(sparkSprite, 2);
				sparkSprite->flg = 0;
				sparkSprite->pri = 2;
			}
		}
		syWait(15);
	}
	syFieldEventWait(30);
	actor->sprStorageKind = 1;
	sprite->sunkRows = 0;
	sprite->flg = 0;
	fdEventEnd();
	actor->x.fixed = state->x;
	actor->z.fixed = state->z;
	state->footing = FOOTING_FIRM_GROUND;
	state->depth = 0;
}

/* Stepping onto one of the bubbles in the swamp's mud, Felix is drawn to its */
/* middle and lifted back out to full height. */
void scTaopoSwampRiseOnBubble(void) {
	SCENE_STATE *state = &SceneStateWork;
	ACTOR *actor;
	SPR *sprite;

	if(state->footing != FOOTING_BUBBLE) {
		actor = fdActorGet(syFieldPartyGetLeaderActor());
		sprite = actor->spr;
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		scFieldActorSetMoveTargetFX(
			actor, (actor->x.fixed & -toFIXED(16)) + toFIXED(8), actor->y.fixed,
			(actor->z.fixed & -toFIXED(16)) + toFIXED(8));
		scFieldActorWaitForMove(actor);
		while(sprite->sunkRows != 0) {
			sprite->sunkRows--;
			sprite->dirty = 1;
			syWait(1);
		}
		fdEventEnd();
		sprite->flg |= SPRITE_FLAG_SUNK;
		sprite->flg &= ~SPRITE_FLAG_SHADOW;
		state->footing = FOOTING_BUBBLE;
		state->depth = 0;
	}
}

/* Stepping onto one of the four firm cells at the edges of the swamp's mud, */
/* (22, 9), (37, 14), (39, 19) and (8, 25), Felix stands at full height again,
 */
/* and the mud will bring him back here if he sinks. */
void scTaopoSwampStepOnFirmGround(void) {
	SCENE_STATE *state = &SceneStateWork;
	SPR *sprite;

	scTaopoSwampSaveFirmGround();
	if(state->footing != FOOTING_FIRM_GROUND) {
		sprite = fdActorGet(syFieldPartyGetLeaderActor())->spr;
		if(sprite->entries[1])
			sprite->entries[1]->pri = SPRITE_LAYER_NOT_DRAWN;
		sprite->flg = (sprite->flg & ~SPRITE_FLAG_SUNK) | SPRITE_FLAG_SHADOW;
		sprite->sunkRows = 0;
		sprite->dirty = 1;
		state->footing = FOOTING_FIRM_GROUND;
		state->depth = 0;
	}
}

/* Keeps the cell Felix stands on as the one the mud returns him to. */
void scTaopoSwampSaveFirmGround(void) {
	SCENE_STATE *state = &SceneStateWork;
	ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());

	state->x = (actor->x.fixed & -toFIXED(16)) + toFIXED(8);
	state->z = (actor->z.fixed & -toFIXED(16)) + toFIXED(8);
}

#include "../field/actor_vent_bounce.c"

#include "shared/actor_start_scripted.h"

/* A puff of dust from a vent at position, as its gust blows. */
void scVentPuff(s32 *position, s32 base) {
	ACTOR *actor = scTaopoSwampCreateActor(
		SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));

	if(actor != 0) {
		actor->bounce =
			syCos((syRandomBelow(TRIG_TURN * 3 / 16)) + TRIG_TURN * 3 / 16);
		actor->gravity = (u16)syRandom();
		actor->sceneWord = (u16)(syRandom() << 1) + base;
		/* A spin of up to half a turn a frame, either way. */
		actor->tmr = syRandomBelow(TRIG_TURN) - DEGtoANG(180);
		fdActorStartScripted(actor, 3);
		actor->update = (OBJECT_UPDATE)scVentBounce;
		fdActorSetBlendMode(actor, 1);
	}
}

#include "shared/vent_update.h"

#include "shared/vents_start.h"

#include "shared/vent_is_blowing.h"

/* Starts or stops vent index's gust. */
void scVentSetBlowing(s32 index, s32 blowing) {
	VENT_WORK *state;

	state = VENT_STATE;
	VENT_AT(state, index)->entries[0].active = blowing;
}

/* The vents' frame counter, which their cycles time their gusts by. */
s32 scVentGetCounter(void) { return VENT_STATE->cnt; }

#include "shared/vent_advance_counter.h"

#include "shared/vent_set_gust.h"

#include "shared/vent_set_id.h"
#undef DEFINE_TAOPO_SWAMP_FREEZE_SPOT
#undef DEFINE_DRIP_AT_RANDOM

void scTaopoSwampFightFlower(void);
void scTaopoSwampScatterWeeds(void);
void scTaopoSwampBlowFlowerOffLedge(void);
void scTaopoSwampShowSparkleMessage(void);

/* The vent whose gust each of the vents room's gust triggers is, from */
/* TRIGGER_FIRST_GUST on, as places in TaopoSwampVentsVents. */
u8 TaopoSwampVentsGustVents[] = {
	0, 2, 9, 6, 4, 6, 5, 1, 3,
};

/* The same for the round spot's room, as places in TaopoSwampPillarGapVents. */
u8 TaopoSwampPillarGapGustVents[] = {
	0, 1, 0, 1, 0, 0, 0,
};

/* clang-format off */
u32 TaopoSwamp_PuffSteamScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ANIMATION(2),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_WAIT(1),
    SC_LOOP(30, SC_TO(1)),
    SC_RELEASE,
};

#include "shared/debris_script.c"

u32 TaopoSwamp_SparkFades[] = {
    SC_WAIT(30),
    SC_RELEASE,
};

s32 TaopoSwamp_DripAtRandomScript[] = {
    SC_CALL((s32)scTaopoSwampLandDrop),
    SC_RELEASE,
};

s32 TaopoSwamp_DripFromCeilingScript[] = {
    SC_CALL((s32)scTaopoSwampLandDrip),
    SC_RELEASE,
};

s32 TaopoSwamp_DropFalls[] = {
    SC_CALL(scTaopoSwampIsDropFalling),
    SC_RELEASE,
};

/* Each room's vents for syVentsStart: a vent's x and z in pixels and */
/* the speed of its gust, ended by 0. */
u16 TaopoSwampVentsVents[] = {
    200, 200, 4,
    264, 200, 4,
    232, 216, 4,
    280, 216, 4,
    568, 200, 4,
    632, 200, 4,
    536, 216, 4,
    600, 216, 4,
    408, 232, 4,
    472, 232, 4,
    0,
};

/* The actors the vents room's puffs land on, its two pillars, ended by 0. */
u16 TaopoSwampVentsPillars[] = {ACTOR_WEST_VENT_PILLAR, ACTOR_EAST_VENT_PILLAR, 0};

/* The pillar gap room's vents, as TaopoSwampVentsVents. */
u16 TaopoSwampPillarGapVents[] = {
    424, 104, 4,
    536, 136, 4,
    0,
};

/* The drips room's vents, as TaopoSwampVentsVents. */
u16 TaopoSwampDripsVents[] = {
    408, 408, 4,
    472, 360, 4,
    0,
};

/* The rock room's vents, as TaopoSwampVentsVents. */
u16 TaopoSwamp_RockRoomVents[] = {
    216, 184, 4,
    312, 136, 4,
    0, 0,
};

u32 TaopoSwamp_SplashFades[] = {
    SC_WAIT(32),
    SC_RELEASE,
};

#include "shared/dust_burst_script.c"

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(TaopoSwampEntrance),
    SCENE_EXIT(TAOPO_SWAMP_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TAOPO_SWAMP_ENTRANCE),
    SCENE_EXIT(TAOPO_SWAMP_ENTRANCE_TO_TAOPO_SWAMP_MUD_NORTH, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_ENTRANCE_NORTH),
    SCENE_EXIT(TAOPO_SWAMP_ENTRANCE_TO_TAOPO_SWAMP_MUD_SOUTH, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_ENTRANCE_SOUTH),
    SCENE_EXITS(TaopoSwampMud),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_ENTRANCE_NORTH, TaopoSwampEntrance, TAOPO_SWAMP_ENTRANCE_FROM_TAOPO_SWAMP_MUD_NORTH),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_ENTRANCE_SOUTH, TaopoSwampEntrance, TAOPO_SWAMP_ENTRANCE_FROM_TAOPO_SWAMP_MUD_SOUTH),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_TEAR_STONES_WEST, TaopoSwampTearStones, TAOPO_SWAMP_TEAR_STONES_FROM_TAOPO_SWAMP_MUD_WEST),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_POOL, TaopoSwampPool, TAOPO_SWAMP_POOL_FROM_TAOPO_SWAMP_MUD),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_TEAR_STONES_EAST, TaopoSwampTearStones, TAOPO_SWAMP_TEAR_STONES_FROM_TAOPO_SWAMP_MUD_EAST),
    SCENE_EXITS(TaopoSwampTearStones),
    SCENE_EXIT(TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_MUD_WEST, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_TEAR_STONES_WEST),
    SCENE_EXIT(TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_MUD_EAST, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_TEAR_STONES_EAST),
    SCENE_EXIT(TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_VENTS_NORTH, TaopoSwampVents, TAOPO_SWAMP_VENTS_FROM_TAOPO_SWAMP_TEAR_STONES_NORTH),
    SCENE_EXIT(TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_VENTS_SOUTH, TaopoSwampVents, TAOPO_SWAMP_VENTS_FROM_TAOPO_SWAMP_TEAR_STONES_SOUTH),
    SCENE_EXIT(TEAR_STONES_PASSAGE_TO_CAVE, TaopoSwampTearStones, TEAR_STONES_CAVE_TO_PASSAGE),
    SCENE_EXIT(TEAR_STONES_CAVE_TO_PASSAGE, TaopoSwampTearStones, TEAR_STONES_PASSAGE_TO_CAVE),
    SCENE_EXITS(TaopoSwampFlower),
    SCENE_EXIT(TAOPO_SWAMP_FLOWER_TO_TAOPO_SWAMP_VENTS_WEST_A, TaopoSwampVents, TAOPO_SWAMP_VENTS_FROM_TAOPO_SWAMP_FLOWER_WEST_A),
    SCENE_EXIT(TAOPO_SWAMP_FLOWER_TO_TAOPO_SWAMP_VENTS_EAST_A, TaopoSwampVents, TAOPO_SWAMP_VENTS_FROM_TAOPO_SWAMP_FLOWER_EAST_A),
    SCENE_EXITS(TaopoSwampVents),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_WEST_A, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_WEST_A),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_TEAR_STONES_NORTH, TaopoSwampTearStones, TAOPO_SWAMP_TEAR_STONES_FROM_TAOPO_SWAMP_VENTS_NORTH),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_TEAR_STONES_SOUTH, TaopoSwampTearStones, TAOPO_SWAMP_TEAR_STONES_FROM_TAOPO_SWAMP_VENTS_SOUTH),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_A, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_A),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_WEST_B, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_WEST_B),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_WEST_C, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_WEST_C),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_SOUTHEAST, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_SOUTHEAST),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_B, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_B),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_C, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_C),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_D, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_D),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_E, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_E),
    SCENE_EXITS(TaopoSwampPool),
    SCENE_EXIT(TAOPO_SWAMP_POOL_TO_TAOPO_SWAMP_PILLAR_GAP, TaopoSwampPillarGap, TAOPO_SWAMP_PILLAR_GAP_FROM_TAOPO_SWAMP_POOL),
    SCENE_EXIT(TAOPO_SWAMP_POOL_TO_TAOPO_SWAMP_MUD, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_POOL),
    SCENE_EXITS(TaopoSwampPillarGap),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_POOL, TaopoSwampPool, TAOPO_SWAMP_POOL_FROM_TAOPO_SWAMP_PILLAR_GAP),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_SOUTH, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_SOUTH),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_NORTHWEST, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_NORTHWEST),
    SCENE_EXIT(PILLAR_GAP_SOUTH_TO_NORTH_ROOM, TaopoSwampPillarGap, PILLAR_GAP_NORTH_TO_SOUTH_ROOM),
    SCENE_EXIT(PILLAR_GAP_NORTH_TO_SOUTH_ROOM, TaopoSwampPillarGap, PILLAR_GAP_SOUTH_TO_NORTH_ROOM),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_EAST_A, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_EAST),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_EAST_B, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_EAST),
    SCENE_EXITS(TaopoSwampDrips),
    SCENE_EXIT(TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_TREMOR, TaopoSwampTremor, TAOPO_SWAMP_TREMOR_FROM_TAOPO_SWAMP_DRIPS),
    SCENE_EXIT(TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_PILLAR_GAP_SOUTH, TaopoSwampPillarGap, TAOPO_SWAMP_PILLAR_GAP_FROM_TAOPO_SWAMP_DRIPS_SOUTH),
    SCENE_EXIT(TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_PILLAR_GAP_NORTHWEST, TaopoSwampPillarGap, TAOPO_SWAMP_PILLAR_GAP_FROM_TAOPO_SWAMP_DRIPS_NORTHWEST),
    SCENE_EXIT(DRIPS_ROUND_SPOT_TO_ROPE_POSTS, TaopoSwampDrips, DRIPS_ROPE_POSTS_TO_ROUND_SPOT),
    SCENE_EXIT(DRIPS_ROPE_POSTS_TO_ROUND_SPOT, TaopoSwampDrips, DRIPS_ROUND_SPOT_TO_ROPE_POSTS),
    SCENE_EXITS(TaopoSwampTremor),
    SCENE_EXIT(TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_DRIPS, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_TREMOR),
    SCENE_EXIT(TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_STAR_DUST, TaopoSwampStarDust, TAOPO_SWAMP_STAR_DUST_FROM_TAOPO_SWAMP_TREMOR),
    SCENE_EXIT(TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_LAVA_ROCKS_EAST, TaopoSwampLavaRocks, TAOPO_SWAMP_LAVA_ROCKS_FROM_TAOPO_SWAMP_TREMOR_EAST),
    SCENE_EXIT(TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_LAVA_ROCKS_WEST, TaopoSwampLavaRocks, TAOPO_SWAMP_LAVA_ROCKS_FROM_TAOPO_SWAMP_TREMOR_WEST),
    SCENE_EXITS(TaopoSwampLavaRocks),
    SCENE_EXIT(TAOPO_SWAMP_LAVA_ROCKS_TO_TAOPO_SWAMP_TREMOR_EAST, TaopoSwampTremor, TAOPO_SWAMP_TREMOR_FROM_TAOPO_SWAMP_LAVA_ROCKS_EAST),
    SCENE_EXIT(TAOPO_SWAMP_LAVA_ROCKS_TO_TAOPO_SWAMP_TREMOR_WEST, TaopoSwampTremor, TAOPO_SWAMP_TREMOR_FROM_TAOPO_SWAMP_LAVA_ROCKS_WEST),
    SCENE_EXITS(TaopoSwampStarDust),
    SCENE_EXIT(TAOPO_SWAMP_STAR_DUST_TO_TAOPO_SWAMP_TREMOR, TaopoSwampTremor, TAOPO_SWAMP_TREMOR_FROM_TAOPO_SWAMP_STAR_DUST),
    SCENE_EXIT(STAR_DUST_FIRST_TO_MIDDLE_ROOM, TaopoSwampStarDust, STAR_DUST_MIDDLE_TO_FIRST_ROOM),
    SCENE_EXIT(STAR_DUST_MIDDLE_TO_FIRST_ROOM, TaopoSwampStarDust, STAR_DUST_FIRST_TO_MIDDLE_ROOM),
    SCENE_EXIT(STAR_DUST_MIDDLE_TO_LAST_CHAMBER, TaopoSwampStarDust, STAR_DUST_LAST_CHAMBER_TO_MIDDLE),
    SCENE_EXIT(STAR_DUST_LAST_CHAMBER_TO_MIDDLE, TaopoSwampStarDust, STAR_DUST_MIDDLE_TO_LAST_CHAMBER),
    SCENE_EXITS_END,
};

ACTOR_PLACE TaopoSwamp_Placements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE TaopoSwampMudActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 128, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 112, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 112, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 176, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 256, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 272, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, FLAG_TAOPO_SWAMP_STUMP_SUNK, MOVEMENT_TYPE_STAND, 216, 0, 264, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE TaopoSwampFlowerActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_FLOWER), MOVEMENT_TYPE_STAND, 440, 0, 632, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE TaopoSwampVentsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_FLOWER), MOVEMENT_TYPE_STAND, 408, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE TaopoSwampPillarGapActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE TaopoSwampDripsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 440, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE TaopoSwampTremorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 296, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 360, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 376, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROCK_PILE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROCK_PILE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 336, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROCK_PILE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 352, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROCK_PILE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE TaopoSwampLavaRocksActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 360, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE TaopoSwampStarDustActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 632, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 632, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

TRIGGER TaopoSwamp_Triggers[] = {
    {TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampEntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, TAOPO_SWAMP_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, TAOPO_SWAMP_ENTRANCE_TO_TAOPO_SWAMP_MUD_NORTH},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, TAOPO_SWAMP_ENTRANCE_TO_TAOPO_SWAMP_MUD_SOUTH},
    {TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampMudTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_ENTRANCE_NORTH},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_ENTRANCE_SOUTH},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 3, IF_FLAG_SET(FLAG_TAOPO_SWAMP_BUSH_BLOWN + ACTOR_TEAR_STONES_WEST_BUSH), TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_TEAR_STONES_WEST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 4, IF_FLAG_SET(FLAG_TAOPO_SWAMP_BUSH_BLOWN + ACTOR_POOL_BUSH), TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_POOL},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 5, IF_FLAG_SET(FLAG_TAOPO_SWAMP_BUSH_BLOWN + ACTOR_TEAR_STONES_EAST_BUSH), TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_TEAR_STONES_EAST},
    {TRIGGER_STEP, TRIGGER_MUD, CONDITION_ALWAYS, EVENT(scTaopoSwampSinkInMud)},
    {TRIGGER_STEP, TRIGGER_BUBBLE, CONDITION_ALWAYS, EVENT(scTaopoSwampRiseOnBubble)},
    {TRIGGER_STEP, TRIGGER_FIRM_GROUND, CONDITION_ALWAYS, EVENT(scTaopoSwampStepOnFirmGround)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TEAR_STONES_WEST_BUSH, CONDITION_ALWAYS, EVENT(scTaopoSwampBlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_BUSH + 1, CONDITION_ALWAYS, EVENT(scTaopoSwampBlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_BUSH + 2, CONDITION_ALWAYS, EVENT(scTaopoSwampBlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_POOL_BUSH, CONDITION_ALWAYS, EVENT(scTaopoSwampBlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TEAR_STONES_EAST_BUSH, CONDITION_ALWAYS, EVENT(scTaopoSwampBlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_MUD_SPROUT, CONDITION_ALWAYS, EVENT(scTaopoSwampGrowVine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MUD_STUMP, CONDITION_ALWAYS, EVENT(scTaopoSwampSinkStump)},
    {TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampTearStonesTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_MUD_WEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_MUD_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_VENTS_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_VENTS_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, TEAR_STONES_PASSAGE_TO_CAVE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, TEAR_STONES_CAVE_TO_PASSAGE},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), TRIGGER_SECOND_TEAR_STONE, FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(1), EVENT(scTaopoSwampScoopSecondTearStone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), TRIGGER_FIRST_TEAR_STONE, FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(0), EVENT(scTaopoSwampScoopFirstTearStone)},
    {TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampFlowerTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, TAOPO_SWAMP_FLOWER_TO_TAOPO_SWAMP_VENTS_WEST_A},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, TAOPO_SWAMP_FLOWER_TO_TAOPO_SWAMP_VENTS_EAST_A},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(scTaopoSwampFightFlower)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), TRIGGER_WEEDS, CONDITION_ALWAYS, EVENT(scTaopoSwampScatterWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), TRIGGER_BRAMBLE_SEED_WEEDS, CONDITION_ALWAYS, EVENT(scTaopoSwampUncoverBrambleSeed)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), TRIGGER_MAD_PLANT_WEEDS, CONDITION_ALWAYS, EVENT(scTaopoSwampStartMadPlantBattle)},
    {TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampVentsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_WEST_A},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_TEAR_STONES_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_TEAR_STONES_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_A},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_FIRST_GUST, CONDITION_ALWAYS, EVENT(scTaopoSwampDropToLowerFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_FIRST_GUST + 1, CONDITION_ALWAYS, EVENT(scTaopoSwampDropToLowerFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_FIRST_GUST + 2, CONDITION_ALWAYS, EVENT(scTaopoSwampDropToLowerFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_FIRST_GUST + 3, CONDITION_ALWAYS, EVENT(scTaopoSwampDropToLowerFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_FIRST_GUST + 4, CONDITION_ALWAYS, EVENT(scTaopoSwampDropToLowerFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_FIRST_GUST + 5, CONDITION_ALWAYS, EVENT(scTaopoSwampDropToLowerFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_FIRST_GUST + 6, CONDITION_ALWAYS, EVENT(scTaopoSwampDropToLowerFloor)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST + 1, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST + 2, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST + 3, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST + 4, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST + 5, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST + 6, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST + 7, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {TRIGGER_STEP, TRIGGER_FIRST_GUST + 8, CONDITION_ALWAYS, EVENT(scTaopoSwampStepIntoGust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_VENT_PILLAR, FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT, EVENT(scTaopoSwampPushPillarOverVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_VENT_PILLAR, FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT, EVENT(scTaopoSwampPushPillarOverVent)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scTaopoSwampPushPillarOverVent)},
    {TRIGGER_RAISED, TRIGGER_FLOWER_IN_GUST, FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE, EVENT(scTaopoSwampBlowFlowerOffLedge)},
    {TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampPoolTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, TAOPO_SWAMP_POOL_TO_TAOPO_SWAMP_PILLAR_GAP},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, TAOPO_SWAMP_POOL_TO_TAOPO_SWAMP_MUD},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

TRIGGER TaopoSwampPillarGapTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
	 TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_POOL},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS,
	 TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_SOUTH},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS,
	 TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_NORTHWEST},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, PILLAR_GAP_SOUTH_TO_NORTH_ROOM},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS, PILLAR_GAP_NORTH_TO_SOUTH_ROOM},
	{STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TRIGGER_PAST_SPOT_GUST,
	 CONDITION_ALWAYS, EVENT(scTaopoSwampDropToLowerFloor)},
	{TRIGGER_STEP, TRIGGER_PAST_SPOT_GUST, CONDITION_ALWAYS,
	 EVENT(scTaopoSwampStepIntoGust)},
	{TRIGGER_STEP, TRIGGER_SPOT_GUST, CONDITION_ALWAYS,
	 EVENT(scTaopoSwampStepIntoGust)},
	{TRIGGER_STEP, TRIGGER_FIRST_GUST + 1, CONDITION_ALWAYS,
	 EVENT(scTaopoSwampStepIntoGust)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GAP_PILLAR,
	 FLAG_TAOPO_SWAMP_PILLAR_IN_GAP, EVENT(scTaopoSwampDropPillarIntoGap)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scTaopoSwampDropPillarIntoGap)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_ACTS),
	 ACTOR_GAP_PUDDLE, FLAG_TAOPO_SWAMP_SPOT_WET,
	 EVENT(scTaopoSwampDouseVentSpot)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_GAP_PUDDLE,
	 IF_FLAG_SET(FLAG_TAOPO_SWAMP_SPOT_PUDDLE),
	 EVENT(TaopoSwamp_FreezeVentSpot)},
	{TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampDripsTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS,
	 TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_TREMOR},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS,
	 TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_PILLAR_GAP_SOUTH},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS,
	 TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_PILLAR_GAP_NORTHWEST},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
	{TRIGGER_STEP, TRIGGER_FIRST_GUST, CONDITION_ALWAYS,
	 EVENT(scTaopoSwampStepIntoGust)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_WEST_ROPE_POST,
	 FLAG_TAOPO_SWAMP_ROPE_TIED, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_ACTS),
	 ACTOR_DRIP_PUDDLE, FLAG_TAOPO_SWAMP_SPOT_WET,
	 EVENT(scTaopoSwampFillDripSpot)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_DRIP_PUDDLE,
	 IF_FLAG_SET(FLAG_TAOPO_SWAMP_SPOT_PUDDLE),
	 EVENT(TaopoSwamp_FreezeDripSpot)},
	{TRIGGER_TABLE_END},
};

/* The cooled pool's countdowns: the steam puffs still to rise, one a frame */
/* (scTaopoSwampPuffSteam); the frames the passage takes to dry, eight to a row
 */
/* (scTaopoSwampOpenPassage); and the pool's flickers */
/* (scTaopoSwampFlickerPool). */
s32 TaopoSwamp_SteamPuffs = 112;
s32 TaopoSwamp_PassageFrames = 120;
s32 TaopoSwamp_PoolFrames = 24;

TRIGGER TaopoSwampTremorTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
	 TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_DRIPS},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS,
	 TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_STAR_DUST},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS,
	 TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_LAVA_ROCKS_EAST},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS,
	 TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_LAVA_ROCKS_WEST},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TREMOR_LAVA_ROCK,
	 FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED, EVENT(scTaopoSwampCoolLavaRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TREMOR_PILLAR_ROCK,
	 FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED, EVENT(scTaopoSwampCoolPillarRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TREMOR_LAVA_ROCK,
	 FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE,
	 EVENT(scTaopoSwampPushLavaRockIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TREMOR_PILLAR_ROCK,
	 FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE,
	 EVENT(scTaopoSwampPushPillarRockIn)},
	{TRIGGER_PUSHED, 0, FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE,
	 EVENT(scTaopoSwampPushPillarRockIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_TREMOR_PILLAR,
	 FLAG_TAOPO_SWAMP_PASSAGE_OPEN, EVENT(scTaopoSwampTremorPillar)},
	{TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampLavaRocksTriggers[] = {
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS,
	 TAOPO_SWAMP_LAVA_ROCKS_TO_TAOPO_SWAMP_TREMOR_EAST},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS,
	 TAOPO_SWAMP_LAVA_ROCKS_TO_TAOPO_SWAMP_TREMOR_WEST},
	{TRIGGER_STEP, TRIGGER_FIRST_GUST, CONDITION_ALWAYS,
	 EVENT(scTaopoSwampStepIntoGust)},
	{TRIGGER_STEP, TRIGGER_FIRST_GUST + 1, CONDITION_ALWAYS,
	 EVENT(scTaopoSwampStepIntoGust)},
#if TLA_PATCH_1
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_JUMP_TO_ROCK,
	 CONDITION_ALWAYS, EVENT(scTaopoSwampJumpForward)},
#endif
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_CHEST_ROCK,
	 FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED, EVENT(scTaopoSwampCoolChestRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_EXIT_ROCK,
	 FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED, EVENT(scTaopoSwampCoolExitRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CHEST_ROCK,
	 FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE, EVENT(scTaopoSwampPushChestRockIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EXIT_ROCK,
	 FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE, EVENT(scTaopoSwampPushExitRockIn)},
	{TRIGGER_TABLE_END},
};

TRIGGER TaopoSwampStarDustTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS,
	 TAOPO_SWAMP_STAR_DUST_TO_TAOPO_SWAMP_TREMOR},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, STAR_DUST_FIRST_TO_MIDDLE_ROOM},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, STAR_DUST_MIDDLE_TO_FIRST_ROOM},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS, STAR_DUST_MIDDLE_TO_LAST_CHAMBER},
	{TRIGGER_EXIT, 6, CONDITION_ALWAYS, STAR_DUST_LAST_CHAMBER_TO_MIDDLE},
	{EXAMINE_TRIGGER(EXAMINE_GROUND), TRIGGER_STAR_DUST,
	 FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP, EVENT(scTaopoSwampShowSparkleMessage)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_DARK_ROCK,
	 FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED, EVENT(scTaopoSwampCoolDarkRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_LAST_ROCK,
	 FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED, EVENT(scTaopoSwampCoolLastRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DARK_ROCK,
	 FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE, 0},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_ROCK,
	 FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE, 0},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
	 TRIGGER_STAR_DUST, FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP,
	 EVENT(scTaopoSwampScoopStarDust)},
	{TRIGGER_TABLE_END},
};

/* The Star Dust's sparkle, in frames from 0 to 59. */
s32 TaopoSwamp_SparkleFrame = 0;
