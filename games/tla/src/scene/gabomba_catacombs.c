/* The Gabomba Catacombs, the caves below the Gabomba Statue, which Felix
   reaches by the ladder down from inside the statue once Piers can cast Frost.
   Each cave is a room of this overlay.

   The ladder comes down at the north-east of the first cave, where three stones
   lie hidden under the weeds. Cyclone on one uncovers it, and a whirlwind
   carries Felix down into the cave below, each stone to its own spot. There the
   Venus Djinni Mud waits in plain sight and fights
   (scGabombaCatacombsFightMud); stairs lead back up to the first cave, and a
   way on leads into the cave of the hidden wall. In that cave Cyclone on the
   big patch of weeds in the east uncovers a Mint. In the west it scatters a
   cross of five weeds from a puddle, and Frost freezes the puddle into a pillar
   Felix hops across to a dead end ringed by six more weeds over puddles; Reveal
   there shows the passage on to the stairs down. In the stream cave below, the
   shallow water pushes Felix south as he wades, and Cyclone and Frost make a
   second pillar on the way to the stairs on the rock in the middle. They climb
   to the last cave, where Cyclone on the weeds wakes the first Mad Plant, and
   where the Great Gabomba, speaking through the idol at the top, gives the
   party the Tomegathericon (scGabombaCatacombsGabombaBestowsPower). */

#include "scene/script_layer.h"
#include "field/field_mode_script.h"
#include "field/field_mode.h"
#include "graphics/palette_buffers.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "game/game.h"
#include "edition.h"
#include "field/actor_script.h"

/* Set once the Great Gabomba has given the party its power, the Tomegathericon
   (scGabombaCatacombsGabombaBestowsPower). */
#define FLAG_GABOMBA_BESTOWED_POWER 0x9ce

extern Uint32 GabombaCatacombsSplashScript[];

/* A Mad Plant of the given sprite leaps out at Felix from (x, z) and
   attacks. */
static __inline__ void scGabombaCatacombsMadPlantLeaps(
	Sint32 sprite, Sint32 x, Sint32 z)
{
	fdMadPlantLeapAtLeader(sprite, x, z);
}

extern Uint32		FieldEffectMediumScript[];
extern SCENE_REGION GabombaCatacombsTeleportersExitRegions[];
extern ACTOR_PLACE	GabombaCatacombsPlacements[];
extern ACTOR_PLACE	GabombaCatacombsTeleportersActorPlacements[];
extern ACTOR_PLACE	GabombaCatacombsMudActorPlacements[];
extern ACTOR_PLACE	GabombaCatacombsHiddenWallActorPlacements[];
extern ACTOR_PLACE	GabombaCatacombsTomegathericonActorPlacements[];
extern ACTOR_PLACE	GabombaCatacombsStreamActorPlacements[];
extern CUE			GabombaCatacombsTeleportersCues[];
extern CUE			GabombaCatacombsMudCues[];
extern CUE			GabombaCatacombsHiddenWallCues[];
extern CUE			GabombaCatacombsTomegathericonCues[];
extern CUE			GabombaCatacombsStreamCues[];

#include "text/msg_ids.h"

/* Whether the leader stands on the metatile (metatileX, metatileZ). */
#define LEADER_ON_METATILE(leader, metatileX, metatileZ) \
	(FIELD_METATILE((leader)->x.fixed) == (metatileX) && \
		FIELD_METATILE((leader)->z.fixed) == (metatileZ))

/* Takes control from the player and starts Felix's hop off a square. */
#define LEADER_HOP() \
	(fdScriptStart(), fdScriptEndLastingPsynergy(0), \
		fdEffectSetAttached(MainRoutineStateWork.leaderActorId, \
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE))

/* Mud, the Djinni waiting in the cave below the teleport stones. */
enum { ACTOR_MUD = ACTOR_FIRST_PLACED };

/* The hidden wall's cave's seven puddles under the weeds Cyclone scatters (the
   first under the cross of five weeds in the west, the rest in the ring of six
   at the dead end below the hidden wall), the stream cave's puddle, and the
   last cave's seated idol the Great Gabomba speaks through, and Kraden. */
enum {
	ACTOR_FIRST_PUDDLE = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_PUDDLE,
	ACTOR_THIRD_PUDDLE,
	ACTOR_FOURTH_PUDDLE,
	ACTOR_FIFTH_PUDDLE,
	ACTOR_SIXTH_PUDDLE,
	ACTOR_SEVENTH_PUDDLE
};

/* The stream cave's puddle. */
enum { ACTOR_STREAM_PUDDLE = ACTOR_FIRST_PLACED };

/* The last cave's seated idol, and Kraden. */
enum { ACTOR_SEATED_IDOL = ACTOR_FIRST_PLACED, ACTOR_KRADEN };

/* The seated idol's own animations (SPRITE_SEATED_IDOL): its head glowing and
   pulsing as it speaks, and the glow dying away. */
enum { ANIM_SEATED_IDOL_GLOW = 2, ANIM_SEATED_IDOL_GLOW_FADE = 4 };

