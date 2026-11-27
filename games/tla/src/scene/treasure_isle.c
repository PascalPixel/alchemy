/* Treasure Isle, the island cave in the north-east corner of the world, its
   five rooms one overlay. It can be entered early, but finishing it needs Grind
   and Isaac's Lift.

   Past the entrance room's three pillars, the chests' room is a lake of small
   islands joined by jumps and a tightrope: a red button on one island raises
   spiky rocks out of the water, which Grind breaks again, and a pillar Moved
   onto a switch in its east part raises one more. The Jester's Armlet room
   beyond has another button and three spiky rocks in a row. In Gale's room,
   boulders stand in the way; Lift raises them into the air for good, and Gale,
   the Jupiter Djinni, waits on the west side. In the last rooms, a Venus Djinni
   from The Broken Seal that the party lacks waits in the west room, and in the
   east chamber the Star Magician, the guardian of water, fights for the Azul
   tablet behind it. */
#include "edition.h"
#include "macros.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/metatile_target.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

/* Set as Move pushes the entrance room's pillar n (0 to 2) aside from its
   doorway. scInit sets all three on entry, so the doorways always stand
   open. */
#define FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(n) (FLAG_AREA_TEMP_00 + (n))
/* Set as spiky rock n (0 to 4) of the chests' room rises, cleared as Grind
   breaks it (TreasureIsleChestsSpikyRocks). */
#define FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(n)		(FLAG_AREA_TEMP_03 + (n))
/* Set as Move pushes the chests' room's stone pillar to its place. */
#define FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED		FLAG_AREA_TEMP_08
/* Set as Move pushes the Jester's Armlet room's stone pillar to its place. */
#define FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED		FLAG_AREA_TEMP_09
/* Set as spiky rock n (0 to 2) of the Jester's Armlet room rises, cleared as
   Grind breaks it (TreasureIsleArmletSpikyRocks). */
#define FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(n)		(FLAG_AREA_TEMP_0A + (n))
/* Set as Move pushes the stone pillar of Gale's room to its place. */
#define FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED	FLAG_AREA_TEMP_0D
/* Set as Lift raises boulder n (0 to 7) of Gale's room; while it is clear the
   boulder stands and the party can jump onto it. */
#define FLAG_TREASURE_ISLE_BOULDER_LIFTED(n)		(FLAG_SCENE_TEMP_00 + (n))
/* Set once the Star Magician, the guardian of water, has been beaten; it is
   gone. */
#define FLAG_STAR_MAGICIAN_BEATEN					0x9bb

/* Gale answers from any side in the Japanese edition; from the international
   editions on, only while the leader faces east. */
#define GALE_TALK_CUE \
	WHEN(GALE_TALK_NEEDS_LEADER_FACING_EAST, TALK_CUE_FACING(FACING_EAST), \
		CUE_TALK)

/* The entrance room's three stone pillars, one beside each doorway they blocked
   (TreasureIsleEntrancePlacements). */
enum {
	ACTOR_ENTRANCE_PILLAR_A = ACTOR_FIRST_PLACED,
	ACTOR_ENTRANCE_PILLAR_B,
	ACTOR_ENTRANCE_PILLAR_C
};

/* The chests' and the Jester's Armlet rooms' tightrope posts and the stone
   pillar Move pushes; the chests' room has a second pillar. */
enum {
	ACTOR_TIGHTROPE_POST_A = ACTOR_FIRST_PLACED,
	ACTOR_TIGHTROPE_POST_B,
	ACTOR_STONE_PILLAR,
	ACTOR_SECOND_STONE_PILLAR
};

/* Gale's room (TreasureIsleGalePlacements): its stone pillar, the eight
   boulders Lift raises, the posts of its three tightropes, and Gale. */
typedef enum TREASURE_ISLE_GALE_ROOM_ACTOR_ID {
	ACTOR_GALE_ROOM_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_FIRST_BOULDER,
	ACTOR_LAST_BOULDER = ACTOR_FIRST_BOULDER + 7,
	ACTOR_GALE_ROOM_TIGHTROPE_POST_A,
	ACTOR_GALE_ROOM_TIGHTROPE_POST_B,
	ACTOR_GALE_ROOM_TIGHTROPE_POST_C,
	ACTOR_GALE_ROOM_TIGHTROPE_POST_D,
	ACTOR_GALE_ROOM_TIGHTROPE_POST_E,
	ACTOR_GALE_ROOM_TIGHTROPE_POST_F,
	ACTOR_GALE
} TREASURE_ISLE_GALE_ROOM_ACTOR_ID;

/* The Star Magician, the guardian of water, in the last room
   (TreasureIsleGuardianPlacements), and the Venus Djinni waiting there: the
   spare DJINNI_EARTH19 stands for one of The Broken Seal's Venus Djinn the
   party lacks (SCENE_STATE_CHOSEN_DJINN), and with none lacking it is not
   placed. */
enum { ACTOR_STAR_MAGICIAN = ACTOR_FIRST_PLACED, ACTOR_VENUS_DJINNI };

/* Where Move leaves each stone pillar, in pixels: scInit puts a pillar whose
   flag is set back there. */
#define ENTRANCE_PILLAR_A_MOVED_X	 264
#define ENTRANCE_PILLAR_A_MOVED_Z	 440
#define ENTRANCE_PILLAR_B_MOVED_X	 328
#define ENTRANCE_PILLAR_B_MOVED_Z	 440
#define ENTRANCE_PILLAR_C_MOVED_X	 440
#define ENTRANCE_PILLAR_C_MOVED_Z	 344
#define CHESTS_PILLAR_MOVED_X		 264
#define CHESTS_PILLAR_MOVED_Z		 184
#define ARMLET_PILLAR_MOVED_X		 664
#define ARMLET_PILLAR_MOVED_Z		 248
#define GALE_ROOM_PILLAR_MOVED_X	 696
#define GALE_ROOM_PILLAR_MOVED_Z	 600
/* The place of the chests' room's second pillar that raises the last spiky
   rock. */
