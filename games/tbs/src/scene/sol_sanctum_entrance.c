/* The way into Sol Sanctum: the mountainside before its door, and the first
   rooms inside, where Isaac, Garet and Jenna follow Kraden on the morning the
   story begins. Two scenes share this overlay, and every entry point chooses by
   MainRoutineStateWork.next.

   Sol Entrance (SceneIdSolEntrance) is the paved terrace at the top of the
   path from Vale, with the sanctum's carved door in the cliff. Walking north
   into the door (metatiles (6, 6) to (8, 6)) slides it open and takes the party
   in (scSolSanctumEntranceOpenDoorAndEnter). Retreat does not work here. Until
   the eruption the path leads down to Vale; during it, to Vale with the
   villagers gathered (SceneIdValeEruption) until the party has left. After the
   eruption the ground rumbles now and then. Once Isaac and Garet are back from
   the mountain the door no longer opens: Isaac walks up to it and stops, and
   searching it finds it "tightly locked". Once the party has left Vale, a
   bearded man in a long robe stands in the path and warns everyone away.

   Sol Sanctum (SceneIdSolSanctumReliefs) is the run of rooms behind the door,
   in the order the party meets them:
   - The entrance hall, where Kraden welcomes them on the first visit
	 (scSolSanctumEntranceIntroduceSanctum).
   - Two rooms of stepping stones over water, where Isaac jumps one-metatile
	 gaps, joined by a corridor (SceneIdSolSanctumCorridor). In the north of
	 the second lie the chest with the first Small Jewel (metatile (17, 3)) and,
	 at metatile (30, 2), a minotaur relief with an empty eye socket. Setting
	 the jewel in it opens an arched doorway at the end of the west path,
	 between two lit sconces (metatiles (3, 1) to (5, 2)).
   - The statue hall, the hall of eleven statues on a checkered floor that the
	 first relief opens. Kraden searches it for the hidden passage Saturos and
	 Menardi spoke of (scSolSanctumEntranceKradenSuspectsHiddenPassage). Its
	 westmost and eastmost statues stand in front of doorways, metatiles (94, 7)
	 and (114, 7), and can be pushed or moved aside: the west one hides a small
	 room with a statue that will not budge, the east one the steps up to the
	 maze (SceneIdSolSanctumMaze) and its chest with the second Small Jewel.
   - The second relief room, at the maze's far end, where six statues stand in
	 alcoves across a channel of water. Kraden sees them out of reach
	 (scSolSanctumEntranceKradenSeesStatuesOutOfReach); Move shifts them. The
	 westmost hides the second minotaur relief, the next a doorway into a
	 winding passage that leads nowhere. Once a statue is moved, Isaac can jump
	 to its alcove from the stepping stone in the water before it, and push it
	 by hand. Setting the second jewel opens a way behind the eastmost statue,
	 on to the rooms of Sol and Luna (SceneIdSolSanctumLunaSolRooms).

   Each relief room has actor placement and cue tables of its own.
   scGetActorPlacements and _GetCues choose them by the entrance the party came
   in at, so a room's statues and their scripts exist only while the party is in
   it.

   A pushed statue stays where it was left for the rest of the visit. Each
   pushable statue's column is kept in two area flags, one for each side, which
   scSolSanctumEntranceRestoreInteriorState reads to put it back on entry.
   gmArrive clears them when the party enters another area, so the statues are
   back at their starts on the next visit.

   Searching a relief while its Small Jewel is held asks for the field menu as
   the search ends, so that the jewel can be used on it at once.

   The locked door is two changes in Sol Entrance's setup, once
   FLAG_VALE_RETURNED_FROM_MT_ALEPH is set. It makes the three
   METATILE_BEHAVIOR_SOL_ENTRANCE_DOOR metatiles before the door plain ground,
   and the three wall metatiles across the door
   METATILE_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR, copied from a spare row in the
   cliff at the map's top left, metatiles (0, 1) to (2, 1), which the player
   never reaches. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/pushed_actor.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE SolSanctumEntranceFallbackEntrances[];
extern SCENE_ENTRANCE SolEntranceEntrances[];
extern SCENE_ENTRANCE SolSanctumReliefsEntrances[];
extern Uint32		  SolSanctumEntranceExits[];
extern ACTOR_PLACE	  SolSanctumEntranceFallbackActorPlacements[];
extern ACTOR_PLACE	  SolSanctumReliefsActorPlacements[];
extern ACTOR_PLACE	  SolSanctumReliefsFirstReliefRoomActorPlacements[];
extern ACTOR_PLACE	  SolSanctumReliefsSecondReliefRoomActorPlacements[];
extern ACTOR_PLACE	  SolEntranceActorPlacements[];
extern CUE			  SolSanctumEntranceFallbackCues[];
extern CUE			  SolEntranceCues[];
extern CUE			  SolSanctumReliefsCues[];
extern CUE			  SolSanctumReliefsFirstReliefRoomCues[];
extern CUE			  SolSanctumReliefsSecondReliefRoomCues[];

/* In Sol Sanctum (SceneIdSolSanctumReliefs), Kraden follows the party; Garet
   and Jenna, placed before him, take their own party slots. */
enum { ACTOR_KRADEN = ACTOR_FIRST_PLACED };

/* At Sol Entrance (SceneIdSolEntrance), the one placed actor is a man with
   long brown hair and a beard in a long robe, who stands in the path once the
   party has left Vale and warns everyone away while Mt. Aleph is active; read
   with Mind Read, he is all alone in this dangerous place and asks how the
   Great Healer can treat him like this. */
enum { ACTOR_WARNING_MAN = ACTOR_FIRST_PLACED };

/* The statues, placed after Kraden. They share one sprite, which Kraden calls
   statues ("More statues..."). Outside the two rooms below, Sol Sanctum places
   one, in the statue nook off the statue hall
   (SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_STATUE_HALL). */
enum { ACTOR_STATUE = ACTOR_FIRST_PLACED + 1 };

/* The statue hall's eleven statues: the two that can be pushed, then nine that
   cannot. */
enum {
	ACTOR_WEST_STATUE = ACTOR_FIRST_PLACED + 1,
	ACTOR_EAST_STATUE,
	/* The first of the nine. */
	ACTOR_FIRST_FIXED_STATUE
};

/* The second relief room's row of six, each of which can be pushed a metatile
   east or west; alike, they are lettered west to east, each starting 64 pixels
   east of the one before (scSolSanctumEntranceRestoreInteriorState). */
