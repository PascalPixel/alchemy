/* The Islet Cave, which the lonely turtle on the Sea of Time Islet opens once
   he has Li'l Turtle for a friend. Its three maps are all here.

   The halls (SCENE_ISLET_CAVE_HALLS): stairs come down from the islet into a
   small south room, and more stairs lead down from it into the west hall, a
   lake with an island holding a Teleport circle. Golden logs float in the water
   and carry Felix across as he walks onto them (scIsletCaveRideLog): one from
   the island to the west shore and the Turtle Boots chest, one across the north
   of the lake to the ledge of the Rusty Staff chest, four that cross the east
   hall's long channel in pairs, and a seventh that carries him all the way up
   that channel to Meld, a Venus Djinni.

   With Teleport, the island's circle sends the party to a second circle on a
   ledge at the north of the lake, behind which a doorway leads into five
   corridors (SCENE_ISLET_CAVE_CORRIDORS), each darker and bluer than the last
   (scIsletCaveDarkenPalette). Serac, a Mercury Djinni, hides in the winged
   statue of the second corridor until Tremor shakes him out. The fifth
   corridor's door opens on the Sentinel's room (SCENE_ISLET_CAVE_SENTINEL),
   where the Sentinel guards the Catastrophe tablet. */

#include "field/field.h"
#include "graphics/palette_buffers.h"
#include "macros.h"
#include "field/script_inline.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/field_sprite.h"
#include "game/game.h"
#include "field/actor_script.h"

void scTeleportArriveWhirling(void);

Sint32 syListCountEntries(Uint16 *list);
void   fdPaletteCycleFade(void);
void   scIsletCavePlaceLogs(void);
void   scIsletCaveFightMeld(void);
void   scIsletCaveFightSerac(void);
void   scIsletCaveTeleportToNorthCircle(void);
void   scIsletCaveTeleportToIslandCircle(void);
void   scIsletCaveOpenDoorAndEnter(void);
void   scIsletCaveSettleFirstLog(void);
void   scIsletCaveSettleSecondLog(void);
void   scIsletCaveSettleThirdLog(void);
void   scIsletCaveSettleFourthLog(void);
void   scIsletCaveSettleFifthLog(void);
void   scIsletCaveSettleSixthLog(void);
void   scIsletCaveSettleSeventhLog(void);
void   scIsletCaveRideFirstLogEast(void);
void   scIsletCaveRideSecondLogEast(void);
void   scIsletCaveRideSecondLogWest(void);
void   scIsletCaveRideThirdLogEast(void);
void   scIsletCaveRideFourthLogEast(void);
void   scIsletCaveRideFourthLogWest(void);
void   scIsletCaveRideFifthLogEast(void);
void   scIsletCaveRideFifthLogWest(void);
void   scIsletCaveRideSixthLogEast(void);
void   scIsletCaveRideSixthLogWest(void);
void   scIsletCaveRideSeventhLogNorth(void);
void   scIsletCaveRideSeventhLogSouth(void);
void   scIsletCaveSentinelAwaits(void);
void   scIsletCaveTremorRevealsSerac(void);

extern Uint32 IsletCaveSentinelScript[];

extern Sint32 IsletCaveShrunkFireScript[];
extern Sint16 IsletCaveDoorFrames[];

/* The Teleport circle's shimmer (shared/teleport_shimmer.c): the colours it
   writes and the palette cycle that runs them round the circle. */
Uint16 PaletteCycleBright		  BSS_DATA;
Uint16 *PaletteCycleSlots		  BSS_DATA;
Uint16 PaletteCycleTimer		  BSS_DATA;
Uint16 PaletteCycleDelay BSS_DATA ALIGNED(4);
Uint16 *PaletteSlots			  BSS_DATA;
Uint16 *PaletteColors			  BSS_DATA;
Uint16 *PaletteCycleColors		  BSS_DATA;
Uint16 PaletteCycleLength		  BSS_DATA;
Uint16 PaletteCycleIndex BSS_DATA ALIGNED(4);
void							  fdTeleportPlaySparks(void);

#include "shared/teleport_shimmer.c"

#include "text/msg_ids.h"

/* Meld, the Djinni on the ledge at the north end of the east hall, placed after
   the halls' seven logs. */
enum { ACTOR_MELD = ACTOR_FIRST_PLACED + 7 };

/* Talking to Meld, the Venus Djinni on the ledge the seventh log carries Felix
   up to at the north end of the east hall, starts a battle with him. */
void scIsletCaveFightMeld(void)
{
	fdRequestDjinnBattle(ACTOR_MELD, DJINNI_MELD);
}

/* Serac, the Djinni hiding in the second corridor, and the winged statue Tremor
   shakes him out of. */
enum { ACTOR_WINGED_STATUE = ACTOR_FIRST_PLACED, ACTOR_SERAC };

/* The Sentinel, and the first of the ten small fires burning in pairs down both
   sides of its room. */
enum { ACTOR_SENTINEL = ACTOR_FIRST_PLACED, ACTOR_FIRST_FIRE };

/* Set once the Sentinel is beaten, as the party comes back to its room. */
#define FLAG_SENTINEL_DEFEATED 0x9cf

/* Set once Tremor has shaken Serac out of the winged statue, or on entering
   once he is caught or has run off to hide: the statue then stays still. */
#define FLAG_SERAC_REVEALED FLAG_SCENE_TEMP_30

/* Where the battle with the Sentinel puts the party back in its room: the spot
   it stood on. */
enum { ENTRANCE_AFTER_SENTINEL_BATTLE = 2 };

/* The metatiles Teleport answers on: the west hall's two circles. */
enum {
	METATILE_BEHAVIOR_ISLAND_CIRCLE = 50,
	METATILE_BEHAVIOR_NORTH_CIRCLE = 51
};

