// Gaia Rock's inside, under the great rock near Izumo where the Serpent lives, in five scenes of
// one overlay.
//
// The cave mouth on the first cliffs (gaia_rock.c) opens onto the first altar's room
// (SceneIdGaiaRockInsideFirstAltar), where two great red faces on the north wall stand in two
// doorways, one either side of an altar. The Dancing Idol, found on the altar on top of the rock,
// dances on this altar and the faces slide aside (scGaiaRockInsidePlaceDollOnFirstAltar). Three
// ways open: through the west and east doorways to the room of leafy platforms
// (SceneIdGaiaRockInsidePlatforms), and a third, past two stumps Felix Moves out of the way, to the
// maze.
//
// Cyclone on the bushes of the platforms' room sets its leafy platforms swaying
// (scGaiaRockInsideCycloneStartsPlatforms), and they carry the party on to the twin altars' room
// (SceneIdGaiaRockInsideTwinAltars) and the altar room above it (SceneIdGaiaRockInsideAltar). There
// Cyclone flushes a Mad Plant out of the bushes, three stone heads burst out of a wall of handholds
// under Felix, and Cyclone tears a great cobweb so that the altar it holds falls into the twin
// altars' room. Four beams of sunlight can be let down into the Serpent's lair: the Dancing Idol
// makes the face over each of three altars slide aside, and Cyclone clears the bushes under the
// fourth beam in the twin altars' room. While the Serpent lives, the game shows each new beam
// falling into the lair.
//
// The maze (SceneIdGaiaRockInsideMaze by entrances 1 to 4) is fourteen rooms alike, a crossroads
// with a little plant in the middle that Growth turns into an arrow pointing the way on. Beyond it
// is the Serpent's lair (the same scene by entrances 9 to 20), where Susa feeds the Serpent casks
// of Dragonsbane to save Kushinada. The fewer beams reach the lair, the more the Serpent heals in
// battle (scGaiaRockInsideChallengeSerpent). Once the party beats it Susa slays it, and a slate
// rises from the floor that teaches Felix Sand (scGaiaRockInsideReadSandTablet). With Sand he
// passes the stone Serpent to the Cloud Brand behind it (scGaiaRockInsideFindCloudBrand).

#include "scene/script_layer.h"
#include "macros.h"
#include "item_ids.h"
#include "field/field_mode.h"
#include "field/script_inline.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "edition.h"
#include "scene/scene.h"
#include "field/call.h"
#include "shared/gaia_rock.h"
#include "field/actor_script.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

// The four beams of sunlight that reach the Serpent's lair. Three are set once the face over the
// altar room's altar, the west altar or the east altar has slid aside, the fourth once Cyclone has
// cleared the bushes under the twin altars' room's beam. scGaiaRockInsideDrawSunbeams draws them in
// the lair.
#define FLAG_ALTAR_ROOM_SUNBEAM         0x9bc
#define FLAG_WEST_ALTAR_SUNBEAM         0x9bd
#define FLAG_EAST_ALTAR_SUNBEAM         0x9be
#define FLAG_BUSHES_SUNBEAM             0x9bf
// A beam let in before the Serpent is slain, waiting to be shown falling into the lair (scInit,
// entrance 20), which then sets its flag above.
#define FLAG_ALTAR_ROOM_SUNBEAM_TO_SHOW FLAG_AREA_TEMP_07
#define FLAG_WEST_ALTAR_SUNBEAM_TO_SHOW FLAG_AREA_TEMP_08
#define FLAG_EAST_ALTAR_SUNBEAM_TO_SHOW FLAG_AREA_TEMP_09
#define FLAG_BUSHES_SUNBEAM_TO_SHOW     FLAG_AREA_TEMP_0A
#define FLAG_SUSA_ATTACKED_SERPENT      0x9ac // "At last... The serpent has finally had its fill."
#define FLAG_MET_SUSA                   0x9de // "Who are you? How did you get here?"
#define FLAG_LEARNED_SAND               0x9ba // Felix touched the slate and learned Sand
// the slate rose from the lair's floor
#define FLAG_SAND_TABLET_RAISED         0x9ee
#define FLAG_ALTAR_FELL                 0x9c4 // Cyclone tore the cobweb holding it up
#define FLAG_FIRST_ALTAR_FACES_MOVED    0xa62
#define FLAG_PLATFORMS_SWAYING          0xa63
// The three stone heads that burst out of the altar room's wall of handholds under Felix; each
// stays out until the party enters another area.
#define FLAG_WEST_STONE_HEAD_RISEN      FLAG_AREA_TEMP_03
#define FLAG_MIDDLE_STONE_HEAD_RISEN    FLAG_AREA_TEMP_04
#define FLAG_EAST_STONE_HEAD_RISEN      FLAG_AREA_TEMP_05
// Where the first altar's room's two stumps were Moved to, kept so that they stay there
// (scGaiaRockInsideRecordStumpSpots).
#define FLAG_STUMP_SPOTS                FLAG_SCENE_TEMP_10
// While the party stands in Susa's way the cask waits for him, and while it stands in the cask's
// way Susa waits for it (scGaiaRockInsideCheckBlocked).
#define FLAG_SUSA_WAITS_FOR_CASK        FLAG_SCENE_TEMP_05
#define FLAG_CASK_WAITS_FOR_SUSA        FLAG_SCENE_TEMP_06

// The metatile behaviors of the cue tables, by room.
typedef enum GAIA_ROCK_INSIDE_METATILE_BEHAVIOR
{
	// The four ways out of a maze room, in the order of MAZE_WAY.
	METATILE_BEHAVIOR_MAZE_SOUTH = 1,
	METATILE_BEHAVIOR_MAZE_WEST,
	METATILE_BEHAVIOR_MAZE_EAST,
	METATILE_BEHAVIOR_MAZE_NORTH,
	// The bushes Cyclone clears: the patch in the platforms' room, and the cross of five under the
	// beam of sunlight in the twin altars' room.
	METATILE_BEHAVIOR_PLATFORMS_BUSHES = 20,
	METATILE_BEHAVIOR_SUNBEAM_BUSHES   = 20,
	// The altar room's wall of handholds: its three trap holds, west to east, in its overhead layer
	// (scGaiaRockCueWallMetatile reads them as Felix climbs).
	METATILE_BEHAVIOR_WEST_STONE_HEAD  = 30,
	METATILE_BEHAVIOR_MIDDLE_STONE_HEAD,
	METATILE_BEHAVIOR_EAST_STONE_HEAD,
	// The altar room's bushes: those ringing the altar, those over the Mad Plant and the one it
	// hides under, the cobweb, and the ledge at the cobweb's north edge (a metatile of the web for
	// Cyclone).
	METATILE_BEHAVIOR_ALTAR_BUSHES = 40,
	METATILE_BEHAVIOR_MAD_PLANT_BUSHES,
	METATILE_BEHAVIOR_COBWEB,
	METATILE_BEHAVIOR_COBWEB_LEDGE,
	METATILE_BEHAVIOR_MAD_PLANT,
	// The lair: the steps just east of where Susa works, the floor before the Serpent, the cup of
	// Dragonsbane at its feet, the Serpent itself, and the Cloud Brand's spot behind its tail.
	METATILE_BEHAVIOR_BY_SUSA        = 40,
	METATILE_BEHAVIOR_BEFORE_SERPENT = 42,
	METATILE_BEHAVIOR_CUP            = 50,
	METATILE_BEHAVIOR_SERPENT        = 55,
	METATILE_BEHAVIOR_SERPENT_WEST,
	METATILE_BEHAVIOR_CLOUD_BRAND = 77
} GAIA_ROCK_INSIDE_METATILE_BEHAVIOR;

// The maze's one actor, in the middle of each of its rooms, and its treasures in scene_items.s's
// order: the Mimic's chest in the third room, the Rusty Mace's in the ninth, and the Cloud Brand
// hidden behind the stone Serpent in the lair.
enum
{
	// A sprig that Growth turns into an arrow pointing the way on
	// (scGaiaRockInsidePlantPointsTheWay).
	ACTOR_ARROW_PLANT = 8,
	ACTOR_MIMIC_CHEST = FIELD_TREASURE_ACTOR,
	ACTOR_RUSTY_MACE_CHEST,
	ACTOR_CLOUD_BRAND
};

// Leaves the Mimic's chest out of the maze's third room while set. Nothing in the code sets it.
#define FLAG_MIMIC_CHEST_LEFT_OUT FLAG_AREA_TEMP_F6

// The exit the lair would take after showing a beam if none waited to be shown: the scene has no
// exit of this number, but the party only comes in by entrance 20 from a script that has just let a
// beam in.
#define EXIT_NO_SUNBEAM           20

// The passages within the first altar's room and the altar room, from one side of a doorway to the
// other (scene_exits.h names only those between scenes). An exit and the entrance on its side of
// the doorway share a number. In the first altar's room, the doorway east of the altar leads to the
// stumps and the maze, and the passages north of the two faces' doorways to the leafy platforms. In
// the altar room, the three doorways along the wall of handholds lead to the west and east sides of
// the altar ringed by bushes and to the great cobweb; the middle one comes in by
// GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE_A and the east one by
// GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_A.
typedef enum GAIA_ROCK_INSIDE_PASSAGE
{
	GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_STUMPS = 2,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_STUMPS_TO_ALTAR,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_WEST_PASSAGE = 5,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_WEST_PASSAGE_TO_ALTAR,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_EAST_PASSAGE = 8,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_PASSAGE_TO_ALTAR,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_STUMPS = 2,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_STUMPS_FROM_ALTAR,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_WEST_PASSAGE = 5,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_WEST_PASSAGE_FROM_ALTAR,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_EAST_PASSAGE = 8,
	GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_PASSAGE_FROM_ALTAR,
	GAIA_ROCK_INSIDE_ALTAR_WALL_TO_ALTAR_WEST = 1,
	GAIA_ROCK_INSIDE_ALTAR_ALTAR_WEST_TO_WALL,
	GAIA_ROCK_INSIDE_ALTAR_WALL_TO_ALTAR_EAST,
	GAIA_ROCK_INSIDE_ALTAR_ALTAR_EAST_TO_WALL,
	GAIA_ROCK_INSIDE_ALTAR_WALL_TO_COBWEB,
	GAIA_ROCK_INSIDE_ALTAR_COBWEB_TO_WALL,
	GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_ALTAR_WEST = 1,
	GAIA_ROCK_INSIDE_ALTAR_ALTAR_WEST_FROM_WALL,
	GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_ALTAR_EAST,
	GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_COBWEB = 5
} GAIA_ROCK_INSIDE_PASSAGE;

// The cue Susa's hook raises when the party has stood in his or his cask's way for half a second
// (scGaiaRockInsideCheckBlocked).
#define RAISED_SUSA_BLOCKED 42

// The lair's entrances: its way in from the south, and where the battle with the Serpent brings the
// party back: won, lost without the Dancing Idol, and lost holding it before any beam of sunlight
// reaches the lair (scGaiaRockInsideChallengeSerpent).
enum
{
	ENTRANCE_LAIR                = 9,
	ENTRANCE_SERPENT_BEATEN      = 10,
	ENTRANCE_BEATEN_BY_SERPENT   = 11,
	ENTRANCE_BEATEN_HOLDING_IDOL = 12
};

// The Serpent's lair (SceneIdGaiaRockInsideMaze by entrances 9 to 20). The Serpent sits on the sand
// at its north end; Susa works among the casks south of it.
enum
{
	ACTOR_SERPENT     = 8,
	// The cask of Dragonsbane Susa feeds the Serpent from: "How many casks does it take to weaken
	// this stupid serpent?"
	ACTOR_CASK        = 9,
	ACTOR_SUSA        = 10,
	// Kraden, who points Felix to the slate once the Serpent is beaten.
	ACTOR_KRADEN      = 11,
	// The slate that teaches Sand (scGaiaRockInsideReadSandTablet).
	ACTOR_SAND_TABLET = 12
};

// The room of the first altar (SceneIdGaiaRockInsideFirstAltar): the two great red faces that stand
// in the doorways either side of the altar until the Dancing Idol dances on it, and the two stumps
// Felix Moves out of the way on the path to the maze.
enum
{
	ACTOR_FIRST_ALTAR_WEST_FACE = 8,
	ACTOR_FIRST_ALTAR_EAST_FACE = 9,
	ACTOR_STUMP_A               = 10,
	ACTOR_STUMP_B               = 11,
	ACTOR_FIRST_ALTAR           = 12,
	// The Dancing Idol, set on the altar while it dances.
	ACTOR_FIRST_ALTAR_DOLL      = 13
};

// The room of the twin altars (SceneIdGaiaRockInsideTwinAltars): a face on the north wall over each
// altar. The east altar is the one that falls through from the altar room when Cyclone tears its
// cobweb.
enum
{
	ACTOR_WEST_FACE        = 8,
	ACTOR_EAST_FACE        = 9,
	ACTOR_WEST_ALTAR       = 10,
	ACTOR_EAST_ALTAR       = 11,
	ACTOR_TWIN_ALTARS_DOLL = 12
};

// The nine leafy platforms Cyclone sets swaying in the platforms' room
// (scGaiaRockInsideStartPlatforms).
enum
{
	ACTOR_FIRST_PLATFORM = 8,
	ACTOR_LAST_PLATFORM  = 16
};

// The altar room (SceneIdGaiaRockInsideAltar): the altar ringed by bushes in its west part, with a
// face on the wall beside it, the wall of handholds along its south part, and the altar on the
// great cobweb in its east part.
enum
{
	ACTOR_ALTAR_FACE        = 8,
	ACTOR_ALTAR             = 9,
	ACTOR_HANGING_VINE      = 10,
	// The three stone heads that burst out of the wall of handholds under Felix
	// (scStoneHeadBurstOut).
	ACTOR_WEST_STONE_HEAD   = 11,
	ACTOR_MIDDLE_STONE_HEAD = 12,
	ACTOR_EAST_STONE_HEAD   = 13,
	ACTOR_ALTAR_DOLL        = 14,
	// The altar the cobweb holds up; Cyclone tears the web and it falls a floor
	// (scGaiaRockInsideCycloneClearsCobweb).
	ACTOR_FALLING_ALTAR     = 15,
	ACTOR_GANGWAY           = 16
};

struct ACTOR;

// A grid metatile's step that marks the grid's border.
#define MAZE_BORDER 99

// One grid metatile: the step that stands on it (0 for none, MAZE_BORDER for the border) and the
// column and row of the next step.
typedef struct
{
	Sint8 step;
	Sint8 nextX;
	Sint8 nextY;
} PATH_METATILE;

// The steps of the maze's path, from the way in to the way on.
#define MAZE_STEPS 14

// One step of the path: its number, then the step found below, to the left, to the right and above
// it.
typedef struct
{
	Uint8 number;
	Uint8 spare;
	Uint8 next[4]; // the step each way leads to, by MAZE_WAY
	Uint8 padding[2];
} PATH_STEP;

// The four ways out of a step, in the order PATH_STEP.next keeps them.
typedef enum MAZE_WAY
{
	MAZE_SOUTH,
	MAZE_WEST,
	MAZE_EAST,
	MAZE_NORTH
} MAZE_WAY;

// PATH_STEP.next's marks for the ways out of the maze, each the exit it takes: back out from its
// first step, and on from its last.
#define MAZE_BACK 250
#define MAZE_EXIT 251

// The maze's path: the step the party stands on, set to the first once the path is laid, and the
// steps, from 1 to MAZE_STEPS.
typedef struct
{
	Sint32    current;
	PATH_STEP steps[MAZE_STEPS + 1];
} PATH;

// The maze's path, kept 256 bytes into the inventory snapshot's spare room.
#define PathWork                      ((PATH *)(InventorySnapshot + INVENTORY_SNAPSHOT_SPARE + 256))

// Set once the plant on a step of the maze has pointed the way on; it points again whenever the
// party comes back to that step.
#define FLAG_MAZE_PLANT_POINTED(step) (0xa50 + (step))
// The maze's path is laid for this visit (scGaiaRockInsideLayMaze).
#define FLAG_MAZE_LAID                FLAG_AREA_TEMP_F0
// The plant of the step the party stands on points the way: Growth on it does nothing more.
#define FLAG_PLANT_POINTING           FLAG_SCENE_TEMP_00

// A map grid metatile, its column and row.
typedef struct METATILE_POINT
{
	Sint16 pos[2];
} METATILE_POINT;

// The platforms' sway (scGaiaRockInsideSwayPlatforms).
typedef struct SWAY_STATE
{
	Uint16 frame;   // ticks into the sway's 240
	Uint16 pending; // ticks still to run, which a push holds back
} SWAY_STATE;

extern Uint32                               GaiaRockInsideBurstScript[];
extern Sint32                               GaiaRockInsideCaskTossedLeftScript[];
extern Sint32                               GaiaRockInsideCaskTossedRightScript[];

extern ACTOR_PLACE                          GaiaRockInsidePlacements[];
extern ACTOR_PLACE                          GaiaRockInsideLairPlacements[];
extern ACTOR_PLACE                          GaiaRockInsideTwinAltarsActorPlacements[];
extern ACTOR_PLACE                          GaiaRockInsidePlatformsActorPlacements[];
extern ACTOR_PLACE                          GaiaRockInsideAltarActorPlacements[];
extern Sint32                               GaiaRockInsideSquashYScript[];

extern Uint32                               GaiaRockInsideLeaderStepsBackScript[];
extern Sint32                               GaiaRockInsideAltarShakesScript[];
extern Sint32                               GaiaRockInsideAltarDropsScript[];

ACTOR GaiaRockInsideSavedSusa               BSS_DATA;
Sint32 GaiaRockInsideSavedSusaX             BSS_DATA;
Sint32 GaiaRockInsideSavedSusaZ             BSS_DATA;
Sint32 GaiaRockInsideSavedSusaAnimation     BSS_DATA;
Sint32 GaiaRockInsideSavedSusaAnimationStep BSS_DATA;

extern Sint32                               GaiaRockInsideCaskThrownScript[];
extern Sint32                               GaiaRockInsideCaskEmptiesScript[];

void                                        scGaiaRockInsideSaveSusa(void);
void                                        scGaiaRockInsideRestoreSusa(void);

void               scGaiaRockInsidePlacePlatform(Sint32 id, Sint32 x, Sint32 z);

void               scGaiaRockInsideSetPlatformMode(Uint32 id, Sint8 mode);
void               scGaiaRockInsideSwayPlatforms(void);

void               scGaiaRockInsideBlankLayer(Sint32 layer, Sint32 keep);

extern Uint16      GaiaRockInsideLairBlendScript[12];

void               scGaiaRockInsideLayMaze(Sint32 level);
void               scGaiaRockInsideDrawMazeRoom(void);
Sint32             scGaiaRockInsideGetMazeWayOn(void);

// Copies whole metatiles as the map stores them, from a script.
static inline void scMapGridCopyRawMetatiles(
	Sint32 x, Sint32 z, Sint32 width, Sint32 height, Sint32 metatileX, Sint32 metatileZ)
{
	fdMapCopyRawMetatilesRect(x, z, width, height, metatileX, metatileZ);
}

extern Uint32         FieldEffectShortScript[];
extern Uint32         FieldEffectMediumScript[];
extern Uint32         FieldEffectLongScript[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

extern SCENE_REGION GaiaRockInsideAltarExitRegions[];

const SCENE_REGION *scGetExitRegions(void)
{
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
		return GaiaRockInsideAltarExitRegions;
	return 0;
}

// An actor springs down off the ledge before it (fdActorJumpDownLedge).
static __inline__ void scGaiaRockInsideJumpDownLedge(Sint32 id, Sint32 pose, Sint32 param)
{
	fdActorJumpDownLedge(id, pose, param);
}

// Felix walks south off the ledge at the north edge of the altar room's cobweb pit, metatile (44,
// 20), springs down past the altar through the web and drops out of sight; he lands in the twin
// altars' room below in a ring of dust (scGaiaRockInsideRaiseDust). Once the altar has fallen he
// jumps into the empty pit the same way.
void scGaiaRockInsideJumpDownCobwebPit(void)
{
	scFieldActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 3);
	fdActorCreateSilhouette(toFIXED(712), 0, toFIXED(468), SPRITE_CRATE);
	fdActorCreateSilhouette(toFIXED(712), 0, toFIXED(490), SPRITE_CRATE);
	// 0x800 is no colour the dust's byte keeps, and not the bit that skips the wait for the
	// landing, which Taopo Swamp's and Ankohl Ruins' drops like this one pass: the jump goes as
	// with 0.
	scGaiaRockInsideJumpDownLedge(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SPRING, 0x800);
	fdScriptPlaySound(SE_FALL);
	fdScriptSetFlag(FLAG_ARRIVE_FALLING);
	syFieldScriptSetExitRequest(
		GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_B);
}

// Includes the shared fdEffectAdvanceMotion in this scene. Neither it nor its caller
// scGaiaRockInsideRaiseDust exists in the Japanese scene.
#if PIT_FALL_DUST_LANDING
#	include "../field/field_effect_advance_motion.c"
#endif

