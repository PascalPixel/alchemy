/* @file */
/* The Kandorean Temple of Master Poi, south-west of Daila, in nine rooms that
 */
/* share this overlay (SceneId_KandoreanTempleLifts to */
/* SceneId_KandoreanTemplePoi). */
/* */
/* Its gate is shut for the meditation (scKandoreanTempleStartGate): Kraden */
/* spots a suspicious wall, and Whirlwind on the bush in the courtyard's north
 */
/* west corner opens a hidden way in through a cave of shallow water */
/* (scKandoreanTempleFindTheHiddenPassage). Inside, Poi tests three students */
/* (scKandoreanTempleTestTheStudents), and the party follows the one who can */
/* levitate, through the door below Poi's statue, into the caverns of his */
/* trials: the Mimic's chest, the student who gave up, a sheet of water whose */
/* current pushes Felix south (scKandoreanTemplePushAlongTheCurrent), two vents
 */
/* whose round covers rise on gushing columns unless the pillar holds them down
 */
/* (KandoreanTemple_RunWestVent, KandoreanTemple_RunEastVent), the lifts those
 */
/* columns push up into the room above, and its shaft the pillar there can */
/* block; then a pot boiling over a hot floor, and three Chestbeaters */
/* (scKandoreanTempleFightChestbeaters), after which a ladder comes down to */
/* Poi's hall. Poi gives the party the Lash Pebble */
/* (scKandoreanTempleReceiveLash), and Lash ties the rope across to Fog, the */
/* Mercury Djinni (scKandoreanTempleFightFog). Once Poi has met the party, the
 */
/* temple's door in the courtyard opens as Felix walks up to it */
/* (scKandoreanTempleOpenTheTempleDoor). */

#include "scene/event_layer.h"
#include "item_ids.h"
#include "macros.h"
#include "field/actor_script.h"
#include "version.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "graphics/glyph.h"
#include "scene/scene.h"
#include "field/call.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

/* The lift room's actors (SceneId_KandoreanTempleLifts), numbering the rows of
 */
/* KandoreanTemple_LiftRoomPlacements. */
typedef enum KANDOREAN_TEMPLE_LIFT_ROOM_ACTOR_ID {
	/* The posts of the two tightropes, a pair to each rope. */
	ACTOR_WEST_ROPE_WEST_POST = ACTOR_FIRST_PLACED,
	ACTOR_WEST_ROPE_EAST_POST,
	ACTOR_EAST_ROPE_WEST_POST,
	ACTOR_EAST_ROPE_EAST_POST,
	/* The pillar Move pushes along (KandoreanTemple_BlockActors). */
	ACTOR_LIFT_ROOM_PILLAR,
	ACTOR_WEST_LIFT,
	ACTOR_EAST_LIFT,
	/* The columns that grow under the lifts as they rise. */
	ACTOR_WEST_LIFT_COLUMN,
	ACTOR_EAST_LIFT_COLUMN
} KANDOREAN_TEMPLE_LIFT_ROOM_ACTOR_ID;

/* The Chestbeaters' room's actors (SceneId_KandoreanTempleChestbeaters), */
/* numbering the rows of KandoreanTemple_ChestbeaterRoomPlacements. */
typedef enum KANDOREAN_TEMPLE_CHESTBEATER_ROOM_ACTOR_ID {
	/* The pot of boiling water Move chucks aside */
	/* (KandoreanTemple_PillarActors). */
	ACTOR_BOILING_POT = ACTOR_FIRST_PLACED,
	/* The red embers the pot boils over: an actor in the pot's sprite at the */
	/* pot's place, in its animation 2, the sprite sheet's last picture alone.
	   It */
	/* is drawn underfoot with no shadow, hidden under the pot, and stays
	   glowing */
	/* on the floor where the pot stood when the pot is chucked aside. */
	ACTOR_EMBERS_UNDER_THE_POT,
	/* The sign by the hot floor, which tells of Master Poi's trial. */
	ACTOR_POIS_SIGN,
	ACTOR_MIDDLE_CHESTBEATER,
	ACTOR_WEST_CHESTBEATER,
	ACTOR_EAST_CHESTBEATER
} KANDOREAN_TEMPLE_CHESTBEATER_ROOM_ACTOR_ID;

/* The vent room's actors (SceneId_KandoreanTemplePillar), numbering the rows */
/* of KandoreanTemple_VentRoomPlacements. */
typedef enum KANDOREAN_TEMPLE_VENT_ROOM_ACTOR_ID {
	/* The pillar Move sets on either vent's cover to hold it down. */
	ACTOR_VENT_ROOM_PILLAR = ACTOR_FIRST_PLACED,
	/* The round stone covers of the two vents, which their columns of water */
	/* lift. */
	ACTOR_WEST_VENT_COVER,
	ACTOR_EAST_VENT_COVER,
	/* The gushing tops of the columns of water that grow under the covers as */
	/* they rise. */
	ACTOR_WEST_COLUMN_TOP,
	ACTOR_EAST_COLUMN_TOP,
	/* The pieces of the west column below its top, from the highest down. A */
	/* cover rises on all four while the pillar holds the other vent down; */
	/* otherwise on the upper two. */
	ACTOR_WEST_COLUMN_UPPER_PIECE,
	ACTOR_WEST_COLUMN_MIDDLE_PIECE,
	ACTOR_WEST_COLUMN_LOWER_PIECE,
	ACTOR_WEST_COLUMN_FOOT,
	ACTOR_EAST_COLUMN_UPPER_PIECE,
	ACTOR_EAST_COLUMN_MIDDLE_PIECE,
	ACTOR_EAST_COLUMN_LOWER_PIECE,
	ACTOR_EAST_COLUMN_FOOT
} KANDOREAN_TEMPLE_VENT_ROOM_ACTOR_ID;

/* Fog's room's actors (SceneId_KandoreanTempleFog), numbering the rows of */
/* KandoreanTemple_FogRoomPlacements. */
typedef enum KANDOREAN_TEMPLE_FOG_ROOM_ACTOR_ID {
	/* The posts of the rope Lash ties across to Fog's ledge. */
	ACTOR_FOG_ROPE_POST = ACTOR_FIRST_PLACED,
	ACTOR_FOG_ROPE_OTHER_POST,
	/* The student who has given up on the test: "It seems I may not have been
	 */
	/* ready after all..." He sits on the floor, squashing and stretching now */
	/* and then, until the party has met Poi. */
	ACTOR_STUDENT_WHO_GAVE_UP,
	/* Fog, the Mercury Djinni. */
	ACTOR_FOG
} KANDOREAN_TEMPLE_FOG_ROOM_ACTOR_ID;

/* The temple gate's actors (SceneId_KandoreanTempleGate), numbering the rows */
/* of KandoreanTemple_GatePlacements. */
typedef enum KANDOREAN_TEMPLE_GATE_ACTOR_ID {
	/* The weeds over the mouth of the cave, which Whirlwind scatters. */
	ACTOR_WEEDS_OVER_THE_CAVE = ACTOR_FIRST_PLACED,
	/* "Have you come to see Master Poi?" */
	ACTOR_STUDENT_ASKING_ABOUT_POI,
	/* "Once the period of meditation has begun, none may enter the temple */
	/* grounds." */
	ACTOR_STUDENT_ON_GUARD_DUTY,
	/* The three students meditating in the courtyard. */
	/* @{ */
	ACTOR_STUDENT_WARY_OF_OUTSIDERS,
	ACTOR_STUDENT_ON_THE_FAILED_TEST,
	ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
	/* @} */
	ACTOR_GATE_KRADEN
} KANDOREAN_TEMPLE_GATE_ACTOR_ID;

/* Poi's room's actors (SceneId_KandoreanTemplePoi), numbering the rows of */
/* KandoreanTemple_PoisRoomPlacements. */
typedef enum KANDOREAN_TEMPLE_POIS_ROOM_ACTOR_ID {
	/* The posts of the room's two ropes: Lash ties the loose end of each. */
	ACTOR_SOUTH_ROPE_WEST_POST = ACTOR_FIRST_PLACED,
	ACTOR_SOUTH_ROPE_EAST_POST,
	ACTOR_NORTH_ROPE_EAST_POST,
	ACTOR_NORTH_ROPE_WEST_POST,
	ACTOR_POI,
	/* The three students Poi tests, in turn; only the third can levitate well
	 */
	/* enough to go on to the final test. */
	/* @{ */
	ACTOR_FIRST_TESTED_STUDENT,
	ACTOR_SECOND_TESTED_STUDENT,
	ACTOR_THIRD_TESTED_STUDENT,
	/* @} */
	ACTOR_POIS_ROOM_KRADEN
} KANDOREAN_TEMPLE_POIS_ROOM_ACTOR_ID;

/* Poi's room's story exits, each leading back into the room by the entrance of
 */
/* its number: finding the rope still lying on the floor */
/* (scKandoreanTempleFindTheRopeOnTheFloor), and coming in through the cave to
 */
/* meet Poi (scKandoreanTempleMeetPoi). */
enum {
	KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR = 13,
	KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE = 14
};

/* The ways between the three parts of Poi's hall, each exit leading in by the
 */
/* entrance on its far side: the test hall in the south, where Poi tests his */
/* students before the door below his statue; the upper hall north of it, with
 */
/* Poi's table and the north rope, reached by two openings in the test hall's */
/* north wall; and the passage down the west side, from the ladder up out of */
/* the Chestbeaters' room to the upper hall's west doorway. */
enum {
	KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_WEST = 2,
	KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_EAST,
	KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_WEST,
	KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_EAST,
	KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_WEST_PASSAGE,
	KANDOREAN_TEMPLE_POI_WEST_PASSAGE_TO_UPPER_HALL
};
/* The entrances, one for each exit above. */
enum {
	KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_WEST = 2,
	KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_EAST,
	KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_WEST,
	KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_EAST,
	KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_WEST_PASSAGE,
	KANDOREAN_TEMPLE_POI_WEST_PASSAGE_FROM_UPPER_HALL
};

/* The rooms' own cell behaviors, named by room. */
typedef enum KANDOREAN_TEMPLE_CELL_BEHAVIOR {
	/* The lift room's shaft, cells (43, 10) to (43, 13). */
	CELL_BEHAVIOR_SHAFT = 10,
	/* The west and east lifts while risen, cells (27, 12) and (37, 14). */
	CELL_BEHAVIOR_WEST_LIFT = 12,
	CELL_BEHAVIOR_EAST_LIFT,
	/* The Chestbeaters' room: the cells west, south and east of the hot floor
	 */
	/* under the pot, (19, 29), (20, 30) and (21, 29), and the floor before the
	 */
	/* Chestbeaters, (19, 12) to (21, 12). */
	CELL_BEHAVIOR_WEST_OF_HOT_FLOOR = 10,
	CELL_BEHAVIOR_SOUTH_OF_HOT_FLOOR,
	CELL_BEHAVIOR_EAST_OF_HOT_FLOOR,
	CELL_BEHAVIOR_BEFORE_CHESTBEATERS = 6,
	/* The vent room's covers while risen, which carry Felix up. */
	CELL_BEHAVIOR_WEST_VENT_COVER = 20,
	CELL_BEHAVIOR_EAST_VENT_COVER,
	/* The room of the current: the sheet of water the current runs through. */
	CELL_BEHAVIOR_CURRENT = 10,
	/* Fog's room: behind and beside the rope's posts. */
	CELL_BEHAVIOR_BEHIND_POST = 10,
	CELL_BEHAVIOR_BESIDE_POST,
	/* The courtyard: the cells just inside and just before the temple's door,
	 */
	/* kept at (2, 0) and (2, 2) in the rock of the top left corner and laid at
	 */
	/* (17, 24) and (17, 26) once Poi has been met; the well, cells (25, 17) to
	 */
	/* (26, 18); the west wall */
	/* Kraden suspects, (5, 15) and (6, 15); the temple's door, (17, 25) and */
	/* (18, 25); and the five clumps of weeds Cyclone scatters. */
	CELL_BEHAVIOR_INSIDE_TEMPLE_DOOR = 4,
	CELL_BEHAVIOR_BEFORE_TEMPLE_DOOR,
	CELL_BEHAVIOR_COURTYARD_WELL,
	CELL_BEHAVIOR_SUSPICIOUS_WALL = 20,
	CELL_BEHAVIOR_TEMPLE_DOOR = 10,
	CELL_BEHAVIOR_FIRST_WEED_CLUMP = 30,
	CELL_BEHAVIOR_SECOND_WEED_CLUMP,
	CELL_BEHAVIOR_THIRD_WEED_CLUMP,
	CELL_BEHAVIOR_FOURTH_WEED_CLUMP,
	CELL_BEHAVIOR_FIFTH_WEED_CLUMP,
	/* Poi's hall: round the opening down to the Chestbeaters' room, cells */
	/* (9, 5), (11, 5) and (10, 6); before the door below Poi's statue, (24, 40)
	 */
	/* and (25, 40); and Poi's table with the Lash Pebble, (25, 21). */
	CELL_BEHAVIOR_BY_THE_LADDER = 8,
	CELL_BEHAVIOR_CAVE_DOOR,
	CELL_BEHAVIOR_POIS_TABLE = 16
} KANDOREAN_TEMPLE_CELL_BEHAVIOR;

/* The Chestbeaters' room's entrance after their battle, which no exit leads */
/* to: the party is back before the north wall, and the ladder comes down. */
enum { ENTRANCE_AFTER_CHESTBEATER_BATTLE = 3 };

/* The temple's scene and area flags, by the rooms that read them. */
enum {
	/* The leader stands on the lift room's west or east lift, or on the vent */
	/* room's west or east cover. */
	FLAG_ON_WEST_LIFT = FLAG_SCENE_TEMP_08,
	FLAG_ON_EAST_LIFT = FLAG_SCENE_TEMP_09,
	/* Move has lifted the vent room's pillar off a vent's cover. */
	FLAG_PILLAR_LIFTED_OFF_VENT = FLAG_SCENE_TEMP_0A,
	/* Move is carrying the vent room's pillar. */
	FLAG_PILLAR_MOVING = FLAG_SCENE_TEMP_0B,
	/* The vent room's west or east vent blows, its column of water rising out
	 */
	/* of its hole under its cover. */
	FLAG_WEST_VENT_BLOWING = FLAG_SCENE_TEMP_0C,
	FLAG_EAST_VENT_BLOWING = FLAG_SCENE_TEMP_0D,
	/* The vents' rush of steam is playing (SE_WAVE). */
	FLAG_VENT_SOUND_PLAYING = FLAG_SCENE_TEMP_0E,
	/* Lash has tied the rope across to Fog, or the south rope of Poi's room. */
	FLAG_FOG_ROPE_TIED = FLAG_SCENE_TEMP_0F,
	FLAG_SOUTH_ROPE_TIED = FLAG_SCENE_TEMP_0F,
	/* The north rope of Poi's room is tied across. */
	FLAG_NORTH_ROPE_TIED = FLAG_SCENE_TEMP_10,
	/* The lift room's pillar blocks the shaft, so stepping over it no longer */
	/* sinks Felix into it. */
	FLAG_PILLAR_BLOCKS_SHAFT = FLAG_SCENE_TEMP_11,
	/* The door to the cave below Poi's statue, or the temple's door in the */
	/* courtyard, is open. */
	FLAG_CAVE_DOOR_OPEN = FLAG_SCENE_TEMP_12,
	FLAG_TEMPLE_DOOR_OPEN = FLAG_SCENE_TEMP_12,
	/* Kraden has pointed out the suspicious wall by the gate on this visit. */
	FLAG_KRADEN_SAW_THE_WALL = FLAG_SCENE_TEMP_30,
	/* Where the vent room's pillar stands, which also decides which of the lift
	 */
	/* room's lifts runs. */
	FLAG_PILLAR_ON_WEST_VENT = FLAG_AREA_TEMP_00,
	FLAG_PILLAR_ON_EAST_VENT = FLAG_AREA_TEMP_01,
	/* Where Move has chucked the Chestbeaters' room's pot of boiling water. */
	FLAG_POT_PUSHED_WEST = FLAG_AREA_TEMP_02,
	FLAG_POT_PUSHED_EAST = FLAG_AREA_TEMP_03,
	/* The pot is out of the way, and the floor no longer burns. */
	FLAG_POT_MOVED = FLAG_AREA_TEMP_04
};

s32 KandoreanTemple_SplashRingCounter BSS_DATA;
s32 KandoreanTemple_WestVentBlowing BSS_DATA;
s32 KandoreanTemple_EastVentBlowing BSS_DATA;
s32 KandoreanTemple_WestTimer BSS_DATA; /* frames into its cycle */
s32 KandoreanTemple_EastTimer BSS_DATA; /* frames into its cycle */
u16 KandoreanTemple_WestFloatPhase BSS_DATA;
u16 KandoreanTemple_EastFloatPhase BSS_DATA;
ACTOR *KandoreanTemple_LashPebble BSS_DATA;
extern s32 KandoreanTemple_SteamScript[];
extern u32 KandoreanTemple_SplashRingScript[];
extern u32 KandoreanTemple_SplashScript[];

void scKandoreanTempleStartGate(void);
extern s32 KandoreanTemple_MeditateScript[];
extern s32 KandoreanTemple_LevitateScript[];

extern u32 KandoreanTemple_LookAroundScript[];
extern s16 KandoreanTemple_BlockActors[];
extern s16 KandoreanTemple_PillarActors[];

/* Creates an actor of the given sprite at a 16.16 position. */
static __inline__ ACTOR *scKandoreanTempleCreateActor(s32 type, FIXED x,
													  FIXED y, FIXED z) {
	return ((ACTOR *)syImportActorCreate(type, x, y, z));
}

/* The Kandorean Temple's story, from the hidden passage Kraden suspects to */
/* the ladder that unrolls once the Chestbeaters are beaten. */
#define FLAG_HIDDEN_PASSAGE_FOUND	0x842
#define FLAG_ASKED_ABOUT_POI		0x845
#define FLAG_TOLD_OF_THE_MEDITATION 0x846
#define FLAG_STUDENTS_TESTED		0x848
#define FLAG_MET_POI				0x849
#define FLAG_LASH_RECEIVED			0x84a
#define FLAG_LADDER_LOWERED			0x92d

/* The map state as the two water rooms write it: MAP_STATE.groundEffs */
/* as its two fields, the footprints and the kind of wading, which they set */
/* to 1. */
typedef struct MAP_STATE_GROUND_EFFECTS {
	s32 *origin;
	s32 shakeX, shakeY, shakeDecay;
	u8 *script;
	u16 flg;
	u8 wideTiles;
	u8 footprints : 2;
	u8 wading : 2;
} MAP_STATE_GROUND_EFFECTS;

extern SCENE_REGION KandoreanTemple_StreamRoomRegions[];
extern ACTOR_PLACE KandoreanTemple_GatePlacements[];
extern ACTOR_PLACE KandoreanTemple_PoisRoomPlacements[];
extern TRIGGER KandoreanTemple_MimicRoomEvents[];
void scKandoreanTempleArriveOnEastLift(void);

void scKandoreanTempleTestTheStudents(void);
extern u32 KandoreanTemple_LeaderStepsAsideScript[];
extern u32 KandoreanTemple_KradenStepsAsideScript[];
extern u32 KandoreanTemple_PoiWalksToTheRopeScript[];

void scKandoreanTempleFindTheHiddenPassage(void);
void scKandoreanTempleFightChestbeaters(void);

extern u32 FieldEffect_ShortScript[];

extern u32 FieldEffect_MediumScript[];

extern s32 KandoreanTemple_HighFlameScript[];
extern s32 KandoreanTemple_LowFlameScript[];
extern s32 KandoreanTemple_FlameScript[];

void scKandoreanTempleArriveOnWestLift(void);
void scKandoreanTempleLeaveSplashRings(ACTOR *actor, s32 step);
void scKandoreanTempleSpawnSplashRing(ACTOR *actor);
void scKandoreanTemplePushSouth(ACTOR *actor, FIXED dz);

/* Talking to Fog starts its battle. */
void scKandoreanTempleFightFog(void) {
	fdRequestDjinnBattle(ACTOR_FOG, DJINNI_FOG);
}

/* Unused: a cloud of steam's script call, from the steam script's unreached */
/* tail; counts down its life and reports whether any is left. */
u32 scKandoreanTempleCountDownSteam(ACTOR *actor) {
	if(--actor->userValue)
		return 1;
	return 0;
}

/* A meditating student's update: now and then, every four to eight seconds, an
 */
/* ellipsis over his head. */
s32 scKandoreanTempleMeditate(ACTOR *actor) {
	if(actor->tmr != 0) {
		actor->tmr--;
	}
	else {
		fdActorShowEmoteById(actor->userValue, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
							 0);
		actor->tmr = (syRandomBelow(240)) + 240;
	}
	return 1;
}

/* An actor script call, each frame: every 60 to 149 frames the actor squashes
 */
