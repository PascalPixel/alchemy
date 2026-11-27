// Yallam, the village near Taopo Swamp where Sunshine the blacksmith lives (its houses, inn and
// shops are in yallam_rooms.c). East of the village is a meadow of stumps ringed by worn paths
// round a big tree, where four children play the game that goes with Yepp's songs: asked, the boy
// standing there leads Felix and Jenna round the rings to the words of the songs of the stars, the
// moon and the sun, which tell the way through the Sea of Time. Cyclone scatters weeds round the
// village: the pair south of the Sanctum's door hide an Antidote and the three in the south-east
// corner a Nut. Once the party has Force, the log standing beside the cave mouth near Sunshine's
// house can be tipped into the gap below it, and Cyclone on the weed patch in the north-west corner
// opens a hole down into the cave that leads to the Masamune's chest.
#include "scene/script_layer.h"
#include "macros.h"
#include "edition.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/field.h"
#include "field/field_sprite.h"
#include "field/field_mode_script.h"
#include "game/game.h"
#include "scene/scene.h"
#include "window/window.h"
#include "text/msg_ids.h"

// Set once Force has pushed the log off its ledge, where it now lies as a step.
#define FLAG_YALLAM_LOG_FALLEN FLAG_AREA_TEMP_00

// Yallam's actors, numbering the rows of YallamPlacements.
typedef enum YALLAM_ACTOR_ID
{
	// The tree trunk on the ledge: its broken stump, which stays, and the log standing on it, which
	// Force pushes off (scYallamForceLogDown).
	ACTOR_LOG_STUMP = ACTOR_FIRST_PLACED,
	ACTOR_FALLING_LOG,
	// Unused: a standing log, placed at the map's corner out of sight, where nothing moves it.
	ACTOR_STANDING_LOG,
	// "Yallam doesn't get many visitors. Hasn't for a long time."
	ACTOR_VILLAGER_FEW_VISITORS,
	// "Our master boatsman, Yepp, died."
	ACTOR_VILLAGER_YEPP_DIED,
	// "If Yepp were alive today, our village would be thriving."
	ACTOR_VILLAGER_IF_YEPP_LIVED,
	// "Yepp found this huge bed of pearls and sold them in another town."
	ACTOR_VILLAGER_PEARL_BED,
	// "There used to be a lot of pearl divers here in Yallam."
	ACTOR_VILLAGER_PEARL_DIVERS,
	// "When we lost Yepp, all of Yallam practically fell to pieces."
	ACTOR_VILLAGER_LOST_YEPP,
	// "Yepp was the one who taught me how to handle a boat."
	ACTOR_BOATSMAN_TAUGHT_BY_YEPP,
	// "Yepp told me the only thing he could teach me was how..."
	ACTOR_BOATSMAN_TAUGHT_LITTLE
} YALLAM_ACTOR_ID;

// The children's meadow's actors, numbering the rows of YallamSongGamePlacements: the four children
// who play the song game.
typedef enum YALLAM_SONG_GAME_ACTOR_ID
{
	// "Yepp's songs are a lot of fun, because he made this game..."; he offers the song
	// (scYallamOfferYeppSong) and leads the game.
	ACTOR_SONG_LEADER = ACTOR_FIRST_PLACED,
	// "Running around in circles here while singing one of..."
	ACTOR_CIRCLES_CHILD,
	// "There's a game that goes along with the song, but it's tough."
	ACTOR_GAME_CHILD,
	// "Yepp wrote all of his children's songs while he was out here..."
	ACTOR_STORY_CHILD
} YALLAM_SONG_GAME_ACTOR_ID;

// Yallam's hidden treasure, the rows of its item table.
typedef enum YALLAM_TREASURE_ACTOR_ID
{
	ACTOR_HIDDEN_NUT = FIELD_TREASURE_ACTOR,
	ACTOR_HIDDEN_COINS,
	ACTOR_HIDDEN_ANTIDOTE
} YALLAM_TREASURE_ACTOR_ID;

// How near in front of it, in pixels, a running child stops for the other child or for the leader.
#define STOP_RANGE           24
// In every edition but the Japanese, a running child walks on while the leader is this near, in
// pixels.
#define LEADER_WALK_ON_RANGE 11

// The running children keep these flags in ACTOR.timer.
enum
{
	// Set on the circles child, who runs after ACTOR_GAME_CHILD.
	CHILD_CHASES_GAME_CHILD = 1,
	// Stops the child for the leader, whatever its heading, while a talk or Mind Read is open;
	// nothing here sets it, since a talk stops a child by its sceneByte.
	CHILD_TALKING           = 2
};

// The map metatiles Cyclone answers on, by the behaviour the map gives them: each set of weeds it
// scatters.
typedef enum YALLAM_METATILE
{
	METATILE_WEEDS_OVER_CAVE_HOLE = 20,
	METATILE_WEEDS_BELOW_SHOPS    = 21,
	METATILE_WEEDS_OVER_ANTIDOTE  = 22,
	METATILE_WEEDS_IN_CORNER      = 23,
	METATILE_WEEDS_OVER_NUT       = 24,
	METATILE_WEEDS_BY_SANCTUM     = 25
} YALLAM_METATILE;

// Where the Antidote and the Nut lie once Cyclone has scattered the weeds over them, in pixels.
#define ANTIDOTE_SPOT_X  424
#define ANTIDOTE_SPOT_Z  232
#define NUT_SPOT_X       680
#define NUT_SPOT_Z       344

// The falling log's spin, in angle units a tick: it gains this much each tick, from rest as it tips
// off the ledge and from TOPPLE_ROLL_SPIN as it rolls down.
#define TOPPLE_SPIN_GAIN 40
#define TOPPLE_ROLL_SPIN 400

void          scYallamSingMoonSong(void);

void          scYallamSingSunSong(void);
void          scYallamStartRunningChildren(void);

void          scYallamUncoverCaveHole(void);

void          scYallamScatterWeedsBelowShops(void);

void          scYallamScatterWeedsInCorner(void);

void          scYallamScatterWeedsBySanctum(void);

void          scYallamOfferYeppSong(void);

void          scYallamTalkAboutSongGame(void);

void          scYallamForceLogDown(void);

void          scYallamNoOperation(void);

extern Sint32 YallamLogRaised[];
extern Sint32 YallamStumpLowered[];
extern Uint32 YallamCirclesChildRuns[];
extern Uint32 YallamGameChildRuns[];
extern Uint16 YallamDoorOpening[];
extern Uint16 YallamSecondDoorOpening[];
extern Uint16 YallamThirdDoorOpening[];
extern Uint16 YallamSanctumDoorOpening[];

// Unused: turns an actor's sprite half a turn, as an actor script's call.
UNUSED Sint32 scYallamTurnSpriteAround(ACTOR *actor)
{
	FIELD_SPR *sprite = actor->sprite;

	sprite->rotation  = DEGtoANG(180);
	return 0;
}

// Takes the shadow off a log's part, as its actor script's call, once the script has set it in
// place.
Sint32 scYallamHideShadow(ACTOR *actor)
{
	fdActorSetSpriteFlags(actor, 0);
	return 0;
}

#include "shared/position_distance.h"

void scYallamRunSongPath(Sint32 id);
void scYallamShowSongGame(void);

// A song game runner's Actor fields of the same names, kept while the game is paused for a talk.
typedef struct SONG_RUNNER_STATE
{
	Sint32 timer;
	Sint32 userValue;
	Sint32 sceneCount;
	Sint32 sceneByte;
} SONG_RUNNER_STATE;

// The song leader's userValue in the song game: a command to every runner, which
// scYallamUpdateSongGame passes on and clears.
enum
{
	SONG_GAME_GO             = 1,
	SONG_GAME_HALT           = 2,
	SONG_GAME_HALT_AND_STAND = 3
};

// A runner's sceneCount in the song game: where it stands on its path.
enum
{
	RUNNER_GOING            = 0, // walks on along its path
	RUNNER_HALTED           = 1, // waits where it stopped
	RUNNER_HALTING          = 2, // stops where it is this tick
	RUNNER_HALTING_TO_STAND = 3, // stops and stands this tick
};

// The song game's five runners, kept while it is paused.
SONG_RUNNER_STATE YallamSavedSongGame[5] BSS_DATA ALIGNED(16);
extern MAP_GRID_METATILE_ANIMATION                YallamDoors[];

Sint32 scYallamStopIfFacingNear(ACTOR *actor, ACTOR *target, Sint32 range, Sint32 force);
void   scYallamUpdateSongGame(void);

// Stops the actor (paused, standing) when the target is nearer than range pixels and lies ahead of
// it, within a sixteenth of a turn either side of its heading; force stops it regardless, and so
// does a talk with it (its sceneByte set). Returns 1 when it stopped. A target out of range sets it
// walking again.
Sint32 scYallamStopIfFacingNear(ACTOR *actor, ACTOR *target, Sint32 range, Sint32 force)
{
	Sint32  result = 0;
	Sint32 *targetPos;
	Sint32 *actorPos;

	if (actor->sceneByte != 0)
	{
		actor->paused = 1;
		return 1;
	}
	targetPos = &target->x.fixed;
	actorPos  = &actor->x.fixed;
	if (scPosDistance(targetPos, actorPos) < range || force != 0)
	{
		Uint32 angle  = (Uint16)syAtan(target->z.fixed - actor->z.fixed, *targetPos - *actorPos);
		Uint32 left   = FACING_SIXTEENTH(angle - FACING_STEP);
		Uint32 right  = FACING_SIXTEENTH(angle + FACING_STEP);
		Uint32 ahead  = FACING_SIXTEENTH(angle);
		Uint32 facing = FACING_SIXTEENTH(actor->facing);

		if (ahead == facing || right == facing || left == facing || force != 0)
		{
			actor->paused = 1;
			fdActorSelectAnimation(actor, ANIM_STAND);
			result = 1;
		}
	}
	else
	{
		actor->paused = 0;
		fdActorSelectAnimation(actor, ANIM_WALK);
	}
	return result;
}