enum {
	ACTOR_ROW_STATUE_A = ACTOR_FIRST_PLACED + 1,
	ACTOR_ROW_STATUE_B,
	ACTOR_ROW_STATUE_C,
	ACTOR_ROW_STATUE_D,
	ACTOR_ROW_STATUE_E,
	ACTOR_ROW_STATUE_F
};

/* Area flags: gmArrive clears 0x300 to 0x3ff when the party enters another
   area, and other areas use the numbers for their own. */

/* The column each statue that can be pushed stands in, east or west of its
   start.

   They are the statue hall's west and east statues, then the row of six in the
   second relief room. Both clear means the start. */
#define FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST  FLAG_AREA_TEMP_00
#define FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST  FLAG_AREA_TEMP_01
#define FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST  FLAG_AREA_TEMP_02
#define FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST  FLAG_AREA_TEMP_03
/* The second relief room's row of six statues, each two flags as above. */
#define FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_EAST FLAG_AREA_TEMP_10
#define FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_WEST FLAG_AREA_TEMP_11
#define FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_EAST FLAG_AREA_TEMP_12
#define FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_WEST FLAG_AREA_TEMP_13
#define FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_EAST FLAG_AREA_TEMP_14
#define FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_WEST FLAG_AREA_TEMP_15
#define FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_EAST FLAG_AREA_TEMP_16
#define FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_WEST FLAG_AREA_TEMP_17
#define FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_EAST FLAG_AREA_TEMP_18
#define FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_WEST FLAG_AREA_TEMP_19
#define FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_EAST FLAG_AREA_TEMP_1A
#define FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_WEST FLAG_AREA_TEMP_1B

/* Scene flags, cleared on every ordinary arrival:
   - FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM: set on entering the second relief
	 room; the Move rows of the statue hall apply only while it is clear,
	 which it always is there: only the second room's entrances set it,
	 every ordinary entrance into the first room clears it, and a return
	 after a battle comes back by the entrance the party last came in
	 by, so the condition never stops a Move. */

/* The metatile behaviors of the metatiles the Sol Sanctum entrance's coord, bg
   and item cues answer.

   Behaviors 1 to 16 in both scenes are the exits, in the exit table's order. */
typedef enum SOL_ENTRANCE_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_SOL_ENTRANCE_DOOR = 2,
	METATILE_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR = 10
} SOL_ENTRANCE_METATILE_BEHAVIOR;

/* The metatile behaviors of the metatiles the relief rooms' coord, bg and item
   cues answer. */
typedef enum SOL_SANCTUM_RELIEFS_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_RELIEF = 20,
	METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF = 21,
	/* The statue hall's statues, pushed from any side. */
	METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE = 22,
	/* The statue hall's statues, pushed east only. */
	METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_EAST = 23,
	/* The second relief room's row of statues. */
	METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_ROW_STATUE = 24,
	/* The statue hall's statues, pushed west only. */
	METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_WEST = 25,
	METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_CHEST = 100
} SOL_SANCTUM_RELIEFS_METATILE_BEHAVIOR;

/* An item cue's subject bit that skips the `<actor> uses <item>.` message, so
   the script shows its own (fdUseItem). */
#define SOL_SANCTUM_ITEM_OWN_MESSAGE 0x400

static void scSolSanctumEntranceApplyEntryState(void);
static void scSolSanctumEntranceRestoreInteriorState(void);
static void scSolSanctumEntranceIntroduceSanctum(void);
static void scSolSanctumEntranceKradenSuspectsHiddenPassage(void);
static void scSolSanctumEntranceKradenSeesStatuesOutOfReach(void);
static void scSolSanctumEntranceRestoreRowStatueMetatiles(Sint32 placeActors);
static void scSolSanctumEntranceUpdateEruptionRumbles(void);

Sint32 SolSanctumEntranceRumbleTimer BSS_DATA;

/* Returns Sol Entrance's entrances (SceneIdSolEntrance) or Sol */
/* Sanctum's (SceneIdSolSanctumReliefs). */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_SOL_ENTRANCE) {
		return SolEntranceEntrances;
	}
	if(sceneId == SCENE_SOL_SANCTUM_RELIEFS) {
		return SolSanctumReliefsEntrances;
	}
	return SolSanctumEntranceFallbackEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SolSanctumEntranceExits;
}

/* Selects the actors for the scene and, in Sol Sanctum, the room. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	ACTOR_PLACE *table;
	Sint32		 firstStatueRoomEntrance =
		SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM;

	if(MainRoutineStateWork.next == SCENE_SOL_SANCTUM_RELIEFS) {
		if(MainRoutineStateWork.entrance >= firstStatueRoomEntrance) {
			if(MainRoutineStateWork.entrance >
				SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM) {
				if(MainRoutineStateWork.entrance <=
					SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
					return SolSanctumReliefsSecondReliefRoomActorPlacements;
				}
			} else {
				return SolSanctumReliefsFirstReliefRoomActorPlacements;
			}
		}
		table = SolSanctumReliefsActorPlacements;
		fdAssignViewFlags(table);
		return table;
	}
	if(MainRoutineStateWork.next == SCENE_SOL_ENTRANCE) {
		return SolEntranceActorPlacements;
	}
	return SolSanctumEntranceFallbackActorPlacements;
}

/* Selects the scripts for the scene and room, by the same entrances as
   scGetActorPlacements. */
const CUE *scGetCues(void)
{
	Sint32 firstStatueRoomEntrance =
		SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM;

	if(MainRoutineStateWork.next == SCENE_SOL_ENTRANCE) {
		return SolEntranceCues;
	}
	if(MainRoutineStateWork.next == SCENE_SOL_SANCTUM_RELIEFS) {
		if(MainRoutineStateWork.entrance >= firstStatueRoomEntrance) {
			if(MainRoutineStateWork.entrance >
				SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM) {
				if(MainRoutineStateWork.entrance <=
					SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
					return SolSanctumReliefsSecondReliefRoomCues;
				}
			} else {
				return SolSanctumReliefsFirstReliefRoomCues;
			}
		}
		return SolSanctumReliefsCues;
	}
	return SolSanctumEntranceFallbackCues;
}

/* Isaac walks north into Sol Sanctum's door at the top of the terrace
   (metatiles (6, 6) to (8, 6)); with a boom the stone door slides open, and he
   walks in as the screen fades.

   The door is drawn open in three steps ten ticks apart, each copying the next
   picture of it from a spare part of the map. */
