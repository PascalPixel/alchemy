/* The Lemurian Ship below deck, at the East Indra Shore, in two scenes
   (SceneId_LemurianShipCabins and SceneId_LemurianShipAquaHydra) whose rooms
   are told apart by the entrance used.

   Piers leads the party down from the main deck through the captain's cabin,
   the room of his logbook, into rooms full of Aqua Jellies. Each jelly beaten
   melts into a puddle that Frost turns into a pillar of ice to jump across, and
   tall crates are pushed between them: two jellies in the first room, six and a
   crate in the room of the lift, six more and a crate among the pipes of the
   room after it. A jelly by the lift takes fright and flees down it; beaten
   below, it leaves a puddle that Frost freezes to hold the lift up. In the hold
   the last jelly calls the others together into the Aqua Hydra, and once it is
   beaten the hold floods, keeping its chest out of reach until Parch dries it
   on a later visit. In the room of two pillars below, the left pillar already
   stands sunk in its square; Douse fills the right one and Move pushes it onto
   the tile marked with a drop of water, where it sinks too and opens the arched
   doorway to the engine room. There Piers sets the Black Crystal in its
   pedestal and the ship sails. */
#include "graphics/palette_buffers.h"
#include "field/field_state.h"
#include "field/field.h"
#include "game/game.h"
#include "scene/lemurian_ship.h"
#include "field/field_sprite.h"
#include "field/script_inline.h"
#include "edition.h"
#include "macros.h"
#include "field/actor_script.h"
#include "item_ids.h"
#include "text/msg_ids.h"

/* The Lemurian Ship's room of two jellies, by the rows of
   LemurianShipTwoJellyRoomPlacements. */
typedef enum LEMURIAN_SHIP_TWO_JELLY_ROOM_ACTOR_ID {
	ACTOR_TWO_JELLY_ROOM_JELLY_A = ACTOR_FIRST_PLACED,
	ACTOR_TWO_JELLY_ROOM_JELLY_B,
	ACTOR_TWO_JELLY_ROOM_PUDDLE_A,
	ACTOR_TWO_JELLY_ROOM_PUDDLE_B
} LEMURIAN_SHIP_TWO_JELLY_ROOM_ACTOR_ID;

/* The Lemurian Ship's room of six jellies and the lift, by the rows of
   LemurianShipSixJellyRoomPlacements. */
typedef enum LEMURIAN_SHIP_SIX_JELLY_ROOM_ACTOR_ID {
	ACTOR_SIX_JELLY_ROOM_CRATE = ACTOR_FIRST_PLACED,
	ACTOR_SIX_JELLY_ROOM_JELLY_A,
	ACTOR_SIX_JELLY_ROOM_JELLY_B,
	ACTOR_SIX_JELLY_ROOM_JELLY_C,
	ACTOR_SIX_JELLY_ROOM_JELLY_D,
	ACTOR_SIX_JELLY_ROOM_JELLY_E,
	ACTOR_SIX_JELLY_ROOM_JELLY_F,
	ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_A,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_B,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_C,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_D,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_E
} LEMURIAN_SHIP_SIX_JELLY_ROOM_ACTOR_ID;

/* The Lemurian Ship's room of the crate on the ledge, by the rows of
   LemurianShipLedgeRoomPlacements. */
typedef enum LEMURIAN_SHIP_LEDGE_ROOM_ACTOR_ID {
	ACTOR_LEDGE_CRATE = ACTOR_FIRST_PLACED
} LEMURIAN_SHIP_LEDGE_ROOM_ACTOR_ID;

/* The Lemurian Ship's room of the pipes, by the rows of
   LemurianShipPipeRoomPlacements. */
typedef enum LEMURIAN_SHIP_PIPE_ROOM_ACTOR_ID {
	ACTOR_PIPE_ROOM_CRATE = ACTOR_FIRST_PLACED,
	ACTOR_PIPE_ROOM_JELLY_A,
	ACTOR_PIPE_ROOM_JELLY_B,
	ACTOR_PIPE_ROOM_JELLY_C,
	ACTOR_PIPE_ROOM_JELLY_D,
	ACTOR_PIPE_ROOM_JELLY_E,
	ACTOR_PIPE_ROOM_JELLY_F, /* Unused: placed in the room's corner */
	ACTOR_PIPE_ROOM_JELLY_G,
	ACTOR_PIPE_ROOM_PUDDLE_A,
	ACTOR_PIPE_ROOM_PUDDLE_B,
	ACTOR_PIPE_ROOM_PUDDLE_C,
	ACTOR_PIPE_ROOM_PUDDLE_D,
	ACTOR_PIPE_ROOM_PUDDLE_E,
	ACTOR_PIPE_ROOM_PUDDLE_F, /* Unused: never placed */
	ACTOR_PIPE_ROOM_PUDDLE_G
} LEMURIAN_SHIP_PIPE_ROOM_ACTOR_ID;

/* The Lemurian Ship's hold, where the jellies gather into the Aqua Hydra, by
   the rows of LemurianShipHoldPlacements; the party's rows go to their
   slots. */
typedef enum LEMURIAN_SHIP_HOLD_ACTOR_ID {
	ACTOR_HOLD_JELLY_A = ACTOR_FIRST_PLACED,
	ACTOR_HOLD_JELLY_B,
	ACTOR_HOLD_JELLY_C,
	ACTOR_HOLD_JELLY_D,
	ACTOR_HOLD_JELLY_E,
	ACTOR_HOLD_JELLY_F
} LEMURIAN_SHIP_HOLD_ACTOR_ID;

/* The Lemurian Ship's room of the two pillars Douse fills, by the rows of
   LemurianShipPillarRoomPlacements. */
typedef enum LEMURIAN_SHIP_PILLAR_ROOM_ACTOR_ID {
	ACTOR_RIGHT_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_LEFT_PILLAR,
	ACTOR_RIGHT_PILLAR_PUDDLE,
	ACTOR_LEFT_PILLAR_PUDDLE
} LEMURIAN_SHIP_PILLAR_ROOM_ACTOR_ID;

/* The Lemurian Ship's engine room, by the rows of
   LemurianShipEngineRoomPlacements; the party's rows go to their slots. */
typedef enum LEMURIAN_SHIP_ENGINE_ROOM_ACTOR_ID {
	ACTOR_ENGINE_ROOM_KRADEN = ACTOR_FIRST_PLACED
} LEMURIAN_SHIP_ENGINE_ROOM_ACTOR_ID;

/* Each Aqua Jelly by its number, and last the Aqua Hydra they become: the
   offset of its flags from FLAG_JELLIES_BEATEN, FLAG_JELLY_MELTING,
   FLAG_PUDDLE_FROZEN and FLAG_FIGHTING_JELLY. */
typedef enum LEMURIAN_SHIP_JELLY {
	JELLY_TWO_JELLY_ROOM_A,
	JELLY_TWO_JELLY_ROOM_B,
	JELLY_SIX_JELLY_ROOM_A, /* the jelly by the lift */
	JELLY_SIX_JELLY_ROOM_B,
	JELLY_SIX_JELLY_ROOM_C,
	JELLY_SIX_JELLY_ROOM_D,
	JELLY_SIX_JELLY_ROOM_E,
	JELLY_SIX_JELLY_ROOM_F,
	JELLY_PIPE_ROOM_A,
	JELLY_PIPE_ROOM_B,
	JELLY_PIPE_ROOM_C,
	JELLY_PIPE_ROOM_D,
	JELLY_PIPE_ROOM_E,
	JELLY_PIPE_ROOM_F, /* placed in the room's corner and never fought */
	JELLY_PIPE_ROOM_G,
	JELLY_AQUA_HYDRA,
	JELLY_COUNT
} LEMURIAN_SHIP_JELLY;

/* The battles below deck by their slot of ENCOUNTER_ZONE_AQUA_JELLIES: an Aqua
   Jelly, or the Aqua Hydra. */
typedef enum LEMURIAN_SHIP_ENCOUNTER {
	ENCOUNTER_AQUA_JELLY,
	ENCOUNTER_AQUA_HYDRA
} LEMURIAN_SHIP_ENCOUNTER;

#define FLAG_JELLIES_BEATEN		 0x930 /* the first, by LEMURIAN_SHIP_JELLY */
/* The Aqua Hydra's, among the jellies' flags. */
#define FLAG_AQUA_HYDRA_BEATEN	 (FLAG_JELLIES_BEATEN + JELLY_AQUA_HYDRA)
#define FLAG_PILLAR_WAY_OPEN	 0x982 /* both pillars sank, the way opened */
#define FLAG_SHIP_ENGINE_RUNNING 0x983 /* the Black Crystal is set */
#define FLAG_HOLD_PARCHED		 0x9e4 /* Parch dried the flooded hold */
/* The tall crate fell off the ledge by the stairs. */
#define FLAG_CRATE_DROPPED		 FLAG_SCENE_TEMP_00
#define FLAG_JELLY_FLED			 FLAG_SCENE_TEMP_01 /* the frightened jelly ran */
/* The right pillar sank into the floor on its tile. */
#define FLAG_RIGHT_PILLAR_SUNK	 FLAG_SCENE_TEMP_02
/* The left pillar sank into the floor on its tile. */
#define FLAG_LEFT_PILLAR_SUNK	 FLAG_SCENE_TEMP_03

/* The pillar room's raised cues: a pillar's puddle has dried, and the pillar
   rises. */
typedef enum LEMURIAN_SHIP_RAISED_CUE {
	RAISED_RIGHT_PILLAR = 200,
	RAISED_LEFT_PILLAR
} LEMURIAN_SHIP_RAISED_CUE;

#define FLAG_RIGHT_PILLAR_FULL FLAG_SCENE_TEMP_04 /* Douse filled it */
/* Douse filled the left pillar. */
#define FLAG_LEFT_PILLAR_FULL  FLAG_SCENE_TEMP_05
/* The lever took the lift down. */
#define FLAG_LIFT_LOWERED	   FLAG_SCENE_TEMP_07
/* The first, by LEMURIAN_SHIP_JELLY: Frost froze the jelly's puddle. */
#define FLAG_PUDDLE_FROZEN	   FLAG_SCENE_TEMP_10
/* The pillar room's puddles share the room of two jellies' puddle rows
   (LemurianShipTwoJellyRoomPuddles) and so its flags: Frost froze the puddle
   on the pillar. */
#define FLAG_RIGHT_PILLAR_PUDDLE_FROZEN \
	(FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_A)
#define FLAG_LEFT_PILLAR_PUDDLE_FROZEN \
	(FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_B)
/* The first, by LEMURIAN_SHIP_JELLY: the jelly was just beaten and melts on
   arrival. */
#define FLAG_JELLY_MELTING	FLAG_SCENE_TEMP_20
/* The first, by LEMURIAN_SHIP_JELLY: the battle with the jelly is on. */
#define FLAG_FIGHTING_JELLY FLAG_AREA_TEMP_00

/* The cabins' entrances from within the scene, by the room each opens into and
   the room it leads from; scene_exits.h names those from the main deck and the
   lower deck. The main deck's door leads into the room of the captain's
   logbook, and four halls join the rooms: one off the room of two jellies, one
   off the stair room, and two off the room of six jellies' west and east
   doors. */
typedef enum LEMURIAN_SHIP_CABINS_ENTRANCE {
	ENTRANCE_LOGBOOK_ROOM_FROM_TWO_JELLY_ROOM = 2,
	ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM,
	ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM,
	ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL,
	ENTRANCE_STAIR_ROOM_FROM_TWO_JELLY_ROOM,
	ENTRANCE_STAIR_ROOM_FROM_STAIR_ROOM_HALL,
	ENTRANCE_SIX_JELLY_ROOM_WEST_HALL_FROM_SIX_JELLY_ROOM = 10,
	ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_SIX_JELLY_ROOM,
	ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_PIPE_ROOM,
	ENTRANCE_TWO_JELLY_ROOM_HALL_FROM_TWO_JELLY_ROOM,
	ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST = 16,
	ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST,
	ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM,
	ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL = 25,
	ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL,
	ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST,
	ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST,
	ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL,
	ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM
} LEMURIAN_SHIP_CABINS_ENTRANCE;

/* The lower deck's entrances from within the scene: the pillar room, the
   corridor beyond it and the engine room. */
typedef enum LEMURIAN_SHIP_LOWER_DECK_ENTRANCE {
	ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR = 6,
	ENTRANCE_CORRIDOR_FROM_PILLAR_ROOM,
	ENTRANCE_CORRIDOR_FROM_ENGINE_ROOM,
	/* Until the engine runs; Piers sets the Black Crystal here. */
	ENTRANCE_ENGINE_ROOM_FROM_CORRIDOR,
	/* Where the Aqua Hydra's battle returns the party, in the flooded hold. */
	ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE = 15,
	/* Once the engine runs: the engine room lit. */
	ENTRANCE_RUNNING_ENGINE_ROOM_FROM_CORRIDOR = 19
} LEMURIAN_SHIP_LOWER_DECK_ENTRANCE;

/* The cabins' exits within the scene, by the room each leaves and the room it
   leads to, as their entrances are named; scene_exits.h names those to the main
   deck and the lower deck. The doors among them open as Felix walks into them
   (LemurianShipDoors). */
typedef enum LEMURIAN_SHIP_CABINS_EXIT {
	EXIT_LOGBOOK_ROOM_TO_TWO_JELLY_ROOM = 2,
	EXIT_TWO_JELLY_ROOM_TO_LOGBOOK_ROOM,
	EXIT_TWO_JELLY_ROOM_TO_STAIR_ROOM,
	EXIT_TWO_JELLY_ROOM_TO_TWO_JELLY_ROOM_HALL,
	EXIT_STAIR_ROOM_TO_TWO_JELLY_ROOM,
	EXIT_STAIR_ROOM_TO_STAIR_ROOM_HALL,
	EXIT_SIX_JELLY_ROOM_WEST_HALL_TO_SIX_JELLY_ROOM = 10,
	EXIT_SIX_JELLY_ROOM_EAST_HALL_TO_SIX_JELLY_ROOM,
	EXIT_SIX_JELLY_ROOM_EAST_HALL_TO_PIPE_ROOM,
	EXIT_TWO_JELLY_ROOM_HALL_TO_TWO_JELLY_ROOM,
	EXIT_LEDGE_ROOM_TO_SIX_JELLY_ROOM_WEST = 16,
	EXIT_LEDGE_ROOM_TO_SIX_JELLY_ROOM_EAST,
	EXIT_LEDGE_ROOM_TO_PIPE_ROOM,
	EXIT_STAIR_ROOM_HALL_TO_STAIR_ROOM = 20,
	EXIT_SIX_JELLY_ROOM_TO_WEST_HALL = 25,
	EXIT_SIX_JELLY_ROOM_TO_EAST_HALL,
	EXIT_SIX_JELLY_ROOM_TO_LEDGE_ROOM_WEST,
	EXIT_SIX_JELLY_ROOM_TO_LEDGE_ROOM_EAST,
	EXIT_PIPE_ROOM_TO_SIX_JELLY_ROOM_EAST_HALL,
	EXIT_PIPE_ROOM_TO_LEDGE_ROOM
} LEMURIAN_SHIP_CABINS_EXIT;