#if CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER
// The update of the two children running their rings. Each stops when it runs up behind the other,
// and otherwise when the leader is in its way; from patch 1 it walks on while the leader is right
// beside it.
Sint32 scYallamUpdateRunningChild(ACTOR *actor)
{
	WIN_STATE   *state = (WIN_STATE *)RamHeapSlots->windowState;
	FIELD_STATE *event = RamHeapSlots->fieldState;
	ACTOR       *leader;
	ACTOR       *target;
	Uint16      *flags;
	Bool         force = FALSE;

	leader             = fdActorGet(MainRoutineStateWork.leaderActorId);

	if (scPosDistance(&leader->x.fixed, &actor->x.fixed) <= LEADER_WALK_ON_RANGE)
	{
		actor->paused = 0;
		fdActorSelectAnimation(actor, ANIM_WALK);
		return 0;
	}
	flags = (Uint16 *)&actor->timer;
	if (*flags & CHILD_CHASES_GAME_CHILD)
		target = fdActorGet(ACTOR_GAME_CHILD);
	else
		target = fdActorGet(ACTOR_CIRCLES_CHILD);
	if (scYallamStopIfFacingNear(actor, target, STOP_RANGE, 0) == 0)
	{
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		if ((event->talk != 0 || state->mindRead != 0) && (*flags & CHILD_TALKING))
			force = TRUE;
		scYallamStopIfFacingNear(actor, leader, STOP_RANGE, force);
	}
	return 0;
}
#else
Sint32 scYallamUpdateRunningChild(ACTOR *actor)
{
	WIN_STATE   *state = (WIN_STATE *)RamHeapSlots->windowState;
	FIELD_STATE *event = RamHeapSlots->fieldState;
	Bool         force = FALSE;
	ACTOR       *target;
	Uint16      *flags = (Uint16 *)&actor->timer;

	if (*flags & CHILD_CHASES_GAME_CHILD)
		target = fdActorGet(ACTOR_GAME_CHILD);
	else
		target = fdActorGet(ACTOR_CIRCLES_CHILD);
	if (scYallamStopIfFacingNear(actor, target, STOP_RANGE, 0) == 0)
	{
		ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

		if ((event->talk != 0 || state->mindRead != 0) && (*flags & CHILD_TALKING))
			force = TRUE;
		scYallamStopIfFacingNear(actor, leader, STOP_RANGE, force);
	}
	return 0;
}
#endif

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

// The children's meadow, where the song game is played: the path off the village's east edge
// (metatiles (45, 15) to (45, 17)) leads there, and its own way out back to the village.
enum
{
	YALLAM_TO_SONG_GAME   = 12,
	YALLAM_FROM_SONG_GAME = 12,
	YALLAM_SONG_GAME      = 15,
	YALLAM_EXIT_SONG_GAME = 15
};

extern ACTOR_PLACE YallamPlacements[];
extern ACTOR_PLACE YallamCavePlacements[];
extern ACTOR_PLACE YallamSongGamePlacements[];

const Uint8       *scGetActorPlacements(void)
{
	if (MainRoutineStateWork.next == SCENE_YALLAM
	    && MainRoutineStateWork.entrance == YALLAM_SONG_GAME)
		return YallamSongGamePlacements;
	if (MainRoutineStateWork.next == SCENE_YALLAM_CAVE)
		return YallamCavePlacements;
	return YallamPlacements;
}

// Walking north into one of Yallam's doors opens it with a click and takes Felix inside: the inn's
// (metatile (35, 11)), Sunshine's by the log (metatile (15, 11)), the two other huts' (metatiles
// (34, 21) and (28, 23)), the three shops' in the ledge (metatiles (20, 20), (16, 19) and (13, 20))
// or the Sanctum's wide door in the cliff (metatiles (26, 8) and (27, 8)). Each door's metatiles
// carry its exit's number, which also picks its picture as it opens from `YallamDoors`.
void scYallamEnterDoor(void)
{
	FIELD_STATE *event = RamHeapSlots->fieldState;
	Uint32       i;
	Sint32       index;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++)
	{
		ACTOR *actor = fdActorGet(i);

		if (actor != NULL)
			actor->motionFlags = 0;
	}
	index = event->stepCue;
	fdScriptPlaySound(SE_DOOR_OPEN);
	index--;
	fdMapGridAnimateMetatiles(YallamDoors[index].steps, YallamDoors[index].x, YallamDoors[index].y);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	{
		Sint32 owner = MainRoutineStateWork.leaderActorId;

		// The sanctum's door is two metatiles wide: the leader walks straight in.
		if (index == YALLAM_TO_MIKASALLA_ROOMS_SANCTUM - 1)
			fdActorSetDestinationOffset(owner, 0, -4);
		else
			fdActorCenterAndWalk(owner, 2, -4);
	}
	syFieldScriptWait(10);
	syFieldScriptSetExitRequest(event->stepCue);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

// Cyclone cast in the weed patch in the village's north-west corner (metatiles (3, 7) to (7, 11))
// scatters the weeds and uncovers a wood-rimmed hole in the middle of the patch, the way down into
// the cave that leads to the Masamune's chest. The hole's picture and its exit to the cave
// (`YALLAM_TO_YALLAM_CAVE_WEST`) are copied from a spare metatile below the village.
void scYallamUncoverCaveHole(void)
{
	fdTileCopiesCopyTouchedMetatiles(67, 25, 5, 5, 67, 5, 1);
	fdMapGridCopyMetatilesTo(23, 29, 5, 6, 1, 1);
	fdMapGridCopyMetatileAttributes(23, 29, 1, 1, 5, 8);
}

// Cyclone cast by the weeds on the ledge below the shops' doors (metatiles (15,
// 22) and (16, 22)) scatters them; nothing lies under them. The eastern of the
// two weeds south of the Sanctum's door (metatile (27, 14) of the raised ground's layer) also runs
// this. Cast there, Cyclone still scatters that pair and shows the Antidote under them
// (scYallamUncoverAntidote). Only the metatiles Cyclone's whirl touched are cleared, and only those
// inside the two metatiles below the shops, so a cast at the Sanctum's pair clears nothing here:
// the weeds below the shops stay until Cyclone is cast beside them.
void scYallamScatterWeedsBelowShops(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(67, 25, 2, 1, 79, 22, 1);
}

// Cyclone cast at the pair of weeds on the raised ground south of the Sanctum's door (metatile (26,
// 14)) scatters both and uncovers the Antidote that lies under them, to be picked up there. Until
// the Antidote is taken, its hidden item (`ACTOR_HIDDEN_ANTIDOTE`) is set at the spot, and if Felix
// is standing on it he hops a metatile east, off it.
void scYallamUncoverAntidote(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	syFieldTileCopiesCopyTouchedMetatiles(26, 95, 2, 1, 26, 76, 2);
	if (fdScriptGetFlag(FLAG_YALLAM_ANTIDOTE) == 0)
	{
		fdActorSetPos(ACTOR_HIDDEN_ANTIDOTE, ANTIDOTE_SPOT_X, ANTIDOTE_SPOT_Z);
		if (FIELD_METATILE(leader->x.fixed) == FIELD_METATILE(toFIXED(ANTIDOTE_SPOT_X))
		    && FIELD_METATILE(leader->z.fixed) == FIELD_METATILE(toFIXED(ANTIDOTE_SPOT_Z)))
		{
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
			                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 16, 0);
			leader->velocityY = toFIXED(6);
			scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdScriptEnd();
		}
	}
}

// Cyclone cast at the three weeds in the village's south-east corner (metatiles (44, 22), (43, 23)
// and (44, 23)) scatters them all; the Nut lies under the one at metatile (42, 21)
// (scYallamUncoverNut).
void scYallamScatterWeedsInCorner(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(67, 25, 3, 3, 106, 20, 1);
}

// Cyclone cast at the westernmost of the three weeds in the village's south-east corner (metatile
// (42, 21)) scatters them all and uncovers the Nut under it, to be picked up there. Until the Nut
// is taken, its hidden item is set at the spot, and if Felix is standing on it he hops a metatile
// north, off it.
void scYallamUncoverNut(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	syFieldTileCopiesCopyTouchedMetatiles(67, 25, 3, 3, 106, 20, 1);
	if (fdScriptGetFlag(FLAG_YALLAM_NUT) == 0)
	{
		fdActorSetPos(ACTOR_HIDDEN_NUT, NUT_SPOT_X, NUT_SPOT_Z);
		if (FIELD_METATILE(leader->x.fixed) == FIELD_METATILE(toFIXED(NUT_SPOT_X))
		    && FIELD_METATILE(leader->z.fixed) == FIELD_METATILE(toFIXED(NUT_SPOT_Z)))
		{
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
			                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, -16);
			leader->velocityY = toFIXED(6);
			scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdScriptEnd();
		}
	}
}

// Cyclone cast at the two weeds just west of the Sanctum's door (metatiles (24,
// 8) and (23, 9)) scatters them; nothing lies under them.
void scYallamScatterWeedsBySanctum(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(67, 25, 2, 2, 87, 8, 1);
}

extern CUE YallamCues[];
extern CUE YallamCaveCues[];
extern CUE YallamSongGameCues[];

const CUE *scGetCues(void)
{
	if (MainRoutineStateWork.next == SCENE_YALLAM
	    && MainRoutineStateWork.entrance == YALLAM_SONG_GAME)
		return YallamSongGameCues;
	if (MainRoutineStateWork.next == SCENE_YALLAM_CAVE)
		return YallamCaveCues;
	return YallamCues;
}

// The boy standing in the children's meadow asks "Do you want to hear one of the songs Yepp
// wrote?". A yes starts the song game (scYallamShowSongGame); a no, and he says this is where all
// the kids in Yallam come to play. Felix talks to him facing any way but west, south-west or
// north-west.
void scYallamOfferYeppSong(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId(MsgDoYouWant);
	// "Do you want to hear one of the songs Yepp wrote?"
	fdScriptOpenMessage(ACTOR_SONG_LEADER, 0);
	// The answer animates Isaac, who is not here, where the other questions pass the leader: nobody
	// nods or shakes his head.
	if (syFieldScriptAskYesNoWithActorAnimation(ACTOR_ISAAC, 0) == 0)
		scYallamShowSongGame();
	else
		fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	fdScriptEnd();
}