#if PIT_FALL_DUST_LANDING
// Felix lands in the twin altars' room after jumping down the altar room's cobweb pit
// (scGaiaRockInsideJumpDownCobwebPit): he drops in from 130 pixels up, east of the east altar's
// face, into a ring of dust with a thump that shakes the room, and sits a moment before he stands.
// The Japanese edition has no form of this function here.
void scGaiaRockInsideRaiseDust(void)
{
	ACTOR         *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Uint32         i;
	EFFECT_OPTIONS options;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	actor->y.fixed             = toFIXED(130);
	actor->gravity             = toFIXED(1) / 2;
	actor->bounce              = 0;
	actor->motionFlags         = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptPlaySound(SE_DUST_PUFF);
	actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldScriptWait(24);
	options.palette     = SPRITE_COLOR_WHITE;
	options.update      = fdEffectAdvanceMotion;
	options.startScaleX = toFIXED(16) / 25;
	options.startScaleY = toFIXED(16) / 25;
	for (i = 0; i <= DUST_RING_PUFFS; i++)
	{
		Sint32 angle;
		FIXED  velocity[3];
		FIXED  cosine;

		angle        = i * FACING_STEP;
		cosine       = syCos(angle);
		velocity[0]  = cosine;
		velocity[1]  = 0;
		velocity[2]  = syMathSin(angle);
		velocity[0] += velocity[0] >> 1;
		velocity[0] += (syRandomBelow(toFIXED(3) / 4)) - toFIXED(3) / 8;
		velocity[2] += (syRandomBelow(toFIXED(3) / 8)) - toFIXED(3) / 16;
		scFieldEffectSpawn(actor->x.fixed,
		                   actor->y.fixed,
		                   actor->z.fixed,
		                   velocity[0],
		                   velocity[1],
		                   velocity[2],
		                   EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
		                   &options);
	}
	fdScriptPlaySound(SE_THUMP);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	actor->gravity = toFIXED(1);
	actor->bounce  = toFIXED(1) / 4;
	syFieldScriptWait(10);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdScriptEnd();
}
#endif

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

// Susa feeds the Serpent Dragonsbane cask by cask, over and over
// (GaiaRockInsideSusaFeedsSerpentScript): he sets a cask down among the others south of the
// Serpent, pushes it a metatile east, and throws it up onto the cup at the Serpent's feet, where it
// tips and pours; then he tosses the empty cask aside, where it bursts. The functions below are its
// steps.
//
// The cask lands and holds still on its third animation.
Sint32 scGaiaRockInsideCaskRests(void)
{
	ACTOR *cask;

	cask = fdActorGet(ACTOR_CASK);
	fdActorSelectAnimation(cask, 3);
	fdActorSetAnimationSpeed(cask, 0);
	return 0;
}

// The cask tips a step over the cup.
Sint32 scGaiaRockInsideCaskTips(void)
{
	ACTOR *actor;

	actor                                   = fdActorGet(ACTOR_CASK);
	((FIELD_SPR *)actor->sprite)->rotation += TRIG_TURN * 3 / 256;
	return 0;
}

extern Uint32 GaiaRockInsidePourScript[];

// The cask rights itself a step.
Sint32        scGaiaRockInsideCaskRights(void)
{
	FIELD_SPR *sprite  = fdActorGet(ACTOR_CASK)->sprite;

	sprite->rotation  -= TRIG_TURN * 3 / 256;
	return 0;
}

// Each tick the pour's size varies at random; shrunk below a sixteenth, it jumps back to an eighth.
void scGaiaRockInsideFlickerPour(ACTOR *actor)
{
	Uint32 random = syRandom();
	Sint32 scale  = actor->scaleX;
	Sint32 bias;
	Sint32 limit;

	bias           = -toFIXED(1) / 32;
	scale         += FIXEDtoINT(random << 12);
	scale         += bias;
	actor->scaleX  = scale;
	actor->scaleY  = actor->scaleY + syRandomBelow(4096) + bias;
	scale          = actor->scaleX;
	limit          = toFIXED(1) / 16 - 1;
	if (scale <= limit)
		actor->scaleX = toFIXED(1) / 8;
	if (actor->scaleY <= limit)
		actor->scaleY = toFIXED(1) / 8;
}

// The cask pours: a spray of Dragonsbane that flickers in size (scGaiaRockInsideFlickerPour).
Sint32 scGaiaRockInsideCaskPours(void)
{
	ACTOR     *src = fdActorGet(ACTOR_CASK);
	ACTOR     *actor;
	FIELD_SPR *sprite;

	actor              = (scFieldActorCreate(
        SPRITE_WADING_SPLASH, src->x.fixed + toFIXED(8), src->y.fixed - toFIXED(2), src->z.fixed));
	actor->motionFlags = 0;
	fdActorSetSpriteFlags(actor, 0);
	fdActorStartScript(actor, GaiaRockInsidePourScript);
	actor->scaleX    = toFIXED(2) / 5;
	actor->scaleY    = toFIXED(2) / 5;
	sprite           = actor->sprite;
	sprite->priority = 1;
	actor->bounce    = 0;
	actor->sceneWord = 0;
	actor->update    = (OBJECT_UPDATE)scGaiaRockInsideFlickerPour;
	return 0;
}

extern Sint32 GaiaRockInsideCaskSetDownScript[];

// Susa sets a new cask down at his feet.
Sint32        scGaiaRockInsideSetDownCask(void)
{
	fdActorStartScript(((Sint32)fdActorGet(ACTOR_CASK)), (Sint32)GaiaRockInsideCaskSetDownScript);
	return 0;
}

extern Uint32 GaiaRockInsideCaskPushedScript[];

// Susa pushes the cask a metatile east, straining.
Sint32        scGaiaRockInsideSusaPushesCask(void)
{
	fdActorStartScript(((Sint32)fdActorGet(ACTOR_CASK)), (Sint32)GaiaRockInsideCaskPushedScript);
	fdActorSetAttachedEffect(ACTOR_SUSA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	return 0;
}

// Susa throws the cask up onto the cup at the Serpent's feet.
Sint32 scGaiaRockInsideSusaThrowsCask(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_CASK);
	fdActorStartScript(actor, (Sint32)GaiaRockInsideCaskThrownScript);
	actor->x.fixed = toFIXED(472);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL);
	fdActorSetAttachedEffect(ACTOR_SUSA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	return 0;
}

// On the cup the cask tips, pours and rights itself again.
Sint32 scGaiaRockInsideCaskEmpties(void)
{
	ACTOR *cask;

	cask = fdActorGet(ACTOR_CASK);
	fdActorStartScript(cask, (Sint32)GaiaRockInsideCaskEmptiesScript);
	fdActorSetAnimationSpeed(cask, 0);
	fdActorSetAttachedEffect(ACTOR_SUSA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	return 0;
}

#include "../field/field_effect_advance_motion_priority.c"

#include "text/msg_ids.h"

// The tossed cask bursts in four puffs of dust.
Sint32 scGaiaRockInsideCaskBursts(void)
{
	ACTOR *src = fdActorGet(ACTOR_CASK);
	Sint32 i;

	fdScriptPlaySound(SE_FLAME_FLARE);
	for (i = 0; i < 4; i++)
	{
		{
			ACTOR *dust =
				(scFieldActorCreate(SPRITE_DUST_CLOUD, src->x.fixed, src->y.fixed, src->z.fixed));

			if (dust != 0)
			{
				dust->motionFlags    = 0;
				dust->collisionFlags = 0;
				fdActorSetSpriteFlags(dust, 0);
				fdActorSelectAnimation(dust, 1);
				fdActorStartScript(dust, GaiaRockInsideBurstScript);
				((FIELD_SPR *)dust->sprite)->priority = 1;
				dust->bounce                          = syCos(syRandomBelow(8) << 14) >> 1;
				dust->sceneWord                       = syMathSin(syRandomBelow(8) << 14) >> 1;
				dust->gravity                         = 0;
				dust->timer                           = syRandomBelow(512) - 256;
				dust->speedLimit                      = 0;
				dust->acceleration                    = 0;
				dust->update                          = fdEffectAdvanceMotionAtLeaderPriority;
			}
		}
	}
	return 0;
}

// The empty cask is tossed aside, west or east, away from where the leader stands.
Sint32 scGaiaRockInsideTossCask(void)
{
	ACTOR *actor  = fdActorGet(ACTOR_CASK);
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	if (FIELD_HALF_METATILE(leader->z.fixed) <= 45 && FIELD_HALF_METATILE(leader->x.fixed) <= 57)
		fdActorStartScript(actor, GaiaRockInsideCaskTossedRightScript);
	else
		fdActorStartScript(actor, GaiaRockInsideCaskTossedLeftScript);
	return 0;
}

// The ticks Susa or the cask may be held up by the party before he tells it to move, and the most
// the count keeps.
enum
{
	SUSA_BLOCKED_TICKS     = 30,
	SUSA_BLOCKED_TICKS_MAX = 35
};

// Susa's hook while he works: when the party has stood in his or the cask's way for
// SUSA_BLOCKED_TICKS, he tells them to move (scGaiaRockInsideSusaSaysMoveAside); while one is held
// up the other waits.
void scGaiaRockInsideCheckBlocked(ACTOR *actor)
{
	ACTOR *other = fdActorGet(ACTOR_CASK);

	if (actor->refused == SUSA_BLOCKED_TICKS || other->refused == SUSA_BLOCKED_TICKS)
	{
		FieldStateWork->raisedCue = RAISED_SUSA_BLOCKED;
		if (actor->refused == SUSA_BLOCKED_TICKS)
			actor->refused = SUSA_BLOCKED_TICKS + 1;
		if (other->refused == SUSA_BLOCKED_TICKS)
			other->refused = SUSA_BLOCKED_TICKS + 1;
	}
	if (actor->refused > 2)
	{
		other->paused = 1;
		fdScriptSetFlag(FLAG_CASK_WAITS_FOR_SUSA);
	}
	{
		Uint8 *otherCount = &other->refused;

		if (*otherCount > 2 && !fdScriptGetFlag(FLAG_CASK_WAITS_FOR_SUSA)
		    && other->script != GaiaRockInsideCaskTossedLeftScript
		    && other->script != GaiaRockInsideCaskTossedRightScript)
		{
			actor->paused = 1;
			fdScriptSetFlag(FLAG_SUSA_WAITS_FOR_CASK);
		}
		if (actor->refused == 0)
		{
			other->paused = 0;
			fdScriptClearFlag(FLAG_CASK_WAITS_FOR_SUSA);
		}
		if (*otherCount == 0)
		{
			actor->paused = 0;
			fdScriptClearFlag(FLAG_SUSA_WAITS_FOR_CASK);
		}
		if (actor->refused > SUSA_BLOCKED_TICKS_MAX)
			actor->refused = SUSA_BLOCKED_TICKS_MAX;
		if (*otherCount > SUSA_BLOCKED_TICKS_MAX)
			*otherCount = SUSA_BLOCKED_TICKS_MAX;
	}
}

extern ACTOR_PLACE GaiaRockInsideFirstAltarActorPlacements[];

extern Sint32      GaiaRockInsideArrowTick;
extern Sint32      GaiaRockInsideSquashXScript[];

// Returns the current room's actors; the maze's rooms and the lair share a scene, told apart by
// entrance.
const void        *scGetActorPlacements(void)
{
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR)
		return GaiaRockInsideFirstAltarActorPlacements;
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS)
		return GaiaRockInsideTwinAltarsActorPlacements;
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_PLATFORMS)
		return GaiaRockInsidePlatformsActorPlacements;
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
		return GaiaRockInsideAltarActorPlacements;
	if (MainRoutineStateWork.entrance >= ENTRANCE_LAIR)
		return GaiaRockInsideLairPlacements;
	return GaiaRockInsidePlacements;
}

// The arrow plant's animation for the heading of the way on: east, south, west and north from
// animation 2; and whether that heading is south or north, the arrow up or down.
#define ARROW_ANIMATION(heading)  ((((heading) & 0xc000) >> 14) + 2)
#define ARROW_UP_OR_DOWN(heading) ((heading) & 0x4000)

// The update of a maze plant Growth has turned into an arrow: it flashes, and about twice a second
// stretches along its arrow with a drip, sideways here and up and down in
// scGaiaRockInsidePulseUpDownArrow.
void scGaiaRockInsidePulseSidewaysArrow(ACTOR *actor)
{
	Sint32 colorMode = 0;

	if ((GaiaRockInsideArrowTick & 7) <= 0)
		colorMode = 7;
	fdActorSetSpriteColorMode(actor, colorMode & 7);
	if ((GaiaRockInsideArrowTick & 31) == 0)
	{
		actor->scaleX = toFIXED(1);
		fdActorStartScript(actor, GaiaRockInsideSquashXScript);
		fdScriptPlaySound(SE_DRIP);
	}
	GaiaRockInsideArrowTick++;
}

// As scGaiaRockInsidePulseSidewaysArrow, for an arrow pointing up or down.
void scGaiaRockInsidePulseUpDownArrow(ACTOR *actor)
{
	Sint32 colorMode = 0;

	if ((GaiaRockInsideArrowTick & 7) <= 0)
		colorMode = 7;
	fdActorSetSpriteColorMode(actor, colorMode & 7);
	if ((GaiaRockInsideArrowTick & 31) == 0)
	{
		actor->scaleY = toFIXED(1);
		fdActorStartScript(actor, GaiaRockInsideSquashYScript);
		fdScriptPlaySound(SE_DRIP);
	}
	GaiaRockInsideArrowTick++;
}

// Felix casts Growth on the little plant in the middle of a maze room, and it grows into a green
// arrow that flashes and pulses, pointing the way on to the next room. It points again whenever the
// party comes back to that room on this visit.
void scGaiaRockInsidePlantPointsTheWay(Sint32 unused, Uint32 id)
{
	ACTOR  *actor = fdActorGet(id);
	Uint32  direction;
	Sint32 *state;

	direction             = scGaiaRockInsideGetMazeWayOn();
	state                 = &PathWork->current;
	actor->collisionFlags = 0;
	fdActorSelectAnimation(actor, ARROW_ANIMATION(direction));
	if (ARROW_UP_OR_DOWN(direction))
		actor->update = (OBJECT_UPDATE)scGaiaRockInsidePulseUpDownArrow;
	else
		actor->update = (OBJECT_UPDATE)scGaiaRockInsidePulseSidewaysArrow;
	actor->collisionFlags = 1;
	fdScriptSetFlag(FLAG_MAZE_PLANT_POINTED(*state));
	fdScriptSetFlag(FLAG_PLANT_POINTING);
	syWait(20);
}

// Felix casts Cyclone on the cross of five bushes south-east of the east face in the twin altars'
// room, around metatile (38, 25), where a beam of sunlight shines down from the ceiling. Once the
// middle bush is gone the beam falls through the hole it hid, metatile (38, 25), and down into the
// Serpent's lair; Felix hops aside if he stands on the hole. While the Serpent lives, the game
// first shows the beam falling into the lair.
void scGaiaRockInsideCycloneClearsTwinAltarsBushes(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(37, 26, 0, 3, 3, 61, 0);
	if (syFieldMapGridGetMetatileBehaviorAt(2, toFIXED(608), toFIXED(400)) == 0)
	{
		fdMapGridCopyMetatileAttributes(36, 23, 1, 1, 38, 25);
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		if (FIELD_METATILE(leader->x.fixed) == 38 && FIELD_METATILE(leader->z.fixed) == 25)
		{
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
			                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 16, 0);
			leader->velocityY = toFIXED(6);
			scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		}
		if (!fdScriptGetFlag(FLAG_SERPENT_SLAIN))
		{
			fdScriptSetFlag(FLAG_BUSHES_SUNBEAM_TO_SHOW);
			syFieldScriptWait(30);
			fdActorTableSnapshot();
			syFieldScriptSetExitRequest(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_MAZE);
		}
		else
		{
			fdScriptSetFlag(FLAG_BUSHES_SUNBEAM);
			fdScriptEnd();
		}
	}
}

static __inline__ Sint32 scGameFlagTest(Sint32 flag)
{
	return syGameFlagGet(flag);
}

// A Mad Plant leaps out at the leader (fdMadPlantLeapAtLeader).
static __inline__ void scGaiaRockInsideMadPlantLeaps(Sint32 sprite, Sint32 x, Sint32 z)
{
	fdMadPlantLeapAtLeader(sprite, x, z);
}

// Felix casts Cyclone on the patch of bushes in the platforms' room, metatiles (16, 25) to (20,
// 29): they blow away, and the first time nine leafy platforms appear over the room's holes and
// start to sway, which carry the party across.
void scGaiaRockInsideCycloneStartsPlatforms(void)
{
	syFieldCycloneCopyTouchedMetatiles(16, 25, 0, 5, 5, 40, 8);
	if (scGameFlagTest(FLAG_PLATFORMS_SWAYING) == 0)
	{
		fdScriptSetFlag(FLAG_PLATFORMS_SWAYING);
		scGaiaRockInsideStartPlatforms(2);
	}
}

// Felix casts Cyclone on the bushes north-west of the altar in the altar room, at the south-east
// bush of the patch, metatile (20, 21): they blow away, and until it is beaten a Mad Plant hiding
// there leaps out at him and a battle starts that the party cannot run from.
void scGaiaRockInsideCycloneMadPlant(void)
{
	syFieldCycloneCopyTouchedMetatiles(18, 17, 0, 3, 3, 31, 0);
	if (fdScriptGetFlag(FLAG_MAD_PLANT(2)) == 0)
	{
		scGaiaRockInsideMadPlantLeaps(SPRITE_MAD_PLANT, 328, 344);
		MainRoutineStateWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(2));
		SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_MAD_PLANTS, 2);
	}
}

// Felix casts Cyclone on the bushes ringing the altar in the altar room's west part, metatiles (19,
// 24) to (22, 26), and they blow away, leaving the altar clear.
void scGaiaRockInsideCycloneClearsAltarBushes(void)
{
	syFieldCycloneCopyTouchedMetatiles(19, 24, 0, 4, 3, 31, 4);
}

// Felix casts Cyclone on the Mad Plant's bushes, north-west of the altar in the altar room, away
// from the south-east bush it hides under, metatiles (18, 19) to (20, 21): they blow away, and no
// Mad Plant leaps out (scGaiaRockInsideCycloneMadPlant).
void scGaiaRockInsideCycloneClearsMadPlantBushes(void)
{
	syFieldCycloneCopyTouchedMetatiles(18, 17, 0, 3, 3, 31, 0);
}

extern METATILE_POINT GaiaRockInsideCobwebMetatiles[12];

// Felix casts Cyclone on the great cobweb in the altar room's east part, at any of its edge
// metatiles around (40, 20) to (47, 27). The web tears; the altar on it shakes, Felix steps aside
// if he stands just south of it, and the altar drops into the pit. The game then shows it landing
// in the twin altars' room below as the east altar, and brings the party back to the pit's edge.
void                  scGaiaRockInsideCycloneClearsCobweb(void)
{
	ACTOR *altar;
	ACTOR *leader;
	Sint16 count;
	Sint16 i;
	Sint32 height = 8;
	Sint32 srcX   = 117;
	Sint32 srcY   = 20;

	fdCycloneCopyTouchedMetatiles(104, srcY, 1, height, height, srcX, srcY);
	if (syGameFlagGet(FLAG_ALTAR_FELL))
		return;
	count = 0;
	for (i = 0; i < (Sint16)ARRAY_COUNT(GaiaRockInsideCobwebMetatiles); i++)
	{
		if (fdMapGridGetMetatileBehaviorAt(
				2,
				FIELD_METATILE_ORIGIN(GaiaRockInsideCobwebMetatiles[i].pos[0]),
				FIELD_METATILE_ORIGIN(GaiaRockInsideCobwebMetatiles[i].pos[1]))
		    == 0)
			count++;
	}
	if (count <= 0)
		return;
	altar  = fdActorGet(ACTOR_FALLING_ALTAR);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if (FIELD_HALF_METATILE(leader->z.fixed) > 53 && FIELD_HALF_METATILE(leader->x.fixed) > 83
	    && FIELD_HALF_METATILE(leader->x.fixed) <= 91)
	{
		leader->speedLimit = toFIXED(2);
		fdActorShowEmote(MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorSetScript(MainRoutineStateWork.leaderActorId, GaiaRockInsideLeaderStepsBackScript);
	}
	fdActorSetScript(ACTOR_FALLING_ALTAR, GaiaRockInsideAltarShakesScript);
	syFieldScriptWait(60);
	fdActorSetScript(ACTOR_FALLING_ALTAR, GaiaRockInsideAltarDropsScript);
	syFieldScriptWait(30);
	fdMapGridCopyMetatilesLayered(117, 20, 7, 8, 104, 20);
	fdMapGridCopyMetatilesLayered(111, 12, 1, 6, 111, 20);
	fdMapGridCopyMetatileAttributes(49, 84, 8, 8, 40, 84);
	syFieldScriptWait(30);
	fdActorCreateSilhouette(toFIXED(700), 0, toFIXED(468), SPRITE_CRATE);
	fdActorCreateSilhouette(toFIXED(700), 0, toFIXED(490), SPRITE_CRATE);
	fdActorCreateSilhouette(toFIXED(724), 0, toFIXED(468), SPRITE_CRATE);
	fdActorCreateSilhouette(toFIXED(724), 0, toFIXED(490), SPRITE_CRATE);
	fdScriptPlaySound(SE_DUST_PUFF);
	altar->motionFlags                     = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	((FIELD_SPR *)altar->sprite)->priority = 3;
	syFieldScriptWait(10);
	fdActorSetPos(ACTOR_FALLING_ALTAR, 0, 0);
	fdActorTableSnapshot();
	fdScriptSetFlag(FLAG_ALTAR_FELL);
	syFieldScriptSetExitRequest(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_B);
}

// Felix stands in the way of Susa or the cask he is pushing in the lair for half a second, and Susa
// says "Move out of the way there."
void scGaiaRockInsideSusaSaysMoveAside(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgGaiaRockInsideMoveOutOfTheWay);
	// "Move out of the way there."
	fdScriptShowMessage(ACTOR_SUSA, 0);
	fdScriptEnd();
}

