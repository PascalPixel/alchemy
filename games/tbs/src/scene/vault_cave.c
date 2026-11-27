/* Vault Cave, the optional cave under Vault that the party can explore once it
   comes back to the town (SceneId_VaultRevisit), with the Vambrace in a chest
   and Sap, the Venus Djinni, waiting outside its far end. Its three parts are
   scenes of their own, all named Vault Cave (area 31).

   The way in is the passage Reveal uncovers among Vault's six gravestones,
   where the dog jumps once given a Bone; a ladder leads down from it. The way
   out is a cave mouth in the town's north-east cliff, above which the bell sent
   Sap.

   The cave is a side trip of puzzles for Move, Frost and Whirlwind. No one
   speaks, and the only messages are "The door is tightly locked." and "Isaac
   flipped the switch."

   - SceneId_VaultCavePassage holds the foot of the ladder from Vault
	 (VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT; Retreat leads there too). Its
	 right-hand branch ends at a chest that is a Mimic (FIND_MIMIC). Its pool
	 room has a switch on a ledge: flipping it shakes the cave and drains the
	 pool (scVaultCaveFlipSwitch, FLAG_VAULT_CAVE_SWITCH_FLIPPED), and the
	 water runs down into the floor of the northern room of
	 SceneId_VaultCaveVambrace, which scInit draws dry until then.
	 Coming down the ladder from Vault again resets the switch.
   - SceneId_VaultCaveVambrace is up the left-hand branch. In the room of two
	 burning torches, Isaac pushes or Moves the west torch east onto the tile
	 with a fire crest (scVaultCavePushSouthernPillar,
	 scVaultCaveMoveSouthernPillar): the puzzle chime plays and the wooden gate
	 beyond opens (FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH). North of it is the
	 room of six puddles, where drops of water fall from the ceiling one at a
	 time. Frost freezes a puddle into a tall ice pillar whose top Isaac can
	 leap across, and frozen pillars make a way over the room; a drop landing
	 on one stops on its top. Up on the room's ledge, Whirlwind blows away the
	 ivy over the way to the Vambrace's chest (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY,
	 kept while the party stays in the cave).
   - SceneId_VaultCaveMouth leads out to the cave mouth
	 (VAULT_CAVE_MOUTH_TO_VAULT_REVISIT). In its room of three torches, Isaac
	 must get the lower burning torch north onto the fire crest below the
	 wooden gate, past the unlit torch in the way, while drops of water fall
	 on a fixed beat (VaultCave_ScheduleFrames). A drop that lands on the
	 burning torch puts its flame out (FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK),
	 and an unlit torch opens nothing until the room is entered again and the
	 flame comes back.

   Each room has a door that only says it is locked (scVaultCaveShowLockedDoor).

   The pushes share scVaultCavePushPillarAhead, which moves the torch the leader
   faces one metatile unless another actor or the rock is in the way. The water
   drops run as events started by scInit. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/actor_step.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern const Sint32		 VaultCave_SplashScript[];
extern SCENE_ENTRANCE	 VaultCavePassage_Entrances[];
extern SCENE_ENTRANCE	 VaultCaveVambrace_Entrances[];
extern SCENE_ENTRANCE	 VaultCaveMouth_Entrances[];
extern SCENE_EXIT_REGION VaultCave_Regions[];
extern Uint32			 VaultCave_FallbackExits[];
extern Uint32			 VaultCavePassage_Exits[];
extern Uint32			 VaultCaveVambrace_Exits[];
extern Uint32			 VaultCaveMouth_Exits[];
extern ACTOR_PLACE		 VaultCave_FallbackActorPlacements[];
extern ACTOR_PLACE		 VaultCavePassage_ActorPlacements[];
extern ACTOR_PLACE		 VaultCaveVambrace_ActorPlacements[];
extern ACTOR_PLACE		 VaultCaveMouth_ActorPlacements[];
extern CUE				 VaultCave_FallbackCues[];
extern CUE				 VaultCavePassage_Cues[];
extern CUE				 VaultCaveVambrace_Cues[];
extern CUE				 VaultCaveMouth_Cues[];
extern Uint32			 VaultCave_ScheduleFrames[];
extern Sint32			 VaultCave_ScheduleTimer;
extern Sint32			 VaultCave_ScheduleIndex;
extern Sint32			 VaultCave_DropFrames;
extern Sint32			 VaultCave_SelectedDrop;

static void scVaultCaveApplySouthernSwitchMetatiles(void);
static void scVaultCaveUseUnderfootFrostPriorities(void);
static void scVaultCaveUseAutomaticFrostPriorities(void);
static void scVaultCaveSpawnSplash(ACTOR *source);

/* The map metatile a 16.16 coordinate lies in, by division: the same as
   FIELD_METATILE's shift but for a coordinate below 0, which it rounds toward
   metatile 0. */
#define METATILE_OF(position) ((position) / toFIXED(16))

/* The metatile behaviors of the metatiles the scripts answer. */
typedef enum VAULT_CAVE_METATILE_BEHAVIOR {
	/* The switch on the ledge in the pool room: metatile (28, 29). */
	METATILE_BEHAVIOR_VAULT_CAVE_SWITCH = 20,
	/* The floor of the room of three torches, where Isaac pushes them:
	   metatiles (12, 14) to (20, 17) and the crest's metatile (16, 12). */
	METATILE_BEHAVIOR_VAULT_CAVE_NORTHERN_PILLAR = 50,
	/* The floor of the room of two torches, where Isaac pushes them: metatiles
	   (10, 55) to (16, 55) and the rows either side. */
	METATILE_BEHAVIOR_VAULT_CAVE_SOUTHERN_PILLAR = 60,
	METATILE_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR = 99,
	METATILE_BEHAVIOR_VAULT_CAVE_CHEST = 100
} VAULT_CAVE_METATILE_BEHAVIOR;

/* Scene flags, and one area flag. The six puddles of SceneId_VaultCaveVambrace
   keep their frozen state in six flags in a row, read by index as well. */
enum {
	/* The upper western puddle, the first of the six. */
	FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
	FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
	FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_02,
	FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_03,
	FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_04,
	FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN = FLAG_SCENE_TEMP_05,
	/* A drop of water has put out the flame of the lower torch in the room of
	   three torches. The drop's metatile matched the torch's
	   (scVaultCaveResetFallingActorAtActorMetatile). */
	FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK = FLAG_SCENE_TEMP_07,
	/* Whirlwind has blown the ivy away. */
	FLAG_VAULT_CAVE_IVY_BLOWN_AWAY = FLAG_AREA_TEMP_00
};

/* SPRITE_FLAME_PILLAR's animations (flame_pillar.tsv). */
typedef enum FLAME_PILLAR_ANIMATION {
	FLAME_PILLAR_ANIMATION_UNLIT = 1,  /* the pillar, no flame on top */
	FLAME_PILLAR_ANIMATION_BURNING = 2 /* a flame flickering on top */
} FLAME_PILLAR_ANIMATION;