// The boy running round the big tree in the children's meadow stops, turns to Felix and says
// running round in circles while singing one of Yepp's songs is pretty tough, then runs on; Mind
// Read hears that there is a lot to remember for each song. The boy keeps his heading through the
// talk, so he runs on the way he was going; Mind Read stops him without turning him.
void scYallamTalkAboutSongCircles(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR       *actor = fdActorGet(ACTOR_CIRCLES_CHILD);
	Sint16       angle = actor->facing;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	actor->sceneByte = 1;
	if (state->psynergy == 0)
	{
		fdActorFaceActor(ACTOR_CIRCLES_CHILD, MainRoutineStateWork.leaderActorId, 0);
		// "Running around in circles here while singing one of Yepp's songs is pretty tough."
		syFieldScriptSetMessageId(MsgRunningAroundIn);
	}
	else
	{
		// "There's a lot of stuff to remember for each song. It's tough to play without making any
		// mistakes."
		syFieldScriptSetMessageId(MsgThereSALot);
	}
	fdScriptShowMessage(ACTOR_CIRCLES_CHILD, 0);
	if (state->psynergy == 0)
		actor->facing = angle;
	actor->sceneByte = 0;
	fdScriptEnd();
}

// The girl with a topknot running round the big tree in the children's meadow stops, turns to Felix
// and says the game that goes with the song is tough and she wants to play something else, then
// runs on; Mind Read hears she once heard that children's songs and stories have hidden meanings.
void scYallamTalkAboutSongGame(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR       *actor = fdActorGet(ACTOR_GAME_CHILD);
	Sint16       angle = actor->facing;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	actor->sceneByte = 1;
	if (state->psynergy == 0)
	{
		fdActorFaceActor(ACTOR_GAME_CHILD, MainRoutineStateWork.leaderActorId, 0);
		// "There's a game that goes along with the song, but it's tough. I want to play something
		// else."
		syFieldScriptSetMessageId(MsgThereSAGame);
	}
	else
	{
		// "I once heard that children's songs and stories have hidden meanings."
		syFieldScriptSetMessageId(MsgIOnceHeard);
	}
	fdScriptShowMessage(ACTOR_GAME_CHILD, 0);
	if (state->psynergy == 0)
		actor->facing = angle;
	actor->sceneByte = 0;
	fdScriptEnd();
}

// Whether an actor has reached its target, which is then unset.
Bool scYallamHasArrived(ACTOR *actor)
{
	Sint32 x = actor->targetX;

	if (x == ACTOR_NO_TARGET && actor->targetZ == ACTOR_NO_TARGET)
		return TRUE;
	return FALSE;
}