#define CHESTS_SECOND_PILLAR_MOVED_X 680
#define CHESTS_SECOND_PILLAR_MOVED_Z 664

/* The ways between two parts of one room: each leads to its partner's entrance
   and back. */
typedef enum TREASURE_ISLE_WAY {
	/* The entrance hall's three doorways in its north wall, each to a small
	   alcove of two chests. */
	HALL_WEST_DOORWAY = 2,
	WEST_ALCOVE,
	HALL_MIDDLE_DOORWAY,
	MIDDLE_ALCOVE,
	HALL_EAST_DOORWAY,
	EAST_ALCOVE,
	/* The chests' room: the south end of the sand path in its west part, to the
	   cave mouth over the east part's water. */
	CHESTS_ROOM_WEST_PATH = 2,
	CHESTS_ROOM_EAST_CAVE_MOUTH,
	/* The Jester's Armlet room: the cave mouth over its east part's water, to
	   the end of the sand path in its west part. */
	ARMLET_ROOM_EAST_CAVE_MOUTH = 3,
	ARMLET_ROOM_WEST_PATH,
	/* Gale's room: the south part's doorways, above the stone pillar and by the
	   chest, to the ends of the north part's paths east and west. */
	GALE_ROOM_PILLAR_DOORWAY = 2,
	GALE_ROOM_EAST_PATH,
	GALE_ROOM_CHEST_DOORWAY,
	GALE_ROOM_WEST_PATH,
	/* The last rooms: the first room's north doorway, to the foot of the path
	   into the Star Magician's chamber. */
	GUARDIAN_ROOM_DOORWAY = 2,
	STAR_MAGICIAN_CHAMBER_PATH
} TREASURE_ISLE_WAY;

/* What the rooms' map metatiles do when Felix steps on them or casts at them,
   beside their exits. */
enum {
	/* The red button in the chests' and the Jester's Armlet rooms. */
	CUE_BUTTON = 30,
	/* The switch in the chests' room's east part, metatile (42, 41). */
	CUE_SWITCH = 31,
	/* A metatile Felix jumps two metatiles on from, over a gap in the water. */
	CUE_JUMP = 35,
	/* In Gale's room, the metatiles beside its first six boulders Felix jumps
	   from, west and east in turn, two to a boulder. */
	CUE_FIRST_BOULDER_JUMP = 31,
	/* The metatiles before the Star Magician in the last chamber. */
	CUE_BEFORE_STAR_MAGICIAN = 42
};

/* The metatiles Grind breaks each spiky rock from, the chests' room's from
   CHESTS_FIRST_ROCK_METATILE and the Jester's Armlet room's from
   ARMLET_FIRST_ROCK_METATILE, in the order of their rooms' lists. */
#define CHESTS_FIRST_ROCK_METATILE 40
#define ARMLET_FIRST_ROCK_METATILE 50

/* The two background palettes of water whose colours cycle. */
#define WATER_PALETTE_A 3
#define WATER_PALETTE_B 10

struct TREASURE_ISLE_METATILE;

extern ACTOR_PLACE TreasureIsleEntrancePlacements[];

extern ACTOR_PLACE TreasureIsleChestsPlacements[];

extern ACTOR_PLACE TreasureIsleArmletPlacements[];
extern ACTOR_PLACE TreasureIsleGalePlacements[];
extern ACTOR_PLACE TreasureIsleGuardianPlacements[];
extern Uint32	   TreasureIsle_RockDust[];

extern TREASURE_ISLE_METATILE TreasureIsleChestsSpikyRocks[];
extern TREASURE_ISLE_METATILE TreasureIsleArmletSpikyRocks[];
extern CUE					  TreasureIsleChestsCues[];
extern CUE					  TreasureIsleArmletCues[];
extern CUE					  TreasureIsleGaleCues[];
extern CUE					  TreasureIsleGuardianCues[];

void scTreasureIsleSetSpikyRock(
	TREASURE_ISLE_METATILE *metatiles, Sint16 index);

void scTreasureIsleCycleWaterCols(void);

extern Uint32 FieldEffect_ShortScript[];
extern Uint32 FieldEffect_MediumScript[];
extern Uint32 FieldEffect_LongScript[];
extern Uint32 TreasureIsle_PostStill[];
extern Uint32 TreasureIsle_LiftRockScale[];

/* One palette's sixteen colours. */
typedef struct PALETTE {
	Uint16 cols[PALETTE_COLORS];
} PALETTE;

PALETTE TreasureIsle_SavedWaterColorsA BSS_DATA;
PALETTE TreasureIsle_WaterColorsA	   BSS_DATA;
PALETTE TreasureIsle_SavedWaterColorsB BSS_DATA;
PALETTE TreasureIsle_WaterColorsB	   BSS_DATA;

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

/* The actors of the room the party enters. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_TREASURE_ISLE_CHESTS)
		return TreasureIsleChestsPlacements;
	if(scene == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST)
		return TreasureIsleArmletPlacements;
	if(scene == SCENE_TREASURE_ISLE_GALE)
		return TreasureIsleGalePlacements;
	if(scene == SCENE_TREASURE_ISLE_GUARDIAN)
		return TreasureIsleGuardianPlacements;
	return TreasureIsleEntrancePlacements;
}

extern MAP_METATILE MapGridMetatileBuffer[];

#include "shared/map_metatile_set.c"

/* Draws a spiky rock risen at its metatile, blocks the metatile and sets its
   flag. */
void scTreasureIsleSetSpikyRock(TREASURE_ISLE_METATILE *metatiles, Sint16 index)
{
	fdMapGridCopyMetatilesLayered(
		0, 100, 3, 4, metatiles[index].x + 63, metatiles[index].z - 2);
	fdMapGridCopyMetatileAttributes(
		1, 0, 1, 1, metatiles[index].x, metatiles[index].z);
	fdScriptSetFlag(metatiles[index].flg);
}

/* A spiky rock rises out of the ground at a listed metatile: the camera goes to
   it, the ground shakes and dust bursts, and scTreasureIsleSetSpikyRock leaves
   it standing. */