/* The stone doors at the north ends of the five corridors: each door's metatile
   behaviour is the exit it leaves by. */
enum {
	EXIT_FIRST_CORRIDOR_DOOR = 2,
	EXIT_SECOND_CORRIDOR_DOOR = 4,
	EXIT_THIRD_CORRIDOR_DOOR = 6,
	EXIT_FOURTH_CORRIDOR_DOOR = 8,
	EXIT_FIFTH_CORRIDOR_DOOR = 10
};

/* The ground just before the Sentinel, where walking up to it wakes it. */
enum { METATILE_BEHAVIOR_BEFORE_SENTINEL = 10 };

/* Talking to Serac, the Mercury Djinni Tremor has shaken out of the winged
   statue in the second corridor, starts a battle with him. */
void scIsletCaveFightSerac(void)
{
	fdRequestDjinnBattle(ACTOR_SERAC, DJINNI_SERAC);
}

/* The last step of each fire's script in the Sentinel's room: clears its sprite
   flags. */
Sint32 scIsletCaveClearScriptedActorSprFlags(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
	return 0;
}

/* The winged statue's hop: one every STATUE_HOP_TICKS, its sprite rocking to
   and fro over the first half, once every STATUE_ROCK_TICKS, swung by the
   cosine's full turn shrunk by STATUE_ROCK_SHRINK. */
#define STATUE_HOP_TICKS   64
#define STATUE_ROCK_TICKS  16
#define STATUE_ROCK_SHRINK 128

/* The winged statue's script while Serac hides in it: it hops, rocking its
   sprite through the first half of each hop. */
Sint32 scIsletCaveHopWingedStatue(ACTOR *actor)
{
	Sint32	   step = actor->sceneCnt & (STATUE_HOP_TICKS - 1);
	FIELD_SPR *spr = actor->spr;

	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	if(step < STATUE_HOP_TICKS / 2) {
		if(step == 0)
			actor->velY = toFIXED(1);
		spr->rotation =
			syCos(step * (TRIG_TURN / STATUE_ROCK_TICKS)) / STATUE_ROCK_SHRINK;
	}
	actor->sceneCnt++;
	return -1;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE IsletCaveSentinelRoomPlacements[];
extern ACTOR_PLACE IsletCaveHallsPlacements[];
extern ACTOR_PLACE IsletCaveCorridorsPlacements[];
extern CUE		   IsletCaveHallsCues[];
extern CUE		   IsletCaveCorridorsCues[];
extern CUE		   IsletCaveSentinelRoomCues[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_ISLET_CAVE_HALLS)
		return IsletCaveHallsPlacements;
	if(scene == SCENE_ISLET_CAVE_CORRIDORS)
		return IsletCaveCorridorsPlacements;
	return IsletCaveSentinelRoomPlacements;
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_ISLET_CAVE_CORRIDORS)
		return IsletCaveCorridorsCues;
	if(scene == SCENE_ISLET_CAVE_SENTINEL)
		return IsletCaveSentinelRoomCues;
	return IsletCaveHallsCues;
}

/* The end of a Teleport between the two circles: the circle shimmers while the
   sparks whirl in, then the cave's colours come back and Felix is shown on
   it. */
void scIsletCaveFinishTeleport(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 effect = state->transition;

	scTeleportStartFastShimmer();
	fdTeleportPlayWhirl();
	sySynch(40);
	scTeleportStopShimmer();
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 1);
	fdPaletteFadeStartInterpolation(16);
	sySynch(16);

	MainRoutineStateWork.teleportState = TELEPORT_NONE;
	fdActorConfigureSprCol(MainRoutineStateWork.leaderActorId, 0);
	state->transition = effect;
}

/* Teleport cast on a circle: the cave turns grey, the circle shimmers and
   sparks carry Felix off, and he comes down on the other circle at (x, z). */
#define DEFINE_ISLET_CAVE_ARRIVE_AT(name, x, z) \
	void name(void) \
	{ \
		scTeleportStartShimmer(); \
		fdTeleportPlaySparks(); \
\
		MainRoutineStateWork.teleportState = TELEPORT_CHOSEN; \
		fdScriptWait(40); \
		fdActorSetHorizontalPosWithTerrain( \
			MainRoutineStateWork.leaderActorId, toFIXED(x), toFIXED(z)); \
		scIsletCaveFinishTeleport(); \
	}

/* Teleport cast on the circle on the west hall's island (metatiles (14, 29) to
   (15, 30)) carries Felix to the circle on the ledge at the north of the lake,
   whose doorway leads to the corridors. The cave turns grey, sparks whirl round
   him and he is gone; the view slides north and he comes down in a whirl of
   sparks on the other circle, and the colours come back. */
DEFINE_ISLET_CAVE_ARRIVE_AT(scIsletCaveTeleportToNorthCircle, 232, 136)

/* Teleport cast on the circle on the ledge at the north of the west hall's lake
   (metatiles (13, 7) to (15, 9)) carries Felix back south to the circle on the
   island, the way he came, with the same grey and sparks. */
DEFINE_ISLET_CAVE_ARRIVE_AT(scIsletCaveTeleportToIslandCircle, 240, 480)

/* One level of a palette fade's colour channel, which holds its 0 to 31 shifted
   up 10 bits: also the scale that leaves a colour as it is. */
#define FADE_CHANNEL_LEVEL (1 << 10)

/* Darkens the room by the scale given, out of FADE_CHANNEL_LEVEL, so that each
   corridor is darker than the last. Through the bug below only the reds and
   greens dim, so the deeper corridors and the Sentinel's room look bluer as
   well as darker. */