// One step of a runner's path round the meadow's rings in the song game, by its step counter,
// stopping or going on by the game's mode. Each verse of the songs ends at a step the singing waits
// for (scYallamShowSongGame).
void scYallamRunSongPath(Sint32 actorId)
{
	ACTOR  *actor = fdActorGetById(actorId);
	Uint8   state = actor->sceneCount;
	Sint16 *timer;

	if (state == RUNNER_HALTED)
		return;
	timer = &actor->timer;
	switch (state)
	{
		case RUNNER_GOING           : fdActorSelectAnimation(actor, ANIM_WALK); break;
		case RUNNER_HALTED          : break;
		case RUNNER_HALTING_TO_STAND: fdActorSelectAnimation(actor, ANIM_STAND);
		case RUNNER_HALTING:
			fdActorSetMoveTarget(actor, actor->x.fixed, actor->y.fixed, actor->z.fixed);
			*timer            &= ~1;
			actor->sceneCount  = RUNNER_HALTED;
			return;
	}
	switch (*timer)
	{
		// The stars song: north past the twins and two swirls, then twice round the trunk.
		case 0:
			fdActorSetDestination(actorId, 344, 824);
			(*timer)++;
			return;
		case 1:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 2:
			fdActorSetDestination(actorId, 324, 820);
			(*timer)++;
			return;
		case 3:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 4:
			fdActorSetDestination(actorId, 320, 808);
			(*timer)++;
			return;
		case 5:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 6:
			fdActorSetDestination(actorId, 324, 796);
			(*timer)++;
			return;
		case 7:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 8:
			fdActorSetDestination(actorId, 344, 792);
			(*timer)++;
			return;
		case 9:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 10:
			fdActorSetDestination(actorId, 364, 796);
			(*timer)++;
			return;
		case 11:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 12:
			fdActorSetDestination(actorId, 368, 808);
			(*timer)++;
			return;
		case 13:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 14:
			fdActorSetDestination(actorId, 364, 820);
			(*timer)++;
			return;
		case 15:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 16:
			fdActorSetDestination(actorId, 344, 824);
			(*timer)++;
			return;
		case 17:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 18:
			fdActorSetDestination(actorId, 324, 820);
			(*timer)++;
			return;
		case 19:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 20:
			fdActorSetDestination(actorId, 320, 808);
			(*timer)++;
			return;
		case 21:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 22:
			fdActorSetDestination(actorId, 324, 796);
			(*timer)++;
			return;
		case 23:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 24:
			fdActorSetDestination(actorId, 344, 792);
			(*timer)++;
			return;
		case 25:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 26:
			fdActorSetDestination(actorId, 364, 796);
			(*timer)++;
			return;
		case 27:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 28:
			fdActorSetDestination(actorId, 368, 808);
			(*timer)++;
			return;
		case 29:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 30:
			fdActorSetDestination(actorId, 364, 820);
			(*timer)++;
			return;
		case 31:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 32:
			fdActorSetDestination(actorId, 344, 824);
			(*timer)++;
			return;
		case 33:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// East, then three to the north.
		case 34:
			fdActorSetSpeedAndAcceleration(actorId, 1.8, 1.8);
			fdActorSetDestination(actorId, 364, 820);
			(*timer)++;
			return;
		case 35:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 36:
			fdActorSetDestination(actorId, 368, 808);
			(*timer)++;
			return;
		case 37:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 38:
			fdActorSetDestination(actorId, 392, 808);
			(*timer)++;
			return;
		case 39:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 40:
			fdActorSetDestination(actorId, 392, 744);
			(*timer)++;
			return;
		case 41:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// Thrice round, then north to the stars.
		case 42:
			fdActorSetSpeedAndAcceleration(actorId, 1.2, 1.2);
			fdActorSetDestination(actorId, 372, 740);
			(*timer)++;
			return;
		case 43:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 44:
			fdActorSetDestination(actorId, 368, 728);
			(*timer)++;
			return;
		case 45:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 46:
			fdActorSetDestination(actorId, 372, 716);
			(*timer)++;
			return;
		case 47:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 48:
			fdActorSetDestination(actorId, 392, 712);
			(*timer)++;
			return;
		case 49:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 50:
			fdActorSetDestination(actorId, 412, 716);
			(*timer)++;
			return;
		case 51:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 52:
			fdActorSetDestination(actorId, 416, 728);
			(*timer)++;
			return;
		case 53:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 54:
			fdActorSetDestination(actorId, 412, 740);
			(*timer)++;
			return;
		case 55:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 56:
			fdActorSetDestination(actorId, 392, 744);
			(*timer)++;
			return;
		case 57:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 58:
			fdActorSetDestination(actorId, 372, 740);
			(*timer)++;
			return;
		case 59:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 60:
			fdActorSetDestination(actorId, 368, 728);
			(*timer)++;
			return;
		case 61:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 62:
			fdActorSetDestination(actorId, 372, 716);
			(*timer)++;
			return;
		case 63:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 64:
			fdActorSetDestination(actorId, 392, 712);
			(*timer)++;
			return;
		case 65:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 66:
			fdActorSetDestination(actorId, 412, 716);
			(*timer)++;
			return;
		case 67:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 68:
			fdActorSetDestination(actorId, 416, 728);
			(*timer)++;
			return;
		case 69:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 70:
			fdActorSetDestination(actorId, 412, 740);
			(*timer)++;
			return;
		case 71:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 72:
			fdActorSetDestination(actorId, 392, 744);
			(*timer)++;
			return;
		case 73:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 74:
			fdActorSetDestination(actorId, 372, 740);
			(*timer)++;
			return;
		case 75:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 76:
			fdActorSetDestination(actorId, 368, 728);
			(*timer)++;
			return;
		case 77:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 78:
			fdActorSetDestination(actorId, 372, 716);
			(*timer)++;
			return;
		case 79:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 80:
			fdActorSetDestination(actorId, 392, 712);
			(*timer)++;
			return;
		case 81:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 82:
			fdActorSetDestination(actorId, 412, 716);
			(*timer)++;
			return;
		case 83:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 84:
			fdActorSetDestination(actorId, 416, 728);
			(*timer)++;
			return;
		case 85:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 86:
			fdActorSetDestination(actorId, 412, 740);
			(*timer)++;
			return;
		case 87:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 88:
			fdActorSetDestination(actorId, 392, 744);
			(*timer)++;
			return;
		case 89:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 90:
			fdActorSetDestination(actorId, 372, 740);
			(*timer)++;
			return;
		case 91:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 92:
			fdActorSetDestination(actorId, 368, 728);
			(*timer)++;
			return;
		case 93:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 94:
			fdActorSetDestination(actorId, 372, 716);
			(*timer)++;
			return;
		case 95:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 96:
			fdActorSetDestination(actorId, 392, 712);
			(*timer)++;
			return;
		case 97:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 98:
			fdActorSetDestination(actorId, 392, 696);
			(*timer)++;
			return;
		case 99:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 100:
			fdActorSetDestination(actorId, 412, 692);
			(*timer)++;
			return;
		case 101:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 102:
			fdActorSetDestination(actorId, 416, 680);
			(*timer)++;
			return;
		case 103:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 104:
			fdActorSetDestination(actorId, 412, 668);
			(*timer)++;
			return;
		case 105:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 106:
			fdActorSetDestination(actorId, 392, 664);
			(*timer)++;
			return;
		case 107:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// The moon song's opening line.
		case 108:
			fdActorSetDestination(actorId, 372, 668);
			(*timer)++;
			return;
		case 109:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 110:
			fdActorSetDestination(actorId, 368, 680);
			(*timer)++;
			return;
		case 111:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 112:
			fdActorSetDestination(actorId, 320, 680);
			(*timer)++;
			return;
		case 113:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// West from the stars past three swirls, then twice round.
		case 114:
			fdActorSetDestination(actorId, 316, 692);
			(*timer)++;
			return;
		case 115:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 116:
			fdActorSetDestination(actorId, 296, 696);
			(*timer)++;
			return;
		case 117:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 118:
			fdActorSetDestination(actorId, 276, 692);
			(*timer)++;
			return;
		case 119:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 120:
			fdActorSetDestination(actorId, 272, 680);
			(*timer)++;
			return;
		case 121:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 122:
			fdActorSetDestination(actorId, 276, 668);
			(*timer)++;
			return;
		case 123:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 124:
			fdActorSetDestination(actorId, 296, 664);
			(*timer)++;
			return;
		case 125:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 126:
			fdActorSetDestination(actorId, 316, 668);
			(*timer)++;
			return;
		case 127:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 128:
			fdActorSetDestination(actorId, 320, 680);
			(*timer)++;
			return;
		case 129:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 130:
			fdActorSetDestination(actorId, 316, 692);
			(*timer)++;
			return;
		case 131:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 132:
			fdActorSetDestination(actorId, 296, 696);
			(*timer)++;
			return;
		case 133:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 134:
			fdActorSetDestination(actorId, 276, 692);
			(*timer)++;
			return;
		case 135:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 136:
			fdActorSetDestination(actorId, 272, 680);
			(*timer)++;
			return;
		case 137:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 138:
			fdActorSetDestination(actorId, 276, 668);
			(*timer)++;
			return;
		case 139:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 140:
			fdActorSetDestination(actorId, 296, 664);
			(*timer)++;
			return;
		case 141:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 142:
			fdActorSetDestination(actorId, 316, 668);
			(*timer)++;
			return;
		case 143:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 144:
			fdActorSetDestination(actorId, 320, 680);
			(*timer)++;
			return;
		case 145:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 146:
			fdActorSetDestination(actorId, 316, 692);
			(*timer)++;
			return;
		case 147:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 148:
			fdActorSetDestination(actorId, 296, 696);
			(*timer)++;
			return;
		case 149:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// South, then one swirl west.
		case 150:
			fdActorSetDestination(actorId, 296, 728);
			(*timer)++;
			return;
		case 151:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 152:
			fdActorSetDestination(actorId, 272, 728);
			(*timer)++;
			return;
		case 153:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// Thrice round, then south to the moon.
		case 154:
			fdActorSetDestination(actorId, 268, 740);
			(*timer)++;
			return;
		case 155:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 156:
			fdActorSetDestination(actorId, 248, 744);
			(*timer)++;
			return;
		case 157:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 158:
			fdActorSetDestination(actorId, 228, 740);
			(*timer)++;
			return;
		case 159:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 160:
			fdActorSetDestination(actorId, 224, 728);
			(*timer)++;
			return;
		case 161:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 162:
			fdActorSetDestination(actorId, 228, 716);
			(*timer)++;
			return;
		case 163:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 164:
			fdActorSetDestination(actorId, 248, 712);
			(*timer)++;
			return;
		case 165:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 166:
			fdActorSetDestination(actorId, 268, 716);
			(*timer)++;
			return;
		case 167:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 168:
			fdActorSetDestination(actorId, 272, 728);
			(*timer)++;
			return;
		case 169:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 170:
			fdActorSetDestination(actorId, 268, 740);
			(*timer)++;
			return;
		case 171:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 172:
			fdActorSetDestination(actorId, 248, 744);
			(*timer)++;
			return;
		case 173:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 174:
			fdActorSetDestination(actorId, 228, 740);
			(*timer)++;
			return;
		case 175:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 176:
			fdActorSetDestination(actorId, 224, 728);
			(*timer)++;
			return;
		case 177:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 178:
			fdActorSetDestination(actorId, 228, 716);
			(*timer)++;
			return;
		case 179:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 180:
			fdActorSetDestination(actorId, 248, 712);
			(*timer)++;
			return;
		case 181:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 182:
			fdActorSetDestination(actorId, 268, 716);
			(*timer)++;
			return;
		case 183:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 184:
			fdActorSetDestination(actorId, 272, 728);
			(*timer)++;
			return;
		case 185:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 186:
			fdActorSetDestination(actorId, 268, 740);
			(*timer)++;
			return;
		case 187:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 188:
			fdActorSetDestination(actorId, 248, 744);
			(*timer)++;
			return;
		case 189:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 190:
			fdActorSetDestination(actorId, 228, 740);
			(*timer)++;
			return;
		case 191:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 192:
			fdActorSetDestination(actorId, 224, 728);
			(*timer)++;
			return;
		case 193:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 194:
			fdActorSetDestination(actorId, 228, 716);
			(*timer)++;
			return;
		case 195:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 196:
			fdActorSetDestination(actorId, 248, 712);
			(*timer)++;
			return;
		case 197:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 198:
			fdActorSetDestination(actorId, 268, 716);
			(*timer)++;
			return;
		case 199:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 200:
			fdActorSetDestination(actorId, 272, 728);
			(*timer)++;
			return;
		case 201:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 202:
			fdActorSetDestination(actorId, 268, 740);
			(*timer)++;
			return;
		case 203:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 204:
			fdActorSetDestination(actorId, 248, 744);
			(*timer)++;
			return;
		case 205:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 206:
			fdActorSetDestination(actorId, 248, 760);
			(*timer)++;
			return;
		case 207:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 208:
			fdActorSetDestination(actorId, 228, 764);
			(*timer)++;
			return;
		case 209:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 210:
			fdActorSetDestination(actorId, 224, 776);
			(*timer)++;
			return;
		case 211:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 212:
			fdActorSetDestination(actorId, 228, 788);
			(*timer)++;
			return;
		case 213:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 214:
			fdActorSetDestination(actorId, 248, 792);
			(*timer)++;
			return;
		case 215:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// The sun song's opening line.
		case 216:
			fdActorSetDestination(actorId, 248, 840);
			(*timer)++;
			return;
		case 217:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// South from the moon through two swirls, then twice round.
		case 218:
			fdActorSetDestination(actorId, 228, 844);
			(*timer)++;
			return;
		case 219:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 220:
			fdActorSetDestination(actorId, 224, 856);
			(*timer)++;
			return;
		case 221:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 222:
			fdActorSetDestination(actorId, 228, 868);
			(*timer)++;
			return;
		case 223:
			if (scYallamHasArrived(actor) != 0)
				actor->timer++;
		case 224:
			fdActorSetDestination(actorId, 248, 872);
			actor->timer++;
			return;
		case 225:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 226:
			fdActorSetDestination(actorId, 268, 868);
			(*timer)++;
			return;
		case 227:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 228:
			fdActorSetDestination(actorId, 272, 856);
			(*timer)++;
			return;
		case 229:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 230:
			fdActorSetDestination(actorId, 268, 844);
			(*timer)++;
			return;
		case 231:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 232:
			fdActorSetDestination(actorId, 248, 840);
			(*timer)++;
			return;
		case 233:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 234:
			fdActorSetDestination(actorId, 228, 844);
			(*timer)++;
			return;
		case 235:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 236:
			fdActorSetDestination(actorId, 224, 856);
			(*timer)++;
			return;
		case 237:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 238:
			fdActorSetDestination(actorId, 228, 868);
			(*timer)++;
			return;
		case 239:
			if (scYallamHasArrived(actor) != 0)
				actor->timer++;
		case 240:
			fdActorSetDestination(actorId, 248, 872);
			actor->timer++;
			return;
		case 241:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 242:
			fdActorSetDestination(actorId, 268, 868);
			(*timer)++;
			return;
		case 243:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 244:
			fdActorSetDestination(actorId, 272, 856);
			(*timer)++;
			return;
		case 245:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 246:
			fdActorSetDestination(actorId, 268, 844);
			(*timer)++;
			return;
		case 247:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 248:
			fdActorSetDestination(actorId, 248, 840);
			(*timer)++;
			return;
		case 249:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 250:
			fdActorSetDestination(actorId, 228, 844);
			(*timer)++;
			return;
		case 251:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 252:
			fdActorSetDestination(actorId, 224, 856);
			(*timer)++;
			return;
		case 253:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// West, then six to the north.
		case 254:
			fdActorSetSpeedAndAcceleration(actorId, 1.8, 1.8);
			fdActorSetDestination(actorId, 200, 856);
			(*timer)++;
			return;
		case 255:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 256:
			fdActorSetDestination(actorId, 200, 744);
			(*timer)++;
			return;
		case 257:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		// Thrice round, then north to the sun.
		case 258:
			fdActorSetSpeedAndAcceleration(actorId, 1.2, 1.2);
			fdActorSetDestination(actorId, 180, 740);
			(*timer)++;
			return;
		case 259:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 260:
			fdActorSetDestination(actorId, 176, 728);
			(*timer)++;
			return;
		case 261:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 262:
			fdActorSetDestination(actorId, 180, 716);
			(*timer)++;
			return;
		case 263:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 264:
			fdActorSetDestination(actorId, 200, 712);
			(*timer)++;
			return;
		case 265:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 266:
			fdActorSetDestination(actorId, 220, 716);
			(*timer)++;
			return;
		case 267:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 268:
			fdActorSetDestination(actorId, 224, 728);
			(*timer)++;
			return;
		case 269:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 270:
			fdActorSetDestination(actorId, 220, 740);
			(*timer)++;
			return;
		case 271:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 272:
			fdActorSetDestination(actorId, 200, 744);
			(*timer)++;
			return;
		case 273:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 274:
			fdActorSetDestination(actorId, 180, 740);
			(*timer)++;
			return;
		case 275:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 276:
			fdActorSetDestination(actorId, 176, 728);
			(*timer)++;
			return;
		case 277:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 278:
			fdActorSetDestination(actorId, 180, 716);
			(*timer)++;
			return;
		case 279:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 280:
			fdActorSetDestination(actorId, 200, 712);
			(*timer)++;
			return;
		case 281:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 282:
			fdActorSetDestination(actorId, 220, 716);
			(*timer)++;
			return;
		case 283:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 284:
			fdActorSetDestination(actorId, 224, 728);
			(*timer)++;
			return;
		case 285:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 286:
			fdActorSetDestination(actorId, 220, 740);
			(*timer)++;
			return;
		case 287:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 288:
			fdActorSetDestination(actorId, 200, 744);
			(*timer)++;
			return;
		case 289:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 290:
			fdActorSetDestination(actorId, 180, 740);
			(*timer)++;
			return;
		case 291:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 292:
			fdActorSetDestination(actorId, 176, 728);
			(*timer)++;
			return;
		case 293:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 294:
			fdActorSetDestination(actorId, 180, 716);
			(*timer)++;
			return;
		case 295:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 296:
			fdActorSetDestination(actorId, 200, 712);
			(*timer)++;
			return;
		case 297:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 298:
			fdActorSetDestination(actorId, 220, 716);
			(*timer)++;
			return;
		case 299:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 300:
			fdActorSetDestination(actorId, 224, 728);
			(*timer)++;
			return;
		case 301:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 302:
			fdActorSetDestination(actorId, 220, 740);
			(*timer)++;
			return;
		case 303:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 304:
			fdActorSetDestination(actorId, 200, 744);
			(*timer)++;
			return;
		case 305:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 306:
			fdActorSetDestination(actorId, 180, 740);
			(*timer)++;
			return;
		case 307:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 308:
			fdActorSetDestination(actorId, 176, 728);
			(*timer)++;
			return;
		case 309:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 310:
			fdActorSetDestination(actorId, 180, 716);
			(*timer)++;
			return;
		case 311:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 312:
			fdActorSetDestination(actorId, 200, 712);
			(*timer)++;
			return;
		case 313:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 314:
			fdActorSetDestination(actorId, 200, 680);
			(*timer)++;
			return;
		case 315:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 316:
			fdActorSetDestination(actorId, 212, 676);
			(*timer)++;
			return;
		case 317:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 318:
			fdActorSetDestination(actorId, 216, 664);
			(*timer)++;
			return;
		case 319:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 320:
			fdActorSetDestination(actorId, 212, 652);
			(*timer)++;
			return;
		case 321:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		case 322:
			fdActorSetDestination(actorId, 200, 648);
			(*timer)++;
			return;
		case 323:
			if (scYallamHasArrived(actor) == 0)
				return;
			(*timer)++;
			return;
		default: return;
	}
	(*timer)++;
}