/* Each puddle Frost has frozen into a pillar, by the puddles above. */
#define FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN	 FLAG_SCENE_TEMP_00
#define FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN	 FLAG_SCENE_TEMP_01
#define FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN	 FLAG_SCENE_TEMP_02
#define FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN	 FLAG_SCENE_TEMP_03
#define FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN	 FLAG_SCENE_TEMP_04
#define FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN	 FLAG_SCENE_TEMP_05
#define FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_06
#define FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN	 FLAG_SCENE_TEMP_00

/* The map metatiles the caves' cue rows answer, by what lies on them. */
typedef enum GABOMBA_CATACOMBS_METATILE_BEHAVIOR {
	/* The first cave: its weeds, and the three teleport stones hidden in its
	   west, east and south patches of weeds. */
	METATILE_BEHAVIOR_TELEPORTERS_WEEDS = 41,
	METATILE_BEHAVIOR_WEST_TELEPORT_STONE = 43,
	METATILE_BEHAVIOR_EAST_TELEPORT_STONE = 44,
	METATILE_BEHAVIOR_SOUTH_TELEPORT_STONE = 45,
	/* The hidden wall's cave: the big patch of weeds in the east and the square
	   in it where the Mint lies, the cross of five weeds in the west and the
	   puddle under it, and the six puddles of the ring at the dead end. */
	METATILE_BEHAVIOR_MINT_PATCH_WEEDS = 40,
	METATILE_BEHAVIOR_CROSS_WEEDS = 41,
	METATILE_BEHAVIOR_FIRST_PUDDLE = 42,
	METATILE_BEHAVIOR_SECOND_PUDDLE,
	METATILE_BEHAVIOR_THIRD_PUDDLE,
	METATILE_BEHAVIOR_FOURTH_PUDDLE,
	METATILE_BEHAVIOR_FIFTH_PUDDLE,
	METATILE_BEHAVIOR_SIXTH_PUDDLE,
	METATILE_BEHAVIOR_SEVENTH_PUDDLE,
	METATILE_BEHAVIOR_MINT = 49,
	/* The stream cave: the weeds by the stairs up, the column of weeds over the
	   puddle in the west and the puddle itself, the weeds in the north, and the
	   water of the stream. */
	METATILE_BEHAVIOR_STAIRS_WEEDS = 40,
	METATILE_BEHAVIOR_PUDDLE_WEEDS = 41,
	METATILE_BEHAVIOR_STREAM_PUDDLE = 42,
	METATILE_BEHAVIOR_NORTH_WEEDS = 43,
	METATILE_BEHAVIOR_STREAM = 50,
	/* The last cave: its clump of weeds and the square in it where the Mad
	   Plant hides, and the square in front of the idol. */
	METATILE_BEHAVIOR_MAD_PLANT_WEEDS = 30,
	METATILE_BEHAVIOR_MAD_PLANT = 31,
	METATILE_BEHAVIOR_BEFORE_THE_IDOL = 40
} GABOMBA_CATACOMBS_METATILE_BEHAVIOR;

/* Talking to Mud, the Venus Djinni waiting in plain sight in the cave below the
   teleport stones, starts its battle. */
void scGabombaCatacombsFightMud(void)
{
	fdRequestDjinnBattle(ACTOR_MUD, DJINNI_MUD);
}

/* Cyclone scatters the weeds of the first cave, below the ladder. */
void scGabombaCatacombsScatterTeleportersWeeds(void)
{
	syFieldCycloneCopyTouchedMetatiles(7, 63, 1, 16, 16, 57, 63);
}

/* Cyclone scatters the big patch of weeds in the east of the hidden wall's
   cave, around metatile (24, 17). */
void scGabombaCatacombsScatterHiddenWallWeeds(void)
{
	syFieldCycloneCopyTouchedMetatiles(22, 15, 0, 5, 5, 63, 52);
}

/* Cyclone scatters the cross of five weeds in the west of the hidden wall's
   cave, around metatile (8, 23). */
void scGabombaCatacombsScatterHiddenWallSmallWeeds(void)
{
	syFieldCycloneCopyTouchedMetatiles(7, 22, 0, 3, 3, 56, 58);
}

/* Cyclone scatters the weeds by the stairs up from the stream cave, around
   metatile (46, 26). */
void scGabombaCatacombsScatterStreamWeeds(void)
{
	syFieldCycloneCopyTouchedMetatiles(44, 25, 0, 4, 3, 66, 57);
}

/* Cyclone scatters the column of weeds over the puddle in the west of the
   stream cave, metatiles (16, 14) to (19, 20). */
void scGabombaCatacombsScatterStreamWideWeeds(void)
{
	syFieldCycloneCopyTouchedMetatiles(16, 13, 0, 4, 8, 61, 52);
}

/* Cyclone scatters the weeds in the north of the stream cave, at metatile (22,
   10). */
void scGabombaCatacombsScatterStreamSmallWeeds(void)
{
	syFieldCycloneCopyTouchedMetatiles(21, 9, 0, 2, 2, 57, 52);
}

/* Cyclone on the big patch of weeds in the east of the hidden wall's cave
   uncovers the Mint hidden at metatile (24, 21), in the middle of the patch's
   south edge. Until the Mint is taken, its treasure is put on that square, and
   if Felix stands there he hops a square east off it. */
