/* Imil Falls Cave, the little cave behind Imil's frozen falls, and Fuchin Falls
   Cave, the grotto behind the waterfall at Fuchin Temple where Master Nyunpa
   sends the party for its test.

   Imil Falls Cave (`SceneIdImilFallsCave`) is reached by sliding across Imil's
   frozen river to the cave mouth at the foot of the falls. Inside, Isaac walks
   up a strip of ice to Fever, a Mars Djinni, which joins the party without a
   fight. Retreat does not work in the cave, and the doorway at the bottom of
   the ice leads back out to Imil.

   Fuchin Falls Cave is Nyunpa's test: he tells the party how to pass through
   Mogall Forest only once they have come back from it with the Orb of Force,
   and back at the temple he names its power Ki (fuchin_temple.c). The cave is
   six rooms, crossed on golden logs that float on the cave's water. Isaac steps
   onto a log and keeps walking the way it can go, and the log floats with him
   on it to the far end of its run, glowing orange as it goes. From some of a
   log's stops he jumps across a gap of water to a stump or to another log. The
   logs are back where they started whenever the party comes into the cave
   again.

   In the order the player meets them:

   - The first room (`SceneIdFuchinFallsCaveEntrance`), behind the falls. On
	 the south shore a tablet reads "The dragon's flame illuminates the path of
	 truth in darkness." The crosswise log on the right floats north to the
	 ledge with a chest, which is a Mimic. The one on the left floats north to
	 the ledge below the room's north half, a pool of stumps that Isaac hops
	 across to the doorway in its north-east corner. An upright log at the
	 pool's west bank ferries Isaac to the east bank and back.
   - The room of the Dragon's Eye (`SceneIdFuchinFallsCaveDragonsEye`), with
	 an upright log, two crosswise logs in a long channel and a small pool in
	 its south-west corner. Stairs down from its west ledge lead to a round
	 chamber where the Dragon's Eye lies on a little table.
   - The dragon statue's room (`SceneIdFuchinFallsCaveDragonStatue`), up the
	 stairs at that room's north-west. Its northern half is dark and blue, its
	 floor a field of spikes with an eyeless stone dragon in the north-east
	 corner, and a tablet in its southern half reads "Rays of light give birth
	 to shadows, revealing the way." Using the Dragon's Eye on the statue makes
	 the dragon breathe fire, lights the room and shows the shadow of a hidden
	 path across the spikes. The path leads to two doorways in the north wall.
   - The room of the Arctic Blade (`SceneIdFuchinFallsCaveArcticBlade`), down
	 the stairs at the west of the dragon statue's room. In its north half a
	 crosswise log and an upright log cross a pool to the chest of the Arctic
	 Blade. In its south half an upright log carries Isaac east across a pool
	 to the way back to the first room's west bank.
   - Zephyr's room (`SceneIdFuchinFallsCaveZephyr`), through the doorway at
	 the first room's east bank. Zephyr, a Jupiter Djinni, waits on a ledge on
	 its east side and fights when spoken to. Its doorway in the north leads to
	 the south end of the Dragon's Eye room.
   - The room of Ki (`SceneIdFuchinFallsCaveOrbOfForce`), through the
	 doorways the hidden path leads to. A tablet reads "The secret of Ki shall
	 be revealed to the disciples of truth.", and the chest beside it holds the
	 Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), which ends the test.

   Each log's place is an area temp flag named beside the includes, and each
   room's rebuild function puts its logs where their flags say. It also rewrites
   the metatiles of every log's run, copied from spare metatiles that each
   room's map keeps beside it: the metatiles under a log get the step cue that
   moves it, and the empty ends of its run become walls of water. The logs'
   moves are scActorFloatLogWithLeader, which carries the log and the leader
   together. The darkness of the dragon statue's room is a palette tint, and the
   hidden path is a run of metatiles copied over the spikes when the eye is set
   (FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/facing.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/ground_mask.h"
#include "field/map_grid.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern Sint16		  IceKeyHeadings[];
extern Sint16		  ImilFuchinFallsSlideHeadings[];
extern ACTOR		 *ImilFuchinFallsDragonsEye;
extern SCENE_ENTRANCE ImilFuchinFallsNoEntrances[];
extern SCENE_ENTRANCE ImilFuchinFallsImilFallsCaveEntrances[];
extern SCENE_ENTRANCE ImilFuchinFallsEntranceEntrances[];
extern SCENE_ENTRANCE ImilFuchinFallsZephyrRoomEntrances[];
extern SCENE_ENTRANCE ImilFuchinFallsDragonsEyeRoomEntrances[];
extern SCENE_ENTRANCE ImilFuchinFallsArcticBladeRoomEntrances[];
extern SCENE_ENTRANCE ImilFuchinFallsKiRoomEntrances[];
extern SCENE_ENTRANCE ImilFuchinFallsDragonRoomEntrances[];
extern Uint32		  ImilFuchinFallsExits[];
extern ACTOR_PLACE	  ImilFuchinFallsNoActorPlacements[];
extern ACTOR_PLACE	  ImilFuchinFallsImilFallsCaveActorPlacements[];
extern ACTOR_PLACE	  ImilFuchinFallsEntranceActorPlacements[];
extern ACTOR_PLACE	  ImilFuchinFallsZephyrRoomActorPlacements[];
extern ACTOR_PLACE	  ImilFuchinFallsDragonsEyeRoomActorPlacements[];
extern ACTOR_PLACE	  ImilFuchinFallsArcticBladeRoomActorPlacements[];
extern ACTOR_PLACE	  ImilFuchinFallsDragonRoomActorPlacements[];
extern CUE			  ImilFuchinFallsNoCues[];
extern CUE			  ImilFuchinFallsImilFallsCaveCues[];
extern CUE			  ImilFuchinFallsEntranceCues[];
extern CUE			  ImilFuchinFallsZephyrRoomCues[];
extern CUE			  ImilFuchinFallsDragonsEyeRoomCues[];
extern CUE			  ImilFuchinFallsArcticBladeRoomCues[];
extern CUE			  ImilFuchinFallsKiRoomCues[];
extern CUE			  ImilFuchinFallsDragonRoomCues[];
extern Sint32		  ImilFuchinFallsShortDustScript[];
extern Sint32		  ImilFuchinFallsMediumDustScript[];
extern Sint32		  ImilFuchinFallsLongDustScript[];

/* The placed actors, numbering each scene's placement rows from
   ACTOR_FIRST_PLACED: Fever in Imil Falls Cave, the floating logs of Fuchin
   Falls Cave's first room, Zephyr's room, the Dragon's Eye room and the Arctic
   Blade's room, followed in Zephyr's room by Zephyr, and the one sprite the
   dragon statue's room places. */
typedef enum IMIL_FUCHIN_FALLS_ACTOR_ID {
	/* A Mars Djinni, at the top of the ice in Imil Falls Cave. */
	ACTOR_FEVER = ACTOR_FIRST_PLACED,
	/* The floating logs: three in the first room, Zephyr's room and the Arctic
	   Blade's room, four in the Dragon's Eye room. The metatile behaviours
	   below say which log is which in each room. They are numbered by their
	   placement rows, not named for each room, because one function serves the
	   same row in every room: the step cues of all four run
	   scFuchinFallsCaveSettleFirstLog for their first log. */
	ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_LOG,
	ACTOR_THIRD_LOG,
	ACTOR_FOURTH_LOG,
	/* Zephyr, a Jupiter Djinni, after the three logs of its room. */
	ACTOR_ZEPHYR = ACTOR_FIRST_PLACED + 3,
	/* A crate in the dragon statue's room, beside the statue at metatile (19,
	   8), which the player never sees: on entry scImilFuchinFallsPrepareRoom
	   stills it at floor height and draws it hidden, as
	   scFuchinFallsCaveCreateHiddenActor draws the two in the Dragon's Eye
	   room. */
	ACTOR_INVISIBLE_DRAGON_ROOM_CRATE = ACTOR_FIRST_PLACED
} IMIL_FUCHIN_FALLS_ACTOR_ID;

/* The metatiles the rooms' step and search cues answer, by the metatile
   behaviour their map gives them. A log's metatiles are the three under it at
   one of its stops; the rebuild functions copy them in where the log lies. */
enum {
	/* Imil Falls Cave: the strip of ice from the doorway north to Fever,
	   metatiles (16, 14) to (16, 18). */
	METATILE_BEHAVIOR_IMIL_FALLS_CAVE_ICE = 10,