// An event: sets every runner's mode from the lead child's and moves them on.
void scYallamUpdateSongGame(void)
{
	ACTOR *songLeader   = fdActorGet(ACTOR_SONG_LEADER);
	ACTOR *gameChild    = fdActorGet(ACTOR_GAME_CHILD);
	ACTOR *circlesChild = fdActorGet(ACTOR_CIRCLES_CHILD);
	ACTOR *storyChild   = fdActorGet(ACTOR_STORY_CHILD);
	ACTOR *leader       = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 command      = songLeader->userValue;

	switch (command)
	{
		case SONG_GAME_GO:
			songLeader->sceneCount   = RUNNER_GOING;
			gameChild->sceneCount    = RUNNER_GOING;
			circlesChild->sceneCount = RUNNER_GOING;
			storyChild->sceneCount   = RUNNER_GOING;
			leader->sceneCount       = RUNNER_GOING;
			break;
		case SONG_GAME_HALT:
			songLeader->sceneCount   = RUNNER_HALTING;
			gameChild->sceneCount    = RUNNER_HALTING;
			circlesChild->sceneCount = RUNNER_HALTING;
			storyChild->sceneCount   = RUNNER_HALTING;
			leader->sceneCount       = RUNNER_HALTING;
			break;
		case SONG_GAME_HALT_AND_STAND:
			songLeader->sceneCount   = RUNNER_HALTING_TO_STAND;
			gameChild->sceneCount    = RUNNER_HALTING_TO_STAND;
			circlesChild->sceneCount = RUNNER_HALTING_TO_STAND;
			storyChild->sceneCount   = RUNNER_HALTING_TO_STAND;
			leader->sceneCount       = RUNNER_HALTING_TO_STAND;
			break;
	}
	songLeader->userValue = 0;
	scYallamRunSongPath(ACTOR_SONG_LEADER);
	scYallamRunSongPath(ACTOR_GAME_CHILD);
	scYallamRunSongPath(ACTOR_CIRCLES_CHILD);
	scYallamRunSongPath(ACTOR_STORY_CHILD);
	scYallamRunSongPath(MainRoutineStateWork.leaderActorId);
}

// Saves every runner's state and stops the game's event.
void scYallamPauseSongGame(void)
{
	ACTOR *actor;

	actor                             = fdActorGet(ACTOR_SONG_LEADER);
	YallamSavedSongGame[0].timer      = actor->timer;
	YallamSavedSongGame[0].userValue  = actor->userValue;
	YallamSavedSongGame[0].sceneCount = actor->sceneCount;
	YallamSavedSongGame[0].sceneByte  = actor->sceneByte;
	actor                             = fdActorGet(ACTOR_GAME_CHILD);
	YallamSavedSongGame[1].timer      = actor->timer;
	YallamSavedSongGame[1].userValue  = actor->userValue;
	YallamSavedSongGame[1].sceneCount = actor->sceneCount;
	YallamSavedSongGame[1].sceneByte  = actor->sceneByte;
	actor                             = fdActorGet(ACTOR_CIRCLES_CHILD);
	YallamSavedSongGame[2].timer      = actor->timer;
	YallamSavedSongGame[2].userValue  = actor->userValue;
	YallamSavedSongGame[2].sceneCount = actor->sceneCount;
	YallamSavedSongGame[2].sceneByte  = actor->sceneByte;
	actor                             = fdActorGet(ACTOR_STORY_CHILD);
	YallamSavedSongGame[3].timer      = actor->timer;
	YallamSavedSongGame[3].userValue  = actor->userValue;
	YallamSavedSongGame[3].sceneCount = actor->sceneCount;
	YallamSavedSongGame[3].sceneByte  = actor->sceneByte;
	actor                             = fdActorGet(MainRoutineStateWork.leaderActorId);
	YallamSavedSongGame[4].timer      = actor->timer;
	YallamSavedSongGame[4].userValue  = actor->userValue;
	YallamSavedSongGame[4].sceneCount = actor->sceneCount;
	YallamSavedSongGame[4].sceneByte  = actor->sceneByte;
	syFieldCloseEvent(scYallamUpdateSongGame);
}

// Restores every runner's state and starts the game's event again.
void scYallamResumeSongGame(void)
{
	ACTOR *actor;

	actor             = fdActorGet(ACTOR_SONG_LEADER);
	actor->timer      = YallamSavedSongGame[0].timer;
	actor->userValue  = YallamSavedSongGame[0].userValue;
	actor->sceneCount = YallamSavedSongGame[0].sceneCount;
	actor->sceneByte  = YallamSavedSongGame[0].sceneByte;
	actor             = fdActorGet(ACTOR_GAME_CHILD);
	actor->timer      = YallamSavedSongGame[1].timer;
	actor->userValue  = YallamSavedSongGame[1].userValue;
	actor->sceneCount = YallamSavedSongGame[1].sceneCount;
	actor->sceneByte  = YallamSavedSongGame[1].sceneByte;
	actor             = fdActorGet(ACTOR_CIRCLES_CHILD);
	actor->timer      = YallamSavedSongGame[2].timer;
	actor->userValue  = YallamSavedSongGame[2].userValue;
	actor->sceneCount = YallamSavedSongGame[2].sceneCount;
	actor->sceneByte  = YallamSavedSongGame[2].sceneByte;
	actor             = fdActorGet(ACTOR_STORY_CHILD);
	actor->timer      = YallamSavedSongGame[3].timer;
	actor->userValue  = YallamSavedSongGame[3].userValue;
	actor->sceneCount = YallamSavedSongGame[3].sceneCount;
	actor->sceneByte  = YallamSavedSongGame[3].sceneByte;
	actor             = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor->timer      = YallamSavedSongGame[4].timer;
	actor->userValue  = YallamSavedSongGame[4].userValue;
	actor->sceneCount = YallamSavedSongGame[4].sceneCount;
	actor->sceneByte  = YallamSavedSongGame[4].sceneByte;
	sySetTickEvent(scYallamUpdateSongGame, EVENT_PASS_TICK);
}

// Readies an actor to run the song game's path from its start.
void scYallamStartRunner(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	fdActorSetSpeedAndAcceleration(id, 1.2, 1.2);
	fdActorSetAnimation(id, ANIM_WALK);
	actor->timer      = 0;
	actor->userValue  = 0;
	actor->sceneCount = RUNNER_HALTED;
	actor->sceneByte  = 0;
}

extern Uint32 YallamCirclesChildGathers[];
extern Uint32 YallamGameChildGathers[];
extern Uint32 YallamSongLeaderForms[];
extern Uint32 YallamCirclesChildForms[];
extern Uint32 YallamGameChildForms[];
extern Uint32 YallamStoryChildForms[];
extern Uint32 YallamSongLeaderStarts[];
extern Uint32 YallamGameChildStarts[];
extern Uint32 YallamCirclesChildStarts[];