void scTreasureIsleRaiseSpikyRock(
	TREASURE_ISLE_METATILE *metatiles, Sint16 index)
{
	EFFECT_OPTIONS options;

	fdCameraSetSpeed(6);
	fdCameraMoveToFX(FIELD_METATILE_CENTER(metatiles[index].x), -1,
		FIELD_METATILE_CENTER(metatiles[index].z), 1);
	fdCameraWaitForArrival();
	fdMapGridSetShake(0, 16, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptPlaySnd(SE_POUND);
	options.startScaleX = toFIXED(12) / 10;
	options.startScaleY = toFIXED(3) / 2;
	options.pri = 1;
	options.type = SPRITE_WATER_SPRAY;
	options.script = TreasureIsle_RockDust;
	scFieldEffectSpawn(FIELD_METATILE_CENTER(metatiles[index].x), 0,
		FIELD_METATILE_ORIGIN(metatiles[index].z) + toFIXED(20), 0, 0, 0,
		EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
			EFFECT_USE_PRIORITY,
		&options);
	options.startScaleX = toFIXED(6) / 10;
	scFieldEffectSpawn(FIELD_METATILE_ORIGIN(metatiles[index].x) - toFIXED(2),
		0, FIELD_METATILE_CENTER(metatiles[index].z), 0, 0, 0,
		EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
			EFFECT_USE_PRIORITY,
		&options);
	scFieldEffectSpawn(FIELD_METATILE_ORIGIN(metatiles[index].x) + toFIXED(18),
		0, FIELD_METATILE_CENTER(metatiles[index].z), 0, 0, 0,
		EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
			EFFECT_USE_PRIORITY,
		&options);
	syFieldScriptWait(10);
	fdMapGridCopyMetatilesLayered(
		6, 100, 3, 4, metatiles[index].x + 63, metatiles[index].z - 2);
	syFieldScriptWait(10);
	fdMapGridCopyMetatilesLayered(
		3, 100, 3, 4, metatiles[index].x + 63, metatiles[index].z - 2);
	syFieldScriptWait(10);
	scTreasureIsleSetSpikyRock(metatiles, index);
}

/* Stepping on the red button on a small island in the chests' room's water,
   metatile (9, 15), raises the room's first four spiky rocks out of the water
   one after another, the camera going to each as it bursts up in a spray: at
   metatiles (7, 21), (7, 17), (12, 15) and (9, 11). A rock Grind has broken
   rises again; one still standing is left as it is. */
void scTreasureIslePressChestsRoomButton(void)
{
	Sint16 count = 0;

	if(!fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0))) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scTreasureIsleRaiseSpikyRock(TreasureIsleChestsSpikyRocks, 0);
		fdScriptSetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0));
		count = 1;
	}
	if(!fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1))) {
		if(count == 0)
			fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scTreasureIsleRaiseSpikyRock(TreasureIsleChestsSpikyRocks, 1);
		fdScriptSetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1));
		count++;
	}
	if(!fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2))) {
		if(count == 0)
			fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scTreasureIsleRaiseSpikyRock(TreasureIsleChestsSpikyRocks, 2);
		fdScriptSetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2));
		count++;
	}
	if(!fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3))) {
		if(count == 0)
			fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scTreasureIsleRaiseSpikyRock(TreasureIsleChestsSpikyRocks, 3);
		fdScriptSetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3));
		count++;
	}
	if(count != 0)
		fdScriptEnd();
}

/* Stepping on the red button in the Jester's Armlet room, metatile (47, 27),
   raises the room's three spiky rocks out of the water in a row, one after
   another, at metatiles (41, 23), (47, 23) and (53, 23). A rock Grind has
   broken rises again. */
void scTreasureIslePressArmletRoomButton(void)
{
	Sint16 count = 0;

	if(!fdScriptGetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0))) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scTreasureIsleRaiseSpikyRock(TreasureIsleArmletSpikyRocks, 0);
		fdScriptSetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0));
		count = 1;
	}
	if(!fdScriptGetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1))) {
		if(count == 0)
			fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scTreasureIsleRaiseSpikyRock(TreasureIsleArmletSpikyRocks, 1);
		fdScriptSetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1));
		count++;
	}
	if(!fdScriptGetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2))) {
		if(count == 0)
			fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scTreasureIsleRaiseSpikyRock(TreasureIsleArmletSpikyRocks, 2);
		fdScriptSetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2));
		count++;
	}
	if(count != 0)
		fdScriptEnd();
}

/* Stepping on the switch in the chests' room's east part, metatile (42, 41), or
   pushing its stone pillar onto it, raises a fifth spiky rock out of the water
   at metatile (46, 38), once. */
void scTreasureIsleRaiseLastSpikyRock(void)
{
	if(!fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4))) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scTreasureIsleRaiseSpikyRock(TreasureIsleChestsSpikyRocks, 4);
		fdScriptSetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4));
		fdScriptEnd();
	}
}

/* Move or a push leaving one of the stone pillars where it belongs sets its
   flag, so the pillar is put back there next time; nothing more is seen. The
   pillar in the chests' room's east part, left on its switch, metatile (42,
   41), raises the fifth spiky rock (scTreasureIsleRaiseLastSpikyRock). */