// Stops Susa and the cask he is handling where they are, keeping Susa's place and animation to be
// given back after a talk.
void scGaiaRockInsideSaveSusa(void)
{
	ACTOR         *walker    = fdActorGet(ACTOR_CASK);
	ACTOR         *actor     = fdActorGet(ACTOR_SUSA);
	FIELD_SPR     *sprite    = actor->sprite;
	SPR_ANIMATION *animation = sprite->record;

	syWordsCopyEntry(&GaiaRockInsideSavedSusa, actor, sizeof(ACTOR));
	GaiaRockInsideSavedSusaAnimationStep = animation->step;
	GaiaRockInsideSavedSusaAnimation     = sprite->lastNo;
	{
		FIXED x = actor->x.fixed;
		FIXED z;

		GaiaRockInsideSavedSusaX = x;
		z                        = actor->z.fixed;
		actor->update            = 0;
		GaiaRockInsideSavedSusaZ = z;
		actor->velocityZ         = 0;
		actor->velocityY         = 0;
		actor->velocityX         = 0;
		actor->targetX           = x;
		actor->targetY           = actor->y.fixed;
		actor->targetZ           = z;
	}
	fdActorResetTargetsAndVelocity(ACTOR_SUSA);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 5 / 8);
	actor->speedLimit = toFIXED(1);
	if (walker->script == GaiaRockInsideCaskThrownScript
	    || walker->script == GaiaRockInsideCaskPushedScript)
	{
		walker->paused = 1;
		fdActorSetAnimationSpeed(walker, 0);
	}
}

// After a talk, Susa walks back to where scGaiaRockInsideSaveSusa found him and takes up his
// animation, and the cask goes on.
void scGaiaRockInsideRestoreSusa(void)
{
	ACTOR *walker = fdActorGet(ACTOR_CASK);
	ACTOR *actor  = fdActorGet(ACTOR_SUSA);

	if (GaiaRockInsideSavedSusaX != actor->x.fixed || GaiaRockInsideSavedSusaZ != actor->z.fixed)
	{
		Sint16 *savedZ = (Sint16 *)&GaiaRockInsideSavedSusaZ;

		fdActorWalkTo(ACTOR_SUSA, FIXEDtoINT(GaiaRockInsideSavedSusaX), savedZ[1]);
		scFieldActorWaitForMoveById(ACTOR_SUSA);
	}
	if (walker->script == GaiaRockInsideCaskThrownScript
	    || walker->script == GaiaRockInsideCaskPushedScript)
	{
		walker->paused = 0;
		fdActorSetAnimationSpeed(walker, ANIM_STEP_NORMAL);
	}
	syWordsCopyEntry(actor, &GaiaRockInsideSavedSusa, sizeof(ACTOR));
	fdActorSetAnimation(ACTOR_SUSA, GaiaRockInsideSavedSusaAnimation);
	fdActorSetAnimationSpeed(actor, GaiaRockInsideSavedSusaAnimationStep);
}

// Felix reaches the steps just east of where Susa works in the lair, metatile (32, 28), the first
// time. Susa turns on him ("Who are you? How did you get here?"), explains he is getting the
// Serpent to drink Dragonsbane while the view pans to it, tells the party to stand aside, and goes
// back to work. If he was walking back from tossing a cask, he takes up where he was going.
void scGaiaRockInsideMeetSusa(void)
{
	ACTOR *actor = fdActorGet(ACTOR_CASK);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgGaiaRockInsideWhoAreYouHowDid);
	syScriptCameraFollowActor(ACTOR_SUSA, 1);
	if (actor->script == GaiaRockInsideCaskEmptiesScript
	    || actor->script == GaiaRockInsideCaskPushedScript)
		scFieldActorWaitForScriptEndById(ACTOR_CASK);
	scGaiaRockInsideSaveSusa();
	fdActorSetAnimation(ACTOR_SUSA, ANIM_STAND);
	fdActorFaceActor(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	syFieldScriptWait(30);
	// "Who are you? How did you get here?"
	fdScriptShowMessage(ACTOR_SUSA, 0);
	fdActorWalkTo(ACTOR_SUSA, 496, 456);
	scFieldActorWaitForMoveById(ACTOR_SUSA);
	fdActorFaceDirection(ACTOR_SUSA, FACING_EAST_SOUTHEAST, 0);
	syFieldScriptWait(10);
	// "Only Uzume, Kushinada, and I possess the ability to come here."
	fdScriptShowMessage(ACTOR_SUSA, 0);
	fdActorShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	syFieldScriptWait(45);
	// "Never mind. I don't have time to talk. I have my hands full getting..."
	fdScriptShowMessage(ACTOR_SUSA, 0);
	fdActorFaceDirection(ACTOR_SUSA, FACING_NORTH_NORTHEAST, 0);
	syFieldScriptWait(30);
	fdCameraMoveTo(488, CAMERA_KEEP, 392, 1);
	fdCameraWaitForArrival();
	// "Once it's groggy, that's when I'll strike..."
	fdScriptShowMessage(ACTOR_SUSA, 0);
	syScriptCameraFollowActor(ACTOR_SUSA, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_SUSA, FACING_EAST_SOUTHEAST, 0);
	syFieldScriptWait(10);
	// "If you understand, then get out of my way. Stand over there."
	fdScriptShowMessage(ACTOR_SUSA, 0);
	fdActorSetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdScriptSetFlag(FLAG_MET_SUSA);
	if (actor->script == GaiaRockInsideCaskTossedLeftScript
	    || actor->script == GaiaRockInsideCaskTossedRightScript)
	{
		GaiaRockInsideSavedSusa.x.fixed = GaiaRockInsideSavedSusaX =
			GaiaRockInsideSavedSusa.targetX;
		GaiaRockInsideSavedSusa.z.fixed = GaiaRockInsideSavedSusaZ =
			GaiaRockInsideSavedSusa.targetZ;
	}
	scGaiaRockInsideRestoreSusa();
	fdScriptEnd();
}