static void scSolSanctumEntranceOpenDoorAndEnter(void)
{
	fdScriptStart();
	/* The door opens in three steps. */
	sySoundPlay(SE_BOOM);
	fdMapGridCopyMetatilesTo(16, 28, 21, 3, 3, 2);
	sySynch(10);
	fdMapGridCopyMetatilesTo(16, 30, 21, 3, 3, 2);
	sySynch(10);
	fdMapGridCopyMetatilesTo(16, 32, 21, 3, 3, 2);
	sySynch(10);
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
	fdScriptWait(10);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS);
	fdScriptEnd();
}

/* Isaac searches the minotaur relief at the north of the second room of
   stepping stones (metatile (30, 2)): "The right eye is sparkling, but there's
   a hole where the left eye should be." Once the Small Jewel is set, both eyes
   sparkle.

   While Isaac holds the jewel, the field menu opens as the message ends, so
   that he can use it on the relief at once. */
static void scSolSanctumEntranceInspectFirstMinotaurRelief(void)
{
	fdScriptStart();

	if(gmFlagGet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
		/* "It's the minotaur relief, with both eyes sparkling brightly." */
		mnFieldMessageShow(MsgSolSanctumMinotaurReliefBothEyes, 1);
	} else {
		/* "It's a relief of a minotaur..." */
		mnFieldMessageShow(MsgSolSanctumMinotaurReliefOneEye, 1);
		if(gmFlagGet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
			FieldStateWork->fieldMenu = 1;
		}
	}

	fdScriptEnd();
}

/* The room shakes as stone shifts, and the leader looks round, startled, and
   jumps. */
static inline void scSolSanctumEntranceReactToOpeningPassage(void)
{
	fdMapGridSetShake(1, 1, 1);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdMapGridSetShake(2, 2, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 20);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 40);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptWait(40);
}

/* Isaac uses the Small Jewel on the first minotaur relief (metatile (30, 2))
   and sets it into the empty socket. "You hear the sound of shifting stone":
   the room shakes, Isaac looks round, startled, and an arched doorway opens at
   the end of the room's west path, between two lit sconces.

   The jewel is used up. The doorway is metatiles (3, 1) to (5, 2), copied in
   with their heights from a spare part of the map; scInit redraws it on later
   visits. */
static void scSolSanctumEntranceSetFirstSmallJewel(void)
{
	Sint32 msg;

	if(gmFlagGet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
		if(fdScriptGetFlag(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) == 0) {
			fdScriptStart();
			fdEndBusyAndReveal();
			/* The eye sparkles, then the wall opens. */
			sySoundPlay(SE_GEM_SET);
			fdMapGridCopyMetatilesTo(0, 70, 30, 42, 1, 1);
			fdMapGridCenterOnFollowedPoint();
			fdScriptWait(40);
			msg = MsgSolSanctumSetSmallGem;
			/* "Isaac set the small gem into the empty socket." */
			mnFieldMessageShow(msg, 1);
			fdScriptWait(20);
			sySoundPlay(SE_STONE_RUMBLE);
			fdMapGridCopyMetatilesTo(0, 29, 3, 1, 3, 2);
			fdMapGridCopyMetatileAttributes(0, 29, 3, 2, 3, 1);
			fdMapGridCopyMetatilesTo(1, 109, 4, 81, 1, 1);
			fdMapGridCenterOnFollowedPoint();
			scSolSanctumEntranceReactToOpeningPassage();
			/* "You hear the sound of shifting stone." */
			mnFieldMessageShow(msg + 1, 1);
			gmFlagSet(FLAG_ITEM_USED_UP);
			gmFlagSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET);
			fdScriptEnd();
		}
	}
}

/* Isaac searches the second minotaur relief, uncovered behind the westmost of
   the six statues across the water (metatile (100, 31)). It too has one eye
   sparkling and a hole for the other, until the second Small Jewel is set. */
static void scSolSanctumEntranceInspectSecondMinotaurRelief(void)
{
	FIELD_STATE *state;

	fdScriptStart();

	if(gmFlagGet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) != 0) {
		/* "It's the minotaur relief, with both eyes sparkling brightly." */
		mnFieldMessageShow(MsgSolSanctumMinotaurReliefBothEyes, 1);
	} else if(gmFlagGet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0) {
		state = FieldStateWork;
		/* "It's a relief of a minotaur..." */
		mnFieldMessageShow(MsgSolSanctumMinotaurReliefOneEye, 1);
		state->fieldMenu = 1;
	} else {
		/* "It's a relief of a minotaur..." */
		mnFieldMessageShow(MsgSolSanctumMinotaurReliefOneEye, 1);
	}

	fdScriptEnd();
}

/* Isaac uses the second Small Jewel on the second minotaur relief (metatile
   (100, 31)) and sets it. Stone shifts, the room shakes, and a doorway opens in
   the wall behind the eastmost of the six statues (metatiles (120, 30) to (120,
   31)), the way on to the rooms of Sol and Luna.

   The eastmost statue still stands in front of the doorway until it is moved
   aside. */
static void scSolSanctumEntranceSetSecondSmallJewel(void)
{
	Sint32 msg;

	if(gmFlagGet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0 &&
		fdScriptGetFlag(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) == 0) {
		fdScriptStart();
		fdEndBusyAndReveal();
		/* The eye sparkles, then the doorway opens. */
		sySoundPlay(SE_GEM_SET);
		fdMapGridCopyMetatilesTo(0, 71, 100, 71, 1, 1);
		fdMapGridCenterOnFollowedPoint();
		fdScriptWait(40);
		msg = MsgSolSanctumSetSmallGem;
		/* "Isaac set the small gem into the empty socket." */
		mnFieldMessageShow(msg, 1);
		fdScriptWait(20);
		sySoundPlay(SE_STONE_RUMBLE);
		fdMapGridCopyMetatilesTo(122, 20, 120, 30, 1, 2);
		fdMapGridCopyMetatileAttributes(122, 20, 1, 2, 120, 30);
		fdMapGridCenterOnFollowedPoint();
		scSolSanctumEntranceReactToOpeningPassage();
		/* "You hear the sound of shifting stone." */
		mnFieldMessageShow(msg + 1, 1);
		gmFlagSet(FLAG_ITEM_USED_UP);
		gmFlagSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET);
		fdScriptEnd();
	}
}

/* Notes which side of its starting column a moved statue now stands on, so it
   stays there for the rest of the visit: flagBase (its _AT_EAST flag) for
   rightColumn, the flag after it (_AT_WEST) for leftColumn, neither back at its
   start. */