/* The lower deck's exits within the scene. */
typedef enum LEMURIAN_SHIP_LOWER_DECK_EXIT {
	/* The arched doorway, open once both pillars have sunk. */
	EXIT_PILLAR_ROOM_TO_CORRIDOR = 6,
	EXIT_CORRIDOR_TO_PILLAR_ROOM,
	EXIT_CORRIDOR_TO_ENGINE_ROOM,
	EXIT_ENGINE_ROOM_TO_CORRIDOR
} LEMURIAN_SHIP_LOWER_DECK_EXIT;

/* The metatile behaviors the cabins' and the hold's cues answer. */
typedef enum LEMURIAN_SHIP_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_LIFT_LEVER = 40, /* checked beside the lift */
	/* stepping onto the lift from the east */
	METATILE_BEHAVIOR_LIFT_EAST = 41,
	/* Near the jelly by the lift, which flees down it again. */
	METATILE_BEHAVIOR_NEAR_LIFT_JELLY = 42,
	/* stepping onto the lift from the west */
	METATILE_BEHAVIOR_LIFT_WEST = 43,
	/* the hold's water, which Parch dries */
	METATILE_BEHAVIOR_FLOODED_HOLD = 255
} LEMURIAN_SHIP_METATILE_BEHAVIOR;

extern SCENE_ENTRANCE LemurianShipAquaHydraEntrances[];

Uint8 *scGetEntrances(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_LEMURIAN_SHIP_CABINS)
		return LemurianShipCabinsEntrances;
	if(scene == SCENE_LEMURIAN_SHIP_AQUA_HYDRA)
		return (Uint8 *)LemurianShipAquaHydraEntrances;
	return LemurianShipEntrances;
}

Uint8 *scGetExitRegions(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_LEMURIAN_SHIP_CABINS)
		return LemurianShipCabinsRegions;
	if(scene == SCENE_LEMURIAN_SHIP_AQUA_HYDRA)
		return LemurianShipAquaHydraRegions;
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

/* The cabin doors (LemurianShipDoors). */
#define CABIN_DOOR_COUNT 10
/* The lower deck's doors (LemurianShipLowerDoors); the hold's two doors
   follow, as they open while it is flooded. */
#define LOWER_DOOR_COUNT 4

/* The ship's maps keep their overhead layer 60 rows south of the ground's. A
   door swings open there, drawn over the leader walking through it. */
#define OVERHEAD_LAYER_ROW 60

/* The sinking pillar's sprite sheet's animations: hollow and empty, full of
   water, and sunk into the floor to its watery rim. */
typedef enum SINKING_PILLAR_ANIMATION {
	SINKING_PILLAR_EMPTY = 1,
	SINKING_PILLAR_FULL,
	SINKING_PILLAR_SUNK
} SINKING_PILLAR_ANIMATION;

/* The lift in the room of six jellies, its 3 by 3 metatiles at (50, 64) drawn
   lowered from spare metatiles at (68, 68) or raised from (72, 68). */
#define scLemurianShipDrawLiftLowered() \
	fdMapGridCopyMetatilesTo(68, 68, 50, 64, 3, 3)
#define scLemurianShipDrawLiftRaised() \
	fdMapGridCopyMetatilesTo(72, 68, 50, 64, 3, 3)

void scLemurianShipLeapIntoHydra(Sint32 id);
void scLemurianShipCheckPillarsOnDropTiles(void);
void scLemurianShipSetUpPuddle(Sint32 id);
void scLemurianShipSetUpLowPuddle(Sint32 id);

extern Sint32 LemurianShipJellyMelts[];
extern Sint32 LemurianShipPuddleSpreads[];
extern Sint32 LemurianShipHydraForms[];
extern Sint16 LemurianShipCrate[];
extern Sint16 LemurianShipTwoJellyRoomPuddles[];
extern Sint16 LemurianShipSixJellyRoomPuddles[];
extern Sint16 LemurianShipPipeRoomPuddles[];
extern Sint16 LemurianShipPillars[];

void		  scLemurianShipJellyFlees(void);
void		  scLemurianShipSetBlackCrystal(void);
extern Uint32 LemurianShipLightToOrb[];

extern ACTOR_PLACE LemurianShipLedgeRoomPlacements[];
extern ACTOR_PLACE LemurianShipPipeRoomPlacements[];

ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
		switch(MainRoutineStateWork.entrance) {
		case ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM:
		case ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM:
		case ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL:
			return LemurianShipTwoJellyRoomPlacements;
		case ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST:
			return LemurianShipSixJellyRoomPlacements;
		case LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A:
		case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST:
		case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST:
		case ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM:
			return LemurianShipLedgeRoomPlacements;
		case ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL:
		case ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM:
			return LemurianShipPipeRoomPlacements;
		}
	} else if(MainRoutineStateWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
		switch(MainRoutineStateWork.entrance) {
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C:
		case ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_B:
			return LemurianShipHoldPlacements;
		case ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_EAST:
			return LemurianShipPillarRoomPlacements;
		default:
			return LemurianShipEngineRoomPlacements;
		}
	}
	return LemurianShipPlacements;
}

/* Felix walks north into one of the cabins' wooden doors: it swings open, he
   steps through, and the screen closes and opens on the room beyond.

   The doors are LemurianShipDoors, found by the metatile's step cue, which is
   also the exit taken; the first is the logbook room's, metatile (4, 4). Every
   placed actor stops moving while he goes. */
void scLemurianShipOpenDoor(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*actor;
	Uint32		 found;
	Uint32		 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	found = 0;
	for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
		actor = ((ACTOR *)fdActorGetById(i));
		if(actor != NULL)
			actor->motionFlg = 0;
	}
	for(i = 0; i < CABIN_DOOR_COUNT; i++) {
		if(state->stepCue == LemurianShipDoors[i].exit)
			found = i;
	}
	fdScriptPlaySnd(SE_DOOR_OPEN);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	fdMapGridCopyMetatilesTo(
		0, 0, LemurianShipDoors[found].x, LemurianShipDoors[found].z, 1, 2);
	fdMapGridAnimateMetatiles(LemurianShipDoors[found].value,
		LemurianShipDoors[found].x,
		LemurianShipDoors[found].z + OVERHEAD_LAYER_ROW);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	{
		Sint32 owner = MainRoutineStateWork.leaderActorId;

		/* DEAD: found never reaches 12 among the ten doors; the ROM keeps the
		   test. */
		if(found == 12)
			fdActorSetDestinationOffset(owner, 0, -4);
		else
			fdActorCenterAndWalk(owner, 2, -4);
	}
	fdScriptWait(4);
	syImportScriptSetExitRequest(state->stepCue);
}

/* Felix walks north into one of the two doors at the top of the hold, metatiles
   (4, 4) and (10, 4): it swings open and he steps through.

   While the hold is flooded (after the Aqua Hydra, until Parch) the party walks
   a copy of the hold drawn 19 rows further south, and the doors there,
   metatiles (4, 23) and (10, 23), are the last rows of
   LemurianShipLowerDoors. */
void scLemurianShipOpenLowerDoor(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*actor;
	Uint32		 found;
	Uint32		 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	found = 0;
	for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
		actor = ((ACTOR *)fdActorGetById(i));
		if(actor != NULL)
			actor->motionFlg = 0;
	}
	for(i = 0; i < LOWER_DOOR_COUNT; i++) {
		if(state->stepCue == LemurianShipLowerDoors[i].exit)
			found = i;
	}
	if(found < 2 && fdScriptGetFlag(FLAG_AQUA_HYDRA_BEATEN) &&
		fdScriptGetFlag(FLAG_HOLD_PARCHED) == 0)
		found += LOWER_DOOR_COUNT;
	fdScriptPlaySnd(SE_DOOR_OPEN);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	fdMapGridCopyMetatilesTo(0, 0, LemurianShipLowerDoors[found].x,
		LemurianShipLowerDoors[found].z, 1, 2);
	fdMapGridAnimateMetatiles(LemurianShipLowerDoors[found].value,
		LemurianShipLowerDoors[found].x,
		LemurianShipLowerDoors[found].z + OVERHEAD_LAYER_ROW);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorCenterOnColumn(MainRoutineStateWork.leaderActorId, 2, -4);
	fdScriptWait(4);
	syImportScriptSetExitRequest(state->stepCue);
}

/* Once both pillars have sunk, Felix walks north into the arched doorway at the
   top of the room of two pillars, metatile (32, 6), and shrinks away into the
   passage as the screen closes. Before that, walking into it does nothing. */
void scLemurianShipWalkIntoPassage(void)
{
	if(fdScriptGetFlag(FLAG_PILLAR_WAY_OPEN)) {
		FIELD_STATE		   *state = Ram_HeapSlots->fieldState;
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
		ACTOR			   *leader;
		FIXED				x;
		Sint32				pixelX;

		leader = ((ACTOR *)fdActorGetById(party->leaderActorId));
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorSetAnimation(party->leaderActorId, ANIM_WALK);
		fdActorSetSpeed(party->leaderActorId, 0.5);
		x = leader->x.fixed;
		pixelX = FIXEDtoINT(x);
		fdActorSetMoveTarget(leader, toFIXED(8 - pixelX % 16) + x,
			leader->y.fixed, leader->z.fixed);
		scFieldActorWaitForMove(leader);
		leader->facing = FACING_NORTH;
		fdActorSetScript(party->leaderActorId, LeaderWalkThroughDoorScript);
		sySynch(12);
		fdScriptPlaySnd(SE_MAP_EXIT);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		syImportScriptSetExitRequest(state->stepCue);
	}
}

extern CUE LemurianShipLedgeRoomCues[];
extern CUE LemurianShipHoldCues[];
extern CUE LemurianShipPillarRoomCues[];

const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
		switch(MainRoutineStateWork.entrance) {
		case ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM:
		case ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM:
		case ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL:
			return LemurianShipTwoJellyRoomCues;
		case ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST:
			return LemurianShipSixJellyRoomCues;
		case LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A:
		case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST:
		case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST:
		case ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM:
			return LemurianShipLedgeRoomCues;
		case ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL:
		case ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM:
			return LemurianShipPipeRoomCues;
		default:
			return LemurianShipCabinCues;
		}
	} else if(MainRoutineStateWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
		switch(MainRoutineStateWork.entrance) {
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C:
		case ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_B:
			return LemurianShipHoldCues;
		case ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_EAST:
			return LemurianShipPillarRoomCues;
		default:
			return LemurianShipLowerDeckCues;
		}
	}
	return LemurianShipCues;
}

/* Felix checks the lever beside the lift in the room of six jellies, metatile
   (53, 3): before the engine runs, "There's no reaction... It doesn't seem to
   be running."; once it runs, the lift goes down or comes back up with a
   crunch, or does nothing while the frozen puddle holds it up. */
void scLemurianShipExamineLiftSwitch(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
		if(fdScriptGetFlag(FLAG_LIFT_LOWERED)) {
			fdScriptPlaySnd(SE_CRUNCH);
			scFieldActorSetPosAndResetMotion(
				((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE)),
				toFIXED(824), -toFIXED(32), toFIXED(72));
			((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE))
				->groundHeight = -toFIXED(32);
			scLemurianShipDrawLiftRaised();
			fdMapGridCopyMetatileAttributes(106, 16, 3, 2, 110, 4);
			fdMapGridCopyMetatileAttributes(50, 1, 3, 1, 50, 4);
			fdScriptClearFlag(FLAG_LIFT_LOWERED);
		} else if(fdScriptGetFlag(
					  (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A))) {
			fdScriptPlaySnd(SE_ITEM_REMOVE);
		} else {
			fdScriptPlaySnd(SE_CRUNCH);
			fdActorSetPos(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, 0, 0);
			scLemurianShipDrawLiftLowered();
			fdMapGridCopyMetatileAttributes(102, 16, 3, 2, 110, 4);
			fdMapGridCopyMetatileAttributes(50, 0, 3, 1, 50, 4);
			fdScriptSetFlag(FLAG_LIFT_LOWERED);
		}
	} else {
		/* "There's no reaction... It doesn't seem to be running." */
		mnFieldMessageShow((Sint32)MsgLemurianShipTheresNoReactionItDoesnt, 1);
	}
	fdScriptEnd();
}

/* Nothing visible happens as Felix starts to Move or push the tall crate in the
   room of six jellies or the room of the pipes; the game takes the crate up so
   that it can slide (`fdActorLiftTargetBlock`). */
void scLemurianShipLiftCrate(void)
{
	fdActorLiftTargetBlock(LemurianShipCrate);
}

/* Felix slides the tall crate one metatile with Move or a push, in the room of
   six jellies or the room of the pipes; it stays where it stops, so that the
   frozen puddles and the crate make a row to jump along. */
void scLemurianShipSetDownCrate(void)
{
	fdActorPlaceMovedBlock();
}

/* Nothing visible happens as Felix starts to Move or push the tall crate on the
   ledge by the stairs; the game takes the crate up so that it can slide. */
void scLemurianShipLiftLedgeCrate(void)
{
	fdActorLiftTargetBlock(LemurianShipCrate);
}

/* Felix slides the tall crate along the ledge by the stairs with Move or a
   push. Once it reaches metatile (16, 46), at the ledge's edge, it falls with a
   thump to the floor below and stays there. */
void scLemurianShipPushCrateOffLedge(void)
{
	ACTOR *actor;
	Sint32 x;
	Sint32 moving;

	fdActorPlaceMovedBlock();
	actor = ((ACTOR *)fdActorGetById(ACTOR_LEDGE_CRATE));
	x = FIELD_METATILE(actor->x.fixed);
	if(x == 16) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdMapGridCopyMetatileAttributes(16, 47, 1, 1, x, 46);
		sySynch(1);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		do {
			sySynch(1);
			moving = actor->velY;
		} while(moving != 0);
		fdScriptPlaySnd(SE_THUMP);
		sySynch(10);
		actor->motionFlg = moving;
		fdScriptSetFlag(FLAG_CRATE_DROPPED);
		fdMapGridCopyMetatileAttributes(14, 46, 1, 1, 16, 46);
		fdScriptEnd();
	}
}

/* Felix casts Frost on the puddle a jelly left in the first room below deck: it
   freezes into a pillar of ice he can jump onto, the way to the room's
   chest. */
void scLemurianShipFreezeTwoJellyRoomPuddle(void)
{
	fdFlagBlockApplyFlags(LemurianShipTwoJellyRoomPuddles);
}