// Felix talks to Susa in the lair. While Susa feeds the Serpent he stops, turns and tells the party
// to stand aside ("If you understand, then get out of my way."). Once he has attacked it and lies
// hurt, he says the Serpent is still too strong, or, once any beam of sunlight reaches the lair,
// "I'm done for... That dragon has finished me off...". While the party is in his way nothing
// happens (scGaiaRockInsideSusaSaysMoveAside speaks instead).
void scGaiaRockInsideTalkToSusa(void)
{
	if (fdActorGet(ACTOR_SUSA)->refused > 2)
		return;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if (fdScriptGetFlag(FLAG_SUSA_ATTACKED_SERPENT))
	{
		if (fdScriptGetFlag(FLAG_ALTAR_ROOM_SUNBEAM) || fdScriptGetFlag(FLAG_WEST_ALTAR_SUNBEAM)
		    || fdScriptGetFlag(FLAG_EAST_ALTAR_SUNBEAM) || fdScriptGetFlag(FLAG_BUSHES_SUNBEAM))
			// "I'm done for... That dragon has finished me off..."
			syFieldScriptSetMessageId((Sint32)MsgGaiaRockInsideImDoneForThatDragon);
		else
			// "The serpent is still too strong. If only there were a way to weaken it further..."
			syFieldScriptSetMessageId((Sint32)MsgGaiaRockInsideTheSerpentIsStillToo);
		fdScriptShowMessage(ACTOR_SUSA, 0);
	}
	else
	{
		if (fdActorGet(ACTOR_CASK)->script == GaiaRockInsideCaskEmptiesScript)
			scFieldActorWaitForScriptEndById(ACTOR_CASK);
		scGaiaRockInsideSaveSusa();
		syFieldScriptSetMessageId((Sint32)MsgGaiaRockInsideIfYouUnderstandThenGet);
		fdActorFaceActor(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
		syFieldScriptWait(10);
		// "If you understand, then get out of my way. Stand over there."
		fdScriptShowMessage(ACTOR_SUSA, 0);
		fdActorSetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
		syFieldScriptWait(10);
		scGaiaRockInsideRestoreSusa();
	}
	fdScriptEnd();
}

// Felix reads Susa's mind in the lair. While Susa works: "How many casks does it take to weaken
// this stupid serpent?"; once the party holds the Dancing Idol: "Who do they think they are? I'm
// the only one who can save Kushinada."
void scGaiaRockInsideReadSusasMind(void)
{
	ACTOR *actor;
	Sint32 holder;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	holder = syFieldPartyInventoryFindMember(ITEM_DANCING_IDOL);
	if (holder != -1)
	{
		syFieldScriptSetMessageId((Sint32)MsgGaiaRockInsideWhoDoTheyThinkThey);
		// "Who do they think they are? I'm the only one who can save Kushinada."
		fdScriptShowMessage(ACTOR_SUSA, 0);
	}
	else
	{
		actor = fdActorGet(ACTOR_CASK);
		if (actor->script == (Sint32)GaiaRockInsideCaskEmptiesScript)
		{
			scFieldActorWaitForScriptEndById(ACTOR_CASK);
		}
		scGaiaRockInsideSaveSusa();
		syFieldScriptSetMessageId((Sint32)MsgGaiaRockInsideHowManyCasksDoesIt);
		// "How many casks does it take to weaken this stupid serpent?"
		fdScriptShowMessage(ACTOR_SUSA, 0);
		scGaiaRockInsideRestoreSusa();
	}
	fdScriptEnd();
}

extern Uint32 OrbitSparkScript[];

#include "shared/orbit_spark_spawn.h"

#include "shared/orbiter_spiral.h"

// Moves around in a circle.
#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

void scFieldEffectSpawn(Sint32                x,
                        Sint32                y,
                        Sint32                z,
                        Sint32                velocityX,
                        Sint32                velocityY,
                        Sint32                velocityZ,
                        Uint32                flags,
                        const EFFECT_OPTIONS *options);

#include "shared/actor_emit_sparkles.h"

extern Uint32 LessonTabletRiseScript[];
extern Uint32 GaiaRockInsideRisingSparkScript[];
extern Sint32 GaiaRockInsideCirclingSparkScript[];
extern Uint32 LessonAdeptRiseScript[];
extern Sint32 LessonSeekingSparkScript[];
extern Sint32 LessonTabletSettleScript[];
extern Sint32 LessonAdeptSettleScript[];

// Felix touches the slate in the lair and learns Sand: the screen warms to gold, the slate and then
// Felix rise among circling sparks, a white flash, and they settle again.
void          scGaiaRockInsideFelixLearnsSand(Sint32 tabletId, Sint32 adeptId)
{
	ACTOR        *tablet = fdActorGetById(tabletId);
	ACTOR        *adept  = fdActorGetById(adeptId);
	Sint32        i;
	Sint16       *angle;
	// LEFTOVER(frame): an unused local the ROM's frame reserves and never
	// * touches.
	UNUSED Sint32 unused[3];

	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdScriptWait(30);
	fdCameraSetSpeed(0.5);
	fdCameraMoveWithinMapBounds(toFIXED(tablet->x.part[0].integer),
	                            toFIXED(tablet->y.part[0].integer),
	                            toFIXED(tablet->z.part[0].integer),
	                            1);
	sySoundPlay(SE_RUMBLE);
	fdMapGridSetShake(2, 2, 1);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 1);
	fdPaletteFadeStartInterpolation(60);
	tablet->motionFlags  = 0;
	tablet->acceleration = toFIXED(1) / 5;
	tablet->speedLimit   = toFIXED(1) / 5;
	fdActorSetMovementTypeById(tabletId, LessonTabletRiseScript);
	sySoundPlay(SE_TABLET_RISE);
	fdScriptWait(45);
	fdActorConfigureSpriteColor(tabletId, SPRITE_COLOR_BLINK);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT   = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	for (i = 0; i != TABLET_RISING_SPARKS; i++)
	{
		ACTOR *spark =
			syImportActorCreate(SPRITE_PARTICLE, tablet->x.fixed, tablet->y.fixed, tablet->z.fixed);
		sySoundPlay(SE_GLITTER);
		spark->x.fixed      += toFIXED((syRandom() & 31) - 16);
		spark->y.fixed      += toFIXED((syRandom() & 15) - 8);
		spark->speedLimit    = toFIXED(8);
		spark->acceleration  = toFIXED(1) / 5;
		spark->motionFlags   = 0;
		fdActorSetSpriteColor(spark, 2);
		fdSetActorSpritePriority(spark, 1);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSpriteFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		fdActorStartScript(spark, GaiaRockInsideRisingSparkScript);
		REG_BLDALPHA = BLEND_ALPHA(4, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(10, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(16, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(10, 16);
		sySynch(2);
	}
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	sySynch(30);
	for (i = 0; i != LEARN_SPARKS; i++)
	{
		ACTOR *spark =
			scFieldActorCreate(SPRITE_PARTICLE, adept->x.fixed, adept->y.fixed, adept->z.fixed);
		sySoundPlay(SE_SPARKS);
		angle               = &spark->timer;
		*angle              = (syRandom() & 0x7fff) + DEGtoANG(180);
		spark->userValue    = ((i & 1) * 2 - 1) << ((syRandom() & 3) + 9);
		spark->motionFlags  = 0;
		spark->sceneWord    = toFIXED(122);
		spark->watchedActor = (FIELD_OBJECT *)adept;
		spark->sceneCount   = (LEARN_SPARKS - i) * LEARN_SPARK_INTERVAL + LEARN_SPIRAL_TICKS;
		fdActorSetSpriteColor(spark, 2);
		fdSetActorSpritePriority(spark, 1);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSpriteFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		syFieldVectorAddPolarOffsetFX(spark->sceneWord, *angle, &spark->x.fixed);
		fdActorStartScript(spark, GaiaRockInsideCirclingSparkScript);
		if (i == 3)
		{
			fdActorSetSpriteColorById(adeptId, SPRITE_COLOR_BLINK);
			adept->motionFlags  = 0;
			adept->acceleration = toFIXED(1) / 5;
			adept->speedLimit   = toFIXED(1) / 5;
			fdActorSetMovementTypeById(adeptId, LessonAdeptRiseScript);
		}
		sySynch(LEARN_SPARK_INTERVAL);
	}
	sySoundPlay(SE_SHIMMER);
	sySynch(16);
	for (i = 2; i != ORBIT_HALF_TURN; i += 2)
	{
		ACTOR *spark         = scFieldActorCreate(SPRITE_PARTICLE,
                                          adept->x.fixed,
                                          adept->y.fixed + toFIXED(i) - toFIXED(8),
                                          adept->z.fixed);
		spark->timer         = syRandom();
		spark->userValue     = TRIG_TURN / 32;
		spark->sceneCount    = i;
		spark->sceneByte     = 1;
		spark->bounce        = spark->x.fixed;
		spark->sceneWord     = 0;
		spark->gravity       = spark->z.fixed;
		spark->watchedActor  = (FIELD_OBJECT *)adept;
		spark->motionFlags   = 0;
		spark->priorityFlags = 1;
		fdActorSetSpriteColor(spark, 2);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSpriteFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		fdActorStartScript(spark, LessonSeekingSparkScript);
	}
	sySoundPlay(SE_SHIMMER);
	sySynch(50);
	fdPaletteFadeSetTarget(RGB_WHITE, 0);
	fdPaletteFadeStartInterpolation(8);
	sySynch(16);
	fdMapGridSetShake(0, 0, 0);
	fdActorConfigureSpriteColor(tabletId, 0);
	fdActorConfigureSpriteColor(adeptId, 0);
	sySoundPlay(SE_FLASH);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(80);
	fdActorSetMovementTypeById(tabletId, LessonTabletSettleScript);
	fdActorSetMovementTypeById(adeptId, LessonAdeptSettleScript);
	adept->update = scActorEmitSparkles;
	sySynch(120);
	adept->update = 0;
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmPlayMusic();
	sySynch(1);
}

// Felix searches the slate that rose beside the slain Serpent ("Wielder of Earth's might, Lay your
// hands upon this stone."). Kraden steps out of the party ("That'd be you, Felix. Touch it."),
// Felix touches it, the ground shakes and the slate teaches him Sand
// (scGaiaRockInsideFelixLearnsSand), and Kraden walks back into the party.
void scGaiaRockInsideReadSandTablet(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Uint16 level;
	Sint32 msg;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgGaiaRockInsideSearchedTheSlateThereIs;
	((Sint32 (*)())mnFieldMessageShow)(msg, 1);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	if (FIELD_HALF_METATILE(leader->z.fixed) <= 39)
	{
		fdActorWalkTo(MainRoutineStateWork.leaderActorId, 440, 328);
		scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	}
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 470, 354);
	fdActorWalkTo(ACTOR_KRADEN, 446, 354);
	scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldScriptSetMessageId(msg + 1);
	// "Wielder of Earth's might... That'd be you, Felix. Touch it. Go on..."
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syFieldScriptWait(10);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(10);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldScriptWait(10);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 466, 342);
	scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	mnMessageShowCentered(msg + 2, 1);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	fdBlendScriptSuspend();
	syIORead16(&REG_BLDALPHA, &level);
	level &= BLEND_ALPHA_TOP_MASK;
	while (level != 0)
	{
		REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
		level--;
		syWait(1);
	}
	REG_DISPCNT &= ~DISPCNT_BG1_ON;
	scGaiaRockInsideFelixLearnsSand(ACTOR_SAND_TABLET, ACTOR_FELIX);
	REG_DISPCNT  |= DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON;
	REG_BLDALPHA  = level | BLEND_ALPHA(0, 16);
	REG_BLDCNT    = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	while (level != 8)
	{
		REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
		level++;
		syWait(1);
	}
	fdBlendScriptResume();
	fdActorSetPos(ACTOR_SAND_TABLET, 0, 0);
	fdPartyLearnAbility(FELIX, ABILITY_SAND);
	fdScriptSetFlag(FLAG_LEARNED_SAND);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	{
		ACTOR *owner = fdActorGet(MainRoutineStateWork.leaderActorId);

		if (owner != 0)
			fdActorSetDestination(ACTOR_KRADEN, owner->x.part[0].integer, owner->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syFieldScriptWait(20);
	fdScriptEnd();
}

// The same function as fdEffectAdvanceMotion, under another name. The dust's update: it moves by
// its velocity, slowing a little across the ground each tick, and grows and spins.
void scEffectAdvanceMotionLocal(FIELD_OBJECT *object)
{
	FIELD_EFFECT *effect = &object->effect;
	Sint32        velocityX;
	Sint32        velocityZ;
	Sint32        scaleRateX;
	FIELD_SPR    *sprite;

	velocityX           = effect->velocity.x;
	effect->x          += velocityX;
	effect->y          += effect->velocity.y;
	velocityZ           = effect->velocity.z;
	effect->z          += velocityZ;

	effect->velocity.x  = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
	effect->velocity.z  = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;

	scaleRateX          = effect->scaleRateX;
	effect->scaleX     += scaleRateX;
	sprite              = effect->sprite;
	effect->scaleY     += effect->scaleRateY;
	sprite->rotation   += effect->spin;
}

void scFieldEffectSpawn(Sint32                x,
                        Sint32                y,
                        Sint32                z,
                        Sint32                velocityX,
                        Sint32                velocityY,
                        Sint32                velocityZ,
                        Uint32                flags,
                        const EFFECT_OPTIONS *options);

// A ring of dust spreads from where the slate rises, one puff every eighth of a turn.
void scGaiaRockInsideRingOfDust(void)
{
	FIXED          velocity[3];
	EFFECT_OPTIONS options;
	Uint32         i;

	options.update = scEffectAdvanceMotionLocal;
	i              = 0;
	do
	{
		{
			Sint32 angle  = i * FACING_STEP;

			velocity[0]   = syCos(angle);
			velocity[1]   = 0;
			velocity[2]   = syMathSin(angle);
			velocity[0]  += velocity[0] / 3;
			scFieldEffectSpawn(toFIXED(464),
			                   toFIXED(-32),
			                   toFIXED(328),
			                   velocity[0],
			                   velocity[1],
			                   velocity[2],
			                   EFFECT_USE_UPDATE | 1,
			                   &options);
			i += 2;
		}
	} while (i <= TRIG_TURN / FACING_STEP);
}

// The pixels the slate rises from the lair's floor, one every 2 ticks.
enum
{
	TABLET_RISE_PIXELS = 20
};

// Felix steps onto the floor before the slain Serpent, metatiles (29, 24) to (31, 24), and the
// ground shakes and the slate rises from the floor west of it, in two rings of dust, unless it has
// risen already.
void scGaiaRockInsideRaiseSandTablet(void)
{
	// Play never reaches this with the slate still down: only the lair's entrances 10 to 12 use
	// this table. The arrival after the battle won (scInit, entrance 10) raises the slate itself;
	// the arrivals after a battle lost (11 and 12) send the party on to the restart point before
	// the player has control; and later visits come by entrance 9, whose table lacks this cue.
	ACTOR *tablet;
	ACTOR *actor;
	Uint16 level;
	Sint32 i;

	if (syGameFlagGet(FLAG_SAND_TABLET_RAISED) || syGameFlagGet(FLAG_LEARNED_SAND))
		return;
	tablet = fdActorGet(ACTOR_SAND_TABLET);
	syIORead16(&REG_BLDALPHA, &level);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdMapGridSetShake(10, 10, 1);
	fdScriptPlaySound(SE_TREMOR);
	fdBlendScriptSuspend();
	level &= BLEND_ALPHA_TOP_MASK;
	while (level != 0)
	{
		REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
		level--;
		syWait(1);
	}
	REG_DISPCNT &= ~DISPCNT_BG1_ON;
	syFieldScriptWait(30);
	actor = fdActorCreateSilhouette(toFIXED(464), -toFIXED(32), toFIXED(348), SPRITE_SLATE);
	fdActorSetPos(ACTOR_SAND_TABLET, 464, 348);
	scGaiaRockInsideRingOfDust();
	for (i = 0; i < TABLET_RISE_PIXELS; i++)
	{
		if (i == TABLET_RISE_PIXELS / 2)
			scGaiaRockInsideRingOfDust();
		tablet->z.fixed -= toFIXED(1);
		syFieldScriptWait(2);
	}
	fdScriptSetFlag(FLAG_SAND_TABLET_RAISED);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	actor->x.fixed = 0;
	actor->z.fixed = 0;
	syFieldScriptWait(1);
	fdActorRelease(actor);
	fdScriptPlaySound(SOUND_FADE_OUT_ACTION_SE);
	syFieldScriptWait(30);
	REG_DISPCNT  |= DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON;
	REG_BLDALPHA  = level | BLEND_ALPHA(0, 16);
	REG_BLDCNT    = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	while (level != 8)
	{
		REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
		level++;
		syWait(1);
	}
	fdBlendScriptResume();
	fdScriptEnd();
}

// Felix examines the Serpent on the sand at the north end of the lair, and the battle with it
// starts, one the party cannot run from. The fewer beams of sunlight reach the lair, the more it
// heals each turn. Won, Susa comes back and slays it, and the slate rises (scInit, entrance 10).
// Lost without the Dancing Idol, Susa scolds the party ("You! You cannot fight the serpent!"); lost
// holding it before any beam reaches the lair, Susa, lying hurt, says Dragonsbane alone cannot beat
// it. Either way the party is sent back to its restart point. Once it is slain the Serpent, turned
// to stone, only "looks almost alive".
void scGaiaRockInsideChallengeSerpent(void)
{
	Sint32 count;

	if (fdScriptGetFlag(FLAG_SERPENT_SLAIN))
	{
		// "The serpent, with its eyes closed, looks almost alive."
		mnMessageShowCentered((Sint32)MsgGaiaRockInsideTheSerpentWithItsEyes, 1);
	}
	else
	{
		Sint32 altarRoomBeam = fdScriptGetFlag(FLAG_ALTAR_ROOM_SUNBEAM);
		Sint32 westAltarBeam = fdScriptGetFlag(FLAG_WEST_ALTAR_SUNBEAM);
		Sint32 eastAltarBeam = fdScriptGetFlag(FLAG_EAST_ALTAR_SUNBEAM);
		Sint32 scene;

		count =
			altarRoomBeam + westAltarBeam + eastAltarBeam + fdScriptGetFlag(FLAG_BUSHES_SUNBEAM);
		SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
		scene = SCENE_GAIA_ROCK_INSIDE_MAZE;
		syFieldScriptSetAfterBattleDestination(scene, ENTRANCE_SERPENT_BEATEN);
		if (syFieldPartyInventoryFindMember(ITEM_DANCING_IDOL) != -1)
		{
			if (count == 0)
				fdScriptSetDefeatDestination(scene, ENTRANCE_BEATEN_HOLDING_IDOL);
		}
		else
		{
			fdScriptSetDefeatDestination(scene, ENTRANCE_BEATEN_BY_SERPENT);
		}
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SERPENT, count);
	}
}

// Felix casts Sand in the lair and sinks into the sand as a moving mound; he now moves on the map's
// sand layer, by which he can pass the Serpent to the Cloud Brand behind its tail.
void scGaiaRockInsideSinkIntoSand(void)
{
	fdActorGet(MainRoutineStateWork.leaderActorId)->layer = 2;
}

// Felix comes up out of the sand as Sand ends, and walks on the floor again.
void scGaiaRockInsideRiseFromSand(void)
{
	fdActorGet(MainRoutineStateWork.leaderActorId)->layer = 0;
}

// Felix examines the floor by the end of the Serpent's tail, metatile (34, 18), which he reaches
// with Sand, and finds the Cloud Brand. The game raises the hidden item to the raised floor's
// height there and gives it without the usual message of searching.
void scGaiaRockInsideFindCloudBrand(void)
{
	ACTOR *actor;

	actor               = fdActorGet(ACTOR_CLOUD_BRAND);
	actor->x.fixed      = toFIXED(552);
	actor->z.fixed      = toFIXED(296);
	actor->y.fixed      = toFIXED(-30);
	actor->groundHeight = toFIXED(-32);
	fdScriptClearFlag(FLAG_NOTHING_HAPPENS);
	fdTalkToActor(FIELD_TALK_SILENT | ACTOR_CLOUD_BRAND);
}

// Felix examines one of the altars where the Dancing Idol dances: "Felix checked the altar. It
// looks like something can be placed here." Holding the Dancing Idol, the field menu then opens so
// that he can use it.
void scGaiaRockInsideCheckAltar(void)
{
	const Uint8 *msg = MsgCheckedTheAltar;
	const Uint8 *next;
	Sint32       owner;

	mnMessageShowCentered(msg, 1);
	owner = syFieldPartyInventoryFindMember(ITEM_DANCING_IDOL);
	next  = msg + 2;
	if (owner != -1)
	{
		FIELD_STATE *state = RamHeapSlots->fieldState;

		mnMessageShowCentered(next, 1);
		state->fieldMenu = 1;
	}
	else
	{
		mnMessageShowCentered(next, 1);
	}
}

// How a stone face trembles while the doll dances on its altar: for FACE_TREMBLE_TICKS, then on as
// it slides aside, for FACE_SLIDE_TICKS_MAX at most, and FACE_SETTLE_TICKS more once there; a face
// already slid aside only trembles, for FACE_TREMBLE_LONG_TICKS.
enum
{
	FACE_TREMBLE_TICKS      = 20,
	FACE_SLIDE_TICKS_MAX    = 300,
	FACE_SETTLE_TICKS       = 10,
	FACE_TREMBLE_LONG_TICKS = 120
};

// Felix uses the Dancing Idol on the altar in the first altar's room. The doll hops onto the altar
// and dances, the two great red faces either side of it glare and glow, and the first time they
// tremble and slide in toward the altar, uncovering the doorways behind them, west at metatile (25,
// 42) and east at metatile (36, 42). The doll hops back to Felix.
void scGaiaRockInsidePlaceDollOnFirstAltar(void)
{
	ACTOR              *westFace = fdActorGet(ACTOR_FIRST_ALTAR_WEST_FACE);
	ACTOR              *eastFace = fdActorGet(ACTOR_FIRST_ALTAR_EAST_FACE);
	ACTOR              *doll     = fdActorGet(ACTOR_FIRST_ALTAR_DOLL);
	ACTOR              *leader;
	MAIN_ROUTINE_STATE *state;
	Sint32             *leaderId;
	Sint32              i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	// "Felix placed a doll on the altar."
	mnMessageShowCentered((Sint32)&MsgGaiaRockInsidePlacedADollOnThe, 1);
	fdScriptPlaySound(SE_DOLL_PLACED);
	fdActorSetSpritePriorityDirect(doll, 1);
	state    = &MainRoutineStateWork;
	leaderId = &state->leaderActorId;
	fdActorPlaceOnActor(ACTOR_FIRST_ALTAR_DOLL, *leaderId);
	fdActorSetDestination(ACTOR_FIRST_ALTAR_DOLL, 496, 712);
	doll->velocityY = toFIXED(6);
	scFieldActorWaitForMoveById(ACTOR_FIRST_ALTAR_DOLL);
	syFieldScriptWait(45);
	fdActorFaceActor(ACTOR_FIRST_ALTAR_WEST_FACE, *leaderId, 0);
	fdActorFaceActor(ACTOR_FIRST_ALTAR_EAST_FACE, *leaderId, 0);
	fdScriptPlaySound(SE_STATUE_STIRS);
	fdActorSetAnimation(ACTOR_FIRST_ALTAR_WEST_FACE, 5);
	fdActorSetAnimation(ACTOR_FIRST_ALTAR_EAST_FACE, 5);
	syWait(30);
	fdActorSetAnimation(ACTOR_FIRST_ALTAR_WEST_FACE, 2);
	fdActorSetAnimation(ACTOR_FIRST_ALTAR_EAST_FACE, 2);
	syFieldScriptWait(30);
	fdActorSetAnimation(ACTOR_FIRST_ALTAR_WEST_FACE, 3);
	fdActorSetAnimation(ACTOR_FIRST_ALTAR_EAST_FACE, 3);
	syFieldScriptWait(40);
	fdScriptPlaySound(SE_TABLET_RISE_HIGH);
	if (!fdScriptGetFlag(FLAG_FIRST_ALTAR_FACES_MOVED))
	{
		for (i = 0; i < FACE_TREMBLE_TICKS; i++)
		{
			westFace->velocityY = syRandom();
			eastFace->velocityY = syRandom();
			syWait(1);
		}
		fdActorSetSpeedAndAcceleration(ACTOR_FIRST_ALTAR_WEST_FACE, 0.25, 0.05);
		fdActorSetSpeedAndAcceleration(ACTOR_FIRST_ALTAR_EAST_FACE, 0.25, 0.05);
		fdActorSetDestinationOffset(ACTOR_FIRST_ALTAR_WEST_FACE, 24, 0);
		fdActorSetDestinationOffset(ACTOR_FIRST_ALTAR_EAST_FACE, -24, 0);
		for (i = 0; i < FACE_SLIDE_TICKS_MAX && westFace->arrivalAxis != 0; i++)
		{
			westFace->velocityY = syRandom();
			eastFace->velocityY = syRandom();
			syWait(1);
		}
		for (i = 0; i < FACE_SETTLE_TICKS; i++)
		{
			westFace->velocityY = syRandom();
			eastFace->velocityY = syRandom();
			syWait(1);
		}
		fdMapGridCopyMetatiles(119, 31, 1, 2, 89, 40);
		fdMapGridCopyMetatiles(119, 31, 1, 2, 100, 40);
		fdScriptSetFlag(FLAG_FIRST_ALTAR_FACES_MOVED);
	}
	else
	{
		for (i = 0; i < FACE_TREMBLE_LONG_TICKS; i++)
		{
			westFace->velocityY = syRandom();
			eastFace->velocityY = syRandom();
			syWait(1);
		}
		syFieldScriptWait(30);
	}
	fdActorSetAnimation(ACTOR_FIRST_ALTAR_WEST_FACE, 4);
	fdActorSetAnimation(ACTOR_FIRST_ALTAR_EAST_FACE, 4);
	syFieldScriptWait(30);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if (leader != NULL)
		fdActorSetDestination(
			ACTOR_FIRST_ALTAR_DOLL, leader->x.part[0].integer, leader->z.part[0].integer);
	doll->velocityY = toFIXED(6);
	scFieldActorWaitForMoveById(ACTOR_FIRST_ALTAR_DOLL);
	fdActorSetPos(ACTOR_FIRST_ALTAR_DOLL, 0, 0);
	fdScriptEnd();
}

// Felix uses the Dancing Idol on the west altar of the twin altars' room. The doll hops onto the
// altar and dances, and the face on the wall above glares and trembles. The first time, the face
// slides east and uncovers a shaft of sunlight, whose beam falls into the hole in the floor before
// it and down into the Serpent's lair; the doll hops back to Felix, and while the Serpent lives the
// game first shows the beam falling into the lair. The east altar's and the altar room's are alike.
void scGaiaRockInsidePlaceDollOnWestAltar(void)
{
	ACTOR *face;
	ACTOR *doll;
	ACTOR *leader;
	Sint32 i;

	face = fdActorGetById(ACTOR_WEST_FACE);
	doll = fdActorGetById(ACTOR_TWIN_ALTARS_DOLL);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	// "Felix placed a doll on the altar."
	mnFieldMessageShow((Sint32)MsgGaiaRockInsidePlacedADollOnThe, 1);
	sySoundPlay(SE_DOLL_PLACED);
	fdActorSetSpritePriorityDirect(doll, 1);
	fdActorPlaceOnActor(ACTOR_TWIN_ALTARS_DOLL, MainRoutineStateWork.leaderActorId);
	fdActorSetDestination(ACTOR_TWIN_ALTARS_DOLL, 528, 372);
	doll->velocityY = toFIXED(6);
	fdActorWaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
	fdScriptWait(45);
	fdActorFaceActorById(ACTOR_WEST_FACE, MainRoutineStateWork.leaderActorId, 0);
	sySoundPlay(SE_STATUE_STIRS);
	fdActorSetAnimationById(ACTOR_WEST_FACE, 5);
	sySynch(30);
	fdActorSetAnimationById(ACTOR_WEST_FACE, 2);
	fdScriptWait(30);
	fdActorSetAnimationById(ACTOR_WEST_FACE, 3);
	fdScriptWait(40);
	sySoundPlay(SE_TABLET_RISE_HIGH);
	if (!syGameFlagGet(FLAG_WEST_ALTAR_SUNBEAM))
	{
		for (i = 0; i < FACE_TREMBLE_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		fdActorSetSpeedAndAcceleration(ACTOR_WEST_FACE, 0.25, 0.05);
		fdActorSetDestinationOffsetById(ACTOR_WEST_FACE, 24, 0);
		fdMapGridCopyMetatileAttributes(27, 23, 1, 1, 28, 22);
		fdMapCopyMetatileAttributeRect(32, 16, 2, 1, 29, 22);
		i = 0;
		if (face->arrivalAxis != 0)
		{
			do
			{
				face->velocityY = syRandom();
				sySynch(1);
				if (++i >= FACE_SLIDE_TICKS_MAX)
					break;
			} while (face->arrivalAxis != 0);
		}
		fdMapGridCopyMetatiles(2, 93, 1, 5, 28, 85);
		for (i = 0; i < FACE_SETTLE_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		sySoundPlay(SE_EYES_GLOW);
		fdActorSetAnimationById(ACTOR_WEST_FACE, 4);
		fdScriptWait(30);
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if (leader != NULL)
			fdActorSetDestinationById(
				ACTOR_TWIN_ALTARS_DOLL, leader->x.part[0].integer, leader->z.part[0].integer);
		doll->velocityY = toFIXED(6);
		fdActorWaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
		fdActorSetPos(ACTOR_TWIN_ALTARS_DOLL, 0, 0);
		if (!syGameFlagGet(FLAG_SERPENT_SLAIN))
		{
			syGameFlagSet(FLAG_WEST_ALTAR_SUNBEAM_TO_SHOW);
			fdScriptWait(30);
			fdActorTableSnapshot();
			syImportScriptSetExitRequest(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_MAZE);
		}
		else
		{
			syGameFlagSet(FLAG_WEST_ALTAR_SUNBEAM);
			fdScriptEnd();
		}
	}
	else
	{
		for (i = 0; i < FACE_TREMBLE_LONG_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		fdScriptWait(30);
		fdActorSetAnimationById(ACTOR_WEST_FACE, 4);
		fdScriptWait(30);
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if (leader != NULL)
			fdActorSetDestinationById(
				ACTOR_TWIN_ALTARS_DOLL, leader->x.part[0].integer, leader->z.part[0].integer);
		doll->velocityY = toFIXED(6);
		fdActorWaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
		fdActorSetPos(ACTOR_TWIN_ALTARS_DOLL, 0, 0);
		fdScriptEnd();
	}
}

// Felix uses the Dancing Idol on the east altar of the twin altars' room, the one that fell from
// the altar room's cobweb; the face above it slides east and lets in its beam, as on the west altar
// (scGaiaRockInsidePlaceDollOnWestAltar).
void scGaiaRockInsidePlaceDollOnEastAltar(void)
{
	ACTOR *face;
	ACTOR *doll;
	ACTOR *leader;
	Sint32 i;

	face = fdActorGetById(ACTOR_EAST_FACE);
	doll = fdActorGetById(ACTOR_TWIN_ALTARS_DOLL);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	// "Felix placed a doll on the altar."
	mnFieldMessageShow((Sint32)MsgGaiaRockInsidePlacedADollOnThe, 1);
	sySoundPlay(SE_DOLL_PLACED);
	fdActorSetSpritePriorityDirect(doll, 1);
	fdActorPlaceOnActor(ACTOR_TWIN_ALTARS_DOLL, MainRoutineStateWork.leaderActorId);
	fdActorSetDestination(ACTOR_TWIN_ALTARS_DOLL, 672, 372);
	doll->velocityY = toFIXED(6);
	fdActorWaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
	fdScriptWait(45);
	fdActorFaceActorById(ACTOR_EAST_FACE, MainRoutineStateWork.leaderActorId, 0);
	sySoundPlay(SE_STATUE_STIRS);
	fdActorSetAnimationById(ACTOR_EAST_FACE, 5);
	sySynch(30);
	fdActorSetAnimationById(ACTOR_EAST_FACE, 2);
	fdScriptWait(30);
	fdActorSetAnimationById(ACTOR_EAST_FACE, 3);
	fdScriptWait(40);
	sySoundPlay(SE_TABLET_RISE_HIGH);
	if (!syGameFlagGet(FLAG_EAST_ALTAR_SUNBEAM))
	{
		for (i = 0; i < FACE_TREMBLE_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		fdActorSetSpeedAndAcceleration(ACTOR_EAST_FACE, 0.25, 0.05);
		fdActorSetDestination(ACTOR_EAST_FACE, 608, 352);
		fdMapGridCopyMetatileAttributes(35, 23, 1, 1, 36, 22);
		fdMapCopyMetatileAttributeRect(32, 16, 2, 1, 37, 22);
		i = 0;
		if (face->arrivalAxis != 0)
		{
			do
			{
				face->velocityY = syRandom();
				sySynch(1);
				if (++i >= FACE_SLIDE_TICKS_MAX)
					break;
			} while (face->arrivalAxis != 0);
		}
		fdMapGridCopyMetatiles(2, 93, 1, 5, 36, 85);
		for (i = 0; i < FACE_SETTLE_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		sySoundPlay(SE_EYES_GLOW);
		fdActorSetAnimationById(ACTOR_EAST_FACE, 4);
		fdScriptWait(30);
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if (leader != NULL)
			fdActorSetDestinationById(
				ACTOR_TWIN_ALTARS_DOLL, leader->x.part[0].integer, leader->z.part[0].integer);
		doll->velocityY = toFIXED(6);
		fdActorWaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
		fdActorSetPos(ACTOR_TWIN_ALTARS_DOLL, 0, 0);
		if (!syGameFlagGet(FLAG_SERPENT_SLAIN))
		{
			syGameFlagSet(FLAG_EAST_ALTAR_SUNBEAM_TO_SHOW);
			fdScriptWait(30);
			fdActorTableSnapshot();
			syImportScriptSetExitRequest(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_MAZE);
		}
		else
		{
			syGameFlagSet(FLAG_EAST_ALTAR_SUNBEAM);
			fdScriptEnd();
		}
	}
	else
	{
		for (i = 0; i < FACE_TREMBLE_LONG_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		fdScriptWait(30);
		fdActorSetAnimationById(ACTOR_EAST_FACE, 4);
		fdScriptWait(30);
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if (leader != NULL)
			fdActorSetDestinationById(
				ACTOR_TWIN_ALTARS_DOLL, leader->x.part[0].integer, leader->z.part[0].integer);
		doll->velocityY = toFIXED(6);
		fdActorWaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
		fdActorSetPos(ACTOR_TWIN_ALTARS_DOLL, 0, 0);
		fdScriptEnd();
	}
}

// Felix uses the Dancing Idol on the altar ringed by bushes in the altar room's west part; the face
// on the wall beside it slides west and lets in its beam, as on the twin altars' west altar
// (scGaiaRockInsidePlaceDollOnWestAltar). It is the first beam the walkthrough lets in.
void scGaiaRockInsidePlaceDollOnAltarAltar(void)
{
	ACTOR *face;
	ACTOR *doll;
	ACTOR *leader;
	Sint32 i;

	face = fdActorGetById(ACTOR_ALTAR_FACE);
	doll = fdActorGetById(ACTOR_ALTAR_DOLL);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	// "Felix placed a doll on the altar."
	mnFieldMessageShow((Sint32)MsgGaiaRockInsidePlacedADollOnThe, 1);
	sySoundPlay(SE_DOLL_PLACED);
	fdActorSetSpritePriorityDirect(doll, 1);
	fdActorPlaceOnActor(ACTOR_ALTAR_DOLL, MainRoutineStateWork.leaderActorId);
	fdActorSetDestination(ACTOR_ALTAR_DOLL, 336, 396);
	doll->velocityY = toFIXED(6);
	fdActorWaitForMoveById(ACTOR_ALTAR_DOLL);
	fdScriptWait(45);
	fdActorFaceActorById(ACTOR_ALTAR_FACE, MainRoutineStateWork.leaderActorId, 0);
	sySoundPlay(SE_STATUE_STIRS);
	fdActorSetAnimationById(ACTOR_ALTAR_FACE, 5);
	sySynch(30);
	fdActorSetAnimationById(ACTOR_ALTAR_FACE, 2);
	fdScriptWait(30);
	fdActorSetAnimationById(ACTOR_ALTAR_FACE, 3);
	fdScriptWait(40);
	sySoundPlay(SE_TABLET_RISE_HIGH);
	if (!syGameFlagGet(FLAG_ALTAR_ROOM_SUNBEAM))
	{
		for (i = 0; i < FACE_TREMBLE_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		fdActorSetSpeedAndAcceleration(ACTOR_ALTAR_FACE, 0.25, 0.05);
		fdActorSetDestinationOffset(ACTOR_ALTAR_FACE, -16, 0);
		fdMapCopyMetatileAttributeRect(27, 15, 2, 1, 23, 23);
		fdMapCopyMetatileAttributeRect(25, 25, 1, 1, 25, 23);
		i = 0;
		if (face->arrivalAxis != 0)
		{
			do
			{
				face->velocityY = syRandom();
				sySynch(1);
				if (++i >= FACE_SLIDE_TICKS_MAX)
					break;
			} while (face->arrivalAxis != 0);
		}
		fdMapGridCopyMetatiles(1, 85, 1, 4, 25, 85);
		for (i = 0; i < FACE_SETTLE_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		sySoundPlay(SE_EYES_GLOW);
		fdActorSetAnimationById(ACTOR_ALTAR_FACE, 4);
		fdScriptWait(30);
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if (leader != NULL)
			fdActorSetDestinationById(
				ACTOR_ALTAR_DOLL, leader->x.part[0].integer, leader->z.part[0].integer);
		doll->velocityY = toFIXED(6);
		fdActorWaitForMoveById(ACTOR_ALTAR_DOLL);
		fdActorSetPos(ACTOR_ALTAR_DOLL, 0, 0);
		if (!syGameFlagGet(FLAG_SERPENT_SLAIN))
		{
			syGameFlagSet(FLAG_ALTAR_ROOM_SUNBEAM_TO_SHOW);
			fdScriptWait(30);
			fdActorTableSnapshot();
			syImportScriptSetExitRequest(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_MAZE_B);
		}
		else
		{
			syGameFlagSet(FLAG_ALTAR_ROOM_SUNBEAM);
			fdScriptEnd();
		}
	}
	else
	{
		for (i = 0; i < FACE_TREMBLE_LONG_TICKS; i++)
		{
			face->velocityY = syRandom();
			sySynch(1);
		}
		fdScriptWait(30);
		fdActorSetAnimationById(ACTOR_ALTAR_FACE, 4);
		fdScriptWait(30);
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if (leader != NULL)
			fdActorSetDestinationById(
				ACTOR_ALTAR_DOLL, leader->x.part[0].integer, leader->z.part[0].integer);
		doll->velocityY = toFIXED(6);
		fdActorWaitForMoveById(ACTOR_ALTAR_DOLL);
		fdActorSetPos(ACTOR_ALTAR_DOLL, 0, 0);
		fdScriptEnd();
	}
}

// The spots listed in GaiaRockInsideStumpSpots, from 1; 0 is none.
enum
{
	STUMP_SPOTS = 11
};

extern Sint16 GaiaRockInsideStumpSpots[][2];

// Felix Moves or pushes one of the two stumps on the way to the maze in the first altar's room,
// east of the altar. Nothing more shows than the stump sliding; the game remembers where both
// stumps now stand, so that they are still there when the party comes back. Each is kept as one of
// the STUMP_SPOTS places it can stand, the two in halves of FLAG_STUMP_SPOTS.
Sint32        scGaiaRockInsideRecordStumpSpots(void)
{
	ACTOR *first       = fdActorGet(ACTOR_STUMP_A);
	Sint8  firstIndex  = 0;
	Sint32 firstX      = FIELD_HALF_METATILE(first->x.fixed);
	Sint8  secondIndex = 0;
	Sint32 firstZ      = FIELD_HALF_METATILE(first->z.fixed);
	ACTOR *second      = fdActorGet(ACTOR_STUMP_B);
	Sint32 secondX     = FIELD_HALF_METATILE(second->x.fixed);
	Sint32 secondZ     = FIELD_HALF_METATILE(second->z.fixed);
	Sint8  i;

	for (i = 1; i <= STUMP_SPOTS; i++)
	{
		if (firstX == GaiaRockInsideStumpSpots[i][0] && firstZ == GaiaRockInsideStumpSpots[i][1])
			firstIndex = i;
		if (secondX == GaiaRockInsideStumpSpots[i][0] && secondZ == GaiaRockInsideStumpSpots[i][1])
			secondIndex = i;
	}
	{
		Sint8 spots = firstIndex | secondIndex << 4;

		return syGameFlagSetByte(FLAG_STUMP_SPOTS, spots);
	}
}

// Climbing the wall of handholds along the altar room's south part, Felix reaches its west trap
// hold, drawn at metatile (24, 37) (metatile (24, 101) of the wall's overhead layer): a yellow
// stone head with red eyes bursts out of the wall under him in a shower of rocks, he flails, and he
// is left standing on it, no longer climbing (scStoneHeadBurstOut). Unlike at the stone faces
// outside, he does not fall to the floor: he stays standing up there beside the head.
void scGaiaRockInsideClimbOntoWestStoneHead(void)
{
	scStoneHeadBurstOut(ACTOR_WEST_STONE_HEAD, FLAG_WEST_STONE_HEAD_RISEN);
}

// As scGaiaRockInsideClimbOntoWestStoneHead, for the middle trap hold, a little lower and to the
// east: drawn at metatile (28, 39), metatile (28, 103) of the overhead layer.
void scGaiaRockInsideClimbOntoMiddleStoneHead(void)
{
	scStoneHeadBurstOut(ACTOR_MIDDLE_STONE_HEAD, FLAG_MIDDLE_STONE_HEAD_RISEN);
}

// As scGaiaRockInsideClimbOntoWestStoneHead, for the east trap hold: drawn at metatile (35, 39),
// metatile (35, 103) of the overhead layer.
void scGaiaRockInsideClimbOntoEastStoneHead(void)
{
	scStoneHeadBurstOut(ACTOR_EAST_STONE_HEAD, FLAG_EAST_STONE_HEAD_RISEN);
}

extern CUE GaiaRockInsideCues[];
extern CUE GaiaRockInsideLairCues[];
extern CUE GaiaRockInsideSlainLairCues[];
extern CUE GaiaRockInsideFirstAltarCues[];
extern CUE GaiaRockInsideTwinAltarsCues[];
extern CUE GaiaRockInsidePlatformsCues[];
extern CUE GaiaRockInsideAltarCues[];

// Returns the current room's cues: the lair has one table while the Serpent lives and the party
// comes in by its way in, another after the battle.
const CUE *scGetCues(void)
{
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR)
		return GaiaRockInsideFirstAltarCues;
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS)
		return GaiaRockInsideTwinAltarsCues;
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_PLATFORMS)
		return GaiaRockInsidePlatformsCues;
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
		return GaiaRockInsideAltarCues;
	if (MainRoutineStateWork.entrance == ENTRANCE_LAIR)
		return GaiaRockInsideLairCues;
	if (MainRoutineStateWork.entrance > ENTRANCE_LAIR)
		return GaiaRockInsideSlainLairCues;
	return GaiaRockInsideCues;
}

// Turns a stone face toward its altar and lays its glowing eyes over its sprite, drawn in front.
void scGaiaRockInsideTurnFaceToAltar(Sint32 faceId, Sint32 altarId)
{
	SPR_ANIMATION *eyes;
	ACTOR         *face = fdActorGetById(faceId);

	fdActorFaceActorById(faceId, altarId, 0);
	if (face != NULL)
	{
		eyes            = (SPR_ANIMATION *)fdSpriteAddAnimation(face->sprite, SPRITE_GLOWING_EYES);
		eyes->colorMode = SPRITE_COLOR_NORMAL;
		eyes->priority  = 0;
		fdActorSelectAnimation(face, 0);
		fdActorSelectAnimation(face, 4);
	}
}

// Readies a platform: one others stand on, 16 pixels up, its home its place now, on animation mode.
void scGaiaRockInsideSetPlatformMode(Uint32 id, Sint8 mode)
{
	ACTOR *actor          = fdActorGet(id);

	actor->motionFlags    = 0;
	actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
	actor->acceleration   = toFIXED(1) / 16;
	actor->speedLimit     = toFIXED(1);
	actor->y.fixed        = toFIXED(16);
	actor->timer          = FIXEDtoINT(actor->x.fixed);
	actor->userValue      = FIXEDtoINT(actor->z.fixed);
	fdActorSetAnimation(id, mode);
}

// Sends a platform toward its home moved dx and dz metatiles.
void scGaiaRockInsidePlacePlatform(Sint32 id, Sint32 dx, Sint32 dz)
{
	ACTOR *platform = fdActorGetById(id);

	scFieldActorSetMoveTargetFX(platform,
	                            toFIXED((dx << 4) + platform->timer),
	                            platform->y.fixed,
	                            toFIXED((dz << 4) + platform->userValue));
}

// The scene's view of SceneStateWork (system/globals.h).
#define SceneStateWork (*(SWAY_STATE *)SceneStateWork)

// The platforms' tick event: every four seconds each platform glides three metatiles out one way,
// and two seconds later back home. They all stop while the menu is open or Psynergy is cast.
void scGaiaRockInsideSwayPlatforms(void)
{
	SWAY_STATE  *state      = &SceneStateWork;
	Uint16      *pending    = &state->pending;
	Uint16      *frame      = &state->frame;
	FIELD_STATE *fieldState = RamHeapSlots->fieldState;
	Sint32       stopped    = fdScriptGetFlag(FLAG_MENU_OPEN);
	Sint32       id;

	if (fieldState->psynergy != 0)
		stopped |= 1;
	id = ACTOR_FIRST_PLATFORM;
	do
	{
		fdActorGet(id++)->paused = stopped;
	} while (id <= ACTOR_LAST_PLATFORM);
	if (stopped != 0)
		return;
	++*pending;
	if (fieldState->pushMetatile != 0)
		return;
	if (*pending == 0)
		return;
	do
	{
		switch (*frame)
		{
			case 60:
				scGaiaRockInsidePlacePlatform(ACTOR_FIRST_PLATFORM, 0, 3);
				scGaiaRockInsidePlacePlatform(ACTOR_FIRST_PLATFORM + 1, 3, 0);
				scGaiaRockInsidePlacePlatform(ACTOR_FIRST_PLATFORM + 2, 0, 3);
				scGaiaRockInsidePlacePlatform(ACTOR_FIRST_PLATFORM + 3, 0, 3);
				scGaiaRockInsidePlacePlatform(ACTOR_FIRST_PLATFORM + 4, 0, -3);
				scGaiaRockInsidePlacePlatform(ACTOR_FIRST_PLATFORM + 5, 0, -3);
				scGaiaRockInsidePlacePlatform(ACTOR_FIRST_PLATFORM + 6, 0, 3);
				scGaiaRockInsidePlacePlatform(ACTOR_FIRST_PLATFORM + 7, 0, -3);
				scGaiaRockInsidePlacePlatform(ACTOR_LAST_PLATFORM, 3, 0);
				break;
			case 180:
				for (id = ACTOR_FIRST_PLATFORM; id <= ACTOR_LAST_PLATFORM; id++)
				{
					scGaiaRockInsidePlacePlatform(id, 0, 0);
				}
				break;
		}
		if (++*frame == 240)
			*frame = 0;
	} while (--*pending != 0);
}

extern Sint16 GaiaRockInsidePlatformPlaces[][2];

// Sets the platforms swaying: unless the party keeps its place, clears the sway's tick and places
// each platform, then starts the sway's event.
void          scGaiaRockInsideStartPlatforms(Uint8 mode)
{
	Uint16 *frame   = &SceneStateWork.frame;
	Uint16 *pending = &SceneStateWork.pending;

	if (!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
	{
		Sint32 i;
		Sint32 id;

		*frame   = 0;
		*pending = 0;
		fdActorGet(syFieldPartyGetLeaderActor())->priorityFlags |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		for (i = 0, id = ACTOR_FIRST_PLATFORM; id <= ACTOR_LAST_PLATFORM; i++, id++)
		{
			fdActorSetPosFX(id,
			                FIELD_HALF_METATILE_ORIGIN(GaiaRockInsidePlatformPlaces[i][0]),
			                FIELD_HALF_METATILE_ORIGIN(GaiaRockInsidePlatformPlaces[i][1]));
			scGaiaRockInsideSetPlatformMode(id, mode);
		}
	}
	sySetTickEvent(scGaiaRockInsideSwayPlatforms, EVENT_PASS_TICK);
}

// Draws the beams of sunlight let into the lair, pale shafts slanting down from its top left and
// top right: the altar room's and the west altar's on the west side, the east altar's and the
// bushes' on the east. Each side has a picture for each one of its pair alone and one for both, and
// a strip between the sides that changes with what each shows.
void scGaiaRockInsideDrawSunbeams(void)
{
	Uint8  altarRoomBeam  = syGameFlagGet(FLAG_ALTAR_ROOM_SUNBEAM);
	Uint8  eastAltarBeam  = syGameFlagGet(FLAG_EAST_ALTAR_SUNBEAM);
	Sint32 westAltarBeam  = syGameFlagGet(FLAG_WEST_ALTAR_SUNBEAM);
	Sint8  altarRoomShown = altarRoomBeam;
	Sint8  eastAltarShown = eastAltarBeam;
	Uint8  westSum        = westAltarBeam + altarRoomShown;
	Sint32 bushesBeam     = syGameFlagGet(FLAG_BUSHES_SUNBEAM);
	Uint8  eastSum        = eastAltarShown + bushesBeam;
	Sint8  westBeams      = westSum;
	Sint8  eastBeams      = eastSum;

	if (westBeams != 0)
	{
		if (westBeams == 2)
		{
			fdMapGridCopyMetatilesLayered(74, 115, 7, 12, 24, 76);
			if (eastBeams == 0)
				return;
			if (eastBeams == 2)
				fdMapGridCopyMetatilesLayered(71, 110, 1, 3, 30, 85);
			else if (eastAltarShown != 0)
				fdMapGridCopyMetatilesLayered(67, 110, 1, 3, 30, 85);
			else
				fdMapGridCopyMetatilesLayered(79, 110, 1, 3, 30, 85);
		}
		else if (altarRoomShown != 0)
		{
			fdMapGridCopyMetatilesLayered(66, 95, 7, 12, 24, 76);
			if (eastBeams == 0)
				return;
			if (eastBeams == 2)
				fdMapGridCopyMetatilesLayered(81, 110, 1, 3, 30, 85);
			else if (eastAltarShown != 0)
				fdMapGridCopyMetatilesLayered(75, 110, 1, 3, 30, 85);
			else
				fdMapGridCopyMetatilesLayered(65, 110, 1, 3, 30, 85);
		}
		else
		{
			fdMapGridCopyMetatilesLayered(69, 82, 5, 12, 26, 76);
			if (eastBeams == 0)
				return;
			if (eastBeams == 2)
				fdMapGridCopyMetatilesLayered(69, 110, 1, 3, 30, 85);
			else if (eastAltarShown != 0)
				fdMapGridCopyMetatilesLayered(73, 110, 1, 3, 30, 85);
			else
				fdMapGridCopyMetatilesLayered(77, 110, 1, 3, 30, 85);
		}
	}
	if (eastBeams == 0)
		return;
	if (eastBeams == 2)
	{
		fdMapGridCopyMetatilesLayered(83, 115, 6, 12, 31, 76);
		if (westBeams == 0)
			fdMapGridCopyMetatilesLayered(82, 124, 1, 3, 30, 85);
	}
	else if (eastAltarShown != 0)
	{
		fdMapGridCopyMetatilesLayered(76, 82, 4, 12, 31, 76);
		if (westBeams == 0)
			fdMapGridCopyMetatilesLayered(75, 91, 1, 3, 30, 85);
	}
	else
	{
		fdMapGridCopyMetatilesLayered(82, 82, 6, 12, 31, 76);
		if (westBeams == 0)
			fdMapGridCopyMetatilesLayered(81, 91, 1, 3, 30, 85);
	}
}

#define Test(flag) fdScriptGetFlag(flag)
extern Uint32 GaiaRockInsideSerpentGlowScript[];
extern Uint32 GaiaRockInsideSerpentSmokeScript[];
extern Uint32 GaiaRockInsideAltarShadowScript[];
extern Sint32 GaiaRockInsideSusaFeedsSerpentScript[];

// The stone heads and the wall of handholds, which scInit sets up, come later, with the code the
// altar room shares with Gaia Rock's cliffs.
void          scStoneHeadPlace(Sint32 id, Sint32 x, Sint32 y, Sint32 z);
void          scGaiaRockInsideStartWallCues(void);

// Sets up the room the party arrives in. In the lair: the beams of sunlight let in, the stone
// Serpent and the slate once it is slain, the scene of a new beam falling into the lair (entrance
// 20), Susa's attack on the Serpent the first time the party comes holding the Dancing Idol, and
// the scenes after the battle with it (entrances 10 to 12). In the first altar's room, the faces
// and stumps where the party left them; in the platforms' room, the platforms once they sway; in
// the twin altars' room, the faces, beams and fallen altar, and the altar landing as it falls from
// the cobweb (entrance 6); in the altar room, the vine, the stone heads, the face and the fallen
// cobweb.
Sint32        scInit(void)
{
	MAIN_ROUTINE_STATE *room;
	MAIN_ROUTINE_STATE *state;
	Sint16             *entrance;
	Sint16             *early;

	state = &MainRoutineStateWork;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if (state->next == SCENE_GAIA_ROCK_INSIDE_MAZE)
	{
		early = &state->entrance;
		if (state->entrance >= ENTRANCE_LAIR)
		{
			fdActorSetSpriteFlags(fdActorGetById(ACTOR_SERPENT), 0);
			scGaiaRockInsideDrawSunbeams();
		}
		if (*early == GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_A)
		{
			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdActorSetPos(state->leaderActorId, 0, 0);
			fdActorSetPos(ACTOR_CASK, 0, 0);
			fdActorSetPos(ACTOR_SUSA, 0, 0);
			fdActorStop(ACTOR_CASK);
			fdActorStop(ACTOR_SUSA);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			fdScriptWait(60);
			{
				Sint32 ending = EXIT_NO_SUNBEAM;
				if (Test(FLAG_ALTAR_ROOM_SUNBEAM_TO_SHOW))
				{
					syGameFlagClear(FLAG_ALTAR_ROOM_SUNBEAM_TO_SHOW);
					syGameFlagSet(FLAG_ALTAR_ROOM_SUNBEAM);
					ending = GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_ALTAR;
				}
				if (Test(FLAG_WEST_ALTAR_SUNBEAM_TO_SHOW))
				{
					syGameFlagClear(FLAG_WEST_ALTAR_SUNBEAM_TO_SHOW);
					syGameFlagSet(FLAG_WEST_ALTAR_SUNBEAM);
					ending = GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_B;
				}
				if (Test(FLAG_EAST_ALTAR_SUNBEAM_TO_SHOW))
				{
					syGameFlagClear(FLAG_EAST_ALTAR_SUNBEAM_TO_SHOW);
					syGameFlagSet(FLAG_EAST_ALTAR_SUNBEAM);
					ending = GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_C;
				}
				if (Test(FLAG_BUSHES_SUNBEAM_TO_SHOW))
				{
					syGameFlagClear(FLAG_BUSHES_SUNBEAM_TO_SHOW);
					syGameFlagSet(FLAG_BUSHES_SUNBEAM);
					ending = GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_D;
				}
				sySoundPlay(SE_EYES_GLOW);
				scGaiaRockInsideDrawSunbeams();
				fdScriptWait(15);
				fdActorSetAnimationById(ACTOR_SERPENT, 3);
				sySoundPlay(SE_SEA_OF_TIME_OPENS);
				fdMapGridSetShake(2, 2, 1);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				fdScriptWait(120);
				fdActorSetAnimationById(ACTOR_SERPENT, 1);
				fdScriptWait(60);
				syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
				syImportScriptSetExitRequest(ending);
			}
		}
		if (MainRoutineStateWork.entrance >= ENTRANCE_LAIR
		    && MainRoutineStateWork.entrance <= ENTRANCE_SERPENT_BEATEN)
		{
			syGameFlagSet(FLAG_RETREAT_BLOCKED);
			if (Test(FLAG_SERPENT_SLAIN))
			{
				fdActorSetAnimationById(ACTOR_SERPENT, 4);
				fdMapGridCopyMetatilesLayered(85, 72, 5, 2, 30, 19);
				fdActorSetPos(ACTOR_CASK, 0, 0);
				fdActorSetPos(ACTOR_SUSA, 0, 0);
			}
			if (Test(FLAG_SAND_TABLET_RAISED) && !Test(FLAG_LEARNED_SAND))
				fdActorSetPos(ACTOR_SAND_TABLET, 464, 328);
		}
		room     = &MainRoutineStateWork;
		entrance = &room->entrance;
		if (*entrance == ENTRANCE_LAIR)
		{
			Sint32 found;

			if (!Test(FLAG_SERPENT_SLAIN))
			{
				sySoundPlay(MUS_BATTLE_WITH_FLAME_DRAGON);
				found = syFieldPartyInventoryFindMember(ITEM_DANCING_IDOL);
				if (found != -1)
				{
					Sint32 flag;
					ACTOR *susa;
					ACTOR *serpent;
					Sint32 width;
					Sint32 height;
					Sint32 mask;
					FIXED  fast;
					Sint32 i;
					Sint32 dx;
					Sint32 dz;
					Sint32 movement;
					ACTOR *actor;
					ACTOR *shards[4];

					flag = Test(FLAG_SUSA_ATTACKED_SERPENT);
					if (flag != 0)
					{
						fdActorSetAnimationById(ACTOR_SERPENT, 2);
						fdActorSetPos(ACTOR_CASK, 0, 0);
						fdActorSetPos(ACTOR_SUSA, 432, 320);
						fdActorSetSpritePriority(ACTOR_SUSA, 3);
						fdMapGridCopyMetatileAttributes(30, 19, 1, 1, 27, 19);
						fdActorSetAnimationById(ACTOR_SUSA, 5);
					}
					else
					{
						susa    = fdActorGetById(ACTOR_SUSA);
						serpent = fdActorGetById(ACTOR_SERPENT);
						sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
						syGameFlagSet(FLAG_SUSA_ATTACKED_SERPENT);
						syGameFlagSet(FLAG_MET_SUSA);
						fdScriptStart();
						fdScriptEndLastingPsynergy(0);
						fdActorSetPos(ACTOR_CASK, 0, 0);
						fdActorSetMovementTypeById(ACTOR_CASK, MOVEMENT_TYPE_STAND);
						fdActorSetPos(ACTOR_SUSA, 456, 360);
						fdActorSetMovementTypeById(ACTOR_SUSA, MOVEMENT_TYPE_STAND);
						fdActorFaceDirection(ACTOR_SUSA, FACING_NORTHEAST, 0);
						fdActorSetAnimationById(ACTOR_SERPENT, 2);
						fdScriptStartScreenTransition();
						fdScriptWaitTransitionTicks();
						fdCameraMoveToFX(toFIXED(488), -1, toFIXED(384), 1);
						fdCameraWaitForArrival();
						fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
						fdScriptWait(40);
						fdScriptSetMessageId((Sint32)MsgFieldAtLastTheSerpentHas);
						// "At last... The serpent has finally had its fill. This is the..."
						fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
						fdActorSetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
						// "I will defeat you before the next full moon and rescue..."
						fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
						fdActorSetAnimationById(ACTOR_SUSA, 8);
						// "All right, you so-called guardian! Let's see what you've got!"
						fdScriptShowMessageAndWait(
							SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_SUSA, 0);
						susa->speedLimit = toFIXED(3) / 2;
						fdActorSetDestinationById(ACTOR_SUSA, 488, 328);
						fdScriptWait(5);
						fdActorSetAnimationById(ACTOR_SUSA, 6);
						fdScriptWait(10);
						susa->velocityY = toFIXED(6);
						fdScriptWait(10);
						susa->paused    = 1;
						serpent->paused = 1;
						fdActorSetAnimationSpeed(susa, 0);
						fdActorSetAnimationSpeed(serpent, 0);
						fdActorSetSpriteColorMode(susa, 6);
						fdActorSetSpriteColorMode(serpent, 6);
						width  = 30;
						height = 19;
						fdMapGridCopyMetatileTilesMetatilesRect(85, 76, 5, 2, width, height);
						sySoundPlay(SE_CRUMBLE);
						fdPaletteFadeSetTarget(RGB_WHITE, 0);
						fdPaletteFadeStartInterpolation(10);
						fdScriptWait(10);
						fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
						fdPaletteFadeStartInterpolation(10);
						fdScriptWait(10);
						fdScriptWait(60);
						fdActorSetSpriteColorMode(susa, 0);
						fdActorSetSpriteColorMode(serpent, 0);
						fdMapGridCopyMetatileTilesMetatilesRect(85, 74, 5, 2, width, height);
						susa->paused    = 0;
						serpent->paused = 0;
						fdActorSetAnimationSpeed(susa, ANIM_STEP_NORMAL);
						fdActorSetAnimationSpeed(serpent, ANIM_STEP_NORMAL);
						mask                                       = (Uint8)~ACTOR_FACES_MOTION;
						fdActorGetById(ACTOR_SUSA)->steeringFlags &= mask;
						fdActorWalkTo(ACTOR_SUSA, 456, 360);
						fast            = toFIXED(4);
						susa->velocityY = fast;
						fdActorSetAttachedEffect(ACTOR_SUSA,
						                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
						fdActorWaitForMoveById(ACTOR_SUSA);
						fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
						fdScriptWait(30);
						fdActorSetAnimationById(ACTOR_SUSA, 8);
						fdScriptWait(10);
						susa->speedLimit = toFIXED(3) / 2;
						fdActorSetDestinationById(ACTOR_SUSA, 488, 328);
						fdScriptWait(14);
						susa->velocityY = toFIXED(6);
						fdActorSetAnimationById(ACTOR_SUSA, 6);
						fdScriptWait(10);
						sySoundPlay(SE_CRUMBLE);
						{
							ACTOR *a          = fdActorGetById(ACTOR_SUSA);
							mask             &= a->steeringFlags;
							a->steeringFlags  = mask;
						}
						fdActorWalkToById(ACTOR_SUSA, 456, 360);
						susa->velocityY = fast;
						fdActorSetAttachedEffect(ACTOR_SUSA,
						                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
						fdActorWaitForMoveById(ACTOR_SUSA);
						// "That's weird... It didn't even react..."
						fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
						fdScriptWait(15);
						fdActorSetAnimationSpeed(serpent, 0);
						fdScriptWait(15);
						sySoundPlay(SE_SPLASH_SMALL);
						actor = scFieldActorCreate(
							SPRITE_DUST_CLOUD, toFIXED(488), -toFIXED(16), toFIXED(345));
						fdActorSetSpriteFlags(actor, 0);
						fdActorStartScript(actor, GaiaRockInsideSerpentSmokeScript);
						fdActorSelectAnimation(actor, 1);
						fdActorSetSpriteColorMode(actor, 6);
						fdActorJump(ACTOR_SUSA, 2, 0);
						fdScriptWait(5);
						fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
						fdScriptWait(55);
						fdScriptWait(5);
						sySoundPlay(SE_WHOOSH);
						actor = scFieldActorCreate(
							SPRITE_FIREBALL, toFIXED(488), -toFIXED(16), toFIXED(345));
						fdActorSetSpriteFlags(actor, 0);
						fdActorStartScript(actor, GaiaRockInsideSerpentGlowScript);
						((FIELD_SPR *)actor->sprite)->rotation = DEGtoANG(45);
						actor->speedLimit                      = toFIXED(3) / 2;
						*(Uint8 *)&actor->motionFlags          = 0;
						actor->targetX                         = toFIXED(456);
						actor->targetY                         = -toFIXED(32);
						actor->targetZ                         = toFIXED(358);
						actor->scaleX                          = toFIXED(3) / 8;
						actor->scaleY                          = toFIXED(3) / 8;
						fdScriptWait(10);
						fdActorSetAnimationSpeed(serpent, ANIM_STEP_NORMAL);
						fdScriptWait(10);
						sySoundPlay(SE_IMPACT);
						actor->x.fixed = 0;
						actor->z.fixed = 0;
						fdMapGridSetScreenShake(fast, fast, toFIXED(1));
						for (i = 0; i < 4; i++)
						{
							dx = -8;
							if (i > 1)
								dx = 8;
							dz = -8;
							if ((i & 1) == 0)
								dz = 8;
							shards[i] = (ACTOR *)syImportActorCreate(SPRITE_FLAME_PUFF,
							                                         toFIXED(456) + toFIXED(dx),
							                                         -toFIXED(32),
							                                         toFIXED(358) + toFIXED(dz));
							fdActorSelectAnimation(shards[i], 2);
							fdActorSetSpriteFlags(shards[i], 0);
							fdActorStartScript(shards[i], GaiaRockInsideSerpentSmokeScript);
						}
						susa->velocityY                            = toFIXED(10);
						fdActorGetById(ACTOR_SUSA)->steeringFlags &= ~ACTOR_FACES_MOTION;
						fdActorWalkToById(ACTOR_SUSA, 432, 320);
						fdScriptWait(10);
						fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
						fdActorSetAnimationById(ACTOR_SUSA, 5);
						fdScriptWait(30);
						fdCameraMoveToActorById(ACTOR_SUSA, 1);
						fdCameraWaitForArrival();
						movement = (Sint32)GaiaRockInsideSquashXScript;
						fdActorSetMovementTypeById(ACTOR_SUSA, movement);
						fdActorWaitForScriptEndById(ACTOR_SUSA);
						fdScriptWait(20);
						fdActorSetMovementTypeById(ACTOR_SUSA, movement);
						fdScriptWait(20);
						// "Feeding the serpent Dragonsbane isn't weakening it enough for..."
						fdScriptShowMessageAndWait(
							SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_SUSA, 0);
						fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
						fdCameraWaitForArrival();
						fdActorSetSpritePriority(ACTOR_SUSA, 3);
						fdMapGridCopyMetatileAttributes(30, 19, 1, 1, 27, 19);
						fdScriptEnd();
					}
				}
				else
				{
					if (!Test(FLAG_KEEP_PARTY_POSITION))
					{
						fdActorSetPos(ACTOR_SUSA, 472, 456);
						fdActorSetMovementTypeById(ACTOR_SUSA,
						                           GaiaRockInsideSusaFeedsSerpentScript);
					}
				}
			}
		}
		else if (*entrance == ENTRANCE_SERPENT_BEATEN)
		{
			Sint32          flag;
			ACTOR          *susa;
			Sint32          slow;
			Sint32         *owner;
			ACTOR          *other;
			ACTOR          *actor;
			volatile Uint16 alpha;
			Sint32          i;

			sySoundPlay(MUS_IZUMO);
			flag = Test(FLAG_SERPENT_SLAIN);
			if (flag == 0)
			{
				fdScriptStart();
				fdScriptEndLastingPsynergy(0);
				syGameFlagSet(FLAG_SERPENT_SLAIN);
				fdScriptSetMessageId((Sint32)MsgFieldItLooksLikeTheDragon);
				fdActorSetPos(ACTOR_CASK, 0, 0);
				fdActorSetAnimationById(ACTOR_SERPENT, 3);
				fdActorGetById(ACTOR_KRADEN)->speedLimit = toFIXED(1);
				slow                                     = toFIXED(1) / 2;
				susa                                     = fdActorGetById(ACTOR_SUSA);
				susa->speedLimit                         = slow;
				fdActorSetPos(ACTOR_KRADEN, 440, 312);
				owner = &room->leaderActorId;
				fdActorSetPos(*owner, 456, 328);
				fdActorSetPos(ACTOR_SUSA, 488, 424);
				fdActorFaceDirectionById(*owner, FACING_EAST, 0);
				fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
				fdActorFaceDirection(ACTOR_SUSA, FACING_NORTH, 0);
				fdScriptStartScreenTransition();
				fdScriptWaitTransitionTicks();
				fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
				fdScriptPlaySound(SE_SEA_OF_TIME_OPENS);
				// "It looks like the dragon is in bad shape, but it doesn't seem..."
				fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
				fdScriptPlaySound(SE_SEA_OF_TIME_OPENS);
				// "Even though the serpent's been defeated, I can't rest!"
				fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
				fdActorShowEmote(*owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
				fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
				fdScriptWait(15);
				fdActorFaceDirection(*owner, FACING_SOUTH, 0);
				fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
				fdScriptWait(30);
				fdCameraMoveToActorById(ACTOR_SUSA, 1);
				fdCameraWaitForArrival();
				fdActorStartRepeatedMotion(ACTOR_SUSA, 2);
				fdCameraFollowActor(ACTOR_SUSA, 1);
				fdActorWalkTo(ACTOR_SUSA, 488, 376);
				fdActorWaitForMoveById(ACTOR_SUSA);
				fdActorStartRepeatedMotion(ACTOR_SUSA, 2);
				fdScriptWait(20);
				fdActorWalkToById(ACTOR_SUSA, 472, 360);
				fdActorWaitForMoveById(ACTOR_SUSA);
				fdActorFaceActorById(*owner, ACTOR_SUSA, 0);
				fdActorFaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
				fdActorFaceActorById(ACTOR_SUSA, ACTOR_SERPENT, 0);
				fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
				fdActorSetAnimationById(ACTOR_SUSA, 8);
				// "Be still, serpent, and I will grant you peace."
				fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
				susa->speedLimit = toFIXED(3) / 2;
				fdActorSetDestination(ACTOR_SUSA, 488, 328);
				fdScriptWait(5);
				fdActorSetAnimationById(ACTOR_SUSA, 6);
				fdScriptWait(10);
				susa->velocityY = toFIXED(6);
				fdScriptWait(10);
				sySoundPlay(SE_CRUMBLE);
				susa->paused = 1;
				fdPaletteFadeApplyTarget(RGB_WHITE, 0);
				fdPaletteFadeStartInterpolation(10);
				fdScriptWait(10);
				sySoundPlay(SE_SEA_OF_TIME_OPENS);
				fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
				fdPaletteFadeStartInterpolation(10);
				fdScriptWait(10);
				fdPaletteFadeApplyTarget(RGB_WHITE, 0);
				fdPaletteFadeStartInterpolation(10);
				fdScriptWait(10);
				fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
				fdPaletteFadeStartInterpolation(10);
				fdScriptWait(10);
				fdPaletteFadeSetTarget(RGB_WHITE, 0);
				fdPaletteFadeStartInterpolation(10);
				fdScriptWait(60);
				fdActorSetAnimationById(ACTOR_SERPENT, 4);
				fdMapGridCopyMetatilesLayered(85, 72, 5, 2, 30, 19);
				susa->y.fixed    = -toFIXED(32);
				susa->velocityY  = 0;
				susa->speedLimit = slow;
				fdActorSetPos(ACTOR_SUSA, 456, 360);
				fdActorSetAnimationById(ACTOR_SUSA, ANIM_STAND);
				fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
				fdPaletteFadeStartInterpolation(10);
				fdScriptWait(10);
				fdActorFaceActorById(ACTOR_SUSA, *owner, 0);
				// "And with this, Kushinada's life has been spared."
				fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
				fdActorSetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
				// "You have my sincerest thanks."
				fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
				fdActorFaceDirection(ACTOR_SUSA, FACING_SOUTH, 0);
				fdActorSetAnimationById(ACTOR_SUSA, ANIM_WALK);
				fdActorWalkTo(ACTOR_SUSA, 488, 392);
				fdActorStartRepeatedMotion(ACTOR_KRADEN, 1);
				fdScriptWait(5);
				// "Where are you going?"
				fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
				fdActorWaitForMoveById(ACTOR_SUSA);
				fdActorSetAnimationById(ACTOR_SUSA, ANIM_STILL);
				fdActorFaceActorById(ACTOR_SUSA, ACTOR_KRADEN, 0);
				fdScriptWait(15);
				// "To Kushinada, of course."
				fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
				fdActorSetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
				fdActorFaceDirection(ACTOR_SUSA, FACING_SOUTH, 0);
				fdActorSetAnimationById(ACTOR_SUSA, ANIM_WALK);
				fdActorWalkTo(ACTOR_SUSA, 488, 536);
				fdCameraFollowActor(*owner, 1);
				fdActorFaceActorById(*owner, ACTOR_KRADEN, 0);
				fdActorFaceActorById(ACTOR_KRADEN, *owner, 0);
				fdScriptWait(30);
				fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
				fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
				{
					ACTOR *ownerActor = fdActorGetById(*owner);

					if (ownerActor != NULL)
						fdActorSetDestinationById(ACTOR_KRADEN,
						                          FIXEDtoINT(ownerActor->x.fixed),
						                          FIXEDtoINT(ownerActor->z.fixed));
				}
				fdActorWaitForMoveById(ACTOR_KRADEN);
				fdActorSetPos(ACTOR_KRADEN, 0, 0);
				fdActorWaitForMoveById(ACTOR_KRADEN);
				fdActorSetPos(ACTOR_SUSA, 0, 0);
				fdActorWalkToById(*owner, 488, 376);
				fdActorWaitForMoveById(*owner);
				fdMapGridSetShakeFX(toFIXED(5), toFIXED(5), toFIXED(1));
				sySoundPlay(SE_TREMOR);
				fdScriptWait(30);
				fdActorShowEmoteById(*owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
				fdScriptWait(15);
				fdActorFaceDirectionById(*owner, FACING_NORTH, 0);
				fdScriptWait(15);
				other = fdActorGetById(ACTOR_SAND_TABLET);
				alpha = REG_BLDALPHA;
				fdMapGridSetShakeFX(toFIXED(10), toFIXED(10), toFIXED(1));
				sySoundPlay(SE_TREMOR);
				fdBlendScriptSuspend();
				alpha = *(Uint8 *)&alpha;
				while (alpha != 0)
				{
					REG_BLDALPHA = alpha | BLEND_ALPHA(0, 16);
					alpha        = alpha - 1;
					sySynch(1);
				}
				REG_DISPCNT &= (Uint16)~DISPCNT_BG1_ON;
				fdScriptWait(30);
				actor =
					fdActorCreateSilhouette(toFIXED(464), -toFIXED(32), toFIXED(348), SPRITE_SLATE);
				fdActorSetPos(ACTOR_SAND_TABLET, 464, 348);
				for (i = 0; i < TABLET_RISE_PIXELS; i++)
				{
					if (i % 5 == 0)
						scGaiaRockInsideRingOfDust();
					other->z.fixed += -toFIXED(1);
					fdScriptWait(2);
				}
				syGameFlagSet(FLAG_SAND_TABLET_RAISED);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				actor->x.fixed = 0;
				actor->z.fixed = 0;
				fdScriptWait(1);
				fdActorRelease(actor);
				sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
				fdScriptWait(30);
				REG_DISPCNT  |= DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON;
				REG_BLDALPHA  = alpha | BLEND_ALPHA(0, 16);
				REG_BLDCNT    = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
				while (alpha != 8)
				{
					REG_BLDALPHA = alpha | BLEND_ALPHA(0, 16);
					alpha++;
					sySynch(1);
				}
				fdBlendScriptResume();
				fdScriptEnd();
			}
		}
		else if (*entrance == ENTRANCE_BEATEN_BY_SERPENT)
		{
			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			fdActorSetPos(ACTOR_CASK, 0, 0);
			fdActorSetPos(ACTOR_SUSA, 448, 344);
			{
				Sint32 *lead = &room->leaderActorId;

				fdActorFaceActorById(ACTOR_SUSA, *lead, 0);
				scFieldActorSetAnimation(*lead, ANIM_COLLAPSE);
			}
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			fdScriptSetMessageId((Sint32)MsgFieldYouYouCannotFightThe);
			// "You! You cannot fight the serpent! Not while it's still in full..."
			fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
			fdScriptFinishScreenTransition();
			fdScriptWaitTransitionTicks();
			fdScriptSetDestination((Sint16)room->restartScene, (Sint16)room->restartEntrance);
#if DEFEAT_SCENE_RESTART_MOVES_SHIP
			fdWorldMapSetReturnLeavingScene(room->next, *entrance);
#endif
			fdScriptEnd();
		}
		else if (*entrance == ENTRANCE_BEATEN_HOLDING_IDOL)
		{
			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			fdActorSetPos(ACTOR_CASK, 0, 0);
			fdActorSetPos(ACTOR_SUSA, 430, 320);
			fdActorSetAnimationById(ACTOR_SUSA, 5);
			fdActorSetAnimationById(room->leaderActorId, ANIM_COLLAPSE);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			fdScriptWait(30);
			fdActorStartRepeatedMotion(ACTOR_SUSA, 2);
			fdScriptWait(30);
			fdScriptSetMessageId((Sint32)MsgFieldLikeISaidTheSerpent);
			// "Like I said, the serpent can't be defeated by just weakening it..."
			fdScriptShowMessageAndWait(ACTOR_SUSA, 0);
			fdScriptFinishScreenTransition();
			fdScriptWaitTransitionTicks();
			fdScriptSetDestination((Sint16)room->restartScene, (Sint16)room->restartEntrance);
#if DEFEAT_SCENE_RESTART_MOVES_SHIP
			fdWorldMapSetReturnLeavingScene(room->next, *entrance);
#endif
			fdScriptEnd();
		}
	}
	else if (state->next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR)
	{
		Sint32 mask = ACTOR_PRIORITY_IGNORES_RISES;
		Sint32 low;
		Sint32 high;
		ACTOR *actor          = fdActorGet(ACTOR_FIRST_ALTAR_WEST_FACE);

		actor->priorityFlags |= mask;
		fdActorSetSpritePriorityDirect(actor, 3);
		actor                 = fdActorGet(ACTOR_FIRST_ALTAR_EAST_FACE);
		actor->priorityFlags |= mask;
		fdActorSetSpritePriorityDirect(actor, 3);
		scGaiaRockInsideTurnFaceToAltar(ACTOR_FIRST_ALTAR_WEST_FACE, ACTOR_FIRST_ALTAR);
		scGaiaRockInsideTurnFaceToAltar(ACTOR_FIRST_ALTAR_EAST_FACE, ACTOR_FIRST_ALTAR);
		if (Test(FLAG_FIRST_ALTAR_FACES_MOVED))
		{
			fdActorSetPos(ACTOR_FIRST_ALTAR_WEST_FACE, 440, 688);
			fdActorSetPos(ACTOR_FIRST_ALTAR_EAST_FACE, 552, 688);
		}
		else
		{
			Sint32 height = 40;

			fdMapCopyMetatilesRect(88, 40, 1, 2, 89, height);
			fdMapCopyMetatilesRect(99, 40, 1, 2, 100, height);
		}
		low  = scFieldGameFlagGetByte(FLAG_STUMP_SPOTS) & 0xf;
		high = (Uint8)(scFieldGameFlagGetByte(FLAG_STUMP_SPOTS) >> 4);
		if (low != 0)
			fdActorSetHorizontalPosWithTerrain(
				ACTOR_STUMP_A,
				FIELD_HALF_METATILE_ORIGIN(GaiaRockInsideStumpSpots[low][0]),
				FIELD_HALF_METATILE_ORIGIN(GaiaRockInsideStumpSpots[low][1]));
		if ((Sint8)high != 0)
			fdActorSetHorizontalPosWithTerrain(
				ACTOR_STUMP_B,
				FIELD_HALF_METATILE_ORIGIN(GaiaRockInsideStumpSpots[(Sint8)high][0]),
				FIELD_HALF_METATILE_ORIGIN(GaiaRockInsideStumpSpots[(Sint8)high][1]));
	}
	else if (state->next == SCENE_GAIA_ROCK_INSIDE_PLATFORMS)
	{
		if (Test(FLAG_PLATFORMS_SWAYING))
			scGaiaRockInsideStartPlatforms(1);
	}
	else if (state->next == SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS)
	{
		ACTOR *actor;
		ACTOR *other;

		actor = fdActorGetById(ACTOR_WEST_ALTAR);
		fdActorSetSpritePriorityDirect(actor, 3);
		scGaiaRockInsideTurnFaceToAltar(ACTOR_WEST_FACE, ACTOR_WEST_ALTAR);
		scGaiaRockInsideTurnFaceToAltar(ACTOR_EAST_FACE, ACTOR_EAST_ALTAR);
		if (!Test(FLAG_ALTAR_ROOM_SUNBEAM))
			fdMapGridCopyMetatiles(25, 77, 1, 15, 26, 77);
		if (Test(FLAG_WEST_ALTAR_SUNBEAM))
		{
			Sint32 width;
			Sint32 height;

			fdActorSetPos(ACTOR_WEST_FACE, 480, 352);
			width = 28;
			fdMapCopyMetatilesRect(2, 93, 1, 5, width, 85);
			height = 22;
			fdMapCopyMetatileAttributeRect(27, 23, 1, 1, width, height);
			fdMapCopyMetatileAttributeRect(32, 16, 2, 1, 29, height);
		}
		if (Test(FLAG_EAST_ALTAR_SUNBEAM))
		{
			Sint32 width;
			Sint32 height;

			fdActorSetPos(ACTOR_EAST_FACE, 608, 352);
			width = 36;
			fdMapCopyMetatilesRect(2, 93, 1, 5, width, 85);
			height = 22;
			fdMapCopyMetatileAttributeRect(35, 23, 1, 1, width, height);
			fdMapCopyMetatileAttributeRect(32, 16, 2, 1, 37, height);
		}
		if (Test(FLAG_BUSHES_SUNBEAM))
		{
			fdMapGridCopyMetatilesLayered(61, 0, 3, 3, 37, 26);
			fdMapGridCopyMetatileAttributes(36, 23, 1, 1, 38, 25);
		}
		if (state->entrance == GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_B)
		{
			Sint32 ticks = 60;

			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdActorSetPos(ACTOR_EAST_ALTAR, 0, 0);
			fdActorSetPos(state->leaderActorId, 0, 0);
			fdScriptStartScreenTransition();
			fdScriptWait(15);
			fdActorSetPos(ACTOR_EAST_ALTAR, 672, 384);
			fdActorSetSpriteFlags(fdActorGetById(ACTOR_EAST_ALTAR), 0);
			actor = fdActorGetById(ACTOR_EAST_ALTAR);
			other =
				scFieldActorCreate(SPRITE_LIGHT, actor->x.fixed, actor->y.fixed, actor->z.fixed);
			if (other != NULL)
			{
				fdActorSetSpriteFlags(other, 0);
				fdActorStartScript(other, GaiaRockInsideAltarShadowScript);
				fdActorSelectAnimation(other, 1);
				fdActorSetSpriteColorMode(other, SPRITE_COLOR_BLACK);
				// The high byte of its OAM attribute 2: drawn at priority 3.
				((FIELD_SPR *)other->sprite)->priority = 3;
				other->scaleX                          = toFIXED(6) / 5;
				other->scaleY                          = toFIXED(6) / 5;
			}
			actor->y.fixed += toFIXED(128);
			for (; ticks != 0; ticks--)
			{
				sySynch(1);
				if (actor->velocityY > -1)
					break;
			}
			sySoundPlay(SE_THUMP);
			fdScriptWait(30);
			syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
			syImportScriptSetExitRequest(
				GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A);
		}
		if (!Test(FLAG_ALTAR_FELL))
			fdActorSetPos(ACTOR_EAST_ALTAR, 0, 0);
#if PIT_FALL_DUST_LANDING
		if (!Test(FLAG_KEEP_PARTY_POSITION)
		    && MainRoutineStateWork.entrance
		           == GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A)
			scGaiaRockInsideRaiseDust();
#endif
	}
	else if (state->next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
	{
		fdHangingVinePlace(ACTOR_HANGING_VINE, 1);
		scGaiaRockInsideTurnFaceToAltar(ACTOR_ALTAR_FACE, ACTOR_ALTAR);
		if (state->entrance == GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_ALTAR_WEST
		    || state->entrance == GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_ALTAR_EAST
		    || state->entrance == GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_COBWEB)
			scGaiaRockInsideStartWallCues();
		if (Test(FLAG_WEST_STONE_HEAD_RISEN))
			scStoneHeadPlace(ACTOR_WEST_STONE_HEAD, toFIXED(384), toFIXED(75), toFIXED(680));
		if (Test(FLAG_MIDDLE_STONE_HEAD_RISEN))
			scStoneHeadPlace(ACTOR_MIDDLE_STONE_HEAD, toFIXED(463), toFIXED(51), toFIXED(680));
		if (Test(FLAG_EAST_STONE_HEAD_RISEN))
			scStoneHeadPlace(ACTOR_EAST_STONE_HEAD, toFIXED(565), toFIXED(55), toFIXED(680));
		if (Test(FLAG_ALTAR_ROOM_SUNBEAM))
		{
			Sint32 width;
			Sint32 height;

			fdActorSetPos(ACTOR_ALTAR_FACE, 384, 368);
			width = 25;
			fdMapCopyMetatilesRect(1, 85, 1, 4, width, 85);
			height = 23;
			fdMapCopyMetatileAttributeRect(27, 15, 2, 1, height, height);
			fdMapCopyMetatileAttributeRect(25, 25, 1, 1, width, height);
		}
		fdActorSetSpriteFlags(fdActorGetById(ACTOR_FALLING_ALTAR), 0);
		if (Test(FLAG_ALTAR_FELL))
		{
			Sint32 height = 20;

			fdMapGridCopyMetatileTilesMetatilesRect(117, 20, 7, 8, 104, height);
			fdMapGridCopyMetatileTilesMetatilesRect(111, 12, 1, 6, 111, height);
			fdMapGridCopyMetatileAttributes(49, 84, 8, 8, 40, 84);
			fdActorSetPos(ACTOR_FALLING_ALTAR, 0, 0);
		}
	}
	fdEffectStartBlankOverlay(0);
	return 0;
}

// The parts of the altar rooms' overhead layer, each marked by its metatiles' collision code:
// arriving keeps only the part around the entrance.
enum
{
	OVERHEAD_PART_A = 100,
	OVERHEAD_PART_B,
	OVERHEAD_PART_C,
	OVERHEAD_PART_D
};

// Blanks the overhead layer of an altar room but for the part keep, the one around the entrance the
// party arrives by.
void scGaiaRockInsideKeepOverheadPart(Sint32 keep)
{
	scGaiaRockInsideBlankLayer(1, keep);
}

// Prepares the map. In the maze it lays the path, from the letters of the leader's name, once a
// visit, and draws the room the party stands in, with the Mimic's chest in the third room and the
// Rusty Mace's in the ninth, and the plant's arrow if Growth has shown it; in the lair it starts
// the slow shimmer of its overhead layer, where the beams of sunlight are drawn. In the first
// altar's room and the altar room it blanks the overhead layer but for the part around the
// entrance.
Sint32 scPrepareMap(void)
{
	if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_MAZE)
	{
		Uint8 *map = (Uint8 *)RamHeapSlots->mapGrid;

		if (MainRoutineStateWork.entrance <= 4)
		{
			UNIT   *unit;
			Sint32  total;
			Sint32 *stage;
			Sint32  i;

			total = 0;
			unit  = syUnitGet(MainRoutineStateWork.leaderActorId);

			for (i = 0; i < UNIT_NAME_SIZE - 1; i++)
				total += unit->name[i];
			scGaiaRockInsideLayMaze(total);
			fdActorGetById(fdPartyGetLeaderActor())->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
			((MAP_STATE *)map)->enabled[1]                          = 1;
			{
				Uint16 control;

				*&control = REG_BG2CNT & ~3;
				syIOWrite16(control | 1, &REG_BG2CNT);
			}
			scGaiaRockInsideDrawMazeRoom();
			stage = &PathWork->current;
			if (*stage == 3 && !syGameFlagGet(FLAG_MIMIC_CHEST_LEFT_OUT))
				fdActorSetPos(ACTOR_MIMIC_CHEST, 104, 88);
			if (*stage == 9)
				fdActorSetPos(ACTOR_RUSTY_MACE_CHEST, 104, 72);
			if (syGameFlagGet(FLAG_MAZE_PLANT_POINTED(*stage)))
			{
				ACTOR *plant          = fdActorGetById(ACTOR_ARROW_PLANT);
				Uint32 angle          = scGaiaRockInsideGetMazeWayOn();

				plant->collisionFlags = 0;
				fdActorSelectAnimation(plant, ARROW_ANIMATION(angle));
				plant->update         = ARROW_UP_OR_DOWN(angle) != 0
				                          ? (void *)scGaiaRockInsidePulseUpDownArrow
				                          : (void *)scGaiaRockInsidePulseSidewaysArrow;
				plant->collisionFlags = 1;
				syGameFlagSet(FLAG_PLANT_POINTING);
			}
		}
		else if (MainRoutineStateWork.entrance >= ENTRANCE_LAIR)
		{
			MAP_LAYER *layer;

			PathWork->current = 1;
			layer             = &((MAP_STATE *)map)->layers[2];
			layer->maskX      = MAPGRID_SIZE - 1;
			layer->speedX     = 0;
			layer->speedY     = 0;
			layer->phaseX     = 0;
			layer->phaseY     = 0;
			layer->scaleX     = toFIXED(1);
			layer->scaleY     = toFIXED(1);
			layer->maskY      = MAPGRID_ROW_MASK;
			layer->x = syMulFX(((MAP_STATE *)map)->view[0], layer->scaleX) + layer->offsetX;
			layer->y = syMulFX(((MAP_STATE *)map)->view[1], layer->scaleY) + layer->offsetY;
			fdMapGridCenterOnFollowedPoint();
			syWait(1);
			syWordsCopy(MAPGRID_BLEND_SCRIPT,
			            GaiaRockInsideLairBlendScript,
			            sizeof(GaiaRockInsideLairBlendScript));
			fdBlendScriptStart((Uint16 *)MAPGRID_BLEND_SCRIPT);
		}
	}
	else if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR)
	{
		switch (MainRoutineStateWork.entrance)
		{
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_SUSA:
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_STUMPS:
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_WEST_PASSAGE:
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_EAST_PASSAGE:
				scGaiaRockInsideKeepOverheadPart(OVERHEAD_PART_A);
				break;
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_PASSAGE_FROM_ALTAR:
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_PLATFORMS_EAST:
				scGaiaRockInsideKeepOverheadPart(OVERHEAD_PART_C);
				break;
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_STUMPS_FROM_ALTAR:
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE:
				scGaiaRockInsideKeepOverheadPart(OVERHEAD_PART_D);
				break;
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_WEST_PASSAGE_FROM_ALTAR:
			case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_PLATFORMS_WEST:
				scGaiaRockInsideKeepOverheadPart(OVERHEAD_PART_B);
				break;
		}
	}
	else if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
	{
		switch (MainRoutineStateWork.entrance)
		{
			case GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_ALTAR_WEST:
			case GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_ALTAR_EAST:
			case GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_COBWEB:
				scGaiaRockInsideKeepOverheadPart(OVERHEAD_PART_A);
				break;
			case GAIA_ROCK_INSIDE_ALTAR_ALTAR_WEST_FROM_WALL:
			case GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE_A:
			case GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_NORTHWEST:
				scGaiaRockInsideKeepOverheadPart(OVERHEAD_PART_B);
				break;
			case GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_A:
				scGaiaRockInsideKeepOverheadPart(OVERHEAD_PART_C);
				break;
		}
	}
	return 0;
}

// The maze grid's width, in its metatiles.
#define GRID_WIDTH 28

// The grid the maze's path is laid on (scGaiaRockInsideLayMaze).
typedef struct
{
	PATH_METATILE metatiles[GRID_WIDTH * GRID_WIDTH];
} PATH_GRID;

// The maze grid's metatile at (x, y), and its address.
#define AT(x, y)            grid->metatiles[(y) * GRID_WIDTH + (x)]
#define AT_ADDRESS(x, y)    ((PATH_METATILE *)(((y) * GRID_WIDTH + (x)) * 4 + (Sint32)grid))

// The headings the walk tries from a step before it starts the path again.
#define MAZE_TRIES_PER_STEP 16

// Lays the maze's path of MAZE_STEPS steps from seed, once a visit, on a random walk over a 28 by
// 28 grid, and records each step's ways out.
void scGaiaRockInsideLayMaze(Sint32 seed)
{
	PATH_GRID     *grid;
	PATH          *path;
	PATH_STEP     *steps;
	PATH_METATILE *metatile;
	Sint32         x, y, nx, ny, step, tries, direction, v, next;
	Uint32         saved;

	saved = RandomSeed;
	path  = PathWork;
	steps = path->steps;
	if (syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
		return;
	if (syGameFlagGet(FLAG_MAZE_LAID))
		return;
	RandomSeed = seed;
restart:
	grid = syHeapCallAlternatePool(syHeapAllocateEWRAM, sizeof *grid);
	syWordsClearEntry(grid, sizeof *grid);
	// BUG: this runs 29 times, so row 28 and column 28 lie past the 28 by 28 metatiles and the last
	// row's marks land after the block.
	{
		PATH_METATILE *a, *b, *e;
		Sint32         o, t;
		x    = GRID_WIDTH / 2;
		y    = GRID_WIDTH / 2;
		o    = 0;
		b    = &AT(GRID_WIDTH, 0);
		// step counts the border's metatiles here, before the walk numbers its steps: a runs down
		// column 0 and b down column GRID_WIDTH, e along row 0, and the byte offset o along row
		// GRID_WIDTH.
		step = GRID_WIDTH;
		a    = grid->metatiles;
		e    = grid->metatiles;
		do
		{
			a->step  = MAZE_BORDER;
			a       += GRID_WIDTH;
			b->step  = MAZE_BORDER;
			b       += GRID_WIDTH;
			e->step  = MAZE_BORDER;
			e++;
			t = o + GRID_WIDTH * GRID_WIDTH * sizeof(PATH_METATILE);
			((PATH_METATILE *)((Sint8 *)grid + t))->step  = MAZE_BORDER;
			o                                            += sizeof(PATH_METATILE);
			step--;
		} while (step >= 0);
	}
	for (step = 1; step <= MAZE_STEPS;)
	{
		tries = 0;
		for (;;)
		{
			if (++tries > MAZE_TRIES_PER_STEP)
				goto restart;
			nx        = x;
			ny        = y;
			direction = syRandomBelow(4);
			if (direction == MAZE_SOUTH && (step == 1 || step == MAZE_STEPS - 1))
				continue;
			switch (direction)
			{
				case MAZE_SOUTH:
					if (AT(x, y + 1).step != 0 || AT(x, y + 2).step != 0
					    || AT(x - 1, y + 1).step != 0 || AT(x + 1, y + 1).step != 0)
						continue;
					ny = y + 1;
					break;
				case MAZE_WEST:
					if (AT(x - 1, y).step != 0 || AT(x - 2, y).step != 0
					    || AT(x - 1, y - 1).step != 0 || AT(x - 1, y + 1).step != 0)
						continue;
					nx = x - 1;
					break;
				case MAZE_EAST:
					if (AT(x + 1, y).step != 0 || AT(x + 2, y).step != 0
					    || AT(x + 1, y - 1).step != 0 || AT(x + 1, y + 1).step != 0)
						continue;
					nx = x + 1;
					break;
				case MAZE_NORTH:
					if (AT(x, y - 1).step != 0 || AT(x, y - 2).step != 0
					    || AT(x - 1, y - 1).step != 0 || AT(x + 1, y - 1).step != 0)
						continue;
					ny = y - 1;
					break;
			}
			break;
		}
		metatile       = AT_ADDRESS(x, y);
		metatile->step = step;
		step++;
		metatile->nextX = nx;
		metatile->nextY = ny;
		x               = nx;
		y               = ny;
	}

	x = GRID_WIDTH / 2;
	y = GRID_WIDTH / 2;
	for (step = 1; step <= MAZE_STEPS; step = next)
	{
		metatile = AT_ADDRESS(x, y);
		next     = step + 1;

		{
			Sint32 c;

			v = step;
			c = AT(x, y + 1).step;

			if (c == next)
				v = next;
			else if (c == step - 1)
				v = c;
			else
			{
				PATH_METATILE *p = metatile;
				do
				{
					p -= GRID_WIDTH;
					c  = p->step;
					if (c != v - 1)
					{
						break;
					}
					v = c;
				} while (1);
			}
			if (v == 0)
				v = 1;
		}
		steps[step].next[MAZE_SOUTH] = v;

		{
			Sint32 c;

			v = step;
			c = AT(x, y - 1).step;

			if (c == next)
				v = next;
			else if (c == step - 1)
				v = c;
			else
			{
				PATH_METATILE *p = AT_ADDRESS(x, y);
				do
				{
					p += GRID_WIDTH;
					c  = p->step;
					if (c != v - 1)
					{
						break;
					}
					v = c;
				} while (1);
			}
			if (v == 0)
				v = 1;
		}
		steps[step].next[MAZE_NORTH] = v;

		{
			Sint32 c;

			v = step;
			c = AT(x - 1, y).step;

			if (c == next)
				v = next;
			else if (c == step - 1)
				v = c;
			else
			{
				nx = x;
				do
				{
					c = AT(++nx, y).step;
					if (c != v - 1)
					{
						break;
					}
					v = c;
				} while (1);
			}
			if (v == 0)
				v = 1;
		}
		steps[step].next[MAZE_WEST] = v;

		{
			Sint32 c;

			v = step;
			c = AT(x + 1, y).step;

			if (c == next)
				v = next;
			else if (c == step - 1)
				v = c;
			else
			{
				nx = x;
				do
				{
					c = AT(--nx, y).step;
					if (c != v - 1)
					{
						break;
					}
					v = c;
				} while (1);
			}
			if (v == 0)
				v = 1;
		}
		steps[step].next[MAZE_EAST] = v;
		x                           = metatile->nextX;
		steps[step].number          = step;
		y                           = metatile->nextY;
	}
	steps[MAZE_STEPS].next[MAZE_NORTH] = MAZE_EXIT;
	steps[1].next[MAZE_SOUTH]          = MAZE_BACK;
	path->current                      = 1;
	syHeapRelease(grid);
	fdScriptSetFlag(FLAG_MAZE_LAID);
	RandomSeed = saved;
}

// Felix walks out of a maze room by one of its four ways, and a room just like it appears. The way
// the plant's arrow points leads on to the next room, and the way the party came in leads back to
// the last. Going straight on where the path turns leads back along it, to the room where it last
// turned, and the fourth way leads back into the same room. South out of the first room leads back
// to the first altar's room, and the way on from the fourteenth into the Serpent's lair.
void scGaiaRockInsideTakeMazeExit(Sint32 direction)
{
	Sint32 *state = &PathWork->current;
	// The step's PATH_STEP.next, read by its offset from the path's start.
	Uint8  *row   = (Uint8 *)state + *state * sizeof(PATH_STEP);
	Sint32  next  = row[((direction - 1) & 3) + offsetof(PATH, steps[0].next)];

	switch (next)
	{
		case MAZE_BACK:
		case MAZE_EXIT: syFieldScriptSetExitRequest(next); break;
		case 0        : next = 1;
		default:
			*state = next;
			syFieldScriptSetExitRequest(direction);
			break;
	}
	fdScriptPlaySound(SE_MAP_EXIT);
}

extern Sint8 GaiaRockInsideMazeOrigins[][2];

// Draws the maze room the party stands in, a 15 by 10 metatile screen copied from the room's own
// place in the map (GaiaRockInsideMazeOrigins), on both layers; the first room is drawn where it
// stands.
void         scGaiaRockInsideDrawMazeRoom(void)
{
	Sint32     state  = PathWork->current;
	Sint8     *origin = GaiaRockInsideMazeOrigins[state];
	MAP_STATE *map    = RamHeapSlots->mapGrid;

	if (state > 1)
	{
		Sint32     originX = origin[0];
		Sint32     originZ = origin[1];
		MAP_LAYER *layer   = map->layers;
		Sint32     i;

		for (i = 0; i < 2; i++, layer++)
		{
			Sint32 x = FIELD_METATILE(layer->offsetX);
			Sint32 z = FIELD_METATILE(layer->offsetY);

			scMapGridCopyRawMetatiles(originX + x, originZ + z, 15, 10, x + 1, z + 1);
		}
	}
}

// The way on from the step the party stands on: the facing toward the next step or the exit, or -1
// if no way leads on.
Sint32 scGaiaRockInsideGetMazeWayOn(void)
{
	PATH      *path = PathWork;
	Sint32     step = path->current;
	PATH_STEP *node = &path->steps[step];
	Uint32     link;

	// A way leads on when it reaches the exit or the step after this one.
	step++;
	link = node->next[MAZE_SOUTH];
	if (link == MAZE_EXIT || link == step)
		return FACING_SOUTH;
	link = node->next[MAZE_WEST];
	if (link == MAZE_EXIT || link == step)
		return FACING_WEST;
	link = node->next[MAZE_EAST];
	if (link == MAZE_EXIT || link == step)
		return FACING_EAST;
	link = node->next[MAZE_NORTH];
	if (link == MAZE_EXIT || link == step)
		return FACING_NORTH;
	return -1;
}

// Fills every metatile of a map layer within the map's bounds with its first metatile, but those of
// collision code 0 or keep.
void scGaiaRockInsideBlankLayer(Sint32 layer, Sint32 keep)
{
	MAP_STATE *map       = RamHeapSlots->mapGrid;
	MAP_LAYER *mapLayer  = &map->layers[layer];
	Uint32    *metatiles = (Uint32 *)mapLayer->metatiles;
	Sint32     width     = FIELD_METATILE(map->maxX - map->minX);
	Sint32     height    = FIELD_METATILE(map->maxZ - map->minZ);
	Uint32     fill;
	Uint16     z;
	Uint16     x;

	metatiles += (FIELD_METATILE(map->minZ) << 7) + FIELD_METATILE(map->minX);
	fill       = *metatiles;
	for (z = 0; z < height; z++)
	{
		Uint32 *metatile = &metatiles[z * 128];

		for (x = 0; x < width; x++)
		{
			if (((MAP_METATILE *)metatile)->collisionCode != 0
			    && ((MAP_METATILE *)metatile)->collisionCode != keep)
				*metatile = fill;
			metatile++;
		}
	}
}

#include "shared/gaia_rock_cue_wall_metatile.c"

#include "shared/stone_head_place.h"

extern Sint32 StoneHeadRiseScript[];
extern Uint32 DebrisScript[];

#include "shared/stone_head_burst_out.h"

// Starts scGaiaRockCueWallMetatile each tick.
void scGaiaRockInsideStartWallCues(void)
{
	sySetTickEvent((Sint32)scGaiaRockCueWallMetatile, EVENT_PASS_TICK);
}

extern Uint32 GaiaRockInsideIdleScript[];
extern Uint32 GaiaRockInsideStumpScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

Uint32 GaiaRockInsidePourScript[] = {
	SC_WAIT(230),
	SC_SET(SW_HOOK, 0),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};

Uint32 GaiaRockInsideBurstScript[] = {
	SC_WAIT(25),
	SC_SET(SW_HOOK, 0),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};

Sint32 GaiaRockInsideAltarShakesScript[] = {
	SC_MOVE_BY(0, 0, 2),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -4),
	SC_SYNC,
	SC_MOVE_BY(0, 0, 2),
	SC_SYNC,
	SC_WAIT(2),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 GaiaRockInsideAltarDropsScript[] = {
	SC_MOVE_BY(0, 0, 4),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -8),
	SC_SYNC,
	SC_MOVE_BY(0, 0, 4),
	SC_SYNC,
	SC_WAIT(2),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Uint32 GaiaRockInsideLeaderStepsBackScript[] = {
	SC_MOVE(744, 0, 432),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_END,
};

// The twelve metatiles of the cobweb, as column and row; Cyclone clears the web when it reaches one of them (scGaiaRockInsideCycloneClearsCobweb).
METATILE_POINT GaiaRockInsideCobwebMetatiles[] = {
	{{42, 30}}, {{44, 20}}, {{46, 21}}, {{47, 23}}, {{47, 24}}, {{46, 26}},
	{{44, 27}}, {{43, 27}}, {{41, 26}}, {{40, 24}}, {{40, 23}}, {{41, 21}},
};

Uint32 OrbitSparkScript[] = {
	SC_WAIT(6),
	SC_RELEASE,
	SC_END,
};

Uint32 GaiaRockInsideRisingSparkScript[] = {
	SC_MOVE_BY(0, 160, 0),
	SC_SYNC,
	SC_RELEASE,
	SC_END,
};

Sint32 GaiaRockInsideCirclingSparkScript[] = {
	SC_CALL(syOrbiterSpiral),
	SC_RELEASE,
	SC_END,
};

#include "shared/lesson_seeking_spark_script.c"

#include "shared/lesson_scripts.c"

// The metatiles the two stumps may be moved onto, numbered from 1; row 0 is none.
Sint16 GaiaRockInsideStumpSpots[][2] = {{0, 0}, {113, 47}, {115, 47}, {115, 45}, {117, 45}, {119, 45}, {119, 47}, {121, 47}, {117, 49}, {119, 49}, {119, 51}, {121, 51}};

// The swaying platforms' places, in metatiles.
Sint16 GaiaRockInsidePlatformPlaces[][2] = {{45, 51}, {31, 53}, {31, 57}, {39, 59}, {41, 59}, {43, 63}, {45, 63}, {35, 69}, {37, 69}};

// The same script as FieldEffectMediumScript, under another name.
Uint32 GaiaRockInsideSerpentGlowScript[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
	SC_END,
};

// The same script as FieldEffectMediumScript, under another name.
Uint32 GaiaRockInsideSerpentSmokeScript[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
	SC_END,
};

// The dark shadow under the east altar as it drops into the twin altars' room.
Uint32 GaiaRockInsideAltarShadowScript[] = {
	SC_WAIT(1),
	SC_END,
};

// The lair's blend, copied over the map's own blend script: the overhead layer blended over the rest at 4, 6, 8 and 6 sixteenths, 30 ticks each, over and over.
Uint16 GaiaRockInsideLairBlendScript[12] ALIGNED(4) = {
	BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
	BLEND_ALPHA(4, 16),
	30,
	BLEND_ALPHA(6, 16),
	30,
	BLEND_ALPHA(8, 16),
	30,
	BLEND_ALPHA(6, 16),
	30,
	BLEND_SCRIPT_JUMP | 0,
	BLEND_SCRIPT_LOOP,
};

// Each maze room's place in the map, in metatiles.
Sint8 GaiaRockInsideMazeOrigins[][2] = {{0, 0}, {1, 1}, {17, 1}, {33, 1}, {49, 1}, {49, 12}, {49, 23}, {49, 34}, {49, 45}, {33, 45}, {17, 45}, {1, 45}, {1, 34}, {1, 23}, {1, 12}, {0, 0}};

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

// A region below the floor of the altar room's cobweb pit, which leads down to the twin altars' room.
SCENE_REGION GaiaRockInsideAltarExitRegions[] = {
	{700, -30, 330, 720, -10, 350, CONDITION_ALWAYS,
	 GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_B},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(GaiaRockInsideMaze),
	SCENE_EXIT(1, GaiaRockInsideMaze, 4),
	SCENE_EXIT(2, GaiaRockInsideMaze, 3),
	SCENE_EXIT(3, GaiaRockInsideMaze, 2),
	SCENE_EXIT(4, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR_B, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE),
	SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_ALTAR, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_B, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_C, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_D, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR_C, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE),
	SCENE_EXIT(251, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_B),
	SCENE_EXITS(GaiaRockInsideFirstAltar),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_GAIA_ROCK_SUSA, GaiaRockSusa, GAIA_ROCK_SUSA_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_STUMPS, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_STUMPS_FROM_ALTAR),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_STUMPS_TO_ALTAR, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_STUMPS),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_GAIA_ROCK_INSIDE_MAZE, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_WEST_PASSAGE, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_WEST_PASSAGE_FROM_ALTAR),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_WEST_PASSAGE_TO_ALTAR, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_WEST_PASSAGE),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_GAIA_ROCK_INSIDE_PLATFORMS_WEST, GaiaRockInsidePlatforms, GAIA_ROCK_INSIDE_PLATFORMS_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_EAST_PASSAGE, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_PASSAGE_FROM_ALTAR),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_PASSAGE_TO_ALTAR, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_EAST_PASSAGE),
	SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_GAIA_ROCK_INSIDE_PLATFORMS_EAST, GaiaRockInsidePlatforms, GAIA_ROCK_INSIDE_PLATFORMS_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_B),
	SCENE_EXITS(GaiaRockInsideTwinAltars),
	SCENE_EXIT(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_PLATFORMS, GaiaRockInsidePlatforms, GAIA_ROCK_INSIDE_PLATFORMS_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS),
	SCENE_EXIT(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_ALTAR_NORTHWEST, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_NORTHWEST),
	SCENE_EXIT(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_MAZE, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_A),
	SCENE_EXITS(GaiaRockInsidePlatforms),
	SCENE_EXIT(GAIA_ROCK_INSIDE_PLATFORMS_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_A, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_PLATFORMS_EAST),
	SCENE_EXIT(GAIA_ROCK_INSIDE_PLATFORMS_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_B, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_PLATFORMS_WEST),
	SCENE_EXIT(GAIA_ROCK_INSIDE_PLATFORMS_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_PLATFORMS),
	SCENE_EXITS(GaiaRockInsideAltar),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_WALL_TO_ALTAR_WEST, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_ALTAR_WEST_FROM_WALL),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_ALTAR_WEST_TO_WALL, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_ALTAR_WEST),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_WALL_TO_ALTAR_EAST, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_ALTAR_EAST_TO_WALL, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_ALTAR_EAST),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_WALL_TO_COBWEB, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_COBWEB_TO_WALL, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_WALL_FROM_COBWEB),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_NORTHWEST, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_NORTHWEST),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_B, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_B, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_B),
	SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_MAZE_B, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_A),
	SCENE_EXITS_END,
};

Uint32 GaiaRockInsideIdleScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_Y, 0),
	SC_END,
};

Sint32 GaiaRockInsideCaskSetDownScript[] = {
	SC_SPEED(1),
	SC_SET(SW_COLLISION, 129),
	SC_SET(SW_MOTION, 0),
	// Work value 2 is the facing, as SW_FACING is.
	SC_SET(2, FACING_SOUTH_SOUTHEAST),
	SC_POSITION(456, -32, 456),
	SC_SET(SW_GROUND, -toFIXED(32)),
	SC_CALL(scGaiaRockInsideCaskRests),
	SC_END,
};

Uint32 GaiaRockInsideCaskPushedScript[] = {
	SC_SPEED(0.25),
	SC_MOVE_BY(16, 0, 0),
	SC_SYNC,
	SC_END,
};

Sint32 GaiaRockInsideCaskThrownScript[] = {
	SC_SPEED(0.5),
	SC_SET(SW_COLLISION, 129),
	SC_SET(SW_MOTION, 3),
	SC_MOVE(472, -32, 344),
	SC_SYNC,
	SC_SET(SW_COLLISION, 0),
	SC_CALL(scGaiaRockInsideCaskRests),
	SC_END,
};

Sint32 GaiaRockInsideCaskEmptiesScript[] = {
	SC_SPEED(1),
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_COLLISION, 0),
	SC_MOVE_BY(4, 8, 0),
	SC_SYNC,
	SC_LABEL(1),
	SC_CALL(scGaiaRockInsideCaskTips),
	SC_WAIT(2),
	SC_LOOP(8, SC_TO(1)),
	SC_SE_IN_VIEW(197),
	SC_CALL(scGaiaRockInsideCaskPours),
	SC_WAIT(240),
	SC_LABEL(2),
	SC_CALL(scGaiaRockInsideCaskRights),
	SC_WAIT(1),
	SC_LOOP(8, SC_TO(2)),
	SC_MOVE_BY(-4, -8, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 3),
	SC_END,
};