void scGabombaCatacombsUncoverMint(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(22, 15, 0, 5, 5, 63, 52);
	if(syGameFlagGet(FLAG_GABOMBA_CATACOMBS_MINT_TAKEN) == 0) {
		fdActorSetPos(FIELD_TREASURE_ACTOR, 392, 344);
		if(LEADER_ON_METATILE(leader, 24, 21)) {
			LEADER_HOP();
			fdActorSetDestinationOffsetById(
				MainRoutineStateWork.leaderActorId, 16, 0);
			leader->velocityY = toFIXED(6);
			fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdScriptEnd();
		}
	}
}

/* Cyclone on the patch of weeds in the west of the first cave uncovers the
   teleport stone at metatile (10, 15), and a whirlwind carries Felix down into
   the cave below, west of Mud. */
void scGabombaCatacombsUncoverWestTeleportStone(void)
{
	fdMapGridCopyMetatilesLayered(51, 83, 3, 1, 9, 65);
	syImportScriptSetExitRequest(
		GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_WEST);
}

/* Cyclone on the patch of weeds in the east of the first cave uncovers the
   teleport stone at metatile (19, 16), and a whirlwind carries Felix down into
   the cave below, to its east. */
void scGabombaCatacombsUncoverEastTeleportStone(void)
{
	fdMapGridCopyMetatilesLayered(51, 83, 3, 1, 18, 66);
	syImportScriptSetExitRequest(
		GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_EAST);
}

/* Cyclone on the teleport stone in the patch of weeds in the south of the first
   cave, metatile (13, 24), whirls Felix down into the cave below, to its south.
   Unlike the other two stones, this one is never hidden: it lies bare among the
   weeds from the start, so no picture is redrawn here. The cast scatters
   leaves, the whirlwind carries Felix onto the stone and spins him down through
   it, and he lands on the path above the ladder in the cave below; the weeds
   around the stone stay. */
void scGabombaCatacombsTakeSouthTeleportStone(void)
{
	syImportScriptSetExitRequest(
		GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_SOUTH);
}

/* Cyclone on the middle of the cross of five weeds in the west of the hidden
   wall's cave, metatile (8, 23), shows the puddle under it. If Felix stands on
   the puddle's square, he hops a square east off it. */
void scGabombaCatacombsUncoverFirstPuddle(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(7, 22, 0, 3, 3, 56, 58);
	fdActorSetPos(ACTOR_FIRST_PUDDLE, 136, 376);
	if(LEADER_ON_METATILE(leader, 8, 23)) {
		LEADER_HOP();
		fdActorSetDestinationOffsetById(
			MainRoutineStateWork.leaderActorId, 16, 0);
		leader->velocityY = toFIXED(6);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdScriptEnd();
	}
}

/* Cyclone on the weeds in the north-east of the ring of six weeds at the dead
   end below the hidden wall, metatile (8, 15), shows the puddle under them. If
   Felix stands on the puddle's square, he hops a square east off it. */
void scGabombaCatacombsUncoverSecondPuddle(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(5, 13, 0, 5, 3, 56, 52);
	fdActorSetPos(ACTOR_SECOND_PUDDLE, 136, 248);
	if(LEADER_ON_METATILE(leader, 8, 15)) {
		LEADER_HOP();
		fdActorSetDestinationOffsetById(
			MainRoutineStateWork.leaderActorId, 16, 0);
		leader->velocityY = toFIXED(6);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdScriptEnd();
	}
}

/* Cyclone on the weeds in the east of the ring of six weeds at the dead end
   below the hidden wall, metatile (9, 16), shows the puddle under them. If
   Felix stands on the puddle's square, he hops a square west off it. */
void scGabombaCatacombsUncoverThirdPuddle(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(5, 13, 0, 5, 3, 56, 52);
	fdActorSetPos(ACTOR_THIRD_PUDDLE, 152, 264);
	if(LEADER_ON_METATILE(leader, 9, 16)) {
		LEADER_HOP();
		fdActorSetDestinationOffsetById(
			MainRoutineStateWork.leaderActorId, -16, 0);
		leader->velocityY = toFIXED(6);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdScriptEnd();
	}
}

/* Cyclone on the weeds in the south-east of the ring of six weeds at the dead
   end below the hidden wall, metatile (8, 17), shows the puddle under them. If
   Felix stands on the puddle's square, he hops a square east off it. */
void scGabombaCatacombsUncoverFourthPuddle(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(5, 13, 0, 5, 3, 56, 52);
	fdActorSetPos(ACTOR_FOURTH_PUDDLE, 136, 280);
	if(LEADER_ON_METATILE(leader, 8, 17)) {
		LEADER_HOP();
		fdActorSetDestinationOffsetById(
			MainRoutineStateWork.leaderActorId, 16, 0);
		leader->velocityY = toFIXED(6);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdScriptEnd();
	}
}

/* Cyclone on the weeds in the south-west of the ring of six weeds at the dead
   end below the hidden wall, metatile (6, 17), shows the puddle under them. If
   Felix stands on the puddle's square, he hops a square east off it. */