static inline void scSolSanctumEntranceRecordStatueColumn(
	Sint32 column, Sint32 flagBase, Sint32 leftColumn, Sint32 rightColumn)
{
	gmFlagClear(flagBase);
	gmFlagClear(flagBase + 1);
	if(column == leftColumn) {
		gmFlagSet(flagBase + 1);
	} else if(column == rightColumn) {
		gmFlagSet(flagBase);
	}
}

/* Isaac shifts the westmost statue of the checkered statue hall a metatile east
   or west, with Move or by pushing it, and uncovers the dark doorway behind it
   (metatile (94, 7)) to a small room whose statue will not budge. Nothing more
   happens on screen.

   The Move row runs this after Move has shifted the statue, and
   scSolSanctumEntrancePushStatue after a push. The statue starts in column 94;
   columns 93 and 95 set its two flags. */
static void scSolSanctumEntranceWestStatueMoved(void)
{
	ACTOR *statue = fdActorGet(ACTOR_WEST_STATUE);
	Sint32 column;

	if(statue == NULL) {
		return;
	}
	column = FIELD_METATILE(statue->x.fixed);
	scSolSanctumEntranceRecordStatueColumn(
		column, FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST, 93, 95);
}

/* Isaac shifts the eastmost statue of the checkered statue hall a metatile east
   or west, with Move or by pushing it, and uncovers the doorway behind it
   (metatile (114, 7)) to the steps up to the maze. As for the west statue, the
   game notes which side it now stands on.

   The statue starts in column 114; columns 113 and 115 set its two flags. */