/* Felix casts Frost on a puddle a jelly left in the room of six jellies: it
   freezes into a pillar of ice to jump along. */
void scLemurianShipFreezeSixJellyRoomPuddle(void)
{
	fdFlagBlockApplyFlags(LemurianShipSixJellyRoomPuddles);
}

/* Felix casts Frost on the puddle the frightened jelly left beside the lift in
   the room of six jellies: it freezes into ice that keeps the lift up, so that
   stepping onto it no longer drops it. */
void scLemurianShipFreezeLiftPuddle(void)
{
	fdMapGridCopyMetatileAttributes(110, 1, 3, 1, 110, 4);
	fdScriptSetFlag((FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A));
}

/* Felix casts Frost on a puddle a jelly left in the room of the pipes: it
   freezes into a pillar of ice to jump along. */
void scLemurianShipFreezePipeRoomPuddle(void)
{
	fdFlagBlockApplyFlags(LemurianShipPipeRoomPuddles);
}

/* Felix steps onto the grated lift at the top right of the room of six jellies
   from the east, metatile (52, 4), before the engine runs and before its puddle
   is frozen: the lift shudders and sinks, Felix jumps back off, and it comes
   back up with a crunch. While the jelly that fled down it is still unbeaten,
   it rides up on the lift and hops off beside it. */
void scLemurianShipStepOnLift(void)
{
	ACTOR *leader;
	ACTOR *jelly;
	Sint32 jellyBeaten;

	if(!fdScriptGetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
		leader = ((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId));
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 840, 72);
		jellyBeaten =
			fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A));
		if(jellyBeaten) {
			fdScriptPlaySnd(SE_LEMURIAN_SHIP_LIFT);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdMapGridSetShake(0, 2, 1);
			fdScriptWait(20);
			fdMapGridSetShake(0, 0, 0);
			scLemurianShipDrawLiftLowered();
			leader->layer = 2;
			fdScriptWait(20);
			fdActorJump(MainRoutineStateWork.leaderActorId, 4, 0);
			fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
			fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 840, 88);
			fdScriptWait(20);
			fdScriptPlaySnd(SE_CRUNCH);
			scLemurianShipDrawLiftRaised();
			fdScriptWait(20);
			leader->layer = 1;
		} else {
			jelly = ((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_JELLY_A));
			fdActorSetSprPriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2);
			fdActorSetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2.4);
			((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))
				->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorWalkToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 824, 88);
			fdScriptWait(1);
			((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))
				->steeringFlg |= ACTOR_FACES_MOTION;
			fdScriptPlaySnd(SE_LEMURIAN_SHIP_LIFT);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdMapGridSetShake(0, 2, 1);
			fdScriptWait(20);
			fdMapGridSetShake(0, 0, 0);
			scLemurianShipDrawLiftLowered();
			leader->layer = 2;
			fdScriptWait(20);
			fdActorFaceDirection(
				MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST, 20);
			fdActorJump(MainRoutineStateWork.leaderActorId, 4, 0);
			fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
			fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 840, 88);
			fdActorFaceDirectionAndWait(
				MainRoutineStateWork.leaderActorId, FACING_WEST);
			fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
				ACTOR_SIX_JELLY_ROOM_JELLY_A);
			jelly->gravity = toFIXED(1);
			jelly->layer = 2;
			fdActorJump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 4, 0);
			fdActorMoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 832, 74);
			fdActorJump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 4, 0);
			fdActorMoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 856, 88);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdScriptWait(20);
			fdScriptPlaySnd(SE_CRUNCH);
			scLemurianShipDrawLiftRaised();
			fdScriptWait(20);
			fdActorStop(MainRoutineStateWork.leaderActorId);
			leader->layer = 1;
			jelly->layer = 0;
			sySynch(1);
			fdScriptClearFlag(FLAG_JELLY_FLED);
		}
		fdScriptEnd();
	}
}

/* Felix steps onto the grated lift from its west side, metatile (50, 4), before
   the engine runs and before its puddle is frozen: as from the east, the lift
   shudders and sinks, Felix jumps back off, and it comes back up with a
   crunch. */
void scLemurianShipStepOnLiftWestSide(void)
{
	ACTOR *leader;

	if(!fdScriptGetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
		leader = ((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId));
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 812, 72);
		fdScriptPlaySnd(SE_LEMURIAN_SHIP_LIFT);
		fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdMapGridSetShake(0, 2, 1);
		fdScriptWait(20);
		fdMapGridSetShake(0, 0, 0);
		scLemurianShipDrawLiftLowered();
		leader->layer = 2;
		fdScriptWait(20);
		fdActorJump(MainRoutineStateWork.leaderActorId, 4, 0);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 3.2);
		fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 812, 88);
		fdScriptWait(20);
		fdScriptPlaySnd(SE_CRUNCH);
		scLemurianShipDrawLiftRaised();
		fdScriptWait(20);
		leader->layer = 1;
	}
}

/* Defines what talking to an Aqua Jelly does: it turns to Felix and quivers,
   and a battle starts that cannot be run from, its jelly's FLAG_FIGHTING_JELLY
   flag set (the room's first jelly firstJelly is actor firstActor). */
#define DEFINE_JELLY_BATTLE(name, firstJelly, firstActor) \
	void name(Sint32 id) \
	{ \
		fdScriptStart(); \
		fdScriptEndLastingPsynergy(0); \
		fdActorFaceActorById(id, MainRoutineStateWork.leaderActorId, 0); \
		fdActorStartRepeatedMotion(id, 2); \
		fdActorSetAttachedEffect( \
			id, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE); \
		fdScriptWait(20); \
		MainRoutineStateWork.battleType = BATTLE_TYPE_NO_ESCAPE; \
		fdRequestBattleFromEncounterTable( \
			ENCOUNTER_ZONE_AQUA_JELLIES, ENCOUNTER_AQUA_JELLY); \
		syGameFlagSet( \
			id + (FLAG_FIGHTING_JELLY + (firstJelly) - (firstActor))); \
	}

/* Felix talks to an Aqua Jelly in the first room below deck: it turns to him
   and quivers, and a battle starts that cannot be run from. Its
   `FLAG_FIGHTING_JELLY` flag lets a win melt it into a puddle. */
DEFINE_JELLY_BATTLE(LemurianShip_FightTwoJellyRoomJelly, JELLY_TWO_JELLY_ROOM_A,
	ACTOR_TWO_JELLY_ROOM_JELLY_A)

/* Felix talks to an Aqua Jelly in the room of six jellies: it turns to him and
   quivers, and a battle starts that cannot be run from. Its
   `FLAG_FIGHTING_JELLY` flag lets a win melt it into a puddle. */
DEFINE_JELLY_BATTLE(LemurianShip_FightSixJellyRoomJelly, JELLY_SIX_JELLY_ROOM_A,
	ACTOR_SIX_JELLY_ROOM_JELLY_A)

/* Felix talks to an Aqua Jelly in the room of the pipes: it turns to him and
   quivers, and a battle starts that cannot be run from. Its
   `FLAG_FIGHTING_JELLY` flag lets a win melt it into a puddle. */
DEFINE_JELLY_BATTLE(
	LemurianShip_FightPipeRoomJelly, JELLY_PIPE_ROOM_A, ACTOR_PIPE_ROOM_JELLY_A)

/* A jelly in the hold leaps into the middle of it, where the Aqua Hydra
   forms. */
void scLemurianShipLeapIntoHydra(Sint32 id)
{
	ACTOR *actor = ((ACTOR *)fdActorGetById(id));

	fdActorSetSpeed(id, 3.2);
	actor->gravity = toFIXED(1) / 2;
	actor->velY = toFIXED(8);
	fdActorSetDestination(id, 120, 120);
}

/* Felix talks to the jelly in the middle of the hold: the five around it
   quiver, cry out and leap into it, and it swells into the Aqua Hydra, whose
   battle cannot be run from. */
void scLemurianShipFightAquaHydra(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(MUS_SILENCE);
	fdActorStartRepeatedMotion(ACTOR_HOLD_JELLY_B, 2);
	fdActorStartRepeatedMotion(ACTOR_HOLD_JELLY_C, 2);
	fdActorStartRepeatedMotion(ACTOR_HOLD_JELLY_D, 2);
	fdActorStartRepeatedMotion(ACTOR_HOLD_JELLY_E, 2);
	fdActorRunRepeatedMotion(ACTOR_HOLD_JELLY_F, 2);
	fdScriptWait(20);
	fdScriptPlaySnd(SE_BEAST_CRY);
	scLemurianShipLeapIntoHydra(ACTOR_HOLD_JELLY_B);
	scLemurianShipLeapIntoHydra(ACTOR_HOLD_JELLY_C);
	scLemurianShipLeapIntoHydra(ACTOR_HOLD_JELLY_D);
	scLemurianShipLeapIntoHydra(ACTOR_HOLD_JELLY_E);
	scLemurianShipLeapIntoHydra(ACTOR_HOLD_JELLY_F);
	fdActorWaitForMoveById(ACTOR_HOLD_JELLY_F);
	fdActorSetPos(ACTOR_HOLD_JELLY_B, 0, 0);
	fdActorSetPos(ACTOR_HOLD_JELLY_C, 0, 0);
	fdActorSetPos(ACTOR_HOLD_JELLY_D, 0, 0);
	fdActorSetPos(ACTOR_HOLD_JELLY_E, 0, 0);
	fdActorSetPos(ACTOR_HOLD_JELLY_F, 0, 0);
	fdScriptWait(20);
	fdScriptPlaySnd(MUS_AQUA_HYDRA);
	fdActorRunScriptAndWait(ACTOR_HOLD_JELLY_A, LemurianShipHydraForms);

	MainRoutineStateWork.battleType = BATTLE_TYPE_NO_ESCAPE;
	fdScriptSetAfterBattleDestination(
		SCENE_LEMURIAN_SHIP_AQUA_HYDRA, ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_AQUA_JELLIES, ENCOUNTER_AQUA_HYDRA);
	fdScriptSetFlag((FLAG_FIGHTING_JELLY + JELLY_AQUA_HYDRA));
}

/* In the room of two pillars, a pillar full of water that stands on its tile
   marked with a drop of water, metatile (35, 9) for the right one and (29, 9)
   for the left, sinks slowly into the floor with a rumble. Once both have sunk,
   a thump opens the arched doorway to the engine room. */
void scLemurianShipCheckPillarsOnDropTiles(void)
{
	ACTOR *actor;
	ACTOR *leader;
	Sint32 column;
	Sint32 row;
	Uint32 frm;
	Sint32 done;

	done = fdScriptGetFlag(FLAG_RIGHT_PILLAR_SUNK);
	if(!done && fdScriptGetFlag(FLAG_RIGHT_PILLAR_FULL)) {
		actor = ((ACTOR *)fdActorGetById(ACTOR_RIGHT_PILLAR));
		column = FIELD_METATILE(actor->x.fixed);
		if(column == 35) {
			row = FIELD_METATILE(actor->z.fixed);
			if(row == 9) {
				leader = ((ACTOR *)fdActorGetById(
					MainRoutineStateWork.leaderActorId));
				fdScriptStart();
				fdScriptEndLastingPsynergy(0);
				fdScriptWait(20);
				fdScriptPlaySnd(SE_STONE_RUMBLE);
				fdMapGridCopyMetatilesTo(35, 68, 35, 69, 1, 1);
				fdMapGridCopyMetatileAttributes(35, 0, 1, 1, column, row);
				actor->motionFlg = done;
				fdActorSetSprPriority(ACTOR_RIGHT_PILLAR, 3);
				if(actor->z.fixed > leader->z.fixed)
					fdActorSetSprPriority(
						MainRoutineStateWork.leaderActorId, 3);
				for(frm = 0; frm < 128; frm++) {
					actor->y.fixed -= toFIXED(1) / 8;
					sySynch(1);
				}
				fdActorSetAnimation(ACTOR_RIGHT_PILLAR, SINKING_PILLAR_SUNK);
				fdActorSetPos(ACTOR_RIGHT_PILLAR_PUDDLE, 568, 152);
				fdScriptWait(20);
				fdScriptSetFlag(FLAG_RIGHT_PILLAR_SUNK);
				if(actor->z.fixed > leader->z.fixed) {
					fdActorSetSprPriority(
						MainRoutineStateWork.leaderActorId, 2);
					((ACTOR *)fdActorGetById(
						 MainRoutineStateWork.leaderActorId))
						->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
				}
				fdScriptEnd();
			}
		}
	}
	done = fdScriptGetFlag(FLAG_LEFT_PILLAR_SUNK);
	if(!done && fdScriptGetFlag(FLAG_LEFT_PILLAR_FULL)) {
		actor = ((ACTOR *)fdActorGetById(ACTOR_LEFT_PILLAR));
		column = FIELD_METATILE(actor->x.fixed);
		if(column == 29) {
			row = FIELD_METATILE(actor->z.fixed);
			if(row == 9) {
				leader = ((ACTOR *)fdActorGetById(
					MainRoutineStateWork.leaderActorId));
				fdScriptStart();
				fdScriptEndLastingPsynergy(0);
				fdScriptWait(20);
				fdScriptPlaySnd(SE_STONE_RUMBLE);
				fdMapGridCopyMetatilesTo(29, 68, 29, 69, 1, 1);
				fdMapGridCopyMetatileAttributes(35, 0, 1, 1, column, row);
				actor->motionFlg = done;
				fdActorSetSprPriority(ACTOR_LEFT_PILLAR, 3);
				if(actor->z.fixed > leader->z.fixed)
					fdActorSetSprPriority(
						MainRoutineStateWork.leaderActorId, 3);
				for(frm = 0; frm < 128; frm++) {
					actor->y.fixed -= toFIXED(1) / 8;
					sySynch(1);
				}
				fdActorSetAnimation(ACTOR_LEFT_PILLAR, SINKING_PILLAR_SUNK);
				fdActorSetPos(ACTOR_LEFT_PILLAR_PUDDLE, 472, 152);
				fdScriptWait(20);
				fdScriptSetFlag(FLAG_LEFT_PILLAR_SUNK);
				if(actor->z.fixed > leader->z.fixed) {
					fdActorSetSprPriority(
						MainRoutineStateWork.leaderActorId, 2);
					((ACTOR *)fdActorGetById(
						 MainRoutineStateWork.leaderActorId))
						->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
				}
				fdScriptEnd();
			}
		}
	}
	if(fdScriptGetFlag(FLAG_RIGHT_PILLAR_SUNK) &&
		fdScriptGetFlag(FLAG_LEFT_PILLAR_SUNK)) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptPlaySnd(SE_THUMP);
		fdMapGridCopyMetatilesTo(4, 124, 31, 64, 3, 2);
		fdScriptWait(20);
		fdScriptSetFlag(FLAG_PILLAR_WAY_OPEN);
		fdScriptEnd();
	}
}