	/* First room: the tablet on the south shore, metatile (15, 35). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_TABLET = 1,
	/* First room: the upright log at the pool's west bank, metatiles (8, 30) to
	   (8, 32). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_WEST = 11,
	/* First room: the upright log at the pool's east bank, metatiles (22, 30)
	   to (22, 32). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST = 12,
	/* First room: the left crosswise log below the ledge on the north side,
	   metatiles (11, 29) to (13, 29). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH = 13,
	/* First room: the left crosswise log at the south shore, metatiles (11, 33)
	   to (13, 33). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_SOUTH = 14,
	/* First room: the right crosswise log below the ledge with the chest,
	   metatiles (17, 29) to (19, 29). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH = 15,
	/* First room: the right crosswise log at the south shore, metatiles (17,
	   33) to (19, 33). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_SOUTH = 16,

	/* Zephyr's room: the upright log where it starts, at the west end of the
	   pool, metatiles (13, 19) to (13, 21). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_START = 11,
	/* Zephyr's room: the upright log at the east end, below Zephyr's ledge,
	   metatiles (21, 19) to (21, 21). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END = 12,
	/* Zephyr's room: the crosswise log at the foot of the sandy path down from
	   the spiked floor, metatiles (14, 17) to (16, 17). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_NORTH = 13,
	/* Zephyr's room: the crosswise log at the pool's south shore, metatiles
	   (14, 21) to (16, 21). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH = 14,
	/* Zephyr's room: the crosswise log at the north end of the long channel,
	   beside the spiked floor, metatiles (18, 8) to (20, 8). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH = 15,
	/* Zephyr's room: the crosswise log at the south end of the long channel,
	   metatiles (18, 25) to (20, 25). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_SOUTH = 16,
	/* Zephyr's room: the upright log pushed against the west wall, below the
	   stump, metatiles (12, 19) to (12, 21). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END = 17,

	/* The Dragon's Eye room: the upright log at the pool's east edge, metatiles
	   (20, 24) to (20, 26). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_EAST_END = 10,
	/* The Dragon's Eye room: the upright log in the middle of the pool, below
	   the stump, metatiles (17, 24) to (17, 26). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE = 11,
	/* The Dragon's Eye room: the upright log at the head of the long channel,
	   metatiles (14, 24) to (14, 26). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END = 12,
	/* The Dragon's Eye room: the upright log in the small south-west pool, at
	   its west side, metatiles (13, 34) to (13, 36). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST = 13,
	/* The Dragon's Eye room: the same log at the pool's east side, metatiles
	   (15, 34) to (15, 36). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_EAST = 14,
	/* The Dragon's Eye room: the upper crosswise log at the foot of the
	   channel, row 30. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_SOUTH_END = 15,
	/* The upper crosswise log at the head of the channel, below the north
	   shore, row 22. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END = 16,
	/* The upper crosswise log one metatile down the channel, row 23. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP = 17,
	/* The upper crosswise log halfway down the channel, row 26. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE = 18,
	/* The upper crosswise log at row 27, a stop no move gives it. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP = 19,
	/* The lower crosswise log at the foot of the channel, row 31. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_SOUTH_END = 20,
	/* The lower crosswise log at the head of the channel, row 23. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END = 21,
	/* The lower crosswise log at row 24. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP = 22,
	/* The lower crosswise log at row 27, beside the stump below the west
	   ledge. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE = 23,
	/* The lower crosswise log at row 28, a stop no move gives it. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP = 24,
	/* The Dragon's Eye room: the stump in the pool below the north shore,
	   metatile (17, 22). The north shore above the channel, metatiles (14, 21)
	   to (16, 21), takes it too while the upper crosswise log lies at its upper
	   stop. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_STUMP = 25,
	/* The Dragon's Eye room's chamber: the little table, metatile (15, 9). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_TABLE = 30,

	/* The Arctic Blade's room: the crosswise log at the pool's north shore,
	   metatiles (19, 10) to (21, 10). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_NORTH = 11,
	/* The same log at the south shore, metatiles (19, 17) to (21, 17). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH = 12,
	/* The upright log at the pool's east shore, metatiles (22, 15) to (22,
	   17). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_EAST = 13,
	/* The same log at the west shore, below the way up to the chest, metatiles
	   (14, 15) to (14, 17). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST = 14,
	/* The south half's upright log at the west end of its pool, metatiles (7,
	   45) to (7, 47). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_WEST = 15,
	/* The same log at the pool's east end, metatiles (17, 45) to (17, 47). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST = 16,

	/* The room of Ki: the tablet beside the chest, metatile (16, 5). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_KI_ROOM_TABLET = 7,

	/* The dragon statue's room: the row where the middle passage from the south
	   opens into the dark half, metatiles (14, 17) to (16, 17). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_NORTH_OF_MIDDLE_PASSAGE = 15,
	/* The dragon statue's room: the top of the middle passage, metatiles (14,
	   18) to (16, 18). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_MIDDLE_PASSAGE = 16,
	/* The dragon statue's room: the tablet in the south half, metatile (15,
	   29). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_TABLET = 19,
	/* The dragon statue's room: the steps below the statue, metatiles (20, 14)
	   to (22, 14). */
	METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_STATUE = 20
};

static void scFuchinFallsCaveRebuildEntranceLogs(void);
static void scFuchinFallsCaveRebuildZephyrRoomLogs(void);
static void scFuchinFallsCaveRebuildDragonsEyeRoomLogs(void);
static void scFuchinFallsCaveRebuildArcticBladeRoomLogs(void);
static void scImilWalkTowardFever(void);
static void scImilFuchinFallsPrepareRoom(void);
static void scImilFuchinFallsRebuildLogRoom(void);
static void scHopLeaderBy(Sint32 dx, Sint32 dz);
static void scImilFuchinFallsPlaceDragonsEye(void);
static void scLeaderSlideOnIce(void);

/* Set while the floor's ground mask follows the leader
   (scFuchinFallsCaveGroundMaskFollowsLeader). */
Sint32 ImilFuchinFallsTrackLeader;

/* The fields of EFFECT_OPTIONS that the dragon's fire sets, up to its scales,
   passed to scImilFuchinFallsSpawnDust in their place. */
typedef struct CAVE_DUST_OPTIONS {
	Sint32 priority;
	Sint32 palette;
	Sint32 startScaleX;
	Sint32 startScaleY;
	Sint32 targetScaleX;
	Sint32 targetScaleY;
} CAVE_DUST_OPTIONS;

STATIC_ASSERT(offsetof(CAVE_DUST_OPTIONS, priority) ==
	offsetof(EFFECT_OPTIONS, priority));
STATIC_ASSERT(
	offsetof(CAVE_DUST_OPTIONS, palette) == offsetof(EFFECT_OPTIONS, palette));
STATIC_ASSERT(offsetof(CAVE_DUST_OPTIONS, startScaleX) ==
	offsetof(EFFECT_OPTIONS, startScaleX));
STATIC_ASSERT(offsetof(CAVE_DUST_OPTIONS, startScaleY) ==
	offsetof(EFFECT_OPTIONS, startScaleY));
STATIC_ASSERT(offsetof(CAVE_DUST_OPTIONS, targetScaleX) ==
	offsetof(EFFECT_OPTIONS, targetScaleX));
STATIC_ASSERT(offsetof(CAVE_DUST_OPTIONS, targetScaleY) ==
	offsetof(EFFECT_OPTIONS, targetScaleY));

/* Talking to Fever, the Mars Djinni at the top of the ice in Imil Falls Cave,
   makes it join the party without a fight. */
static void scImilFeverJoins(void)
{
	fdDjinnJoinParty(ACTOR_FEVER, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER));
}

/* Talking to Zephyr, the Jupiter Djinni on the ledge on the east side of its
   room in Fuchin Falls Cave, starts a battle with it. */
static void scFuchinFallsCaveFightZephyr(void)
{
	fdRequestDjinnBattle(ACTOR_ZEPHYR, DJINNI_ZEPHYR);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_IMIL_FALLS_CAVE) {
		return ImilFuchinFallsImilFallsCaveEntrances;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ENTRANCE) {
		return ImilFuchinFallsEntranceEntrances;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
		return ImilFuchinFallsZephyrRoomEntrances;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE) {
		return ImilFuchinFallsDragonsEyeRoomEntrances;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE) {
		return ImilFuchinFallsArcticBladeRoomEntrances;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ORB_OF_FORCE) {
		return ImilFuchinFallsKiRoomEntrances;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE) {
		return ImilFuchinFallsDragonRoomEntrances;
	}
	return ImilFuchinFallsNoEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

/* Returns the exit table every scene of the overlay shares. */
const Uint32 *scGetExits(void)
{
	return ImilFuchinFallsExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_IMIL_FALLS_CAVE) {
		return ImilFuchinFallsImilFallsCaveActorPlacements;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ENTRANCE) {
		return ImilFuchinFallsEntranceActorPlacements;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
		return ImilFuchinFallsZephyrRoomActorPlacements;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE) {
		return ImilFuchinFallsDragonsEyeRoomActorPlacements;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE) {
		return ImilFuchinFallsArcticBladeRoomActorPlacements;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE) {
		return ImilFuchinFallsDragonRoomActorPlacements;
	}
	return ImilFuchinFallsNoActorPlacements;
}

#include "shared/log_ride.c"

/* The first room's upright log lies at the pool's east bank. */
#define FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST			   FLAG_AREA_TEMP_01
/* The first room's left crosswise log lies below the ledge on the north
   side. */
#define FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH			   FLAG_AREA_TEMP_02
/* The first room's right crosswise log lies below the ledge with the chest. */
#define FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH			   FLAG_AREA_TEMP_03
/* Zephyr's room's upright log lies at the east end of the pool, below Zephyr's
   ledge. */
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END	   FLAG_AREA_TEMP_04
/* Zephyr's room's upright log lies against the west wall, a metatile west of
   where it starts, unless it has since been taken to the east end. */
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END	   FLAG_AREA_TEMP_05
/* Zephyr's room's crosswise log lies at the pool's south shore. */
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH		   FLAG_AREA_TEMP_06
/* The crosswise log in Zephyr's room's long channel lies at its north end. */
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH		   FLAG_AREA_TEMP_07
/* The Dragon's Eye room's upright log lies at the head of the long channel. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END  FLAG_AREA_TEMP_08
/* The Dragon's Eye room's upright log lies in the middle of the pool, below the
   stump, where a crosswise log stopped it. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE	   FLAG_AREA_TEMP_09
/* The upright log in the Dragon's Eye room's small south-west pool lies at the
   pool's west side. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST	   FLAG_AREA_TEMP_0A
/* The Dragon's Eye room's upper crosswise log lies at the head of the
   channel. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END FLAG_AREA_TEMP_0B
/* The upper crosswise log lies a metatile down the channel from its head. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP \
	FLAG_AREA_TEMP_0C
/* The upper crosswise log lies halfway down the channel. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE FLAG_AREA_TEMP_0D
/* The upper crosswise log lies a metatile below halfway. No move sets it. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP \
	FLAG_AREA_TEMP_0E
/* The Dragon's Eye room's lower crosswise log lies a metatile below the
   channel's head, as far up as it goes. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END \
	FLAG_AREA_TEMP_0F
/* The lower crosswise log lies two metatiles below the channel's head. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP \
	FLAG_AREA_TEMP_10
/* The lower crosswise log lies halfway down the channel, beside the stump. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE FLAG_AREA_TEMP_11
/* The lower crosswise log lies a metatile below halfway. No move sets it. */
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP \
	FLAG_AREA_TEMP_12
/* The Arctic Blade's room's crosswise log lies at the pool's south shore. */
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH FLAG_AREA_TEMP_13
/* The Arctic Blade's room's upright log lies at the pool's west shore. */
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST FLAG_AREA_TEMP_14
/* The upright log in the south half of the Arctic Blade's room lies at the
   pool's east side. */
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST	FLAG_AREA_TEMP_15

/* Ends a log's move by fading out the sound of the moving water, then puts its
   room's logs and their metatiles to rights. */