// The boy sings the song of the stars ("If you want to go to the stars...") while the children lead
// Felix and Jenna round the meadow's rings, one verse at a time; then he offers the second song.
void          scYallamShowSongGame(void)
{
	ACTOR *actor = fdActorGet(ACTOR_SONG_LEADER);
	ACTOR *leader;
	syFieldScriptSetMessageId(MsgHereLetMe);
	// "Here, let me show you!"
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	scFieldActorStop(ACTOR_CIRCLES_CHILD);
	scFieldActorStop(ACTOR_GAME_CHILD);
	(fdActorGet(ACTOR_CIRCLES_CHILD))->update = 0;
	(fdActorGet(ACTOR_GAME_CHILD))->update    = 0;
	syWait(1);
	fdActorSetSpeed(ACTOR_CIRCLES_CHILD, 1.6);
	fdActorSetSpeed(ACTOR_GAME_CHILD, 1.6);
	fdActorSetScript(ACTOR_CIRCLES_CHILD, YallamCirclesChildGathers);
	fdActorSetScript(ACTOR_GAME_CHILD, YallamGameChildGathers);
	(fdCameraGetActor())->motionFlags = 0;
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(304, CAMERA_KEEP, 878, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 296, 872);
	fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId, FACING_EAST);
	fdScriptPlaySound(SOUND_FADE_OUT_BGM);
	fdActorFaceDirection(ACTOR_SONG_LEADER, FACING_NORTH_NORTHWEST, 20);
	fdActorFaceDirectionAndWait(ACTOR_SONG_LEADER, FACING_NORTH_NORTHEAST);
	scFieldActorJump(ACTOR_SONG_LEADER, 2, 10);
	scFieldActorJump(ACTOR_SONG_LEADER, 4, 10);
	fdActorSetAnimationAndWait(ACTOR_SONG_LEADER, ANIM_NOD);
	fdActorSetSpeed(ACTOR_SONG_LEADER, 1.6);
	fdActorSetSpeed(ACTOR_STORY_CHILD, 1.6);
	fdActorSetScript(ACTOR_SONG_LEADER, YallamSongLeaderForms);
	fdActorSetScript(ACTOR_CIRCLES_CHILD, YallamCirclesChildForms);
	fdActorSetScript(ACTOR_GAME_CHILD, YallamGameChildForms);
	fdActorRunScriptAndWait(ACTOR_STORY_CHILD, YallamStoryChildForms);
	fdActorSetPos(ACTOR_JENNA, 296, 872);
	syWait(1);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorWalkToAndWait(ACTOR_JENNA, 312, 854);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if (leader != 0)
	{
		fdActorSetDestination(ACTOR_JENNA, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 316, 896);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 344, 912);
	fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId, FACING_NORTH);
	fdScriptPlaySound(MUS_CHILDRENS_SONG);
	fdActorFaceDirectionAndWait(ACTOR_SONG_LEADER, FACING_NORTH);
	// "If you want to go to the stars, if you want to go to the stars."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	syScriptCameraFollowActor(ACTOR_SONG_LEADER, 1);
	fdActorSetScript(ACTOR_SONG_LEADER, YallamSongLeaderStarts);
	fdActorSetScript(ACTOR_GAME_CHILD, YallamGameChildStarts);
	fdActorRunScriptAndWait(ACTOR_CIRCLES_CHILD, YallamCirclesChildStarts);
	fdActorWalkToAndWait(ACTOR_STORY_CHILD, 344, 890);
	fdActorFaceDirectionAndWait(ACTOR_STORY_CHILD, FACING_NORTH);
	scYallamStartRunner(ACTOR_SONG_LEADER);
	scYallamStartRunner(ACTOR_GAME_CHILD);
	scYallamStartRunner(ACTOR_CIRCLES_CHILD);
	scYallamStartRunner(ACTOR_STORY_CHILD);
	scYallamStartRunner(MainRoutineStateWork.leaderActorId);
	sySetTickEvent(scYallamUpdateSongGame, EVENT_PASS_TICK);
	// "Go north past the twins, passing two swirls, run deasil twice 'round the trunk."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 34);
	actor->userValue = SONG_GAME_HALT;
	syWait(1);
	// "Haste without waste, and head to the east, past three to the north."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 42);
	actor->userValue = SONG_GAME_HALT;
	syWait(1);
	// "There, run 'round thrice and wait for the waves. When they stop, run north and go to the
	// stars."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 108);
	actor->userValue = SONG_GAME_HALT_AND_STAND;
	syWait(1);
	// "If you race full of folly and take the wrong way, you'll find yourself a watery grave."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	scYallamPauseSongGame();
	scFieldActorJump(ACTOR_SONG_LEADER, 4, 0);
	scFieldActorJump(ACTOR_GAME_CHILD, 4, 0);
	scFieldActorJump(ACTOR_CIRCLES_CHILD, 4, 0);
	scFieldActorJump(ACTOR_STORY_CHILD, 4, 40);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SONG_LEADER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GAME_CHILD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_CIRCLES_CHILD, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_STORY_CHILD, FACING_WEST);
	// "Do you wanna hear the second children's song Yepp wrote?"
	fdScriptOpenMessage(ACTOR_SONG_LEADER, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0) == 0)
	{
		scYallamSingMoonSong();
	}
	else
	{
		// "Well, that's just fine then. Give me a holler if you ever do."
		syFieldScriptShowMessageThenWait(ACTOR_SONG_LEADER, 0, 20);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptPlaySound(SE_MAP_EXIT);
		syFieldScriptWait(40);
		syFieldScriptSetExitRequest(YALLAM_EXIT_SONG_GAME);
	}
}

// The moon song's words, sung verse by verse.
extern Uint8 MsgIfYouWant[];

// The song of the moon, sung and run round the meadow's rings verse by verse as the stars song was;
// then the boy offers the third song.
void         scYallamSingMoonSong(void)
{
	ACTOR *actor = fdActorGet(ACTOR_SONG_LEADER);

	fdActorFaceDirection(ACTOR_SONG_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GAME_CHILD, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_CIRCLES_CHILD, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_STORY_CHILD, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId, FACING_EAST);
	syFieldScriptSetMessageId((Sint32)&MsgIfYouWant);
	// "If you want to go to the moon, if you want to go to the moon."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	scYallamResumeSongGame();
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 114);
	actor->userValue = SONG_GAME_HALT;
	syWait(1);
	// "Face west from the stars and run straight past three swirls, then circle 'round twice."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 150);
	actor->userValue = SONG_GAME_HALT;
	syWait(1);
	// "Haste without waste, and head to the south, then one swirl west."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 154);
	actor->userValue = SONG_GAME_HALT;
	syWait(1);
	// "There, run 'round thrice and wait for the waves. When they stop, run south and go for the
	// moon."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 216);
	actor->userValue = SONG_GAME_HALT_AND_STAND;
	syWait(1);
	// "If you race full of folly and take the wrong way, you'll find yourself a watery grave."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	scYallamPauseSongGame();
	scFieldActorJump(ACTOR_SONG_LEADER, 4, 0);
	scFieldActorJump(ACTOR_GAME_CHILD, 4, 0);
	scFieldActorJump(ACTOR_CIRCLES_CHILD, 4, 0);
	scFieldActorJump(ACTOR_STORY_CHILD, 4, 40);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SONG_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GAME_CHILD, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_CIRCLES_CHILD, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_STORY_CHILD, FACING_NORTH_NORTHEAST);
	// "OK, so, now do you want to hear the third children's song?"
	fdScriptOpenMessage(ACTOR_SONG_LEADER, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0) == 0)
	{
		scYallamSingSunSong();
	}
	else
	{
		// "Well, that's just fine then. Give me a holler if you ever do."
		syFieldScriptShowMessageThenWait(ACTOR_SONG_LEADER, 0, 20);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptPlaySound(SE_MAP_EXIT);
		syFieldScriptWait(40);
		syFieldScriptSetExitRequest(YALLAM_EXIT_SONG_GAME);
	}
}

// The sun song's words, sung verse by verse.
extern Uint8 MsgIfYouWantTo[];

// The song of the sun, sung and run round the meadow's rings verse by verse; then each child hops
// away and back in turn, the boy says that is all the games they have, and the screen fades back to
// the village.
void         scYallamSingSunSong(void)
{
	ACTOR *actor = fdActorGet(ACTOR_SONG_LEADER);
	FIXED  x, z;

	fdActorFaceDirection(ACTOR_SONG_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GAME_CHILD, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_CIRCLES_CHILD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_STORY_CHILD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST);
	syFieldScriptSetMessageId((Sint32)&MsgIfYouWantTo);
	// "If you want to go to the sun, if you want to go to the sun."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	scYallamResumeSongGame();
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 218);
	actor->userValue = SONG_GAME_HALT;
	syWait(1);
	// "Face south from the moon and run straight through two swirls, then circle 'round twice."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 254);
	actor->userValue = SONG_GAME_HALT;
	syWait(1);
	// "Haste without waste, and head to the west, then six to the north."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 258);
	actor->userValue = SONG_GAME_HALT;
	syWait(1);
	// "There, run 'round thrice and wait for the waves. When they stop, run north and go for the
	// sun."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	actor->userValue = SONG_GAME_GO;
	do
		syWait(1);
	while (actor->timer != 324);
	actor->userValue = SONG_GAME_HALT_AND_STAND;
	syWait(1);
	// "If you race full of folly and take the wrong way, you'll find yourself a watery grave."
	fdScriptShowMessage(ACTOR_SONG_LEADER, 0);
	scYallamPauseSongGame();
	fdActorFaceDirectionAndWait(ACTOR_SONG_LEADER, FACING_SOUTH_SOUTHWEST);
	fdActorGet(ACTOR_SONG_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
	actor                                         = fdActorGet(ACTOR_SONG_LEADER);
	z                                             = actor->z.fixed;
	x                                             = actor->x.fixed;
	fdActorSetSpeed(ACTOR_SONG_LEADER, 2.4);
	scFieldActorJump(ACTOR_SONG_LEADER, 6, 0);
	fdActorMoveToAndWait(ACTOR_SONG_LEADER, 200, 670);
	scFieldActorJump(ACTOR_SONG_LEADER, 4, 0);
	scFieldActorSetMoveTargetFX(actor, x, 0, z);
	syFieldScriptWait(5);
	fdActorFaceDirectionAndWait(ACTOR_GAME_CHILD, FACING_SOUTH_SOUTHWEST);
	fdActorGet(ACTOR_GAME_CHILD)->steeringFlags &= ~ACTOR_FACES_MOTION;
	actor                                        = fdActorGet(ACTOR_GAME_CHILD);
	z                                            = actor->z.fixed;
	x                                            = actor->x.fixed;
	fdActorSetSpeed(ACTOR_GAME_CHILD, 2.4);
	scFieldActorJump(ACTOR_GAME_CHILD, 6, 0);
	fdActorMoveToAndWait(ACTOR_GAME_CHILD, 200, 670);
	scFieldActorJump(ACTOR_GAME_CHILD, 4, 0);
	scFieldActorSetMoveTargetFX(actor, x, 0, z);
	syFieldScriptWait(5);
	fdActorFaceDirectionAndWait(ACTOR_CIRCLES_CHILD, FACING_WEST);
	fdActorGet(ACTOR_CIRCLES_CHILD)->steeringFlags &= ~ACTOR_FACES_MOTION;
	actor                                           = fdActorGet(ACTOR_CIRCLES_CHILD);
	z                                               = actor->z.fixed;
	x                                               = actor->x.fixed;
	fdActorSetSpeed(ACTOR_CIRCLES_CHILD, 2.4);
	scFieldActorJump(ACTOR_CIRCLES_CHILD, 6, 0);
	fdActorMoveToAndWait(ACTOR_CIRCLES_CHILD, 200, 670);
	scFieldActorJump(ACTOR_CIRCLES_CHILD, 4, 0);
	scFieldActorSetMoveTargetFX(actor, x, 0, z);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_STORY_CHILD, FACING_NORTH_NORTHWEST);
	fdActorGet(ACTOR_STORY_CHILD)->steeringFlags &= ~ACTOR_FACES_MOTION;
	actor                                         = fdActorGet(ACTOR_STORY_CHILD);
	z                                             = actor->z.fixed;
	x                                             = actor->x.fixed;
	fdActorSetSpeed(ACTOR_STORY_CHILD, 2.4);
	scFieldActorJump(ACTOR_STORY_CHILD, 6, 0);
	fdActorMoveToAndWait(ACTOR_STORY_CHILD, 200, 670);
	scFieldActorJump(ACTOR_STORY_CHILD, 4, 0);
	scFieldActorSetMoveTargetFX(actor, x, 0, z);
	scFieldActorWaitForMoveById(ACTOR_STORY_CHILD);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 184, 686);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 186, 674);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
	fdActorGet(ACTOR_SONG_LEADER)->steeringFlags   |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_GAME_CHILD)->steeringFlags    |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_CIRCLES_CHILD)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_STORY_CHILD)->steeringFlags   |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_SONG_LEADER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GAME_CHILD, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_CIRCLES_CHILD, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_STORY_CHILD, FACING_WEST, 20);
	// "That's it. That's all the games we have for the songs... If you want to play some more, just
	// give me a holler."
	syFieldScriptShowMessageThenWait(ACTOR_SONG_LEADER, 0, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptPlaySound(SE_MAP_EXIT);
	syFieldScriptWait(40);
	syFieldScriptSetExitRequest(YALLAM_EXIT_SONG_GAME);
}