static void scSolSanctumEntranceEastStatueMoved(void)
{
	ACTOR *statue = fdActorGet(ACTOR_EAST_STATUE);
	Sint32 column;

	if(statue == NULL) {
		return;
	}
	column = FIELD_METATILE(statue->x.fixed);
	fdScriptClearFlag(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
	fdScriptClearFlag(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
	if(column == 115) {
		gmFlagSet(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
	} else if(column == 113) {
		gmFlagSet(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
	}
}

/* Isaac uses Move on the westmost of the six statues across the water (metatile
   (100, 32)) and shifts it a metatile east or west, uncovering the second
   minotaur relief in the wall behind it.

   scSolSanctumEntrancePushStatue runs this too, after a push by hand.
   scSolSanctumEntranceRestoreRowStatueMetatiles then lets Isaac jump into the
   metatile the statue left, from the stepping stone before it. */
static void scSolSanctumEntranceRowStatueAMoved(void)
{
	ACTOR *statue = fdActorGet(ACTOR_ROW_STATUE_A);
	Sint32 column;

	if(statue == NULL) {
		return;
	}
	column = FIELD_METATILE(statue->x.fixed);
	scSolSanctumEntranceRecordStatueColumn(
		column, FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_EAST, 99, 101);
	scSolSanctumEntranceRestoreRowStatueMetatiles(0);
}

/* Isaac uses Move on the second of the six statues from the west (metatile
   (104, 32)) and shifts it aside, uncovering a dark doorway behind it into a
   winding passage that leads nowhere. */
static void scSolSanctumEntranceRowStatueBMoved(void)
{
	ACTOR *statue = fdActorGet(ACTOR_ROW_STATUE_B);
	Sint32 column;

	if(statue == NULL) {
		return;
	}
	column = FIELD_METATILE(statue->x.fixed);
	scSolSanctumEntranceRecordStatueColumn(
		column, FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_EAST, 103, 105);
	scSolSanctumEntranceRestoreRowStatueMetatiles(0);
}

/* Isaac uses Move on the third of the six statues from the west (metatile (108,
   32)) and shifts it aside; only plain wall stands behind it. */
static void scSolSanctumEntranceRowStatueCMoved(void)
{
	ACTOR *statue = fdActorGet(ACTOR_ROW_STATUE_C);
	Sint32 column;

	if(statue == NULL) {
		return;
	}
	column = FIELD_METATILE(statue->x.fixed);
	scSolSanctumEntranceRecordStatueColumn(
		column, FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_EAST, 107, 109);
	scSolSanctumEntranceRestoreRowStatueMetatiles(0);
}

/* Isaac uses Move on the fourth of the six statues from the west (metatile
   (112, 32)) and shifts it aside; only plain wall stands behind it. */
static void scSolSanctumEntranceRowStatueDMoved(void)
{
	ACTOR *statue = fdActorGet(ACTOR_ROW_STATUE_D);
	Sint32 column;

	if(statue == NULL) {
		return;
	}
	column = FIELD_METATILE(statue->x.fixed);
	scSolSanctumEntranceRecordStatueColumn(
		column, FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_EAST, 111, 113);
	scSolSanctumEntranceRestoreRowStatueMetatiles(0);
}

/* Isaac uses Move on the fifth of the six statues from the west (metatile (116,
   32)) and shifts it aside; only plain wall stands behind it. */
static void scSolSanctumEntranceRowStatueEMoved(void)
{
	ACTOR *statue = fdActorGet(ACTOR_ROW_STATUE_E);
	Sint32 column;

	if(statue == NULL) {
		return;
	}
	column = FIELD_METATILE(statue->x.fixed);
	scSolSanctumEntranceRecordStatueColumn(
		column, FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_EAST, 115, 117);
	scSolSanctumEntranceRestoreRowStatueMetatiles(0);
}

/* Isaac uses Move on the eastmost of the six statues (metatile (120, 32)) and
   shifts it aside. Behind it is plain wall until the second Small Jewel is set
   in the relief, then the doorway on to the rooms of Sol and Luna. */
static void scSolSanctumEntranceRowStatueFMoved(void)
{
	ACTOR *statue = fdActorGet(ACTOR_ROW_STATUE_F);
	Sint32 column;

	if(statue == NULL)
		return;
	column = FIELD_METATILE(statue->x.fixed);
	scSolSanctumEntranceRecordStatueColumn(
		column, FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_EAST, 119, 121);
	scSolSanctumEntranceRestoreRowStatueMetatiles(0);
}

#include "shared/actor_find_at_metatile.c"

/* The party has entered the second relief room, which turns off Move on the
   statue hall's statues. */
#define FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM FLAG_SCENE_TEMP_34

/* Isaac walks into one of the statues that can be pushed and shoves it one
   metatile, following behind it, if nothing is in the way: the westmost and
   eastmost statues of the checkered hall (metatiles (94, 8) and (114, 8)),
   which a dotted line on the floor marks, or a statue of the row across the
   water, once he has jumped into its alcove.

   Each statue's metatiles and the metatiles beside it carry the push
   behaviours; the statue pushed is whichever stands one step ahead of the
   leader. After the push the room's Moved functions note where every statue now
   stands. */
static void scSolSanctumEntrancePushStatue(void)
{
	ACTOR	 *leader;
	ACTOR	 *statue;
	Uint32	  direction;
	FIELD_POS target;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	direction = FACING_INDEX(leader->facing);
	statue = scActorFindAtMetatile(
		(leader->x.part[0].integer +
			(FIXEDtoINT(PushedActorDirectionSteps[direction]))) >>
			4,
		(leader->z.part[0].integer +
			(Sint16)PushedActorDirectionSteps[direction]) >>
			4);
	if(statue != NULL) {
		statue->layer = 2;
		fdActorStepFrom(&target, statue, PushedActorDirectionSteps[direction]);
		/* Move only after the statue's proposed position passes collision. */
		if(syFieldActorCheckMovementCollision(statue, &target) <= 0) {
			fdActorSelectAnimation(leader, ANIM_PUSH);
			sySynch(15);
			sySoundPlay(SE_PUSH);
			statue->speedLimit = toFIXED(1) / 5;
			statue->acceleration = toFIXED(1) / 5;
			syFieldActorSetMoveTargetFX(statue, target.x, target.y, target.z);
			leader->speedLimit = toFIXED(1) / 5;
			leader->acceleration = toFIXED(1) / 5;
			syFieldActorSetMoveTargetFX(leader, target.x, target.y, target.z);
			fdActorWaitForMove(statue);
			statue->x.fixed = target.x;
			statue->z.fixed = target.z;
			statue->velocityX = 0;
			statue->velocityZ = 0;
			fdActorSelectAnimation(leader, ANIM_STAND);
			switch(MainRoutineStateWork.entrance) {
			case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM:
			case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STATUE_NOOK:
			case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM:
				scSolSanctumEntranceWestStatueMoved();
				scSolSanctumEntranceEastStatueMoved();
				break;
			case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
			case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
			case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
				scSolSanctumEntranceRowStatueAMoved();
				scSolSanctumEntranceRowStatueBMoved();
				scSolSanctumEntranceRowStatueCMoved();
				scSolSanctumEntranceRowStatueDMoved();
				scSolSanctumEntranceRowStatueEMoved();
				scSolSanctumEntranceRowStatueFMoved();
				break;
			}
		}
	}
}

/* Sets up Sol Entrance or Sol Sanctum. */
Sint32 scInit(void)
{
	Sint32 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_SOL_ENTRANCE) {
		scSolSanctumEntranceApplyEntryState();
	} else if(sceneId == SCENE_SOL_SANCTUM_RELIEFS) {
		scSolSanctumEntranceRestoreInteriorState();
	}
	return 0;
}

/* Sets up Sol Entrance (SceneIdSolEntrance). */
static void scSolSanctumEntranceApplyEntryState(void)
{
	/* Retreat does not work here. */
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
		SolSanctumEntranceRumbleTimer = 0;
		sySetTickEvent(
			scSolSanctumEntranceUpdateEruptionRumbles, EVENT_PASS_TICK);
	}
	if(gmFlagGet(FLAG_VALE_RETURNED_FROM_MT_ALEPH) != 0) {
		fdMapGridCopyMetatileAttributes(5, 6, 1, 1, 6, 6);
		fdMapGridCopyMetatileAttributes(5, 6, 1, 1, 7, 6);
		fdMapGridCopyMetatileAttributes(5, 6, 1, 1, 8, 6);
		fdMapGridCopyMetatileAttributes(0, 1, 3, 1, 6, 5);
	}
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
		fdActorSetPos(ACTOR_WARNING_MAN, 120, 232);
		fdMapGridCopyMetatileAttributes(2, 10, 1, 1, 6, 14);
		fdMapGridCopyMetatileAttributes(2, 10, 1, 1, 7, 14);
		fdMapGridCopyMetatileAttributes(2, 10, 1, 1, 8, 14);
	}
}

/* Sets up Sol Sanctum (SceneIdSolSanctumReliefs). */
static void scSolSanctumEntranceRestoreInteriorState(void)
{
	Sint32 entrance;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
		fdQueueMusic(SE_RUMBLE);
		fdMapGridSetShake(1, 1, 1);
		fdEffectInitSparkles();
	}

	entrance = MainRoutineStateWork.entrance;
	switch(entrance) {
	case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST:
	case SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_STATUE_HALL:
		/* With the first small jewel set, an arched doorway stands open in the
		   north wall of this room, between its two lit sconces. */
		if(gmFlagGet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
			fdMapGridCopyMetatilesTo(1, 109, 4, 81, 1, 1);
			fdMapGridCopyMetatilesTo(0, 70, 30, 42, 1, 1);
			fdMapGridCopyMetatilesTo(0, 29, 3, 1, 3, 2);
			fdMapGridCopyMetatileAttributes(0, 29, 3, 2, 3, 1);
			fdMapGridCenterOnFollowedPoint();
		}
		break;

	case SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_STATUE_HALL:
		fdActorSetSpriteFlags(fdActorGet(ACTOR_STATUE), 0);
		break;

	case SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE:
		MainRoutineStateWork.retreatScene = SCENE_SOL_SANCTUM_RELIEFS;
		MainRoutineStateWork.retreatEntrance =
			SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE;
		if(gmFlagGet(FLAG_SOL_SANCTUM_ENTERED) == 0)
			scSolSanctumEntranceIntroduceSanctum();
		break;

	case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM:
	case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STATUE_NOOK:
	case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM:
		fdActorSetSpriteFlags(fdActorGet(ACTOR_WEST_STATUE), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_EAST_STATUE), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE + 1), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE + 2), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE + 3), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE + 4), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE + 5), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE + 6), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE + 7), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_FIXED_STATUE + 8), 0);
		if(gmFlagGet(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE) == 0)
			scSolSanctumEntranceKradenSuspectsHiddenPassage();
		if(gmFlagGet(FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST) != 0)
			fdActorSetPos(ACTOR_WEST_STATUE, 1496, 136);
		else if(gmFlagGet(FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST) != 0)
			fdActorSetPos(ACTOR_WEST_STATUE, 1528, 136);
		if(gmFlagGet(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST) != 0)
			fdActorSetPos(ACTOR_EAST_STATUE, 1816, 136);
		else if(gmFlagGet(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST) != 0)
			fdActorSetPos(ACTOR_EAST_STATUE, 1848, 136);
		break;

	case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
	case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
	case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
		fdActorSetSpriteFlags(fdActorGet(ACTOR_ROW_STATUE_A), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_ROW_STATUE_B), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_ROW_STATUE_C), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_ROW_STATUE_D), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_ROW_STATUE_E), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_ROW_STATUE_F), 0);
		if(gmFlagGet(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN) == 0)
			scSolSanctumEntranceKradenSeesStatuesOutOfReach();
		scSolSanctumEntranceRestoreRowStatueMetatiles(1);
		gmFlagSet(FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM);
		if(gmFlagGet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) != 0) {
			fdMapGridCopyMetatilesTo(0, 71, 100, 71, 1, 1);
			fdMapGridCopyMetatilesTo(122, 20, 120, 30, 1, 2);
			fdMapGridCopyMetatileAttributes(122, 20, 1, 2, 120, 30);
			fdMapGridCenterOnFollowedPoint();
		}
		break;
	}
}