void scGabombaCatacombsUncoverFifthPuddle(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(5, 13, 0, 5, 3, 56, 52);
	fdActorSetPos(ACTOR_FIFTH_PUDDLE, 104, 280);
	if(LEADER_ON_METATILE(leader, 6, 17)) {
		LEADER_HOP();
		fdActorSetDestinationOffsetById(
			MainRoutineStateWork.leaderActorId, 16, 0);
		leader->velocityY = toFIXED(6);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdScriptEnd();
	}
}

/* Cyclone on the weeds in the west of the ring of six weeds at the dead end
   below the hidden wall, metatile (5, 16), shows the puddle under them. If
   Felix stands on the puddle's square, he hops a square east off it. */
void scGabombaCatacombsUncoverSixthPuddle(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(5, 13, 0, 5, 3, 56, 52);
	fdActorSetPos(ACTOR_SIXTH_PUDDLE, 88, 264);
	if(LEADER_ON_METATILE(leader, 5, 16)) {
		LEADER_HOP();
		fdActorSetDestinationOffsetById(
			MainRoutineStateWork.leaderActorId, 16, 0);
		leader->velocityY = toFIXED(6);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdScriptEnd();
	}
}

/* Cyclone on the weeds in the north-west of the ring of six weeds at the dead
   end below the hidden wall, metatile (6, 15), shows the puddle under them. If
   Felix stands on the puddle's square, he hops a square east off it. */
void scGabombaCatacombsUncoverSeventhPuddle(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(5, 13, 0, 5, 3, 56, 52);
	fdActorSetPos(ACTOR_SEVENTH_PUDDLE, 104, 248);
	if(LEADER_ON_METATILE(leader, 6, 15)) {
		LEADER_HOP();
		fdActorSetDestinationOffsetById(
			MainRoutineStateWork.leaderActorId, 16, 0);
		leader->velocityY = toFIXED(6);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdScriptEnd();
	}
}

/* Cyclone on the column of weeds in the west of the stream cave shows the
   puddle under them at metatile (17, 18). If Felix stands on the puddle's
   square, he hops a square north off it. */
void scGabombaCatacombsUncoverStreamPuddle(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldCycloneCopyTouchedMetatiles(16, 13, 0, 4, 8, 61, 52);
	fdActorSetPos(ACTOR_STREAM_PUDDLE, 280, 296);
	if(LEADER_ON_METATILE(leader, 17, 18)) {
		LEADER_HOP();
		fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, -16);
		leader->velocityY = toFIXED(6);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdScriptEnd();
	}
}

/* Cyclone scatters the clump of weeds in the south-west of the last cave. */
void scGabombaCatacombsScatterTomegathericonWeeds(void)
{
	syFieldCycloneCopyTouchedMetatiles(7, 23, 0, 4, 5, 60, 56);
}

/* Cyclone on the clump of weeds in the south-west of the last cave, at metatile
   (10, 26), wakes the first Mad Plant, which leaps out at Felix and fights; the
   party cannot run. Once it is beaten it does not come back. */
void scGabombaCatacombsUncoverMadPlant(void)
{
	syFieldCycloneCopyTouchedMetatiles(7, 23, 0, 4, 5, 60, 56);
	if(syGameFlagGet(FLAG_MAD_PLANT(0)) == 0) {
		scGabombaCatacombsMadPlantLeaps(SPRITE_MAD_PLANT, 168, 424);
		MainRoutineStateWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(0));
		SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_MAD_PLANTS, 0);
	}
}

/* Reveal at the dead end below the hidden wall shows a passage through the wall
   north of the ring of weeds, metatiles (7, 11) to (7, 14), on to the stairs
   down to the stream cave. */
void scGabombaCatacombsRevealWall(void)
{
	fdMapGridCopyMetatileTilesMetatilesRect(0, 0, 1, 2, 7, 11);
	fdMapCopyMetatileAttributeRect(2, 4, 1, 1, 7, 14);
}

/* As Reveal wears off, the wall north of the ring of weeds closes again. */
void scGabombaCatacombsHideWall(void)
{
	fdMapGridCopyMetatileTilesMetatilesRect(1, 0, 1, 2, 7, 11);
	fdMapCopyMetatileAttributeRect(2, 5, 1, 1, 7, 14);
}

/* Frost on the puddle in the cross of weeds in the west of the hidden wall's
   cave, metatile (8, 23), freezes it into an ice pillar. Its square becomes one
   Felix hops onto, like the squares on either side, so he can hop across to the
   dead end below the hidden wall. */
void scGabombaCatacombsFreezeFirstPuddle(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_FIRST_PUDDLE);

	syGameFlagSet(FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN);
	actor->motionFlags = 0;
	actor->priorityFlags = 0;
	fdMapGridCopyMetatileAttributes(6, 23, 1, 1, 8, 23);
}

/* Frost on the puddle in the north-east of the ring of six weeds at the dead
   end below the hidden wall, metatile (8, 15), freezes it into an ice pillar.
   The ring's pillars, unlike the cross's, do not become squares to hop onto:
   each takes the attributes of metatile (0, 4), plain ground 48 pixels up, 16
   above the ring's floor. The ring's six metatiles, (6, 15), (8, 15), (9, 16),
   (8, 17), (6, 17) and (5, 16), lie on that floor and not over a gap, so its
   pillars bridge nothing. */