/* Nothing visible happens as Felix starts to Move or push one of the two
   pillars; the game takes it up so that it can slide. */
void scLemurianShipLiftPillar(void)
{
	fdActorLiftTargetBlock(LemurianShipPillars);
}

/* Felix slides one of the two pillars a metatile with Move or a push. If it is
   full of water and stops on its tile marked with a drop of water, it sinks;
   once both have sunk, the arched doorway opens. */
void scLemurianShipSetDownPillar(void)
{
	fdActorPlaceMovedBlock();
	scLemurianShipCheckPillarsOnDropTiles();
}

/* Defines what Douse on a hollow pillar does: rain fills it, its fullFlag is
   set, and it sinks if it stands on its tile marked with a drop of water. */
#define DEFINE_DOUSE_PILLAR(name, pillar, fullFlag) \
	void name(void) \
	{ \
		sySndPlay(SE_DRIP); \
		fdActorSetAnimationById(pillar, SINKING_PILLAR_FULL); \
		syGameFlagSet(fullFlag); \
		scLemurianShipCheckPillarsOnDropTiles(); \
	}

/* Felix casts Douse on the hollow white pillar on the right of the room of two
   pillars: rain falls into it and it fills with water, ready to sink once it
   stands on the tile marked with a drop of water. */
DEFINE_DOUSE_PILLAR(
	LemurianShip_DouseRightPillar, ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_FULL)

/* Felix casts Douse on the left pillar of the room of two pillars, once it has
   risen again: it fills with water, ready to sink on its tile. */
DEFINE_DOUSE_PILLAR(
	LemurianShip_DouseLeftPillar, ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_FULL)

/* Felix casts Frost on the water in the right pillar after it has sunk: the
   water, unseen until now, shows as it freezes into a pillar of ice. */
void scLemurianShipShowRightPillarPuddle(void)
{
	scFieldActorSetSprCol(
		((ACTOR *)fdActorGetById(ACTOR_RIGHT_PILLAR_PUDDLE)), 0);
}

/* Felix casts Frost on the water in the sunk left pillar, metatile (29, 9): the
   water, unseen until now, shows as it freezes into a pillar of ice above the
   sunk pillar. */
void scLemurianShipShowLeftPillarPuddle(void)
{
	scFieldActorSetSprCol(
		((ACTOR *)fdActorGetById(ACTOR_LEFT_PILLAR_PUDDLE)), 0);
}

/* Defines a sunk pillar rising once its water is gone: its loweredFlag and
   fullFlag are cleared, it rises 16 pixels with a chime and a thump, its
   metatile in column column is solid again, and the arched doorway closes. */
#define DEFINE_RAISE_PILLAR(name, pillar, loweredFlag, fullFlag, column) \
	void name(void) \
	{ \
		FIELD_STATE *state = Ram_HeapSlots->fieldState; \
		ACTOR		*actor = ((ACTOR *)fdActorGetById(pillar)); \
		Uint32		 tick; \
\
		fdScriptStart(); \
		fdScriptEndLastingPsynergy(0); \
		state->raisedCue = 0; \
		syGameFlagClear(loweredFlag); \
		syGameFlagClear(fullFlag); \
		fdScriptWait(20); \
		sySndPlay(SE_SOL_SANCTUM_LIGHT_UP); \
		for(tick = 0; tick <= 63; tick++) { \
			actor->y.fixed += toFIXED(1) / 4; \
			sySynch(1); \
		} \
		fdActorSetSprPriority(pillar, 2); \
		fdMapGridCopyMetatileTilesIndicesRect(4, 123, column, 69, 1, 1); \
		fdMapGridCopyMetatileAttributes(36, 0, 1, 1, column, 9); \
		sySndPlay(SE_THUMP); \
		fdMapGridCopyMetatilesTo(7, 124, 31, 64, 3, 2); \
		syGameFlagClear(FLAG_PILLAR_WAY_OPEN); \
		fdScriptWait(20); \
		fdScriptEnd(); \
	}

/* Felix casts Parch on the water standing in the sunk right pillar, not frozen:
   it dries up in a white flash, and the pillar rises out of the floor with a
   chime and a thump, and the arched doorway closes again. Parch takes any
   unfrozen puddle it finds off the map (fdPsynergyParch), and the room's watch
   (scLemurianShipWatchPillarPuddles) raises the pillar once its puddle is
   gone. */
DEFINE_RAISE_PILLAR(LemurianShip_RaiseRightPillar, ACTOR_RIGHT_PILLAR,
	FLAG_RIGHT_PILLAR_SUNK, FLAG_RIGHT_PILLAR_FULL, 35)

/* Felix casts Parch on the water standing in the sunk left pillar, not frozen:
   it dries up in a white flash, and the pillar rises out of the floor with a
   chime and a thump, and the arched doorway closes again. Parch takes any
   unfrozen puddle it finds off the map (fdPsynergyParch), and the room's watch
   (scLemurianShipWatchPillarPuddles) raises the pillar once its puddle is
   gone. */
DEFINE_RAISE_PILLAR(LemurianShip_RaiseLeftPillar, ACTOR_LEFT_PILLAR,
	FLAG_LEFT_PILLAR_SUNK, FLAG_LEFT_PILLAR_FULL, 29)

/* Felix casts Parch in the flooded hold: the water dries up, and the chest the
   flood had kept out of reach, with its Mist Potion, can be opened.

   The flooded hold is a copy drawn 304 pixels further south; Parch moves the
   party and the chest back to the dry hold, which looks the same without the
   water, and sets the camera's bounds to it. */
void scLemurianShipParchHold(void)
{
	ACTOR *leader =
		((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId));
	ACTOR *actor;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdSetCameraBounds(toFIXED(8), toFIXED(16), toFIXED(248), toFIXED(256));
	fdActorSetPosFX(MainRoutineStateWork.leaderActorId, leader->x.fixed,
		leader->z.fixed - toFIXED(304));
	actor = ((ACTOR *)fdActorGetById(FIELD_TREASURE_ACTOR));
	fdActorSetPosFX(
		FIELD_TREASURE_ACTOR, actor->x.fixed, actor->z.fixed - toFIXED(304));
	scFieldActorSetSprCol(((ACTOR *)fdActorGetById(FIELD_TREASURE_ACTOR)), 0);
	sySynch(10);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
	sySynch(1);
	fdMapGridCopyMetatileAttributes(6, 0, 1, 2, 6, 4);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptSetFlag(FLAG_HOLD_PARCHED);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdScriptEnd();
}

/* Nothing visible happens as Felix begins to cast Parch in the flooded hold;
   the game settles any actions still pending before scLemurianShipParchHold
   dries it. */
void scLemurianShipStartParch(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptEndLastingPsynergy(0);
	fdScriptEnd();
}

/* Defines an actor setup that keeps a frozen puddle flat on the floor, drawn at
   sprite priority priority. */
#define DEFINE_SET_UP_PUDDLE(name, priority) \
	void name(Sint32 id) \
	{ \
		ACTOR *actor = ((ACTOR *)fdActorGetById(id)); \
\
		((ACTOR *)fdActorGetById(id))->priFlg |= ACTOR_PRIORITY_IGNORES_RISES; \
		fdActorSetSprPriority(id, priority); \
		actor->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT; \
		actor->sceneCnt = 1; \
	}

/* Keeps a frozen puddle flat on the floor, drawn at sprite priority 1. */
DEFINE_SET_UP_PUDDLE(scLemurianShipSetUpPuddle, 1)

/* In every edition but the Japanese; in the Japanese release the pillar room's
   puddles also take priority 1. */
#if PILLAR_PUDDLES_LOW_PRIORITY
/* Keeps a frozen puddle flat on the floor, drawn at sprite priority 2. */
DEFINE_SET_UP_PUDDLE(scLemurianShipSetUpLowPuddle, 2)
#endif

/* Keeps the room of six jellies' puddles flat on the floor. */
void scLemurianShipKeepSixJellyRoomPuddles(void)
{
	((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetSprPriority(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, 3);
	((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE))->priFlg &=
		~ACTOR_PRIORITY_UNDERFOOT;
	scLemurianShipSetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_A);
	scLemurianShipSetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_B);
	scLemurianShipSetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_C);
	scLemurianShipSetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_D);
	scLemurianShipSetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_E);
}

/* Keeps the room of the pipes' puddles flat on the floor. */
void scLemurianShipKeepPipeRoomPuddles(void)
{
	scLemurianShipSetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_A);
	scLemurianShipSetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_B);
	scLemurianShipSetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_C);
	scLemurianShipSetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_D);
	scLemurianShipSetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_E);
	scLemurianShipSetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_G);
}

/* Keeps the pillar room's two puddles flat on the floor. */
void scLemurianShipKeepPillarRoomPuddles(void)
{
#if !PILLAR_PUDDLES_LOW_PRIORITY
	scLemurianShipSetUpPuddle(ACTOR_RIGHT_PILLAR_PUDDLE);
	scLemurianShipSetUpPuddle(ACTOR_LEFT_PILLAR_PUDDLE);
#else
	scLemurianShipSetUpLowPuddle(ACTOR_RIGHT_PILLAR_PUDDLE);
	scLemurianShipSetUpLowPuddle(ACTOR_LEFT_PILLAR_PUDDLE);
#endif
}

/* A row of the flooded hold (the hold's copy 19 rows south) on the overhead
   layer. */
#define WADE_ROW(row) ((row) + OVERHEAD_LAYER_ROW)

/* An event: in the flooded hold, draws the water rippling around Felix as he
   wades. Each tick the overhead layer's water over columns 4 to 11 of rows 25
   to 30 is drawn afresh from a plain copy at (74, 105); where he stands on one
   of seven of those metatiles, the ripples' picture at (18, 126) is drawn on
   his metatile, and on all but (7, 26) on the metatile south of it too. */
void scLemurianShipRippleAroundLeader(void)
{
	ACTOR *leader =
		((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId));
	Sint32 x = FIELD_METATILE(leader->x.fixed);
	Sint32 z = FIELD_METATILE(leader->z.fixed);

	fdMapGridCopyMetatilesTo(74, 105, 4, WADE_ROW(25), 8, 6);
	if(z == 25) {
		if(x == 4)
			fdMapGridCopyMetatilesTo(18, 126, 4, WADE_ROW(25), 1, 2);
	} else if(z == 26) {
		if(x == 7)
			fdMapGridCopyMetatilesTo(18, 126, 7, WADE_ROW(26), 1, 1);
	} else if(z == 27) {
		if(x == 4)
			fdMapGridCopyMetatilesTo(18, 126, 4, WADE_ROW(27), 1, 2);
		else if(x == 6)
			fdMapGridCopyMetatilesTo(18, 126, 6, WADE_ROW(27), 1, 2);
		else if(x == 7)
			fdMapGridCopyMetatilesTo(18, 126, 7, WADE_ROW(27), 1, 2);
		else if(x == 9)
			fdMapGridCopyMetatilesTo(18, 126, 9, WADE_ROW(27), 1, 2);
	} else if(z == 29) {
		if(x == 6)
			fdMapGridCopyMetatilesTo(18, 126, 6, WADE_ROW(29), 1, 2);
	}
}

#undef WADE_ROW

/* An event: once the water on a sunk pillar is gone (its puddle is no longer
   placed), the pillar rises again. */
void scLemurianShipWatchPillarPuddles(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*actor;

	if(fdScriptGetFlag(FLAG_RIGHT_PILLAR_SUNK)) {
		actor = ((ACTOR *)fdActorGetById(ACTOR_RIGHT_PILLAR_PUDDLE));
		if((actor->x.fixed | actor->z.fixed) == 0) {
			state->raisedCue = RAISED_RIGHT_PILLAR;
			fdActorSetAnimation(ACTOR_RIGHT_PILLAR, SINKING_PILLAR_EMPTY);
		}
	}
	if(fdScriptGetFlag(FLAG_LEFT_PILLAR_SUNK)) {
		actor = ((ACTOR *)fdActorGetById(ACTOR_LEFT_PILLAR_PUDDLE));
		if((actor->x.fixed | actor->z.fixed) == 0) {
			state->raisedCue = RAISED_LEFT_PILLAR;
			fdActorSetAnimation(ACTOR_LEFT_PILLAR, SINKING_PILLAR_EMPTY);
		}
	}
}

/* An event: as Piers sets the Black Crystal, glints of light drift in from
   around the pedestal toward it. */
void scLemurianShipDrawLightToOrb(void)
{
	Sint32 pos[3];
	ACTOR *actor;

	SET_VECTOR(pos, toFIXED(808), toFIXED(8), toFIXED(140));
	syVectorAddPolarOffset(toFIXED(32), syRandom(), pos);
	actor =
		((ACTOR *)syImportActorCreate(SPRITE_GLINT, pos[0], pos[1], pos[2]));
	if(actor != NULL) {
		actor->motionFlg = 0;
		actor->speedLimit = toFIXED(2);
		actor->acceleration = toFIXED(1) / 2;
		fdActorSetSprFlags(actor, 0);
		fdActorSelectAnimation(actor, 0);
		scFieldActorSetMoveTarget(actor, 808, 8, 140);
		fdActorStartScript(actor, LemurianShipLightToOrb);
	}
}

/* Felix walks into the engine room for the first time: Piers steps up to the
   pedestal, "How did all these monsters get on board?", and sets the Black
   Crystal in it. Light gathers, the engine starts, and the ship sails out to
   sea with every monster aboard gone. */