void scTreasureIsleSettleMovedPillar(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	if(MainRoutineStateWork.next == SCENE_TREASURE_ISLE_ENTRANCE) {
		if(id == ACTOR_ENTRANCE_PILLAR_A &&
			FIELD_METATILE(actor->x.fixed) ==
				FIELD_METATILE(toFIXED(ENTRANCE_PILLAR_A_MOVED_X)))
			fdScriptSetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0));
		if(id == ACTOR_ENTRANCE_PILLAR_B &&
			FIELD_METATILE(actor->x.fixed) ==
				FIELD_METATILE(toFIXED(ENTRANCE_PILLAR_B_MOVED_X)))
			fdScriptSetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1));
		if(id == ACTOR_ENTRANCE_PILLAR_C &&
			FIELD_METATILE(actor->x.fixed) ==
				FIELD_METATILE(toFIXED(ENTRANCE_PILLAR_C_MOVED_X)))
			fdScriptSetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2));
	}
	if(MainRoutineStateWork.next == SCENE_TREASURE_ISLE_CHESTS) {
		if(id == ACTOR_STONE_PILLAR &&
			FIELD_HALF_METATILE(actor->x.fixed) ==
				FIELD_HALF_METATILE(toFIXED(CHESTS_PILLAR_MOVED_X)))
			fdScriptSetFlag(FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED);
		if(id == ACTOR_SECOND_STONE_PILLAR &&
			FIELD_HALF_METATILE(actor->x.fixed) ==
				FIELD_HALF_METATILE(toFIXED(CHESTS_SECOND_PILLAR_MOVED_X)) &&
			FIELD_HALF_METATILE(actor->z.fixed) ==
				FIELD_HALF_METATILE(toFIXED(CHESTS_SECOND_PILLAR_MOVED_Z)))
			scTreasureIsleRaiseLastSpikyRock();
	}
	if(MainRoutineStateWork.next == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST) {
		if(id == ACTOR_STONE_PILLAR &&
			FIELD_HALF_METATILE(actor->x.fixed) ==
				FIELD_HALF_METATILE(toFIXED(ARMLET_PILLAR_MOVED_X)))
			fdScriptSetFlag(FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED);
	}
	if(MainRoutineStateWork.next == SCENE_TREASURE_ISLE_GALE) {
		if(id == ACTOR_GALE_ROOM_PILLAR &&
			FIELD_HALF_METATILE(actor->x.fixed) ==
				FIELD_HALF_METATILE(toFIXED(GALE_ROOM_PILLAR_MOVED_X)))
			fdScriptSetFlag(FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED);
	}
}

/* Talking to Gale, the Jupiter Djinni, starts its battle. */
void scTreasureIsleFightGale(void)
{
	fdRequestDjinnBattle(ACTOR_GALE, DJINNI_GALE);
}

/* Talking to the Venus Djinni waiting in the west room before the Star
   Magician's chamber, metatile (12, 8), makes it join the party without a
   fight. It is one of The Broken Seal's Venus Djinn the party lacks, and is not
   there when none is lacking. */
void scTreasureIsleVenusDjinniJoins(void)
{
	fdDjinnJoinParty(ACTOR_VENUS_DJINNI, DJINNI_EARTH19 / DJINN_PER_ELEMENT,
		DJINNI_INDEX(DJINNI_EARTH19));
}

/* Grind on a spiky rock in the chests' or the Jester's Armlet room crushes it
   back into the water in three steps, clearing the way. Grind calls this as it
   plays, with how far it has got in `frame`, and `id` says which rock. */
void scTreasureIsleGrindSpikyRock(Sint32 unused, Sint32 frame, Sint32 id)
{
	TREASURE_ISLE_METATILE *metatiles;

	if(id < ARMLET_FIRST_ROCK_METATILE) {
		metatiles = TreasureIsleChestsSpikyRocks;
		id -= CHESTS_FIRST_ROCK_METATILE;
	} else {
		metatiles = TreasureIsleArmletSpikyRocks;
		id -= ARMLET_FIRST_ROCK_METATILE;
	}
	if(frame == 15) {
		fdMapGridCopyMetatilesLayered(
			3, 100, 3, 4, metatiles[id].x + 63, metatiles[id].z - 2);
	} else if(frame == 30) {
		fdMapGridCopyMetatilesLayered(
			6, 100, 3, 4, metatiles[id].x + 63, metatiles[id].z - 2);
	} else if(frame == 45) {
		fdMapGridCopyMetatilesLayered(
			9, 100, 3, 4, metatiles[id].x + 63, metatiles[id].z - 2);
		fdMapGridCopyMetatileAttributes(
			0, 0, 1, 1, metatiles[id].x, metatiles[id].z);
		fdScriptClearFlag(metatiles[id].flg);
	}
}

/* Lift raises boulder n of Gale's room out of the way for good: its sprite is
   drawn at priority 1 from now on, no longer taken from the map, and its flag
   is set. */
#define DEFINE_LIFT_ROCK(name, n) \
	void name(void) \
	{ \
		ACTOR *boulder = fdActorGet(ACTOR_FIRST_BOULDER + (n)); \
\
		((FIELD_SPR *)boulder->spr)->pri = 1; \
		boulder->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC; \
		fdScriptSetFlag(FLAG_TREASURE_ISLE_BOULDER_LIFTED(n)); \
	}

/* Lift on the east boulder of the south row in the west of Gale's room,
   metatile (20, 20), raises it into the air out of the way for good. While it
   stands, Felix can jump across it from the metatiles either side. */
DEFINE_LIFT_ROCK(TreasureIsle_LiftFirstRock, 0)

/* Lift on the middle boulder of the south row in the west of Gale's room,
   metatile (13, 19), raises it into the air out of the way for good. While it
   stands, Felix can jump across it. */
DEFINE_LIFT_ROCK(TreasureIsle_LiftSecondRock, 1)

/* Lift on the west boulder of the south row in the west of Gale's room,
   metatile (6, 20), raises it into the air out of the way for good. While it
   stands, Felix can jump across it. */
DEFINE_LIFT_ROCK(TreasureIsle_LiftThirdRock, 2)

/* Lift on the east boulder of the north row in the west of Gale's room,
   metatile (19, 13), raises it into the air out of the way for good. While it
   stands, Felix can jump across it. */
DEFINE_LIFT_ROCK(TreasureIsle_LiftFourthRock, 3)

/* Lift on the middle boulder of the north row in the west of Gale's room,
   metatile (13, 12), raises it into the air out of the way for good. While it
   stands, Felix can jump across it. */
DEFINE_LIFT_ROCK(TreasureIsle_LiftFifthRock, 4)

/* Lift on the west boulder of the north row in the west of Gale's room,
   metatile (7, 13), raises it into the air out of the way for good. While it
   stands, Felix can jump across it. */
DEFINE_LIFT_ROCK(TreasureIsle_LiftSixthRock, 5)