/* and stretches back to its own shape over six frames. */
s32 scKandoreanTempleSquashAndStretch(ACTOR *actor) {
	switch(actor->tmr) {
	case 6:
		actor->scaleX -= FIXED_ONE / 4;
		actor->scaleY += FIXED_ONE / 8;
		break;
	case 4:
		actor->scaleX += FIXED_ONE / 8;
		actor->scaleY -= FIXED_ONE / 16;
		break;
	case 2:
		actor->scaleX += FIXED_ONE / 16;
		actor->scaleY -= FIXED_ONE / 32;
		break;
	case 0:
		actor->scaleX = FIXED_ONE;
		actor->scaleY = FIXED_ONE;
		actor->tmr = syRandom() % 90 + 60;
		break;
	}
	actor->tmr--;
	return 1;
}

/* Sets up a torch's flame in Poi's room: still in the air, at four fifths of */
/* its size, with no shadow, never a talk or Psynergy target. */
s32 scKandoreanTemplePlaceFlame(ACTOR *actor) {
	actor->motionFlg = 0;
	actor->scaleX = FIXED_ONE * 4 / 5;
	actor->scaleY = FIXED_ONE * 4 / 5;
	actor->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorSetSprFlags(actor, 0);
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

extern SCENE_REGION KandoreanTemple_ChestbeaterRoomRegions[];

/* The regions of the rooms whose exits are rectangles: the Chestbeaters' room
 */
/* and the stream room. */
const SCENE_REGION *scGetExitRegions(void) {
	s32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
		return KandoreanTemple_ChestbeaterRoomRegions;
	if(scene == SCENE_KANDOREAN_TEMPLE_STREAM)
		return KandoreanTemple_StreamRoomRegions;
	return 0;
}

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE KandoreanTemple_Placements[];
extern ACTOR_PLACE KandoreanTemple_LiftRoomPlacements[];
extern ACTOR_PLACE KandoreanTemple_ChestbeaterRoomPlacements[];
extern ACTOR_PLACE KandoreanTemple_VentRoomPlacements[];
extern ACTOR_PLACE KandoreanTemple_FogRoomPlacements[];

/* The current room's actors. Once the party has met Poi, the gate's three */
/* students, no longer meditating, turn to the party when spoken to; in Poi's */
/* room the first and third tested students turn too, and the second stands */
/* elsewhere. */
void *scGetActorPlacements(void) {
	s32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KANDOREAN_TEMPLE_LIFTS)
		return KandoreanTemple_LiftRoomPlacements;
	if(scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
		return KandoreanTemple_ChestbeaterRoomPlacements;
	if(scene == SCENE_KANDOREAN_TEMPLE_PILLAR)
		return KandoreanTemple_VentRoomPlacements;
	if(scene == SCENE_KANDOREAN_TEMPLE_FOG)
		return KandoreanTemple_FogRoomPlacements;
	if(scene == SCENE_KANDOREAN_TEMPLE_GATE) {
		if(syGameFlagGet(FLAG_MET_POI)) {
			KandoreanTemple_GatePlacements[ACTOR_STUDENT_WARY_OF_OUTSIDERS -
										   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
			KandoreanTemple_GatePlacements[ACTOR_STUDENT_ON_THE_FAILED_TEST -
										   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
			KandoreanTemple_GatePlacements[ACTOR_STUDENT_WHO_WOULD_BE_A_MONK -
										   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
		}
		return KandoreanTemple_GatePlacements;
	}
	if(scene == SCENE_KANDOREAN_TEMPLE_POI) {
		if(syGameFlagGet(FLAG_MET_POI)) {
			KandoreanTemple_PoisRoomPlacements[ACTOR_FIRST_TESTED_STUDENT -
											   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_THEN_WATCH;
			KandoreanTemple_PoisRoomPlacements[ACTOR_SECOND_TESTED_STUDENT -
											   ACTOR_FIRST_PLACED]
				.x = toFIXED(368);
			KandoreanTemple_PoisRoomPlacements[ACTOR_SECOND_TESTED_STUDENT -
											   ACTOR_FIRST_PLACED]
				.z = toFIXED(694);
			KandoreanTemple_PoisRoomPlacements[ACTOR_THIRD_TESTED_STUDENT -
											   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_AND_BACK;
		}
		return KandoreanTemple_PoisRoomPlacements;
	}
	return KandoreanTemple_Placements;
}

/* Felix walks past the courtyard's west wall, cells (5, 15) and (6, 15), */
/* before the hidden passage is found: Kraden walks up beside him and says the
 */
/* wall looks rather suspicious, and the two nod. */
void scKandoreanTempleKradenSuspectsTheWall(void) {
	ACTOR *actor;

	if(syGameFlagGet(FLAG_HIDDEN_PASSAGE_FOUND) == 0) {
		syGameFlagSet(FLAG_KRADEN_SAW_THE_WALL);
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdActorWalkToAndWaitById(ACTOR_FELIX, 104, 252);
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
		scFieldActorLaunchFromLeader(ACTOR_GATE_KRADEN, -16, -16, FACING_NORTH);
		fdActorWaitForMoveById(ACTOR_GATE_KRADEN);
		fdEventWait(20);
		fdEventSetMessageId((s32)&MsgKandoreanTempleSuspiciousWall);
		/* "This wall looks rather suspicious, wouldn't you say, Felix?" */
		fdEventShowMessageAndWait(ACTOR_GATE_KRADEN, 0);
		fdActorFaceDirectionAndRefreshById(ACTOR_GATE_KRADEN, FACING_SOUTHEAST);
		fdEventWait(30);
		fdActorSetAnimationById(ACTOR_GATE_KRADEN, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_GATE_KRADEN, 1.2);
		fdActorSetAnimationById(ACTOR_GATE_KRADEN, ANIM_WALK);
		actor = fdActorGetById(ACTOR_FELIX);
		if(actor)
			fdActorSetDestinationById(ACTOR_GATE_KRADEN,
									  actor->x.part[0].integer,
									  actor->z.part[0].integer);
		fdActorWaitForMoveById(ACTOR_GATE_KRADEN);
		fdActorSetPos(ACTOR_GATE_KRADEN, 0, 0);
		fdEventEnd();
	}
}

extern TRIGGER KandoreanTemple_Events[];
extern TRIGGER KandoreanTemple_LiftRoomEvents[];
extern TRIGGER KandoreanTemple_ChestbeaterRoomEvents[];
extern TRIGGER KandoreanTemple_VentRoomEvents[];
extern TRIGGER KandoreanTemple_StreamRoomEvents[];
extern TRIGGER KandoreanTemple_CurrentRoomEvents[];
extern TRIGGER KandoreanTemple_FogRoomEvents[];
extern TRIGGER KandoreanTemple_GateEvents[];
extern TRIGGER KandoreanTemple_PoisRoomEvents[];

/* The current room's triggers. */
void *scGetTriggers(void) {
	s32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KANDOREAN_TEMPLE_LIFTS)
		return KandoreanTemple_LiftRoomEvents;
	if(scene == SCENE_KANDOREAN_TEMPLE_MIMIC)
		return KandoreanTemple_MimicRoomEvents;
	if(scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
		return KandoreanTemple_ChestbeaterRoomEvents;
	if(scene == SCENE_KANDOREAN_TEMPLE_PILLAR)
		return KandoreanTemple_VentRoomEvents;
	if(scene == SCENE_KANDOREAN_TEMPLE_STREAM)
		return KandoreanTemple_StreamRoomEvents;
	if(scene == SCENE_KANDOREAN_TEMPLE_CURRENT)
		return KandoreanTemple_CurrentRoomEvents;
	if(scene == SCENE_KANDOREAN_TEMPLE_FOG)
		return KandoreanTemple_FogRoomEvents;
	if(scene == SCENE_KANDOREAN_TEMPLE_GATE)
		return KandoreanTemple_GateEvents;
	if(scene == SCENE_KANDOREAN_TEMPLE_POI)
		return KandoreanTemple_PoisRoomEvents;
	return KandoreanTemple_Events;
}

/* Each call of scKandoreanTempleLeaveSplashRings on a moving wader counts two
 */
/* toward the next ring on the water, or four when half its speeds across and */
/* down together pass SPLASH_RING_FAST_SPEED (a 16.16 speed, just under three */
/* quarters of a pixel a frame); past SPLASH_RING_STRIDE a ring spreads at its
 */
/* feet. */
#define SPLASH_RING_FAST_SPEED 48000
#define SPLASH_RING_STRIDE	   40

/* Unused: takes the leader's shadow away and counts one step of its wading */
/* (scKandoreanTempleLeaveSplashRings). */
UNUSED void scKandoreanTempleStartSplashRings(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	scKandoreanTempleLeaveSplashRings(leader, 0);
}

/* Counts one step of an actor through the water, more when it moves fast, */
/* and spreads a ring on the water at its feet once enough are counted. */
void scKandoreanTempleLeaveSplashRings(ACTOR *actor, s32 step) {
	s32 speed = (actor->velX + actor->velZ) >> 1;
	s32 *counter = &KandoreanTemple_SplashRingCounter;

	if(speed < 0)
		speed = -speed;
	if((u32)speed > SPLASH_RING_FAST_SPEED)
		*counter += 4;
	else if(speed != 0)
		*counter += 2;
	KandoreanTemple_SplashRingCounter += step;
	if(KandoreanTemple_SplashRingCounter > SPLASH_RING_STRIDE) {
		scKandoreanTempleSpawnSplashRing(actor);
		KandoreanTemple_SplashRingCounter = 0;
	}
}

/* Spreads a translucent ring on the water at an actor's feet, which goes after
 */
/* 24 frames. */
void scKandoreanTempleSpawnSplashRing(ACTOR *actor) {
	ACTOR *ring = scKandoreanTempleCreateActor(
		SPRITE_SPLASH_RING, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	FIELD_SPR *sprite;

	if(ring) {
		sprite = ring->spr;
		fdActorStartScript(ring, KandoreanTemple_SplashRingScript);
		ring->motionFlg = 0;
		ring->layer = 1;
		ring->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		if(sprite) {
			fdSprStartAnimation(sprite, 2);
			sprite->objMode = OBJ_MODE_SEMI_TRANSPARENT;
			sprite->flg = 0;
			sprite->pri = 3;
		}
	}
}

/* Felix wades into the sheet of water that pours south from the waterfall in */
/* the room of the current, any of its cells: the current pushes him south, */
/* splashing at his feet, unless he runs against it. */
void scKandoreanTemplePushAlongTheCurrent(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *splash;

	if((FrameCount & 7) == 0) {
		splash = ((ACTOR *)syImportActorCreate(
			SPRITE_WADING_SPLASH, leader->x.fixed, leader->y.fixed + toFIXED(6),
			leader->z.fixed));
		if(splash) {
			fdActorSetSprFlags(splash, 0);
			fdActorSelectAnimation(splash, 1);
			fdActorStartScript(splash, KandoreanTemple_SplashScript);
		}
	}
	scKandoreanTemplePushSouth(leader, toFIXED(0.85));
}

/* Moves an actor and the camera south by dz, unless the map stops it eight */
/* pixels ahead or ahead to either side. */
void scKandoreanTemplePushSouth(ACTOR *actor, FIXED dz) {
	ACTOR *focus = FieldStateWork->camera;
	FIXED position[3];

	position[0] = actor->x.fixed;
	position[1] = actor->y.fixed;
	position[2] = actor->z.fixed + dz;
	fdMapGridCheckCollision(actor, position);
	position[0] = actor->x.fixed;
	position[1] = actor->y.fixed;
	position[2] = actor->z.fixed + toFIXED(8);
	if(fdMapGridCheckCollision(actor, position) > 0)
		return;
	position[0] = actor->x.fixed + toFIXED(5.7);
	position[1] = actor->y.fixed;
	position[2] = actor->z.fixed + toFIXED(5.7);
	if(fdMapGridCheckCollision(actor, position) > 0)
		return;
	position[0] = actor->x.fixed - toFIXED(5.7);
	position[1] = actor->y.fixed;
	position[2] = actor->z.fixed + toFIXED(5.7);
	if(fdMapGridCheckCollision(actor, position) > 0)
		return;
	focus->z.fixed += dz;
	actor->z.fixed += dz;
}

/* Whether actor a is within twelve pixels across and eight deep of actor b. */
s32 scKandoreanTempleIsOverActor(ACTOR *a, ACTOR *b) {
	FIXED left = b->x.fixed - toFIXED(12);
	FIXED right = b->x.fixed + toFIXED(12);

	if(a->x.fixed > left && a->x.fixed < right &&
	   a->z.fixed > b->z.fixed - toFIXED(8) &&
	   a->z.fixed < b->z.fixed + toFIXED(8))
		return 1;
	return 0;
}

/* A cloud of the vents' steam: drifts and slows, grows and turns, and rises */
/* instead of drifting on where the leader or the pillar stands over it. */
void scKandoreanTempleUpdateVentSteam(ACTOR *actor) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *other = fdActorGetById(ACTOR_VENT_ROOM_PILLAR);
	FIXED vx = actor->bounce;
	FIXED vz;
	FIELD_SPR *sprite;
	s32 rate;
	s32 scale;

	actor->x.fixed += vx;
	actor->y.fixed += actor->gravity;
	vz = actor->sceneWord;
	actor->z.fixed += vz;
	actor->bounce = vx - vx / FIELD_EFFECT_DRAG_X;
	actor->sceneWord = vz - vz / FIELD_EFFECT_DRAG_Z;
	rate = actor->speedLimit;
	scale = actor->scaleX;
	sprite = actor->spr;
	actor->scaleX = scale + rate;
	actor->scaleY += actor->acceleration;
	sprite->rotation += actor->tmr;
	if(leader->y.fixed == 0) {
		if(scKandoreanTempleIsOverActor(actor, leader)) {
			actor->z.fixed -= actor->sceneWord;
			actor->gravity += actor->sceneWord >> 2;
			actor->sceneWord = 0;
		}
		if(leader->y.fixed == 0 && scKandoreanTempleIsOverActor(actor, other)) {
			actor->z.fixed -= actor->sceneWord;
			actor->gravity += actor->sceneWord >> 2;
			actor->sceneWord = 0;
		}
	}
}

/* Sets up a cloud of steam on the given sprite priority: translucent, no */
/* shadow, its script. */
void scKandoreanTempleInitSteam(ACTOR *actor, s32 priority) {
	FIELD_SPR *sprite;

	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	sprite = actor->spr;
	sprite->pri = priority;
	fdActorSetSprFlags(actor, 0);
	fdActorSelectAnimation(actor, 2);
	fdActorStartScript(actor, KandoreanTemple_SteamScript);
	fdActorSetSprCol(actor, SPRITE_COLOR_BLUE_TINT);
	actor->tmr = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}

/* Blows a cloud of steam from a vent at position, rising at the given speed. */
void scKandoreanTempleBlowSteam(s32 *position, s32 value) {
	ACTOR *actor = scKandoreanTempleCreateActor(
		SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));

	if(actor) {
		s32 timer;

		actor->bounce =
			syCos((syRandomBelow(TRIG_TURN * 3 / 16)) + TRIG_TURN * 3 / 16);
		actor->gravity = (u16)syRandom();
		actor->sceneWord = (u16)(syRandom() * 2) + value;
		timer = (u16)syRandom() - 32768;
		actor->tmr = timer;
		scKandoreanTempleInitSteam(actor, 3);
		actor->update = (OBJECT_UPDATE)scKandoreanTempleUpdateVentSteam;
		fdActorSetBlendMode(actor, 1);
	}
}

/* A cloud of the lift room's stream of steam: as the vents' clouds, held down
 */
/* by the leader or the lift room's pillar. */
void scKandoreanTempleUpdateStreamSteam(ACTOR *actor) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *other = fdActorGetById(ACTOR_LIFT_ROOM_PILLAR);
	FIXED vx = actor->bounce;
	FIXED vz;
	FIELD_SPR *sprite;
	s32 rate;
	s32 scale;

	actor->x.fixed += vx;
	actor->y.fixed += actor->gravity;
	vz = actor->sceneWord;
	actor->z.fixed += vz;
	actor->bounce = vx - vx / FIELD_EFFECT_DRAG_X;
	actor->sceneWord = vz - vz / FIELD_EFFECT_DRAG_Z;
	rate = actor->speedLimit;
	scale = actor->scaleX;
	sprite = actor->spr;
	actor->scaleX = scale + rate;
	actor->scaleY += actor->acceleration;
	sprite->rotation += actor->tmr;
	if(scKandoreanTempleIsOverActor(actor, leader)) {
		((FIELD_SPR *)actor->spr)->pri = 3;
		actor->z.fixed -= actor->sceneWord;
		actor->gravity += actor->sceneWord >> 2;
		actor->sceneWord = 0;
	}
	if(scKandoreanTempleIsOverActor(actor, other)) {
		((FIELD_SPR *)actor->spr)->pri = 3;
		actor->z.fixed -= actor->sceneWord;
		actor->gravity += actor->sceneWord >> 3;
		actor->sceneWord = 0;
	}
}

/* Blows a cloud of steam from the lift room's stream. */
void scKandoreanTempleBlowStreamSteam(void) {
	ACTOR *actor = ((ACTOR *)syImportActorCreate(
		SPRITE_DUST_CLOUD, toFIXED(696), toFIXED(28), toFIXED(156)));

	if(actor) {
		s32 timer;

		actor->bounce =
			syCos((syRandomBelow(TRIG_TURN * 3 / 16)) + TRIG_TURN * 3 / 16);
		actor->gravity = (u16)syRandom();
		actor->sceneWord = (u16)(syRandom() * 2) + toFIXED(11) / 2;
		timer = (u16)syRandom() - 32768;
		actor->tmr = timer;
		scKandoreanTempleInitSteam(actor, 1);
		actor->update = (OBJECT_UPDATE)scKandoreanTempleUpdateStreamSteam;
		fdActorSetBlendMode(actor, 1);
	}
}

/* Gives the leader its shadow back. */
void scKandoreanTempleShowLeader(void) {
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId),
					   SPRITE_FLAG_SHADOW);
}

/* Sets a lift or a vent's cover at rest: on the ground, in front, in its */
/* lowered animation, disabled. */
void scKandoreanTemplePrepareLift(s32 id) {
	ACTOR *actor = fdActorGetById(id);

	actor->sceneCnt = 1;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetAnimationById(id, 1);
	fdActorSetSprPriority(id, 3);
	fdActorDisableById(id);
}

/* Sets a lift's column or a piece of one at rest, disabled. */
void scKandoreanTemplePrepareColumn(s32 id) {
	fdActorGetById(id)->sceneCnt = 1;
	fdActorSetSprPriority(id, 2);
	fdActorDisableById(id);
}

/* Poi casts Lash on the north rope's east post. */
void scKandoreanTempleCastLash(void) {
	fdPsynergyLoadResources(ABILITY_LASH, 1);
	fdPsynergySetupActorPair(ACTOR_POI, ACTOR_NORTH_ROPE_EAST_POST);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	fdPsynergyEnd();
}

/* Defines a vent's frame task: while blowing is set, a cloud of steam of the */
/* given strength puffs from (x, z), 16.16 pixels, every period frames. */
#define DEFINE_KANDOREAN_TEMPLE_VENT(name, blowing, period, x, z, strength)    \
	void name(void) {                                                          \
		s32 position[3];                                                       \
                                                                               \
		if(blowing != 0 && FrameCount % period == 0) {                         \
			SET_VECTOR(position, x, 0, z);                                     \
			scKandoreanTempleBlowSteam(position, strength);                    \
		}                                                                      \
	}

/* The west vent's task: while it blows, a cloud of steam every ninth frame. */
DEFINE_KANDOREAN_TEMPLE_VENT(KandoreanTemple_BlowWestVent,
							 KandoreanTemple_WestVentBlowing, 9, toFIXED(296),
							 toFIXED(176), toFIXED(2))

/* The east vent's task: while it blows, a stronger cloud every fifth frame. */
DEFINE_KANDOREAN_TEMPLE_VENT(KandoreanTemple_BlowEastVent,
							 KandoreanTemple_EastVentBlowing, 5, toFIXED(344),
							 toFIXED(160), toFIXED(4))

/* The lift room's task: a cloud of steam from the stream every fifth frame. */
void scKandoreanTempleBlowStream(void) {
	if(FrameCount % 5 == 0)
		scKandoreanTempleBlowStreamSteam();
}

/* Lowers a piece of the west column flat under its cover. */
void scKandoreanTempleLowerWestColumn(s32 id) {
	fdActorGetById(id)->scaleY = FIXED_ONE / 32;
	scFieldActorSetPosAndResetMotion(fdActorGetById(id), toFIXED(216), 0,
									 toFIXED(216));
}

/* Lowers a piece of the east column flat under its cover. */
void scKandoreanTempleLowerEastColumn(s32 id) {
	fdActorGetById(id)->scaleY = FIXED_ONE / 32;
	scFieldActorSetPosAndResetMotion(fdActorGetById(id), toFIXED(376), 0,
									 toFIXED(248));
}

/* Grows a column piece by scale and raises it by dy. */
void scKandoreanTempleGrowColumn(s32 id, FIXED dy, s32 scale) {
	ACTOR *actor = fdActorGetById(id);

	actor->scaleY += scale;
	actor->y.fixed += dy;
	actor->z.fixed -= dy;
}

/* Shrinks a column piece by scale and lowers it by dy. */
void scKandoreanTempleShrinkColumn(s32 id, FIXED dy, s32 scale) {
	ACTOR *actor = fdActorGetById(id);

	actor->scaleY -= scale;
	actor->y.fixed -= dy;
	actor->z.fixed += dy;
}

/* Whether the west vent stays shut: while Move carries the pillar off a */
/* cover, or while the pillar weighs the west cover down. */
s32 scKandoreanTempleIsWestVentBlocked(void) {
	if(syGameFlagGet(FLAG_PILLAR_LIFTED_OFF_VENT) ||
	   syGameFlagGet(FLAG_PILLAR_ON_WEST_VENT))
		return 1;
	return 0;
}

/* Whether the east vent stays shut: while Move carries the pillar off a */
/* cover, or while the pillar weighs the east cover down. */
s32 scKandoreanTempleIsEastVentBlocked(void) {
	if(syGameFlagGet(FLAG_PILLAR_LIFTED_OFF_VENT) ||
	   syGameFlagGet(FLAG_PILLAR_ON_EAST_VENT))
		return 1;
	return 0;
}

/* A vent starts blowing: its column and cover ready to rise, carrying the */
/* leader if it stands there. `lowerColumn` lays a piece of the column flat */
/* under the cover; the two lower pieces are laid out only when the pillar */
/* weighs the other vent down (`pillarOnOther`). */
#define DEFINE_KANDOREAN_TEMPLE_START_VENT(                                    \
	name, coverId, coverX, coverZ, top, upper, middle, lower, foot, onLift,    \
	lowerColumn, pillarOnOther, blowingVar, blowingFlag)                       \
	void name(void) {                                                          \
		fdActorSetAnimationById(top, 2);                                       \
		fdActorSetAnimationById(upper, 3);                                     \
		fdActorSetAnimationById(middle, 3);                                    \
		fdActorSetAnimationById(lower, 3);                                     \
		fdActorSetAnimationById(foot, 3);                                      \
		fdActorSetAnimationById(coverId, 0);                                   \
		fdActorSetSprPriority(coverId, 2);                                     \
		fdActorGetById(coverId)->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;          \
		scFieldActorSetPosAndResetMotion(fdActorGetById(coverId),              \
										 toFIXED(coverX), toFIXED(8),          \
										 toFIXED(coverZ));                     \
		if(syGameFlagGet(onLift)) {                                            \
			fdActorSetAnimationById(coverId, 4);                               \
			scFieldActorSetPosAndResetMotion(                                  \
				fdActorGetById(MainRoutineStateWork.leaderActorId),            \
				toFIXED(coverX), toFIXED(12), toFIXED(coverZ));                \
			fdActorSetSprFlags(                                                \
				fdActorGetById(MainRoutineStateWork.leaderActorId), 0);        \
		}                                                                      \
		fdActorGetById(top)->scaleY = FIXED_ONE;                               \
		scFieldActorSetPosAndResetMotion(fdActorGetById(top), toFIXED(coverX), \
										 0, toFIXED(coverZ));                  \
		lowerColumn(upper);                                                    \
		lowerColumn(middle);                                                   \
		if(syGameFlagGet(pillarOnOther)) {                                     \
			lowerColumn(lower);                                                \
			lowerColumn(foot);                                                 \
		}                                                                      \
		blowingVar = 1;                                                        \
		fdEventSetFlag(blowingFlag);                                           \
	}

DEFINE_KANDOREAN_TEMPLE_START_VENT(
	KandoreanTemple_StartWestVent, ACTOR_WEST_VENT_COVER, 216, 216,
	ACTOR_WEST_COLUMN_TOP, ACTOR_WEST_COLUMN_UPPER_PIECE,
	ACTOR_WEST_COLUMN_MIDDLE_PIECE, ACTOR_WEST_COLUMN_LOWER_PIECE,
	ACTOR_WEST_COLUMN_FOOT, FLAG_ON_WEST_LIFT, scKandoreanTempleLowerWestColumn,
	FLAG_PILLAR_ON_EAST_VENT, KandoreanTemple_WestVentBlowing,
	FLAG_WEST_VENT_BLOWING)

DEFINE_KANDOREAN_TEMPLE_START_VENT(
	KandoreanTemple_StartEastVent, ACTOR_EAST_VENT_COVER, 376, 248,
	ACTOR_EAST_COLUMN_TOP, ACTOR_EAST_COLUMN_UPPER_PIECE,
	ACTOR_EAST_COLUMN_MIDDLE_PIECE, ACTOR_EAST_COLUMN_LOWER_PIECE,
	ACTOR_EAST_COLUMN_FOOT, FLAG_ON_EAST_LIFT, scKandoreanTempleLowerEastColumn,
	FLAG_PILLAR_ON_WEST_VENT, KandoreanTemple_EastVentBlowing,
	FLAG_EAST_VENT_BLOWING)

/* A vent's cycle, in frames of its timer. Shut, it counts only while nothing */
/* holds it shut; at VENT_BLOW it blows, walls off its cover's cell and lifts */
/* the cover on its column until VENT_RISE_END, higher when the pillar holds */
/* the other vent down. The cover floats on the column until VENT_FLOAT_END and
 */
/* on until its float comes round, then sinks from VENT_SINK; once the column */
/* is flat the timer jumps to VENT_SHUT, which lowers everything and frees the
 */
/* cell, and the vent rests a random while before its timer starts again. A */
/* leader riding the risen cover at VENT_CARRY_LEADER is carried up to the */
/* room above. */
typedef enum VENT_FRAME {
	VENT_BLOW = 300,
	VENT_RISE_END = 316,
	VENT_CARRY_LEADER = 400,
	VENT_FLOAT_END = 600,
	VENT_SINK = 1000,
	VENT_SHUT = 3000
} VENT_FRAME;

/* A vent's task, its timer through its cycle. Its cover stands on cell */
/* (`cellX`, `cellZ`), walled off while it blows, with the same cell of the */
/* map's second layer 50 columns east; `carryCell`, in the map's top row, is */
/* the cell copied there to carry a riding leader up to the lift above. */
/* `pillarOnOther` is the flag of the pillar weighing the other vent down, */
/* which sends this one higher. */
#define DEFINE_KANDOREAN_TEMPLE_RUN_VENT(                                      \
	name, timerVar, phaseVar, blowingVar, blowingFlag, isBlocked, start,       \
	cellX, cellZ, carryCell, coverId, coverX, coverZ, top, upper, middle,      \
	lower, foot, onLift, pillarOnOther)                                        \
	void name(void) {                                                          \
		s32 *timer = &timerVar;                                                \
		ACTOR *cover;                                                          \
		ACTOR *leader;                                                         \
		u16 *phase;                                                            \
		s32 limit;                                                             \
		s32 raised;                                                            \
		s32 last;                                                              \
		s32 t;                                                                 \
                                                                               \
		t = *timer;                                                            \
		if(t < VENT_BLOW) {                                                    \
			if(isBlocked() != 0)                                               \
				return;                                                        \
		}                                                                      \
		else if(t == VENT_BLOW) {                                              \
			fdMapGridCopyCellAttributes(15, 15, 1, 1, cellX, cellZ);           \
			fdMapGridCopyCellAttributes(65, 15, 1, 1, (cellX) + 50, cellZ);    \
			start();                                                           \
		}                                                                      \
		else {                                                                 \
			last = VENT_SHUT - 1;                                              \
			if(t <= last) {                                                    \
				cover = fdActorGetById(coverId);                               \
				raised = syGameFlagGet(pillarOnOther);                         \
				t = *timer;                                                    \
				if(raised) {                                                   \
					if(t <= VENT_RISE_END) {                                   \
						cover->y.fixed += toFIXED(9) / 2;                      \
						limit = FIXED_ONE / 16;                                \
						scKandoreanTempleGrowColumn(top, toFIXED(2), limit);   \
						scKandoreanTempleGrowColumn(upper, toFIXED(3) / 2,     \
													limit);                    \
						scKandoreanTempleGrowColumn(middle, toFIXED(1),        \
													limit);                    \
						scKandoreanTempleGrowColumn(lower, toFIXED(1) / 2,     \
													limit);                    \
						scKandoreanTempleGrowColumn(foot, 0, limit);           \
					}                                                          \
					else if(t < VENT_FLOAT_END) {                              \
						if(syGameFlagGet(onLift) &&                            \
						   *timer == VENT_CARRY_LEADER) {                      \
							fdMapGridCopyCellAttributes(carryCell, 0, 1, 1,    \
														cellX, cellZ);         \
							syCloseEvent(name);                                \
						}                                                      \
						phase = &phaseVar;                                     \
						cover->y.fixed += sySin(*phase) >> 1;                  \
						*phase += TRIG_TURN / 16;                              \
					}                                                          \
					else if(t < VENT_SINK) {                                   \
						phase = &phaseVar;                                     \
						if(*phase == 0) {                                      \
							blowingVar = 0;                                    \
							*timer = VENT_SINK - 1;                            \
							syGameFlagClear(blowingFlag);                      \
						}                                                      \
						cover->y.fixed += sySin(*phase) >> 1;                  \
						*phase += TRIG_TURN / 16;                              \
					}                                                          \
					else {                                                     \
						cover->y.fixed += -toFIXED(27) / 4;                    \
						limit = FIXED_ONE * 3 / 32;                            \
						scKandoreanTempleShrinkColumn(top, toFIXED(3), limit); \
						scKandoreanTempleShrinkColumn(upper, toFIXED(9) / 4,   \
													  limit);                  \
						scKandoreanTempleShrinkColumn(middle, toFIXED(3) / 2,  \
													  limit);                  \
						scKandoreanTempleShrinkColumn(lower, toFIXED(3) / 4,   \
													  limit);                  \
						scKandoreanTempleShrinkColumn(foot, 0, limit);         \
						if(fdActorGetById(foot)->scaleY <= limit)              \
							*timer = last;                                     \
					}                                                          \
				}                                                              \
				else {                                                         \
					if(t <= VENT_RISE_END) {                                   \
						cover->y.fixed += toFIXED(2.475);                      \
						limit = FIXED_ONE / 16;                                \
						scKandoreanTempleGrowColumn(top, toFIXED(1), limit);   \
						scKandoreanTempleGrowColumn(upper, toFIXED(1) / 2,     \
													limit);                    \
						scKandoreanTempleGrowColumn(middle, 0, limit);         \
					}                                                          \
					else if(t < VENT_FLOAT_END) {                              \
						phase = &phaseVar;                                     \
						cover->y.fixed += sySin(*phase) >> 1;                  \
						*phase += TRIG_TURN / 16;                              \
					}                                                          \
					else if(t < VENT_SINK) {                                   \
						phase = &phaseVar;                                     \
						if(*phase == 0) {                                      \
							blowingVar = 0;                                    \
							*timer = VENT_SINK - 1;                            \
							syGameFlagClear(blowingFlag);                      \
						}                                                      \
						cover->y.fixed += sySin(*phase) >> 1;                  \
						*phase += TRIG_TURN / 16;                              \
					}                                                          \
					else {                                                     \
						cover->y.fixed += -toFIXED(3.7125);                    \
						limit = FIXED_ONE * 3 / 32;                            \
						scKandoreanTempleShrinkColumn(top, toFIXED(3) / 2,     \
													  limit);                  \
						scKandoreanTempleShrinkColumn(upper, toFIXED(3) / 4,   \
													  limit);                  \
						scKandoreanTempleShrinkColumn(middle, 0, limit);       \
						if(fdActorGetById(middle)->scaleY <= limit)            \
							*timer = last;                                     \
					}                                                          \
				}                                                              \
				if(syGameFlagGet(onLift)) {                                    \
					leader =                                                   \
						fdActorGetById(MainRoutineStateWork.leaderActorId);    \
					leader->y.fixed = cover->y.fixed + toFIXED(4);             \
					leader->tgtY = ACTOR_NO_TARGET;                            \
					leader->velY = 0;                                          \
				}                                                              \
			}                                                                  \
			else if(t == VENT_SHUT) {                                          \
				fdMapGridCopyCellAttributes(8, 0, 1, 1, cellX, cellZ);         \
				fdMapGridCopyCellAttributes(58, 0, 1, 1, (cellX) + 50, cellZ); \
				fdActorSetPos(top, 0, 0);                                      \
				fdActorSetPos(upper, 0, 0);                                    \
				fdActorSetPos(middle, 0, 0);                                   \
				fdActorSetPos(lower, 0, 0);                                    \
				fdActorSetPos(foot, 0, 0);                                     \
				fdActorSetAnimationById(coverId, 1);                           \
				scFieldActorSetPosAndResetMotion(fdActorGetById(coverId),      \
												 toFIXED(coverX), 0,           \
												 toFIXED(coverZ));             \
				fdActorGetById(coverId)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;   \
				fdActorSetSprPriority(coverId, 3);                             \
				if(syGameFlagGet(onLift)) {                                    \
					leader =                                                   \
						fdActorGetById(MainRoutineStateWork.leaderActorId);    \
					fdActorSetPosAndResetMotion(leader, leader->x.fixed, 0,    \
												leader->z.fixed);              \
					fdActorSetSprFlags(                                        \
						fdActorGetById(MainRoutineStateWork.leaderActorId),    \
						SPRITE_FLAG_SHADOW);                                   \
				}                                                              \
			}                                                                  \
			else {                                                             \
				s32 none = isBlocked();                                        \
                                                                               \
				if(none != 0)                                                  \
					return;                                                    \
				*timer += syRandomBelow(128);                                  \
				if((u32)*timer > (syRandomBelow(10000)) + 10000)               \
					*timer = none;                                             \
			}                                                                  \
		}                                                                      \
		timerVar++;                                                            \
	}