void scLemurianShipSetBlackCrystal(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgLemurianShipHowDidAllTheseMonsters);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 808, 208);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(808, CAMERA_KEEP, 176, 1);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(
		ACTOR_ENGINE_ROOM_KRADEN, MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetSpeed(ACTOR_ENGINE_ROOM_KRADEN, 0.8);
	fdActorSetScript(ACTOR_SHEBA, LemurianShipShebaStepsUp);
	fdActorSetScript(ACTOR_JENNA, LemurianShipJennaStepsUp);
	fdActorSetScript(ACTOR_ENGINE_ROOM_KRADEN, LemurianShipKradenStepsUp);
	fdActorRunScriptAndWait(ACTOR_PIERS, LemurianShipPiersToPedestal);
	fdScriptWait(20);
	/* "How did all these monsters get on board? I could swear I locked
	   the..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	/* "It doesn't matter. Once we get this ship moving, I'm sure they'll..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	/* "OK, now, I must set the black orb in this pedestal." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorWalkToAndWait(ACTOR_PIERS, 808, 148);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	fdActorSetSprPriority(ACTOR_PIERS, 1);
	fdScriptPlaySnd(SE_SHIMMER);
	sySetEvent(scLemurianShipDrawLightToOrb, EVENT_PASS_TICK);
	fdScriptWait(60);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdScriptWait(20);
	syCloseEvent(scLemurianShipDrawLightToOrb);
	fdScriptPlaySnd(SE_BLACK_CRYSTAL_SET);
	fdSetCameraBounds(toFIXED(688), 0, toFIXED(928), toFIXED(544));
	fdCameraMoveTo(808, CAMERA_KEEP, 448, 0);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 808, 480);
	fdActorSetPos(ACTOR_PIERS, 808, 420);
	fdActorSetPos(ACTOR_SHEBA, 784, 468);
	fdActorSetPos(ACTOR_JENNA, 832, 468);
	fdActorSetPos(ACTOR_ENGINE_ROOM_KRADEN, 792, 488);
	fdMapGridCenterOnFollowedPoint();
	fdScriptWait(20);
	fdScriptPlaySnd(SE_GLITTER);
	fdScriptWait(80);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmPartyInventoryRemove(ITEM_BLACK_CRYSTAL);
	fdScriptSetFlag(FLAG_SHIP_ENGINE_RUNNING);
	fdScriptSetFlag(FLAG_JELLIES_BEATEN);
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_TWO_JELLY_ROOM_B));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_B));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_C));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_D));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_E));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_F));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_A));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_B));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_C));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_D));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_E));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_G));
	fdScriptSetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A));
	fdScriptSetFlag(FLAG_AQUA_HYDRA_BEATEN);
	MainRoutineStateWork.ownerOverride = 1;
	syImportScriptSetExitRequest(LEMURIAN_SHIP_AQUA_HYDRA_TO_WORLD_MAP);
}

/* Back from a battle with an Aqua Jelly, the jelly melts away where it stood
   and a puddle spreads in its place. */
void scLemurianShipMeltJellyIntoPuddle(Sint32 jelly, Sint32 puddle)
{
	ACTOR *actor = ((ACTOR *)fdActorGetById(puddle));

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	actor->scaleX = toFIXED(1) / 16;
	actor->scaleY = toFIXED(1) / 16;
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdScriptPlaySnd(SE_CURSE);
	fdActorSetScript(jelly, LemurianShipJellyMelts);
	fdActorRunScriptAndWait(puddle, LemurianShipPuddleSpreads);
	fdScriptWait(20);
	fdScriptEnd();
}

/* Felix comes into the room of six jellies while the jelly by the lift is
   unbeaten: it takes fright, jumps and flees down the lift. */
void scLemurianShipScareJellyOnArrival(void)
{
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A)) == 0) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptWait(20);
		scLemurianShipJellyFlees();
		fdScriptEnd();
	}
}

/* Felix steps near the jelly by the lift, metatile (54, 5), while it is
   unbeaten and back up: it takes fright and flees down the lift again. */
void scLemurianShipScareJelly(void)
{
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A)) == 0) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		scLemurianShipJellyFlees();
		fdScriptEnd();
	}
}

/* The jelly by the lift startles, jumps and flees down the lift. */
void scLemurianShipJellyFlees(void)
{
	ACTOR *actor = ((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_JELLY_A));

	scFieldActorShowEmote(
		ACTOR_SIX_JELLY_ROOM_JELLY_A, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2);
	actor->gravity = toFIXED(1) / 2;
	fdActorSetSprPriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 1);
	fdActorSetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 0.8);
	fdActorJump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 6, 0);
	fdActorMoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 832, 88);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 1.6);
	fdActorSetSprPriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 3);
	fdActorMoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 824, 72);
	fdActorFaceDirection(
		ACTOR_SIX_JELLY_ROOM_JELLY_A, FACING_SOUTH_SOUTHEAST, 40);
	fdScriptSetFlag(FLAG_JELLY_FLED);
}

/* Back from beating the Aqua Hydra, the party stands in the flooded hold; a
   sound plays and the game waits two seconds before Felix can move. */
void scLemurianShipFloodHold(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_SCATTER);
	fdScriptWait(120);
	fdScriptEnd();
}

void scLemurianShipSetUpTwoJellyRoom(void);

Sint32 scInit(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	if(fdScriptGetFlag(FLAG_SHIP_AT_ATTEKA_INLET))
		fdScriptSetFlag(FLAG_RETREAT_BLOCKED);
	if(MainRoutineStateWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
		switch(MainRoutineStateWork.entrance) {
		case ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM:
		case ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM:
		case ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL:
			scLemurianShipSetUpTwoJellyRoom();
			break;
		case ENTRANCE_STAIR_ROOM_FROM_TWO_JELLY_ROOM:
		case ENTRANCE_STAIR_ROOM_FROM_STAIR_ROOM_HALL:
		case LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A:
			scLemurianShipSetUpStairRoom();
			break;
		case ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST:
		case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST:
			scLemurianShipSetUpSixJellyRoom();
			break;
		case LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A:
		case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST:
		case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST:
		case ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM:
			scLemurianShipSetUpLedgeRoom();
			break;
		case ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL:
		case ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM:
			scLemurianShipSetUpPipeRoom();
			break;
		}
	} else if(MainRoutineStateWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
		switch(MainRoutineStateWork.entrance) {
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C:
		case ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_B:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_B:
			scLemurianShipSetUpHold();
			break;
		case ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR:
		case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_EAST:
			scLemurianShipSetUpPillarRoom();
			break;
		case ENTRANCE_ENGINE_ROOM_FROM_CORRIDOR:
			scLemurianShipSetUpEngineRoom();
			break;
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	Uint32 i;

	for(i = 0; i < JELLY_COUNT; i++) {
		if(fdScriptGetFlag(FLAG_FIGHTING_JELLY + i)) {
			MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;

			if(state->modeResult == 1) {
				fdScriptSetFlag(FLAG_JELLIES_BEATEN + i);
				fdScriptSetFlag(FLAG_JELLY_MELTING + i);
			}
		}
		fdScriptClearFlag(FLAG_FIGHTING_JELLY + i);
	}
	return 0;
}

/* The first room below deck: its two jellies, or the puddles they left, and a
   jelly just beaten melting into its puddle. */
void scLemurianShipSetUpTwoJellyRoom(void)
{
	Sint32 flg;

	if(fdScriptGetFlag(FLAG_JELLIES_BEATEN)) {
		flg = fdScriptGetFlag(FLAG_JELLY_MELTING);
		if(flg != 0) {
			fdMapGridCopyMetatileAttributes(0, 25, 1, 3, 6, 25);
			fdActorSetPosAndResetMotion(
				((ACTOR *)fdActorGetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A)),
				toFIXED(104), 0, toFIXED(424));
			((ACTOR *)fdActorGetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A))
				->groundHeight = 0;
		} else {
			fdActorSetPos(ACTOR_TWO_JELLY_ROOM_JELLY_A, 0, 0);
			fdMapGridCopyMetatileAttributes(0, 25, 1, 3, 6, 25);
			scFieldActorSetPosAndResetMotion(
				((ACTOR *)fdActorGetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A)),
				toFIXED(104), 0, toFIXED(424));
			((ACTOR *)fdActorGetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A))
				->groundHeight = 0;
		}
		sySynch(1);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_TWO_JELLY_ROOM_B))) {
		if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_TWO_JELLY_ROOM_B)))
			fdActorSetSprPriority(ACTOR_TWO_JELLY_ROOM_JELLY_B, 1);
		else
			fdActorSetPos(ACTOR_TWO_JELLY_ROOM_JELLY_B, 0, 0);
		scFieldActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_B)),
			toFIXED(104), -toFIXED(32), toFIXED(536));
		((ACTOR *)fdActorGetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_B))->groundHeight =
			-toFIXED(32);
		fdActorSetSprPriority(ACTOR_TWO_JELLY_ROOM_PUDDLE_B, 1);
		sySynch(1);
	}
	fdFlagBlockPlaceAll(LemurianShipTwoJellyRoomPuddles);
	if(fdScriptGetFlag(FLAG_JELLY_MELTING)) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_TWO_JELLY_ROOM_JELLY_A, ACTOR_TWO_JELLY_ROOM_PUDDLE_A);
		fdScriptClearFlag(FLAG_JELLY_MELTING);
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_TWO_JELLY_ROOM_B))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_TWO_JELLY_ROOM_JELLY_B, ACTOR_TWO_JELLY_ROOM_PUDDLE_B);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_TWO_JELLY_ROOM_B));
	}
}

/* Sets a jelly up on the room of six jellies' lower floor, 32 pixels down. */
void scLemurianShipSetUpLowerFloorJelly(Sint32 id)
{
	ACTOR *actor = ((ACTOR *)fdActorGetById(id));

	((ACTOR *)fdActorGetById(id))->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetSprPriority(id, 1);
	actor->motionFlg = 0;
	actor->y.fixed = -toFIXED(32);
	actor->groundHeight = -toFIXED(32);
}

/* Places a puddle on the lower floor, where its jelly sat. */
#define PLACE_ON_LOWER_FLOOR(id, x, z) \
	{ \
		scFieldActorSetPosAndResetMotion( \
			((ACTOR *)fdActorGetById(id)), x, -toFIXED(32), z); \
		((ACTOR *)fdActorGetById(id))->groundHeight = -toFIXED(32); \
	}

/* The room of six jellies: its crate, its jellies, and the puddles of those
   beaten. */
void scLemurianShipSetUpSixJellyRoom(void)
{
	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorPlaceBlocks(LemurianShipCrate);
	((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_CRATE))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetSprPriority(ACTOR_SIX_JELLY_ROOM_CRATE, 1);
	((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A))) {
		ACTOR *actor =
			((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE));

		actor->motionFlg = 0;
		scFieldActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE)),
			toFIXED(824), -toFIXED(32), toFIXED(72));
		actor->groundHeight = -toFIXED(32);
		if(fdScriptGetFlag((FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A)))
			fdMapGridCopyMetatileAttributes(110, 1, 3, 1, 110, 4);
		else
			fdMapGridCopyMetatileAttributes(110, 0, 3, 1, 110, 4);
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_A)))
			fdActorSetPos(ACTOR_SIX_JELLY_ROOM_JELLY_A, 0, 0);
	}
	if(fdScriptGetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
		if(fdScriptGetFlag(FLAG_LIFT_LOWERED)) {
			fdActorSetPos(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, 0, 0);
			scLemurianShipDrawLiftLowered();
			fdMapGridCopyMetatileAttributes(102, 16, 3, 2, 110, 4);
			fdMapGridCopyMetatileAttributes(50, 0, 3, 1, 50, 4);
		} else {
			PLACE_ON_LOWER_FLOOR(
				ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, toFIXED(824), toFIXED(72))
			scLemurianShipDrawLiftRaised();
			fdMapGridCopyMetatileAttributes(106, 16, 3, 2, 110, 4);
			fdMapGridCopyMetatileAttributes(50, 1, 3, 1, 50, 4);
			if(fdScriptGetFlag((FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A)))
				fdMapGridCopyMetatileAttributes(110, 1, 3, 1, 110, 4);
			else
				fdMapGridCopyMetatileAttributes(110, 0, 3, 1, 110, 4);
		}
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_B))) {
		fdMapGridCopyMetatileAttributes(42, 0, 1, 1, 46, 5);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 45, 5);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 47, 5);
		PLACE_ON_LOWER_FLOOR(
			ACTOR_SIX_JELLY_ROOM_PUDDLE_A, toFIXED(744), toFIXED(88))
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_B)))
			fdActorSetPos(ACTOR_SIX_JELLY_ROOM_JELLY_B, 0, 0);
	} else {
		scLemurianShipSetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_B);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_C))) {
		ACTOR *block = ((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_CRATE));

		fdMapGridCopyMetatileAttributes(42, 0, 1, 1, 47, 9);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 46, 9);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 48, 9);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 47, 10);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 47, 8);
		PLACE_ON_LOWER_FLOOR(
			ACTOR_SIX_JELLY_ROOM_PUDDLE_B, toFIXED(760), toFIXED(152))
		if(FIELD_METATILE(block->x.fixed) == 47 &&
			FIELD_METATILE(block->z.fixed) == 8)
			fdActorPlaceBlocks(LemurianShipCrate);
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_C)))
			fdActorSetPos(ACTOR_SIX_JELLY_ROOM_JELLY_C, 0, 0);
	} else {
		scLemurianShipSetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_C);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_D))) {
		ACTOR *block = ((ACTOR *)fdActorGetById(ACTOR_SIX_JELLY_ROOM_CRATE));
		Sint32 x = FIELD_METATILE(block->x.fixed);
		Sint32 z = FIELD_METATILE(block->z.fixed);

		fdMapGridCopyMetatileAttributes(42, 0, 1, 1, 49, 7);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 49, 6);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 50, 7);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 48, 7);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 49, 8);
		PLACE_ON_LOWER_FLOOR(
			ACTOR_SIX_JELLY_ROOM_PUDDLE_C, toFIXED(792), toFIXED(120))
		if((x == 48 && z == 7) || (x == 49 && z == 8))
			fdActorPlaceBlocks(LemurianShipCrate);
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_D)))
			fdActorSetPos(ACTOR_SIX_JELLY_ROOM_JELLY_D, 0, 0);
	} else {
		scLemurianShipSetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_D);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_E))) {
		fdMapGridCopyMetatileAttributes(42, 0, 1, 1, 51, 7);
		PLACE_ON_LOWER_FLOOR(
			ACTOR_SIX_JELLY_ROOM_PUDDLE_D, toFIXED(824), toFIXED(120))
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_E)))
			fdActorSetPos(ACTOR_SIX_JELLY_ROOM_JELLY_E, 0, 0);
	} else {
		scLemurianShipSetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_E);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_F))) {
		fdMapGridCopyMetatileAttributes(42, 0, 1, 1, 51, 9);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 51, 8);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 50, 9);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 52, 9);
		fdMapGridCopyMetatileAttributes(43, 0, 1, 1, 51, 10);
		PLACE_ON_LOWER_FLOOR(
			ACTOR_SIX_JELLY_ROOM_PUDDLE_E, toFIXED(824), toFIXED(152))
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_F)))
			fdActorSetPos(ACTOR_SIX_JELLY_ROOM_JELLY_F, 0, 0);
	} else {
		scLemurianShipSetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_F);
	}
	fdFlagBlockPlaceAll(LemurianShipSixJellyRoomPuddles);
	sySetEvent(scLemurianShipKeepSixJellyRoomPuddles, EVENT_PASS_TICK);
	if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_A))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_SIX_JELLY_ROOM_JELLY_A, ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_A));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_B))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_SIX_JELLY_ROOM_JELLY_B, ACTOR_SIX_JELLY_ROOM_PUDDLE_A);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_B));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_C))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_SIX_JELLY_ROOM_JELLY_C, ACTOR_SIX_JELLY_ROOM_PUDDLE_B);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_C));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_D))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_SIX_JELLY_ROOM_JELLY_D, ACTOR_SIX_JELLY_ROOM_PUDDLE_C);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_D));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_E))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_SIX_JELLY_ROOM_JELLY_E, ACTOR_SIX_JELLY_ROOM_PUDDLE_D);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_E));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_F))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_SIX_JELLY_ROOM_JELLY_F, ACTOR_SIX_JELLY_ROOM_PUDDLE_E);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_F));
	}
	if(MainRoutineStateWork.entrance ==
			ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL &&
		!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
		scLemurianShipScareJellyOnArrival();
}