void scIsletCaveDarkenPalette(Sint32 scale)
{
	PALETTE_FADE *fade = Ram_HeapSlots->palFade;
	Uint16		 *src = fade->pal;
	Uint16		 *channel = fade->tgt;
	Uint32		  cnt = ARRAY_COUNT(fade->pal) / 2;
	Uint32		  i = 0;
	Uint32		  col, value, mask;

	do {
		col = *src++;
		mask = ~(FADE_CHANNEL_LEVEL - 1);
		value = RGB_BLUE(col);
#if FIX_ISLET_CAVE_BLUE_NEVER_DIMS
		value *= scale;
#else
		/* BUG: blue is multiplied by the full scale, not by scale, so it never
		   dims. */
		value *= FADE_CHANNEL_LEVEL;
#endif
		value &= mask;
		if(value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
			value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
		*channel++ = value;
		value = RGB_GREEN(col);
		value *= scale;
		value &= mask;
		if(value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
			value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
		*channel++ = value;
		value = RGB_RED(col);
		value *= scale;
		value &= mask;
		if(value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
			value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
		*channel++ = value;
		i++;
	} while(i < cnt);
}

extern MAP_GRID_METATILE_ANIMATION IsletCaveDoors[];

/* The last of the actor ids the scene's placed actors take. */
#define LAST_PLACED_ACTOR 63

/* Walking north into the grey stone door at the north end of a corridor opens
   it: everything stops, the door slides open, Felix steps into the dark
   doorway, and the next corridor comes up with him at its south end. The fifth
   corridor's door leads to the Sentinel's room. The doors are at metatiles (8,
   6), (20, 6), (32, 6), (43, 6) and (54, 6), each metatile's behaviour the exit
   it leaves by. */
void scIsletCaveOpenDoorAndEnter(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Uint32		 i;
	Uint32		 idx;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	for(i = ACTOR_FIRST_PLACED; i <= LAST_PLACED_ACTOR; i++) {
		ACTOR *actor = fdActorGetById(i);

		if(actor != 0)
			actor->motionFlg = 0;
	}
	idx = state->stepCue - 1;
	sySndPlay(SE_DOOR_OPEN);
	idx >>= 1;
	{
		Sint32 x = IsletCaveDoors[idx].x;
		Sint32 y = IsletCaveDoors[idx].y;

		fdMapGridAnimateMetatiles(IsletCaveDoors[idx].steps, x, y);
	}
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, -4);
	fdScriptWait(6);
	syImportScriptSetExitRequest(state->stepCue);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

extern Uint16 TeleportCirclePaletteSlots[];
extern Uint16 TeleportCircleColors[];
extern Uint16 TeleportCircleShimmerColors[];
extern Uint16 TeleportCircleShimmerSlots[];
extern Sint32 IsletCaveTremorTargetScript[];

/* Sets up the map being entered: in the halls, the logs where the party left
   them and the Teleport circles' colours; in the Sentinel's room, the Sentinel
   gone once beaten; in the corridors, Serac hidden in the winged statue until
   Tremor. The corridors darken an eighth at a time from the third, and the
   Sentinel's room is as dark as the fifth. */
Sint32 scInit(void)
{
	Sint32 dim;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	fdEffectStartBlankOverlay(0);
	if(MainRoutineStateWork.next == SCENE_ISLET_CAVE_HALLS) {
		scIsletCavePlaceLogs();
		scPaletteSetColsFade(TeleportCirclePaletteSlots, TeleportCircleColors,
			TeleportCircleShimmerColors, TeleportCircleShimmerSlots);
	} else if(MainRoutineStateWork.next == SCENE_ISLET_CAVE_SENTINEL) {
		fdActorSetSprFlags(fdActorGetById(ACTOR_FIRST_FIRE), 0);
		if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_SENTINEL_BATTLE) {
			fdActorSetPos(ACTOR_SENTINEL, 0, 0);
			syGameFlagSet(FLAG_SENTINEL_DEFEATED);
		}
	} else if(MainRoutineStateWork.next == SCENE_ISLET_CAVE_CORRIDORS) {
		if(syGameFlagGet(FLAG_DJINNI(DJINNI_SERAC)) ||
			syGameFlagGet(FLAG_DJINNI_HIDING(DJINNI_SERAC))) {
			syGameFlagSet(FLAG_SERAC_REVEALED);
		} else if(!syGameFlagGet(FLAG_SERAC_REVEALED)) {
			fdActorSetPos(ACTOR_SERAC, 0, 0);
			fdActorSetMovementTypeById(
				ACTOR_WINGED_STATUE, IsletCaveTremorTargetScript);
		}
	}
	if(MainRoutineStateWork.next == SCENE_ISLET_CAVE_CORRIDORS ||
		MainRoutineStateWork.next == SCENE_ISLET_CAVE_SENTINEL) {
		if(MainRoutineStateWork.next == SCENE_ISLET_CAVE_CORRIDORS)
			dim = (MainRoutineStateWork.entrance - 3) / 2;
		else
			dim = 3;
		scIsletCaveDarkenPalette(
			FADE_CHANNEL_LEVEL - dim * (FADE_CHANNEL_LEVEL / 8));
		fdPaletteFadePackTarget();
		fdPaletteFadeStartInterpolation(1);
		fdScriptWaitTransitionTicks();
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* The golden logs floating in the halls' water. The first lies between the
   Teleport circle's island and the west shore, the second at the north of the
   west hall's lake, and the third to sixth in pairs at the sides of the east
   hall's channel, the third and fourth at its north end and the fifth and sixth
   halfway down. These six lie north to south and float east and west. The
   seventh lies west to east across the channel's south end and floats north and
   south. */
enum {
	ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_LOG,
	ACTOR_THIRD_LOG,
	ACTOR_FOURTH_LOG,
	ACTOR_FIFTH_LOG,
	ACTOR_SIXTH_LOG,
	ACTOR_SEVENTH_LOG
};

/* The metatiles under each log, which scIsletCavePlaceLogs copies in under it
   from the map's top row: stepping onto them dips the log, and walking on
   across them rides it. */
enum {
	METATILE_BEHAVIOR_ISLET_CAVE_FIRST_LOG = 20,
	METATILE_BEHAVIOR_ISLET_CAVE_SECOND_LOG,
	METATILE_BEHAVIOR_ISLET_CAVE_THIRD_LOG,
	METATILE_BEHAVIOR_ISLET_CAVE_FOURTH_LOG,
	METATILE_BEHAVIOR_ISLET_CAVE_FIFTH_LOG,
	METATILE_BEHAVIOR_ISLET_CAVE_SIXTH_LOG,
	METATILE_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG
};

/* Set while a log lies at the far side of its water from where it starts, so
   that it is still there when the party comes back into the hall;
   scIsletCavePlaceLogs reads them. They are scene flags, so every log is back
   at its start once the party has left the cave.

   The fourth log is the odd one: its ride west, away from its start, clears its
   flag as the ride back does, so the next time the party comes into the east
   hall that log is back at the channel's east side. */
enum {
	FLAG_ISLET_CAVE_FIRST_LOG_MOVED = FLAG_SCENE_TEMP_80,
	FLAG_ISLET_CAVE_SECOND_LOG_MOVED = FLAG_SCENE_TEMP_81,
	FLAG_ISLET_CAVE_THIRD_LOG_MOVED = FLAG_SCENE_TEMP_82,
	FLAG_ISLET_CAVE_FOURTH_LOG_MOVED = FLAG_SCENE_TEMP_83,
	FLAG_ISLET_CAVE_FIFTH_LOG_MOVED = FLAG_SCENE_TEMP_84,
	FLAG_ISLET_CAVE_SIXTH_LOG_MOVED = FLAG_SCENE_TEMP_85,
	FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED = FLAG_SCENE_TEMP_86
};

/* Places a log lying west to east, three metatiles wide: at (x, z) if its flag
   says it is at the far side, then copies its metatiles in under it from the
   stored row at (srcColumn, srcRow). Its metatile behaviour is passed and
   not used. */
void scIsletCavePlaceWestEastLog(Sint32 actorId, Sint32 srcColumn,
	Sint32 srcRow, Sint32 x, Sint32 z, Sint32 metatileBehavior, Sint32 flg)
{
	ACTOR *actor = fdActorGetById(actorId);

	if(actor != NULL) {
		if(syGameFlagGet(flg))
			fdActorSetPos(actorId, x, z);
		fdActorSetSprPriority(actorId, 3);
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(srcColumn, srcRow, 3, 1,
			FIELD_METATILE(actor->x.fixed) - 1, FIELD_METATILE(actor->z.fixed));
	}
}

/* As scIsletCavePlaceWestEastLog, for a log lying north to south, three
   metatiles tall. */
void scIsletCavePlaceNorthSouthLog(Sint32 actorId, Sint32 srcColumn,
	Sint32 srcRow, Sint32 x, Sint32 z, Sint32 metatileBehavior, Sint32 flg)
{
	ACTOR *actor;

	if(fdActorGetById(actorId) != 0) {
		fdActorSetSprPriority(actorId, 3);
		if(syGameFlagGet(flg))
			fdActorSetPos(actorId, x, z);
		actor = fdActorGetById(actorId);
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(srcColumn, srcRow, 1, 3,
			FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed) - 1);
	}
}

/* Stepping onto a log plays its dip under Felix's step, set back to floating
   first so that the dip plays from its start: as he steps onto it, the log
   spreads wider and flashes darker orange for about a third of a second, then
   settles back to its narrow yellow look. */
void scIsletCaveSettleLog(Sint32 actorId)
{
	fdActorSetAnimationById(actorId, FLOATING_LOG_ANIMATION_FLOAT);
	fdActorSetAnimationById(actorId, FLOATING_LOG_ANIMATION_SETTLE);
}

/* Places the seven logs where the party left them, each with its metatiles. */
void scIsletCavePlaceLogs(void)
{
	scIsletCavePlaceNorthSouthLog(ACTOR_FIRST_LOG, 27, 0, 136, 504,
		METATILE_BEHAVIOR_ISLET_CAVE_FIRST_LOG,
		FLAG_ISLET_CAVE_FIRST_LOG_MOVED);
	scIsletCavePlaceNorthSouthLog(ACTOR_SECOND_LOG, 28, 0, 136, 232,
		METATILE_BEHAVIOR_ISLET_CAVE_SECOND_LOG,
		FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
	scIsletCavePlaceNorthSouthLog(ACTOR_THIRD_LOG, 29, 0, 856, 152,
		METATILE_BEHAVIOR_ISLET_CAVE_THIRD_LOG,
		FLAG_ISLET_CAVE_THIRD_LOG_MOVED);
	scIsletCavePlaceNorthSouthLog(ACTOR_FOURTH_LOG, 30, 0, 792, 152,
		METATILE_BEHAVIOR_ISLET_CAVE_FOURTH_LOG,
		FLAG_ISLET_CAVE_FOURTH_LOG_MOVED);
	scIsletCavePlaceNorthSouthLog(ACTOR_FIFTH_LOG, 31, 0, 856, 376,
		METATILE_BEHAVIOR_ISLET_CAVE_FIFTH_LOG,
		FLAG_ISLET_CAVE_FIFTH_LOG_MOVED);
	scIsletCavePlaceNorthSouthLog(ACTOR_SIXTH_LOG, 32, 0, 792, 376,
		METATILE_BEHAVIOR_ISLET_CAVE_SIXTH_LOG,
		FLAG_ISLET_CAVE_SIXTH_LOG_MOVED);
	scIsletCavePlaceWestEastLog(ACTOR_SEVENTH_LOG, 33, 0, 824, 120,
		METATILE_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG,
		FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
}

/* Stepping onto the first log, between the Teleport circle's island and the
   west shore of the west hall, plays the log's dip under Felix's step. */
void scIsletCaveSettleFirstLog(void)
{
	scIsletCaveSettleLog(ACTOR_FIRST_LOG);
}

/* Stepping onto the second log, at the north of the west hall's lake, plays the
   log's dip under Felix's step. */
void scIsletCaveSettleSecondLog(void)
{
	scIsletCaveSettleLog(ACTOR_SECOND_LOG);
}

/* Stepping onto the third log, at the north end of the east hall's channel,
   plays the log's dip under Felix's step. */
void scIsletCaveSettleThirdLog(void)
{
	scIsletCaveSettleLog(ACTOR_THIRD_LOG);
}

/* Stepping onto the fourth log, at the north end of the east hall's channel,
   plays the log's dip under Felix's step. */
void scIsletCaveSettleFourthLog(void)
{
	scIsletCaveSettleLog(ACTOR_FOURTH_LOG);
}

/* Stepping onto the fifth log, halfway down the east hall's channel, plays the
   log's dip under Felix's step. */
void scIsletCaveSettleFifthLog(void)
{
	scIsletCaveSettleLog(ACTOR_FIFTH_LOG);
}

/* Stepping onto the sixth log, halfway down the east hall's channel, plays the
   log's dip under Felix's step. */
void scIsletCaveSettleSixthLog(void)
{
	scIsletCaveSettleLog(ACTOR_SIXTH_LOG);
}

/* Stepping onto the seventh log, which lies across the east hall's channel,
   plays the log's dip under Felix's step. */
void scIsletCaveSettleSeventhLog(void)
{
	scIsletCaveSettleLog(ACTOR_SEVENTH_LOG);
}

/* Rides a log one stage, `dx` and `dz` pixels, carrying Felix with it: the log
   rolls over as it floats, he treads on top facing back the way he came, and it
   settles where it stops. */
void scIsletCaveRideLog(Sint32 actorId, Sint32 dx, Sint32 dz)
{
	ACTOR *leader;
	ACTOR *actor;
	FIXED  x;
	FIXED  z;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	actor = fdActorGetById(actorId);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	x = ((leader->x.fixed + toFIXED(dx)) & FIELD_METATILE_MASK) + toFIXED(8);
	z = ((leader->z.fixed + toFIXED(dz)) & FIELD_METATILE_MASK) + toFIXED(8);
	leader->speedLimit = toFIXED(1);
	leader->acceleration = toFIXED(1) / 2;
	fdActorSetMoveTarget(leader, x, leader->y.fixed, z);
	fdActorSelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
	x = ((actor->x.fixed + toFIXED(dx)) & FIELD_METATILE_MASK) + toFIXED(8);
	z = ((actor->z.fixed + toFIXED(dz)) & FIELD_METATILE_MASK) + toFIXED(8);
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	fdActorSetMoveTarget(actor, x, actor->y.fixed, z);
	if(dx < 0 || dz < 0) {
		fdActorSelectAnimation(
			actor, FLOATING_LOG_ANIMATION_ROLL_WEST_OR_NORTH);
	} else {
		fdActorSelectAnimation(
			actor, FLOATING_LOG_ANIMATION_ROLL_EAST_OR_SOUTH);
	}
	sySndPlay(SE_LOG_PUSH);
	fdActorWaitForMove(leader);
	fdActorSelectAnimation(actor, FLOATING_LOG_ANIMATION_SETTLE);
	sySndPlay(SE_STOP_EXIT_SE);
	fdScriptEnd();
}

/* Rides log `logId` `dx` pixels across the water and moves its metatiles with
   it, then sets or clears its flag (`flagCall`) to say which side it lies
   at. */
#define DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(name, logId, dx, flagCall, flag) \
	void name(void) \
	{ \
		Sint32 fromX = fdActorGetById(logId)->x.fixed; \
		Sint32 fromZ = fdActorGetById(logId)->z.fixed; \
\
		scIsletCaveRideLog(logId, dx, 0); \
		sySynch(2); \
		fdMapCopyMetatileAttributeRect(FIELD_METATILE(fromX), \
			FIELD_METATILE(fromZ) - 1, 1, 3, \
			FIELD_METATILE(fdActorGetById(logId)->x.fixed), \
			FIELD_METATILE(fromZ) - 1); \
		fdMapCopyMetatileAttributeRect( \
			0, 0, 1, 3, FIELD_METATILE(fromX), FIELD_METATILE(fromZ) - 1); \
		flagCall(flag); \
	}

/* Walking east from the west shore of the west hall, by the Turtle Boots chest,
   onto the first log (metatiles (8, 30) to (8, 32)) floats it two metatiles
   east with Felix aboard, back to the Teleport circle's island. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(scIsletCaveRideFirstLogEast, ACTOR_FIRST_LOG,
	32, syGameFlagClear, FLAG_ISLET_CAVE_FIRST_LOG_MOVED)

/* Walking west from the Teleport circle's island onto the first log (metatiles
   (10, 30) to (10, 32)) floats it two metatiles west with Felix aboard, across
   to the west shore and the Turtle Boots chest. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFirstLogWest, ACTOR_FIRST_LOG,
	-32, syGameFlagSet, FLAG_ISLET_CAVE_FIRST_LOG_MOVED)

/* Walking east from the little ledge of the Rusty Staff chest, at the west of
   the west hall's lake, onto the second log (metatiles (8, 13) to (8, 15))
   floats it 13 metatiles east with Felix aboard, with a stop halfway, back to
   the ledge where the passage from the east hall comes in. */
void scIsletCaveRideSecondLogEast(void)
{
	FIXED fromX = fdActorGetById(ACTOR_SECOND_LOG)->x.fixed;
	FIXED fromZ = fdActorGetById(ACTOR_SECOND_LOG)->z.fixed;

	scIsletCaveRideLog(ACTOR_SECOND_LOG, 112, 0);
	scIsletCaveRideLog(ACTOR_SECOND_LOG, 96, 0);
	sySynch(2);
	fdMapCopyMetatileAttributeRect(FIELD_METATILE(fromX),
		FIELD_METATILE(fromZ) - 1, 1, 3,
		FIELD_METATILE(fdActorGetById(ACTOR_SECOND_LOG)->x.fixed),
		FIELD_METATILE(fromZ) - 1);
	fdMapCopyMetatileAttributeRect(
		0, 0, 1, 3, FIELD_METATILE(fromX), FIELD_METATILE(fromZ) - 1);
	syGameFlagClear(FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
}

/* Walking west from the ledge at the east of the west hall's lake, where the
   passage from the east hall's north-west comes in, onto the second log
   (metatiles (21, 13) to (21, 15)) floats it 13 metatiles west with Felix
   aboard, with a stop halfway, across the north of the lake to the little ledge
   of the Rusty Staff chest. */
void scIsletCaveRideSecondLogWest(void)
{
	FIXED fromX = fdActorGetById(ACTOR_SECOND_LOG)->x.fixed;
	FIXED fromZ = fdActorGetById(ACTOR_SECOND_LOG)->z.fixed;

	scIsletCaveRideLog(ACTOR_SECOND_LOG, -112, 0);
	scIsletCaveRideLog(ACTOR_SECOND_LOG, -96, 0);
	sySynch(2);
	fdMapCopyMetatileAttributeRect(FIELD_METATILE(fromX),
		FIELD_METATILE(fromZ) - 1, 1, 3,
		FIELD_METATILE(fdActorGetById(ACTOR_SECOND_LOG)->x.fixed),
		FIELD_METATILE(fromZ) - 1);
	fdMapCopyMetatileAttributeRect(
		0, 0, 1, 3, FIELD_METATILE(fromX), FIELD_METATILE(fromZ) - 1);
	syGameFlagSet(FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
}

/* Walking east from the ledge at the north-west of the east hall onto the third
   log (metatiles (48, 8) to (48, 10)) floats it five metatiles east with Felix
   aboard, across the channel to its narrow east bank. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(scIsletCaveRideThirdLogEast, ACTOR_THIRD_LOG,
	80, syGameFlagSet, FLAG_ISLET_CAVE_THIRD_LOG_MOVED)

/* Walking west from the east hall's narrow east bank onto the third log
   (metatiles (53, 8) to (53, 10)) floats it five metatiles west with Felix
   aboard, back to the ledge at the channel's north-west. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideThirdLogWest, ACTOR_THIRD_LOG,
	-80, syGameFlagClear, FLAG_ISLET_CAVE_THIRD_LOG_MOVED)

/* Walking east from the ledge at the north-west of the east hall onto the
   fourth log (metatiles (49, 8) to (49, 10)) floats it five metatiles east with
   Felix aboard, back across the channel to its narrow east bank. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(scIsletCaveRideFourthLogEast,
	ACTOR_FOURTH_LOG, 80, syGameFlagClear, FLAG_ISLET_CAVE_FOURTH_LOG_MOVED)

/* Walking west from the east hall's narrow east bank onto the fourth log
   (metatiles (54, 8) to (54, 10)) floats it five metatiles west with Felix
   aboard, across the channel to the ledge at its north-west, where the passage
   to the west hall's north leaves. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(scIsletCaveRideFourthLogWest,
	ACTOR_FOURTH_LOG, -80, syGameFlagClear, FLAG_ISLET_CAVE_FOURTH_LOG_MOVED)

/* Walking east from the east hall's south-west shore, where the passage from
   the west hall comes in, onto the fifth log (metatiles (48, 22) to (48, 24))
   floats it five metatiles east with Felix aboard, across the channel to its
   east bank. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(scIsletCaveRideFifthLogEast, ACTOR_FIFTH_LOG,
	80, syGameFlagSet, FLAG_ISLET_CAVE_FIFTH_LOG_MOVED)

/* Walking west from the east hall's east bank onto the fifth log (metatiles
   (53, 22) to (53, 24)) floats it five metatiles west with Felix aboard, back
   to the south-west shore. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(scIsletCaveRideFifthLogWest, ACTOR_FIFTH_LOG,
	-80, syGameFlagClear, FLAG_ISLET_CAVE_FIFTH_LOG_MOVED)

/* Walking east from the east hall's south-west shore onto the sixth log
   (metatiles (49, 22) to (49, 24)) floats it five metatiles east with Felix
   aboard, back across the channel to its east bank. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(scIsletCaveRideSixthLogEast, ACTOR_SIXTH_LOG,
	80, syGameFlagClear, FLAG_ISLET_CAVE_SIXTH_LOG_MOVED)

/* Walking west from the east hall's east bank onto the sixth log (metatiles
   (54, 22) to (54, 24)) floats it five metatiles west with Felix aboard, across
   the channel to the south-west shore. */
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(scIsletCaveRideSixthLogWest, ACTOR_SIXTH_LOG,
	-80, syGameFlagSet, FLAG_ISLET_CAVE_SIXTH_LOG_MOVED)

/* Walking north from the ledge at the south end of the east hall onto the
   seventh log (metatiles (50, 29) to (52, 29)) floats it 22 metatiles north up
   the channel with Felix aboard, past the other logs, stopping twice on the
   way, to the foot of Meld's ledge. */
void scIsletCaveRideSeventhLogNorth(void)
{
	FIXED  fromX = fdActorGetById(ACTOR_SEVENTH_LOG)->x.fixed;
	Sint32 column = fromX;
	Sint32 dz = 112;
	Sint32 fromRow = fdActorGetById(ACTOR_SEVENTH_LOG)->z.fixed;

	dz = -dz;
	fromRow = FIELD_METATILE(fromRow);
	scIsletCaveRideLog(ACTOR_SEVENTH_LOG, 0, dz);
	scIsletCaveRideLog(ACTOR_SEVENTH_LOG, 0, dz);
	scIsletCaveRideLog(ACTOR_SEVENTH_LOG, 0, -128);
	sySynch(2);
	column = FIELD_METATILE(column) - 1;
	fdMapGridCopyMetatileAttributes(column, fromRow, 3, 1, column,
		FIELD_METATILE(fdActorGetById(ACTOR_SEVENTH_LOG)->z.fixed));
	fdMapCopyMetatileAttributeRect(0, 0, 3, 1, column, fromRow);
	syGameFlagSet(FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
}

/* Walking south from Meld's ledge at the north end of the east hall onto the
   seventh log (metatiles (50, 7) to (52, 7)) floats it 22 metatiles south down
   the channel with Felix aboard, stopping twice on the way, back to the ledge
   at the channel's south end. */
void scIsletCaveRideSeventhLogSouth(void)
{
	FIXED  fromX = fdActorGetById(ACTOR_SEVENTH_LOG)->x.fixed;
	Sint32 column = fromX;
	FIXED  fromZ = fdActorGetById(ACTOR_SEVENTH_LOG)->z.fixed;
	Sint32 fromRow = fromZ;

	scIsletCaveRideLog(ACTOR_SEVENTH_LOG, 0, 112);
	scIsletCaveRideLog(ACTOR_SEVENTH_LOG, 0, 112);
	scIsletCaveRideLog(ACTOR_SEVENTH_LOG, 0, 128);
	sySynch(2);
	column = FIELD_METATILE(column) - 1;
	fromRow = FIELD_METATILE(fromRow);
	fdMapGridCopyMetatileAttributes(column, fromRow, 3, 1, column,
		FIELD_METATILE(fdActorGetById(ACTOR_SEVENTH_LOG)->z.fixed));
	fdMapCopyMetatileAttributeRect(0, 0, 3, 1, column, fromRow);
	syGameFlagClear(FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
}

/* Walking up the Sentinel's room to the great armoured warrior at its north end
   (metatiles (9, 11) to (13, 13), the ground just before it) wakes it. It
   springs twice, the room glows warm, and it says across the screen "I am the
   guardian of wind. If you would claim my power, you must prove your worth."
   Then it comes at Felix and a battle with it starts, one the party cannot run
   from. Beating it leaves the Catastrophe tablet free behind its spot. */
void scIsletCaveSentinelAwaits(void)
{
	Sint32 snd;
	ACTOR *actor;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorStartRepeatedMotion(ACTOR_SENTINEL, 2);
	fdScriptWait(20);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 1);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(60);
	syScriptMessageShowCentered((Sint32)MsgSummonGuardianOfWind, 0, 2);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(60);
	actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(actor != 0) {
		fdActorSetDestinationById(
			ACTOR_SENTINEL, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdScriptWait(20);
	syFieldScriptSetAfterBattleDestination(
		SCENE_ISLET_CAVE_SENTINEL, ENTRANCE_AFTER_SENTINEL_BATTLE);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_HIDDEN_BOSSES, HIDDEN_BOSS_SENTINEL);
	fdScriptEnd();
}

/* Tremor cast on the winged statue that rocks and hops on its pillar at the
   west side of the second corridor shakes Serac out. The Mercury Djinni leaps
   from the statue and lands in the corridor beside Felix, and the statue stands
   still. While Serac hides, nothing marks him but the rocking statue. */
void scIsletCaveTremorRevealsSerac(void)
{
	if(!fdScriptGetFlag(FLAG_SERAC_REVEALED)) {
		ACTOR *actor = fdActorGetById(ACTOR_SERAC);
		ACTOR *owner = fdActorGetById(MainRoutineStateWork.leaderActorId);

		fdScriptSetFlag(FLAG_SERAC_REVEALED);
		fdActorSetPos(ACTOR_SERAC, 296, 160);
		actor->speedLimit = toFIXED(3) / 2;
		actor->velY = toFIXED(15);
		fdActorMoveToAndWait(
			ACTOR_SERAC, 344, FIELD_HALF_METATILE(owner->z.fixed) * 8 + 24);
		fdActorStop(ACTOR_WINGED_STATUE);
		actor->speedLimit = toFIXED(1);
		fdActorTurnToFaceEachOther(
			MainRoutineStateWork.leaderActorId, ACTOR_SERAC, 0);
	}
}

#undef DEFINE_ISLET_CAVE_ARRIVE_AT
#undef DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS

/* clang-format off */
#define TELEPORT_CIRCLE_FIRST_ENTRY 9
#include "shared/teleport_circle_palette.c"

Uint32 IsletCaveSentinelScript[] = {
	SC_SET(SW_SCALE_Y, toFIXED(13) / 10),
	SC_SET(SW_SCALE_X, toFIXED(13) / 10),
	SC_END,
};

Sint32 IsletCaveShrunkFireScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, toFIXED(-4)),
	SC_SET(SW_SCALE_X, toFIXED(4) / 5),
	SC_SET(SW_SCALE_Y, toFIXED(4) / 5),
	SC_CALL(scIsletCaveClearScriptedActorSprFlags),
	SC_END,
};

Sint32 IsletCaveTremorTargetScript[] = {
	SC_CALL(scIsletCaveHopWingedStatue),
	SC_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(IsletCaveHalls),
	SCENE_EXIT_UNLESS(ISLET_CAVE_HALLS_TO_SEA_OF_TIME_ISLET, SeaOfTimeIslet, SEA_OF_TIME_ISLET_FROM_ISLET_CAVE_HALLS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, IsletCaveHalls, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, IsletCaveHalls, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(4, IsletCaveHalls, 5, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, IsletCaveHalls, 4, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, IsletCaveHalls, 7, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, IsletCaveHalls, 6, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ISLET_CAVE_HALLS_TO_ISLET_CAVE_CORRIDORS, IsletCaveCorridors, ISLET_CAVE_CORRIDORS_FROM_ISLET_CAVE_HALLS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(9, IsletCaveHalls, 10, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(10, IsletCaveHalls, 9, EXIT_FLAG_NONE),
	SCENE_EXITS(IsletCaveCorridors),
	SCENE_EXIT_UNLESS(ISLET_CAVE_CORRIDORS_TO_ISLET_CAVE_HALLS, IsletCaveHalls, ISLET_CAVE_HALLS_FROM_ISLET_CAVE_CORRIDORS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, IsletCaveCorridors, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, IsletCaveCorridors, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(4, IsletCaveCorridors, 5, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, IsletCaveCorridors, 4, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, IsletCaveCorridors, 7, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, IsletCaveCorridors, 6, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(8, IsletCaveCorridors, 9, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(9, IsletCaveCorridors, 8, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ISLET_CAVE_CORRIDORS_TO_ISLET_CAVE_SENTINEL, IsletCaveSentinel, ISLET_CAVE_SENTINEL_FROM_ISLET_CAVE_CORRIDORS, EXIT_FLAG_NONE),
	SCENE_EXITS(IsletCaveSentinel),
	SCENE_EXIT_UNLESS(ISLET_CAVE_SENTINEL_TO_ISLET_CAVE_CORRIDORS, IsletCaveCorridors, ISLET_CAVE_CORRIDORS_FROM_ISLET_CAVE_SENTINEL, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE IsletCaveSentinelRoomPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SENTINEL, FLAG_SENTINEL_DEFEATED, (Sint32)IsletCaveSentinelScript, 184, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 152, 0, 144, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 216, 0, 144, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 136, 0, 176, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 232, 0, 176, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 152, 0, 224, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 216, 0, 224, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 136, 0, 256, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 232, 0, 256, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 136, 0, 304, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)IsletCaveShrunkFireScript, 232, 0, 304, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

ACTOR_PLACE IsletCaveHallsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 168, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 344, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 776, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 872, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 776, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 872, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 824, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MELD), MOVEMENT_TYPE_STAND, 824, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE IsletCaveCorridorsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SERAC), MOVEMENT_TYPE_STAND, 336, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

CUE IsletCaveHallsCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideFirstLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, SCRIPT_REF(IsletCave_RideFirstLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideSecondLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideSecondLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideThirdLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, SCRIPT_REF(IsletCave_RideThirdLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideFourthLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideFourthLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideFifthLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideFifthLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideSixthLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideSixthLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideSeventhLogNorth)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveRideSeventhLogSouth)},
	{CUE_STEP, METATILE_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveSettleFirstLog)},
	{CUE_STEP, METATILE_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveSettleSecondLog)},
	{CUE_STEP, METATILE_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveSettleThirdLog)},
	{CUE_STEP, METATILE_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveSettleFourthLog)},
	{CUE_STEP, METATILE_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveSettleFifthLog)},
	{CUE_STEP, METATILE_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveSettleSixthLog)},
	{CUE_STEP, METATILE_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveSettleSeventhLog)},
	{CUE_TALK, ACTOR_MELD, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveFightMeld)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), METATILE_BEHAVIOR_ISLAND_CIRCLE, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveTeleportToNorthCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), METATILE_BEHAVIOR_NORTH_CIRCLE, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveTeleportToIslandCircle)},
	{CUE_TABLE_END},
};