/* The west vent's task. */
DEFINE_KANDOREAN_TEMPLE_RUN_VENT(
	KandoreanTemple_RunWestVent, KandoreanTemple_WestTimer,
	KandoreanTemple_WestFloatPhase, KandoreanTemple_WestVentBlowing,
	FLAG_WEST_VENT_BLOWING, scKandoreanTempleIsWestVentBlocked,
	KandoreanTemple_StartWestVent, 13, 13, 22, ACTOR_WEST_VENT_COVER, 216, 216,
	ACTOR_WEST_COLUMN_TOP, ACTOR_WEST_COLUMN_UPPER_PIECE,
	ACTOR_WEST_COLUMN_MIDDLE_PIECE, ACTOR_WEST_COLUMN_LOWER_PIECE,
	ACTOR_WEST_COLUMN_FOOT, FLAG_ON_WEST_LIFT, FLAG_PILLAR_ON_EAST_VENT)

/* The east vent's task. */
DEFINE_KANDOREAN_TEMPLE_RUN_VENT(
	KandoreanTemple_RunEastVent, KandoreanTemple_EastTimer,
	KandoreanTemple_EastFloatPhase, KandoreanTemple_EastVentBlowing,
	FLAG_EAST_VENT_BLOWING, scKandoreanTempleIsEastVentBlocked,
	KandoreanTemple_StartEastVent, 23, 15, 23, ACTOR_EAST_VENT_COVER, 376, 248,
	ACTOR_EAST_COLUMN_TOP, ACTOR_EAST_COLUMN_UPPER_PIECE,
	ACTOR_EAST_COLUMN_MIDDLE_PIECE, ACTOR_EAST_COLUMN_LOWER_PIECE,
	ACTOR_EAST_COLUMN_FOOT, FLAG_ON_EAST_LIFT, FLAG_PILLAR_ON_WEST_VENT)

/* A lift's cycle, in frames of its timer: at LIFT_RISE it starts up its column
 */
/* from LIFT_SUNK_HEIGHT below the floor until it stands level with it, frees */
/* its cell, and its timer jumps to LIFT_RISEN. It floats on the column, walls
 */
/* its cell off again at LIFT_WALL_OFF, and once its float comes round after */
/* LIFT_FLOAT_END it takes a leader standing on it down to the vent room, or */
/* with no one on it sinks from LIFT_SINK. */
typedef enum LIFT_FRAME {
	LIFT_RISE = 300,
	LIFT_RISEN = 2000,
	LIFT_WALL_OFF = 2280,
	LIFT_FLOAT_END = 2300,
	LIFT_SINK = 2800
} LIFT_FRAME;

/* How far below the lift room's floor a lift starts and ends, in pixels. */
#define LIFT_SUNK_HEIGHT (-20)

/* A lift of the lift room, the top of a vent's column, its timer through its */
/* cycle. It rises at (`x`, `z`), its column 4 pixels north of it, and frees */
/* cell (`cellX`, `cellZ`) while it stands level with the floor; `carryCell`, */
/* in the map's top row, is the cell copied there to take a riding leader */
/* down. */
#define DEFINE_KANDOREAN_TEMPLE_RUN_LIFT(name, liftId, columnId, timerVar,     \
										 phaseVar, x, z, cellX, cellZ,         \
										 carryCell, onLift)                    \
	void name(void) {                                                          \
		ACTOR *lift = fdActorGetById(liftId);                                  \
		ACTOR *base = fdActorGetById(columnId);                                \
		s32 *timer = &timerVar;                                                \
                                                                               \
		if(*timer == LIFT_RISE) {                                              \
			scQueueMusic(SE_WAVE);                                             \
			scFieldActorSetPosAndResetMotion(                                  \
				fdActorGetById(liftId), toFIXED(x), toFIXED(LIFT_SUNK_HEIGHT), \
				toFIXED(z));                                                   \
			fdActorSetPosAndResetMotion(fdActorGetById(columnId), toFIXED(x),  \
										toFIXED(LIFT_SUNK_HEIGHT),             \
										toFIXED((z) - 4));                     \
			fdActorSetSprPriority(liftId, 3);                                  \
			fdActorSetSprPriority(columnId, 3);                                \
			base->priFlg |= ACTOR_PRIORITY_UNDERFOOT;                          \
			base->scaleY = FIXED_ONE / 64;                                     \
			fdActorSetAnimationById(liftId, 0);                                \
			fdActorSetAnimationById(columnId, 2);                              \
		}                                                                      \
		else if(*timer > LIFT_RISE && *timer < LIFT_RISEN) {                   \
			lift->y.fixed += toFIXED(4);                                       \
			scKandoreanTempleGrowColumn(columnId, toFIXED(0.3), toFIXED(0.3)); \
			if(lift->y.fixed > -toFIXED(5))                                    \
				fdActorSetSprPriority(liftId, 2);                              \
			if(lift->y.fixed >= 0) {                                           \
				fdMapGridCopyCellAttributes(0, 0, 1, 1, cellX, cellZ);         \
				*timer = LIFT_RISEN;                                           \
			}                                                                  \
		}                                                                      \
		else if(*timer < LIFT_SINK) {                                          \
			u16 *phase = &phaseVar;                                            \
                                                                               \
			lift->y.fixed += sySin(*phase) >> 1;                               \
			*phase += TRIG_TURN / 16;                                          \
			if(*timer == LIFT_WALL_OFF)                                        \
				fdMapGridCopyCellAttributes(cellX, (cellZ) - 2, 1, 1, cellX,   \
											cellZ);                            \
			if(*timer > LIFT_FLOAT_END && *phase == 0) {                       \
				if(syGameFlagGet(onLift)) {                                    \
					fdMapGridCopyCellAttributes(carryCell, 0, 1, 1, cellX,     \
												cellZ);                        \
					syCloseEvent(name);                                        \
				}                                                              \
				else {                                                         \
					*timer = LIFT_SINK;                                        \
				}                                                              \
			}                                                                  \
		}                                                                      \
		else {                                                                 \
			lift->y.fixed += toFIXED(-4);                                      \
			scKandoreanTempleShrinkColumn(columnId, toFIXED(0.3),              \
										  toFIXED(0.3));                       \
			if(lift->y.fixed < -toFIXED(5))                                    \
				fdActorSetSprPriority(liftId, 3);                              \
			if(lift->y.fixed <= toFIXED(LIFT_SUNK_HEIGHT)) {                   \
				scQueueMusic(SE_AMBIENT_STREAM);                               \
				fdActorSetPos(liftId, 0, 0);                                   \
				fdActorSetPos(columnId, 0, 0);                                 \
				timerVar = 0;                                                  \
			}                                                                  \
		}                                                                      \
		timerVar++;                                                            \
	}

/* The lift room's west lift, the top of the west vent's column. */
DEFINE_KANDOREAN_TEMPLE_RUN_LIFT(KandoreanTemple_RunWestLift, ACTOR_WEST_LIFT,
								 ACTOR_WEST_LIFT_COLUMN,
								 KandoreanTemple_WestTimer,
								 KandoreanTemple_WestFloatPhase, 440, 204, 27,
								 12, 21, FLAG_ON_WEST_LIFT)

/* The lift room's east lift, the top of the east vent's column. */
DEFINE_KANDOREAN_TEMPLE_RUN_LIFT(KandoreanTemple_RunEastLift, ACTOR_EAST_LIFT,
								 ACTOR_EAST_LIFT_COLUMN,
								 KandoreanTemple_EastTimer,
								 KandoreanTemple_EastFloatPhase, 600, 236, 37,
								 14, 22, FLAG_ON_EAST_LIFT)