/* An event: draws the ledge crate at the leader's sprite priority. */
void scLemurianShipMatchCratePriority(void)
{
	ACTOR *leader =
		((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId));

	fdActorSetSprPriority(ACTOR_LEDGE_CRATE, ((FIELD_SPR *)leader->spr)->pri);
}

/* The room of the crate on the ledge. Flag 0x8, which would give metatile (16,
   46) the ledge floor of metatile (14, 46) as the crate's drop does, is set by
   no code of the game, so on entering the room that metatile is always the
   map's own, 32 pixels down beside the ledge's edge. */
void scLemurianShipSetUpLedgeRoom(void)
{
	if(fdScriptGetFlag(0x8))
		fdMapGridCopyMetatileAttributes(14, 46, 1, 1, 16, 46);
	fdActorPlaceBlocks(LemurianShipCrate);
	sySetEvent(scLemurianShipMatchCratePriority, EVENT_PASS_TICK);
}

/* Sets a jelly up on the floor. */
void scLemurianShipSetUpJelly(Sint32 id)
{
	ACTOR *actor = ((ACTOR *)fdActorGetById(id));

	((ACTOR *)fdActorGetById(id))->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetSprPriority(id, 1);
	actor->motionFlg = 0;
	actor->y.fixed = 0;
	actor->groundHeight = 0;
}

/* The room of the pipes: each beaten one leaves a puddle and opens its
   metatile. */
void scLemurianShipSetUpPipeRoom(void)
{
	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorPlaceBlocks(LemurianShipCrate);
	((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_CRATE))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetSprPriority(ACTOR_PIPE_ROOM_CRATE, 1);
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_A))) {
		fdMapGridCopyMetatileAttributes(44, 0, 1, 1, 48, 32);
		fdActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_A)), toFIXED(776),
			0, toFIXED(520));
		((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_A))->groundHeight = 0;
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_A)))
			fdActorSetPos(ACTOR_PIPE_ROOM_JELLY_A, 0, 0);
	} else {
		scLemurianShipSetUpJelly(ACTOR_PIPE_ROOM_JELLY_A);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_B))) {
		fdMapGridCopyMetatileAttributes(44, 0, 1, 1, 53, 32);
		fdActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_B)), toFIXED(856),
			0, toFIXED(520));
		((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_B))->groundHeight = 0;
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_B)))
			fdActorSetPos(ACTOR_PIPE_ROOM_JELLY_B, 0, 0);
	} else {
		scLemurianShipSetUpJelly(ACTOR_PIPE_ROOM_JELLY_B);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_C))) {
		fdMapGridCopyMetatileAttributes(44, 0, 1, 1, 43, 37);
		fdActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_C)), toFIXED(696),
			0, toFIXED(600));
		((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_C))->groundHeight = 0;
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_C)))
			fdActorSetPos(ACTOR_PIPE_ROOM_JELLY_C, 0, 0);
	} else {
		scLemurianShipSetUpJelly(ACTOR_PIPE_ROOM_JELLY_C);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_D))) {
		fdMapGridCopyMetatileAttributes(44, 0, 1, 1, 48, 37);
		fdActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_D)), toFIXED(776),
			0, toFIXED(600));
		((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_D))->groundHeight = 0;
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_D)))
			fdActorSetPos(ACTOR_PIPE_ROOM_JELLY_D, 0, 0);
	} else {
		scLemurianShipSetUpJelly(ACTOR_PIPE_ROOM_JELLY_D);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_E))) {
		fdMapGridCopyMetatileAttributes(44, 0, 1, 1, 50, 37);
		fdActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_E)), toFIXED(808),
			0, toFIXED(600));
		((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_E))->groundHeight = 0;
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_E)))
			fdActorSetPos(ACTOR_PIPE_ROOM_JELLY_E, 0, 0);
	} else {
		scLemurianShipSetUpJelly(ACTOR_PIPE_ROOM_JELLY_E);
	}
	if(fdScriptGetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_G))) {
		fdMapGridCopyMetatileAttributes(44, 0, 1, 1, 48, 43);
		fdActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_G)), toFIXED(776),
			0, toFIXED(696));
		((ACTOR *)fdActorGetById(ACTOR_PIPE_ROOM_PUDDLE_G))->groundHeight = 0;
		if(!fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_G)))
			fdActorSetPos(ACTOR_PIPE_ROOM_JELLY_G, 0, 0);
	} else {
		scLemurianShipSetUpJelly(ACTOR_PIPE_ROOM_JELLY_G);
	}
	fdFlagBlockPlaceAll(LemurianShipPipeRoomPuddles);
	syFieldSetTickEvent(scLemurianShipKeepPipeRoomPuddles, EVENT_PASS_TICK);
	if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_A))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_PIPE_ROOM_JELLY_A, ACTOR_PIPE_ROOM_PUDDLE_A);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_A));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_B))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_PIPE_ROOM_JELLY_B, ACTOR_PIPE_ROOM_PUDDLE_B);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_B));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_C))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_PIPE_ROOM_JELLY_C, ACTOR_PIPE_ROOM_PUDDLE_C);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_C));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_D))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_PIPE_ROOM_JELLY_D, ACTOR_PIPE_ROOM_PUDDLE_D);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_D));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_E))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_PIPE_ROOM_JELLY_E, ACTOR_PIPE_ROOM_PUDDLE_E);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_E));
	} else if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_G))) {
		scLemurianShipMeltJellyIntoPuddle(
			ACTOR_PIPE_ROOM_JELLY_G, ACTOR_PIPE_ROOM_PUDDLE_G);
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_G));
	}
}

/* The stair room: once the engine runs, its stairs are open. */
void scLemurianShipSetUpStairRoom(void)
{
	if(fdScriptGetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
		fdMapGridCopyMetatilesTo(17, 125, 27, 63, 3, 3);
		fdMapGridCopyMetatileAttributes(25, 0, 5, 3, 25, 4);
	}
}

/* The hold of the Aqua Hydra: once it is beaten the hold is flooded, and once
   Parch has dried it the chest lies on dry floor. */
void scLemurianShipSetUpHold(void)
{
	ACTOR *actor;

	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	sySetEvent(scLemurianShipRippleAroundLeader, EVENT_PASS_TICK);
	if(fdScriptGetFlag(FLAG_AQUA_HYDRA_BEATEN)) {
		actor = ((ACTOR *)fdActorGetById(FIELD_TREASURE_ACTOR));
		actor->motionFlg = 0;
		fdActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(FIELD_TREASURE_ACTOR)), toFIXED(136),
			-toFIXED(32), toFIXED(392));
		actor->groundHeight = -toFIXED(16);
		fdActorSetSprPriority(FIELD_TREASURE_ACTOR, 3);
		scFieldActorSetSprCol(
			((ACTOR *)fdActorGetById(FIELD_TREASURE_ACTOR)), 1);
		fdActorSetPos(ACTOR_HOLD_JELLY_A, 0, 0);
		fdActorSetPos(ACTOR_HOLD_JELLY_B, 0, 0);
		fdActorSetPos(ACTOR_HOLD_JELLY_C, 0, 0);
		fdActorSetPos(ACTOR_HOLD_JELLY_D, 0, 0);
		fdActorSetPos(ACTOR_HOLD_JELLY_E, 0, 0);
		fdActorSetPos(ACTOR_HOLD_JELLY_F, 0, 0);
		sySynch(1);
	}
	if(fdScriptGetFlag(FLAG_HOLD_PARCHED)) {
		ACTOR *chest = ((ACTOR *)fdActorGetById(FIELD_TREASURE_ACTOR));

		fdActorSetPosFX(FIELD_TREASURE_ACTOR, chest->x.fixed,
			chest->z.fixed - toFIXED(304));
		scFieldActorSetSprCol(
			((ACTOR *)fdActorGetById(FIELD_TREASURE_ACTOR)), 0);
		fdMapGridCopyMetatileAttributes(6, 0, 1, 2, 6, 4);
		fdMapGridSetCameraBounds(
			toFIXED(8), toFIXED(16), toFIXED(248), toFIXED(256));
		fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	}
	if(fdScriptGetFlag((FLAG_JELLY_MELTING + JELLY_AQUA_HYDRA))) {
		scLemurianShipFloodHold();
		fdScriptClearFlag((FLAG_JELLY_MELTING + JELLY_AQUA_HYDRA));
	}
}

/* The room of the two pillars Douse fills, as the player left them. */
void scLemurianShipSetUpPillarRoom(void)
{
	ACTOR *actor;

	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorPlaceBlocks(LemurianShipPillars);
	if(fdScriptGetFlag(FLAG_PILLAR_WAY_OPEN)) {
		fdMapGridCopyMetatilesTo(4, 124, 31, 64, 3, 2);
		fdScriptSetFlag(FLAG_RIGHT_PILLAR_SUNK);
		fdScriptSetFlag(FLAG_LEFT_PILLAR_SUNK);
	}
	fdActorSetSprFlags(((ACTOR *)fdActorGetById(ACTOR_RIGHT_PILLAR)), 0);
	if(fdScriptGetFlag(FLAG_RIGHT_PILLAR_SUNK)) {
		actor = ((ACTOR *)fdActorGetById(ACTOR_RIGHT_PILLAR));
		fdMapGridCopyMetatilesTo(35, 68, 35, 69, 1, 1);
		fdMapGridCopyMetatileAttributes(35, 0, 1, 1, 35, 9);
		actor->motionFlg = 0;
		scFieldActorSetPosAndResetMotion(
			actor, toFIXED(568), -toFIXED(16), toFIXED(152));
		fdActorSetSprPriority(ACTOR_RIGHT_PILLAR, 3);
		fdScriptSetFlag(FLAG_RIGHT_PILLAR_FULL);
		fdActorSetPos(ACTOR_RIGHT_PILLAR_PUDDLE, 568, 152);
	}
	if(fdScriptGetFlag(FLAG_RIGHT_PILLAR_FULL)) {
		if(fdScriptGetFlag(FLAG_RIGHT_PILLAR_SUNK))
			fdActorSetAnimation(ACTOR_RIGHT_PILLAR, SINKING_PILLAR_SUNK);
		else
			fdActorSetAnimation(ACTOR_RIGHT_PILLAR, SINKING_PILLAR_FULL);
	}
	fdActorSetSprFlags(((ACTOR *)fdActorGetById(ACTOR_LEFT_PILLAR)), 0);
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
		fdScriptSetFlag(FLAG_LEFT_PILLAR_SUNK);
	if(fdScriptGetFlag(FLAG_LEFT_PILLAR_SUNK)) {
		actor = ((ACTOR *)fdActorGetById(ACTOR_LEFT_PILLAR));
		fdMapGridCopyMetatilesTo(29, 68, 29, 69, 1, 1);
		fdMapGridCopyMetatileAttributes(35, 0, 1, 1, 29, 9);
		actor->motionFlg = 0;
		actor->y.fixed = -toFIXED(16);
		fdActorSetSprPriority(ACTOR_LEFT_PILLAR, 3);
		fdScriptSetFlag(FLAG_LEFT_PILLAR_FULL);
		fdActorSetPos(ACTOR_LEFT_PILLAR_PUDDLE, 472, 152);
	}
	if(fdScriptGetFlag(FLAG_LEFT_PILLAR_FULL)) {
		if(fdScriptGetFlag(FLAG_LEFT_PILLAR_SUNK))
			fdActorSetAnimation(ACTOR_LEFT_PILLAR, SINKING_PILLAR_SUNK);
		else
			fdActorSetAnimation(ACTOR_LEFT_PILLAR, SINKING_PILLAR_FULL);
	}
	sySynch(1);
	fdFlagBlockPlaceAll(LemurianShipTwoJellyRoomPuddles);
	syFieldSetTickEvent(scLemurianShipKeepPillarRoomPuddles, EVENT_PASS_TICK);
	if(!fdScriptGetFlag(FLAG_RIGHT_PILLAR_PUDDLE_FROZEN))
		scFieldActorSetSprCol(
			((ACTOR *)fdActorGetById(ACTOR_RIGHT_PILLAR_PUDDLE)),
			SPRITE_COLOR_HIDDEN);
	if(!fdScriptGetFlag(FLAG_LEFT_PILLAR_PUDDLE_FROZEN))
		scFieldActorSetSprCol(
			((ACTOR *)fdActorGetById(ACTOR_LEFT_PILLAR_PUDDLE)),
			SPRITE_COLOR_HIDDEN);
	sySetEvent(scLemurianShipWatchPillarPuddles, EVENT_PASS_TICK);
}

/* The engine room: until the engine runs, Piers sets the Black Crystal as the
   party arrives. */
void scLemurianShipSetUpEngineRoom(void)
{
	if(fdScriptGetFlag(FLAG_SHIP_ENGINE_RUNNING) == 0)
		scLemurianShipSetBlackCrystal();
}

/* clang-format off */
#include "shared/leader_walk_through_door_script.c"

/* A beaten jelly melting away: it sinks flat over a second and is put away. */
Sint32 LemurianShipJellyMelts[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 128),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

/* The puddle spreading where the jelly melted. */
Sint32 LemurianShipPuddleSpreads[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 64),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 64),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_END,
};

/* The Aqua Hydra forming out of the gathered jellies, growing in pulses. */
Sint32 LemurianShipHydraForms[] = {
	SC_ADD(SW_SCALE_X, toFIXED(3) / 16),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 16),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_X, -toFIXED(3) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(3) / 32),
	SC_WAIT(4),
	SC_LOOP(8, SC_TOP),
	SC_END,
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 128),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 128),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_END,
};

/* Piers walking up to the Black Crystal's pedestal. */
Uint32 LemurianShipPiersToPedestal[] = {
	SC_ANIMATION(ANIM_WALK), SC_MOVE(808, 0, 168), SC_SYNC,
	SC_ANIMATION(ANIM_STAND), SC_END,
};

/* Sheba stepping up beside Piers. */
Uint32 LemurianShipShebaStepsUp[] = {
	SC_ANIMATION(ANIM_WALK), SC_MOVE(784, 0, 196), SC_SYNC,
	SC_ANIMATION(ANIM_STAND), SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(10), SC_END,
};

/* Jenna stepping up beside Piers. */
Uint32 LemurianShipJennaStepsUp[] = {
	SC_ANIMATION(ANIM_WALK), SC_MOVE(832, 0, 196), SC_SYNC,
	SC_ANIMATION(ANIM_STAND), SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10), SC_END,
};