static inline void scFuchinFallsCaveFinishLogMove(void (*rebuildLogs)(void))
{
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	sySynch(2);
	rebuildLogs();
}

/* In the first room, Isaac steps onto the upright log at the pool's west bank
   (metatile (8, 31)) and walks on east, and the log carries him across the pool
   to the east bank, by the doorway to Zephyr's room. */
static void scFuchinFallsCaveEntranceMoveFirstLogEast(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 112, 0);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 112, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST);
	sySynch(2);
	scFuchinFallsCaveRebuildEntranceLogs();
}

/* In the first room, Isaac walks west on the upright log at the pool's east
   bank (metatile (22, 31)), and it carries him back across the pool to the west
   bank, by the doorway from the Arctic Blade's room. */
static void scFuchinFallsCaveEntranceMoveFirstLogWest(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, -112, 0);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, -112, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST);
	sySynch(2);
	scFuchinFallsCaveRebuildEntranceLogs();
}

/* In the first room, Isaac walks south on the left crosswise log below the
   ledge on the north side (metatile (12, 29)), and it floats him back to the
   south shore, west of the tablet. */
static void scFuchinFallsCaveEntranceMoveSecondLogSouth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_SECOND_LOG, 0, 64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH);
	sySynch(2);
	scFuchinFallsCaveRebuildEntranceLogs();
}

/* In the first room, Isaac steps onto the left crosswise log at the south shore
   (metatile (12, 33)) and walks on north, and it floats him to the ledge on the
   north side, where a passage leads up to the room's north half. */
static void scFuchinFallsCaveEntranceMoveSecondLogNorth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_SECOND_LOG, 0, -64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH);
	sySynch(2);
	scFuchinFallsCaveRebuildEntranceLogs();
}

/* In the first room, Isaac walks south on the right crosswise log below the
   ledge with the chest (metatile (18, 29)), and it floats him back to the south
   shore, east of the tablet. */
static void scFuchinFallsCaveEntranceMoveThirdLogSouth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH);
	sySynch(2);
	scFuchinFallsCaveRebuildEntranceLogs();
}

/* In the first room, Isaac steps onto the right crosswise log at the south
   shore (metatile (18, 33)) and walks on north, and it floats him to the ledge
   with the chest, which holds a Mimic. */
static void scFuchinFallsCaveEntranceMoveThirdLogNorth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH);
	sySynch(2);
	scFuchinFallsCaveRebuildEntranceLogs();
}

/* In Zephyr's room, Isaac walks east on the upright log where it starts, at the
   west end of the pool (metatile (13, 20)), and it carries him the length of
   the pool to below Zephyr's ledge.

   It goes only while the crosswise log is not at the south shore, where it
   would lie across the way. */
static void scFuchinFallsCaveZephyrRoomMoveFirstLogToEastEnd(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 144, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
	sySynch(2);
	scFuchinFallsCaveRebuildZephyrRoomLogs();
}

/* In Zephyr's room, Isaac walks west on the upright log below Zephyr's ledge
   (metatile (21, 20)), and it carries him back along the pool to the west wall,
   below the stump. */
static void scFuchinFallsCaveZephyrRoomMoveFirstLogToWestEnd(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, -144, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
	gmFlagSet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
	sySynch(2);
	scFuchinFallsCaveRebuildZephyrRoomLogs();
}

/* In Zephyr's room, Isaac walks west on the upright log where it starts
   (metatile (13, 20)), and it bumps the last little way west against the wall,
   right below the stump.

   From there Isaac can hop north onto the stump, and a hop west from the
   crosswise log at the south shore lands on the upright log. */
static void scFuchinFallsCaveZephyrRoomNudgeFirstLogWest(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, -14, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
	sySynch(2);
	scFuchinFallsCaveRebuildZephyrRoomLogs();
}

/* In Zephyr's room, Isaac walks east on the upright log against the west wall
   (metatile (12, 20)), and it carries him the length of the pool to below
   Zephyr's ledge.

   While the crosswise log lies at the south shore, across the way, the upright
   log only goes back the one metatile to where it started. */
static void scFuchinFallsCaveZephyrRoomMoveFirstLogEast(void)
{
	sySoundPlay(SE_WATER_MOVE);

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH) != 0) {
		scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 16, 0);
		gmFlagClear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
	} else {
		scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 144, 0);

		gmFlagSet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
	}

	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildZephyrRoomLogs);
}

/* In Zephyr's room, Isaac steps onto the crosswise log at the foot of the sandy
   path down from the spiked floor (metatile (15, 17)) and walks on south, and
   it floats him across the pool to the south shore. */
static void scFuchinFallsCaveZephyrRoomMoveSecondLogSouth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_SECOND_LOG, 0, 64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH);
	sySynch(2);
	scFuchinFallsCaveRebuildZephyrRoomLogs();
}

/* In Zephyr's room, Isaac walks north on the crosswise log at the pool's south
   shore (metatile (15, 21)), and it floats him back to the foot of the sandy
   path down from the spiked floor. */
static void scFuchinFallsCaveZephyrRoomMoveSecondLogNorth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_SECOND_LOG, 0, -64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH);
	sySynch(2);
	scFuchinFallsCaveRebuildZephyrRoomLogs();
}

/* In Zephyr's room, Isaac steps onto the crosswise log at the north end of the
   long channel, beside the spiked floor (metatile (19, 8)), and walks on south,
   and it floats him all the way down the channel, past Zephyr's ledge, to its
   south end. */
static void scFuchinFallsCaveZephyrRoomMoveThirdLogSouth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 144);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 128);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH);
	sySynch(2);
	scFuchinFallsCaveRebuildZephyrRoomLogs();
}

/* In Zephyr's room, Isaac walks north on the crosswise log at the south end of
   the long channel (metatile (19, 25)), and it floats him all the way up the
   channel, past Zephyr's ledge, to the spiked floor below the north doorway. */
static void scFuchinFallsCaveZephyrRoomMoveThirdLogNorth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -96);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -96);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -80);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH);
	sySynch(2);
	scFuchinFallsCaveRebuildZephyrRoomLogs();
}

/* In the Dragon's Eye room, Isaac steps onto the upright log at the pool's east
   edge (metatile (20, 25)) and walks on west, and it carries him across the
   pool to the head of the long channel.

   When a crosswise log lies in the channel across its way (the lower one a
   metatile from the channel's head, or the upper one halfway down), it stops in
   the middle of the pool, below the stump, instead. */
static void scFuchinFallsCaveDragonsEyeRoomMoveFirstLogWest(void)
{
	sySoundPlay(SE_WATER_MOVE);

	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
			0 ||
		gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
		scActorFloatLogWithLeader(ACTOR_FIRST_LOG, -48, 0);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
	} else {
		scActorFloatLogWithLeader(ACTOR_FIRST_LOG, -96, 0);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
	}

	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* In the Dragon's Eye room, Isaac walks east on the upright log in the middle
   of the pool, below the stump (metatile (17, 25)), and it carries him back to
   the pool's east edge. */
static void scFuchinFallsCaveDragonsEyeRoomMoveFirstLogEastFromMiddle(void)
{
	sySoundPlay(SE_WATER_MOVE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 48, 0);
	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* In the Dragon's Eye room, Isaac walks west on the upright log in the middle
   of the pool (metatile (17, 25)), and it carries him on to the head of the
   long channel.

   While a crosswise log still lies across the channel there (the lower one a
   metatile from the head, or the upper one halfway down), nothing happens. */
static void scFuchinFallsCaveDragonsEyeRoomMoveFirstLogWestIfClear(void)
{
	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
		0) {
		return;
	}
	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
		return;
	}

	sySoundPlay(SE_WATER_MOVE);

	gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);

	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, -48, 0);
	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* In the Dragon's Eye room, Isaac walks east on the upright log at the head of
   the long channel (metatile (14, 25)), and it carries him back across the pool
   to its east edge. */
static void scFuchinFallsCaveDragonsEyeRoomMoveFirstLogEastFromWestEnd(void)
{
	sySoundPlay(SE_WATER_MOVE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 96, 0);
	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* In the Dragon's Eye room, Isaac steps onto the upright log in the small pool
   in the south-west corner (metatile (15, 35)) and walks on west, and it floats
   him across to the pool's west side, by the doorway to the first room. */
static void scFuchinFallsCaveDragonsEyeRoomMoveSecondLogWest(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_SECOND_LOG, -32, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST);
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* In the Dragon's Eye room, Isaac walks east on the upright log at the west
   side of the small south-west pool (metatile (13, 35)), and it floats him back
   across to the pool's east side. */
static void scFuchinFallsCaveDragonsEyeRoomMoveSecondLogEast(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_SECOND_LOG, 32, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST);
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* XXX (no evidence): whether the log was meant to stop below the upright log
   (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP, which no move
   sets); the move here ends between the two stops. In the Dragon's Eye room,
   Isaac steps onto the upper of the two crosswise logs at the foot of the long
   channel (metatile (15, 30)) and walks on north, and it floats him up the
   channel to its head, below the north shore.

   While the upright log lies at the head of the channel, the crosswise log
   stops against it, then is put back at its stop a metatile from the head,
   beyond the upright log, and Isaac is left standing on the water where it
   stopped. */
static void scFuchinFallsCaveDragonsEyeRoomMoveThirdLogNorthFromSouthEnd(void)
{
	sySoundPlay(SE_WATER_MOVE);
	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) != 0) {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -64);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	} else {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -128);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	}
	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* In the Dragon's Eye room, Isaac walks south on the upper crosswise log at the
   head of the long channel (metatile (15, 22)), and it floats him back down the
   channel as far as it can go.

   That is one metatile when the upright log or the lower crosswise log lies
   just below, halfway when the lower crosswise log lies halfway down, and
   otherwise all the way to the foot of the channel. */
static void scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromNorthEnd(void)
{
	sySoundPlay(SE_WATER_MOVE);

	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) != 0) {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 16);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	} else if(gmFlagGet(
				  FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
		0) {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 16);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	} else if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) !=
		0) {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 64);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	} else {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 128);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	}
	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* In the Dragon's Eye room, Isaac walks north on the upper crosswise log a
   metatile down the long channel (metatile (15, 23)), and it floats him the
   last metatile up to the head of the channel. */