/* The actors of SceneId_VaultCaveVambrace, numbering the rows of
   VaultCaveVambrace_ActorPlacements. */
typedef enum VAULT_CAVE_VAMBRACE_ACTOR_ID {
	/* The burning torch beside the wooden gate in the room of two torches, two
	   metatiles east of the fire crest. It is drawn underfoot when the party
	   arrives by VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH or
	   VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM. */
	ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH = ACTOR_FIRST_PLACED,
	/* The west torch, which goes onto the fire crest at metatile (15, 54). */
	ACTOR_SOUTHERN_PILLAR,
	/* The first of six drops of water that fall at random, one every 64 frames
	   (scVaultCaveUpdateFrostControlledFallingActors). scVaultCaveSpawnSplash
	   also uses their sprite for the splash where one lands. */
	ACTOR_FIRST_WATER_DROP,
	/* The six puddles under the drops, in their order and in the order of their
	   flags (FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN to
	   FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN). Frost freezes each into an ice
	   pillar, as in bilibin_cave.c, and a drop landing on one stops on its top,
	   32 pixels up. */
	ACTOR_WEST_UPPER_PUDDLE = ACTOR_FIRST_WATER_DROP + 6,
	ACTOR_WEST_LOWER_PUDDLE,
	ACTOR_INNER_WEST_LOWER_PUDDLE,
	ACTOR_EAST_UPPER_PUDDLE,
	ACTOR_EAST_LOWER_PUDDLE,
	ACTOR_FAR_EAST_PUDDLE,
	/* The ivy on the ledge above the room of six puddles, over the way to the
	   Vambrace's chest (SPRITE_IVY, as in bilibin.c, drawn half as tall again).
	   It is placed until Whirlwind blows it away
	   (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY). */
	ACTOR_IVY
} VAULT_CAVE_VAMBRACE_ACTOR_ID;

/* The actors of SceneId_VaultCaveMouth, numbering the rows of
   VaultCaveMouth_ActorPlacements. The fire crest in the room of three torches
   is map metatile (16, 12). */
typedef enum VAULT_CAVE_MOUTH_ACTOR_ID {
	/* The burning torch beside the wooden gate, two metatiles east of the fire
	   crest. */
	ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH = ACTOR_FIRST_PLACED,
	/* The unlit torch two metatiles south of the fire crest, in the way. */
	ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH,
	/* The lower burning torch, which goes onto the fire crest. */
	ACTOR_NORTHERN_PILLAR,
	/* The first of four drops of water that fall on a fixed schedule
	   (scVaultCaveUpdateTimedFallingActors). */
	ACTOR_FIRST_TIMED_WATER_DROP
} VAULT_CAVE_MOUTH_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VAULT_CAVE_PASSAGE) {
		return VaultCavePassage_Entrances;
	}
	if(sceneId == SCENE_VAULT_CAVE_VAMBRACE) {
		return VaultCaveVambrace_Entrances;
	}
	if(sceneId == SCENE_VAULT_CAVE_MOUTH) {
		return VaultCaveMouth_Entrances;
	}
	return Scene_FallbackEntrances;
}

/* Returns the region boxes used by the cave's exits. */
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return VaultCave_Regions;
}

const Uint32 *scGetExits(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VAULT_CAVE_PASSAGE) {
		return VaultCavePassage_Exits;
	}
	if(sceneId == SCENE_VAULT_CAVE_VAMBRACE) {
		return VaultCaveVambrace_Exits;
	}
	if(sceneId == SCENE_VAULT_CAVE_MOUTH) {
		return VaultCaveMouth_Exits;
	}
	return VaultCave_FallbackExits;
}

/* Selects the torches, drops of water, puddles and ivy of the room. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VAULT_CAVE_PASSAGE) {
		return VaultCavePassage_ActorPlacements;
	}
	if(sceneId == SCENE_VAULT_CAVE_VAMBRACE) {
		return VaultCaveVambrace_ActorPlacements;
	}
	if(sceneId == SCENE_VAULT_CAVE_MOUTH) {
		return VaultCaveMouth_ActorPlacements;
	}
	return VaultCave_FallbackActorPlacements;
}

/* Finds a placed actor with collision enabled at the requested map metatile. */
static ACTOR *scVaultCaveFindCollidableActorAtMetatile(
	Sint32 column, Sint32 row)
{
	FIELD_STATE *state = FieldStateWork;
	Uint32		 actorSlot;

	for(actorSlot = ACTOR_FIRST_PLACED; actorSlot <= ACTOR_LAST_ID;
		actorSlot++) {
		ACTOR *actor = state->actors[actorSlot];

		/* UB: map creation leaves unused actor slots NULL. */
		if(
#if FIX_NULL_DEREFERENCE
			actor != NULL &&
#endif
			column == FIELD_METATILE(actor->x.fixed) &&
			row == FIELD_METATILE(actor->z.fixed) && actor->collisionFlg != 0) {
			return actor;
		}
	}
	return NULL;
}

/* Pushes the torch Isaac faces one metatile, unless another torch or the rock
   is in the way. */