/* The vent room's task: notes whether the leader stands on a cover (where */
/* it cannot save), lets a blowing vent's steam push it south, and starts or */
/* stops the vents' sound. */
void scKandoreanTempleWatchVents(void) {
	ACTOR *leader;
	s32 cellX, cellZ, onCell;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	cellX = FIELD_CELL(leader->x.fixed);
	cellZ = FIELD_CELL(leader->z.fixed);
	onCell = 0;
	if(cellX == 13 && cellZ == 13) {
		fdEventSetFlag(FLAG_ON_WEST_LIFT);
		onCell = 1;
	}
	else {
		fdEventClearFlag(FLAG_ON_WEST_LIFT);
	}
	if(cellX == 23 && cellZ == 15) {
		fdEventSetFlag(FLAG_ON_EAST_LIFT);
		onCell = 1;
	}
	else {
		fdEventClearFlag(FLAG_ON_EAST_LIFT);
	}
	if(onCell)
		fdEventSetFlag(FLAG_NO_PLACE_TO_SAVE);
	else
		fdEventClearFlag(FLAG_NO_PLACE_TO_SAVE);
	if(KandoreanTemple_WestVentBlowing != 0 && leader->y.fixed == 0 &&
	   leader->x.fixed > toFIXED(284) && leader->x.fixed < toFIXED(308) &&
	   leader->z.fixed < toFIXED(200) && !fdEventGetFlag(FLAG_PILLAR_MOVING))
		scKandoreanTemplePushSouth(leader, toFIXED(2.4));
	if(KandoreanTemple_EastVentBlowing != 0 && leader->y.fixed == 0 &&
	   leader->x.fixed > toFIXED(330) && leader->x.fixed <= toFIXED(358) - 1 &&
	   leader->z.fixed < toFIXED(216) && !fdEventGetFlag(FLAG_PILLAR_MOVING))
		scKandoreanTemplePushSouth(leader, toFIXED(3.6));
	if(fdEventGetFlag(FLAG_VENT_SOUND_PLAYING)) {
		if(!fdEventGetFlag(FLAG_WEST_VENT_BLOWING) &&
		   !fdEventGetFlag(FLAG_EAST_VENT_BLOWING)) {
			scQueueMusic(-1);
			fdEventClearFlag(FLAG_VENT_SOUND_PLAYING);
		}
	}
	else if(fdEventGetFlag(FLAG_WEST_VENT_BLOWING) ||
			fdEventGetFlag(FLAG_EAST_VENT_BLOWING)) {
		scQueueMusic(SE_WAVE);
		fdEventSetFlag(FLAG_VENT_SOUND_PLAYING);
	}
}

/* The lift room's task: a leader stepping onto a lift rides it, and cannot */
/* save while it does. */
void scKandoreanTempleWatchLifts(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIXED x = leader->x.fixed;
	s32 cellX = FIELD_CELL(x);
	FIXED z = leader->z.fixed;
	s32 cellZ = FIELD_CELL(z);
	s32 riding = 0;

	if(leader->y.fixed > toFIXED(16))
		return;
	if((u32)(x - toFIXED(400)) > toFIXED(240))
		return;
	if((u32)(z - toFIXED(180)) > toFIXED(100))
		return;
	if(cellX == 27 && cellZ == 12) {
		ACTOR *lift = fdActorGetById(ACTOR_WEST_LIFT);
		fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId),
						   0);
		fdActorSetAnimationById(ACTOR_WEST_LIFT, 4);
		leader->motionFlg = riding;
		leader->y.fixed = lift->y.fixed;
		syGameFlagSet(FLAG_ON_WEST_LIFT);
		riding = 1;
	}
	if(cellX == 37 && cellZ == 14) {
		ACTOR *lift = fdActorGetById(ACTOR_EAST_LIFT);
		fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId),
						   0);
		fdActorSetAnimationById(ACTOR_EAST_LIFT, 4);
		leader->motionFlg = 0;
		leader->y.fixed = lift->y.fixed;
		syGameFlagSet(FLAG_ON_EAST_LIFT);
		riding = 1;
	}
	if(riding) {
		syGameFlagSet(FLAG_NO_PLACE_TO_SAVE);
		return;
	}
	fdActorSetAnimationById(ACTOR_WEST_LIFT, 0);
	fdActorSetAnimationById(ACTOR_EAST_LIFT, 0);
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId),
					   SPRITE_FLAG_SHADOW);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syGameFlagClear(FLAG_ON_WEST_LIFT);
	syGameFlagClear(FLAG_ON_EAST_LIFT);
	syGameFlagClear(FLAG_NO_PLACE_TO_SAVE);
}

/* Poi's room's task: draws the three tested students at the leader's sprite */
/* priority, in front of or behind the room's scenery as the leader is. */
void scKandoreanTempleMatchStudentsToLeader(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 priority = ((FIELD_SPR *)leader->spr)->pri;

	fdActorSetSprPriority(ACTOR_FIRST_TESTED_STUDENT, priority);
	fdActorSetSprPriority(ACTOR_SECOND_TESTED_STUDENT, priority);
	fdActorSetSprPriority(ACTOR_THIRD_TESTED_STUDENT, priority);
}

/* The vent room's task: a leader standing over a cell the map marks low drops
 */
/* to the ground. */
void scKandoreanTempleDropOffLowCells(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_CELL *cell = &map->layers[1].cells[fdActorGetCellIndex(leader)];

	if(cell->collisionCode != 0) {
		leader->y.fixed = 0;
		leader->groundHeight = 0;
	}
}

/* Reads the sign by the hot floor. */
void scKandoreanTempleReadPoisSign(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "Extinguish your desire, and the fire will cool. Empty your mind, and..."
	 */
	mnFieldMessageShow((s32)&MsgKandoreanTempleExtinguishYourDesire, 1);
	fdEventEnd();
}

/* Felix checks the temple's door in the middle of the courtyard, cells */
/* (17, 25) and (18, 25), before it is open: "The door is tightly locked." */
void scKandoreanTempleTryTheLockedDoor(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "The door is tightly locked." */
	mnFieldMessageShow((s32)&MsgKandoreanTempleDoorLocked, 1);
	fdEventEnd();
}

/* The student by the gate asks whether the party has come to see Master Poi. */
void scKandoreanTempleAskAboutPoi(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorStop(ACTOR_STUDENT_ASKING_ABOUT_POI);
	sySynch(1);
	fdActorFaceActorById(ACTOR_STUDENT_ASKING_ABOUT_POI,
						 MainRoutineStateWork.leaderActorId, 0);
	fdEventSetMessageId((s32)&MsgKandoreanTempleComeToSeePoi);
	/* "Have you come to see Master Poi?" */
	fdEventAskYesNoAndShowResponse(ACTOR_STUDENT_ASKING_ABOUT_POI, 0);
	fdActorGetById(ACTOR_STUDENT_ASKING_ABOUT_POI)->facing =
		FACING_SOUTH_SOUTHEAST;
	sySynch(1);
	fdActorSetMovementTypeById(ACTOR_STUDENT_ASKING_ABOUT_POI,
							   KandoreanTemple_LookAroundScript);
	sySynch(1);
	fdEventEnd();
	syGameFlagSet(FLAG_ASKED_ABOUT_POI);
}

/* The student on guard tells the party that none may enter during the */
/* meditation. */
void scKandoreanTempleTellOfTheMeditation(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorFaceActorById(ACTOR_STUDENT_ON_GUARD_DUTY,
						 MainRoutineStateWork.leaderActorId, 0);
	fdEventSetMessageId((s32)&MsgKandoreanTempleMeditationBegun);
	/* "Once the period of meditation has begun, none may enter the temple..."
	 */
	fdEventShowMessageAndWait(ACTOR_STUDENT_ON_GUARD_DUTY, 0);
	fdActorGetById(ACTOR_STUDENT_ON_GUARD_DUTY)->facing =
		FACING_SOUTH_SOUTHWEST;
	sySynch(1);
	fdEventEnd();
	syGameFlagSet(FLAG_TOLD_OF_THE_MEDITATION);
}

/* The first tested student asks whether the party passed the test. */
void scKandoreanTempleAskWhoPassedTheTest(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)&MsgKandoreanTemplePassedTheTest);
	/* "You were the ones who passed the test?" */
	fdEventAskYesNoAndShowResponse(ACTOR_FIRST_TESTED_STUDENT, 0);
	fdEventEnd();
}

/* Mind Read on the student by the gate startles him into asking about Poi. */
void scKandoreanTempleReadTheGateStudent(void) {
	fdActorShowEmoteById(ACTOR_STUDENT_ASKING_ABOUT_POI,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scKandoreanTempleAskAboutPoi();
}

/* Mind Read on the student on guard startles him into telling of the */
/* meditation. */
void scKandoreanTempleReadTheGuard(void) {
	fdActorShowEmoteById(ACTOR_STUDENT_ON_GUARD_DUTY,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scKandoreanTempleTellOfTheMeditation();
}

/* Felix pushes towards the opening in the floor of Poi's hall, cell (10, 5), */
/* from beside or below it: he climbs down the ladder into the Chestbeaters' */
/* room. */
void scKandoreanTempleClimbDownTheLadder(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(leader->x.fixed);
	s32 z = FIELD_CELL(leader->z.fixed);

	if(z == 5) {
		if(x == 9 && (InputWork.held & KEY_RIGHT) == 0)
			return;
		if(x == 11 && (InputWork.held & KEY_LEFT) == 0)
			return;
	}
	else if(x == 10 && z == 6 && (InputWork.held & KEY_UP) == 0) {
		return;
	}
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	leader->motionFlg = 0;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 168, 80);
	fdActorFaceDirectionAndRefreshById(MainRoutineStateWork.leaderActorId,
									   FACING_NORTH);
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	scFieldActorSetMoveTarget(leader, 168, -12, 80);
	fdActorWaitForMove(leader);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_CLIMB);
	sySndPlay(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_CHESTBEATERS);
}

/* Opens the door to the cave below Poi's statue. */
void scKandoreanTempleOpenTheCaveDoor(void) {
	fdMapGridCopyCellTilesIndicesRect(39, 39, 24, 38, 2, 2);
	sySynch(10);
	fdMapGridCopyCellTilesIndicesRect(39, 41, 24, 38, 2, 2);
	sySynch(20);
	syGameFlagSet(FLAG_CAVE_DOOR_OPEN);
}

/* Felix walks north into the door below Poi's statue, cells (24, 40) and */
/* (25, 40): it opens with a thump if it is shut, and he goes through into the
 */
/* caverns of the trials. */
void scKandoreanTempleEnterTheCave(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 400, 648);
	fdActorFaceDirectionAndRefreshById(MainRoutineStateWork.leaderActorId,
									   FACING_NORTH);
	if(syGameFlagGet(FLAG_CAVE_DOOR_OPEN) == 0) {
		sySndPlay(SE_THUMP);
		scKandoreanTempleOpenTheCaveDoor();
	}
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 3);
	fdActorWalkToById(MainRoutineStateWork.leaderActorId, 400, 640);
	sySndPlay(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_MIMIC);
}

/* Felix walks south into the stone well in the courtyard's east side, cells */
/* (25, 17) to (26, 18): he climbs down into it, to the passageway at its */
/* bottom that examining the well tells of, and comes out in the east end of */
/* the cave of shallow water. */
void scKandoreanTempleClimbDownTheWell(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	u32 frame;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 416, 288);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 10);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
	leader->motionFlg = 0;
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	for(frame = 0; frame < 8; frame++) {
		leader->z.fixed += toFIXED(1);
		sySynch(1);
	}
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	for(frame = 0; frame < 8; frame++) {
		leader->y.fixed -= toFIXED(1);
		sySynch(1);
	}
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_EAST);
}

/* Move takes hold of the pillar in the lift room, or Felix pushes it. */
void scKandoreanTempleStartMovingBlock(void) {
	fdActorLiftTargetBlock(KandoreanTemple_BlockActors);
}

/* The pillar in the lift room comes to rest; pushed east as far as column */
/* 43, over the shaft at cells (43, 10) to (43, 13), it blocks the shaft, and */
/* Felix can walk across without sinking into it. */
void scKandoreanTempleFinishMovingBlock(void) {
	s32 x = FIELD_CELL(fdActorGetById(ACTOR_LIFT_ROOM_PILLAR)->x.fixed);

	fdActorPlaceMovedBlock();
	if(x == 43)
		syGameFlagSet(FLAG_PILLAR_BLOCKS_SHAFT);
	else
		syGameFlagClear(FLAG_PILLAR_BLOCKS_SHAFT);
	if(x != 39)
		fdMapGridCopyCellAttributes(39, 0, 1, 1, 39, 10);
}

/* Move takes hold of the pillar in the vent room, or Felix pushes it; lifted */
/* off a vent's cover, that vent is free to blow again. */
void scKandoreanTempleStartMovingPillar(void) {
	if((u32)(FIELD_CELL(fdActorGetById(ACTOR_VENT_ROOM_PILLAR)->x.fixed) - 15) >
	   6)
		syGameFlagSet(FLAG_PILLAR_LIFTED_OFF_VENT);
	syGameFlagSet(FLAG_PILLAR_MOVING);
	fdActorLiftTargetBlock(KandoreanTemple_PillarActors);
}

/* The pillar in the vent room comes to rest. On the west vent's round cover, */
/* cell (13, 13), or the east one's, cell (23, 15), it weighs that vent down, */
/* and the other vent's column rises higher, up to the lift room. */
void scKandoreanTempleFinishMovingPillar(void) {
	ACTOR *actor = fdActorGetById(ACTOR_VENT_ROOM_PILLAR);
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);

	fdActorPlaceMovedBlock();
	if(x == 13 && z == 13)
		syGameFlagSet(FLAG_PILLAR_ON_WEST_VENT);
	else
		syGameFlagClear(FLAG_PILLAR_ON_WEST_VENT);
	if(x == 23 && z == 15)
		syGameFlagSet(FLAG_PILLAR_ON_EAST_VENT);
	else
		syGameFlagClear(FLAG_PILLAR_ON_EAST_VENT);
	syGameFlagClear(FLAG_PILLAR_LIFTED_OFF_VENT);
	syGameFlagClear(FLAG_PILLAR_MOVING);
}

/* Move takes hold of the pot boiling over the hot floor in the Chestbeaters' */
/* room. */
void scKandoreanTempleStartMovingPot(void) {
	syGameFlagClear(FLAG_POT_PUSHED_WEST);
	syGameFlagClear(FLAG_POT_PUSHED_EAST);
	fdActorLiftTargetBlock(KandoreanTemple_PillarActors);
}

/* The pot of boiling water, chucked one cell west or east by Move, lands out */
/* of the way, and the floor round where it stood, cells (19, 28) to (21, 30),
 */
/* stops burning. */
void scKandoreanTempleFinishMovingPot(void) {
	s32 x = FIELD_CELL(fdActorGetById(ACTOR_BOILING_POT)->x.fixed);

	fdActorPlaceMovedBlock();
	if(x == 19) {
		syGameFlagSet(FLAG_POT_PUSHED_WEST);
		syGameFlagSet(FLAG_POT_MOVED);
		fdMapCopyCellAttributeRect(0, 0, 3, 3, x, 28);
	}
	else if(x == 21) {
		syGameFlagSet(FLAG_POT_PUSHED_EAST);
		syGameFlagSet(FLAG_POT_MOVED);
		fdMapGridCopyCellAttributes(0, 4, 3, 3, 19, 28);
	}
}

/* Felix walks onto the hot floor under the pot of boiling water, from the */
/* west, south or east of it: he jumps back with a start, his sprite flushed */
/* red. */
void scKandoreanTempleTouchTheHotFloor(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	sySndPlay(SE_WATER);
	fdActorSetSprCol(fdActorGetById(MainRoutineStateWork.leaderActorId),
					 SPRITE_COLOR_RED_TINT);
	leader->velY = toFIXED(6);
	sySynch(20);
	while(leader->velY != 0)
		sySynch(1);
	fdActorSetSprCol(fdActorGetById(MainRoutineStateWork.leaderActorId),
					 SPRITE_COLOR_NORMAL);
	fdEventEnd();
}

/* Once Felix has met Poi, the temple's wooden door in the middle of the */
/* courtyard swings open with a thump as he walks up to it, from the courtyard
 */
/* (cell (17, 26)) or from inside (cell (17, 24)). Its cells, (17, 25) and */
/* (18, 25), stop answering "The door is tightly locked." and let him through.
 */
void scKandoreanTempleOpenTheTempleDoor(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_THUMP);
	fdMapGridCopyCellTilesIndicesRect(77, 33, 77, 24, 2, 2);
	fdMapGridCopyCellAttributes(32, 30, 2, 1, 17, 25);
	sySynch(10);
	syGameFlagSet(FLAG_TEMPLE_DOOR_OPEN);
	fdEventEnd();
}

/* Whirlwind has scattered the weeds: the passage behind them opens and Kraden
 */
/* follows the leader in. */
void scKandoreanTempleFindTheHiddenPassage(void) {
	ACTOR *actor;

	fdMapGridCopyCellAttributes(0, 0, 1, 1, 5, 13);
	syGameFlagSet(FLAG_HIDDEN_PASSAGE_FOUND);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorWalkToAndWaitById(ACTOR_FELIX, 89, 237);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorLaunchFromLeader(ACTOR_GATE_KRADEN, 16, 16, FACING_NORTHWEST);
	fdActorWaitForMoveById(ACTOR_GATE_KRADEN);
	fdEventWait(20);
	fdEventSetMessageId((s32)&MsgKandoreanTempleWatchfulEye);
	/* "Fantastic work, Felix! Keep a watchful eye for any strange-looking..."
	 */
	fdEventShowMessageAndWait(ACTOR_GATE_KRADEN, 0);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_GATE_KRADEN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_GATE_KRADEN, 1.2);
	fdActorSetAnimationById(ACTOR_GATE_KRADEN, ANIM_WALK);
	actor = fdActorGetById(ACTOR_FELIX);
	if(actor)
		fdActorSetDestinationById(ACTOR_GATE_KRADEN, actor->x.part[0].integer,
								  actor->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_GATE_KRADEN);
	fdActorSetPos(ACTOR_GATE_KRADEN, 0, 0);
	fdEventEnd();
}

/* Examining the Lash Pebble on Poi's table: Poi gives the party Lash, tells */
/* how to use it and crosses the north rope back to the upper hall. */
void scKandoreanTempleReceiveLash(void) {
	s16 angle =
		*(s16 *)&fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;
	ACTOR *actor;
	FIELD_SPR *sprite;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraGetActor()->motionFlg = 0;
	((void (*)())fdActorFaceDirectionById)(ACTOR_POI, FACING_EAST);
	fdEventSetMessageId((s32)&MsgKandoreanTempleSecretOfLash);
	/* "This is the power and the secret of our temple!" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_POI, 0);
	fdActorSetAnimation(ACTOR_POI, ANIM_NOD);
	/* "If you are worthy, you will use it with ease!" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	fdActorSetAnimation(ACTOR_POI, ANIM_NOD);
	/* "When you see a rope, use Lash on it..." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	fdActorSetAnimation(ACTOR_POI, ANIM_SHAKE_HEAD);
	/* "Perhaps you will find this useful on your travels." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_POI, ANIM_NOD);
	/* "Take this power with you, and do good." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, (u16)angle,
							 10);
	if(KandoreanTemple_LashPebble != 0)
		fdItemDispShowPicture(KandoreanTemple_LashPebble,
							  ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	gmPartyGiveItem(ITEM_LASH_PEBBLE, 0);
	if(KandoreanTemple_LashPebble != 0)
		fdActorRelease(KandoreanTemple_LashPebble);
	fdActorSetAnimation(ACTOR_POI, ANIM_SHAKE_HEAD);
	/* "I hope that someday, my students will be worthy of this honor, too." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	/* Poi hops onto the north rope, slides back across it tilted, hops off its
	 */
	/* east end and walks back into the upper hall. */
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(424, 0, 400, 1);
	actor = fdActorGetById(ACTOR_POI);
	sprite = actor->spr;
	fdActorSetSpeed(ACTOR_POI, 0.8);
	fdActorWalkToAndWait(ACTOR_POI, 424, 376);
	scFieldActorSetSprPriority(ACTOR_POI, 1);
	fdActorSetSpeed(ACTOR_POI, 1.6);
	scFieldActorJump(ACTOR_POI, 4, 0);
	fdActorMoveToAndWait(ACTOR_POI, 444, 376);
	fdActorSetAnimation(ACTOR_POI, 7);
	actor->scaleX = -FIXED_ONE;
	sprite->rotation = TRIG_TURN * 7 / 8;
	actor->motionFlg = 0;
	actor->y.fixed += toFIXED(4);
	fdActorSetSprFlags(fdActorGetById(ACTOR_POI), 0);
	fdActorSetSpeed(ACTOR_POI, 0.4);
	scFieldActorSetMoveTarget(actor, 492, 8, 376);
	scFieldActorWaitForMoveById(ACTOR_POI);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	actor->groundHeight = 0;
	actor->scaleX = FIXED_ONE;
	sprite->rotation = 0;
	fdActorSetAnimation(ACTOR_POI, ANIM_STAND);
	fdActorSetSprFlags(fdActorGetById(ACTOR_POI), SPRITE_FLAG_SHADOW);
	fdActorSetSpeed(ACTOR_POI, 1.6);
	scFieldActorJump(ACTOR_POI, 4, 0);
	fdActorMoveToAndWait(ACTOR_POI, 504, 376);
	fdActorSetSpeed(ACTOR_POI, 0.8);
	fdActorWalkToAndWait(ACTOR_POI, 496, 394);
	fdActorWalkToAndWait(ACTOR_POI, 432, 394);
	scFieldActorWatchAndSetScript(ACTOR_POI,
								  MainRoutineStateWork.leaderActorId);
	scFieldActorSetSprPriority(ACTOR_POI, 2);
	fdEventSetFlag(FLAG_LASH_RECEIVED);
	fdEventEnd();
}