// Once Force has pushed it, the log stands in the gap where it fell.
void scYallamPlaceFallenLog(void)
{
	if (fdScriptGetFlag(FLAG_YALLAM_LOG_FALLEN))
	{
		SET_ACTOR_POSITION(fdActorGet(ACTOR_FALLING_LOG), toFIXED(168), 0, toFIXED(192));
	}
}

Sint32 scInit(void)
{
	if (MainRoutineStateWork.next == SCENE_YALLAM)
	{
		if (MainRoutineStateWork.entrance == YALLAM_SONG_GAME)
			scYallamStartRunningChildren();
		else
			scYallamPlaceFallenLog();
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	if (MainRoutineStateWork.next == SCENE_YALLAM && fdScriptGetFlag(FLAG_YALLAM_LOG_FALLEN))
		fdMapGridCopyMetatileAttributes(8, 12, 1, 1, 10, 12);
	return 0;
}

// Nothing happens as Force first reaches the log; the push itself is scYallamForceLogDown.
void scYallamNoOperation(void)
{
}

// Force cast at the log standing on its stump east of the cave mouth near Sunshine's house tips it
// over west: it spins off the ledge with a crash and drops upright into the gap below (metatile
// (10, 12)), a post Felix can now hop onto. The hop comes from the map's own hop metatile at (8,
// 12), whose flags are copied onto the log's metatile.
void scYallamForceLogDown(void)
{
	Sint32     step;
	Sint32     i;
	ACTOR     *actor  = fdActorGet(ACTOR_FALLING_LOG);
	FIELD_SPR *sprite = actor->sprite;

	fdScriptSetFlag(FLAG_YALLAM_LOG_FALLEN);
	actor->acceleration = toFIXED(3) / 2;
	actor->speedLimit   = toFIXED(8);
	fdScriptPlaySound(SE_TOPPLE);
	scFieldActorSetMoveTarget(actor, 176, 40, 184);
	step = 0;
	for (i = 0; i < 16; i++)
	{
		sprite->rotation -= step;
		syWait(1);
		step += TOPPLE_SPIN_GAIN;
	}
	scFieldActorSetMoveTarget(actor, 168, 16, 184);
	step = TOPPLE_ROLL_SPIN;
	for (i = 0; i < 26; i++)
	{
		sprite->rotation -= step;
		syWait(1);
		step += TOPPLE_SPIN_GAIN;
	}
	scFieldActorWaitForMove(actor);
	syFieldScriptWait(2);
	fdScriptPlaySound(SE_LOG_FALL);
	// The log stands upright and still where it fell.
	i                = 0;
	sprite->rotation = i;
	actor->x.fixed   = toFIXED(168);
	actor->z.fixed   = toFIXED(192);
	actor->y.fixed   = i;
	actor->velocityY = i;
	actor->velocityX = i;
	fdMapGridCopyMetatileAttributes(8, 12, 1, 1, 10, 12);
}

// The boy and the girl with a topknot run round the big tree in the children's meadow, each after
// the other (scYallamUpdateRunningChild).
void scYallamStartRunningChildren(void)
{
	ACTOR *actor = fdActorGet(ACTOR_CIRCLES_CHILD);
	ACTOR *other;

	actor->timer     = CHILD_CHASES_GAME_CHILD;
	actor->sceneByte = 0;
	actor->update    = scYallamUpdateRunningChild;
	other            = fdActorGet(ACTOR_GAME_CHILD);
	other->timer     = 0;
	other->sceneByte = 0;
	other->update    = scYallamUpdateRunningChild;
}

void          scYallamEnterDoor(void);
void          scYallamUncoverAntidote(void);
void          scYallamUncoverNut(void);
void          scYallamTalkAboutSongCircles(void);

extern Sint32 YallamStandingLogLowered[];

// The log standing on its stump on the ledge, raised onto it.
Sint32        YallamLogRaised[] = {
    SC_ANIMATION(3),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(2)),
    SC_CALL((Sint32)scYallamHideShadow),
    SC_END,
};

// The stump, sunk into the ledge below the log.
Sint32 YallamStumpLowered[] = {
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, -toFIXED(16)),
	SC_CALL((Sint32)scYallamHideShadow),
	SC_END,
};

// The standing log, shrunk and sunk.
Sint32 YallamStandingLogLowered[] = {
	SC_SET(SW_SCALE_X, toFIXED(3) / 5),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 5),
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, -toFIXED(16)),
	SC_CALL((Sint32)scYallamHideShadow),
	SC_END,
};

// clang-format off
// The boy runs round the big tree in the children's meadow, a six-sided lap.
Uint32 YallamCirclesChildRuns[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SPEED(1),
	SC_ACCELERATION(0.4),
	SC_MOVE(328, 0, 752),
	SC_SYNC,
	SC_MOVE(360, 0, 752),
	SC_SYNC,
	SC_MOVE(376, 0, 776),
	SC_SYNC,
	SC_MOVE(360, 0, 792),
	SC_SYNC,
	SC_MOVE(328, 0, 792),
	SC_SYNC,
	SC_MOVE(312, 0, 776),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// As YallamCirclesChildRuns, for the game child, half a lap ahead of it.
Uint32 YallamGameChildRuns[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SPEED(1),
	SC_ACCELERATION(0.4),
	SC_MOVE(360, 0, 792),
	SC_SYNC,
	SC_MOVE(328, 0, 792),
	SC_SYNC,
	SC_MOVE(312, 0, 776),
	SC_SYNC,
	SC_MOVE(328, 0, 752),
	SC_SYNC,
	SC_MOVE(360, 0, 752),
	SC_SYNC,
	SC_MOVE(376, 0, 776),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// The children who were running gather by the song leader.
Uint32 YallamCirclesChildGathers[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(336, 0, 792),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
	SC_WAIT(10),
	SC_END,
};

// As YallamCirclesChildGathers, for the game child.
Uint32 YallamGameChildGathers[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(352, 0, 792),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
	SC_WAIT(10),
	SC_END,
};

// The children take their places in the ring for the game.
Uint32 YallamSongLeaderForms[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(316, 0, 888),
	SC_SYNC,
	SC_MOVE(336, 0, 888),
	SC_SYNC,
	SC_MOVE(344, 0, 872),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_END,
};

// As YallamSongLeaderForms, for the circles child.
Uint32 YallamCirclesChildForms[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(362, 0, 792),
	SC_SYNC,
	SC_MOVE(384, 0, 848),
	SC_SYNC,
	SC_MOVE(382, 0, 888),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_END,
};

// As YallamSongLeaderForms, for the game child.
Uint32 YallamGameChildForms[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(364, 0, 808),
	SC_SYNC,
	SC_MOVE(374, 0, 878),
	SC_SYNC,
	SC_MOVE(360, 0, 888),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_END,
};

// As YallamSongLeaderForms, for the story child.
Uint32 YallamStoryChildForms[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(274, 0, 888),
	SC_SYNC,
	SC_MOVE(328, 0, 888),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_END,
};

// The children step to where the song's run begins.
Uint32 YallamSongLeaderStarts[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(344, 0, 840),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

// As YallamSongLeaderStarts, for the game child.
Uint32 YallamGameChildStarts[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(344, 0, 888),
	SC_SYNC,
	SC_MOVE(344, 0, 856),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_END,
};

// As YallamSongLeaderStarts, for the circles child.
Uint32 YallamCirclesChildStarts[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(344, 0, 888),
	SC_SYNC,
	SC_MOVE(344, 0, 874),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(Yallam),
	SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_INN_SOUTHEAST, YallamRoomsInn, YALLAM_ROOMS_INN_FROM_YALLAM_SOUTHEAST),
	SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_INN_SOUTH, YallamRoomsInn, YALLAM_ROOMS_INN_FROM_YALLAM_SOUTH),
	SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_SHOPS_EAST, YallamRoomsShops, YALLAM_ROOMS_SHOPS_FROM_YALLAM_EAST),
	SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_INN_WEST, YallamRoomsInn, YALLAM_ROOMS_INN_FROM_YALLAM_WEST),
	SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_SHOPS_NORTH, YallamRoomsShops, YALLAM_ROOMS_SHOPS_FROM_YALLAM_NORTH),
	SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_SHOPS_WEST, YallamRoomsShops, YALLAM_ROOMS_SHOPS_FROM_YALLAM_WEST),
	SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_INN_NORTHEAST, YallamRoomsInn, YALLAM_ROOMS_INN_FROM_YALLAM_NORTHEAST),
	SCENE_EXIT(YALLAM_TO_MIKASALLA_ROOMS_SANCTUM, MikasallaRoomsSanctum, MIKASALLA_ROOMS_SANCTUM_FROM_YALLAM),
	SCENE_EXIT(YALLAM_TO_YALLAM_CAVE_EAST, YallamCave, YALLAM_CAVE_FROM_YALLAM_EAST),
	SCENE_EXIT(YALLAM_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_YALLAM_SOUTH),
	SCENE_EXIT(YALLAM_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_YALLAM_NORTH),
	SCENE_EXIT(YALLAM_TO_SONG_GAME, Yallam, YALLAM_SONG_GAME),
	SCENE_EXIT(YALLAM_TO_YALLAM_CAVE_WEST, YallamCave, YALLAM_CAVE_FROM_YALLAM_WEST),
	SCENE_EXIT(YALLAM_EXIT_SONG_GAME, Yallam, YALLAM_FROM_SONG_GAME),
	SCENE_EXITS(YallamCave),
	SCENE_EXIT(YALLAM_CAVE_TO_YALLAM_EAST, Yallam, YALLAM_FROM_YALLAM_CAVE_EAST),
	SCENE_EXIT(YALLAM_CAVE_TO_YALLAM_WEST, Yallam, YALLAM_FROM_YALLAM_CAVE_WEST),
	SCENE_EXITS_END,
};