/* The first time the party comes in through the door, Kraden steps up in the
   entrance hall, looks round and asks if this is their first time here; the
   answer gets "Take a good look at it" or "I doubt that...". */
static void scSolSanctumEntranceIntroduceSanctum(void)
{
	fdScriptStart();
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
	fdScriptWait(4);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(1224, CAMERA_KEEP, 136, 1);
	scPlaceAtLeader(ACTOR_KRADEN);
	scPlaceAtLeader(ACTOR_JENNA);
	scPlaceAtLeader(ACTOR_GARET);
	fdActorSetSpeed(ACTOR_KRADEN, 0.6);
	fdActorSetSpeed(ACTOR_JENNA, 0.6);
	fdActorSetSpeed(ACTOR_GARET, 0.6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_GARET, -16, 0);
	fdActorSetDestinationOffset(ACTOR_JENNA, 16, 0);
	fdActorSetDestinationOffset(ACTOR_KRADEN, 0, -32);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STILL);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STILL);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
	fdActorJump(ACTOR_KRADEN, 4, 20);
	fdScriptSetMessageId(MsgSolSanctumKradenFirstTimeAtSol);
	/* "This is the entrance to Sol Sanctum. This is your first time..." Yes:
	   "It's an important relic of our past. Take a good look at it." No: "I
	   doubt that... Few ever get the chance to see this sanctum..." */
	fdScriptAskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdScriptWait(20);
	fdCameraMoveTo(1224, CAMERA_KEEP, 148, 1);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	scWalkTowardLeader(ACTOR_GARET);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	scWalkTowardLeader(ACTOR_JENNA);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	scWalkTowardLeader(ACTOR_KRADEN);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	gmFlagSet(FLAG_SOL_SANCTUM_ENTERED);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	fdScriptEnd();
}

/* The first time the party enters the checkered statue hall, Kraden looks round
   both sides of it, then tells the others that Saturos and Menardi spoke of a
   hidden passage leading deeper in, and that a sanctum built in honour of the
   sun ought to show it somewhere. "Let's see if we can find it!" Garet asks
   whether they should help him, and whatever Isaac answers, the three agree to
   stay together and help. */
static void scSolSanctumEntranceKradenSuspectsHiddenPassage(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL)
		fdActorSetPosFX(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL)
		fdActorSetPosFX(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL)
		fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);

	fdActorSetSpeed(ACTOR_KRADEN, 0.6);
	fdActorSetSpeed(ACTOR_JENNA, 0.6);
	fdActorSetSpeed(ACTOR_GARET, 0.6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_GARET, -16, 0);
	fdActorSetDestinationOffset(ACTOR_JENNA, 16, 0);
	fdActorSetDestinationOffset(ACTOR_KRADEN, 0, -16);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STILL);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STILL);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 30);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_KRADEN, 0, -16);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdScriptWait(6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_KRADEN, 0, -32);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);

	/* Survey both sides of the room before returning to Kraden. */
	fdCameraSetSpeed(2);
	fdCameraMoveTo(1585, CAMERA_KEEP, 150, 1);
	fdCameraWaitForMove();
	fdScriptWait(10);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(1621, CAMERA_KEEP, 100, 1);
	fdCameraWaitForMove();
	fdCameraMoveTo(1718, CAMERA_KEEP, 100, 1);
	fdCameraWaitForMove();
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdCameraMoveTo(1752, CAMERA_KEEP, 150, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(1668, CAMERA_KEEP, 256, 1);
	fdCameraWaitForMove();
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);

	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdScriptSetMessageId(MsgSolSanctumWrongKraden);
	/* "What's wrong, Kraden?" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 2);
	/* Kraden: "Those two said there was a hidden passage leading deeper into
	   the sanctum." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(20);
	/* Kraden: the sanctum always looked strange to him, not as he imagined.
	   "I've come here many times, and I've always thought it..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "I had always imagined Sol Sanctum would look different, somehow." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, 5, 0);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	/* Jenna: "What do you mean?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* Kraden: a sanctum built in honour of the sun ought to show it somewhere.
	   "Sol Sanctum was built in honor of the sun. There ought to be..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
	/* "I was always puzzled why there wasn't." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	fdScriptWait(10);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	/* Kraden: "If there is a hidden passage, then maybe it leads to..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
	fdActorJump(ACTOR_KRADEN, 2, 20);

	/* Kraden hops: "Let's see if we can find it!" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
	/* Garet: "I think he's serious! Should we help him, Isaac?" Yes: Garet,
	   "But it's too dangerous for us to split up. We should stay together." No:
	   Jenna, "You can't seriously want to turn back after we've come this
	   far." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdScriptSetMessageId(MsgSolSanctumDangerousSplitStay);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
	} else {
		/* Isaac winces, and Garet nods and agrees to help Kraden. */
		fdScriptSetMessageId(MsgSolSanctumCantSeriouslyWant);
		fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 20);
		fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 60);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 30);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptWait(10);
		/* "Let's help Kraden. We should stick together, after all." */
		fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
	}

	fdActorSetSpeed(ACTOR_KRADEN, 0.6);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_KRADEN, 0, 48);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdScriptWait(6);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL)
		fdActorSetDestination(
			ACTOR_GARET, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL)
		fdActorSetDestination(
			ACTOR_JENNA, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL)
		fdActorSetDestination(
			ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);

	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	gmFlagSet(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	fdScriptEnd();
}

/* The first time the party comes into the second relief room, the view pans
   along the six statues across the water and Kraden says, "More statues... It
   doesn't look like we can reach them," then, "Perhaps Psynergy is the
   key..." */