void scGabombaCatacombsFreezeSecondPuddle(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_SECOND_PUDDLE);

	syGameFlagSet(FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN);
	actor->motionFlags = 0;
	actor->priorityFlags = 0;
	fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 8, 15);
}

/* Frost on the puddle in the east of the ring of six weeds at the dead end
   below the hidden wall, metatile (9, 16), freezes it into an ice pillar. */
void scGabombaCatacombsFreezeThirdPuddle(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_THIRD_PUDDLE);

	syGameFlagSet(FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN);
	actor->motionFlags = 0;
	actor->priorityFlags = 0;
	fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 9, 16);
}

/* Frost on the puddle in the south-east of the ring of six weeds at the dead
   end below the hidden wall, metatile (8, 17), freezes it into an ice
   pillar. */
void scGabombaCatacombsFreezeFourthPuddle(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_FOURTH_PUDDLE);

	syGameFlagSet(FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN);
	actor->motionFlags = 0;
	actor->priorityFlags = 0;
	fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 8, 17);
}

/* Frost on the puddle in the south-west of the ring of six weeds at the dead
   end below the hidden wall, metatile (6, 17), freezes it into an ice
   pillar. */
void scGabombaCatacombsFreezeFifthPuddle(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_FIFTH_PUDDLE);

	syGameFlagSet(FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN);
	actor->motionFlags = 0;
	actor->priorityFlags = 0;
	fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 6, 17);
}

/* Frost on the puddle in the west of the ring of six weeds at the dead end
   below the hidden wall, metatile (5, 16), freezes it into an ice pillar. */
void scGabombaCatacombsFreezeSixthPuddle(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_SIXTH_PUDDLE);

	syGameFlagSet(FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN);
	actor->motionFlags = 0;
	actor->priorityFlags = 0;
	fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 5, 16);
}

/* Frost on the puddle in the north-west of the ring of six weeds at the dead
   end below the hidden wall, metatile (6, 15), freezes it into an ice
   pillar. */
void scGabombaCatacombsFreezeSeventhPuddle(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_SEVENTH_PUDDLE);

	syGameFlagSet(FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN);
	actor->motionFlags = 0;
	actor->priorityFlags = 0;
	fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 6, 15);
}

/* Frost on the puddle in the west of the stream cave, metatile (17, 18),
   freezes it into an ice pillar. Its square becomes one Felix hops onto, like
   the squares on either side, on the way to the stairs on the rock in the
   middle. */
void scGabombaCatacombsFreezeStreamPuddle(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_STREAM_PUDDLE);

	syGameFlagSet(FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN);
	actor->motionFlags = 0;
	actor->priorityFlags = 0;
	fdMapGridCopyMetatileAttributes(15, 18, 1, 1, 17, 18);
}

/* The stream carries Felix, and the view with him, dz further south, unless a
   wall or post lies just south of him or diagonally ahead. */
void scGabombaCatacombsDriftWithStream(ACTOR *actor, Sint32 dz)
{
	ACTOR *camera;
	POS	   pos;

	camera = FieldStateWork->camera;

	pos.x = actor->x.fixed;
	pos.y = actor->y.fixed;
	pos.z = actor->z.fixed + dz;
	fdMapGridCheckCollision(actor, &pos);
	pos.x = actor->x.fixed;
	pos.y = actor->y.fixed;
	pos.z = actor->z.fixed + toFIXED(8);
	if(fdMapGridCheckCollision(actor, &pos) <= 0) {
		pos.x = actor->x.fixed + toFIXED(57) / 10;
		pos.y = actor->y.fixed;
		pos.z = actor->z.fixed + toFIXED(57) / 10;
		if(fdMapGridCheckCollision(actor, &pos) <= 0) {
			pos.x = actor->x.fixed - toFIXED(57) / 10;
			pos.y = actor->y.fixed;
			pos.z = actor->z.fixed + toFIXED(57) / 10;
			if(fdMapGridCheckCollision(actor, &pos) <= 0) {
				camera->z.fixed += dz;
				actor->z.fixed += dz;
			}
		}
	}
}

/* Wading in the shallow stream that runs south through the stream cave, Felix
   splashes, with the sound of wading, every eighth tick, and the water pushes
   him slowly south. */
void scGabombaCatacombsWadeStream(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *splash;

	if((TickCount & 7) == 0) {
		splash = ((ACTOR *)syImportActorCreate(SPRITE_WADING_SPLASH,
			leader->x.fixed, leader->y.fixed + toFIXED(6), leader->z.fixed));
		if(splash) {
			fdActorSetSpriteFlags(splash, 0);
			fdActorSelectAnimation(splash, 1);
			fdActorStartScript(splash, GabombaCatacombsSplashScript);
		}
		sySoundPlay(SE_WADE);
	}
	scGabombaCatacombsDriftWithStream(leader, toFIXED(17) / 20);
}

/* The flash of light about the Tomegathericon as it appears on the idol's lap:
   each tick it shrinks by a twentieth of its full size. */
Sint32 scGabombaCatacombsShrinkGlow(ACTOR *actor)
{
	actor->scaleX -= toFIXED(1) / 20;
	actor->scaleY -= toFIXED(1) / 20;
	return 0;
}