Sint32 GaiaRockInsideCaskTossedLeftScript[] = {
	SC_SPEED(2),
	SC_MOVE(424, 0, 344),
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_SYNC,
	SC_CALL(scGaiaRockInsideCaskBursts),
	SC_WAIT(2),
	SC_POSITION(0, 0, 0),
	SC_LABEL(1),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

Sint32 GaiaRockInsideCaskTossedRightScript[] = {
	SC_SPEED(2),
	SC_MOVE(512, 0, 384),
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_SYNC,
	SC_CALL(scGaiaRockInsideCaskBursts),
	SC_WAIT(2),
	SC_POSITION(0, 0, 0),
	SC_LABEL(1),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

Sint32 GaiaRockInsideSusaFeedsSerpentScript[] = {
	SC_SPEED(1),
	SC_SET(SW_HOOK, (Sint32)scGaiaRockInsideCheckBlocked),
	SC_SET(SW_COLLISION, 129),
	SC_LABEL(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_CALL(scGaiaRockInsideSetDownCask),
	SC_WAIT(15),
	SC_SPEED(0.25),
	SC_SET(SW_STEERING, 0),
	SC_ANIMATION(9),
	SC_CALL(scGaiaRockInsideSusaPushesCask),
	SC_MOVE_BY(16, 0, 0),
	SC_SYNC,
	SC_SPEED(1),
	SC_SET(SW_STEERING, 1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ANIMATION(2),
	SC_MOVE_BY(0, 0, 16),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_WEST),
	SC_MOVE_BY(-16, 0, 0),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH),
	SC_SPEED(0.5),
	SC_CALL(scGaiaRockInsideSusaThrowsCask),
	SC_ANIMATION(7),
	SC_WAIT(15),
	SC_MOVE(472, 0, 360),
	SC_SYNC,
	SC_WAIT(10),
	SC_ANIMATION(0),
	SC_CALL(scGaiaRockInsideCaskEmpties),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(260),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_CALL(scGaiaRockInsideTossCask),
	SC_WAIT(10),
	SC_SPEED(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ANIMATION(2),
	SC_MOVE(472, 0, 456),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_WEST),
	SC_ANIMATION(0),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

ACTOR_PLACE GaiaRockInsidePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_ARROW_PLANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRockInsideLairPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SERPENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BARREL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SLATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

Uint32 GaiaRockInsideStumpScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(3) / 4),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 4),
	SC_WAIT(1),
	SC_END,
};