static void scSolSanctumEntranceKradenSeesStatuesOutOfReach(void)
{
	fdScriptStart();
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	scPlaceAtLeader(ACTOR_KRADEN);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_KRADEN, 24, -10);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdScriptWait(6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(1672, CAMERA_KEEP, 524, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(1880, CAMERA_KEEP, 524, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdCameraSetSpeed(3.2);
	fdCameraMoveTo(1769, CAMERA_KEEP, 548, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 30);
	fdScriptSetMessageId(MsgSolSanctumMoreStatuesOutOfReach);
	/* "More statues... Hmm... It doesn't look like we can reach..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	/* Kraden: "Perhaps Psynergy is the key..." */
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
	scWalkIntoLeader(ACTOR_KRADEN);
	gmFlagSet(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN);
	fdScriptEnd();
}

/* Sets the ground under the six statues across the water. Each statue's start
   metatile is a wall while it stands there. Once it has been moved, the
   metatile is one Isaac can jump to (METATILE_BEHAVIOR_JUMP_FORWARD) from the
   stepping stone in the water two rows south, so he can get into the alcove.
   With placeActors, the moved statues are also put back where they were
   left. */
static void scSolSanctumEntranceRestoreRowStatueMetatiles(Sint32 placeActors)
{
	fdMapGridCopyMetatileAttributes(122, 20, 1, 1, 100, 32);
	fdMapGridCopyMetatileAttributes(122, 20, 1, 1, 104, 32);
	fdMapGridCopyMetatileAttributes(122, 20, 1, 1, 108, 32);
	fdMapGridCopyMetatileAttributes(122, 20, 1, 1, 112, 32);
	fdMapGridCopyMetatileAttributes(122, 20, 1, 1, 116, 32);
	fdMapGridCopyMetatileAttributes(122, 20, 1, 1, 120, 32);
	if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_WEST) != 0) {
		fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 100, 32);
		if(placeActors != 0) {
			fdActorSetPos(ACTOR_ROW_STATUE_A, 1592, 520);
		}
	} else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_EAST) != 0) {
			fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 100, 32);
			if(placeActors != 0) {
				fdActorSetPos(ACTOR_ROW_STATUE_A, 1624, 520);
			}
		}
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_WEST) != 0) {
		fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 104, 32);
		if(placeActors != 0) {
			fdActorSetPos(ACTOR_ROW_STATUE_B, 1656, 520);
		}
	} else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_EAST) != 0) {
			fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 104, 32);
			if(placeActors != 0) {
				fdActorSetPos(ACTOR_ROW_STATUE_B, 1688, 520);
			}
		}
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_WEST) != 0) {
		fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 108, 32);
		if(placeActors != 0) {
			fdActorSetPos(ACTOR_ROW_STATUE_C, 1720, 520);
		}
	} else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_EAST) != 0) {
			fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 108, 32);
			if(placeActors != 0) {
				fdActorSetPos(ACTOR_ROW_STATUE_C, 1752, 520);
			}
		}
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_WEST) != 0) {
		fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 112, 32);
		if(placeActors != 0) {
			fdActorSetPos(ACTOR_ROW_STATUE_D, 1784, 520);
		}
	} else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_EAST) != 0) {
			fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 112, 32);
			if(placeActors != 0) {
				fdActorSetPos(ACTOR_ROW_STATUE_D, 1816, 520);
			}
		}
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_WEST) != 0) {
		fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 116, 32);
		if(placeActors != 0) {
			fdActorSetPos(ACTOR_ROW_STATUE_E, 1848, 520);
		}
	} else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_EAST) != 0) {
			fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 116, 32);
			if(placeActors != 0) {
				fdActorSetPos(ACTOR_ROW_STATUE_E, 1880, 520);
			}
		}
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_WEST) != 0) {
		fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 120, 32);
		if(placeActors != 0) {
			fdActorSetPos(ACTOR_ROW_STATUE_F, 1912, 520);
		}
	} else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_EAST) != 0) {
			fdMapGridCopyMetatileAttributes(121, 20, 1, 1, 120, 32);
			if(placeActors != 0) {
				fdActorSetPos(ACTOR_ROW_STATUE_F, 1944, 520);
			}
		}
	}
}

/* Isaac searches Sol Sanctum's door once he and Garet are back from the
   mountain: "The door is tightly locked."

   The search answers the wall metatiles across the door, (6, 5) to (8, 5),
   which Sol Entrance's setup gives this behaviour only then. */
static void scSolSanctumEntranceDoorTightlyLocked(void)
{
	fdScriptStart();
	/* "The door is tightly locked." */
	mnFieldMessageShow(MsgFieldDoorTightlyLocked, 1);
	fdScriptEnd();
}

/* After the eruption the terrace before the door rumbles and shakes every few
   seconds, at random, each shake dying away.

   A shake lasts 80 ticks and starts to fade after 40. Between shakes, each
   tick has one chance in 120 of starting the next. */
static void scSolSanctumEntranceUpdateEruptionRumbles(void)
{
	if(SolSanctumEntranceRumbleTimer != 0) {
		if(--SolSanctumEntranceRumbleTimer == 40) {
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		}
	} else {
		if(syRandomBelow(120) == 0) {
			sySoundPlay(SE_ITEM_BREAK);
			fdMapGridSetShake(1, 2, 1);
			SolSanctumEntranceRumbleTimer = 80;
		}
	}
}