/* Lift on the boulder blocking the narrow passage north in the south-east of
   Gale's room, metatile (46, 45), raises it into the air above the passage for
   good. */
DEFINE_LIFT_ROCK(TreasureIsle_LiftSeventhRock, 6)

/* Lift on the boulder in the south of Gale's room, metatile (26, 42), raises it
   into the air out of the way for good. */
DEFINE_LIFT_ROCK(TreasureIsle_LiftEighthRock, 7)

/* Walking up the steps to the Star Magician in the last chamber, metatiles (48,
   10) to (50, 10), until it is beaten: it stirs, the screen glows warm, and it
   says "I am the guardian of water. If you would claim my power, you must prove
   your worth." Then it moves at Felix and the battle with it and its balls
   starts, with no escape. */
void scTreasureIsleMeetStarMagician(void)
{
	ACTOR *leader;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorStartRepeatedMotion(ACTOR_STAR_MAGICIAN, 2);
	syFieldScriptWait(20);
	fdPaletteFadeApplySource(toFIXED(1), 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
	fdPaletteFadeInterpolate(60);
	syFieldScriptWait(60);
	mnFieldMessageShowCentered((Sint32)MsgIAmTheGuardian, 0, 6);
	fdPaletteFadeApplyTarget(toFIXED(1), 0);
	fdPaletteFadeInterpolate(60);
	syFieldScriptWait(60);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0) {
		fdActorSetDestination(ACTOR_STAR_MAGICIAN, leader->x.part[0].integer,
			leader->z.part[0].integer);
	}
	syFieldScriptWait(20);
	MainRoutineStateWork.pendingDjinni = IF_FLAG_SET(FLAG_STAR_MAGICIAN_BEATEN);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_HIDDEN_BOSSES, HIDDEN_BOSS_TREASURE_ISLE);
}

extern CUE TreasureIsleEntranceCues[];

/* The cues of the room the party enters. */
const CUE *scGetCues(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_TREASURE_ISLE_CHESTS)
		return TreasureIsleChestsCues;
	if(scene == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST)
		return TreasureIsleArmletCues;
	if(scene == SCENE_TREASURE_ISLE_GALE)
		return TreasureIsleGaleCues;
	if(scene == SCENE_TREASURE_ISLE_GUARDIAN)
		return TreasureIsleGuardianCues;
	return TreasureIsleEntranceCues;
}

/* The water colours' cycle frame, -1 until the first frame saves the
   palettes. */
extern Sint32 TreasureIsle_ColorCycleFrame[];

/* An event: cycles four colours of the two water palettes every eight frames,
   saving both palettes the first time. */
void scTreasureIsleCycleWaterCols(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 phase;
	Sint32		 i;
	Sint32		 busy;

	if(state->psynergy != 0)
		return;
	busy = (Sint8)state->revealActive;
	if(busy != 0)
		return;
	if(TreasureIsle_ColorCycleFrame[0] == -1) {
		syWordsCopy(TreasureIsle_SavedWaterColorsA.cols,
			(void *)&BG_PLTT_COLOR(WATER_PALETTE_A, 0), sizeof(PALETTE));
		syWordsCopyEntry(TreasureIsle_WaterColorsA.cols,
			TreasureIsle_SavedWaterColorsA.cols, sizeof(PALETTE));
		syWordsCopyEntry(TreasureIsle_SavedWaterColorsB.cols,
			(void *)&BG_PLTT_COLOR(WATER_PALETTE_B, 0), sizeof(PALETTE));
		syWordsCopyEntry(TreasureIsle_WaterColorsB.cols,
			TreasureIsle_SavedWaterColorsB.cols, sizeof(PALETTE));
		TreasureIsle_ColorCycleFrame[0] = 0;
	}
	phase = TreasureIsle_ColorCycleFrame[0] / 8 % 4;
	for(i = 0; i <= 3; i++) {
		TreasureIsle_WaterColorsA.cols[(phase + i) % 4 + 2] =
			TreasureIsle_SavedWaterColorsA.cols[i + 2];
		TreasureIsle_WaterColorsB.cols[(phase + i) % 4 + 2] =
			TreasureIsle_SavedWaterColorsB.cols[i + 2];
	}
	IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(WATER_PALETTE_A, 0),
		(Uint32)TreasureIsle_WaterColorsA.cols,
		DMA_COPY_WORDS(PALETTE_COLORS / 2));
	IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(WATER_PALETTE_B, 0),
		(Uint32)TreasureIsle_WaterColorsB.cols,
		DMA_COPY_WORDS(PALETTE_COLORS / 2));
	TreasureIsle_ColorCycleFrame[0]++;
}

/* Treasure Isle's setup for each of its rooms: places the pillars already
   pushed to their places, cycles the water colours in the chests' and the
   Jester's Armlet rooms and stands their risen spiky rocks again, and in Gale's
   room strings the tightropes and draws the boulders' shadows at the lowest
   priority. */