/* Shows the conversation's next line, centred. */
#define TALK() \
	syScriptMessageShowCentered( \
		(Sint16)((SCRIPT_LINE *)&state->msg)->line++, 0, -3)

/* When Felix steps in front of the idol at the top of the last cave, between
   the two candlesticks, the cave darkens and the Great Gabomba speaks through
   the idol: "By reaching this place, you have proven your power." The
   Tomegathericon appears in a flash on the idol's lap. Kraden steps out beside
   Felix and worries that they have wronged Akafubu, and the Gabomba gives the
   power on the condition that they never tell Akafubu. Kraden agrees, the
   Gabomba goes back to sleep, and the light returns with the book still on the
   idol's lap. */
void scGabombaCatacombsGabombaBestowsPower(void)
{
	ACTOR		*kraden = fdActorGetById(ACTOR_KRADEN);
	ACTOR		*treasure = fdActorGetById(FIELD_TREASURE_ACTOR);
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*glow, *leader;
	Sint32		 speed;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(8, 8, 12), 1);
	fdPaletteFadeStartInterpolation(60);
	fdCameraMoveTo(168, CAMERA_KEEP, 158, 1);
	sySoundPlay(MUS_JUPITER_LIGHTHOUSE);
	fdScriptWait(60);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, ANIM_SEATED_IDOL_GLOW);
	sySoundPlay(SE_STATUE_STIRS);
	fdScriptWait(60);
	fdScriptSetMessageId((Sint32)MsgByReachingThis);
	TALK();
	fdActorSetPos(FIELD_TREASURE_ACTOR, 168, 152);
	treasure->motionFlags = ACTOR_FALLS | ACTOR_FLOATS;
	treasure->groundHeight += toFIXED(16);
	sySoundPlay(SE_SHIMMER);
	glow = ((ACTOR *)syImportActorCreate(SPRITE_FIREBALL, 0, 0, 0));
	fdActorStartScript(glow, FieldEffectMediumScript);
	fdActorSetSpriteColorMode(glow, 1);
	glow->motionFlags = 0;
	glow->x.fixed = treasure->x.fixed;
	glow->y.fixed = treasure->y.fixed + toFIXED(8);
	glow->z.fixed = treasure->z.fixed;
	glow->groundHeight = treasure->groundHeight;
	glow->update = scGabombaCatacombsShrinkGlow;
	fdScriptWait(15);
	glow->x.fixed = 0;
	glow->y.fixed = 0;
	glow->z.fixed = 0;
	glow->update = 0;
	TALK();
	fdActorSetDestinationById(MainRoutineStateWork.leaderActorId, 166, 188);
	fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	speed = toFIXED(1);
	kraden->speedLimit = speed;
	fdActorSetDestinationById(ACTOR_KRADEN, 184, 188);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	/* "I am grateful for your promise...But I am worried that we have
	   done..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	TALK();
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(30);
	/* "Then what are we to do? Does this mean we won't receive your magic?" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	TALK();
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdScriptWait(30);
	TALK();
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	fdScriptWait(30);
	/* "Wh-What kind of conditions?" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	TALK();
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(10);
	fdActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	TALK();
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdScriptWait(60);
	TALK();
	fdScriptWait(40);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdScriptWait(40);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	/* "Of course!" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, ANIM_SEATED_IDOL_GLOW);
	TALK();
	fdScriptWait(30);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, ANIM_SEATED_IDOL_GLOW_FADE);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(
			ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	syGameFlagSet(FLAG_GABOMBA_BESTOWED_POWER);
	/* The cave's music and its light come back. */
	gmPlayMusic();
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(60);
	fdCameraMoveToActorById(MainRoutineStateWork.leaderActorId, 1);
	fdScriptWait(60);
	fdScriptEnd();
}

/* Talking to the idol at the top of the last cave, once the Tomegathericon lies
   on its lap, takes the book. */
void scGabombaCatacombsTalkToStatue(void)
{
	ACTOR *actor = fdActorGetById(FIELD_TREASURE_ACTOR);

	if(actor != NULL && actor->x.fixed != 0)
		fdTalkToActor(FIELD_TALK_SILENT | FIELD_TREASURE_ACTOR);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
		return GabombaCatacombsTeleportersExitRegions;
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern CUE GabombaCatacombsCues[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
		return GabombaCatacombsTeleportersActorPlacements;
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_MUD)
		return GabombaCatacombsMudActorPlacements;
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL)
		return GabombaCatacombsHiddenWallActorPlacements;
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON)
		return GabombaCatacombsTomegathericonActorPlacements;
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_STREAM)
		return GabombaCatacombsStreamActorPlacements;
	return GabombaCatacombsPlacements;
}

const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
		return GabombaCatacombsTeleportersCues;
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_MUD)
		return GabombaCatacombsMudCues;
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL)
		return GabombaCatacombsHiddenWallCues;
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON)
		return GabombaCatacombsTomegathericonCues;
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_STREAM)
		return GabombaCatacombsStreamCues;
	return GabombaCatacombsCues;
}

/* The transition the catacombs open with: the window's shape in the Japanese
   edition, an interlaced wipe from it. */