SCENE_ENTRANCE SolSanctumEntranceFallbackEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE SolEntranceEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{SOL_ENTRANCE_FROM_VALE, CONDITION_ALWAYS, 80, 0, 288, FACING_NORTH, 0, -1,
		-1, -1, -1, 0},
	{SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS, CONDITION_ALWAYS, 120, 0, 112,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE SolSanctumReliefsEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST, CONDITION_ALWAYS, 280,
		0, 472, FACING_NORTH, 0, 5, 0, 555, 510, 0},
	{SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_STATUE_HALL, CONDITION_ALWAYS, 72, 0,
		56, FACING_SOUTH, 0, 5, 0, 555, 510, 0},
	{SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_STATUE_HALL, CONDITION_ALWAYS, 744, 0,
		152, FACING_NORTH, 0, 620, 0, 860, 195, 0},
	{SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_FROM_ENTRANCE_ROOM,
		CONDITION_ALWAYS, 696, 0, 568, FACING_NORTH, 0, 575, 265, 815, 610, 0},
	{SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST, CONDITION_ALWAYS, 696,
		0, 328, FACING_SOUTH, 0, 575, 265, 815, 610, 0},
	{SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_STATUE_HALL, CONDITION_ALWAYS, 984, 0,
		152, FACING_NORTH, 0, 860, 0, 1100, 195, 0},
	{SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_WEST, CONDITION_ALWAYS, 984, 0,
		56, FACING_SOUTH, 0, 860, 0, 1100, 195, 0},
	{SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 1224, 0, 184,
		FACING_NORTH, 0, 1100, 20, 1340, 220, 0},
	{SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM,
		CONDITION_ALWAYS, 1224, 0, 104, FACING_SOUTH, 0, 1100, 20, 1340, 220,
		0},
	{SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_FROM_SECOND_RELIEF_ROOM,
		CONDITION_ALWAYS, 1320, 0, 456, FACING_NORTH, 0, 905, 285, 1365, 490,
		0},
	{SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM, CONDITION_ALWAYS, 1672, 0,
		280, FACING_NORTH, 0, 1455, 0, 1890, 310, 0},
	{SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STATUE_NOOK, CONDITION_ALWAYS, 1512,
		0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310, 0},
	{SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM, CONDITION_ALWAYS, 1832, 0,
		136, FACING_SOUTH, 0, 1455, 0, 1890, 310, 0},
	{SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST, CONDITION_ALWAYS, 1768, 0,
		568, FACING_NORTH, 0, 1540, 440, 1995, 600, 0},
	{SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE,
		CONDITION_ALWAYS, 1672, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600,
		0},
	{SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS, CONDITION_ALWAYS,
		1928, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600, 0},
	{SCENE_TABLE_END},
};

/* Sol Entrance (SceneIdSolEntrance), then Sol Sanctum
   (SceneIdSolSanctumReliefs). Sol Entrance's way down leads to SceneIdVale
   until the eruption, then to Vale during the eruption (SceneIdValeEruption)
   until the party has left Vale. */
Uint32 SolSanctumEntranceExits[] = {
	SCENE_EXITS(SolEntrance),
	SCENE_EXIT_UNLESS(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
		FLAG_SOL_SANCTUM_ERUPTED),
	SCENE_EXIT_UNLESS(SOL_ENTRANCE_TO_VALE, ValeEruption,
		VALE_ERUPTION_FROM_SOL_ENTRANCE, FLAG_VALE_PARTY_LEFT),
	SCENE_EXIT_UNLESS(
		SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS, SolSanctumReliefs,
		SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXITS(SolSanctumReliefs),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST,
		SolSanctumCorridor, SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_STATUE_HALL,
		SolSanctumReliefs, SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_STATUE_HALL,
		SolSanctumReliefs, SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STATUE_NOOK,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM,
		SolSanctumReliefs,
		SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST,
		SolSanctumCorridor, SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_STATUE_HALL,
		SolSanctumReliefs, SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST,
		SolSanctumMaze, SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_SOUTH,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE, SolEntrance,
		SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM,
		SolSanctumReliefs,
		SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_FROM_ENTRANCE_ROOM,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM,
		SolSanctumReliefs,
		SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_CHEST_ROOM,
		SolSanctumReliefs, SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_STATUE_HALL,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STATUE_NOOK,
		SolSanctumReliefs, SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_STATUE_HALL,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STEPS_ROOM,
		SolSanctumReliefs, SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_STATUE_HALL,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST,
		SolSanctumMaze, SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_NORTH,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE,
		SolSanctumReliefs,
		SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_FROM_SECOND_RELIEF_ROOM,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS,
		SolSanctumLunaSolRooms,
		SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE SolSanctumEntranceFallbackActorPlacements[] = {
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE SolSanctumReliefsActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		744, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE SolSanctumReliefsFirstReliefRoomActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1512, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1832, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1560, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1784, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1640, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1640, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1640, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1672, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1704, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1704, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1704, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE SolSanctumReliefsSecondReliefRoomActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1608, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1672, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1736, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1800, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1864, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1928, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE SolEntranceActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE SolSanctumEntranceFallbackCues[] = {
	{ CUE_TABLE_END },
};

CUE SolEntranceCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, SOL_ENTRANCE_TO_VALE },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_SOL_ENTRANCE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceOpenDoorAndEnter) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceDoorTightlyLocked) },
	{ CUE_TALK, ACTOR_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
	{ CUE_TABLE_END },
};

CUE SolSanctumReliefsCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_STATUE_HALL },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_STATUE_HALL },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_STATUE_HALL },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
	{ CUE_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_CHEST_ROOM },
	{ CUE_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STATUE_NOOK },
	{ CUE_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STEPS_ROOM },
	{ CUE_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
	{ CUE_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
	{ CUE_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_CHEST, FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND, FIND_ITEM(ITEM_SMALL_JEWEL) },
	{ USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_RELIEF, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceSetFirstSmallJewel) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_RELIEF, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceInspectFirstMinotaurRelief) },
	{ CUE_TABLE_END },
};

CUE SolSanctumReliefsFirstReliefRoomCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_STATUE_HALL },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_STATUE_HALL },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_STATUE_HALL },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
	{ CUE_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_CHEST_ROOM },
	{ CUE_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STATUE_NOOK },
	{ CUE_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STEPS_ROOM },
	{ CUE_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
	{ CUE_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
	{ CUE_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_STATUE, FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM, SCRIPT_REF(scSolSanctumEntranceWestStatueMoved) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_STATUE, FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM, SCRIPT_REF(scSolSanctumEntranceEastStatueMoved) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntrancePushStatue) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_EAST, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntrancePushStatue) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_WEST, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntrancePushStatue) },
	{ CUE_TABLE_END },
};

CUE SolSanctumReliefsSecondReliefRoomCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_STATUE_HALL },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_STATUE_HALL },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_STATUE_HALL },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
	{ CUE_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_CHEST_ROOM },
	{ CUE_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STATUE_NOOK },
	{ CUE_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STEPS_ROOM },
	{ CUE_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
	{ CUE_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
	{ CUE_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_A, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceRowStatueAMoved) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_B, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceRowStatueBMoved) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_C, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceRowStatueCMoved) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_D, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceRowStatueDMoved) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_E, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceRowStatueEMoved) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_F, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceRowStatueFMoved) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_ROW_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntrancePushStatue) },
	{ USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceSetSecondSmallJewel) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumEntranceInspectSecondMinotaurRelief) },
	{ CUE_TABLE_END },
};
/* clang-format on */

#include "shared/pushed_actor_direction_steps.c"