static void scFuchinFallsCaveDragonsEyeRoomNudgeThirdLogNorth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -16);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* In the Dragon's Eye room, Isaac walks south on the upper crosswise log a
   metatile down the long channel (metatile (15, 23)), and it floats him back
   down: to halfway when the lower crosswise log lies halfway down, else all the
   way to the foot of the channel.

   It does not go while the upright log lies across the channel below it. With
   the lower crosswise log right below it, the log floats two metatiles and is
   put back where it was, and Isaac is left on the lower log. */
static void scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromUpperStop(void)
{
	sySoundPlay(SE_WATER_MOVE);
	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) != 0) {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 48);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);

		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	} else if(gmFlagGet(
				  FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
		0) {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 32);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	} else {
		scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 112);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	}
	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* In the Dragon's Eye room, Isaac walks north on the upper crosswise log
   halfway down the long channel (metatile (15, 26)), and it floats him up to
   the head of the channel. */
static void scFuchinFallsCaveDragonsEyeRoomMoveThirdLogNorthFromMiddle(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* In the Dragon's Eye room, Isaac walks south on the upper crosswise log
   halfway down the long channel (metatile (15, 26)), and it floats him back to
   the foot of the channel. It does not go while the lower crosswise log lies a
   metatile from the channel's head. */
static void scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromMiddle(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* Unused: walking north on the upper crosswise log at row 27 of the Dragon's
   Eye room's long channel (metatile (15, 27)) would float it to the head of the
   channel.

   The log never stops there: only
   FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP puts it there, and no
   move sets that flag. */
static void scFuchinFallsCaveDragonsEyeRoomMoveThirdLogNorthFromLowerStop(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, -80);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* Unused: walking south on the upper crosswise log at row 27 of the Dragon's
   Eye room's long channel (metatile (15, 27)) would float it back to the foot
   of the channel. The log never stops there, as
   scFuchinFallsCaveDragonsEyeRoomMoveThirdLogNorthFromLowerStop says.

   The same function as
   scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromMiddle under another
   name: the room's step table holds both. */
static void scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromLowerStop(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 0, 64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* In the Dragon's Eye room, Isaac steps onto the lower of the two crosswise
   logs at the foot of the long channel (metatile (15, 31)) and walks on north,
   and it floats him up the channel until something stops it.

   It stops a metatile below the channel's head, below the upper crosswise log
   there; a metatile lower when the upper log lies a metatile down; and halfway,
   beside the stump below the west ledge, when the upright log lies across the
   channel or the upper crosswise log lies halfway down. From there Isaac can
   hop west onto the stump and reach the stairs down to the Dragon's Eye. */
static void scFuchinFallsCaveDragonsEyeRoomMoveFourthLogNorth(void)
{
	sySoundPlay(SE_WATER_MOVE);

	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) != 0 ||
		gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
		scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, -64);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
	} else if(gmFlagGet(
				  FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) !=
		0) {
		scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, -112);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
	} else {
		scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, -128);
		gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
		gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
	}
	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* Forgets the stops of the Dragon's Eye room's lower crosswise log, which puts
   it back at the foot of the long channel. */
static void scFuchinFallsCaveClearFourthLogStops(void)
{
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
}

/* In the Dragon's Eye room, Isaac walks south on the lower crosswise log a
   metatile below the head of the long channel (metatile (15, 23)), and it
   floats him all the way back to the foot of the channel. It does not go while
   the upright log lies across the channel below it. */
static void scFuchinFallsCaveDragonsEyeRoomMoveFourthLogSouthFromNorthEnd(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, 128);
	scFuchinFallsCaveClearFourthLogStops();
	scFuchinFallsCaveFinishLogMove(scFuchinFallsCaveRebuildDragonsEyeRoomLogs);
}

/* In the Dragon's Eye room, Isaac walks north on the lower crosswise log two
   metatiles below the head of the long channel (metatile (15, 24)), and it
   floats him one metatile further up. */
static void scFuchinFallsCaveDragonsEyeRoomNudgeFourthLogNorth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, -16);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
	gmFlagClear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* In the Dragon's Eye room, Isaac walks south on the lower crosswise log two
   metatiles below the head of the long channel (metatile (15, 24)), and it
   floats him back to the foot of the channel. */
static void scFuchinFallsCaveDragonsEyeRoomMoveFourthLogSouthFromUpperStop(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, 112);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	scFuchinFallsCaveClearFourthLogStops();
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* In the Dragon's Eye room, Isaac walks south on the lower crosswise log
   halfway down the long channel, beside the stump (metatile (15, 27)), and it
   floats him back to the foot of the channel. */
static void scFuchinFallsCaveDragonsEyeRoomMoveFourthLogSouthFromMiddle(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, 64);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	scFuchinFallsCaveClearFourthLogStops();
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* XXX (no evidence): whether walking north was meant to float the log north, as
   the upper log's matching stop does; the log never stops here, so the game
   never shows which. */
/* Unused: walking north on the lower crosswise log at row 28 of the Dragon's
   Eye room's long channel (metatile (15, 28)) would float it south instead,
   five metatiles, past the foot of the channel.

   The log never stops there: only
   FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP puts it there, and
   no move sets that flag. With the flag set by hand, walking north on the log
   carries Isaac south past the channel's foot, and the rebuild puts the log
   back at the foot. */
static void
scFuchinFallsCaveDragonsEyeRoomUnusedMoveFourthLogSouthFromLowerStop(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, 80);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	scFuchinFallsCaveClearFourthLogStops();
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* Unused: walking south on the lower crosswise log at row 28 of the Dragon's
   Eye room's long channel (metatile (15, 28)) would float it back to the foot
   of the channel. The log never stops there, as
   scFuchinFallsCaveDragonsEyeRoomUnusedMoveFourthLogSouthFromLowerStop says. */
static void scFuchinFallsCaveDragonsEyeRoomUnusedMoveFourthLogSouthFromLastStop(
	void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FOURTH_LOG, 0, 48);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	scFuchinFallsCaveClearFourthLogStops();
	sySynch(2);
	scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
}

/* In the Arctic Blade's room, Isaac steps onto the crosswise log at the pool's
   north shore, below the stairs (metatile (20, 10)), and walks on south, and it
   floats him across the pool to the south shore, where the way leads on to the
   room's south half. */
static void scFuchinFallsCaveArcticBladeRoomMoveFirstLogSouth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 0, 112);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH);
	sySynch(2);
	scFuchinFallsCaveRebuildArcticBladeRoomLogs();
}

/* In the Arctic Blade's room, Isaac walks north on the crosswise log at the
   pool's south shore (metatile (20, 17)), and it floats him back across to the
   north shore, below the stairs. */
static void scFuchinFallsCaveArcticBladeRoomMoveFirstLogNorth(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_FIRST_LOG, 0, -112);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH);
	sySynch(2);
	scFuchinFallsCaveRebuildArcticBladeRoomLogs();
}

/* In the Arctic Blade's room, Isaac steps onto the upright log at the pool's
   east shore (metatile (22, 16)) and walks on west, and it carries him across
   the pool to the west shore, below the way up to the chest of the Arctic
   Blade.

   It goes only while the crosswise log is at the north shore; at the south
   shore it lies across the way. */
static void scFuchinFallsCaveArcticBladeRoomMoveSecondLogWest(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_SECOND_LOG, -128, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST);
	sySynch(2);
	scFuchinFallsCaveRebuildArcticBladeRoomLogs();
}

/* In the Arctic Blade's room, Isaac walks east on the upright log at the pool's
   west shore (metatile (14, 16)), and it carries him back across to the east
   shore. */
static void scFuchinFallsCaveArcticBladeRoomMoveSecondLogEast(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_SECOND_LOG, 128, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST);
	sySynch(2);
	scFuchinFallsCaveRebuildArcticBladeRoomLogs();
}

/* In the south half of the Arctic Blade's room, Isaac steps onto the upright
   log at the west end of the pool (metatile (7, 46)) and walks on east, and it
   carries him across to the east side, where the way leads on to the first
   room's west bank. */
static void scFuchinFallsCaveArcticBladeRoomMoveThirdLogEast(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, 160, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagSet(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST);
	sySynch(2);
	scFuchinFallsCaveRebuildArcticBladeRoomLogs();
}

/* In the south half of the Arctic Blade's room, Isaac walks west on the upright
   log at the pool's east side (metatile (17, 46)), and it carries him back
   across to the west end. */
static void scFuchinFallsCaveArcticBladeRoomMoveThirdLogWest(void)
{
	sySoundPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_THIRD_LOG, -160, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	gmFlagClear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST);
	sySynch(2);
	scFuchinFallsCaveRebuildArcticBladeRoomLogs();
}

/* Stepping onto a room's first log makes it dip under Isaac's step and then bob
   on the water: the upright log of the first room, of Zephyr's room and of the
   Dragon's Eye room, and the crosswise log in the north of the Arctic Blade's
   room. */