Sint32 scInit(void)
{
	Sint32 i;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdTightropeInit();
	if(MainRoutineStateWork.next == SCENE_TREASURE_ISLE_ENTRANCE) {
		fdScriptSetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0));
		fdScriptSetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1));
		fdScriptSetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2));
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0)))
			fdActorSetPos(ACTOR_ENTRANCE_PILLAR_A, ENTRANCE_PILLAR_A_MOVED_X,
				ENTRANCE_PILLAR_A_MOVED_Z);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1)))
			fdActorSetPos(ACTOR_ENTRANCE_PILLAR_B, ENTRANCE_PILLAR_B_MOVED_X,
				ENTRANCE_PILLAR_B_MOVED_Z);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2)))
			fdActorSetPos(ACTOR_ENTRANCE_PILLAR_C, ENTRANCE_PILLAR_C_MOVED_X,
				ENTRANCE_PILLAR_C_MOVED_Z);
	}
	if(MainRoutineStateWork.next == SCENE_TREASURE_ISLE_CHESTS) {
		sySetFrameEvent(scTreasureIsleCycleWaterCols, EVENT_PASS_FRAME);
		fdTightropePlace(0, ACTOR_TIGHTROPE_POST_A, ACTOR_TIGHTROPE_POST_B);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED))
			fdActorSetPos(ACTOR_STONE_PILLAR, CHESTS_PILLAR_MOVED_X,
				CHESTS_PILLAR_MOVED_Z);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)))
			scTreasureIsleSetSpikyRock(TreasureIsleChestsSpikyRocks, 0);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)))
			scTreasureIsleSetSpikyRock(TreasureIsleChestsSpikyRocks, 1);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)))
			scTreasureIsleSetSpikyRock(TreasureIsleChestsSpikyRocks, 2);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)))
			scTreasureIsleSetSpikyRock(TreasureIsleChestsSpikyRocks, 3);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4)))
			scTreasureIsleSetSpikyRock(TreasureIsleChestsSpikyRocks, 4);
	}
	if(MainRoutineStateWork.next == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST) {
		sySetFrameEvent(scTreasureIsleCycleWaterCols, EVENT_PASS_FRAME);
		fdTightropePlace(0, ACTOR_TIGHTROPE_POST_A, ACTOR_TIGHTROPE_POST_B);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED))
			fdActorSetPos(ACTOR_STONE_PILLAR, ARMLET_PILLAR_MOVED_X,
				ARMLET_PILLAR_MOVED_Z);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)))
			scTreasureIsleSetSpikyRock(TreasureIsleArmletSpikyRocks, 0);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)))
			scTreasureIsleSetSpikyRock(TreasureIsleArmletSpikyRocks, 1);
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2)))
			scTreasureIsleSetSpikyRock(TreasureIsleArmletSpikyRocks, 2);
	}
	if(MainRoutineStateWork.next == SCENE_TREASURE_ISLE_GALE) {
		if(fdScriptGetFlag(FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED))
			fdActorSetPos(ACTOR_GALE_ROOM_PILLAR, GALE_ROOM_PILLAR_MOVED_X,
				GALE_ROOM_PILLAR_MOVED_Z);
		fdTightropePlace(0, ACTOR_GALE_ROOM_TIGHTROPE_POST_A,
			ACTOR_GALE_ROOM_TIGHTROPE_POST_B);
		fdTightropePlace(1, ACTOR_GALE_ROOM_TIGHTROPE_POST_C,
			ACTOR_GALE_ROOM_TIGHTROPE_POST_D);
		fdTightropePlace(2, ACTOR_GALE_ROOM_TIGHTROPE_POST_E,
			ACTOR_GALE_ROOM_TIGHTROPE_POST_F);
		for(i = 0; i <= 7; i++)
			((FIELD_SPR *)fdActorGet(i + ACTOR_FIRST_BOULDER)->spr)->partPri =
				3;
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Walking towards a gap from one of its jump metatiles, Felix jumps two
   metatiles across it if there is ground to land on: in the chests' room from
   metatile (18, 11) east, in the Jester's Armlet room from metatile (43, 15)
   north or east, and in Gale's room across a standing boulder. */
void scTreasureIsleJumpForward(void)
{
	fdTryJumpForward();
}

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

Uint32 TreasureIsle_RockDust[] = {
	SC_ANIMATION(2),
	SC_LABEL(1),
	SC_WAIT(1),
	SC_LOOP(44, SC_TO(1)),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
	SC_END,
};

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* The spiky rocks that rise in the chests' room and the Armlet's room: each */
/* rock's metatile, a number no code reads (in the chests' room, one of its */
/* five sea rocks' actor ids), and the flag set while it stands. */
TREASURE_ISLE_METATILE TreasureIsleChestsSpikyRocks[] = {
	{7, 21, 12, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)},
	{7, 17, 13, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)},
	{12, 15, 14, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)},
	{9, 11, 15, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)},
	{46, 38, 16, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4)},
};
TREASURE_ISLE_METATILE TreasureIsleArmletSpikyRocks[] = {
	{41, 23, 11, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)},
	{47, 23, 12, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)},
	{53, 23, 13, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2)},
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(TreasureIsleEntrance),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TREASURE_ISLE_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HALL_WEST_DOORWAY, TreasureIsleEntrance, WEST_ALCOVE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WEST_ALCOVE, TreasureIsleEntrance, HALL_WEST_DOORWAY, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HALL_MIDDLE_DOORWAY, TreasureIsleEntrance, MIDDLE_ALCOVE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(MIDDLE_ALCOVE, TreasureIsleEntrance, HALL_MIDDLE_DOORWAY, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HALL_EAST_DOORWAY, TreasureIsleEntrance, EAST_ALCOVE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EAST_ALCOVE, TreasureIsleEntrance, HALL_EAST_DOORWAY, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_ENTRANCE_TO_TREASURE_ISLE_CHESTS, TreasureIsleChests, TREASURE_ISLE_CHESTS_FROM_TREASURE_ISLE_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXITS(TreasureIsleChests),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_ENTRANCE, TreasureIsleEntrance, TREASURE_ISLE_ENTRANCE_FROM_TREASURE_ISLE_CHESTS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHESTS_ROOM_WEST_PATH, TreasureIsleChests, CHESTS_ROOM_EAST_CAVE_MOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(CHESTS_ROOM_EAST_CAVE_MOUTH, TreasureIsleChests, CHESTS_ROOM_WEST_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST_EAST, TreasureIsleJestersArmletChest, TREASURE_ISLE_JESTERS_ARMLET_CHEST_FROM_TREASURE_ISLE_CHESTS_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST_WEST, TreasureIsleJestersArmletChest, TREASURE_ISLE_JESTERS_ARMLET_CHEST_FROM_TREASURE_ISLE_CHESTS_WEST, EXIT_FLAG_NONE),
	SCENE_EXITS(TreasureIsleJestersArmletChest),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_CHESTS_EAST, TreasureIsleChests, TREASURE_ISLE_CHESTS_FROM_TREASURE_ISLE_JESTERS_ARMLET_CHEST_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_CHESTS_WEST, TreasureIsleChests, TREASURE_ISLE_CHESTS_FROM_TREASURE_ISLE_JESTERS_ARMLET_CHEST_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ARMLET_ROOM_EAST_CAVE_MOUTH, TreasureIsleJestersArmletChest, ARMLET_ROOM_WEST_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ARMLET_ROOM_WEST_PATH, TreasureIsleJestersArmletChest, ARMLET_ROOM_EAST_CAVE_MOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_GALE, TreasureIsleGale, TREASURE_ISLE_GALE_FROM_TREASURE_ISLE_JESTERS_ARMLET_CHEST, EXIT_FLAG_NONE),
	SCENE_EXITS(TreasureIsleGale),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_GALE_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST, TreasureIsleJestersArmletChest, TREASURE_ISLE_JESTERS_ARMLET_CHEST_FROM_TREASURE_ISLE_GALE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GALE_ROOM_PILLAR_DOORWAY, TreasureIsleGale, GALE_ROOM_EAST_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GALE_ROOM_EAST_PATH, TreasureIsleGale, GALE_ROOM_PILLAR_DOORWAY, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GALE_ROOM_CHEST_DOORWAY, TreasureIsleGale, GALE_ROOM_WEST_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GALE_ROOM_WEST_PATH, TreasureIsleGale, GALE_ROOM_CHEST_DOORWAY, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_GALE_TO_TREASURE_ISLE_GUARDIAN, TreasureIsleGuardian, TREASURE_ISLE_GUARDIAN_FROM_TREASURE_ISLE_GALE, EXIT_FLAG_NONE),
	SCENE_EXITS(TreasureIsleGuardian),
	SCENE_EXIT_UNLESS(TREASURE_ISLE_GUARDIAN_TO_TREASURE_ISLE_GALE, TreasureIsleGale, TREASURE_ISLE_GALE_FROM_TREASURE_ISLE_GUARDIAN, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GUARDIAN_ROOM_DOORWAY, TreasureIsleGuardian, STAR_MAGICIAN_CHAMBER_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(STAR_MAGICIAN_CHAMBER_PATH, TreasureIsleGuardian, GUARDIAN_ROOM_DOORWAY, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

Uint32 TreasureIsle_PostStill[] = {
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_END,
};

ACTOR_PLACE TreasureIsleEntrancePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE TreasureIsleChestsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 600, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 680, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE TreasureIsleArmletPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 824, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 824, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

Uint32 TreasureIsle_LiftRockScale[] = {
	SC_SET(SW_SCALE_X, toFIXED(4) / 5),
	SC_SET(SW_SCALE_Y, toFIXED(4) / 5),
	SC_WAIT(1),
	SC_END,
};

ACTOR_PLACE TreasureIsleGalePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (Sint32)TreasureIsle_LiftRockScale, 328, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (Sint32)TreasureIsle_LiftRockScale, 216, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (Sint32)TreasureIsle_LiftRockScale, 104, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (Sint32)TreasureIsle_LiftRockScale, 312, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (Sint32)TreasureIsle_LiftRockScale, 216, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (Sint32)TreasureIsle_LiftRockScale, 120, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (Sint32)TreasureIsle_LiftRockScale, 744, 0, 728, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (Sint32)TreasureIsle_LiftRockScale, 424, 0, 680, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 360, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 360, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 152, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 152, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 72, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)TreasureIsle_PostStill, 72, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_GALE), MOVEMENT_TYPE_STAND, 88, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE TreasureIsleGuardianPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STAR_MAGICIAN, FLAG_STAR_MAGICIAN_BEATEN, MOVEMENT_TYPE_STAND, 792, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_EARTH19), MOVEMENT_TYPE_STAND, 200, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE TreasureIsleEntranceCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, TREASURE_ISLE_ENTRANCE_TO_WORLD_MAP},
	{CUE_EXIT, HALL_WEST_DOORWAY, CONDITION_ALWAYS, HALL_WEST_DOORWAY},
	{CUE_EXIT, WEST_ALCOVE, CONDITION_ALWAYS, WEST_ALCOVE},
	{CUE_EXIT, HALL_MIDDLE_DOORWAY, CONDITION_ALWAYS, HALL_MIDDLE_DOORWAY},
	{CUE_EXIT, MIDDLE_ALCOVE, CONDITION_ALWAYS, MIDDLE_ALCOVE},
	{CUE_EXIT, HALL_EAST_DOORWAY, CONDITION_ALWAYS, HALL_EAST_DOORWAY},
	{CUE_EXIT, EAST_ALCOVE, CONDITION_ALWAYS, EAST_ALCOVE},
	{CUE_EXIT, 8, CONDITION_ALWAYS,
		TREASURE_ISLE_ENTRANCE_TO_TREASURE_ISLE_CHESTS},
	{CUE_PUSHED, 0, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENTRANCE_PILLAR_A,
		FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0),
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENTRANCE_PILLAR_B,
		FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1),
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENTRANCE_PILLAR_C,
		FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2),
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{CUE_TABLE_END},
};