static void scVaultCavePushPillarAhead(void)
{
	ACTOR	 *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32	  direction = FACING_INDEX(leader->facing);
	ACTOR	 *pillar;
	ACTOR	 *beyond;
	FIELD_POS destination;
	Sint32	  fallingIndex;
	Sint32	  column;
	Sint32	  row;

	column = (leader->x.part[0].integer +
				 (FIXEDtoINT(PushedActor_DirectionSteps[direction]))) >>
		4;
	row = (leader->z.part[0].integer +
			  (Sint16)PushedActor_DirectionSteps[direction]) >>
		4;
	pillar = scVaultCaveFindCollidableActorAtMetatile(column, row);
	/* UB: the pillar's collisionFlags is read before the test for no pillar. */
#if FIX_NULL_DEREFERENCE
	if(pillar == NULL || pillar->collisionFlg == 0) {
#else
	if(pillar->collisionFlg == 0 || pillar == NULL) {
#endif
		return;
	}
	/* SceneId_VaultCaveMouth's water drops are never pushed; in
	   SceneId_VaultCaveVambrace these slots hold water drops too. */
	for(fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
		if(pillar == fdActorGet(fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP)) {
			return;
		}
	}
	column = (pillar->x.part[0].integer +
				 (FIXEDtoINT(PushedActor_DirectionSteps[direction]))) >>
		4;
	row = (pillar->z.part[0].integer +
			  (Sint16)PushedActor_DirectionSteps[direction]) >>
		4;
	beyond = scVaultCaveFindCollidableActorAtMetatile(column, row);
	if(beyond != NULL && (beyond->collisionFlg & ACTOR_SOLID)) {
		return;
	}
	pillar->layer = 2;
	fdActorStepFrom(
		&destination, pillar, PushedActor_DirectionSteps[direction]);
	if(syFieldActorCheckMovementCollision(pillar, &destination) > 0) {
		return;
	}
	fdActorSelectAnimation(leader, ANIM_PUSH);
	sySynch(15);
	sySndPlay(SE_PUSH_STEP);
	pillar->speedLimit = toFIXED(1) / 5;
	pillar->acceleration = toFIXED(1) / 5;
	syFieldActorSetMoveTargetFX(
		pillar, destination.x, destination.y, destination.z);
	leader->speedLimit = toFIXED(1) / 5;
	leader->acceleration = toFIXED(1) / 5;
	syFieldActorSetMoveTargetFX(
		leader, destination.x, destination.y, destination.z);
	fdActorWaitForMove(pillar);
	sySndPlay(SE_STOP_ACTION_SE);
	pillar->x.fixed = destination.x;
	pillar->z.fixed = destination.z;
	pillar->velX = 0;
	pillar->velZ = 0;
	leader->velX = 0;
	leader->velZ = 0;
	leader->tgtX = ACTOR_NO_TARGET;
	leader->tgtZ = ACTOR_NO_TARGET;
	fdActorSelectAnimation(leader, ANIM_STAND);
}

/* Whirlwind on the ivy on the ledge above the room of six puddles blows it
   away, opening the way to the Vambrace's chest. */
static void scVaultCaveUseWhirlwind(void)
{
	gmFlagSet(FLAG_VAULT_CAVE_IVY_BLOWN_AWAY);
}

/* Searching a locked door in Vault Cave: "The door is tightly locked." */
static void scVaultCaveShowLockedDoor(void)
{
	mnFieldMessageShow(MSGNO(MsgFieldDoorTightlyLocked), 1);
}

/* Selects the cave scene's examine cues, movement checks and Psynergy
   callbacks. */
const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VAULT_CAVE_PASSAGE) {
		return VaultCavePassage_Cues;
	}
	if(sceneId == SCENE_VAULT_CAVE_VAMBRACE) {
		return VaultCaveVambrace_Cues;
	}
	if(sceneId == SCENE_VAULT_CAVE_MOUTH) {
		return VaultCaveMouth_Cues;
	}
	return VaultCave_FallbackCues;
}

/* Whether the west torch of the room of two torches stands on the fire
   crest. */
static Bool scVaultCaveIsSouthernPillarOnSwitch(void)
{
	ACTOR *pillar = fdActorGet(ACTOR_SOUTHERN_PILLAR);
	Sint32 row = pillar->z.fixed / toFIXED(16);
	Sint32 column = pillar->x.fixed / toFIXED(16);

	if(column == 15 && row == 54) {
		return TRUE;
	}
	return FALSE;
}

/* Once the torch is on the fire crest, plays the puzzle chime and opens the
   wooden gate. */
static inline void scVaultCaveCompleteSouthernSwitch(void)
{
	if(scVaultCaveIsSouthernPillarOnSwitch() != 0) {
		gmFlagSet(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH);
		sySndPlay(MUS_PUZZLE_SOLVED);
		scVaultCaveApplySouthernSwitchMetatiles();
	}
}

/* In the room of two burning torches, up the left-hand branch, Isaac pushes a
   torch a metatile. Once the west torch reaches the tile with the fire crest
   (metatile (15, 54)), the puzzle chime plays and the wooden gate north of it
   opens. */
static void scVaultCavePushSouthernPillar(void)
{
	if(fdScriptGetFlag(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) == 0) {
		scVaultCavePushPillarAhead();
		scVaultCaveCompleteSouthernSwitch();
	}
}

/* Draws the room of two torches' wooden gate open and lets the party through
   it. */
static void scVaultCaveApplySouthernSwitchMetatiles(void)
{
	fdMapGridCopyMetatiles(87, 50, 2, 4, 80, 50);
	fdMapGridCopyMetatiles(23, 52, 1, 2, 16, 52);
	fdMapGridCopyMetatileAttributes(16, 52, 1, 1, 16, 53);
}

/* In the room of two burning torches, Isaac uses Move on the west torch: a
   great hand slides it a metatile, and if that puts it on the fire crest the
   puzzle chime plays and the wooden gate opens. */
static void scVaultCaveMoveSouthernPillar(void)
{
	if(fdScriptGetFlag(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) == 0) {
		scVaultCaveCompleteSouthernSwitch();
	}
}

static void UNUSED scVaultCaveUnusedNoOperation(void)
{
}

/* Draws the room of three torches' wooden gate open and lets the party through
   it. */
static void scVaultCaveApplyNorthernSwitchMetatiles(void)
{
	fdMapGridCopyMetatiles(90, 9, 2, 3, 80, 9);
	fdMapGridCopyMetatiles(27, 10, 1, 2, 17, 10);
	fdMapGridCopyMetatileAttributes(17, 10, 1, 1, 17, 11);
}

/* Whether the lower torch of the room of three torches stands on the fire
   crest. */
static Bool scVaultCaveIsNorthernPillarOnSwitch(void)
{
	ACTOR *pillar = fdActorGet(ACTOR_NORTHERN_PILLAR);
	Sint32 row = pillar->z.fixed / toFIXED(16);
	Sint32 column = pillar->x.fixed / toFIXED(16);

	if(column == 16 && row == 12) {
		return TRUE;
	}
	return FALSE;
}

/* Opens the gate only for a torch on the fire crest whose flame no drop has put
   out. */