/* Nothing visible happens as Felix steps behind a post of the rope across to */
/* Fog; he is drawn behind the post (`fdActorSetSprPriority`) while he is */
/* there. */
void scKandoreanTempleStepBehindThePost(void) {
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
}

/* Nothing visible happens as Felix steps out from behind the rope's post; he */
/* is drawn in front of it again. */
void scKandoreanTempleStepOutFromThePost(void) {
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg &=
		~ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_AUTOMATIC;
}

/* Felix walks up to the Chestbeaters at the north end of their room, cells */
/* (19, 12) to (21, 12): the three leap down at him with a cry and their */
/* battle starts. */
void scKandoreanTempleFightChestbeaters(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(328, CAMERA_KEEP, 174, 1);
	fdCameraWaitForArrival();
	fdEventWait(20);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 328, 174);
	fdActorRunRepeatedMotion(ACTOR_MIDDLE_CHESTBEATER, 2);
	sySndPlay(SE_ENEMY_CRY);
	fdActorSetAnimationById(ACTOR_MIDDLE_CHESTBEATER, 2);
	fdActorGetById(ACTOR_MIDDLE_CHESTBEATER)->velY = toFIXED(4);
	fdEventWait(10);
	sySndPlay(SE_ENEMY_CRY);
	fdActorSetAnimationById(ACTOR_WEST_CHESTBEATER, 2);
	fdActorSetAnimationById(ACTOR_EAST_CHESTBEATER, 2);
	fdActorGetById(ACTOR_WEST_CHESTBEATER)->velY = toFIXED(6);
	fdActorGetById(ACTOR_EAST_CHESTBEATER)->velY = toFIXED(6);
	fdActorSetDestinationById(ACTOR_WEST_CHESTBEATER, 300, 164);
	fdActorMoveToAndWaitById(ACTOR_EAST_CHESTBEATER, 356, 164);
	fdEventWait(40);
	btSetType(BATTLE_TYPE_NO_ESCAPE);
	fdEventSetAfterBattleDestination(SCENE_KANDOREAN_TEMPLE_CHESTBEATERS,
									 ENTRANCE_AFTER_CHESTBEATER_BATTLE);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 4);
	fdEventEnd();
}

/* Felix casts Cyclone over one of the five clumps of weeds in the courtyard */
/* before the temple: leaves whirl up and the clump is gone. The first covers */
/* cells (13, 14) to (14, 15); the others one cell each, (15, 16), (14, 18), */
/* (20, 18) and (27, 15). */
void scKandoreanTempleCycloneFirstClump(void) {
	syFieldTileCopiesCopyTouchedCells(76, 0, 2, 2, 73, 14, 1);
}

void scKandoreanTempleCycloneSecondClump(void) {
	syFieldTileCopiesCopyTouchedCells(76, 0, 1, 1, 75, 16, 1);
}

void scKandoreanTempleCycloneThirdClump(void) {
	syFieldTileCopiesCopyTouchedCells(76, 0, 1, 1, 74, 18, 1);
}

void scKandoreanTempleCycloneFourthClump(void) {
	syFieldTileCopiesCopyTouchedCells(76, 0, 1, 1, 80, 18, 1);
}

void scKandoreanTempleCycloneFifthClump(void) {
	syFieldTileCopiesCopyTouchedCells(76, 0, 1, 1, 87, 15, 1);
}

/* Sets up the room being entered. */
s32 scInit(void) {
	s32 scene;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_KANDOREAN_TEMPLE_LIFTS)
		scKandoreanTempleStartLiftRoom();
	else if(scene == SCENE_KANDOREAN_TEMPLE_MIMIC)
		scKandoreanTempleStartMimicRoom();
	else if(scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
		scKandoreanTempleStartChestbeaterRoom();
	else if(scene == SCENE_KANDOREAN_TEMPLE_PILLAR)
		scKandoreanTempleStartVentRoom();
	else if(scene == SCENE_KANDOREAN_TEMPLE_STREAM)
		scKandoreanTempleStartStreamRoom();
	else if(scene == SCENE_KANDOREAN_TEMPLE_CURRENT)
		scKandoreanTempleStartCurrentRoom();
	else if(scene == SCENE_KANDOREAN_TEMPLE_FOG)
		scKandoreanTempleStartFogRoom();
	else if(scene == SCENE_KANDOREAN_TEMPLE_GATE)
		scKandoreanTempleStartGate();
	else if(scene == SCENE_KANDOREAN_TEMPLE_POI)
		scKandoreanTempleStartPoisRoom();
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Sets up the lift room: its tightropes, the pillar, the stream of steam and */
/* the lifts, and the lift the leader arrives on. */
void scKandoreanTempleStartLiftRoom(void) {
	fdTightropeInit();
	fdTightropePlace(0, ACTOR_WEST_ROPE_WEST_POST, ACTOR_WEST_ROPE_EAST_POST);
	fdTightropePlace(1, ACTOR_EAST_ROPE_WEST_POST, ACTOR_EAST_ROPE_EAST_POST);
	fdActorSetAnimationById(ACTOR_WEST_ROPE_WEST_POST, 4);
	fdActorSetAnimationById(ACTOR_WEST_ROPE_EAST_POST, 4);
	fdActorSetAnimationById(ACTOR_EAST_ROPE_WEST_POST, 4);
	fdActorSetAnimationById(ACTOR_EAST_ROPE_EAST_POST, 4);
	fdActorPlaceBlocks(KandoreanTemple_BlockActors);
	REG_BLDALPHA = BLEND_ALPHA(8, 12);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	scQueueMusic(SE_AMBIENT_STREAM);
	sySetEvent(scKandoreanTempleBlowStream, TASK_PASS_FRAME);
	fdActorSetPos(ACTOR_WEST_LIFT, 0, 0);
	fdActorSetPos(ACTOR_EAST_LIFT, 0, 0);
	fdActorSetPos(ACTOR_WEST_LIFT_COLUMN, 0, 0);
	fdActorSetPos(ACTOR_EAST_LIFT_COLUMN, 0, 0);
	scKandoreanTemplePrepareLift(ACTOR_WEST_LIFT);
	scKandoreanTemplePrepareLift(ACTOR_EAST_LIFT);
	scKandoreanTemplePrepareColumn(ACTOR_WEST_LIFT_COLUMN);
	scKandoreanTemplePrepareColumn(ACTOR_EAST_LIFT_COLUMN);
	KandoreanTemple_WestTimer = 0;
	KandoreanTemple_EastTimer = 0;
	KandoreanTemple_WestFloatPhase = 0;
	KandoreanTemple_EastFloatPhase = 0;
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		if(MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_WEST_LIFT)
			scKandoreanTempleArriveOnWestLift();
		else if(MainRoutineStateWork.entrance ==
				KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_EAST_LIFT)
			scKandoreanTempleArriveOnEastLift();
	}
	if(syGameFlagGet(FLAG_PILLAR_ON_EAST_VENT))
		sySetEvent(KandoreanTemple_RunWestLift, TASK_PASS_FRAME);
	else if(syGameFlagGet(FLAG_PILLAR_ON_WEST_VENT))
		sySetEvent(KandoreanTemple_RunEastLift, TASK_PASS_FRAME);
	sySetEvent(scKandoreanTempleWatchLifts, TASK_PASS_FRAME);
	fdEffectStartBlankOverlay(0);
}

/* Sets up the Mimic's room. Outside the Japanese edition, coming into it */
/* from Poi's hall does not show the area's name again. */
void scKandoreanTempleStartMimicRoom(void) {
	fdEffectStartBlankOverlay(0);
#if TLA_PATCH_1 /* From patch 1. */
	if(MainRoutineStateWork.entrance ==
	   KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_POI)
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
#endif
}

/* Sets up the Chestbeaters' room: the pot where Move left it, the sign, and */
/* the Chestbeaters gone once the ladder is lowered. */
void scKandoreanTempleStartChestbeaterRoom(void) {
	ACTOR *actor = fdActorGetById(ACTOR_EMBERS_UNDER_THE_POT);

	fdActorDisableById(ACTOR_EMBERS_UNDER_THE_POT);
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetAnimationById(ACTOR_EMBERS_UNDER_THE_POT, 2);
	actor = fdActorGetById(ACTOR_BOILING_POT);
	actor->motionFlg = 0;
	if(syGameFlagGet(FLAG_POT_PUSHED_WEST)) {
		scFieldActorSetPosAndResetMotion(fdActorGetById(ACTOR_BOILING_POT),
										 toFIXED(312), toFIXED(32),
										 toFIXED(472));
		fdMapGridCopyCellAttributes(0, 0, 3, 3, 19, 28);
	}
	else if(syGameFlagGet(FLAG_POT_PUSHED_EAST)) {
		scFieldActorSetPosAndResetMotion(fdActorGetById(ACTOR_BOILING_POT),
										 toFIXED(344), toFIXED(32),
										 toFIXED(472));
		fdMapGridCopyCellAttributes(0, 4, 3, 3, 19, 28);
	}
	fdActorPlaceBlocks(KandoreanTemple_PillarActors);
	actor = fdActorGetById(ACTOR_POIS_SIGN);
	actor->scaleX = FIXED_ONE * 7 / 10;
	actor->scaleY = FIXED_ONE * 7 / 10;
	actor->collisionFlg = 0;
	fdEffectStartBlankOverlay(0);
	if(fdEventGetFlag(FLAG_LADDER_LOWERED)) {
		{
			s32 e = 1;
			s32 f = 4;

			fdMapGridCopyCellTilesIndicesRect(41, 0, 20, 0, e, f);
		}
		fdActorSetPos(ACTOR_MIDDLE_CHESTBEATER, 0, 0);
		fdActorSetPos(ACTOR_WEST_CHESTBEATER, 0, 0);
		fdActorSetPos(ACTOR_EAST_CHESTBEATER, 0, 0);
	}
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		if(MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_POI) {
			fdActorGetById(MainRoutineStateWork.leaderActorId)->y.fixed =
				toFIXED(64);
		}
		else if(MainRoutineStateWork.entrance ==
					ENTRANCE_AFTER_CHESTBEATER_BATTLE &&
				fdEventGetFlag(FLAG_LADDER_LOWERED) == 0) {
			fdActorSetPos(ACTOR_MIDDLE_CHESTBEATER, 0, 0);
			fdActorSetPos(ACTOR_WEST_CHESTBEATER, 0, 0);
			fdActorSetPos(ACTOR_EAST_CHESTBEATER, 0, 0);
			scKandoreanTempleLowerTheLadder();
		}
	}
#if TLA_PATCH_1
	if(MainRoutineStateWork.entrance ==
	   KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_POI)
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
#endif
}

/* Sets up the vent room: the covers and columns at rest, the pillar where */
/* Move left it, the vents' steam and the tasks that run them, and the ride */
/* down or the fall the leader arrives by. */
void scKandoreanTempleStartVentRoom(void) {
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		scFieldActorSetPosAndResetMotion(fdActorGetById(ACTOR_WEST_VENT_COVER),
										 toFIXED(216), 0, toFIXED(216));
		fdActorSetPos(ACTOR_WEST_COLUMN_TOP, 0, 0);
		fdActorSetPos(ACTOR_WEST_COLUMN_UPPER_PIECE, 0, 0);
		fdActorSetPos(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, 0);
		fdActorSetPos(ACTOR_WEST_COLUMN_LOWER_PIECE, 0, 0);
		fdActorSetPos(ACTOR_WEST_COLUMN_FOOT, 0, 0);
		fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_EAST_VENT_COVER),
									toFIXED(376), 0, toFIXED(248));
		fdActorSetPos(ACTOR_EAST_COLUMN_TOP, 0, 0);
		fdActorSetPos(ACTOR_EAST_COLUMN_UPPER_PIECE, 0, 0);
		fdActorSetPos(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0, 0);
		fdActorSetPos(ACTOR_EAST_COLUMN_LOWER_PIECE, 0, 0);
		fdActorSetPos(ACTOR_EAST_COLUMN_FOOT, 0, 0);
		syGameFlagClear(FLAG_VENT_SOUND_PLAYING);
		syGameFlagClear(FLAG_WEST_VENT_BLOWING);
		syGameFlagClear(FLAG_EAST_VENT_BLOWING);
	}
	if(syGameFlagGet(FLAG_PILLAR_ON_WEST_VENT))
		fdActorSetPos(ACTOR_VENT_ROOM_PILLAR, 216, 216);
	else if(syGameFlagGet(FLAG_PILLAR_ON_EAST_VENT))
		fdActorSetPos(ACTOR_VENT_ROOM_PILLAR, 376, 248);
	REG_BLDALPHA = BLEND_ALPHA(8, 12);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	KandoreanTemple_WestVentBlowing = 0;
	KandoreanTemple_EastVentBlowing = 0;
	syTaskAddFrame(KandoreanTemple_BlowWestVent, TASK_PASS_FRAME);
	syTaskAddFrame(KandoreanTemple_BlowEastVent, TASK_PASS_FRAME);
	sySetEvent(scKandoreanTempleDropOffLowCells, TASK_PASS_FRAME);
	scKandoreanTemplePrepareLift(ACTOR_WEST_VENT_COVER);
	scKandoreanTemplePrepareLift(ACTOR_EAST_VENT_COVER);
	scKandoreanTemplePrepareColumn(ACTOR_WEST_COLUMN_TOP);
	scKandoreanTemplePrepareColumn(ACTOR_WEST_COLUMN_UPPER_PIECE);
	scKandoreanTemplePrepareColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE);
	scKandoreanTemplePrepareColumn(ACTOR_WEST_COLUMN_LOWER_PIECE);
	scKandoreanTemplePrepareColumn(ACTOR_WEST_COLUMN_FOOT);
	scKandoreanTemplePrepareColumn(ACTOR_EAST_COLUMN_TOP);
	scKandoreanTemplePrepareColumn(ACTOR_EAST_COLUMN_UPPER_PIECE);
	scKandoreanTemplePrepareColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE);
	scKandoreanTemplePrepareColumn(ACTOR_EAST_COLUMN_LOWER_PIECE);
	scKandoreanTemplePrepareColumn(ACTOR_EAST_COLUMN_FOOT);
	fdActorPlaceBlocks(KandoreanTemple_PillarActors);
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		if(MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_WEST_LIFT)
			scKandoreanTempleArriveOnWestVent();
		else if(MainRoutineStateWork.entrance ==
				KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_EAST_LIFT)
			scKandoreanTempleArriveOnEastVent();
	}
	KandoreanTemple_WestTimer = 0;
	/* The east vent starts at rest, out of step with the west. */
	KandoreanTemple_EastTimer = VENT_SHUT + 1000;
	KandoreanTemple_WestFloatPhase = 0;
	KandoreanTemple_EastFloatPhase = 0;
	syTaskAddFrame(KandoreanTemple_RunWestVent, TASK_PASS_FRAME);
	sySetEvent(KandoreanTemple_RunEastVent, TASK_PASS_FRAME);
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION) &&
	   MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_SHAFT)
		scKandoreanTempleLandFromAbove();
	syTaskAddFrame(scKandoreanTempleWatchVents, TASK_PASS_FRAME);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 2);
	fdEffectStartBlankOverlay(0);
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_KANDOREAN_TEMPLE_VENT
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
/* The Kandorean Temple's events, from the vents' rides to Poi's test, and its
 */
/* tables. */

/* Felix drops into the vent room from the lift room's shaft above and lands in
 */
/* a puff of dust that shakes the screen. */
void scKandoreanTempleLandFromAbove(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	EFFECT_OPTIONS options;
	u32 particleIndex;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	sySndPlay(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdEventWait(24);
	options.pal = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = FIXED_ONE * 4 / 5;
	options.startScaleY = FIXED_ONE * 4 / 5;
	for(particleIndex = 0; particleIndex <= DUST_RING_PUFFS; particleIndex++) {
		FIXED velocity[3];

		velocity[0] = syCos(particleIndex * (TRIG_TURN / DUST_RING_PUFFS));
		velocity[1] = 0;
		velocity[2] = sySin(particleIndex * (TRIG_TURN / DUST_RING_PUFFS));
		velocity[0] += velocity[0] >> 1;
		velocity[0] += syRandomBelow(toFIXED(3) / 4);
		velocity[0] += -toFIXED(3) / 8;
		velocity[2] += syRandomBelow(toFIXED(3) / 8) + -toFIXED(3) / 16;
		scFieldEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
						   velocity[0], velocity[1], velocity[2],
						   EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
							   EFFECT_USE_PALETTE | 1,
						   &options);
	}
	sySndPlay(SE_THUMP);
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_SIT);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	leader->gravity = toFIXED(1);
	leader->bounce = FIXED_ONE / 4;
	fdEventWait(10);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdEventEnd();
}

/* One step of a steam column sinking back under its cover: the leader, the */
/* cover and the camera go down, and each of the column's five pieces shrinks,
 */
/* the top fastest. */
#define SINK_COVER(top, upper, middle, lower, foot)                            \
	do {                                                                       \
		leader->y.fixed += -toFIXED(27) / 4;                                   \
		cover->y.fixed += -toFIXED(27) / 4;                                    \
		camera->y.fixed += -toFIXED(27) / 10;                                  \
		limit = FIXED_ONE * 3 / 32;                                            \
		scKandoreanTempleShrinkColumn(top, toFIXED(3), limit);                 \
		scKandoreanTempleShrinkColumn(upper, toFIXED(9) / 4, limit);           \
		scKandoreanTempleShrinkColumn(middle, toFIXED(3) / 2, limit);          \
		scKandoreanTempleShrinkColumn(lower, toFIXED(3) / 4, limit);           \
		scKandoreanTempleShrinkColumn(foot, 0, limit);                         \
		sySynch(1);                                                            \
	} while(fdActorGetById(foot)->scaleY > limit);

/* Coming down a lift from the room above: the party arrives atop that vent's */
/* cover, raised high on its column, and sinks with it to the ground. It now */
/* rides this side's lift and the pillar weighs the other vent down: */
/* `westSide` is syGameFlagSet on the west lift's arrival and syGameFlagClear */
/* on the east's, `eastSide` the other way round. */
#define DEFINE_KANDOREAN_TEMPLE_ARRIVE_ON_VENT(                                \
	name, coverId, coverX, coverZ, top, upper, middle, lower, foot, westSide,  \
	eastSide, start, blowingVar, blowingFlag)                                  \
	void name(void) {                                                          \
		ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);    \
		ACTOR *cover = fdActorGetById(coverId);                                \
		ACTOR *camera = fdCameraGetActor();                                    \
		s16 i;                                                                 \
		u16 next;                                                              \
		s32 limit;                                                             \
                                                                               \
		fdEventStart();                                                        \
		fdEventEndLastingPsynergy(0);                                          \
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);              \
		fdCameraGetActor()->motionFlg = 0;                                     \
		leader->motionFlg = 0;                                                 \
		sySynch(1);                                                            \
		westSide(FLAG_ON_WEST_LIFT);                                           \
		eastSide(FLAG_ON_EAST_LIFT);                                           \
		westSide(FLAG_PILLAR_ON_EAST_VENT);                                    \
		eastSide(FLAG_PILLAR_ON_WEST_VENT);                                    \
		sySndPlay(SE_WAVE);                                                    \
		start();                                                               \
		i = 0;                                                                 \
		do {                                                                   \
			leader->y.fixed += toFIXED(9) / 2;                                 \
			cover->y.fixed += toFIXED(9) / 2;                                  \
			camera->y.fixed += toFIXED(9) / 5;                                 \
			limit = FIXED_ONE / 16;                                            \
			scKandoreanTempleGrowColumn(top, toFIXED(2), limit);               \
			scKandoreanTempleGrowColumn(upper, toFIXED(3) / 2, limit);         \
			scKandoreanTempleGrowColumn(middle, toFIXED(1), limit);            \
			scKandoreanTempleGrowColumn(lower, toFIXED(1) / 2, limit);         \
			scKandoreanTempleGrowColumn(foot, 0, limit);                       \
			next = i;                                                          \
			next++;                                                            \
			i = next;                                                          \
		} while(next < VENT_RISE_END - VENT_BLOW);                             \
		fdMapGridCenterOnFollowedPoint();                                      \
		sySynch(1);                                                            \
		FieldStateWork->transition =                                           \
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);     \
		fdEventStartScrTransition();                                           \
		fdEventWaitTransitionFrames();                                         \
		fdEventWait(10);                                                       \
		SINK_COVER(top, upper, middle, lower, foot)                            \
		blowingVar = 0;                                                        \
		syGameFlagClear(blowingFlag);                                          \
		sySndPlay(SOUND_FADE_OUT_ACTION_SE);                                   \
		fdActorSetPos(top, 0, 0);                                              \
		fdActorSetPos(upper, 0, 0);                                            \
		fdActorSetPos(middle, 0, 0);                                           \
		fdActorSetPos(lower, 0, 0);                                            \
		fdActorSetPos(foot, 0, 0);                                             \
		fdActorSetAnimationById(coverId, 1);                                   \
		scFieldActorSetPosAndResetMotion(fdActorGetById(coverId),              \
										 toFIXED(coverX), 0, toFIXED(coverZ)); \
		cover->priFlg |= ACTOR_PRIORITY_UNDERFOOT;                             \
		fdActorSetSprPriority(coverId, 3);                                     \
		fdActorSetPosAndResetMotion(leader, leader->x.fixed, 0,                \
									leader->z.fixed);                          \
		fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), \
						   SPRITE_FLAG_SHADOW);                                \
		sySynch(1);                                                            \
		FieldStateWork->transition = SCENE_TRANSITION(                         \
			TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);             \
		fdEventEnd();                                                          \
	}