#if !REVISED_ENTRANCE_TRANSITIONS
#define CATACOMBS_SCREEN_EFFECT \
	SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#else
#define CATACOMBS_SCREEN_EFFECT \
	SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE)
#endif

extern char SceneIdGabombaStatuePinion, SceneIdGabombaStatueChamberIdol;

/* Sets up the room the party comes into: coming down the ladder into the first
   cave, where a retreat leads back to in the statue above; landing from a
   teleport stone beside Mud, in a ring of light; the puddles frozen into
   pillars on this visit; and the Tomegathericon on the idol's lap until it is
   taken. */
Sint32 scInit(void)
{
	Sint32 entrance;
	ACTOR *treasure;

	FieldStateWork->transition = CATACOMBS_SCREEN_EFFECT;
	fdEffectStartBlankOverlay(0);
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS &&
		(entrance = MainRoutineStateWork.entrance) ==
			GABOMBA_CATACOMBS_TELEPORTERS_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR) {
		if(syGameFlagGet(FLAG_GABOMBA_RETREAT_TO_PINION)) {
			MainRoutineStateWork.retreatScene = SCENE_GABOMBA_STATUE_PINION;
			MainRoutineStateWork.retreatEntrance = entrance;
		} else if(syGameFlagGet(FLAG_GABOMBA_RETREAT_TO_IDOL)) {
			MainRoutineStateWork.retreatScene =
				SCENE_GABOMBA_STATUE_CHAMBER_IDOL;
			MainRoutineStateWork.retreatEntrance =
				GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS;
		}
	}
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION) &&
		MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_MUD &&
		MainRoutineStateWork.entrance >
			GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_HIDDEN_WALL)
		fdActorDepartInRing(MainRoutineStateWork.leaderActorId);
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL) {
		if(syGameFlagGet(FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN))
			fdMapGridCopyMetatileAttributes(6, 23, 1, 1, 8, 23);
		if(syGameFlagGet(FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN))
			fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 8, 15);
		if(syGameFlagGet(FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN))
			fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 9, 16);
		if(syGameFlagGet(FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN))
			fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 8, 17);
		if(syGameFlagGet(FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN))
			fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 6, 17);
		if(syGameFlagGet(FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN))
			fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 5, 16);
		if(syGameFlagGet(FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN))
			fdMapGridCopyMetatileAttributes(0, 4, 1, 1, 6, 15);
	}
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_STREAM &&
		syGameFlagGet(FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN))
		fdMapGridCopyMetatileAttributes(15, 18, 1, 1, 17, 18);
	if(MainRoutineStateWork.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON &&
		syGameFlagGet(FLAG_GABOMBA_BESTOWED_POWER) &&
		!syGameFlagGet(FLAG_TOMEGATHERICON_TAKEN) &&
		!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		treasure = fdActorGetById(FIELD_TREASURE_ACTOR);
		fdActorSetPos(FIELD_TREASURE_ACTOR, 168, 152);
		treasure->motionFlags = ACTOR_FALLS | ACTOR_FLOATS;
		treasure->groundHeight += toFIXED(16);
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* clang-format off */
Uint32 GabombaCatacombsSplashScript[] = {
	SC_WAIT(15),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};

Uint32 FieldEffectMediumScript[] = {
	SC_WAIT(1), SC_LOOP(44, SC_TOP),
	SC_WAIT(1), SC_POSITION(0, 0, 0),
	SC_WAIT(1), SC_RELEASE,
	SC_END,
};

#include "shared/scene_entrances_default.c"

SCENE_REGION GabombaCatacombsTeleportersExitRegions[] = {
	{388, 46, 276, 396, 54, 284, CONDITION_ALWAYS, 1},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(GabombaCatacombsTeleporters),
	SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR, GabombaStatueChamberWitchDoctor, GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_FROM_GABOMBA_CATACOMBS_TELEPORTERS),
	SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_NORTHEAST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST),
	SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_WEST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_WEST),
	SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_EAST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_EAST),
	SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_SOUTH, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_SOUTH),
	SCENE_EXITS(GabombaCatacombsMud),
	SCENE_EXIT(GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST, GabombaCatacombsTeleporters, GABOMBA_CATACOMBS_TELEPORTERS_FROM_GABOMBA_CATACOMBS_MUD_NORTHEAST),
	SCENE_EXIT(GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_HIDDEN_WALL, GabombaCatacombsHiddenWall, GABOMBA_CATACOMBS_HIDDEN_WALL_FROM_GABOMBA_CATACOMBS_MUD),
	SCENE_EXITS(GabombaCatacombsHiddenWall),
	SCENE_EXIT(GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_MUD, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_HIDDEN_WALL),
	SCENE_EXIT(GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_STREAM, GabombaCatacombsStream, GABOMBA_CATACOMBS_STREAM_FROM_GABOMBA_CATACOMBS_HIDDEN_WALL),
	SCENE_EXITS(GabombaCatacombsTomegathericon),
	SCENE_EXIT(GABOMBA_CATACOMBS_TOMEGATHERICON_TO_GABOMBA_CATACOMBS_STREAM, GabombaCatacombsStream, GABOMBA_CATACOMBS_STREAM_FROM_GABOMBA_CATACOMBS_TOMEGATHERICON),
	SCENE_EXITS(GabombaCatacombsStream),
	SCENE_EXIT(GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_HIDDEN_WALL, GabombaCatacombsHiddenWall, GABOMBA_CATACOMBS_HIDDEN_WALL_FROM_GABOMBA_CATACOMBS_STREAM),
	SCENE_EXIT(GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_TOMEGATHERICON, GabombaCatacombsTomegathericon, GABOMBA_CATACOMBS_TOMEGATHERICON_FROM_GABOMBA_CATACOMBS_STREAM),
	SCENE_EXITS_END,
};