static inline void scVaultCaveCompleteNorthernSwitch(void)
{
	if(gmFlagGet(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH) == 0) {
		if(scVaultCaveIsNorthernPillarOnSwitch() != 0) {
			if(gmFlagGet(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
				sySndPlay(MUS_PUZZLE_SOLVED);
				scVaultCaveApplyNorthernSwitchMetatiles();
				gmFlagSet(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH);
			}
		}
	}
}

/* In the room of three torches, on the way to the cave mouth, Isaac pushes a
   torch a metatile. If the lower torch reaches the fire crest below the wooden
   gate (metatile (16, 12)) still burning, the puzzle chime plays and the gate
   opens; if a drop of water has put its flame out, nothing happens. */
static void scVaultCavePushNorthernPillar(void)
{
	scVaultCavePushPillarAhead();
	scVaultCaveCompleteNorthernSwitch();
}

/* In the room of three torches, Isaac uses Move on the unlit torch just south
   of the fire crest. The great hand slides it a metatile like any torch, even
   onto the crest, but an unlit torch opens nothing.

   The script itself is empty: Move does the sliding. */
static void scVaultCaveMoveUnlitTorch(void)
{
}

/* In the room of three torches, Isaac uses Move on the lower burning torch: a
   great hand slides it a metatile, and if that puts it on the fire crest still
   burning, the puzzle chime plays and the wooden gate opens. */
static void scVaultCaveMoveNorthernPillar(void)
{
	scVaultCaveCompleteNorthernSwitch();
}

/* In the room of six puddles, Isaac casts Frost on the north-west puddle
   (metatile (23, 32)), and it freezes into a tall ice pillar whose top Isaac
   can leap across. */
static void scVaultCaveFreezeWestUpperPuddle(void)
{
	ACTOR *target = fdActorGet(ACTOR_WEST_UPPER_PUDDLE);

	fdMapGridCopyMetatileAttributes(26, 30, 1, 1, 23, 32);

	if(target != NULL) {
		fdActorGet(ACTOR_WEST_UPPER_PUDDLE)->motionFlg = 0;
		target->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}

	gmFlagSet(FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN);
}

/* In the room of six puddles, Isaac casts Frost on the south-west puddle
   (metatile (23, 34)), and it freezes into a tall ice pillar whose top Isaac
   can leap across. */
static void scVaultCaveFreezeWestLowerPuddle(void)
{
	ACTOR *target = fdActorGet(ACTOR_WEST_LOWER_PUDDLE);

	fdMapGridCopyMetatileAttributes(26, 30, 1, 1, 23, 34);

	if(target != NULL) {
		fdActorGet(ACTOR_WEST_LOWER_PUDDLE)->motionFlg = 0;
		target->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}

	gmFlagSet(FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN);
}

/* In the room of six puddles, Isaac casts Frost on the puddle just east of the
   south-west one (metatile (24, 34)), and it freezes into a tall ice pillar
   whose top Isaac can leap across. */
static void scVaultCaveFreezeInnerWestLowerPuddle(void)
{
	ACTOR *target = fdActorGet(ACTOR_INNER_WEST_LOWER_PUDDLE);

	fdMapGridCopyMetatileAttributes(26, 30, 1, 1, 24, 34);

	if(target != NULL) {
		fdActorGet(ACTOR_INNER_WEST_LOWER_PUDDLE)->motionFlg = 0;
		target->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}

	gmFlagSet(FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN);
}

/* In the room of six puddles, Isaac casts Frost on the north-east puddle
   (metatile (26, 32)), and it freezes into a tall ice pillar whose top Isaac
   can leap across. */
static void scVaultCaveFreezeEastUpperPuddle(void)
{
	ACTOR *target = fdActorGet(ACTOR_EAST_UPPER_PUDDLE);

	fdMapGridCopyMetatileAttributes(26, 30, 1, 1, 26, 32);

	if(target != NULL) {
		fdActorSetSprFlags(target, 0);

		fdActorGet(ACTOR_EAST_UPPER_PUDDLE)->motionFlg = 0;
		target->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}

	gmFlagSet(FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN);
}

/* In the room of six puddles, Isaac casts Frost on the south-east puddle
   (metatile (26, 34)), and it freezes into a tall ice pillar whose top Isaac
   can leap across. */
static void scVaultCaveFreezeEastLowerPuddle(void)
{
	ACTOR *target = fdActorGet(ACTOR_EAST_LOWER_PUDDLE);

	fdMapGridCopyMetatileAttributes(26, 30, 1, 1, 26, 34);

	if(target != NULL) {
		fdActorSetSprFlags(target, 0);

		fdActorGet(ACTOR_EAST_LOWER_PUDDLE)->motionFlg = 0;
		target->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}

	gmFlagSet(FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN);
}

/* In the room of six puddles, Isaac casts Frost on the far east puddle
   (metatile (28, 33)), and it freezes into a tall ice pillar whose top Isaac
   can leap across. */
static void scVaultCaveFreezeFarEastPuddle(void)
{
	ACTOR *target = fdActorGet(ACTOR_FAR_EAST_PUDDLE);

	fdMapGridCopyMetatileAttributes(26, 30, 1, 1, 28, 33);

	if(target != NULL) {
		fdActorSetSprFlags(target, 0);

		fdActorGet(ACTOR_FAR_EAST_PUDDLE)->motionFlg = 0;
		target->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}

	gmFlagSet(FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN);
}

/* Keeps the ice pillars drawn under Isaac while he is up on top of one. */
static void scVaultCaveUpdatePuddlePriorities(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(leader->y.fixed >= toFIXED(16)) {
		scVaultCaveUseUnderfootFrostPriorities();
	} else {
		scVaultCaveUseAutomaticFrostPriorities();
	}
}

/* Keeps all six puddles beneath the leader's feet. */
static void scVaultCaveUseUnderfootFrostPriorities(void)
{
	Sint32 targetIndex;
	Sint32 actorSlot;

	actorSlot = ACTOR_WEST_UPPER_PUDDLE;
	for(targetIndex = 0; targetIndex < 6; targetIndex++) {
		fdActorGet(actorSlot)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		actorSlot++;
	}
}

/* Restores automatic draw priority for the same six puddles. */
static void scVaultCaveUseAutomaticFrostPriorities(void)
{
	Sint32 targetIndex;
	Sint32 actorSlot;

	actorSlot = ACTOR_WEST_UPPER_PUDDLE;
	for(targetIndex = 0; targetIndex < 6; targetIndex++) {
		fdActorGet(actorSlot)->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		actorSlot++;
	}
}

/* The cave rumbles and shakes for eight seconds while the pool's water churns,
   then the water fades away and the drained pool is drawn. */
static void scVaultCaveRunRumble(void)
{
	MAP_GRID_LAYER_SCROLL *layer =
		&((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];
	Sint32 frame;
	Sint32 fadeLevel;
	Sint32 lowFadeLevel;
	Sint32 upperFadeBits;

	fdScriptStart();
	if(FrameCount & 1) {
		layer->speedX = 1;
		layer->speedY = 1;
	} else {
		layer->speedX = -1;
		layer->speedY = -1;
	}
	fdMapGridSetShake(3, 3, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	sySndPlay(SE_RUSH);
	/* The water churns, its layer drifting by 0.04 to 0.07 of a pixel a
	   frame. */
	for(frame = 0; frame < 480; frame++) {
		Uint32 scrollDrift = syRandomBelow(2048);

		layer->phaseY =
			(double)layer->phaseY - (4718.592 - (double)scrollDrift);
		fdScriptWait(1);
	}
	fadeLevel = 6;
	lowFadeLevel = 6;
	frame = 0;
	/* The water fades off the floor in four steps, one every 20 frames. The
	   blend is packed as if it were a colour, the levels in RGB's three fields:
	   REG_BLDALPHA reads lowFadeLevel as the water's weight and the first
	   level, in the blue field, as the floor's, 24, above full; the green field
	   falls in unused bits. */
	upperFadeBits = fadeLevel << BLUE_SHIFT;
	for(; frame <= 69; frame++) {
		syIOWriteQueueStore16((Uint32)&REG_BLDALPHA,
			upperFadeBits | (fadeLevel << GREEN_SHIFT) | lowFadeLevel);
		fdScriptWait(1);
		if(frame % 20 == 0) {
			lowFadeLevel--;
			fadeLevel--;
		}
	}
	fdMapGridCopyMetatiles(19, 83, 15, 8, 19, 91);
	sySndPlay(SE_STOP_ACTION_SE);
	fdMapGridCenterOnFollowedPoint();
	fdMapGridWaitForShakeToSettle();
	fdScriptEnd();
}

/* Isaac flips the switch on the ledge in the pool room (metatile (28, 29)).
   "Isaac flipped the switch." The cave rumbles and shakes, and the pool drains
   away, and its water runs down into the northern room of
   SceneId_VaultCaveVambrace below, whose floor scInit draws dry until then.

   The switch can be flipped once each time the party comes down the ladder from
   Vault. */
static void scVaultCaveFlipSwitch(void)
{
	Sint32 destinationColumn;

	if(fdScriptGetFlag(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
		mnFieldMessageShow(MSGNO(MsgFieldFlippedSwitch), 1);
		gmFlagSet(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
		destinationColumn = 92;
		sySndPlay(SE_SWITCH);
		fdMapGridCopyMetatiles(107, 27, 1, 1, destinationColumn, 27);
		fdScriptWait(39);
		fdMapGridCopyMetatiles(108, 27, 1, 1, destinationColumn, 27);
		fdScriptWait(50);
		destinationColumn = 25;
		sySndPlay(SE_SWITCH_MECHANISM);
		fdMapGridCopyMetatiles(1, 24, 1, 2, destinationColumn, 27);
		fdScriptWait(40);
		fdMapGridCopyMetatiles(2, 24, 1, 2, destinationColumn, 27);
		fdScriptWait(40);
		scVaultCaveRunRumble();
	}
}

/* Whether a drop of water about to land is over the given actor's metatile; if
   so, the drop is sent back up to fall again. */
static Sint32 scVaultCaveResetFallingActorAtActorMetatile(Sint32 subjectActorId)
{
	ACTOR *subject = fdActorGet(subjectActorId);
	Sint32 fallingIndex;

	for(fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
		ACTOR *fallingActor =
			fdActorGet(fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP);

		if(fallingActor->y.fixed > 0 && fallingActor->y.fixed < toFIXED(16)) {
			Sint32 fallingRow = METATILE_OF(fallingActor->z.fixed);
			Sint32 fallingColumn = METATILE_OF(fallingActor->x.fixed);
			Sint32 subjectRow = METATILE_OF(subject->z.fixed);
			Sint32 subjectColumn = METATILE_OF(subject->x.fixed);
			Sint32 zDelta = subjectRow - fallingRow;
			Sint32 xDelta = subjectColumn - fallingColumn;

			if(xDelta == 0 && zDelta == 0) {
				fallingActor->y.fixed = toFIXED(255);
				fallingActor->gravity = 0;
				fallingActor->velY = 0;
				return 1;
			}
		}
	}

	return 0;
}

/* In the room of three torches, lets the four drops of water fall on their
   fixed beat, splashing where they land and putting out the lower torch's flame
   if one lands on it. */
static void scVaultCaveUpdateTimedFallingActors(void)
{
	Sint32 fallingIndex;
	ACTOR *fallingActor;
	Sint32 paused;
	Sint32 scheduleIndex;

	paused = fdActorGet(ACTOR_NORTHERN_PILLAR)->paused;
	if(paused == 0) {
		if(++VaultCave_ScheduleTimer > 190) {
			VaultCave_ScheduleTimer = 0;
		}
		scheduleIndex = VaultCave_ScheduleIndex;
		if(VaultCave_ScheduleFrames[scheduleIndex] == VaultCave_ScheduleTimer) {
			fallingActor =
				fdActorGet((scheduleIndex + ACTOR_FIRST_TIMED_WATER_DROP));
			fallingActor->gravity = toFIXED(1) / 25;
			if(++VaultCave_ScheduleIndex > 3) {
				VaultCave_ScheduleIndex = 0;
			}
		}
		for(fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
			fallingActor =
				fdActorGet((fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP));
			if(fallingActor->velY >= 0) {
				if(fallingActor->y.fixed < toFIXED(1)) {
					scVaultCaveSpawnSplash(fallingActor);
					fallingActor->y.fixed = toFIXED(255);
					fallingActor->gravity = 0;
					fallingActor->velY = 0;
					fallingActor->paused = 0;
					sySndPlay(SE_DRIP);
				}
			}
		}
		/* A water drop landing on either movable pillar restarts at the top. */
		if(scVaultCaveResetFallingActorAtActorMetatile(ACTOR_NORTHERN_PILLAR) !=
			0) {
			fdActorSetAnimation(
				ACTOR_NORTHERN_PILLAR, FLAME_PILLAR_ANIMATION_UNLIT);
			if(fdScriptGetFlag(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
				gmFlagSet(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK);
				sySndPlay(SE_DUST_PUFF);
			} else {
				sySndPlay(SE_DRIP);
			}
		}
		if(scVaultCaveResetFallingActorAtActorMetatile(
			   ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH) != 0) {
			sySndPlay(SE_DRIP);
		}
	}
}

/* In the room of six puddles, lets a drop of water chosen at random fall every
   64 frames, onto its puddle or onto the top of its ice pillar. */
static void scVaultCaveUpdateFrostControlledFallingActors(void)
{
	Sint32 fallingIndex;
	ACTOR *fallingActor;
	Sint32 frozen;

	fallingActor = fdActorGet(ACTOR_FIRST_WATER_DROP);
	if(fallingActor->paused == 0) {
		if((++VaultCave_DropFrames & 63) == 0) {
			VaultCave_SelectedDrop = syMathRemainderUnsigned(syRandomNext(), 6);
			fallingActor =
				fdActorGet(VaultCave_SelectedDrop + ACTOR_FIRST_WATER_DROP);
			fallingActor->gravity = toFIXED(1) / 25;
		}
		/* A drop goes back up once it reaches its puddle, or the top of the ice
		   pillar 32 pixels up when the puddle is frozen. */
		for(fallingIndex = 0; fallingIndex <= 5; fallingIndex++) {
			fallingActor = fdActorGet((fallingIndex + ACTOR_FIRST_WATER_DROP));
			frozen = gmFlagGet(
				(fallingIndex + FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN));
			if(frozen != 0) {
				if(fallingActor->velY <= 0) {
					if(fallingActor->y.fixed >= toFIXED(33)) {
						continue;
					}
				}
				fallingActor->y.fixed = toFIXED(255);
				fallingActor->gravity = 0;
				fallingActor->velY = 0;
				sySndPlay(SE_DRIP);
			} else {
				if(fallingActor->velY <= 0) {
					if(fallingActor->y.fixed >= toFIXED(1)) {
						continue;
					}
				}
				/* Both stop: frozen is 0 here. */
				fallingActor->gravity = frozen;
				fallingActor->velY = frozen;
				fallingActor->y.fixed = toFIXED(255);
				sySndPlay(SE_DRIP);
			}
		}
	}
}

/* Offsets an actor's current position by a polar displacement. */
static void scVaultCaveOffsetActorRadially(
	ACTOR *actor, Sint32 radius, Sint32 angle)
{
	Sint32 destination[3];

	if(actor == NULL) {
		return;
	}
	SET_VECTOR(destination, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syVectorAddPolarOffset(radius, angle, destination);
	syFieldActorSetMoveTargetFX(
		actor, destination[0], destination[1], destination[2]);
}

/* Splashes up to four little drops around a drop of water where it lands. */
static void scVaultCaveSpawnSplash(ACTOR *source)
{
	ACTOR *drop;
	Sint32 dropIndex;

	for(dropIndex = 0; dropIndex < 4; dropIndex++) {
		drop = syFieldActorCreate(
			SPRITE_DROPLET, source->x.fixed, source->y.fixed, source->z.fixed);
		if(drop == NULL)
			break;
		drop->scaleY = toFIXED(11) / 20;
		drop->scaleX = toFIXED(11) / 20;
		drop->motionFlg = ACTOR_FALLS;
		drop->velY = -toFIXED(1);
		drop->speedLimit = syRandomNext() + toFIXED(4) / 5;
		drop->collisionFlg = ACTOR_SOLID;
		scVaultCaveOffsetActorRadially(drop, toFIXED(32), syRandomNext());
		drop->wait = 8;
		fdActorStartScript(drop, VaultCave_SplashScript);
	}
}

/* Readies the room of six puddles' drops of water and starts them falling. */
static void scVaultCaveStartFrostControlledFalls(void)
{
	Sint32 actorSlot = ACTOR_FIRST_WATER_DROP;
	Sint32 i;

	for(i = 0; i < 6; i++) {
		ACTOR *fallingActor;

		fdActorSetSprFlags(fdActorGet(actorSlot), 0);
		fallingActor = fdActorGet(actorSlot);
		fallingActor->bounce = toFIXED(1) / 10;
		fallingActor->gravity = 0;
		fallingActor->y.fixed = toFIXED(255);
		actorSlot++;
	}

	sySetFrameEvent(
		scVaultCaveUpdateFrostControlledFallingActors, EVENT_PASS_FRAME);
}

/* Readies the room of three torches' drops of water and starts them falling. */
static void scVaultCaveStartTimedFalls(void)
{
	Sint32 actorSlot = ACTOR_FIRST_TIMED_WATER_DROP;
	Sint32 fallingIndex = 0;

	do {
		ACTOR *fallingActor;

		fdActorSetSprFlags(fdActorGet(actorSlot), 0);
		fallingActor = fdActorGet(actorSlot);
		fallingActor->bounce = toFIXED(1) / 10;
		fallingActor->gravity = 0;
		fallingActor->y.fixed = toFIXED(255);
		fdActorSetSprPriority(fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP, 1);
		fallingIndex++;
		actorSlot++;
	} while(fallingIndex <= 3);

	sySetFrameEvent(scVaultCaveUpdateTimedFallingActors, EVENT_PASS_FRAME);
}

/* Arriving in any room of Vault Cave restores the room's puzzles and actors. It
   then sets the Retreat destination, the foot of the ladder from Vault. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next == SCENE_VAULT_CAVE_PASSAGE) {
		switch(MainRoutineStateWork.entrance) {
		case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST:
		case VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST:
		case VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST:
		case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST:
		case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST:
			if(gmFlagGet(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
				fdMapGridCopyMetatileAttributes(22, 29, 1, 1, 21, 29);
			} else {
				fdMapGridCopyMetatiles(108, 27, 1, 1, 92, 27);
				sySynch(1);
				fdMapGridCopyMetatiles(19, 83, 15, 8, 19, 91);
				sySynch(1);
				fdMapGridCopyMetatiles(2, 24, 1, 2, 25, 27);
			}
			fdMapGridCenterOnFollowedPoint();
			sySynch(1);
			break;
		case VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT:
			gmFlagClear(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
			break;
		}
	}
	if(MainRoutineStateWork.next == SCENE_VAULT_CAVE_VAMBRACE) {
		if(gmFlagGet(FLAG_VAULT_CAVE_IVY_BLOWN_AWAY) == 0) {
			fdActorGet(ACTOR_IVY)->scaleY = toFIXED(3) / 2;
		}
		switch(MainRoutineStateWork.entrance) {
		case VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH:
		case VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH:
		case VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM:
		case VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST:
			if(gmFlagGet(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
				fdMapGridCopyMetatiles(5, 81, 11, 7, 5, 73);
			} else {
				fdMapGridCopyMetatileAttributes(5, 12, 1, 1, 6, 12);
				fdMapGridCopyMetatileAttributes(12, 10, 1, 1, 12, 11);
			}
			break;
		case VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_NORTH:
		case VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_SOUTH:
		case VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_CHEST:
			scVaultCaveStartFrostControlledFalls();
			if(gmFlagGet(FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN) != 0) {
				fdActorSetAnimation(
					ACTOR_WEST_UPPER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
				scVaultCaveFreezeWestUpperPuddle();
			}
			if(gmFlagGet(FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN) != 0) {
				fdActorSetAnimation(
					ACTOR_WEST_LOWER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
				scVaultCaveFreezeWestLowerPuddle();
			}
			if(gmFlagGet(FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN) != 0) {
				fdActorSetAnimation(
					ACTOR_INNER_WEST_LOWER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
				scVaultCaveFreezeInnerWestLowerPuddle();
			}
			if(gmFlagGet(FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN) != 0) {
				fdActorSetAnimation(
					ACTOR_EAST_UPPER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
				scVaultCaveFreezeEastUpperPuddle();
			}
			if(gmFlagGet(FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN) != 0) {
				fdActorSetAnimation(
					ACTOR_EAST_LOWER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
				scVaultCaveFreezeEastLowerPuddle();
			}
			if(gmFlagGet(FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN) != 0) {
				fdActorSetAnimation(
					ACTOR_FAR_EAST_PUDDLE, PUDDLE_ANIMATION_PILLAR);
				scVaultCaveFreezeFarEastPuddle();
			}
			sySetFrameEvent(
				scVaultCaveUpdatePuddlePriorities, EVENT_PASS_FRAME);
			break;
		case VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH:
		case VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM:
			if(gmFlagGet(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) != 0) {
				scVaultCaveApplySouthernSwitchMetatiles();
				fdActorSetPos(ACTOR_SOUTHERN_PILLAR, 248, 876);
			}
			fdActorGet(ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH)->priFlg =
				ACTOR_PRIORITY_UNDERFOOT;
			break;
		}
		fdActorSetAnimation(ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH,
			FLAME_PILLAR_ANIMATION_BURNING);
		fdActorSetAnimation(
			ACTOR_SOUTHERN_PILLAR, FLAME_PILLAR_ANIMATION_BURNING);
		fdActorSetSprFlags(fdActorGet(ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH), 0);
		fdActorSetSprFlags(fdActorGet(ACTOR_SOUTHERN_PILLAR), 0);
		fdActorGet(ACTOR_SOUTHERN_PILLAR)->collisionFlg = ACTOR_SOLID;
	}
	if(MainRoutineStateWork.next == SCENE_VAULT_CAVE_MOUTH) {
		fdActorSetAnimation(ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH,
			FLAME_PILLAR_ANIMATION_BURNING);
		if(gmFlagGet(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
			fdActorSetAnimation(
				ACTOR_NORTHERN_PILLAR, FLAME_PILLAR_ANIMATION_BURNING);
		}
		fdActorSetSprFlags(fdActorGet(ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH), 0);
		fdActorSetSprFlags(fdActorGet(ACTOR_NORTHERN_PILLAR), 0);
		fdActorSetSprFlags(
			fdActorGet(ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH), 0);
		fdActorGet(ACTOR_NORTHERN_PILLAR)->collisionFlg |=
			ACTOR_BLOCKED_BY_SOLID;
		fdActorGet(ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH)->collisionFlg |=
			ACTOR_BLOCKED_BY_SOLID;
		switch(MainRoutineStateWork.entrance) {
		case VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH:
		case VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH:
			scVaultCaveStartTimedFalls();
			fdActorGet(ACTOR_FIRST_TIMED_WATER_DROP)->collisionFlg =
				ACTOR_PUSHABLE;
			fdActorGet(ACTOR_FIRST_TIMED_WATER_DROP + 1)->collisionFlg =
				ACTOR_PUSHABLE;
			fdActorGet(ACTOR_FIRST_TIMED_WATER_DROP + 2)->collisionFlg =
				ACTOR_PUSHABLE;
			fdActorGet(ACTOR_FIRST_TIMED_WATER_DROP + 3)->collisionFlg =
				ACTOR_PUSHABLE;
			fdActorGet(ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH)->collisionFlg =
				ACTOR_SOLID;
			fdActorGet(ACTOR_NORTHERN_PILLAR)->collisionFlg = ACTOR_SOLID;
			fdActorGet(ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH)->collisionFlg =
				ACTOR_SOLID;
			if(gmFlagGet(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH) != 0) {
				scVaultCaveApplyNorthernSwitchMetatiles();
				fdActorSetPos(ACTOR_NORTHERN_PILLAR, 264, 204);
			}
			break;
		}
	}
	/* Retreat always returns to the foot of SceneId_VaultCavePassage's ladder
	   from Vault. */
	MainRoutineStateWork.retreatEntrance =
		VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT;
	MainRoutineStateWork.retreatScene = SCENE_VAULT_CAVE_PASSAGE;
	return 0;
}

/* The cave keeps its pushed-actor steps with its read-only data, though they
   are not const: the pushes read the step again after each call. */
#define PUSHED_ACTOR_TABLES_READ_ONLY
#include "shared/pushed_actor_direction_steps.c"
/* clang-format off */

const Sint32 VaultCave_SplashScript[] = {
	SC_RELEASE,
};

#include "shared/fallback_entrances.c"

SCENE_ENTRANCE VaultCavePassage_Entrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_MOUTH, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
	{ VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_NORTHWEST, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
	{ VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_WEST, CONDITION_ALWAYS, 120, 0, 256, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
	{ VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_EAST, CONDITION_ALWAYS, 184, 0, 264, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
	{ VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST, CONDITION_ALWAYS, 344, 0, 464, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
	{ VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST, CONDITION_ALWAYS, 488, 0, 488, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
	{ VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST, CONDITION_ALWAYS, 584, 0, 608, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
	{ VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 280, 360, 640, 640, 0 },
	{ VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST, CONDITION_ALWAYS, 408, 0, 472, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
	{ VAULT_CAVE_PASSAGE_CHEST_FROM_WATER_ROOM, CONDITION_ALWAYS, 88, 0, 704, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
	{ VAULT_CAVE_PASSAGE_CHEST_FROM_LADDER, CONDITION_ALWAYS, 184, 0, 864, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
	{ VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 456, 0, 800, FACING_SOUTH, 0, 336, 696, 576, 856, 0 },
	{ VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST, CONDITION_ALWAYS, 456, 0, 840, FACING_NORTH, 0, 336, 696, 576, 856, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE VaultCaveVambrace_Entrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
	{ VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH, CONDITION_ALWAYS, 344, 0, 152, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
	{ VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM, CONDITION_ALWAYS, 344, 0, 184, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
	{ VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST, CONDITION_ALWAYS, 168, 0, 328, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
	{ VAULT_CAVE_VAMBRACE_WEST_FROM_NORTH, CONDITION_ALWAYS, 56, 0, 480, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
	{ VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTHWEST, CONDITION_ALWAYS, 152, 0, 472, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
	{ VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_SOUTH, CONDITION_ALWAYS, 120, 0, 576, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
	{ VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_NORTH, CONDITION_ALWAYS, 344, 0, 456, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
	{ VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_SOUTH, CONDITION_ALWAYS, 424, 0, 664, FACING_NORTH, 0, 280, 344, 544, 672, 0 },
	{ VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_CHEST, CONDITION_ALWAYS, 456, 0, 472, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
	{ VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH, CONDITION_ALWAYS, 200, 0, 776, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
	{ VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM, CONDITION_ALWAYS, 264, 0, 808, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
	{ VAULT_CAVE_VAMBRACE_CHEST_FROM_FROST_ROOM, CONDITION_ALWAYS, 536, 0, 168, FACING_NORTH, 0, 416, 24, 656, 184, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE VaultCaveMouth_Entrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 312, 0, 664, FACING_NORTH, 0, 200, 520, 440, 680, 0 },
	{ VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_PASSAGE, CONDITION_ALWAYS, 312, 0, 600, FACING_SOUTH, 0, 200, 520, 440, 680, 0 },
	{ VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_SOUTH, CONDITION_ALWAYS, 88, 0, 568, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
	{ VAULT_CAVE_MOUTH_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 168, 0, 544, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
	{ VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 248, 0, 336, FACING_NORTH, 0, 128, 40, 376, 384, 0 },
	{ VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH, CONDITION_ALWAYS, 280, 0, 104, FACING_SOUTH, 0, 128, 40, 376, 384, 0 },
	{ SCENE_TABLE_END },
};

SCENE_EXIT_REGION VaultCave_Regions[] = {
	{ 447, 47, 783, 463, 63, 799, -1, VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT },
	{ -1, 0, 0, 0, 0, 0, 0, 0 },
};

Uint32 VaultCave_FallbackExits[] = {
	SCENE_EXITS_END,
};

Uint32 VaultCavePassage_Exits[] = {
	SCENE_EXITS(VaultCavePassage),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_MOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_PASSAGE),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_NORTHWEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_WEST, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_EAST, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_WEST, VaultCavePassage, VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_WEST),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_EAST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTHWEST),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_SOUTHEAST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_CHEST, VaultCavePassage, VAULT_CAVE_PASSAGE_CHEST_FROM_WATER_ROOM),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_CHEST_TO_WATER_ROOM, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE),
	/* A second VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, to */
	/* VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST. */
	SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, VaultCavePassage, VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_LADDER_TO_CHEST, VaultCavePassage, VAULT_CAVE_PASSAGE_CHEST_FROM_LADDER),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_CHEST_TO_LADDER, VaultCavePassage, VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST),
	SCENE_EXIT(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_EAST, VaultCavePassage, VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_EAST),
	SCENE_EXITS_END,
};

Uint32 VaultCaveVambrace_Exits[] = {
	SCENE_EXITS(VaultCaveVambrace),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_NORTH, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_NORTHWEST),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_NORTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_NORTH_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_NORTH),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_NORTH_TO_WEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_WEST_FROM_NORTH),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_WEST_TO_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTHWEST, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_SOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_SOUTH),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_SOUTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTH, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_SOUTH_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_SOUTH),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_CHEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_CHEST_FROM_FROST_ROOM),
	SCENE_EXIT(VAULT_CAVE_VAMBRACE_CHEST_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_CHEST),
	SCENE_EXITS_END,
};

Uint32 VaultCaveMouth_Exits[] = {
	SCENE_EXITS(VaultCaveMouth),
	SCENE_EXIT(VAULT_CAVE_MOUTH_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH),
	SCENE_EXIT(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_PASSAGE, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_MOUTH),
	SCENE_EXIT(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_SOUTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_SOUTH),
	SCENE_EXIT(VAULT_CAVE_MOUTH_SOUTH_TO_NORTH, VaultCaveMouth, VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH),
	SCENE_EXIT(VAULT_CAVE_MOUTH_NORTH_TO_SOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_SOUTH_FROM_NORTH),
	SCENE_EXIT(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH),
	SCENE_EXITS_END,
};

ACTOR_PLACE VaultCave_FallbackActorPlacements[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE VaultCavePassage_ActorPlacements[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE VaultCaveVambrace_ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 876, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		184, 0, 892, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		372, 0, 512, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		372, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		388, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		420, 0, 512, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		420, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		452, 0, 528, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		424, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		424, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		456, 0, 536, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, FLAG_VAULT_CAVE_IVY_BLOWN_AWAY, MOVEMENT_TYPE_STAND,
		456, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE VaultCaveMouth_ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 236, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 300, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE VaultCave_FallbackCues[] = {
	{ CUE_TABLE_END },
};

CUE VaultCavePassage_Cues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_MOUTH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_NORTHWEST },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_WEST },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_EAST },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_EAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_SOUTHEAST },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_CHEST },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_CHEST_TO_WATER_ROOM },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT },
	{ CUE_EXIT, 11, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_LADDER_TO_CHEST },
	{ CUE_EXIT, 12, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_CHEST_TO_LADDER },
	{ CUE_EXIT, 13, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_EAST },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_CAVE_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scVaultCaveFlipSwitch) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultCaveShowLockedDoor) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VAULT_CAVE_CHEST, FLAG_VAULT_CAVE_PASSAGE_MIMIC_BEATEN, FIND_MIMIC | 3 },
	{ CUE_TABLE_END },
};

CUE VaultCaveVambrace_Cues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_NORTH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_NORTH },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_NORTH_TO_FROST_ROOM },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_NORTH_TO_WEST },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_WEST_TO_NORTH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTHWEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_SOUTH },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_NORTH },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_SOUTH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTH },
	{ CUE_EXIT, 11, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_SOUTH_TO_FROST_ROOM },
	{ CUE_EXIT, 13, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_CHEST },
	{ CUE_EXIT, 14, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_CHEST_TO_FROST_ROOM },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VAULT_CAVE_CHEST, FLAG_VAULT_CAVE_VAMBRACE_CHEST, FIND_ITEM(ITEM_VAMBRACE) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultCaveShowLockedDoor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHERN_PILLAR, FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH, SCRIPT_REF(scVaultCaveMoveSouthernPillar) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VAULT_CAVE_SOUTHERN_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVaultCavePushSouthernPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WEST_UPPER_PUDDLE, FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN, SCRIPT_REF(scVaultCaveFreezeWestUpperPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WEST_LOWER_PUDDLE, FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN, SCRIPT_REF(scVaultCaveFreezeWestLowerPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_INNER_WEST_LOWER_PUDDLE, FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN, SCRIPT_REF(scVaultCaveFreezeInnerWestLowerPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_EAST_UPPER_PUDDLE, FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN, SCRIPT_REF(scVaultCaveFreezeEastUpperPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_EAST_LOWER_PUDDLE, FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN, SCRIPT_REF(scVaultCaveFreezeEastLowerPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FAR_EAST_PUDDLE, FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN, SCRIPT_REF(scVaultCaveFreezeFarEastPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, FLAG_VAULT_CAVE_IVY_BLOWN_AWAY, SCRIPT_REF(scVaultCaveUseWhirlwind) },
	{ CUE_TABLE_END },
};

CUE VaultCaveMouth_Cues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_REVISIT },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_PASSAGE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_SOUTH },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_SOUTH_TO_NORTH },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_NORTH_TO_SOUTH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_NORTH },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VAULT_CAVE_NORTHERN_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVaultCavePushNorthernPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHERN_PILLAR, FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH, SCRIPT_REF(scVaultCaveMoveNorthernPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scVaultCaveMoveUnlitTorch) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultCaveShowLockedDoor) },
	{ CUE_TABLE_END },
};

/* The frame of the 191-frame beat at which each timed drop falls. */
Uint32 VaultCave_ScheduleFrames[] = {
	20, 80, 115, 170,
};
/* clang-format on */

/* The frame of the drops' 191-frame beat, and the timed drop that falls next
   when it reaches that drop's frame in VaultCave_ScheduleFrames. */
Sint32 VaultCave_ScheduleTimer = 0;
Sint32 VaultCave_ScheduleIndex = 0;

/* The frames counted for the random drops, one picked every 64 frames, and the
   one picked, from the first of the six. */
Sint32 VaultCave_DropFrames = 0;
Sint32 VaultCave_SelectedDrop = 0;