static void scFuchinFallsCaveSettleFirstLog(void)
{
	fdActorSetAnimation(ACTOR_FIRST_LOG, FLOATING_LOG_ANIMATION_FLOAT);
	fdActorSetAnimation(ACTOR_FIRST_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

/* Stepping onto a room's second log makes it dip under Isaac's step and then
   bob on the water: the left crosswise log of the first room, the crosswise log
   at the middle of Zephyr's room, the upright log in the Dragon's Eye room's
   small south-west pool, and the upright log in the north of the Arctic Blade's
   room. */
static void scFuchinFallsCaveSettleSecondLog(void)
{
	fdActorSetAnimation(ACTOR_SECOND_LOG, FLOATING_LOG_ANIMATION_FLOAT);
	fdActorSetAnimation(ACTOR_SECOND_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

/* Stepping onto a room's third log makes it dip under Isaac's step and then bob
   on the water: the right crosswise log of the first room, the crosswise log in
   Zephyr's room's long channel, the upper crosswise log in the Dragon's Eye
   room's channel, and the upright log in the south of the Arctic Blade's
   room. */
static void scFuchinFallsCaveSettleThirdLog(void)
{
	fdActorSetAnimation(ACTOR_THIRD_LOG, FLOATING_LOG_ANIMATION_FLOAT);
	fdActorSetAnimation(ACTOR_THIRD_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

/* Stepping onto the lower crosswise log in the Dragon's Eye room's long channel
   makes it dip under Isaac's step and then bob on the water. */
static void scFuchinFallsCaveSettleFourthLog(void)
{
	fdActorSetAnimation(ACTOR_FOURTH_LOG, FLOATING_LOG_ANIMATION_FLOAT);
	fdActorSetAnimation(ACTOR_FOURTH_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

/* Creates an actor that stands still and is drawn hidden, so that the player
   never sees it. */
static ACTOR *scFuchinFallsCaveCreateHiddenActor(
	Sint32 x, Sint32 y, Sint32 z, Sint32 resourceId)
{
	ACTOR *actor = syFieldActorCreate(resourceId, x, y, z);

	if(actor != NULL) {
		actor->sprite->image.priority = 1;
		actor->motionFlags = 0;
		fdActorSetSpriteFlags(actor, 0);
		fdActorSetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
		actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		return actor;
	}
	return NULL;
}

const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_IMIL_FALLS_CAVE) {
		return ImilFuchinFallsImilFallsCaveCues;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ENTRANCE) {
		return ImilFuchinFallsEntranceCues;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
		return ImilFuchinFallsZephyrRoomCues;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE) {
		return ImilFuchinFallsDragonsEyeRoomCues;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE) {
		return ImilFuchinFallsArcticBladeRoomCues;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_ORB_OF_FORCE) {
		return ImilFuchinFallsKiRoomCues;
	}
	if(sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE) {
		return ImilFuchinFallsDragonRoomCues;
	}
	return ImilFuchinFallsNoCues;
}

/* Isaac walks out of the doorway at the bottom of Imil Falls Cave (metatile
   (16, 19)) and comes out at the foot of Imil's frozen falls. */
static void scImilLeaveFallsCave(void)
{
	fdScriptStart();
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_SOUTH;
	sySoundPlay(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(IMIL_FALLS_CAVE_TO_IMIL);
}

/* Isaac steps onto the strip of ice between the doorway and Fever in Imil Falls
   Cave (metatiles (16, 14) to (16, 18)) and slides along it the way the pad
   points, until a wall stops him or he slides off the ice. Sliding south takes
   him right out of the doorway. */
static void scImilMoveAcrossIce(void)
{
	scLeaderSlideOnIce();
}

/* Puts a log at a map metatile, drawn under the leader's feet. */
static void scFuchinFallsCavePlaceLogAtMetatile(
	Sint32 id, Sint32 column, Sint32 row)
{
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		fdActorSetSpritePriority(id, 3);
		actor->layer = 2;
		actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		actor->x.fixed = toFIXED(column * 16) + toFIXED(8);
		actor->z.fixed = toFIXED(row * 16) + toFIXED(8);
	}
}

/* Puts the logs of Fuchin Falls Cave's first room where their flags say, and
   rewrites the metatiles of their runs to match from the spare copy of the pool
   kept below the room. */
static void scFuchinFallsCaveRebuildEntranceLogs(void)
{
	fdMapGridCopyMetatileAttributes(8, 42, 15, 5, 8, 29);

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 22, 31);
		fdMapGridCopyMetatileAttributes(9, 30, 1, 3, 8, 30);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 8, 31);
		fdMapGridCopyMetatileAttributes(9, 30, 1, 3, 22, 30);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_SECOND_LOG, 12, 29);
		fdMapGridCopyMetatileAttributes(14, 33, 3, 1, 11, 33);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_SECOND_LOG, 12, 33);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 11, 29);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 18, 29);
		fdMapGridCopyMetatileAttributes(14, 33, 3, 1, 17, 33);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 18, 33);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 17, 29);
	}
}

/* Puts the logs of Zephyr's room where their flags say, and rewrites the
   metatiles of their runs to match. */
static void scFuchinFallsCaveRebuildZephyrRoomLogs(void)
{
	fdMapGridCopyMetatileAttributes(0, 28, 10, 18, 12, 8);

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 21, 20);
		fdMapGridCopyMetatileAttributes(20, 19, 1, 3, 13, 19);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 13, 20);
		fdMapGridCopyMetatileAttributes(20, 19, 1, 3, 21, 19);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 12, 20);
		fdMapGridCopyMetatileAttributes(5, 19, 1, 3, 12, 19);
		fdMapGridCopyMetatileAttributes(20, 19, 1, 3, 13, 19);
		if(gmFlagGet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END) != 0) {
			scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 21, 20);
			fdMapGridCopyMetatileAttributes(20, 19, 1, 3, 13, 19);
			fdMapGridCopyMetatileAttributes(20, 19, 1, 3, 12, 19);
		}
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_SECOND_LOG, 15, 21);
		fdMapGridCopyMetatileAttributes(14, 18, 3, 1, 14, 17);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_SECOND_LOG, 15, 17);
		fdMapGridCopyMetatileAttributes(14, 18, 3, 1, 14, 21);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 19, 8);
		fdMapGridCopyMetatileAttributes(14, 18, 3, 1, 18, 25);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 19, 25);
		fdMapGridCopyMetatileAttributes(14, 18, 3, 1, 18, 8);
	}
}

/* Puts the four logs of the Dragon's Eye room where their flags say, and
   rewrites the metatiles of their runs to match. */
static void scFuchinFallsCaveRebuildDragonsEyeRoomLogs(void)
{
	fdMapGridCopyMetatileAttributes(12, 3, 9, 16, 12, 21);

	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 14, 25);
		fdMapGridCopyMetatileAttributes(16, 24, 1, 3, 20, 24);
	} else if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE) !=
		0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 17, 25);
		fdMapGridCopyMetatileAttributes(18, 24, 1, 3, 20, 24);
		fdMapGridCopyMetatileAttributes(18, 24, 1, 3, 14, 24);
		fdMapGridCopyMetatileAttributes(8, 41, 1, 3, 17, 24);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 20, 25);
		fdMapGridCopyMetatileAttributes(16, 24, 1, 3, 14, 24);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_SECOND_LOG, 13, 35);
		fdMapGridCopyMetatileAttributes(14, 34, 1, 3, 15, 34);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_SECOND_LOG, 15, 35);
		fdMapGridCopyMetatileAttributes(14, 34, 1, 3, 13, 34);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 15, 22);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 30);
		fdMapGridCopyMetatileAttributes(5, 41, 3, 1, 14, 22);
	} else if(gmFlagGet(
				  FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) !=
		0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 15, 23);
		fdMapGridCopyMetatileAttributes(5, 42, 3, 1, 14, 23);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 30);
		fdMapGridCopyMetatileAttributes(10, 44, 3, 1, 14, 21);
	} else if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) !=
		0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 15, 26);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 22);
		fdMapGridCopyMetatileAttributes(5, 43, 3, 1, 14, 26);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 30);
	} else if(gmFlagGet(
				  FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP) !=
		0) {
		/* Unused: no move sets this flag, so the log never lies here. */
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 15, 27);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 22);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 30);
		fdMapGridCopyMetatileAttributes(5, 44, 3, 1, 14, 27);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 15, 30);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END) !=
		0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FOURTH_LOG, 15, 23);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 31);
		fdMapGridCopyMetatileAttributes(10, 40, 3, 1, 14, 23);
	} else if(gmFlagGet(
				  FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
		0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FOURTH_LOG, 15, 24);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 31);
		fdMapGridCopyMetatileAttributes(10, 41, 3, 1, 14, 24);
	} else if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) !=
		0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FOURTH_LOG, 15, 27);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 31);
		fdMapGridCopyMetatileAttributes(10, 42, 3, 1, 14, 27);
	} else if(gmFlagGet(
				  FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP) !=
		0) {
		/* Unused: no move sets
		   FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, so the log
		   never lies here and the room never gets the metatiles of
		   METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP.
		 */
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FOURTH_LOG, 15, 28);
		fdMapGridCopyMetatileAttributes(14, 29, 3, 1, 14, 31);
		fdMapGridCopyMetatileAttributes(10, 43, 3, 1, 14, 28);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FOURTH_LOG, 15, 31);
	}
}

/* Puts the logs of the Arctic Blade's room, in both of its halves, where their
   flags say, and rewrites the metatiles of their runs to match. */
static void scFuchinFallsCaveRebuildArcticBladeRoomLogs(void)
{
	if(MainRoutineStateWork.entrance ==
			FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE ||
		MainRoutineStateWork.entrance ==
			FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF) {
		fdMapGridCopyMetatileAttributes(22, 20, 9, 8, 14, 10);
	} else {
		fdMapGridCopyMetatileAttributes(20, 45, 11, 4, 7, 45);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 20, 17);
		fdMapGridCopyMetatileAttributes(19, 11, 3, 1, 19, 10);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_FIRST_LOG, 20, 10);
		fdMapGridCopyMetatileAttributes(19, 11, 3, 1, 19, 17);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_SECOND_LOG, 14, 16);
		fdMapGridCopyMetatileAttributes(16, 15, 1, 3, 22, 15);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_SECOND_LOG, 22, 16);
		fdMapGridCopyMetatileAttributes(16, 15, 1, 3, 14, 15);
	}

	if(gmFlagGet(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST) != 0) {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 17, 46);
		fdMapGridCopyMetatileAttributes(15, 15, 1, 3, 7, 45);
	} else {
		scFuchinFallsCavePlaceLogAtMetatile(ACTOR_THIRD_LOG, 7, 46);
		fdMapGridCopyMetatileAttributes(15, 15, 1, 3, 17, 45);
	}
}

/* Nothing visible happens when Isaac walks north out of the middle passage into
   the dragon statue's dark room (metatiles (14, 17) to (16, 17)); the ground
   mask the room started takes him up again.

   The ground mask (field/field_mode_ground_mask.c) covers him with squares the
   colour of the floor wherever the ground just south of him stands higher than
   his feet: walking along the south side of the room's sunken middle floor, he
   shows only from the knees down. It runs only when he came in from the room of
   Ki or up the middle passage, and coming up the passage starts it with no one
   to follow; come in any other way, he walks there whole. */