ACTOR_PLACE GabombaCatacombsPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE GabombaCatacombsTeleportersActorPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE GabombaCatacombsMudActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MUD), MOVEMENT_TYPE_STAND, 728, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GabombaCatacombsHiddenWallActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GabombaCatacombsTomegathericonActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SEATED_IDOL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GabombaCatacombsStreamActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE GabombaCatacombsCues[] = {
	{CUE_TABLE_END},
};

CUE GabombaCatacombsTeleportersCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
		GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_NORTHEAST,
		CONDITION_ALWAYS,
		GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_NORTHEAST},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_TELEPORTERS_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsScatterTeleportersWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_WEST_TELEPORT_STONE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverWestTeleportStone)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_EAST_TELEPORT_STONE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverEastTeleportStone)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_SOUTH_TELEPORT_STONE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsTakeSouthTeleportStone)},
	{CUE_TABLE_END},
};

CUE GabombaCatacombsMudCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP,
		GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST,
		CONDITION_ALWAYS,
		GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST},
	{CUE_EXIT, GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_HIDDEN_WALL,
		CONDITION_ALWAYS,
		GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_HIDDEN_WALL},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_MUD, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsFightMud)},
	{CUE_TABLE_END},
};

CUE GabombaCatacombsHiddenWallCues[] = {
	{CUE_EXIT, GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_MUD,
		CONDITION_ALWAYS,
		GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_MUD},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
		GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_STREAM,
		CONDITION_ALWAYS,
		GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_STREAM},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scGabombaCatacombsRevealWall)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scGabombaCatacombsHideWall)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_MINT_PATCH_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsScatterHiddenWallWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_CROSS_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsScatterHiddenWallSmallWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_FIRST_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverFirstPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_SECOND_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverSecondPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_THIRD_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverThirdPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_FOURTH_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverFourthPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_FIFTH_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverFifthPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_SIXTH_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverSixthPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_SEVENTH_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverSeventhPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_MINT, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverMint)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
		FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN,
		SCRIPT_REF(scGabombaCatacombsFreezeFirstPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
		FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN,
		SCRIPT_REF(scGabombaCatacombsFreezeSecondPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
		FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN,
		SCRIPT_REF(scGabombaCatacombsFreezeThirdPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FOURTH_PUDDLE,
		FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN,
		SCRIPT_REF(scGabombaCatacombsFreezeFourthPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIFTH_PUDDLE,
		FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN,
		SCRIPT_REF(scGabombaCatacombsFreezeFifthPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIXTH_PUDDLE,
		FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN,
		SCRIPT_REF(scGabombaCatacombsFreezeSixthPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVENTH_PUDDLE,
		FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN,
		SCRIPT_REF(scGabombaCatacombsFreezeSeventhPuddle)},
	{CUE_TABLE_END},
};

CUE GabombaCatacombsTomegathericonCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
		GABOMBA_CATACOMBS_TOMEGATHERICON_TO_GABOMBA_CATACOMBS_STREAM,
		CONDITION_ALWAYS,
		GABOMBA_CATACOMBS_TOMEGATHERICON_TO_GABOMBA_CATACOMBS_STREAM},
	{CUE_STEP, METATILE_BEHAVIOR_BEFORE_THE_IDOL, FLAG_GABOMBA_BESTOWED_POWER,
		SCRIPT_REF(scGabombaCatacombsGabombaBestowsPower)},
	{CUE_TALK, ACTOR_SEATED_IDOL, FLAG_TOMEGATHERICON_TAKEN,
		SCRIPT_REF(scGabombaCatacombsTalkToStatue)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_MAD_PLANT_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsScatterTomegathericonWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_MAD_PLANT, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverMadPlant)},
	{CUE_TABLE_END},
};

CUE GabombaCatacombsStreamCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP,
		GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_HIDDEN_WALL,
		CONDITION_ALWAYS,
		GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_HIDDEN_WALL},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP,
		GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_TOMEGATHERICON,
		CONDITION_ALWAYS,
		GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_TOMEGATHERICON},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_STAIRS_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsScatterStreamWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_PUDDLE_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsScatterStreamWideWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_NORTH_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsScatterStreamSmallWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_STREAM_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsUncoverStreamPuddle)},
	{CUE_STEP, METATILE_BEHAVIOR_STREAM, CONDITION_ALWAYS,
		SCRIPT_REF(scGabombaCatacombsWadeStream)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_STREAM_PUDDLE,
		FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN,
		SCRIPT_REF(scGabombaCatacombsFreezeStreamPuddle)},
	{CUE_TABLE_END},
};