/* Kraden stepping up beside Piers. */
Uint32 LemurianShipKradenStepsUp[] = {
	SC_ANIMATION(ANIM_WALK), SC_MOVE(792, 0, 216), SC_SYNC,
	SC_ANIMATION(ANIM_STAND), SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(10), SC_END,
};

/* The crate Move and a push take up, by actor id. */
Sint16 LemurianShipCrate[] = {
	ACTOR_FIRST_PLACED, -1,
};

/* The room of two jellies' puddles, by actor id and flag. */
Sint16 LemurianShipTwoJellyRoomPuddles[] = {
	ACTOR_TWO_JELLY_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_A,
	ACTOR_TWO_JELLY_ROOM_PUDDLE_B, FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_B,
	-1,
};

/* The room of six jellies' puddles, by actor id and flag. */
Sint16 LemurianShipSixJellyRoomPuddles[] = {
	ACTOR_SIX_JELLY_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_B,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_B, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_C,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_C, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_D,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_D, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_E,
	ACTOR_SIX_JELLY_ROOM_PUDDLE_E, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_F,
	-1,
};

/* The room of the pipes' puddles, by actor id and flag. */
Sint16 LemurianShipPipeRoomPuddles[] = {
	ACTOR_PIPE_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_A,
	ACTOR_PIPE_ROOM_PUDDLE_B, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_B,
	ACTOR_PIPE_ROOM_PUDDLE_C, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_C,
	ACTOR_PIPE_ROOM_PUDDLE_D, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_D,
	ACTOR_PIPE_ROOM_PUDDLE_E, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_E,
	ACTOR_PIPE_ROOM_PUDDLE_G, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_G,
	-1,
};

/* The two pillars Move and a push take up, by actor id. */
Sint16 LemurianShipPillars[] = {
	ACTOR_RIGHT_PILLAR, ACTOR_LEFT_PILLAR, -1,
};

/* Two halfwords of a scene table row the header declares as words, the first */
/* in the low half. */
#define HALFWORDS(low, high) ((Sint32)(((Uint32)(high) << 16) | (Uint16)(low)))
/* A SCENE_ENTRANCE row as six words: the entrance, at (x, y, z) facing */
/* facing, its camera box from (left, top) to (right, bottom). */
#define ENTRANCE_WORDS(entrance, x, y, z, facing, left, top, right, bottom)    \
  HALFWORDS(entrance, CONDITION_ALWAYS), HALFWORDS(x, y),                      \
	  HALFWORDS(z, facing), HALFWORDS(0, left), HALFWORDS(top, right),         \
	  HALFWORDS(bottom, 0)
/* The SCENE_TABLE_END row of an entrance table, as six words. */
#define ENTRANCE_WORDS_END HALFWORDS(SCENE_TABLE_END, 0), 0, 0, 0, 0, 0
/* A SCENE_REGION row as four words: the box from (minX, minY, minZ) to */
/* (maxX, maxY, maxZ) that leaves by exit. */
#define REGION_WORDS(minX, minY, minZ, maxX, maxY, maxZ, exit)                 \
  HALFWORDS(minX, minY), HALFWORDS(minZ, maxX), HALFWORDS(maxY, maxZ),         \
	  HALFWORDS(CONDITION_ALWAYS, exit)
/* The SCENE_TABLE_END row of a region table, as four words. */
#define REGION_WORDS_END HALFWORDS(SCENE_TABLE_END, 0), 0, 0, 0

/* The entrances of neither scene. */
Sint32 LemurianShipEntrances[] = {
	ENTRANCE_WORDS(0, 172, 0, 149, FACING_SOUTH, -1, -1, -1, -1),
	ENTRANCE_WORDS_END,
};

/* The cabins' entrances. */
Uint32 LemurianShipCabinsEntrances[] = {
	ENTRANCE_WORDS(LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, 406, 0, 376, FACING_NORTH, 336, 264, 616, 424),
	ENTRANCE_WORDS(ENTRANCE_SIX_JELLY_ROOM_WEST_HALL_FROM_SIX_JELLY_ROOM, 550, 0, 377, FACING_NORTH, 336, 264, 616, 424),
	ENTRANCE_WORDS(ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_SIX_JELLY_ROOM, 566, 0, 889, FACING_NORTH, 488, 776, 728, 936),
	ENTRANCE_WORDS(ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_PIPE_ROOM, 648, 0, 894, FACING_NORTH, 488, 776, 728, 936),
	ENTRANCE_WORDS(ENTRANCE_TWO_JELLY_ROOM_HALL_FROM_TWO_JELLY_ROOM, 440, 0, 216, FACING_EAST, 384, 128, 624, 288),
	ENTRANCE_WORDS(LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, 567, 0, 215, FACING_SOUTH, 384, 128, 624, 288),
	ENTRANCE_WORDS_END,
};

/* The lower deck's entrances. */
SCENE_ENTRANCE LemurianShipAquaHydraEntrances[] = {
	{0, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, 8, 16, 248, 256, 0},
	{LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A, CONDITION_ALWAYS, 168, 0, 72, FACING_SOUTH, 0, 8, 16, 248, 256, 0},
	{LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A, CONDITION_ALWAYS, 72, 0, 216, FACING_NORTH, 0, 8, 16, 248, 256, 0},
	{LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A, CONDITION_ALWAYS, 184, 0, 216, FACING_NORTH, 0, 8, 16, 248, 256, 0},
	{LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST, CONDITION_ALWAYS, 72, 0, 376, FACING_SOUTH, 0, 8, 320, 248, 560, 0},
	{LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A, CONDITION_ALWAYS, 168, 0, 376, FACING_SOUTH, 0, 8, 320, 248, 560, 0},
	{LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B, CONDITION_ALWAYS, 72, 0, 512, FACING_NORTH, 0, 8, 320, 248, 560, 0},
	{LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C, CONDITION_ALWAYS, 184, 0, 512, FACING_NORTH, 0, 8, 320, 248, 560, 0},
	{ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE, CONDITION_ALWAYS, 120, 0, 440, FACING_NORTH, 0, 8, 320, 248, 560, 0},
	{SCENE_TABLE_END},
};

/* The cabins' exit regions. */
Uint32 LemurianShipCabinsRegions[] = {
	REGION_WORDS(466, -20, 580, 474, -12, 588,
				 LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_EAST_A),
	REGION_WORDS_END,
};

/* The lower deck's exit regions. */
Uint32 LemurianShipAquaHydraRegions[] = {
	REGION_WORDS(512, 16, 190, 528, 32, 206,
				 LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_EAST),
	REGION_WORDS_END,
};

Uint32 SceneExits[] = {
	SCENE_EXITS(LemurianShipCabins),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, LemurianShipMainDeck, LEMURIAN_SHIP_MAIN_DECK_FROM_LEMURIAN_SHIP_CABINS, FLAG_LEMURIA_REACHED),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, LemuriaDock, LEMURIA_DOCK_FROM_LEMURIAN_SHIP_CABINS, FLAG_SHIP_AT_ATTEKA_INLET),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, AttekaInlet, ATTEKA_INLET_FROM_WORLD_MAP_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT(EXIT_LOGBOOK_ROOM_TO_TWO_JELLY_ROOM, LemurianShipCabins, ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM),
	SCENE_EXIT(EXIT_TWO_JELLY_ROOM_TO_LOGBOOK_ROOM, LemurianShipCabins, ENTRANCE_LOGBOOK_ROOM_FROM_TWO_JELLY_ROOM),
	SCENE_EXIT(EXIT_TWO_JELLY_ROOM_TO_STAIR_ROOM, LemurianShipCabins, ENTRANCE_STAIR_ROOM_FROM_TWO_JELLY_ROOM),
	SCENE_EXIT(EXIT_TWO_JELLY_ROOM_TO_TWO_JELLY_ROOM_HALL, LemurianShipCabins, ENTRANCE_TWO_JELLY_ROOM_HALL_FROM_TWO_JELLY_ROOM),
	SCENE_EXIT(EXIT_STAIR_ROOM_TO_TWO_JELLY_ROOM, LemurianShipCabins, ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM),
	SCENE_EXIT(EXIT_STAIR_ROOM_TO_STAIR_ROOM_HALL, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_STAIR_ROOM),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A, FLAG_AQUA_HYDRA_BEATEN),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST, FLAG_HOLD_PARCHED),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A, FLAG_AQUA_HYDRA_BEATEN),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A, FLAG_HOLD_PARCHED),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_B, EXIT_FLAG_NONE),
	SCENE_EXIT(EXIT_SIX_JELLY_ROOM_WEST_HALL_TO_SIX_JELLY_ROOM, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL),
	SCENE_EXIT(EXIT_SIX_JELLY_ROOM_EAST_HALL_TO_SIX_JELLY_ROOM, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL),
	SCENE_EXIT(EXIT_SIX_JELLY_ROOM_EAST_HALL_TO_PIPE_ROOM, LemurianShipCabins, ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL),
	SCENE_EXIT(EXIT_TWO_JELLY_ROOM_HALL_TO_TWO_JELLY_ROOM, LemurianShipCabins, ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A, FLAG_AQUA_HYDRA_BEATEN),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B, FLAG_HOLD_PARCHED),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A, FLAG_AQUA_HYDRA_BEATEN),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C, FLAG_HOLD_PARCHED),
	SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_B, EXIT_FLAG_NONE),
	SCENE_EXIT(EXIT_LEDGE_ROOM_TO_SIX_JELLY_ROOM_WEST, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST),
	SCENE_EXIT(EXIT_LEDGE_ROOM_TO_SIX_JELLY_ROOM_EAST, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST),
	SCENE_EXIT(EXIT_LEDGE_ROOM_TO_PIPE_ROOM, LemurianShipCabins, ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM),
	SCENE_EXIT(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_EAST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_EAST),
	SCENE_EXIT(EXIT_STAIR_ROOM_HALL_TO_STAIR_ROOM, LemurianShipCabins, ENTRANCE_STAIR_ROOM_FROM_STAIR_ROOM_HALL),
	SCENE_EXIT(EXIT_SIX_JELLY_ROOM_TO_WEST_HALL, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_WEST_HALL_FROM_SIX_JELLY_ROOM),
	SCENE_EXIT(EXIT_SIX_JELLY_ROOM_TO_EAST_HALL, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_SIX_JELLY_ROOM),
	SCENE_EXIT(EXIT_SIX_JELLY_ROOM_TO_LEDGE_ROOM_WEST, LemurianShipCabins, ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST),
	SCENE_EXIT(EXIT_SIX_JELLY_ROOM_TO_LEDGE_ROOM_EAST, LemurianShipCabins, ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST),
	SCENE_EXIT(EXIT_PIPE_ROOM_TO_SIX_JELLY_ROOM_EAST_HALL, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_PIPE_ROOM),
	SCENE_EXIT(EXIT_PIPE_ROOM_TO_LEDGE_ROOM, LemurianShipCabins, ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM),
	SCENE_EXITS(LemurianShipAquaHydra),
	SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTHWEST_A, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A),
	SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTH_A, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A),
	SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_WEST_A, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A),
	SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTHEAST_A, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A),
	SCENE_EXIT(EXIT_PILLAR_ROOM_TO_CORRIDOR, LemurianShipAquaHydra, ENTRANCE_CORRIDOR_FROM_PILLAR_ROOM),
	SCENE_EXIT(EXIT_CORRIDOR_TO_PILLAR_ROOM, LemurianShipAquaHydra, ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR),
	SCENE_EXIT_UNLESS(EXIT_CORRIDOR_TO_ENGINE_ROOM, LemurianShipAquaHydra, ENTRANCE_ENGINE_ROOM_FROM_CORRIDOR, FLAG_SHIP_ENGINE_RUNNING),
	SCENE_EXIT_UNLESS(EXIT_CORRIDOR_TO_ENGINE_ROOM, LemurianShipAquaHydra, ENTRANCE_RUNNING_ENGINE_ROOM_FROM_CORRIDOR, EXIT_FLAG_NONE),
	SCENE_EXIT(EXIT_ENGINE_ROOM_TO_CORRIDOR, LemurianShipAquaHydra, ENTRANCE_CORRIDOR_FROM_ENGINE_ROOM),
	SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_EAST, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_EAST_B),
	SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_WORLD_MAP, WorldMap, WORLD_MAP_SAIL_TO_DECK),
	SCENE_EXITS_END,
};

/* The actors of neither scene: none. */
ACTOR_PLACE LemurianShipPlacements[] = {
	{SCENE_TABLE_END},
};

/* The room of two jellies' actors, by LEMURIAN_SHIP_TWO_JELLY_ROOM_ACTOR_ID. */
ACTOR_PLACE LemurianShipTwoJellyRoomPlacements[] = {
	{SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, 1, toFIXED(104), 0, toFIXED(424), FACING_SOUTH, 0, 0},
	{SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, 1, toFIXED(104), 0, toFIXED(536), FACING_SOUTH, 0, 0},
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The room of six jellies' actors, by LEMURIAN_SHIP_SIX_JELLY_ROOM_ACTOR_ID. */
ACTOR_PLACE LemurianShipSixJellyRoomPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The ledge room's actors, by LEMURIAN_SHIP_LEDGE_ROOM_ACTOR_ID. */
ACTOR_PLACE LemurianShipLedgeRoomPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The room of the pipes' actors, by enum */
/* LEMURIAN_SHIP_PIPE_ROOM_ACTOR_ID. */
ACTOR_PLACE LemurianShipPipeRoomPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The hold's actors, by LEMURIAN_SHIP_HOLD_ACTOR_ID. */
ACTOR_PLACE LemurianShipHoldPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIERS, FLAG_IN_PARTY(ACTOR_ISAAC), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* The pillar room's actors, by LEMURIAN_SHIP_PILLAR_ROOM_ACTOR_ID. */
ACTOR_PLACE LemurianShipPillarRoomPlacements[] = {
	{SPRITE_SINKING_PILLAR, CONDITION_ALWAYS, 1, toFIXED(584), 0, toFIXED(168), FACING_SOUTH, 2, 0},
	{SPRITE_SINKING_PILLAR, CONDITION_ALWAYS, 1, toFIXED(472), 0, toFIXED(152), FACING_SOUTH, 2, 0},
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* The engine room's actors, by LEMURIAN_SHIP_ENGINE_ROOM_ACTOR_ID. */
ACTOR_PLACE LemurianShipEngineRoomPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* How a ship's door swings open, as three ticks of metatiles copied over */
/* the closed door, ended by -1 (MAPGRID_ANIMATION_STEPS_END). */
Sint16 LemurianShipDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(0, 126, 1, 2, 2),
	MAPGRID_ANIMATION_STEP(1, 126, 1, 2, 4),
	MAPGRID_ANIMATION_STEP(2, 126, 1, 2, 4),
	-1,
};

/* How the logbook room's door swings open, in two ticks. */
Sint16 LemurianShipFirstDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(10, 126, 1, 2, 4),
	MAPGRID_ANIMATION_STEP(11, 126, 1, 2, 4),
	-1,
	/* Unused: the same for a door two metatiles wide. */
	MAPGRID_ANIMATION_STEP(4, 126, 2, 2, 2),
	-1,
	0,
};