static void scFuchinFallsCaveGroundMaskFollowsLeader(void)
{
	if(ImilFuchinFallsTrackLeader != 0) {
		GROUND_MASK_STATE *state = *(HeapSlots + HEAP_SLOT_GROUND_MASK);

		state->actor = fdActorGet(ACTOR_PARTY_LEADER);
	}
}

/* Nothing visible happens when Isaac walks south into the top of the middle
   passage out of the dragon statue's dark room (metatiles (14, 18) to (16,
   18)); the room's ground mask stops following him. */
static void scFuchinFallsCaveGroundMaskStopsFollowing(void)
{
	if(ImilFuchinFallsTrackLeader != 0) {
		GROUND_MASK_STATE *state = *(HeapSlots + HEAP_SLOT_GROUND_MASK);

		state->actor = NULL;
	}
}

/* Sets up the scene as the party enters: Isaac walks up to Fever in Imil Falls
   Cave, and each room of Fuchin Falls Cave is put in order. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0 &&
		MainRoutineStateWork.next == SCENE_IMIL_FALLS_CAVE) {
		gmFlagSet(FLAG_RETREAT_BLOCKED);
		scImilWalkTowardFever();
	} else {
		scImilFuchinFallsPrepareRoom();
	}
	return 0;
}

/* Isaac comes into Imil Falls Cave and walks up the ice to Fever. */
static void scImilWalkTowardFever(void)
{
	fdScriptStart();
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FEVER), 0);
	fdScriptStartScreenTransition();
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 0.1);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 264, 196);
	fdScriptEnd();
}

/* Puts a room of Fuchin Falls Cave in order for the way the party comes in: its
   logs, the Dragon's Eye on its table, and the dragon statue's room dark or
   lit. */
static void scImilFuchinFallsPrepareRoom(void)
{
	ACTOR			  *actor;
	GROUND_MASK_STATE *state;

	scImilFuchinFallsRebuildLogRoom();
	if(MainRoutineStateWork.next == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE) {
		if(!gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND) &&
			MainRoutineStateWork.entrance ==
				FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
			scImilFuchinFallsPlaceDragonsEye();
		}
		if(MainRoutineStateWork.entrance >=
				FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE &&
			MainRoutineStateWork.entrance <=
				FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR) {
			/* Two hidden crates on the west ledge by the stairs down, at
			   metatiles (9, 28) and (11, 28). Isaac walks over both metatiles
			   and nothing shows. XXX (no evidence): what they are for; nothing
			   in the scene reads them again, and no recording can take them
			   away to compare (a probe hides an actor only in its photo, where
			   these show nothing already). */
			scFuchinFallsCaveCreateHiddenActor(
				toFIXED(156), 0, toFIXED(452), SPRITE_CRATE);
			scFuchinFallsCaveCreateHiddenActor(
				toFIXED(188), 0, toFIXED(452), SPRITE_CRATE);
		}
	} else if(MainRoutineStateWork.next ==
		SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE) {
		/* Default: NULL unless the arrival from the south half's middle takes
		   the ground mask. */
		state = NULL;
		actor = fdActorGet(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE);
		ImilFuchinFallsTrackLeader = 0;
		actor->motionFlags = 0;
		actor->y.fixed = 0;
		fdActorSetSpritePriority(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE, 1);
		fdActorSetSpriteColorById(
			ACTOR_INVISIBLE_DRAGON_ROOM_CRATE, SPRITE_COLOR_HIDDEN);
		switch(MainRoutineStateWork.entrance) {
		case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST:
		case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST:
			fdEffectStartGroundMask(0);
			ImilFuchinFallsTrackLeader = 1;
			break;
		case FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE:
			fdEffectStartGroundMask(0);
			ImilFuchinFallsTrackLeader = 1;
			state = GroundMaskStateWork;
			state->actor = NULL;
			break;
		}
		if(MainRoutineStateWork.entrance <=
			FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
			/* Once the eye is set, the shadow of the hidden path is drawn
			   again; before, the room's northern half is dimmed to a dark
			   blue. */
			if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET)) {
				fdMapGridCopyMetatilesTo(30, 57, 19, 57, 1, 1);
				fdMapGridCopyMetatilesTo(30, 8, 12, 8, 8, 7);
			} else {
				FieldStateWork->transition =
					SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
				fdPaletteFadeApplySource(
					PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
				fdPaletteFadeApplyTarget(
					PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
				fdPaletteFadeStartInterpolation(1);
				sySynch(1);
			}
		}
	}
}

/* Puts the logs of the room being entered where their flags say, first saving
   the metatiles of their runs as the map draws them, which the rebuild
   functions copy back from. */
static void scImilFuchinFallsRebuildLogRoom(void)
{
	if(MainRoutineStateWork.next == SCENE_FUCHIN_FALLS_CAVE_ENTRANCE) {
		fdMapGridCopyMetatileAttributes(8, 29, 15, 5, 8, 42);
		scFuchinFallsCaveRebuildEntranceLogs();
	} else if(MainRoutineStateWork.next == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
		fdMapGridCopyMetatileAttributes(12, 8, 10, 18, 0, 28);
		scFuchinFallsCaveRebuildZephyrRoomLogs();
	} else if(MainRoutineStateWork.next ==
			SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE &&
		MainRoutineStateWork.entrance !=
			FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
		fdMapGridCopyMetatileAttributes(12, 21, 9, 16, 12, 3);
		scFuchinFallsCaveRebuildDragonsEyeRoomLogs();
	} else if(MainRoutineStateWork.next ==
		SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE) {
		if(MainRoutineStateWork.entrance ==
				FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE ||
			MainRoutineStateWork.entrance ==
				FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF) {
			fdMapGridCopyMetatileAttributes(14, 10, 9, 8, 22, 20);
		} else {
			fdMapGridCopyMetatileAttributes(7, 45, 11, 4, 20, 45);
		}
		scFuchinFallsCaveRebuildArcticBladeRoomLogs();
	}
}

/* Isaac jumps two metatiles across the water: in Zephyr's room from a log onto
   the stump at the pool's west end or onto the upright log below it, and in the
   Dragon's Eye room from the stump below the north shore onto the upright log.

   In Zephyr's room he jumps north from the upright log against the west wall
   (metatile (12, 19)) onto the stump (metatile (12, 17)), west from the
   crosswise log at the foot of the sandy path onto the same stump, and west
   from the crosswise log at the south shore onto the upright log against the
   wall. In the Dragon's Eye room he jumps south from the stump (metatile (17,
   22)) onto the upright log, but only while it lies in the middle of the
   pool. */
static void scImilFuchinFallsHopOnCue(void)
{
	Sint32 cue = FieldStateWork->stepCue;

	if(MainRoutineStateWork.next == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
		if(cue ==
			METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END) {
			scHopLeaderBy(0, -32);
		} else {
			scHopLeaderBy(-32, 0);
		}
	}
	if(MainRoutineStateWork.next == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE &&
		cue == METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_STUMP &&
		gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE)) {
		scHopLeaderBy(0, 32);
	}
}

/* In the Dragon's Eye room, Isaac jumps north from the upright log across the
   water: from the middle of the pool onto the stump (metatile (17, 22)), or
   from the head of the long channel onto the upper crosswise log, when it lies
   at the head too. */
static void scFuchinFallsCaveHopNorth(void)
{
	scHopLeaderBy(0, -32);
}

/* In the Dragon's Eye room, Isaac jumps west from the lower crosswise log
   halfway down the long channel onto the stump below the west ledge (metatile
   (12, 27)), from which he walks on to the stairs down to the Dragon's Eye. */
static void scFuchinFallsCaveHopWest(void)
{
	scHopLeaderBy(-32, 0);
}

#include "shared/leader_hop.c"

/* Puts the Dragon's Eye on the little table in the middle of the round chamber
   below the Dragon's Eye room, drawn as its item icon. */
static void scImilFuchinFallsPlaceDragonsEye(void)
{
	ACTOR		  *actor;
	SPR			  *sprite;
	ICON_TRANSFER *glyph;
	Uint8		   clear = 0;

	actor = syFieldActorCreate(
		SPRITE_BLANK, toFIXED(248), toFIXED(8), toFIXED(152));
	if(actor != NULL) {
		sprite = actor->sprite;
		sprite->flags = clear;
		sprite->count = clear;
		sprite->image.fullColor = 0;
		sprite->image.palette = 0;
		actor->motionFlags = clear;
		actor->retainSpriteCache = 1;
		glyph = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(*glyph));
		mnIconDrawItem(ITEM_DRAGONS_EYE);
		syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
		ImilFuchinFallsDragonsEye = actor;
	}
}

/* Searching the little table in the round chamber below the Dragon's Eye room
   (metatile (15, 9)) gives the party the Dragon's Eye that lies on it. */
static void scFuchinFallsCaveFindDragonsEye(void)
{
	fdScriptStart();

	if(ImilFuchinFallsDragonsEye != NULL) {
		fdItemDisplayShowPicture(ImilFuchinFallsDragonsEye,
			ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	}

	gmPartyGiveItem(ITEM_DRAGONS_EYE, 0);
	gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND);

	if(ImilFuchinFallsDragonsEye != NULL) {
		fdActorRelease(ImilFuchinFallsDragonsEye);
	}

	fdScriptEnd();
}

/* Moves and grows a puff of the dragon's fire for one tick. */
static void scFuchinFallsCaveUpdateDustMotion(ACTOR *actor)
{
	FIELD_EFFECT *puff = ((FIELD_EFFECT *)actor);

	puff->x += puff->velocity.x;
	puff->y += puff->velocity.y;
	puff->z += puff->velocity.z;
	puff->scaleX += puff->scaleRateX;
	puff->scaleY += puff->scaleRateY;
}

/* Hides a puff of the dragon's fire when its script has run out. */
static Sint32 scFuchinFallsCaveHideDust(ACTOR *actor)
{
	fdActorSetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
	return 0;
}

/* Creates one puff of the dust sprite, flying at the given speed, in the given
   colour and growing to the given size: the puffs of the dragon's fire. */