CUE TreasureIsleChestsCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_ENTRANCE},
	{CUE_EXIT, CHESTS_ROOM_WEST_PATH, CONDITION_ALWAYS, CHESTS_ROOM_WEST_PATH},
	{CUE_EXIT, CHESTS_ROOM_EAST_CAVE_MOUTH, CONDITION_ALWAYS,
		CHESTS_ROOM_EAST_CAVE_MOUTH},
	{CUE_EXIT, 4, CONDITION_ALWAYS,
		TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST_EAST},
	{CUE_EXIT, 5, CONDITION_ALWAYS,
		TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST_WEST},
	{CUE_STEP, CUE_BUTTON, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIslePressChestsRoomButton)},
	{CUE_STEP, CUE_SWITCH, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleRaiseLastSpikyRock)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), CUE_JUMP, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
		CHESTS_FIRST_ROCK_METATILE,
		IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)),
		SCRIPT_REF(scTreasureIsleGrindSpikyRock)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
		CHESTS_FIRST_ROCK_METATILE + 1,
		IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)),
		SCRIPT_REF(scTreasureIsleGrindSpikyRock)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
		CHESTS_FIRST_ROCK_METATILE + 2,
		IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)),
		SCRIPT_REF(scTreasureIsleGrindSpikyRock)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
		CHESTS_FIRST_ROCK_METATILE + 3,
		IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)),
		SCRIPT_REF(scTreasureIsleGrindSpikyRock)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
		CHESTS_FIRST_ROCK_METATILE + 4,
		IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4)),
		SCRIPT_REF(scTreasureIsleGrindSpikyRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_PILLAR,
		FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED,
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_STONE_PILLAR,
		CONDITION_ALWAYS, SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{CUE_TABLE_END},
};