/* The cabin doors, by the step cue that opens each. */
LEMURIAN_SHIP_WARP LemurianShipDoors[CABIN_DOOR_COUNT] = {
	{EXIT_LOGBOOK_ROOM_TO_TWO_JELLY_ROOM, (Sint32)LemurianShipFirstDoorOpening, 4, 3},
	{LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, (Sint32)LemurianShipDoorOpening, 35, 11},
	{LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A, (Sint32)LemurianShipDoorOpening, 6, 45},
	{EXIT_LEDGE_ROOM_TO_SIX_JELLY_ROOM_WEST, (Sint32)LemurianShipDoorOpening, 11, 45},
	{EXIT_LEDGE_ROOM_TO_SIX_JELLY_ROOM_EAST, (Sint32)LemurianShipDoorOpening, 21, 45},
	{EXIT_LEDGE_ROOM_TO_PIPE_ROOM, (Sint32)LemurianShipDoorOpening, 26, 46},
	{EXIT_STAIR_ROOM_HALL_TO_STAIR_ROOM, (Sint32)LemurianShipDoorOpening, 29, 28},
	{EXIT_SIX_JELLY_ROOM_TO_WEST_HALL, (Sint32)LemurianShipDoorOpening, 48, 2},
	{EXIT_SIX_JELLY_ROOM_TO_EAST_HALL, (Sint32)LemurianShipDoorOpening, 54, 2},
	{EXIT_PIPE_ROOM_TO_SIX_JELLY_ROOM_EAST_HALL, (Sint32)LemurianShipDoorOpening, 41, 29},
};

/* The lower deck's doors, by the step cue that opens each, then the */
/* hold's two doors while it is flooded. */
LEMURIAN_SHIP_WARP LemurianShipLowerDoors[LOWER_DOOR_COUNT + 2] = {
	{LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTHWEST_A, (Sint32)LemurianShipDoorOpening, 4, 2},
	{LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTH_A, (Sint32)LemurianShipDoorOpening, 10, 2},
	{EXIT_PILLAR_ROOM_TO_CORRIDOR, (Sint32)LemurianShipDoorOpening, 31, 3},
	{EXIT_CORRIDOR_TO_ENGINE_ROOM, (Sint32)LemurianShipDoorOpening, 41, 51},
	{11, (Sint32)LemurianShipDoorOpening, 4, 21},
	{12, (Sint32)LemurianShipDoorOpening, 10, 21},
};

/* The cues of neither scene: none. */
CUE LemurianShipCues[] = {
	{CUE_TABLE_END},
};

/* The cabins' cues. */
CUE LemurianShipCabinCues[] = {
	{CUE_EXIT, LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, CONDITION_ALWAYS, LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_LOGBOOK_ROOM_TO_TWO_JELLY_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{CUE_EXIT, EXIT_STAIR_ROOM_TO_TWO_JELLY_ROOM, CONDITION_ALWAYS, EXIT_STAIR_ROOM_TO_TWO_JELLY_ROOM},
	{CUE_EXIT, EXIT_STAIR_ROOM_TO_STAIR_ROOM_HALL, CONDITION_ALWAYS, EXIT_STAIR_ROOM_TO_STAIR_ROOM_HALL},
	{CUE_EXIT, LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A, CONDITION_ALWAYS, LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A},
	{CUE_EXIT, LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, CONDITION_ALWAYS, LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A},
	{CUE_EXIT, EXIT_SIX_JELLY_ROOM_WEST_HALL_TO_SIX_JELLY_ROOM, CONDITION_ALWAYS, EXIT_SIX_JELLY_ROOM_WEST_HALL_TO_SIX_JELLY_ROOM},
	{CUE_EXIT, EXIT_SIX_JELLY_ROOM_EAST_HALL_TO_SIX_JELLY_ROOM, CONDITION_ALWAYS, EXIT_SIX_JELLY_ROOM_EAST_HALL_TO_SIX_JELLY_ROOM},
	{CUE_EXIT, EXIT_SIX_JELLY_ROOM_EAST_HALL_TO_PIPE_ROOM, CONDITION_ALWAYS, EXIT_SIX_JELLY_ROOM_EAST_HALL_TO_PIPE_ROOM},
	{CUE_EXIT, EXIT_TWO_JELLY_ROOM_HALL_TO_TWO_JELLY_ROOM, CONDITION_ALWAYS, EXIT_TWO_JELLY_ROOM_HALL_TO_TWO_JELLY_ROOM},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_STAIR_ROOM_HALL_TO_STAIR_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{EXAMINE_CUE(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShipItLooksLikeTheresCaptains))},
	{EXAMINE_CUE(EXAMINE_WARDROBE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShipItsPleasantlyCool))},
	{CUE_TABLE_END},
};

/* The room of two jellies' cues. */
CUE LemurianShipTwoJellyRoomCues[] = {
	{CUE_EXIT, EXIT_TWO_JELLY_ROOM_TO_LOGBOOK_ROOM, CONDITION_ALWAYS, EXIT_TWO_JELLY_ROOM_TO_LOGBOOK_ROOM},
	{CUE_EXIT, EXIT_TWO_JELLY_ROOM_TO_STAIR_ROOM, CONDITION_ALWAYS, EXIT_TWO_JELLY_ROOM_TO_STAIR_ROOM},
	{CUE_EXIT, EXIT_TWO_JELLY_ROOM_TO_TWO_JELLY_ROOM_HALL, CONDITION_ALWAYS, EXIT_TWO_JELLY_ROOM_TO_TWO_JELLY_ROOM_HALL},
	{CUE_TALK, ACTOR_TWO_JELLY_ROOM_JELLY_A, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightTwoJellyRoomJelly)},
	{CUE_TALK, ACTOR_TWO_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightTwoJellyRoomJelly)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TWO_JELLY_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_A, SCRIPT_REF(scLemurianShipFreezeTwoJellyRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TWO_JELLY_ROOM_PUDDLE_B, FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_B, SCRIPT_REF(scLemurianShipFreezeTwoJellyRoomPuddle)},
	{CUE_TABLE_END},
};

/* The room of six jellies' cues. */
CUE LemurianShipSixJellyRoomCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_SIX_JELLY_ROOM_TO_WEST_HALL, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_SIX_JELLY_ROOM_TO_EAST_HALL, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{CUE_EXIT, EXIT_SIX_JELLY_ROOM_TO_LEDGE_ROOM_WEST, CONDITION_ALWAYS, EXIT_SIX_JELLY_ROOM_TO_LEDGE_ROOM_WEST},
	{CUE_EXIT, EXIT_SIX_JELLY_ROOM_TO_LEDGE_ROOM_EAST, CONDITION_ALWAYS, EXIT_SIX_JELLY_ROOM_TO_LEDGE_ROOM_EAST},
	{CUE_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_A, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightSixJellyRoomJelly)},
	{CUE_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightSixJellyRoomJelly)},
	{CUE_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_C, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightSixJellyRoomJelly)},
	{CUE_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_D, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightSixJellyRoomJelly)},
	{CUE_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_E, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightSixJellyRoomJelly)},
	{CUE_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_F, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightSixJellyRoomJelly)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A), SCRIPT_REF(scLemurianShipFreezeLiftPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_A, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_B), SCRIPT_REF(scLemurianShipFreezeSixJellyRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_B, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_C), SCRIPT_REF(scLemurianShipFreezeSixJellyRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_C, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_D), SCRIPT_REF(scLemurianShipFreezeSixJellyRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_D, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_E), SCRIPT_REF(scLemurianShipFreezeSixJellyRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_E, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_F), SCRIPT_REF(scLemurianShipFreezeSixJellyRoomPuddle)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SIX_JELLY_ROOM_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipLiftCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SIX_JELLY_ROOM_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipSetDownCrate)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipLiftCrate)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipSetDownCrate)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_LIFT_LEVER, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipExamineLiftSwitch)},
	{CUE_STEP, METATILE_BEHAVIOR_LIFT_EAST, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A), SCRIPT_REF(scLemurianShipStepOnLift)},
	{CUE_STEP, METATILE_BEHAVIOR_LIFT_WEST, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A), SCRIPT_REF(scLemurianShipStepOnLiftWestSide)},
	{CUE_STEP, METATILE_BEHAVIOR_NEAR_LIFT_JELLY, FLAG_JELLY_FLED, SCRIPT_REF(scLemurianShipScareJelly)},
	{CUE_TABLE_END},
};

/* The ledge room's cues. */
CUE LemurianShipLedgeRoomCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_LEDGE_ROOM_TO_SIX_JELLY_ROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_LEDGE_ROOM_TO_SIX_JELLY_ROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_LEDGE_ROOM_TO_PIPE_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEDGE_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipLiftLedgeCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEDGE_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipPushCrateOffLedge)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipLiftLedgeCrate)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipPushCrateOffLedge)},
	{CUE_TABLE_END},
};

/* The room of the pipes' cues. */
CUE LemurianShipPipeRoomCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_PIPE_ROOM_TO_SIX_JELLY_ROOM_EAST_HALL, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenDoor)},
	{CUE_EXIT, EXIT_PIPE_ROOM_TO_LEDGE_ROOM, CONDITION_ALWAYS, EXIT_PIPE_ROOM_TO_LEDGE_ROOM},
	{TALK_CUE_FACING(FACING_EAST), ACTOR_PIPE_ROOM_JELLY_A, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_PIPE_ROOM_JELLY_A, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{TALK_CUE_FACING(FACING_WEST), ACTOR_PIPE_ROOM_JELLY_B, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_PIPE_ROOM_JELLY_B, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_PIPE_ROOM_JELLY_B, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{CUE_TALK, ACTOR_PIPE_ROOM_JELLY_C, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{CUE_TALK, ACTOR_PIPE_ROOM_JELLY_D, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_PIPE_ROOM_JELLY_E, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{TALK_CUE_FACING(FACING_EAST), ACTOR_PIPE_ROOM_JELLY_G, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{TALK_CUE_FACING(FACING_WEST), ACTOR_PIPE_ROOM_JELLY_G, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_PIPE_ROOM_JELLY_G, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_FightPipeRoomJelly)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_A, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_A), SCRIPT_REF(scLemurianShipFreezePipeRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_B, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_B), SCRIPT_REF(scLemurianShipFreezePipeRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_C, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_C), SCRIPT_REF(scLemurianShipFreezePipeRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_D, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_D), SCRIPT_REF(scLemurianShipFreezePipeRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_E, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_E), SCRIPT_REF(scLemurianShipFreezePipeRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_G, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_G), SCRIPT_REF(scLemurianShipFreezePipeRoomPuddle)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PIPE_ROOM_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipLiftCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PIPE_ROOM_CRATE, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipSetDownCrate)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipLiftCrate)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipSetDownCrate)},
	{CUE_TABLE_END},
};

/* The hold's cues. */
CUE LemurianShipHoldCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTHWEST_A, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenLowerDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTH_A, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipOpenLowerDoor)},
	{CUE_EXIT, LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_WEST_A, CONDITION_ALWAYS, LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_WEST_A},
	{CUE_EXIT, LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTHEAST_A, CONDITION_ALWAYS, LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTHEAST_A},
	{CUE_TALK, ACTOR_HOLD_JELLY_A, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipFightAquaHydra)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_FIRST), METATILE_BEHAVIOR_FLOODED_HOLD, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipStartParch)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_FLOODED_HOLD, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipParchHold)},
	{CUE_TABLE_END},
};

/* The pillar room's cues. */
CUE LemurianShipPillarRoomCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_PILLAR_ROOM_TO_CORRIDOR, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipWalkIntoPassage)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_SUNK, SCRIPT_REF(scLemurianShipLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_SUNK, SCRIPT_REF(scLemurianShipSetDownPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_SUNK, SCRIPT_REF(scLemurianShipLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_SUNK, SCRIPT_REF(scLemurianShipSetDownPillar)},
	{CUE_PUSH, 0, FLAG_PILLAR_WAY_OPEN, SCRIPT_REF(scLemurianShipLiftPillar)},
	{CUE_PUSHED, 0, FLAG_PILLAR_WAY_OPEN, SCRIPT_REF(scLemurianShipSetDownPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_FULL, SCRIPT_REF(LemurianShip_DouseRightPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_FULL, SCRIPT_REF(LemurianShip_DouseLeftPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_RIGHT_PILLAR_PUDDLE, FLAG_RIGHT_PILLAR_PUDDLE_FROZEN, SCRIPT_REF(scLemurianShipShowRightPillarPuddle)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_LEFT_PILLAR_PUDDLE, FLAG_LEFT_PILLAR_PUDDLE_FROZEN, SCRIPT_REF(scLemurianShipShowLeftPillarPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_RIGHT_PILLAR_PUDDLE, FLAG_RIGHT_PILLAR_PUDDLE_FROZEN, SCRIPT_REF(scLemurianShipFreezeTwoJellyRoomPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_LEFT_PILLAR_PUDDLE, FLAG_LEFT_PILLAR_PUDDLE_FROZEN, SCRIPT_REF(scLemurianShipFreezeTwoJellyRoomPuddle)},
	{CUE_RAISED, RAISED_RIGHT_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_RaiseRightPillar)},
	{CUE_RAISED, RAISED_LEFT_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(LemurianShip_RaiseLeftPillar)},
	{CUE_TABLE_END},
};

/* The lower deck's cues. */
CUE LemurianShipLowerDeckCues[] = {
	/* Unused: no metatile of the lower deck's map answers 5, and its exits */
	/* list none. */
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, EXIT_CORRIDOR_TO_PILLAR_ROOM, CONDITION_ALWAYS, EXIT_CORRIDOR_TO_PILLAR_ROOM},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_CORRIDOR_TO_ENGINE_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipWalkIntoPassage)},
	{CUE_EXIT, EXIT_ENGINE_ROOM_TO_CORRIDOR, CONDITION_ALWAYS, EXIT_ENGINE_ROOM_TO_CORRIDOR},
	{CUE_TABLE_END},
};

/* A glint drifting in to the orb, gone as it arrives. */
Uint32 LemurianShipLightToOrb[] = {
	SC_WAIT(10),
	SC_SYNC,
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};
/* clang-format on */
#undef PLACE_ON_LOWER_FLOOR
#undef HALFWORDS
#undef ENTRANCE_WORDS
#undef ENTRANCE_WORDS_END
#undef REGION_WORDS
#undef REGION_WORDS_END