DEFINE_KANDOREAN_TEMPLE_ARRIVE_ON_VENT(
	scKandoreanTempleArriveOnWestVent, ACTOR_WEST_VENT_COVER, 216, 216,
	ACTOR_WEST_COLUMN_TOP, ACTOR_WEST_COLUMN_UPPER_PIECE,
	ACTOR_WEST_COLUMN_MIDDLE_PIECE, ACTOR_WEST_COLUMN_LOWER_PIECE,
	ACTOR_WEST_COLUMN_FOOT, syGameFlagSet, syGameFlagClear,
	KandoreanTemple_StartWestVent, KandoreanTemple_WestVentBlowing,
	FLAG_WEST_VENT_BLOWING)

DEFINE_KANDOREAN_TEMPLE_ARRIVE_ON_VENT(
	scKandoreanTempleArriveOnEastVent, ACTOR_EAST_VENT_COVER, 376, 248,
	ACTOR_EAST_COLUMN_TOP, ACTOR_EAST_COLUMN_UPPER_PIECE,
	ACTOR_EAST_COLUMN_MIDDLE_PIECE, ACTOR_EAST_COLUMN_LOWER_PIECE,
	ACTOR_EAST_COLUMN_FOOT, syGameFlagClear, syGameFlagSet,
	KandoreanTemple_StartEastVent, KandoreanTemple_EastVentBlowing,
	FLAG_EAST_VENT_BLOWING)

/* Felix rises out of the vent room on a vent's column, the screen closing in */
/* on him. */
void scKandoreanTempleLeaveBySteam(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
}

/* Felix rides the west vent's round cover as its column lifts it, up through */
/* the ceiling to the west lift in the room above. */
void scKandoreanTempleLeaveByWestVent(void) {
	scKandoreanTempleLeaveBySteam();
	syImportEventSetExitRequest(
		KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_WEST_LIFT);
}

/* Felix rides the east vent's round cover as its column lifts it, up through */
/* the ceiling to the east lift in the room above. */
void scKandoreanTempleLeaveByEastVent(void) {
	scKandoreanTempleLeaveBySteam();
	syImportEventSetExitRequest(
		KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_EAST_LIFT);
}

/* Felix steps onto the lift room's shaft, cells (43, 10) to (43, 13), while */
/* the pillar does not block it: he slips back, throws up his arms and sinks */
/* out of sight, dropping into the vent room below. */
void scKandoreanTempleSinkIntoTheSteam(void) {
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	u32 frame;
	MAIN_ROUTINE_STATE *party;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	actor->motionFlg = 0;
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	actor->facing = 0;
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, 16);
	for(frame = 0; frame <= 7; frame++) {
		fdActorSetPosAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
									actor->z.fixed + toFIXED(3));
		sySynch(1);
	}
	fdEventWait(20);
	party = &MainRoutineStateWork;
	actor->facing = FACING_SOUTH;
	fdEffectSetAttached(party->leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAnimationById(party->leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
	fdEventWait(30);
	fdActorSetAnimationById(party->leaderActorId, ANIM_LEADER_ARMS_RAISED);
	fdEventWait(10);
	fdActorSetSprPriority(party->leaderActorId, 3);
	for(frame = 0; frame <= 39; frame++) {
		{
			s32 scale = actor->scaleX;

			actor->scaleX = scale + -FIXED_ONE / 64;
		}
		actor->scaleY += -FIXED_ONE / 64;
		actor->y.fixed += -toFIXED(2);
		sySynch(1);
		if(frame == 10)
			sySndPlay(SE_DUST_PUFF);
		if(frame == 20) {
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
			fdEventFinishScrTransition();
		}
	}
	syImportEventSetExitRequest(
		KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_SHAFT);
}

/* Felix rises into view on a lift while its column grows under it. */
#define DEFINE_KANDOREAN_TEMPLE_RISE(name, liftId, columnId, x, z, liftZ,      \
									 cellX, cellZ, timer)                      \
	void name(void) {                                                          \
		ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);    \
		ACTOR *first = fdActorGetById(liftId);                                 \
		ACTOR *second = fdActorGetById(columnId);                              \
                                                                               \
		fdEventStart();                                                        \
		fdEventEndLastingPsynergy(0);                                          \
		scQueueMusic(SE_WAVE);                                                 \
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);              \
		sySynch(1);                                                            \
		leader->motionFlg = 0;                                                 \
		scFieldActorSetPosAndResetMotion(                                      \
			fdActorGetById(MainRoutineStateWork.leaderActorId), toFIXED(x),    \
			toFIXED(LIFT_SUNK_HEIGHT), toFIXED(z));                            \
		fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), \
						   0);                                                 \
		scFieldActorSetPosAndResetMotion(fdActorGetById(liftId), toFIXED(x),   \
										 toFIXED(LIFT_SUNK_HEIGHT),            \
										 toFIXED(liftZ));                      \
		scFieldActorSetPosAndResetMotion(fdActorGetById(columnId), toFIXED(x), \
										 toFIXED(LIFT_SUNK_HEIGHT),            \
										 toFIXED(z));                          \
		fdActorSetSprPriority(liftId, 2);                                      \
		fdActorSetSprPriority(columnId, 3);                                    \
		{                                                                      \
			u8 *flags = &second->priFlg;                                       \
			s32 c = ACTOR_PRIORITY_UNDERFOOT | *flags;                         \
                                                                               \
			*flags = c;                                                        \
		}                                                                      \
		second->scaleY = FIXED_ONE / 64;                                       \
		fdActorSetAnimationById(liftId, 4);                                    \
		fdActorSetAnimationById(columnId, 2);                                  \
		FieldStateWork->transition =                                           \
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);     \
		fdEventStartScrTransition();                                           \
		fdEventWaitTransitionFrames();                                         \
		do {                                                                   \
			first->y.fixed += toFIXED(4);                                      \
			leader->y.fixed += toFIXED(4);                                     \
			scKandoreanTempleGrowColumn(columnId, toFIXED(0.3), toFIXED(0.3)); \
			sySynch(1);                                                        \
		} while(first->y.fixed < 0);                                           \
		fdMapGridCopyCellAttributes(0, 0, 1, 1, cellX, cellZ);                 \
		timer = LIFT_RISEN;                                                    \
		FieldStateWork->transition = SCENE_TRANSITION(                         \
			TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);             \
		fdEventEnd();                                                          \
	}

/* Arriving on the west lift, cell (27, 12), from the vent room below: Felix */
/* rises into the room on it as its column grows beneath, and the lift's cycle
 */
/* goes on from LIFT_RISEN. */
DEFINE_KANDOREAN_TEMPLE_RISE(scKandoreanTempleArriveOnWestLift, ACTOR_WEST_LIFT,
							 ACTOR_WEST_LIFT_COLUMN, 440, 200, 204, 27, 12,
							 KandoreanTemple_WestTimer)

/* Arriving on the east lift, cell (37, 14), from the vent room below: Felix */
/* rises into the room on it as its column grows beneath. */
DEFINE_KANDOREAN_TEMPLE_RISE(scKandoreanTempleArriveOnEastLift, ACTOR_EAST_LIFT,
							 ACTOR_EAST_LIFT_COLUMN, 600, 232, 236, 37, 14,
							 KandoreanTemple_EastTimer)

/* Felix steps onto the west lift in the lift room, cell (27, 12), while it */
/* stands risen: it sinks out of sight with him, down its column to the west */
/* vent in the room below. */
void scKandoreanTempleRideWestLiftDown(void) {
	ACTOR *actor = fdActorGetById(ACTOR_WEST_LIFT);

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	do {
		actor->y.fixed -= toFIXED(4);
		scKandoreanTempleShrinkColumn(ACTOR_WEST_LIFT_COLUMN, toFIXED(0.3),
									  toFIXED(0.3));
		sySynch(1);
	} while(actor->y.fixed > toFIXED(LIFT_SUNK_HEIGHT));
	fdActorSetPos(ACTOR_WEST_LIFT_COLUMN, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_WEST_LIFT);
}

/* Felix steps onto the east lift in the lift room, cell (37, 14), while it */
/* stands risen: it sinks out of sight with him, down its column to the east */
/* vent in the room below. */
void scKandoreanTempleRideEastLiftDown(void) {
	ACTOR *actor = fdActorGetById(ACTOR_EAST_LIFT);

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	do {
		actor->y.fixed -= toFIXED(4);
		scKandoreanTempleShrinkColumn(ACTOR_EAST_LIFT_COLUMN, toFIXED(0.3),
									  toFIXED(0.3));
		sySynch(1);
	} while(actor->y.fixed > toFIXED(LIFT_SUNK_HEIGHT));
	fdActorSetPos(ACTOR_EAST_LIFT_COLUMN, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_EAST_LIFT);
}

/* Sets up the cave of shallow water behind the bush by the gate, where the */
/* party wades. */
void scKandoreanTempleStartStreamRoom(void) {
	MAP_STATE_GROUND_EFFECTS *map =
		(MAP_STATE_GROUND_EFFECTS *)Ram_HeapSlots->mapGrid;

	KandoreanTemple_SplashRingCounter = 0;
	map->wading = 1;
	fdEffectStartBlankOverlay(0);
#if TLA_PATCH_1
	if(MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_STREAM_FROM_KANDOREAN_TEMPLE_GATE_WEST ||
	   MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_STREAM_FROM_KANDOREAN_TEMPLE_GATE_EAST)
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
#endif
}

/* Sets up the room of the current, with the rush of its water. */
void scKandoreanTempleStartCurrentRoom(void) {
	MAP_STATE_GROUND_EFFECTS *map =
		(MAP_STATE_GROUND_EFFECTS *)Ram_HeapSlots->mapGrid;

	KandoreanTemple_SplashRingCounter = 0;
	scQueueMusic(SE_AMBIENT_WATER);
	map->wading = 1;
	fdEffectStartBlankOverlay(0);
}

/* Sets up Fog's room: the rope to Fog and, until the party has met Poi, the */
/* student who gave up, sitting on the floor. */
void scKandoreanTempleStartFogRoom(void) {
	fdRopeInit();
	fdRopePlace(0, FLAG_FOG_ROPE_TIED, ACTOR_FOG_ROPE_POST,
				ACTOR_FOG_ROPE_OTHER_POST);
	if(syGameFlagGet(FLAG_MET_POI)) {
		fdActorSetPos(ACTOR_STUDENT_WHO_GAVE_UP, 0, 0);
	}
	else {
		fdActorSetSprFlags(fdActorGetById(ACTOR_STUDENT_WHO_GAVE_UP), 0);
		fdActorSetAnimationById(ACTOR_STUDENT_WHO_GAVE_UP, 6);
		fdActorSetMovementTypeById(ACTOR_STUDENT_WHO_GAVE_UP,
								   KandoreanTemple_LevitateScript);
	}
	fdEffectStartBlankOverlay(0);
}

/* A student sits meditating, his first ellipsis after delay frames and up to */
/* 400 more. */
void scKandoreanTempleStartMeditating(s32 id, s32 delay) {
	ACTOR *actor = fdActorGetById(id);

	fdActorSetAnimationById(id, 5);
	actor->tmr = delay + (syRandomBelow(400));
	actor->userValue = id;
	fdActorSetMovementTypeById(id, KandoreanTemple_MeditateScript);
}

/* Sets up the temple gate: the weeds over the hidden passage gone once */
/* Whirlwind has scattered them, the students meditating or, once Poi has been
 */
/* met, standing by with the cells that open the temple's door laid before it,
 */
/* the door open if it was opened this visit, and Kraden. */
void scKandoreanTempleStartGate(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(syGameFlagGet(FLAG_HIDDEN_PASSAGE_FOUND)) {
		fdActorSetPos(ACTOR_WEEDS_OVER_THE_CAVE, 0, 0);
		fdMapGridCopyCellAttributes(0, 0, 1, 1, 5, 13);
	}
	else {
		fdActorDisableById(ACTOR_WEEDS_OVER_THE_CAVE);
	}
	if(syGameFlagGet(FLAG_MET_POI)) {
		fdMapGridCopyCellAttributes(2, 0, 2, 3, 17, 24);
		fdActorSetPos(ACTOR_STUDENT_ASKING_ABOUT_POI, 0, 0);
		fdActorSetPos(ACTOR_STUDENT_ON_GUARD_DUTY, 0, 0);
		fdActorSetPosAndFacing(ACTOR_STUDENT_WARY_OF_OUTSIDERS, toFIXED(374),
							   toFIXED(282), FACING_EAST);
		fdActorSetPosAndFacing(ACTOR_STUDENT_ON_THE_FAILED_TEST, toFIXED(272),
							   toFIXED(296), FACING_NORTH_NORTHEAST);
		fdActorSetPosAndFacing(ACTOR_STUDENT_WHO_WOULD_BE_A_MONK, toFIXED(216),
							   toFIXED(264), FACING_SOUTH_SOUTHEAST);
	}
	else {
		scKandoreanTempleStartMeditating(ACTOR_STUDENT_WARY_OF_OUTSIDERS, 0);
		scKandoreanTempleStartMeditating(ACTOR_STUDENT_ON_THE_FAILED_TEST, 240);
		scKandoreanTempleStartMeditating(ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
										 480);
	}
	if(syGameFlagGet(FLAG_TEMPLE_DOOR_OPEN)) {
		fdMapGridCopyCellTilesIndicesRect(77, 33, 77, 24, 2, 2);
		fdMapGridCopyCellAttributes(32, 30, 2, 1, 17, 25);
	}
#if TLA_PATCH_1 /* From patch 1. */
	if(MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_WEST ||
	   MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_EAST)
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
#endif
}

/* Before the party has Lash, Poi waits by the north rope, which he has tied */
/* across (FLAG_NORTH_ROPE_TIED) the first time on each visit. */
void scKandoreanTemplePoiWaitsByTheNorthRope(void) {
	if(fdEventGetFlag(FLAG_NORTH_ROPE_TIED) == 0) {
		fdEventSetFlag(FLAG_NORTH_ROPE_TIED);
		Call4(fdRopePlace, 1, FLAG_NORTH_ROPE_TIED, ACTOR_NORTH_ROPE_EAST_POST,
			  ACTOR_NORTH_ROPE_WEST_POST);
	}
	fdActorSetPosAndFacing(ACTOR_POI, toFIXED(392), toFIXED(360),
						   FACING_SOUTH_SOUTHEAST);
}