CUE IsletCaveCorridorsCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_FIRST_CORRIDOR_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_SECOND_CORRIDOR_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_THIRD_CORRIDOR_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_FOURTH_CORRIDOR_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_FIFTH_CORRIDOR_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveOpenDoorAndEnter)},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_WINGED_STATUE, FLAG_SERAC_REVEALED, SCRIPT_REF(scIsletCaveTremorRevealsSerac)},
	{CUE_TALK, ACTOR_SERAC, CONDITION_ALWAYS, SCRIPT_REF(scIsletCaveFightSerac)},
	{CUE_TABLE_END},
};

CUE IsletCaveSentinelRoomCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_STEP, METATILE_BEHAVIOR_BEFORE_SENTINEL, FLAG_SENTINEL_DEFEATED, SCRIPT_REF(scIsletCaveSentinelAwaits)},
	{CUE_TABLE_END},
};

/* A grey stone door sliding open, as MAPGRID_ANIMATION_STEP rows: two steps */
/* of two ticks. */
Sint16 IsletCaveDoorFrames[] = {
	72, 4, 2, 2, 2, 73, 4, 2,
	2, 2, -1, 0,
};
/* clang-format on */

/* The five corridors' doors, a row for each exit from the first: the opening
   and the metatile it plays at. */
MAP_GRID_METATILE_ANIMATION IsletCaveDoors[] = {
	{IsletCaveDoorFrames, 72, 4},
	{IsletCaveDoorFrames, 84, 4},
	{IsletCaveDoorFrames, 96, 4},
	{IsletCaveDoorFrames, 107, 4},
	{IsletCaveDoorFrames, 118, 4},
};