// The village, in YALLAM_ACTOR_ID order.
ACTOR_PLACE YallamPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FALLING_LOG, CONDITION_ALWAYS,
					(Sint32)YallamStumpLowered, 184, 0, 200, FACING_WEST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FALLING_LOG, CONDITION_ALWAYS,
					(Sint32)YallamLogRaised, 184, 0, 200, FACING_WEST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STANDING_LOG, CONDITION_ALWAYS,
					(Sint32)YallamStandingLogLowered, 8, 0, 8, FACING_WEST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 216,
					FACING_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 232,
					FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
#if !YALLAM_CURLY_HAIRED_MAN_WANDERS_LESS
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 552, 0, 232,
					FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
#else
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 552, 0,
					248, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
#endif
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 360, 0, 360,
					FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 280,
					FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376,
					FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 464, 0, 136, FACING_SOUTHWEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 392, 0, 264, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

// The cave places no actors.
ACTOR_PLACE YallamCavePlacements[] = {
	{SCENE_TABLE_END},
};

// The children's meadow, in YALLAM_SONG_GAME_ACTOR_ID order, and Jenna.
ACTOR_PLACE YallamSongGamePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 872, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, (Sint32)YallamCirclesChildRuns, 312, 0, 776, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, (Sint32)YallamGameChildRuns, 376, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 800, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

// The huts' doors' metatile animations as they open, one for each shape of door.
Uint16 YallamDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(93, 28, 1, 2, 6),
	MAPGRID_ANIMATION_STEP(92, 28, 1, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};

// The second shape of hut door's opening, as YallamDoorOpening.
Uint16 YallamSecondDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(95, 28, 1, 2, 6),
	MAPGRID_ANIMATION_STEP(94, 28, 1, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};

// The third shape of hut door's opening, as YallamDoorOpening.
Uint16 YallamThirdDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(89, 28, 1, 2, 6),
	MAPGRID_ANIMATION_STEP(90, 28, 1, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};

// The sanctum's door opening, two metatiles wide.
Uint16 YallamSanctumDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(92, 30, 2, 2, 6),
	MAPGRID_ANIMATION_STEP(94, 30, 2, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};

// Each door's opening and the metatile it is drawn at, by its exit's number.
MAP_GRID_METATILE_ANIMATION YallamDoors[] = {
	{YallamDoorOpening, 98, 19},
	{YallamSecondDoorOpening, 92, 20},
	{YallamThirdDoorOpening, 84, 18},
	{YallamDoorOpening, 79, 7},
	{YallamThirdDoorOpening, 80, 16},
	{YallamThirdDoorOpening, 77, 17},
	{YallamSecondDoorOpening, 99, 8},
	{YallamSanctumDoorOpening, 90, 6},
};

// The village: its exits and doors, its people, the Cyclone spots and the log.
CUE YallamCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_INN_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scYallamEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_INN_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scYallamEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_SHOPS_EAST, CONDITION_ALWAYS, SCRIPT_REF(scYallamEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_INN_WEST, CONDITION_ALWAYS, SCRIPT_REF(scYallamEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_SHOPS_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scYallamEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_SHOPS_WEST, CONDITION_ALWAYS, SCRIPT_REF(scYallamEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_INN_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scYallamEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), YALLAM_TO_MIKASALLA_ROOMS_SANCTUM, CONDITION_ALWAYS, SCRIPT_REF(scYallamEnterDoor)},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, YALLAM_TO_YALLAM_CAVE_EAST, CONDITION_ALWAYS, YALLAM_TO_YALLAM_CAVE_EAST},
	{CUE_EXIT, YALLAM_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS, YALLAM_TO_WORLD_MAP_SOUTH},
	{CUE_EXIT, YALLAM_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS, YALLAM_TO_WORLD_MAP_NORTH},
	{CUE_EXIT, YALLAM_TO_SONG_GAME, CONDITION_ALWAYS, YALLAM_TO_SONG_GAME},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_SOUTH, YALLAM_TO_YALLAM_CAVE_WEST, CONDITION_ALWAYS, YALLAM_TO_YALLAM_CAVE_WEST},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_OVER_CAVE_HOLE, CONDITION_ALWAYS, SCRIPT_REF(scYallamUncoverCaveHole)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_BELOW_SHOPS, CONDITION_ALWAYS, SCRIPT_REF(scYallamScatterWeedsBelowShops)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_OVER_ANTIDOTE, CONDITION_ALWAYS, SCRIPT_REF(scYallamUncoverAntidote)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_IN_CORNER, CONDITION_ALWAYS, SCRIPT_REF(scYallamScatterWeedsInCorner)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_OVER_NUT, CONDITION_ALWAYS, SCRIPT_REF(scYallamUncoverNut)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_BY_SANCTUM, CONDITION_ALWAYS, SCRIPT_REF(scYallamScatterWeedsBySanctum)},
	{CUE_TALK, ACTOR_VILLAGER_FEW_VISITORS, CONDITION_ALWAYS, MSGNO(MsgYallamDoesnT)},
	{CUE_TALK, ACTOR_VILLAGER_YEPP_DIED, CONDITION_ALWAYS, MSGNO(MsgOurMasterBoatsman)},
	{CUE_TALK, ACTOR_VILLAGER_IF_YEPP_LIVED, CONDITION_ALWAYS, MSGNO(MsgIfYeppWere)},
	{CUE_TALK, ACTOR_VILLAGER_PEARL_BED, CONDITION_ALWAYS, MSGNO(MsgYeppFoundThis)},
	{CUE_TALK, ACTOR_VILLAGER_PEARL_DIVERS, CONDITION_ALWAYS, MSGNO(MsgThereUsedTo)},
	{CUE_TALK, ACTOR_VILLAGER_LOST_YEPP, CONDITION_ALWAYS, MSGNO(MsgWhenWeLost)},
	{CUE_TALK, ACTOR_BOATSMAN_TAUGHT_BY_YEPP, CONDITION_ALWAYS, MSGNO(MsgYeppWasThe)},
	{CUE_TALK, ACTOR_BOATSMAN_TAUGHT_LITTLE, CONDITION_ALWAYS, MSGNO(MsgYeppToldMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEW_VISITORS, CONDITION_ALWAYS, MSGNO(MsgYeppWasTheOne)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_YEPP_DIED, CONDITION_ALWAYS, MSGNO(MsgSingingWithYepp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_IF_YEPP_LIVED, CONDITION_ALWAYS, MSGNO(MsgThosePearlsYepp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PEARL_BED, CONDITION_ALWAYS, MSGNO(MsgYeppNeverTold)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PEARL_DIVERS, CONDITION_ALWAYS, MSGNO(MsgYeppWasAmazing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_LOST_YEPP, CONDITION_ALWAYS, MSGNO(MsgSunshineIsA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOATSMAN_TAUGHT_BY_YEPP, CONDITION_ALWAYS, MSGNO(MsgIVeSearched)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOATSMAN_TAUGHT_LITTLE, CONDITION_ALWAYS, MSGNO(MsgYeppAlwaysSang)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LOG_STUMP, FLAG_YALLAM_LOG_FALLEN, (Sint32)(scYallamNoOperation)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_LOG_STUMP, FLAG_YALLAM_LOG_FALLEN, SCRIPT_REF(scYallamForceLogDown)},
	{CUE_TABLE_END},
};

// The cave's exits.
CUE YallamCaveCues[] = {
	{CUE_EXIT, YALLAM_CAVE_TO_YALLAM_EAST, CONDITION_ALWAYS, YALLAM_CAVE_TO_YALLAM_EAST},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, YALLAM_CAVE_TO_YALLAM_WEST, CONDITION_ALWAYS, YALLAM_CAVE_TO_YALLAM_WEST},
	{CUE_TABLE_END},
};
// clang-format on

// The song game's exit and its children.
CUE YallamSongGameCues[] = {
	{CUE_EXIT, YALLAM_EXIT_SONG_GAME, CONDITION_ALWAYS, YALLAM_EXIT_SONG_GAME},
	{TALK_CUE_FACING(FACING_EAST),
     ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     SCRIPT_REF(scYallamOfferYeppSong)},
	{TALK_CUE_FACING(FACING_SOUTHEAST),
     ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     SCRIPT_REF(scYallamOfferYeppSong)},
	{TALK_CUE_FACING(FACING_NORTHEAST),
     ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     SCRIPT_REF(scYallamOfferYeppSong)},
	{TALK_CUE_FACING(FACING_NORTH),
     ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     SCRIPT_REF(scYallamOfferYeppSong)},
	{TALK_CUE_FACING(FACING_SOUTH),
     ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     SCRIPT_REF(scYallamOfferYeppSong)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     MSGNO(MsgYeppSSongs)},
	{CUE_TALK, ACTOR_CIRCLES_CHILD, CONDITION_ALWAYS, SCRIPT_REF(scYallamTalkAboutSongCircles)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_CIRCLES_CHILD, CONDITION_ALWAYS,
     SCRIPT_REF(scYallamTalkAboutSongCircles)},
	{CUE_TALK, ACTOR_GAME_CHILD, CONDITION_ALWAYS, SCRIPT_REF(scYallamTalkAboutSongGame)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_GAME_CHILD, CONDITION_ALWAYS,
     SCRIPT_REF(scYallamTalkAboutSongGame)},
	{CUE_TALK, ACTOR_STORY_CHILD, CONDITION_ALWAYS, MSGNO(MsgYeppWroteAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_STORY_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgTheStoryIs)},
	{CUE_TABLE_END},
};