/* Sets up Poi's hall: its ropes, the tested students, Poi and the cave door, */
/* by the entrance the party takes. */
void scKandoreanTempleStartPoisRoom(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdRopeInit();
	Call4(fdRopePlace, 0, FLAG_SOUTH_ROPE_TIED, ACTOR_SOUTH_ROPE_WEST_POST,
		  ACTOR_SOUTH_ROPE_EAST_POST);
	fdRopePlace(1, FLAG_NORTH_ROPE_TIED, ACTOR_NORTH_ROPE_EAST_POST,
				ACTOR_NORTH_ROPE_WEST_POST);
	if(fdEventGetFlag(FLAG_MET_POI) == 0) {
		fdActorSetAnimationById(ACTOR_FIRST_TESTED_STUDENT, 5);
		fdActorSetAnimationById(ACTOR_THIRD_TESTED_STUDENT, 5);
	}
	fdActorSetAnimationById(ACTOR_SECOND_TESTED_STUDENT, 5);
	sySetEvent(scKandoreanTempleMatchStudentsToLeader, TASK_PASS_FRAME);
	if((MainRoutineStateWork.entrance >=
			KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_WEST &&
		MainRoutineStateWork.entrance <=
			KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_WEST_PASSAGE) ||
	   MainRoutineStateWork.entrance == KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE) {
		if(syGameFlagGet(FLAG_LASH_RECEIVED)) {
			fdActorSetPosAndFacing(ACTOR_POI, toFIXED(432), toFIXED(394),
								   FACING_SOUTH_SOUTHWEST);
		}
		else {
			scKandoreanTempleCreateLashPebble();
			if(syGameFlagGet(FLAG_MET_POI))
				scKandoreanTemplePoiWaitsByTheNorthRope();
		}
	}
	if((MainRoutineStateWork.entrance >=
			KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_GATE &&
		MainRoutineStateWork.entrance <=
			KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_EAST) ||
	   MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_MIMIC) {
		if(syGameFlagGet(FLAG_CAVE_DOOR_OPEN))
			scKandoreanTempleOpenTheCaveDoor();
		if(syGameFlagGet(FLAG_MET_POI)) {
			fdActorSetPos(ACTOR_POI, 0, 0);
			fdActorSetPosAndFacing(ACTOR_THIRD_TESTED_STUDENT, toFIXED(408),
								   toFIXED(664), FACING_NORTH_NORTHWEST);
		}
		else if(syGameFlagGet(FLAG_STUDENTS_TESTED)) {
			fdActorSetPos(ACTOR_POI, 0, 0);
			fdActorSetPos(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
		}
		else {
			scKandoreanTempleTestTheStudents();
		}
	}
	else if(MainRoutineStateWork.entrance ==
			KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_WEST_PASSAGE) {
		if(syGameFlagGet(FLAG_MET_POI) == 0)
			scKandoreanTemplePassTheTest();
	}
	else if(MainRoutineStateWork.entrance ==
			KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR) {
		scKandoreanTempleFindTheRopeOnTheFloor();
	}
	else if(MainRoutineStateWork.entrance ==
			KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE) {
		if(syGameFlagGet(FLAG_MET_POI) == 0)
			scKandoreanTempleMeetPoi();
	}
#if TLA_PATCH_1 /* From patch 1. */
	if(MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_CHESTBEATERS ||
	   MainRoutineStateWork.entrance ==
		   KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_MIMIC)
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
#endif
}

/* Turns a sprite by step (of TRIG_TURN to the turn) each frame for the given */
/* frames. */
void scKandoreanTempleTurnSpr(FIELD_SPR *sprite, u32 frames, s32 step) {
	u32 count = frames;
	u32 i;

	for(i = 0; i < count; i++) {
		sprite->rotation += step;
		fdEventWait(1);
	}
}

/* Moves an actor sideways by step each frame for the given frames. */
void scKandoreanTempleDrift(ACTOR *actor, u32 frames, FIXED step) {
	u32 count = frames;
	u32 i;

	for(i = 0; i < count; i++) {
		actor->x.fixed += step;
		fdEventWait(1);
	}
}

/* Poi tests his three students in turn: the first two fall, and the third */
/* levitates and passes. */
void scKandoreanTempleTestTheStudents(void) {
	ACTOR *actor;
	FIELD_SPR *sprite;
	u32 frame;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlg = 0;
	sySynch(1);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 400, 796);
	sySynch(1);
	fdEventStartScrTransition();
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 400, 752);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(436, 72, 664, 1);
	fdCameraWaitForArrival();
	fdActorJump(ACTOR_POI, 4, 0);
	fdEventSetMessageId((s32)&MsgKandoreanTempleFocusYourMind);
	/* "You there! Focus your mind!" */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdCameraMoveTo(456, 0, 688, 1);
	fdCameraWaitForArrival();
	fdActorRunRepeatedMotion(ACTOR_FIRST_TESTED_STUDENT, 2);
	sySndPlay(SE_PSYNERGY_SPARKLE);
	fdActorConfigureSprCol(ACTOR_FIRST_TESTED_STUDENT, SPRITE_COLOR_BLINK);
	fdEventWait(20);
	fdActorConfigureSprCol(ACTOR_FIRST_TESTED_STUDENT, 0);
	fdEventWait(20);
	actor = fdActorGetById(ACTOR_FIRST_TESTED_STUDENT);
	sprite = actor->spr;
	actor->motionFlg = 0;
	for(frame = 0; frame <= 63; frame++) {
		actor->y.fixed += toFIXED(0.05);
		fdEventWait(1);
	}
	scKandoreanTempleTurnSpr(sprite, 32, TRIG_TURN / 2048);
	fdEventWait(20);
	fdActorSetAttachedEffect(ACTOR_FIRST_TESTED_STUDENT,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scKandoreanTempleTurnSpr(sprite, 32, -TRIG_TURN / 1024);
	scKandoreanTempleTurnSpr(sprite, 32, TRIG_TURN / 1024);
	scKandoreanTempleTurnSpr(sprite, 64, TRIG_TURN / 2048);
	fdEffectSetAttached(ACTOR_FIRST_TESTED_STUDENT,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdEventWait(5);
	sySndPlay(SE_DOOR_CLOSE);
	actor->velY = toFIXED(3);
	sprite->rotation = 0;
	fdEventWait(60);
	/* "Did you see that, Master?" */
	fdEventShowMessageAndWait(ACTOR_FIRST_TESTED_STUDENT, 0);
	fdCameraMoveTo(436, 72, 664, 1);
	fdCameraWaitForArrival();
	fdActorSetSpeed(ACTOR_POI, 0.8);
	fdActorWalkToAndWait(ACTOR_POI, 476, 664);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
	/* "No, no, no! You lack discipline!" */
	fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_POI, 0);
	fdActorWalkToAndWait(ACTOR_POI, 474, 664);
	fdActorWalkToAndWait(ACTOR_POI, 472, 648);
	fdActorSetPos(ACTOR_POI, 0, 0);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(364, 72, 664, 1);
	fdCameraWaitForArrival();
	fdActorSetPos(ACTOR_POI, 328, 648);
	sySynch(1);
	fdActorWalkToAndWait(ACTOR_POI, 326, 664);
	fdActorWalkToAndWait(ACTOR_POI, 320, 664);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 0);
	/* "Now, you!" */
	fdEventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_POI, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(342, 0, 688, 1);
	fdCameraWaitForArrival();
	fdActorRunRepeatedMotion(ACTOR_SECOND_TESTED_STUDENT, 2);
	sySndPlay(SE_PSYNERGY_SPARKLE);
	fdActorConfigureSprCol(ACTOR_SECOND_TESTED_STUDENT, SPRITE_COLOR_BLINK);
	fdEventWait(20);
	fdActorConfigureSprCol(ACTOR_SECOND_TESTED_STUDENT, 0);
	fdEventWait(20);
	fdActorSetAttachedEffect(ACTOR_SECOND_TESTED_STUDENT,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	actor = fdActorGetById(ACTOR_SECOND_TESTED_STUDENT);
	actor->motionFlg = 0;
	for(frame = 0; frame <= 63; frame++) {
		actor->y.fixed += toFIXED(3) / 20;
		fdEventWait(1);
	}
	scKandoreanTempleDrift(actor, 64, toFIXED(1) / 16);
	scKandoreanTempleDrift(actor, 128, -toFIXED(1) / 16);
	scKandoreanTempleDrift(actor, 128, toFIXED(1) / 16);
	scKandoreanTempleDrift(actor, 64, -toFIXED(1) / 16);
	fdEventWait(20);
	fdActorSetAttachedEffect(ACTOR_SECOND_TESTED_STUDENT,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdEventWait(5);
	sySndPlay(SE_DOOR_CLOSE);
	actor->velY = toFIXED(3);
	fdEventWait(60);
	/* "How was that, Master?" */
	fdEventShowMessageAndWait(ACTOR_SECOND_TESTED_STUDENT, 0);
	fdCameraMoveTo(364, 72, 664, 1);
	fdCameraWaitForArrival();
	/* "Improving...slowly! Continue your meditation!" */
	fdEventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_POI, 0);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 10);
	/* "Your turn!" */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdCameraMoveTo(400, 0, 688, 1);
	fdCameraWaitForArrival();
	fdActorRunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 2);
	sySndPlay(SE_PSYNERGY_SPARKLE);
	fdActorConfigureSprCol(ACTOR_THIRD_TESTED_STUDENT, SPRITE_COLOR_BLINK);
	fdEventWait(20);
	fdActorConfigureSprCol(ACTOR_THIRD_TESTED_STUDENT, 0);
	fdEventWait(20);
	actor = fdActorGetById(ACTOR_THIRD_TESTED_STUDENT);
	sprite = actor->spr;
	actor->motionFlg = 0;
	for(frame = 0; frame <= 63; frame++) {
		actor->y.fixed += toFIXED(1) / 5;
		fdEventWait(1);
	}
	scKandoreanTempleTurnSpr(sprite, 32, TRIG_TURN / 1024);
	sprite->rotation = 0;
	fdActorRunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 1);
	fdEventWait(60);
	scKandoreanTempleTurnSpr(sprite, 32, -TRIG_TURN / 1024);
	sprite->rotation = 0;
	fdActorRunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 1);
	fdEventWait(60);
	for(frame = 0; frame <= 63; frame++) {
		actor->y.fixed += -toFIXED(1) / 5;
		fdEventWait(1);
	}
	fdEventWait(20);
	fdActorShowEmoteById(ACTOR_THIRD_TESTED_STUDENT,
						 EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	/* "Master, have I done it?" */
	fdEventShowMessageThenWait(ACTOR_THIRD_TESTED_STUDENT, 0, 20);
	/* "Not bad... You might stand a chance to succeed..." */
	fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_POI, 0);
	fdActorShowEmoteById(ACTOR_THIRD_TESTED_STUDENT,
						 EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorSetAnimationById(ACTOR_THIRD_TESTED_STUDENT, ANIM_STAND);
	fdActorJump(ACTOR_THIRD_TESTED_STUDENT, 4, 0);
	fdActorFaceDirection(ACTOR_THIRD_TESTED_STUDENT, FACING_NORTH_NORTHWEST,
						 20);
	/* "I can do it, Master! Please, just let me try!" */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_THIRD_TESTED_STUDENT,
							  0);
	fdCameraMoveTo(364, 72, 664, 1);
	fdCameraWaitForArrival();
	fdActorShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	/* "Your skills lack polish... It may be dangerous. Do you still wish to */
	/* enter?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	/* "Yes, I do! I do!" */
	fdEventShowMessageAndWait(ACTOR_THIRD_TESTED_STUDENT, 0);
	fdActorFaceDirection(ACTOR_POI, FACING_NORTH_NORTHWEST, 40);
	fdActorSetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
	/* "If you fail, you may lose more than just your pride..." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	/* "I do not fear death, Master!" */
	fdEventShowMessageAndWait(ACTOR_THIRD_TESTED_STUDENT, 0);
	fdActorShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	/* "Ho ho! Is that so?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 20);
	fdActorSetAnimationById(ACTOR_POI, ANIM_NOD);
	/* "Very well, then! It is time for your trial, my son!" */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	fdActorWalkToAndWait(ACTOR_POI, 326, 664);
	fdActorWalkToAndWaitById(ACTOR_POI, 328, 648);
	fdActorSetSprFlags(fdActorGetById(ACTOR_POI), 0);
	fdActorSetSprCol(fdActorGetById(ACTOR_POI), SPRITE_COLOR_HIDDEN);
	fdActorSetPos(ACTOR_POI, 440, 600);
	fdCameraMoveTo(400, 0, 688, 1);
	fdActorSetSpeed(ACTOR_THIRD_TESTED_STUDENT, 0.8);
	fdActorWalkToAndWait(ACTOR_THIRD_TESTED_STUDENT, 400, 644);
	fdEventWait(20);
	sySndPlay(SE_THUMP);
	scKandoreanTempleOpenTheCaveDoor();
	/* "Enter the cave, and ascend!" */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	syCloseEvent(scKandoreanTempleMatchStudentsToLeader);
	fdActorSetAnimationAndWait(ACTOR_THIRD_TESTED_STUDENT, ANIM_NOD);
	fdActorSetSprPriority(ACTOR_THIRD_TESTED_STUDENT, 3);
	fdActorWalkToAndWait(ACTOR_THIRD_TESTED_STUDENT, 400, 640);
	fdActorSetPos(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
	fdActorSetPos(ACTOR_POI, 0, 0);
	fdCameraMoveTo(400, 0, 752, 1);
	fdCameraWaitForArrival();
	sySetEvent(scKandoreanTempleMatchStudentsToLeader, TASK_PASS_FRAME);
	fdEventEnd();
	syGameFlagSet(FLAG_STUDENTS_TESTED);
}

/* Felix comes out of the west passage into the upper hall of Poi's hall, the */
/* way up from the Chestbeaters' room, and Poi praises him for passing the test
 */
/* before he sees the party are not his students and asks how they came so far.
 */
/* He then goes down into the test hall to look at the rope. */
void scKandoreanTemplePassTheTest(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 328, 360);
	fdActorSetPosAndFacing(ACTOR_POI, toFIXED(472), toFIXED(408),
						   FACING_SOUTH_SOUTHWEST);
	fdActorGetById(ACTOR_POIS_ROOM_KRADEN)->facing = FACING_NORTH;
	sySynch(1);
	fdEventStartScrTransition();
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 328, 392);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(408, CAMERA_KEEP, 400, 1);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 376, 408);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_POI, 1);
	fdEventSetMessageId((s32)&MsgKandoreanTempleTestPassed);
	/* "You have passed a very difficult test. You have pleased your teacher!"
	 */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorFaceDirection(ACTOR_POI, FACING_WEST, 20);
	fdActorShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorSetSpeed(ACTOR_POI, 1.6);
	fdActorWalkToAndWait(ACTOR_POI, 456, 408);
	/* "Wait... You're not one of my students! Who are you!?" */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorPlaceOnActor(ACTOR_POIS_ROOM_KRADEN,
						MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorSetSpeed(ACTOR_POIS_ROOM_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_POIS_ROOM_KRADEN, 376, 392);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
	fdActorFaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdActorFaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 20);
	fdActorSetAttachedEffect(ACTOR_POI,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "How did you... come this far?" */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorShowEmoteById(ACTOR_POIS_ROOM_KRADEN,
						 EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	fdActorFaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 20);
	/* "Ah, yes... How indeed?" */
	fdEventShowMessageAndWait(ACTOR_POIS_ROOM_KRADEN, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdActorFaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
	fdActorWalkToAndWait(ACTOR_POI, 468, 428);
	fdActorWalkToAndWait(ACTOR_POI, 472, 454);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR);
}

/* Poi goes down into the test hall and looks about by the south rope's posts:
 */
/* "The rope is still on the floor." Nobody has climbed up that way, so he */
/* goes back up to the party sure that they came through the cave. */
void scKandoreanTempleFindTheRopeOnTheFloor(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
	fdActorSetPos(ACTOR_POI, 0, 0);
	fdActorSetPos(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlg = 0;
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorSetPos(ACTOR_POI, 472, 648);
	sySynch(1);
	fdActorWalkToAndWait(ACTOR_POI, 472, 664);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(456, 40, 664, 1);
	fdCameraWaitForArrival();
	fdEventWait(40);
	fdEventSetMessageId((s32)&MsgKandoreanTempleRopeOnTheFloor);
	/* "The rope is still on the floor." */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
	fdActorWalkToAndWait(ACTOR_POI, 472, 648);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE);
}

/* Poi steps up beside the north rope's east post, turns west to the party and
 */
/* answers Kraden. */
void scKandoreanTemplePoiWalksUp(void) {
	fdActorSetSpeed(ACTOR_POI, 1.6);
	fdActorWalkToAndWait(ACTOR_POI, 496, 392);
	fdActorFaceDirection(ACTOR_POI, FACING_WEST, 0);
	fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
}

/* Poi comes back up into the upper hall to the party: they must have come */
/* through the cave, so he must give them the secret of Lash. He shows them */
/* Lash on the north rope, then crosses it ahead of them to the raised floor by
 */
/* his table, and waits there for them to follow. */
void scKandoreanTempleMeetPoi(void) {
	ACTOR *actor;
	FIELD_SPR *sprite;
	u8 *flags;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlg = 0;
	sySynch(1);
	fdActorSetPosAndFacing(ACTOR_POIS_ROOM_KRADEN, toFIXED(376), toFIXED(392),
						   FACING_EAST);
	fdActorSetHorizontalPosAndAngWithTerrain(MainRoutineStateWork.leaderActorId,
											 toFIXED(376), toFIXED(408),
											 FACING_EAST);
	fdActorSetPosAndFacing(ACTOR_POI, toFIXED(472), toFIXED(454),
						   FACING_NORTH_NORTHWEST);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdActorSetSpeed(ACTOR_POI, 1.2);
	fdActorWalkToAndWait(ACTOR_POI, 464, 428);
	fdActorWalkToAndWait(ACTOR_POI, 432, 408);
	fdActorFaceDirection(ACTOR_POI, FACING_WEST, 0);
	fdEffectSetAttached(ACTOR_POI,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventSetMessageId((s32)&MsgKandoreanTempleThroughTheCave);
	/* "You must have come through the cave..." */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "That means..." */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorFaceDirectionById(ACTOR_POI, FACING_EAST, 40);
	/* "I must give you... the secret of Lash!" */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	/* "Master Poi... You seem perturbed." */
	fdEventShowMessageAndWait(ACTOR_POIS_ROOM_KRADEN, 0);
	fdActorRunRepeatedMotion(ACTOR_POI, 2);
	fdActorFaceDirection(ACTOR_POI, FACING_WEST, 20);
	fdActorSetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
	/* "Oh, no... Not at all..." */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorFaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
	/* "Felix, am I imagining it, or does he seem perturbed?" Yes: "I am not */
	/* perturbed. Merely...surprised." No: "No one from outside the temple has
	 */
	/* ever passed..." */
	fdEventOpenMessage(ACTOR_POIS_ROOM_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdActorSetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
		/* "I am not perturbed. Merely...surprised." */
		fdEventShowMessageAndWait(ACTOR_POI, 0);
		FieldStateWork->msg++;
	}
	else {
		FieldStateWork->msg++;
		fdActorSetAnimationById(ACTOR_POI, ANIM_NOD);
		/* "No one from outside the temple has ever passed the test. It's */
		/* amazing..." */
		fdEventShowMessageAndWait(ACTOR_POI, 0);
	}
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdActorFaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_POIS_ROOM_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 20);
	/* "What is this "Lash" you mentioned a moment ago?" */
	fdEventShowMessageAndWait(ACTOR_POIS_ROOM_KRADEN, 0);
	fdActorSetAttachedEffect(ACTOR_POI,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Lash is a sacred power, passed down for years to the followers of..." */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorSetAnimationById(ACTOR_POIS_ROOM_KRADEN, ANIM_NOD);
	/* "A sacred power? And it's called Lash, you say?" */
	fdEventShowMessageThenWait(ACTOR_POIS_ROOM_KRADEN, 0, 40);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 40);
	/* "And now, I must pass it on to you. It is our way." */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorFaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 40);
	fdActorFaceDirection(ACTOR_POI, FACING_WEST, 20);
	fdActorSetAnimationById(ACTOR_POI, ANIM_NOD);
	/* "It is a gift given to all those who pass the test, as it has been..." */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdActorFaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdActorShowEmoteById(ACTOR_POIS_ROOM_KRADEN,
						 EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "What type of power is this Lash?" */
	fdEventShowMessageAndWait(ACTOR_POIS_ROOM_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_POI, ANIM_NOD);
	/* "Allow me to show you." */
	fdEventShowMessageAndWait(ACTOR_POI, 0);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(424, 16, 384, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorSetSpeed(ACTOR_POIS_ROOM_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_POI, 0.8);
	fdActorSetMovementTypeById(MainRoutineStateWork.leaderActorId,
							   KandoreanTemple_LeaderStepsAsideScript);
	fdActorSetMovementTypeById(ACTOR_POIS_ROOM_KRADEN,
							   KandoreanTemple_KradenStepsAsideScript);
	fdActorSetMovementTypeAndWaitById(ACTOR_POI,
									  KandoreanTemple_PoiWalksToTheRopeScript);
	fdEventWait(40);
	scKandoreanTempleCastLash();
	fdActorFaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHWEST, 20);
	/* "Lash looks quite a bit like Psynergy, doesn't it?" */
	fdEventOpenMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE |
						   ACTOR_POIS_ROOM_KRADEN,
					   0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		/* "Psynergy, hm? We call it spiritual strength, but it seems the same."
		 */
		scKandoreanTemplePoiWalksUp();
		FieldStateWork->msg++;
	}
	else {
		FieldStateWork->msg++;
		/* "I do not know of your Psynergy, but our power comes from within." */
		scKandoreanTemplePoiWalksUp();
	}
	fdActorFaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdActorSetAnimationById(ACTOR_POI, ANIM_NOD);
	/* "This technique uses the spiritual to affect the physical." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_POIS_ROOM_KRADEN, ANIM_NOD);
	/* "One with a strong spirit can lift and move heavy objects farther." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
	fdActorShowEmoteById(ACTOR_POIS_ROOM_KRADEN,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "The distance changes with your level of power? Fascinating!" */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE |
						   ACTOR_POIS_ROOM_KRADEN,
					   0);
	fdActorSetAnimationById(ACTOR_POI, ANIM_NOD);
	/* "Those who cannot pass this cave's trials are not worthy of the */
	/* technique." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
	fdActorFaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHWEST, 0);
	/* "Felix, do you think this cave was designed to test one's control of */
	/* Psynergy?" */
	fdEventOpenMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE |
						   ACTOR_POIS_ROOM_KRADEN,
					   0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdActorSetAnimationById(ACTOR_POI, ANIM_NOD);
		/* "Yes, yes! Well done, old man!" */
		fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
		fdActorFaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
		fdActorShowEmoteById(ACTOR_POIS_ROOM_KRADEN,
							 EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
		/* "Who are you calling an old man!? You're no spring chicken yourself!"
		 */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE |
									  ACTOR_POIS_ROOM_KRADEN,
								  0);
		FieldStateWork->msg += 2;
	}
	else {
		FieldStateWork->msg += 2;
		fdActorSetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
		/* "You are quite mistaken... You should listen to the old man. He
		 * speaks */
		/* wisely." */
		fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
		fdActorFaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
		fdActorShowEmoteById(ACTOR_POIS_ROOM_KRADEN,
							 EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
		/* "Who are you calling an old man? You're no schoolboy yourself!" */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE |
									  ACTOR_POIS_ROOM_KRADEN,
								  0);
	}
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdCameraMoveTo(424, 0, 400, 1);
	fdCameraSetSpeed(0.6);
	fdActorSetSpeed(ACTOR_POI, 0.6);
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_POI);
	fdActorWatchAndSetScript(ACTOR_POIS_ROOM_KRADEN, ACTOR_POI);
	fdActorWalkToAndWait(ACTOR_POI, 488, 416);
	fdActorWalkToAndWait(ACTOR_POI, 472, 432);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 0);
	/* "I wondered if it would be right to allow an outsider to use this */
	/* power..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
	/* "You're...not going to teach us?" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_POIS_ROOM_KRADEN,
							   0, 80);
	fdActorShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorFaceDirection(ACTOR_POI, FACING_NORTH_NORTHWEST, 20);
	/* "But I like you, so I will let you learn our secret." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
	fdActorSetAnimationById(ACTOR_POI, ANIM_NOD);
	/* "Please, follow me." */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
	fdCameraMoveTo(424, 16, 384, 1);
	fdActorSetSpeed(ACTOR_POI, 1.2);
	fdActorWalkToAndWait(ACTOR_POI, 488, 416);
	fdActorWalkToAndWait(ACTOR_POI, 504, 376);
	fdActorFaceDirectionById(ACTOR_POI, FACING_WEST, 20);
	actor = fdActorGetById(ACTOR_POI);
	sprite = actor->spr;
	fdActorSetSprPriority(ACTOR_POI, 1);
	fdActorSetSpeed(ACTOR_POI, 1.6);
	fdActorJump(ACTOR_POI, 4, 0);
	fdActorMoveToAndWait(ACTOR_POI, 492, 376);
	fdActorSetAnimationById(ACTOR_POI, 7);
	actor->scaleX = -FIXED_ONE;
	sprite->rotation = TRIG_TURN * 7 / 8;
	actor->motionFlg = 0;
	actor->y.fixed += toFIXED(4);
	fdActorSetSprFlags(fdActorGetById(ACTOR_POI), 0);
	fdActorSetSpeed(ACTOR_POI, 0.4);
	scFieldActorSetMoveTarget((s32)actor, 444, 40, 376);
	fdActorWaitForMoveById(ACTOR_POI);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetSprFlags(fdActorGetById(ACTOR_POI), SPRITE_FLAG_SHADOW);
	sprite->rotation = 0;
	actor->scaleX = FIXED_ONE;
	actor->groundHeight = toFIXED(32);
	fdActorSetAnimationById(ACTOR_POI, ANIM_STAND);
	fdActorSetSpeed(ACTOR_POI, 1.6);
	fdActorJump(ACTOR_POI, 4, 0);
	fdActorMoveToAndWait(ACTOR_POI, 424, 376);
	fdCameraMoveTo(408, 16, 400, 1);
	fdActorSetSpeed(ACTOR_POI, 0.8);
	fdActorWalkToAndWait(ACTOR_POI, 392, 360);
	fdActorFaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetSprPriority(ACTOR_POI, 2);
	fdActorStop(ACTOR_POIS_ROOM_KRADEN);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorFaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHWEST, 10);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_POIS_ROOM_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_POIS_ROOM_KRADEN, ANIM_WALK);
	{
		ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

		if(leader != NULL)
			fdActorSetDestinationById(ACTOR_POIS_ROOM_KRADEN,
									  leader->x.part[0].integer,
									  leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_POIS_ROOM_KRADEN);
	fdActorSetPos(ACTOR_POIS_ROOM_KRADEN, 0, 0);
	syGameFlagSet(FLAG_MET_POI);
#if TLA_PATCH_1
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
#endif
	fdEventEnd();
}