ACTOR_PLACE GaiaRockInsideFirstAltarActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, (Sint32)GaiaRockInsideStumpScript, 920, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, (Sint32)GaiaRockInsideStumpScript, 952, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 496, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRockInsideTwinAltarsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 672, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRockInsidePlatformsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRockInsideAltarActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 368, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, (Sint32)GaiaRockInsideIdleScript, 704, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GANGWAY, CONDITION_ALWAYS, (Sint32)GaiaRockInsideIdleScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

Sint32 GaiaRockInsideSquashYScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 32),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_Y, -toFIXED(3) / 32),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(2)),
	SC_LABEL(3),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(2, SC_TO(3)),
	SC_LABEL(4),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(2, SC_TO(4)),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(1),
	SC_END,
};

Sint32 GaiaRockInsideSquashXScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, toFIXED(3) / 32),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_X, -toFIXED(3) / 32),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(2)),
	SC_LABEL(3),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(2, SC_TO(3)),
	SC_LABEL(4),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(2, SC_TO(4)),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 16),
	SC_WAIT(1),
	SC_END,
};

// The ticks the arrow plants flash and pulse by.
Sint32 GaiaRockInsideArrowTick = 0;

CUE GaiaRockInsideCues[] = {
	{CUE_STEP, METATILE_BEHAVIOR_MAZE_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideTakeMazeExit)},
	{CUE_STEP, METATILE_BEHAVIOR_MAZE_WEST, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideTakeMazeExit)},
	{CUE_STEP, METATILE_BEHAVIOR_MAZE_EAST, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideTakeMazeExit)},
	{CUE_STEP, METATILE_BEHAVIOR_MAZE_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideTakeMazeExit)},
	{PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_ARROW_PLANT, FLAG_PLANT_POINTING, SCRIPT_REF(scGaiaRockInsidePlantPointsTheWay)},
	{CUE_TABLE_END},
};