void scImilFuchinFallsSpawnDust(FIXED x, FIXED y, FIXED z, Sint32 velocityX,
	Sint32 velocityY, Sint32 velocityZ, Uint32 flags,
	const EFFECT_OPTIONS *extra)
{
	FIELD_EFFECT *effect;
	SPR			 *sprite;
	const Sint32 *script;
	const Sint32 *scripts[3] = {
		ImilFuchinFallsShortDustScript,
		ImilFuchinFallsMediumDustScript,
		ImilFuchinFallsLongDustScript,
	};

	effect = (FIELD_EFFECT *)fdActorCreate(SPRITE_DUST_PUFF, x, y, z);
	if(effect == NULL)
		return;
	sprite = effect->sprite;
	fdActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
	fdActorStartScript((ACTOR *)effect, scripts[flags & EFFECT_SCRIPT_MASK]);
	effect->motionFlags = 0;
	sprite->flags = 0;
	effect->update = scFuchinFallsCaveUpdateDustMotion;
	effect->velocity.x = velocityX;
	effect->velocity.y = velocityY;
	effect->velocity.z = velocityZ;
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	sprite->image.priority = 1;
	if((flags & EFFECT_OPTION_MASK) == 0 || extra == NULL)
		return;
	if(flags & EFFECT_USE_PALETTE)
		fdActorSetSpriteColor((ACTOR *)effect, extra->palette);
	if(flags & EFFECT_USE_PRIORITY) {
		effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
		sprite->image.priority = extra->priority;
	}
	if(flags & EFFECT_USE_START_SCALE) {
		effect->scaleX = extra->startScaleX;
		effect->scaleY = extra->startScaleY;
	}
	if(flags & EFFECT_SCALE_TO_TARGET) {
		script = scripts[flags & EFFECT_SCRIPT_MASK];
		if(flags & EFFECT_USE_START_SCALE) {
			effect->scaleRateX = (extra->targetScaleX - effect->scaleX) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
			effect->scaleRateY = (extra->targetScaleY - effect->scaleY) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		} else {
			effect->scaleRateX = (extra->targetScaleX - toFIXED(1)) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
			effect->scaleRateY = (extra->targetScaleY - toFIXED(1)) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
	}
}

/* Every fourth tick while the dragon statue wakes, a red puff of fire blows
   out of its mouth toward the west, with a whoosh every eighth. */
static void scFuchinFallsCaveBreatheDragonFire(void)
{
	CAVE_DUST_OPTIONS options;
	Sint32			  framePhase;
	FIXED			  velocityX;
	FIXED			  velocityY;

	framePhase = TickCount & 3;
	if(framePhase != 0)
		return;
	options.palette = SPRITE_COLOR_RED_TINT;
	options.startScaleX = toFIXED(1) / 2;
	options.startScaleY = toFIXED(1) / 2;
	options.targetScaleX = toFIXED(9) / 5;
	options.targetScaleY = toFIXED(9) / 5;
	if((TickCount & 7) == 0)
		sySoundPlay(SE_WHOOSH);
	velocityX = -toFIXED(1) - toFIXED(syRandomBelow(2));
	velocityY = -((syRandomBelow(3)) * (toFIXED(1) / 5));
	scImilFuchinFallsSpawnDust(toFIXED(308), toFIXED(64), toFIXED(222),
		velocityX, velocityY, framePhase,
		EFFECT_SCRIPT_MEDIUM | EFFECT_USE_PALETTE | EFFECT_USE_START_SCALE |
			EFFECT_SCALE_TO_TARGET,
		(const EFFECT_OPTIONS *)&options);
}

/* Isaac uses the Dragon's Eye at the steps below the dragon statue (metatiles
   (20, 14) to (22, 14)): the music stops, the dark room flashes, and the dragon
   breathes fire west across the room, which glows red and then comes back in
   its own colours, with the shadow of a hidden path across the spikes.

   The eye is used up, and the room stays lit from then on
   (FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET). */
static void scFuchinFallsCaveLightDragonWithEye(void)
{
	Uint32 pass;

	sySoundPlay(MUS_SILENCE);
	sySoundPlay(SE_GEM_SET);
	fdScriptStart();
	fdEndBusyAndReveal();

	for(pass = 0; pass < 4; pass++) {
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_CUSTOM | RGB(24, 24, 16), PALETTES_BG);
		fdPaletteFadeStartInterpolation(1);
		sySynch(2);
		if(pass == 0) {
			fdMapGridCopyMetatilesTo(30, 8, 12, 8, 8, 7);
			fdMapGridCopyMetatilesTo(30, 57, 19, 57, 1, 1);
		}
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
		fdPaletteFadeStartInterpolation(1);
		sySynch(2);
	}

	sySynch(30);

	/* The dragon breathes fire while the room turns red. */
	sySetTickEvent(scFuchinFallsCaveBreatheDragonFire, EVENT_PASS_TICK);
	sySynch(40);
	fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 4, 4), PALETTES_BG);
	fdPaletteFadeStartInterpolation(40);
	sySynch(80);
	syCloseEvent(scFuchinFallsCaveBreatheDragonFire);
	sySynch(20);

	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(80);

	sySynch(80);

	gmFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET);
	gmPartyInventoryDiscard(ITEM_DRAGONS_EYE);
	gmPlayMusic();

	fdScriptEnd();
}

/* Isaac reads the tablet on the first room's south shore (metatile (15, 35)):
   "The dragon's flame illuminates the path of truth in darkness." */
static void scFuchinFallsCaveReadDragonFlameInscription(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgFuchinDragonFlameShowsPath, 1);
	fdScriptEnd();
}

/* Isaac reads the tablet beside the chest in the room of Ki (metatile (16, 5)):
   "The secret of Ki shall be revealed to the disciples of truth." */
static void scFuchinFallsCaveReadKiInscription(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgFuchinSecretKiRevealed, 1);
	fdScriptEnd();
}

/* Isaac reads the tablet in the south half of the dragon statue's room
   (metatile (15, 29)): "Rays of light give birth to shadows, revealing the
   way." */
static void scFuchinFallsCaveReadShadowInscription(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgFuchinLightRevealsShadows, 1);
	fdScriptEnd();
}

/* Isaac looks at the dragon statue from the steps below it (metatiles (20, 14)
   to (22, 14)): "It's a statue of an eyeless dragon.", and once the eye is set,
   "It's a statue of a dragon with burning red eyes."

   While the party carries the Dragon's Eye, the field menu opens after the
   first, so that the player can use it. */
static void scFuchinFallsCaveSearchDragonStatue(void)
{
	fdScriptStart();

	if(gmFlagGet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET) != 0) {
		mnFieldMessageShow(MsgFuchinDragonRedEyes, 1);
	} else {
		mnFieldMessageShow(MsgFuchinEyelessDragon, 1);
		if(gmPartyInventoryFindMember(ITEM_DRAGONS_EYE) != -1) {
			FieldStateWork->fieldMenu = 1;
		}
	}
	fdScriptEnd();
}

/* When Isaac's Reveal fades in the dark half of the dragon statue's room,
   before the Dragon's Eye is set, the room fades up from black to its dim blue
   again. */
static void scFuchinFallsCaveEndRevealLighting(void)
{
	FIELD_EFFECT_STATE		*effectState;
	SCREEN_TRANSITION_STATE *state;

	if(MainRoutineStateWork.entrance <=
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
		effectState = *(HeapSlots + HEAP_SLOT_FIELD_EFFECT);
		state = *(HeapSlots + HEAP_SLOT_SCREEN_TRANSITION);
		effectState->sceneVariant = 1;
		state->nextMode = 0;
		state->duration = 1;
		state->step = 1;
		fdPaletteFadeApplySource(RGB(0, 0, 0), PALETTES_BG);
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
		fdPaletteFadeStartInterpolation(16);
		sySynch(16);
	}
}

#define ICE_SLIDE_HEADINGS ImilFuchinFallsSlideHeadings
#include "shared/leader_slide_on_ice.c"

/* clang-format off */
#include "shared/ice_key_headings.c"

/* The way the leader slides on the ice for each combination of held */
/* directions (DPAD_INDEX); a diagonal slides east or west. */
Sint16 ImilFuchinFallsSlideHeadings[] = {
	KEY_HEADING_NONE, FACING_EAST, FACING_WEST, KEY_HEADING_NONE, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
	FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, KEY_HEADING_NONE, FACING_EAST, FACING_WEST, KEY_HEADING_NONE,
	FACING_EAST, FACING_EAST,
};

/* The Dragon's Eye's icon on the little table, until the party takes it. */
ACTOR *ImilFuchinFallsDragonsEye = NULL;