CUE TreasureIsleArmletCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_CHESTS_EAST},
	{CUE_EXIT, 2, CONDITION_ALWAYS,
		TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_CHESTS_WEST},
	{CUE_EXIT, ARMLET_ROOM_EAST_CAVE_MOUTH, CONDITION_ALWAYS,
		ARMLET_ROOM_EAST_CAVE_MOUTH},
	{CUE_EXIT, ARMLET_ROOM_WEST_PATH, CONDITION_ALWAYS, ARMLET_ROOM_WEST_PATH},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS,
		TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_GALE},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), CUE_JUMP, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), CUE_JUMP, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{CUE_STEP, CUE_BUTTON, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIslePressArmletRoomButton)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
		ARMLET_FIRST_ROCK_METATILE,
		IF_FLAG_SET(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)),
		SCRIPT_REF(scTreasureIsleGrindSpikyRock)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
		ARMLET_FIRST_ROCK_METATILE + 1,
		IF_FLAG_SET(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)),
		SCRIPT_REF(scTreasureIsleGrindSpikyRock)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
		ARMLET_FIRST_ROCK_METATILE + 2,
		IF_FLAG_SET(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2)),
		SCRIPT_REF(scTreasureIsleGrindSpikyRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_PILLAR,
		FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED,
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{CUE_TABLE_END},
};

/* Gale's room: its exits, the jumps between the pillar tops and boulders while
   each boulder stands, Gale, the stone pillar Move pushes and the boulders Lift
   raises. */
CUE TreasureIsleGaleCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
		TREASURE_ISLE_GALE_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST},
	{CUE_EXIT, GALE_ROOM_PILLAR_DOORWAY, CONDITION_ALWAYS,
		GALE_ROOM_PILLAR_DOORWAY},
	{CUE_EXIT, GALE_ROOM_EAST_PATH, CONDITION_ALWAYS, GALE_ROOM_EAST_PATH},
	{CUE_EXIT, GALE_ROOM_CHEST_DOORWAY, CONDITION_ALWAYS,
		GALE_ROOM_CHEST_DOORWAY},
	{CUE_EXIT, GALE_ROOM_WEST_PATH, CONDITION_ALWAYS, GALE_ROOM_WEST_PATH},
	{CUE_EXIT, 6, CONDITION_ALWAYS,
		TREASURE_ISLE_GALE_TO_TREASURE_ISLE_GUARDIAN},
	{STEP_CUE_WALKING_INTO(FACING_WEST), CUE_FIRST_BOULDER_JUMP,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(0),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), CUE_FIRST_BOULDER_JUMP + 1,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(0),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), CUE_FIRST_BOULDER_JUMP + 2,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(1),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), CUE_FIRST_BOULDER_JUMP + 3,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(1),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), CUE_FIRST_BOULDER_JUMP + 4,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(2),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), CUE_FIRST_BOULDER_JUMP + 5,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(2),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), CUE_FIRST_BOULDER_JUMP + 6,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(3),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), CUE_FIRST_BOULDER_JUMP + 7,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(3),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), CUE_FIRST_BOULDER_JUMP + 8,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(4),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), CUE_FIRST_BOULDER_JUMP + 9,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(4),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), CUE_FIRST_BOULDER_JUMP + 10,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(5),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), CUE_FIRST_BOULDER_JUMP + 11,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(5),
		SCRIPT_REF(scTreasureIsleJumpForward)},
	{GALE_TALK_CUE, ACTOR_GALE, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleFightGale)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GALE_ROOM_PILLAR,
		FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED,
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleSettleMovedPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(0),
		SCRIPT_REF(TreasureIsle_LiftFirstRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 1,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(1),
		SCRIPT_REF(TreasureIsle_LiftSecondRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 2,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(2),
		SCRIPT_REF(TreasureIsle_LiftThirdRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 3,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(3),
		SCRIPT_REF(TreasureIsle_LiftFourthRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 4,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(4),
		SCRIPT_REF(TreasureIsle_LiftFifthRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 5,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(5),
		SCRIPT_REF(TreasureIsle_LiftSixthRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 6,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(6),
		SCRIPT_REF(TreasureIsle_LiftSeventhRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_LAST_BOULDER,
		FLAG_TREASURE_ISLE_BOULDER_LIFTED(7),
		SCRIPT_REF(TreasureIsle_LiftEighthRock)},
	{CUE_TABLE_END},
};

CUE TreasureIsleGuardianCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		TREASURE_ISLE_GUARDIAN_TO_TREASURE_ISLE_GALE},
	{CUE_EXIT, GUARDIAN_ROOM_DOORWAY, CONDITION_ALWAYS, GUARDIAN_ROOM_DOORWAY},
	{CUE_EXIT, STAR_MAGICIAN_CHAMBER_PATH, CONDITION_ALWAYS,
		STAR_MAGICIAN_CHAMBER_PATH},
	{CUE_STEP, CUE_BEFORE_STAR_MAGICIAN, FLAG_STAR_MAGICIAN_BEATEN,
		SCRIPT_REF(scTreasureIsleMeetStarMagician)},
	{CUE_TALK, ACTOR_VENUS_DJINNI, CONDITION_ALWAYS,
		SCRIPT_REF(scTreasureIsleVenusDjinniJoins)},
	{CUE_TABLE_END},
};

Sint32 TreasureIsle_ColorCycleFrame[] = {
	-1,
};

#undef GALE_TALK_CUE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