/* Creates the Lash Pebble's actor on Poi's table, drawn from its item icon. */
void scKandoreanTempleCreateLashPebble(void) {
	ACTOR **slot = &KandoreanTemple_LashPebble;
	u8 clear = 0;
	ACTOR *actor = ((ACTOR *)syImportActorCreate(
		SPRITE_UNSEEN_MARKER, toFIXED(408), toFIXED(40), toFIXED(344)));

	*slot = actor;
	if(actor) {
		FIELD_SPR *sprite = actor->spr;
		ICON_TRANSFER *glyph;

		sprite->fullCol = 0;
		sprite->pal = 0;
		actor->motionFlg = clear;
		actor->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		sprite->flg = clear;
		sprite->cnt = clear;
		glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_LASH_PEBBLE);
		syVramCacheLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
}

/* Back in the Chestbeaters' room after their battle, a ladder unrolls down */
/* the north wall, rung by rung. */
void scKandoreanTempleLowerTheLadder(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	sySndPlay(SE_SCUFFLE);
	fdMapGridCopyCellTilesIndicesRect(41, 0, 20, 0, 1, 1);
	fdEventWait(10);
	fdMapGridCopyCellTilesIndicesRect(41, 1, 20, 1, 1, 1);
	fdEventWait(10);
	fdMapGridCopyCellTilesIndicesRect(41, 2, 20, 2, 1, 1);
	fdEventWait(10);
	fdMapGridCopyCellTilesIndicesRect(41, 3, 20, 3, 1, 1);
	fdEventWait(10);
	syGameFlagSet(FLAG_LADDER_LOWERED);
	fdEventEnd();
}

void scKandoreanTempleFightFog(void);
void scKandoreanTempleReceiveLash(void);

extern u32 FieldEffect_LongScript[];

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* A cloud of steam lives 44 frames, then is put away and released. */
s32 KandoreanTemple_SteamScript[] = {
    SC_WAIT(44),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
    /* Unused: SC_RELEASE has freed the cloud's actor, and nothing starts a */
    /* script here, so its life is never counted down. */
    SC_CALL((s32)scKandoreanTempleCountDownSteam),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

/* The block Move pushes in the lift room, and the vent room's pillar, each */
/* ended by -1. */
s16 KandoreanTemple_BlockActors[] = {
    ACTOR_LIFT_ROOM_PILLAR, -1,
};
s16 KandoreanTemple_PillarActors[] = {
    ACTOR_VENT_ROOM_PILLAR, -1,
};

u32 KandoreanTemple_SplashRingScript[] = {
    SC_WAIT(24),
    SC_RELEASE,
};

u32 KandoreanTemple_SplashScript[] = {
    SC_WAIT(15),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

u32 KandoreanTemple_LookAroundScript[] = {
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KandoreanTemple_MeditateScript[] = {
    SC_CALL((s32)scKandoreanTempleMeditate),
};

s32 KandoreanTemple_LevitateScript[] = {
    SC_CALL(scKandoreanTempleSquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 KandoreanTemple_LeaderStepsAsideScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(456, 0, 408),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 KandoreanTemple_KradenStepsAsideScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(440, 0, 392),                         
      SC_SYNC,     SC_ANIMATION(1),                                                  
      SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),              
      SC_WAIT(1), SC_END,                                                     
  };

u32 KandoreanTemple_PoiWalksToTheRopeScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(496, 0, 392),
    SC_SYNC,
    SC_MOVE(504, 0, 376),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 KandoreanTemple_HighFlameScript[] = {
    SC_CALL(scKandoreanTemplePlaceFlame),
    SC_SET(SW_Y, toFIXED(88)),
    SC_END,
};

s32 KandoreanTemple_LowFlameScript[] = {
    SC_CALL(scKandoreanTemplePlaceFlame),
    SC_SET(SW_Y, toFIXED(44)),
    SC_END,
};

s32 KandoreanTemple_FlameScript[] = {
    SC_CALL(scKandoreanTemplePlaceFlame),
    SC_END,
};

#include "shared/scene_entrances_default.c"

SCENE_REGION KandoreanTemple_ChestbeaterRoomRegions[] = {
    {320, 74, 96, 336, 90, 112, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_POI},
    {SCENE_TABLE_END},
};

SCENE_REGION KandoreanTemple_StreamRoomRegions[] = {
    {512, 42, 272, 528, 58, 288, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_EAST},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(KandoreanTempleLifts),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_WEST, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_LIFTS_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_EAST, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_LIFTS_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_CURRENT, KandoreanTempleCurrent, KANDOREAN_TEMPLE_CURRENT_FROM_KANDOREAN_TEMPLE_LIFTS),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_NORTH, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_NORTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_SHAFT, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_SHAFT),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_WEST_LIFT, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_WEST_LIFT),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_EAST_LIFT, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_EAST_LIFT),
    SCENE_EXITS(KandoreanTempleMimic),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_POI, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_MIMIC),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_SOUTH, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_MIMIC_SOUTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_CHESTBEATERS, KandoreanTempleChestbeaters, KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_MIMIC),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_NORTH, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_MIMIC_NORTH),
    SCENE_EXITS(KandoreanTempleChestbeaters),
    SCENE_EXIT(KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_MIMIC, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_CHESTBEATERS),
    SCENE_EXIT(KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_POI, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_CHESTBEATERS),
    SCENE_EXITS(KandoreanTemplePillar),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_CURRENT, KandoreanTempleCurrent, KANDOREAN_TEMPLE_CURRENT_FROM_KANDOREAN_TEMPLE_PILLAR),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_NORTH, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_NORTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_WEST_LIFT, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_WEST_LIFT),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_EAST_LIFT, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_EAST_LIFT),
    SCENE_EXITS(KandoreanTempleStream),
    SCENE_EXIT(KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_WEST, KandoreanTempleGate, KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_EAST, KandoreanTempleGate, KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_EAST),
    SCENE_EXITS(KandoreanTempleCurrent),
    SCENE_EXIT(KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_LIFTS, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_CURRENT),
    SCENE_EXIT(KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_PILLAR, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_CURRENT),
    SCENE_EXITS(KandoreanTempleFog),
    SCENE_EXIT(KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_WEST, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_FOG_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_NORTH, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_FOG_NORTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_EAST, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_FOG_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_SOUTH, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_FOG_SOUTH),
    SCENE_EXITS(KandoreanTempleGate),
    SCENE_EXIT(KANDOREAN_TEMPLE_GATE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KANDOREAN_TEMPLE_GATE),
    SCENE_EXIT(KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_POI, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_GATE),
    SCENE_EXIT(KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_WEST, KandoreanTempleStream, KANDOREAN_TEMPLE_STREAM_FROM_KANDOREAN_TEMPLE_GATE_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_EAST, KandoreanTempleStream, KANDOREAN_TEMPLE_STREAM_FROM_KANDOREAN_TEMPLE_GATE_EAST),
    SCENE_EXITS(KandoreanTemplePoi),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_GATE, KandoreanTempleGate, KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_POI),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_WEST, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_EAST, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_WEST, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_EAST, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_WEST_PASSAGE, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_WEST_PASSAGE_FROM_UPPER_HALL),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_WEST_PASSAGE_TO_UPPER_HALL, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_WEST_PASSAGE),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_CHESTBEATERS, KandoreanTempleChestbeaters, KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_POI),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_MIMIC, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_POI),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE),
    SCENE_EXITS_END,
};

ACTOR_PLACE KandoreanTemple_Placements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE KandoreanTemple_LiftRoomPlacements[] = {
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE KandoreanTemple_ChestbeaterRoomPlacements[] = {
    {SPRITE_SIGNAL_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SIGNAL_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHESTBEATER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 328, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHESTBEATER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 296, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHESTBEATER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 360, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE KandoreanTemple_VentRoomPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE KandoreanTemple_FogRoomPlacements[] = {
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 340, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_FOG), MOVEMENT_TYPE_STAND, 632, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE KandoreanTemple_GatePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 224, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, (s32)KandoreanTemple_LookAroundScript, 264, 0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 334, 0, 317, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 284, 0, 328, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 246, 0, 318, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE KandoreanTemple_PoisRoomPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 676, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 676, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 664, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)KandoreanTemple_HighFlameScript, 439, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)KandoreanTemple_HighFlameScript, 359, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)KandoreanTemple_LowFlameScript, 412, 0, 324, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)KandoreanTemple_FlameScript, 90, 0, 230, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)KandoreanTemple_FlameScript, 174, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

TRIGGER KandoreanTemple_Events[] = {
    {TRIGGER_TABLE_END},
};

TRIGGER KandoreanTemple_LiftRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_WEST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_WEST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_EAST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_CURRENT, CONDITION_ALWAYS, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_CURRENT},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_NORTH, CONDITION_ALWAYS, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_NORTH},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scKandoreanTempleStartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scKandoreanTempleFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LIFT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(scKandoreanTempleStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LIFT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(scKandoreanTempleFinishMovingBlock)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SHAFT, FLAG_PILLAR_BLOCKS_SHAFT, EVENT(scKandoreanTempleSinkIntoTheSteam)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_LIFT, CONDITION_ALWAYS, EVENT(scKandoreanTempleRideWestLiftDown)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_LIFT, CONDITION_ALWAYS, EVENT(scKandoreanTempleRideEastLiftDown)},
    {TRIGGER_TABLE_END},
};

TRIGGER KandoreanTemple_MimicRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_POI, CONDITION_ALWAYS, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_POI},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_SOUTH, CONDITION_ALWAYS, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_CHESTBEATERS, CONDITION_ALWAYS, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_CHESTBEATERS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_NORTH, CONDITION_ALWAYS, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_NORTH},
    {TRIGGER_TABLE_END},
};

TRIGGER KandoreanTemple_ChestbeaterRoomEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_MIMIC, CONDITION_ALWAYS, KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_MIMIC},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BOILING_POT, FLAG_POT_MOVED, EVENT(scKandoreanTempleStartMovingPot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOILING_POT, FLAG_POT_MOVED, EVENT(scKandoreanTempleFinishMovingPot)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_WEST_OF_HOT_FLOOR, FLAG_POT_MOVED, EVENT(scKandoreanTempleTouchTheHotFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SOUTH_OF_HOT_FLOOR, FLAG_POT_MOVED, EVENT(scKandoreanTempleTouchTheHotFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_EAST_OF_HOT_FLOOR, FLAG_POT_MOVED, EVENT(scKandoreanTempleTouchTheHotFloor)},
    {TRIGGER_TALK, ACTOR_POIS_SIGN, CONDITION_ALWAYS, EVENT(scKandoreanTempleReadPoisSign)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_CHESTBEATERS, FLAG_LADDER_LOWERED, EVENT(scKandoreanTempleFightChestbeaters)},
    {TRIGGER_TABLE_END},
};

TRIGGER KandoreanTemple_VentRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_CURRENT, CONDITION_ALWAYS, KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_CURRENT},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_NORTH, CONDITION_ALWAYS, KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_NORTH},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scKandoreanTempleStartMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scKandoreanTempleFinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VENT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(scKandoreanTempleStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VENT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(scKandoreanTempleFinishMovingPillar)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_VENT_COVER, CONDITION_ALWAYS, EVENT(scKandoreanTempleLeaveByWestVent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_VENT_COVER, CONDITION_ALWAYS, EVENT(scKandoreanTempleLeaveByEastVent)},
    {TRIGGER_TABLE_END},
};

TRIGGER KandoreanTemple_StreamRoomEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_WEST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_WEST},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

TRIGGER KandoreanTemple_CurrentRoomEvents[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_LIFTS, CONDITION_ALWAYS,
	 KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_LIFTS},
	{TRIGGER_EXIT, KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_PILLAR,
	 CONDITION_ALWAYS, KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_PILLAR},
	{TRIGGER_STEP, CELL_BEHAVIOR_CURRENT, CONDITION_ALWAYS,
	 EVENT(scKandoreanTemplePushAlongTheCurrent)},
	{TRIGGER_TABLE_END},
};

TRIGGER KandoreanTemple_FogRoomEvents[] = {
	{TRIGGER_EXIT, KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_WEST,
	 CONDITION_ALWAYS, KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_WEST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_NORTH, CONDITION_ALWAYS,
	 KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_NORTH},
	{TRIGGER_EXIT, KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_EAST,
	 CONDITION_ALWAYS, KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_EAST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_SOUTH, CONDITION_ALWAYS,
	 KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_SOUTH},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FOG_ROPE_POST, FLAG_FOG_ROPE_TIED,
	 0},
	{TRIGGER_TALK, ACTOR_FOG, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleFightFog)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BEHIND_POST, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleStepBehindThePost)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BESIDE_POST, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleStepOutFromThePost)},
	{TRIGGER_TALK, ACTOR_STUDENT_WHO_GAVE_UP, CONDITION_ALWAYS,
	 MSGNO(MsgFieldItSeemsIMayNotHave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WHO_GAVE_UP,
	 CONDITION_ALWAYS, MSGNO(MsgFieldIsItReallyPossibleForAnyone)},
	{TRIGGER_TABLE_END},
};

TRIGGER KandoreanTemple_GateEvents[] = {
	{TRIGGER_EXIT, KANDOREAN_TEMPLE_GATE_TO_WORLD_MAP, CONDITION_ALWAYS,
	 KANDOREAN_TEMPLE_GATE_TO_WORLD_MAP},
	{TRIGGER_EXIT, KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_POI,
	 CONDITION_ALWAYS, KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_POI},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_WEST, CONDITION_ALWAYS,
	 KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_WEST},
	{STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COURTYARD_WELL,
	 CONDITION_ALWAYS, EVENT(scKandoreanTempleClimbDownTheWell)},
	{EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgKandoreanTemplePassageInTheWell))},
	{TRIGGER_STEP, CELL_BEHAVIOR_SUSPICIOUS_WALL, FLAG_KRADEN_SAW_THE_WALL,
	 EVENT(scKandoreanTempleKradenSuspectsTheWall)},
	{STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_INSIDE_TEMPLE_DOOR,
	 FLAG_TEMPLE_DOOR_OPEN, EVENT(scKandoreanTempleOpenTheTempleDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_BEFORE_TEMPLE_DOOR,
	 FLAG_TEMPLE_DOOR_OPEN, EVENT(scKandoreanTempleOpenTheTempleDoor)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_TEMPLE_DOOR, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleTryTheLockedDoor)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS_OVER_THE_CAVE,
	 CONDITION_ALWAYS, EVENT(scKandoreanTempleFindTheHiddenPassage)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_FIRST_WEED_CLUMP, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleCycloneFirstClump)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_SECOND_WEED_CLUMP, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleCycloneSecondClump)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_THIRD_WEED_CLUMP, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleCycloneThirdClump)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_FOURTH_WEED_CLUMP, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleCycloneFourthClump)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_FIFTH_WEED_CLUMP, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleCycloneFifthClump)},
	{TRIGGER_TALK, ACTOR_STUDENT_ASKING_ABOUT_POI, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleAskAboutPoi)},
	{TRIGGER_TALK, ACTOR_STUDENT_ON_GUARD_DUTY, CONDITION_ALWAYS,
	 EVENT(scKandoreanTempleTellOfTheMeditation)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_STUDENT_ASKING_ABOUT_POI | TRIGGER_SUBJECT_OWN_EFFECT,
	 FLAG_ASKED_ABOUT_POI, EVENT(scKandoreanTempleReadTheGateStudent)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_ASKING_ABOUT_POI,
	 CONDITION_ALWAYS, MSGNO(MsgFieldIDoNotUnderstandMasterPoi)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_STUDENT_ON_GUARD_DUTY | TRIGGER_SUBJECT_OWN_EFFECT,
	 FLAG_TOLD_OF_THE_MEDITATION, EVENT(scKandoreanTempleReadTheGuard)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_ON_GUARD_DUTY,
	 CONDITION_ALWAYS, MSGNO(MsgFieldIWasHopingToHearMaster)},
	{TRIGGER_TALK, ACTOR_STUDENT_WARY_OF_OUTSIDERS, FLAG_MET_POI,
	 MSGNO(MsgFieldIHopeToEnterTheTemple)},
	{TRIGGER_TALK, ACTOR_STUDENT_WARY_OF_OUTSIDERS, CONDITION_ALWAYS,
	 MSGNO(MsgFieldHeyWhatsAnOutsiderDoingOn)},
	{TRIGGER_TALK, ACTOR_STUDENT_ON_THE_FAILED_TEST, FLAG_MET_POI,
	 MSGNO(MsgFieldPleaseDontBreakMyConcentrationI)},
	{TRIGGER_TALK, ACTOR_STUDENT_ON_THE_FAILED_TEST, CONDITION_ALWAYS,
	 MSGNO(MsgFieldLooksLikeNoneOfOurBrothers)},
	{TRIGGER_TALK, ACTOR_STUDENT_WHO_WOULD_BE_A_MONK, FLAG_MET_POI,
	 MSGNO(MsgFieldIAmNotJustMeditatingI)},
	{TRIGGER_TALK, ACTOR_STUDENT_WHO_WOULD_BE_A_MONK, CONDITION_ALWAYS,
	 MSGNO(MsgFieldSomedayImGoingToBeA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WARY_OF_OUTSIDERS,
	 FLAG_MET_POI, MSGNO(MsgFieldIStillCantBelieveThatThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WARY_OF_OUTSIDERS,
	 CONDITION_ALWAYS, MSGNO(MsgFieldGahIKeptMyEyesShut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_ON_THE_FAILED_TEST,
	 FLAG_MET_POI, MSGNO(MsgFieldMyMindIsEmptyMyBody)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_ON_THE_FAILED_TEST,
	 CONDITION_ALWAYS, MSGNO(MsgFieldIveBeenMeditatingAllDayBut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
	 FLAG_MET_POI, MSGNO(MsgFieldEmptyTheMindToLightenThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
	 CONDITION_ALWAYS, MSGNO(MsgFieldWheneverTheMasterGoesToTown)},
	{TRIGGER_TABLE_END},
};

/* clang-format off */
TRIGGER KandoreanTemple_PoisRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_GATE, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_GATE},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_WEST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_WEST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_EAST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_EAST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_WEST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_WEST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_EAST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_EAST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_WEST_PASSAGE, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_WEST_PASSAGE},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_WEST_PASSAGE_TO_UPPER_HALL, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_WEST_PASSAGE_TO_UPPER_HALL},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BY_THE_LADDER, CONDITION_ALWAYS, EVENT(scKandoreanTempleClimbDownTheLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CAVE_DOOR, CONDITION_ALWAYS, EVENT(scKandoreanTempleEnterTheCave)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SOUTH_ROPE_WEST_POST, FLAG_SOUTH_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_NORTH_ROPE_EAST_POST, FLAG_NORTH_ROPE_TIED, 0},
    {TRIGGER_TALK, ACTOR_FIRST_TESTED_STUDENT, FLAG_MET_POI, MSGNO(MsgFieldHeWentInBeforeMeWhat)},
    {TRIGGER_TALK, ACTOR_FIRST_TESTED_STUDENT, CONDITION_ALWAYS, EVENT(scKandoreanTempleAskWhoPassedTheTest)},
    {TRIGGER_TALK, ACTOR_SECOND_TESTED_STUDENT, FLAG_MET_POI, MSGNO(MsgFieldEmptyMindEmptyBodyByClearing)},
    {TRIGGER_TALK, ACTOR_SECOND_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldIHearThatSomeonePassedThe)},
    {TRIGGER_TALK, ACTOR_THIRD_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldWhenIAwokeILearnedThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_TESTED_STUDENT, FLAG_MET_POI, MSGNO(MsgFieldTheresNoWayImRootingFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldTheMasterSeemedSoDisappointedThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TESTED_STUDENT, FLAG_MET_POI, MSGNO(MsgFieldArrghIWasSoCloseI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldIKnewItItShouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldBeforeIPassedOutISaw)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_POIS_TABLE, FLAG_LASH_RECEIVED, EVENT(scKandoreanTempleReceiveLash)},
    {TRIGGER_TALK, ACTOR_POI, FLAG_LASH_RECEIVED, MSGNO(MsgFieldTakeThePowerOfLashIt)},
    {TRIGGER_TALK, ACTOR_POI, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWishToPracticeThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_POI, CONDITION_ALWAYS, MSGNO(MsgFieldHoHoIFeelTheStrength)},
    {TRIGGER_TABLE_END},
};
/* clang-format on */