SCENE_ENTRANCE ImilFuchinFallsNoEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ImilFuchinFallsImilFallsCaveEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ IMIL_FALLS_CAVE_FROM_IMIL, CONDITION_ALWAYS, 264, 0, 289, FACING_NORTH, 0, 135, 62, 376, 328, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ImilFuchinFallsEntranceEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 600, FACING_SOUTH, 0, 64, 64, 416, 656, 0 },
	{ FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 248, 0, 632, FACING_NORTH, 0, 64, 368, 416, 656, 0 },
	{ FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 376, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 120, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 232, 0, 408, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 232, 0, 344, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
	{ FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 152, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
	{ FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 120, 0, 200, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
	{ FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ImilFuchinFallsZephyrRoomEntrances[] = {
	{ FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 64, 416, 480, 0 },
	{ FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 152, 0, 200, FACING_NORTH, 0, 96, 64, 416, 480, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ImilFuchinFallsDragonsEyeRoomEntrances[] = {
	{ FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 128, 64, 368, 256, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 184, 0, 328, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 280, 0, 616, FACING_NORTH, 0, 96, 256, 416, 656, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ImilFuchinFallsArcticBladeRoomEntrances[] = {
	{ FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 328, 0, 72, FACING_SOUTH, 0, 64, 16, 416, 416, 0 },
	{ FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 248, 0, 392, FACING_NORTH, 0, 64, 16, 416, 416, 0 },
	{ FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 416, 416, 800, 0 },
	{ FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 64, 416, 416, 800, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ImilFuchinFallsKiRoomEntrances[] = {
	{ FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 264, 0, 232, FACING_NORTH, 0, 128, 16, 384, 256, 0 },
	{ FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 256, 416, 720, 0 },
	{ FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 216, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
	{ FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 280, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ImilFuchinFallsDragonRoomEntrances[] = {
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST, CONDITION_ALWAYS, 280, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST, CONDITION_ALWAYS, 216, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 104, 0, 280, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_WEST, CONDITION_ALWAYS, 200, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE, CONDITION_ALWAYS, 248, 0, 312, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST, CONDITION_ALWAYS, 296, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_WEST, CONDITION_ALWAYS, 200, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_MIDDLE, CONDITION_ALWAYS, 248, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_EAST, CONDITION_ALWAYS, 296, 0, 424, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 472, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST, CONDITION_ALWAYS, 248, 0, 536, FACING_NORTH, 0, 32, 344, 432, 736, 0 },
	{ FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST, CONDITION_ALWAYS, 120, 0, 696, FACING_SOUTH, 0, 32, 344, 432, 752, 0 },
	{ SCENE_TABLE_END },
};

Uint32 ImilFuchinFallsExits[] = {
	SCENE_EXITS(ImilFallsCave),
	SCENE_EXIT(IMIL_FALLS_CAVE_TO_IMIL, Imil, IMIL_FROM_IMIL_FALLS_CAVE),
	SCENE_EXITS(FuchinFallsCaveEntrance),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds, FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR, FuchinFallsCaveZephyr, FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST),
	SCENE_EXITS(FuchinFallsCaveZephyr),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR),
	SCENE_EXITS(FuchinFallsCaveDragonsEye),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR, FuchinFallsCaveZephyr, FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
	SCENE_EXITS(FuchinFallsCaveArcticBlade),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE),
	SCENE_EXITS(FuchinFallsCaveOrbOfForce),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST),
	SCENE_EXITS(FuchinFallsCaveDragonStatue),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_WEST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_MIDDLE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_EAST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_WEST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST),
	SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE ImilFuchinFallsNoActorPlacements[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ImilFuchinFallsImilFallsCaveActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FEVER), MOVEMENT_TYPE_STAND,
		263, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ImilFuchinFallsEntranceActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		136, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		200, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		296, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ImilFuchinFallsZephyrRoomActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		216, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		248, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		312, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_ZEPHYR), MOVEMENT_TYPE_STAND,
		344, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ImilFuchinFallsDragonsEyeRoomActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		328, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		248, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		248, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		248, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ImilFuchinFallsArcticBladeRoomActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		328, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		120, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ImilFuchinFallsDragonRoomActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		312, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE ImilFuchinFallsNoCues[] = {
	{ CUE_TABLE_END },
};

CUE ImilFuchinFallsImilFallsCaveCues[] = {
	{ CUE_STEP, IMIL_FALLS_CAVE_TO_IMIL, CONDITION_ALWAYS, SCRIPT_REF(scImilLeaveFallsCave) },
	{ CUE_STEP, METATILE_BEHAVIOR_IMIL_FALLS_CAVE_ICE, CONDITION_ALWAYS, SCRIPT_REF(scImilMoveAcrossIce) },
	{ CUE_TALK, ACTOR_FEVER, CONDITION_ALWAYS, SCRIPT_REF(scImilFeverJoins) },
	{ CUE_TABLE_END },
};

CUE ImilFuchinFallsEntranceCues[] = {
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_WEST, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveEntranceMoveFirstLogEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveEntranceMoveFirstLogWest) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_WEST, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveEntranceMoveSecondLogSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveEntranceMoveSecondLogNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleSecondLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleSecondLog) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveEntranceMoveThirdLogSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveEntranceMoveThirdLogNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ CUE_EXIT, 21, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS },
	{ CUE_EXIT, 22, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
	{ CUE_EXIT, 23, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE },
	{ CUE_EXIT, 24, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF },
	{ CUE_EXIT, 25, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF },
	{ CUE_EXIT, 26, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 27, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST },
	{ CUE_EXIT, 28, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_TABLET, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveReadDragonFlameInscription) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_MIMIC_BEATEN, FIND_MIMIC | 1 },
	{ CUE_TABLE_END },
};

CUE ImilFuchinFallsZephyrRoomCues[] = {
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_START, FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, SCRIPT_REF(scFuchinFallsCaveZephyrRoomMoveFirstLogToEastEnd) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_START, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveZephyrRoomNudgeFirstLogWest) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, SCRIPT_REF(scImilFuchinFallsHopOnCue) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveZephyrRoomMoveFirstLogEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveZephyrRoomMoveFirstLogToWestEnd) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_START, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scImilFuchinFallsHopOnCue) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END, SCRIPT_REF(scImilFuchinFallsHopOnCue) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveZephyrRoomMoveSecondLogSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveZephyrRoomMoveSecondLogNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleSecondLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleSecondLog) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveZephyrRoomMoveThirdLogSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveZephyrRoomMoveThirdLogNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ CUE_EXIT, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
	{ CUE_TALK, ACTOR_ZEPHYR, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveFightZephyr) },
	{ CUE_TABLE_END },
};

CUE ImilFuchinFallsDragonsEyeRoomCues[] = {
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_EAST_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveFirstLogWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveFirstLogEastFromMiddle) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveFirstLogWestIfClear) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveFirstLogEastFromWestEnd) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, IF_FLAG_SET(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END), SCRIPT_REF(scFuchinFallsCaveHopNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_EAST_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_EAST, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveSecondLogWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveSecondLogEast) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleSecondLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_EAST, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleSecondLog) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_SOUTH_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveThirdLogNorthFromSouthEnd) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromNorthEnd) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomNudgeThirdLogNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromUpperStop) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveThirdLogNorthFromMiddle) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromMiddle) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveThirdLogNorthFromLowerStop) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveThirdLogSouthFromLowerStop) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_SOUTH_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_SOUTH_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveFourthLogNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveFourthLogSouthFromNorthEnd) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomNudgeFourthLogNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveFourthLogSouthFromUpperStop) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveHopWest) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomMoveFourthLogSouthFromMiddle) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomUnusedMoveFourthLogSouthFromLowerStop) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveDragonsEyeRoomUnusedMoveFourthLogSouthFromLastStop) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_SOUTH_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFourthLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFourthLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFourthLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFourthLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFourthLog) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_STUMP, CONDITION_ALWAYS, SCRIPT_REF(scImilFuchinFallsHopOnCue) },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_TABLE, FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND, SCRIPT_REF(scFuchinFallsCaveFindDragonsEye) },
	{ CUE_TABLE_END },
};
/* clang-format on */

CUE ImilFuchinFallsArcticBladeRoomCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_SOUTH),
		METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_NORTH,
		CONDITION_ALWAYS,
		SCRIPT_REF(scFuchinFallsCaveArcticBladeRoomMoveFirstLogSouth)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH,
		CONDITION_ALWAYS,
		SCRIPT_REF(scFuchinFallsCaveArcticBladeRoomMoveFirstLogNorth)},
	{CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_NORTH,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog)},
	{CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleFirstLog)},
	{STEP_CUE_WALKING_INTO(FACING_WEST),
		METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_EAST,
		FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH,
		SCRIPT_REF(scFuchinFallsCaveArcticBladeRoomMoveSecondLogWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST),
		METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST,
		CONDITION_ALWAYS,
		SCRIPT_REF(scFuchinFallsCaveArcticBladeRoomMoveSecondLogEast)},
	{CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_EAST,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleSecondLog)},
	{CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleSecondLog)},
	{STEP_CUE_WALKING_INTO(FACING_EAST),
		METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_WEST,
		CONDITION_ALWAYS,
		SCRIPT_REF(scFuchinFallsCaveArcticBladeRoomMoveThirdLogEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST),
		METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST,
		CONDITION_ALWAYS,
		SCRIPT_REF(scFuchinFallsCaveArcticBladeRoomMoveThirdLogWest)},
	{CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_WEST,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog)},
	{CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSettleThirdLog)},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE},
	{CUE_EXIT, 2, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF},
	{CUE_EXIT, 3, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF},
	{CUE_EXIT, 4, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE},
	{EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_ARCTIC_BLADE,
		FIND_ITEM(ITEM_ARCTIC_BLADE)},
	{CUE_TABLE_END},
};

CUE ImilFuchinFallsKiRoomCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF},
	{CUE_EXIT, 2, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF},
	{CUE_EXIT, 3, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST},
	{CUE_EXIT, 4, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST},
	{EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_FUCHIN_FALLS_KI_ROOM_TABLET,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveReadKiInscription)},
	{EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND,
		FIND_ITEM(ITEM_ORB_OF_FORCE)},
	{CUE_TABLE_END},
};

CUE ImilFuchinFallsDragonRoomCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST},
	{CUE_EXIT, 2, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE},
	{CUE_EXIT, 4, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST},
	{CUE_EXIT, 5, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE},
	{CUE_EXIT, 6, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST},
	{CUE_EXIT, 7, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST},
	{CUE_EXIT, 8, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE},
	{CUE_EXIT, 9, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE},
	{CUE_EXIT, 11, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 12, CONDITION_ALWAYS,
		FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST},
	{CUE_STEP,
		METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_NORTH_OF_MIDDLE_PASSAGE,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveGroundMaskFollowsLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_MIDDLE_PASSAGE,
		CONDITION_ALWAYS,
		SCRIPT_REF(scFuchinFallsCaveGroundMaskStopsFollowing)},
	{EXAMINE_CUE(EXAMINE_SILENT),
		METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_TABLET, CONDITION_ALWAYS,
		SCRIPT_REF(scFuchinFallsCaveReadShadowInscription)},
	{USE_ITEM(ITEM_DRAGONS_EYE), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_STATUE,
		FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
		SCRIPT_REF(scFuchinFallsCaveLightDragonWithEye)},
	{EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_FUCHIN_FALLS_DRAGON_STATUE,
		CONDITION_ALWAYS, SCRIPT_REF(scFuchinFallsCaveSearchDragonStatue)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
		FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
		SCRIPT_REF(scFuchinFallsCaveEndRevealLighting)},
	{CUE_TABLE_END},
};

Sint32 ImilFuchinFallsShortDustScript[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_CALL(scFuchinFallsCaveHideDust),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

Sint32 ImilFuchinFallsMediumDustScript[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_CALL(scFuchinFallsCaveHideDust),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

Sint32 ImilFuchinFallsLongDustScript[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_CALL(scFuchinFallsCaveHideDust),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};