CUE GaiaRockInsideLairCues[] = {
	{CUE_RAISED, RAISED_SUSA_BLOCKED, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideSusaSaysMoveAside)},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_STEP, METATILE_BEHAVIOR_BY_SUSA, FLAG_MET_SUSA, SCRIPT_REF(scGaiaRockInsideMeetSusa)},
	{CUE_TALK, ACTOR_SUSA, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideTalkToSusa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideReadSusasMind)},
	{CUE_TALK, ACTOR_SAND_TABLET, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideReadSandTablet)},
	{EXAMINE_CUE(EXAMINE_GROUND), METATILE_BEHAVIOR_CUP, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheCupsAreOverflowingWith))},
	{CUE_EXAMINE, METATILE_BEHAVIOR_SERPENT, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideChallengeSerpent)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_SERPENT_WEST, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideChallengeSerpent)},
	{EXAMINE_CUE(EXAMINE_GROUND), METATILE_BEHAVIOR_CLOUD_BRAND, FLAG_CLOUD_BRAND_FOUND, SCRIPT_REF(scGaiaRockInsideFindCloudBrand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideSinkIntoSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideRiseFromSand)},
	{CUE_TABLE_END},
};

CUE GaiaRockInsideSlainLairCues[] = {
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_STEP, METATILE_BEHAVIOR_BEFORE_SERPENT, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideRaiseSandTablet)},
	{CUE_TALK, ACTOR_SAND_TABLET, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideReadSandTablet)},
	{EXAMINE_CUE(EXAMINE_GROUND), METATILE_BEHAVIOR_CUP, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheCupsAreOverflowingWith))},
	{CUE_EXAMINE, METATILE_BEHAVIOR_SERPENT, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheSerpentWithItsEyes))},
	{EXAMINE_CUE(EXAMINE_GROUND), METATILE_BEHAVIOR_CLOUD_BRAND, FLAG_CLOUD_BRAND_FOUND, SCRIPT_REF(scGaiaRockInsideFindCloudBrand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideSinkIntoSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideRiseFromSand)},
	{CUE_TABLE_END},
};

CUE GaiaRockInsideFirstAltarCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
	{CUE_TALK, ACTOR_FIRST_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCheckAltar)},
	{USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), CUE_SUBJECT_OWN_EFFECT | ACTOR_FIRST_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsidePlaceDollOnFirstAltar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP_A, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideRecordStumpSpots)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP_B, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideRecordStumpSpots)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideRecordStumpSpots)},
	{CUE_TABLE_END},
};

CUE GaiaRockInsideTwinAltarsCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_TALK, ACTOR_WEST_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCheckAltar)},
	{USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), CUE_SUBJECT_OWN_EFFECT | ACTOR_WEST_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsidePlaceDollOnWestAltar)},
	{CUE_TALK, ACTOR_EAST_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCheckAltar)},
	{USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), CUE_SUBJECT_OWN_EFFECT | ACTOR_EAST_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsidePlaceDollOnEastAltar)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_SUNBEAM_BUSHES, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCycloneClearsTwinAltarsBushes)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), -1, 0, 0},
};

CUE GaiaRockInsidePlatformsCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_PLATFORMS_BUSHES, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCycloneStartsPlatforms)},
	{CUE_TABLE_END},
};

CUE GaiaRockInsideAltarCues[] = {
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING | FACING_NORTH, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING | FACING_NORTH, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING | FACING_NORTH, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_COBWEB_LEDGE, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideJumpDownCobwebPit)},
	{CUE_STEP, METATILE_BEHAVIOR_WEST_STONE_HEAD, FLAG_WEST_STONE_HEAD_RISEN, SCRIPT_REF(scGaiaRockInsideClimbOntoWestStoneHead)},
	{CUE_STEP, METATILE_BEHAVIOR_MIDDLE_STONE_HEAD, FLAG_MIDDLE_STONE_HEAD_RISEN, SCRIPT_REF(scGaiaRockInsideClimbOntoMiddleStoneHead)},
	{CUE_STEP, METATILE_BEHAVIOR_EAST_STONE_HEAD, FLAG_EAST_STONE_HEAD_RISEN, SCRIPT_REF(scGaiaRockInsideClimbOntoEastStoneHead)},
	{CUE_TALK, ACTOR_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCheckAltar)},
	{USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), CUE_SUBJECT_OWN_EFFECT | ACTOR_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsidePlaceDollOnAltarAltar)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_VINE, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_ALTAR_BUSHES, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCycloneClearsAltarBushes)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_MAD_PLANT_BUSHES, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCycloneClearsMadPlantBushes)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_COBWEB, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCycloneClearsCobweb)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_COBWEB_LEDGE, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCycloneClearsCobweb)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_MAD_PLANT, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockInsideCycloneMadPlant)},
	{CUE_TABLE_END},
};

#include "shared/debris_script.c"

#include "shared/stone_head_rise_script.c"
// clang-format on
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef Test
#undef C1
#undef C2
#undef C3
#undef C4
#undef C6
#undef GRID_WIDTH
#undef PathWork
#undef AT
#undef AT_ADDRESS
#undef GAIA_ROCK_INSIDE_PATH
#undef SceneStateWork
#undef scGaiaRockInsidePulseUpDownArrow
#undef scGaiaRockInsidePulseSidewaysArrow
