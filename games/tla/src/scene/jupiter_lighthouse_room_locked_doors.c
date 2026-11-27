/* The level of Jupiter Lighthouse above its central room, with the red and blue
   locked doors that lead out to the two towers. Felix first comes up here by
   the Cyclone teleporter in front of the red door, at metatile (9, 22), and the
   Red Key opens it (scJupiterLighthouseUseRedKey); the teleporter in front of
   the blue door, at metatile (41, 22), only takes him back down
   (scJupiterLighthouseLeaveByRedDoorCyclone,
   scJupiterLighthouseLeaveByBlueDoorCyclone), and the Blue Key opens that door
   (scJupiterLighthouseUseBlueKey). In the room at the back, Reveal shows the
   hidden floor across to the jumping puzzle room, and once the lighthouse is
   activated a hidden block among six hovering ones, the way to the Psy Crystal
   (scJupiterLighthouseRevealThePassage). Further south, the beam of light from
   the central room rises through the floor, lights circling up it
   (scJupiterLighthouseSendUpBeamLights), and brings Felix up among a row of
   hovering blocks; the grey block in the pit west of them, Moved one metatile
   east, rises into line with the row, the way to the Potion's chest
   (scJupiterLighthouseFinishMovingBlock). */

/* This unit declares its own form of fdMapGridGetMetatileBehavior, below. */
#define OWN_fdMapGridGetMetatileBehavior

#include "item_ids.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "field/script_inline.h"
#include "macros.h"
#include "scene/scene.h"
#include "field/call.h"
#include "field/metatile_target.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"
#include "window/window.h"
#include "scene/lighthouse_palettes.h"

/* Set as each locked door opens to its key. */
#define FLAG_RED_DOOR_OPENED	 0xa07
#define FLAG_BLUE_DOOR_OPENED	 0xa08
/* Set once Move has set the bridge block on its metatile, where it rises to
   bridge the gap. */
#define FLAG_BRIDGE_BLOCK_RAISED 0xa06

/* The bridge block's metatile, at the west end of the row of hovering blocks
   round the beam, where it rises into line with them. */
enum { BRIDGE_BLOCK_METATILE_X = 10, BRIDGE_BLOCK_METATILE_Z = 32 };

/* The level's own metatile behaviors. */
typedef enum JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_RED_DOOR = 20,  /* metatile (10, 20) */
	METATILE_BEHAVIOR_BLUE_DOOR = 21, /* metatile (40, 20) */
	/* The teleporter pads in front of the red and the blue door, metatiles (9,
	   22) and (41, 22). */
	METATILE_BEHAVIOR_RED_DOOR_TELEPORTER = 40,
	METATILE_BEHAVIOR_BLUE_DOOR_TELEPORTER
} JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_METATILE_BEHAVIOR;

/* The room's blocks, numbering the rows of ActorPlacements. */
typedef enum JUPITER_LIGHTHOUSE_BLOCK_ID {
	/* The first of the six blocks that rise once the lighthouse is
	   activated. */
	ACTOR_FIRST_BLOCK = ACTOR_FIRST_PLACED,
	/* The block Reveal shows on the hidden passage. */
	ACTOR_HIDDEN_BLOCK = ACTOR_FIRST_PLACED + 6,
	/* The block in the pit west of the beam's row, which Move sets on its
	   metatile to bridge the gap. */
	ACTOR_BRIDGE_BLOCK,
	/* The first of three more that rise once the lighthouse is activated. */
	ACTOR_LAST_BLOCKS,
	/* The unseen block at the top of the beam of light, (232, 520), in the row
	   of hovering blocks: once the lighthouse is activated, Felix carried up
	   the beam stands on it, nothing drawn under him. */
	ACTOR_BEAM_TOP_BLOCK = ACTOR_LAST_BLOCKS + 3
} JUPITER_LIGHTHOUSE_BLOCK_ID;

/* The beam of light from the room below: where it stands, and how far below the
   floor its lights, and the party it carries up, start rising. */
enum { BEAM_X = 232, BEAM_Z = 520, BEAM_DEPTH = 120 };

/* The most steps the beam lifts the party, 3 pixels each; the floor stops it
   first. */
#define BEAM_RISE_MAX_STEPS	 90
/* The step at which a beam light starts to fade, 6 pixels below the floor. */
#define BEAM_LIGHT_FADE_STEP 38

void				   scActorRaiseFromMetatile(Sint32 id);
Sint32 WhirlwindRiding BSS_DATA;
Uint16				   PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours
   after them. */
Uint16					   PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16					   PaletteCycleUnlitColors[14] BSS_DATA;
Sint16 LightSpriteTile	   BSS_DATA;
Sint16 LightSpriteCount	   BSS_DATA;
ACTOR					  *LightSpriteActors[16] BSS_DATA;
LIGHT_OAM				   LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge   BSS_DATA;
Sint16 LightSpritePriority BSS_DATA;
/* The metatiles scCrackedTileStep watches, and the metatile it copies over the
   one the leader steps on. */
METATILE_TARGET CrackedTiles BSS_DATA;
void						 scJupiterLighthouseHideThePassage(void);
void						 scJupiterLighthouseLeaveByRedDoorCyclone(void);
void						 scJupiterLighthouseLeaveByBlueDoorCyclone(void);
void						 scJupiterLighthouseTryLockedDoor(void);
void						 scLightSprsInitA(void);

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

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* Felix casts Reveal in the room at the back: a hidden stretch of floor shows
   at metatile (19, 10), and he can jump across the gap there. Once the
   lighthouse is activated, Reveal also shows a hidden block hovering at
   metatile (31, 10), in the middle of the six hovering blocks, which he can
   jump across too. */
void scJupiterLighthouseRevealThePassage(void)
{
	fdMapGridCopyMetatilesLayered(17, 10, 1, 4, 19, 10);
	fdMapGridCopyMetatileAttributes(17, 10, 1, 1, 19, 10);
	syFieldMapGridSetMetatileBehavior(
		toFIXED(304), toFIXED(160), 0, METATILE_BEHAVIOR_JUMP_FORWARD);
	fdActorSetPos(ACTOR_HIDDEN_BLOCK, 504, 168);
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
		scActorRaiseFromMetatile(ACTOR_HIDDEN_BLOCK);
		syFieldMapGridSetMetatileBehavior(
			toFIXED(496), toFIXED(160), 0, METATILE_BEHAVIOR_JUMP_FORWARD);
	}
}

/* As Reveal wears off, and on entering the level: the hidden floor and the
   hidden block vanish again. */
void scJupiterLighthouseHideThePassage(void)
{
	fdMapGridCopyMetatilesLayered(16, 10, 1, 4, 19, 10);
	fdMapGridCopyMetatileAttributes(16, 10, 1, 1, 19, 10);
	fdActorSetPos(ACTOR_HIDDEN_BLOCK, 0, 0);
	fdMapGridCopyMetatileAttributes(30, 10, 1, 1, 31, 10);
}

/* Nothing visible happens as Move takes hold of the grey block in the pit west
   of the beam's row; its sprite's drawing order is set
   (`fdActorSetSprPriority`) for the slide. */
void scJupiterLighthouseStartMovingBlock(Sint32 unused, Sint32 id)
{
	fdActorSetSprPriority(id, 2);
}

/* The grey block in the pit west of the beam's row, Moved one metatile east to
   metatile (10, 32), rises out of the pit and hovers there for good in line
   with the others, a step for Felix on the way to the Potion's chest. Moved
   anywhere else, it stays in the pit. */
void scJupiterLighthouseFinishMovingBlock(Sint32 unused, Sint32 id)
{
	ACTOR *actor;
	Sint32 i;

	actor = fdActorGet(id);
	if(FIELD_METATILE(actor->x.fixed) == BRIDGE_BLOCK_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == BRIDGE_BLOCK_METATILE_Z) {
		actor->motionFlg = 0;
		for(i = 0; i < 16; i++) {
			actor->y.fixed += toFIXED(2);
			syWait(1);
		}
		scActorRaiseFromMetatile(id);
		fdScriptSetFlag(FLAG_BRIDGE_BLOCK_RAISED);
	}
}

/* Felix casts Cyclone on the purple teleporter pad in front of the red door,
   metatile (9, 22): the party goes back down to the south hall below, by its
   west side. */
void scJupiterLighthouseLeaveByRedDoorCyclone(void)
{
	syImportScriptSetExitRequest(
		JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST);
}

/* Felix casts Cyclone on the purple teleporter pad in front of the blue door,
   metatile (41, 22): the party goes back down to the south hall below, by its
   east side. */
void scJupiterLighthouseLeaveByBlueDoorCyclone(void)
{
	syImportScriptSetExitRequest(
		JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST);
}

/* Shows the red door at metatile (10, 20) open and lets the party through
   it. */
void scJupiterLighthouseOpenRedDoor(void)
{
	fdMapGridCopyMetatilesLayered(73, 18, 1, 4, 74, 18);
	fdMapGridCopyMetatileAttributes(10, 9, 1, 1, 10, 20);
}

/* Shows the blue door at metatile (40, 20) open and lets the party through
   it. */
void scJupiterLighthouseOpenBlueDoor(void)
{
	fdMapGridCopyMetatilesLayered(103, 18, 1, 4, 104, 18);
	fdMapGridCopyMetatileAttributes(40, 9, 1, 1, 40, 20);
}

/* Felix uses the Red Key at the red door, metatile (10, 20), north of the
   teleporter pad: the door opens with a heavy thud and the key is gone. */
void scJupiterLighthouseUseRedKey(Sint32 item)
{
	fdScriptSetFlag(FLAG_RED_DOOR_OPENED);
	fdScriptPlaySnd(SE_HEAVY_IMPACT);
	scJupiterLighthouseOpenRedDoor();
	gmPartyInventoryRemove(item);
}

/* Felix uses the Blue Key at the blue door, metatile (40, 20), north of the
   teleporter pad: the door opens with a heavy thud and the key is gone. */
void scJupiterLighthouseUseBlueKey(Sint32 item)
{
	fdScriptSetFlag(FLAG_BLUE_DOOR_OPENED);
	fdScriptPlaySnd(SE_HEAVY_IMPACT);
	scJupiterLighthouseOpenBlueDoor();
	gmPartyInventoryRemove(item);
}

/* Felix checks the red or the blue door before it is open: "The door's
   locked." */
void scJupiterLighthouseTryLockedDoor(void)
{
	/* As A pressed with nothing ahead does, this asks for the field menu, which
	   the field engine opens once the script is over. */
	FieldStateWork->fieldMenu = 1;
	/* "The door's locked." */
	mnMessageShowCentered(MsgFieldDoorLocked, FIELD_MESSAGE_ZOOM);
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* A light's step up the beam: it circles the beam at its angle (timer) and
   radius and rises 3 pixels with each step (userValue), twinkling from its
   first step and fading near the floor, and starts over once above it; drawn in
   front of the beam while the sine is negative and behind it otherwise. */
void scJupiterLighthouseCircleUpTheBeam(ACTOR *actor)
{
	Sint16 ang = actor->tmr;
	Sint16 height = actor->userValue;

	SET_ACTOR_POSITION(actor,
		syMulFX(syCos(ang), actor->speedLimit) + toFIXED(BEAM_X),
		(height * toFIXED(3)) - toFIXED(BEAM_DEPTH), toFIXED(BEAM_Z));
	if(actor->userValue == 1)
		fdActorSelectAnimation(actor, ANIM_PARTICLE_TWINKLE);
	if(actor->userValue == BEAM_LIGHT_FADE_STEP)
		fdActorSelectAnimation(actor, ANIM_PARTICLE_FADE);
	if(actor->y.fixed > 0)
		actor->userValue = -1;
	if(syMathSin(ang) < 0)
		((FIELD_SPR *)actor->spr)->pri = 1;
	else
		((FIELD_SPR *)actor->spr)->pri = 3;
	actor->tmr += actor->acceleration;
	actor->userValue++;
}

/* Sends seven lights circling up the beam, each at its own radius, speed and
   angle and five steps of rise apart. */
void scJupiterLighthouseSendUpBeamLights(void)
{
	ACTOR *actor;
	Sint32 i;
	Sint32 phase;

	i = 0;
	for(phase = 0; i != 7; i++) {
		actor = (scFieldActorCreate(SPRITE_PARTICLE, 0, 0, 0));
		actor->motionFlg = 0;
		actor->priFlg = 0;
		actor->speedLimit = (syRandom() << 2) + toFIXED(6);
		actor->acceleration = syRandomBelow(2048) + DEGtoANG(45) / 4;
		actor->update = (OBJECT_UPDATE)scJupiterLighthouseCircleUpTheBeam;
		actor->tmr = syRandom();
		actor->userValue = phase;
		fdActorSetSprFlags(actor, 0);
		scFieldActorSetSprCol(actor, SPRITE_COLOR_RBG);
		((FIELD_SPR *)actor->spr)->pri = 1;
		phase += 5;
	}
}

/* Sets up the level as the party enters: the hidden floor hidden, each opened
   door open, the lights sent up the beam, and Felix, come from the central
   room, carried spinning up the beam to land among the hovering blocks, or,
   come by Cyclone from the south hall, arriving in a ring. Returns 0. */
Sint32 scInit(void)
{
	ACTOR *leader;
	Sint32 flag;
	Sint32 i;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scJupiterLighthouseHideThePassage();
	if(fdScriptGetFlag(FLAG_RED_DOOR_OPENED))
		scJupiterLighthouseOpenRedDoor();
	if(fdScriptGetFlag(FLAG_BLUE_DOOR_OPENED))
		scJupiterLighthouseOpenBlueDoor();
	scJupiterLighthouseSendUpBeamLights();
	flag = fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION);
	if(flag == 0 &&
		MainRoutineStateWork.entrance ==
			JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM) {
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		leader->priFlg &= ~ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
		fdActorSetAnimationSpeed(leader, ANIM_STEP_NORMAL * 4);
		leader->motionFlg = flag;
		leader->y.fixed = leader->groundHeight = -toFIXED(BEAM_DEPTH);
		fdScriptStartScreenTransition();
		syFieldScriptWait(10);
		for(i = 0; i < BEAM_RISE_MAX_STEPS && leader->y.fixed < 0; i++) {
			leader->groundHeight = leader->y.fixed =
				leader->y.fixed + toFIXED(3);
			leader->facing += TRIG_TURN * 3 / 32;
			syWait(2);
		}
		leader->groundHeight = leader->y.fixed = 0;
		leader->facing = FACING_SOUTH;
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		leader->velY = toFIXED(3);
		leader->gravity = toFIXED(1) / 2;
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
		syFieldScriptWait(40);
		leader->gravity = toFIXED(1);
		leader->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_AUTOMATIC;
		fdScriptEnd();
	}
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) &&
		MainRoutineStateWork.entrance ==
			JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST)
		fdActorArriveInRing(MainRoutineStateWork.leaderActorId);
	return 0;
}

/* Sets the scene's arrival flags and loads the lighthouse's palettes; lets
   Felix walk onto the blocks, raises them once the lighthouse is activated, and
   puts the bridge block back in line. Returns 0. */
Sint32 scPrepareMap(void)
{
	scSetArrivalFlags(0);
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
		scActorRaiseFromMetatile(ACTOR_FIRST_BLOCK);
		scActorRaiseFromMetatile(ACTOR_FIRST_BLOCK + 1);
		scActorRaiseFromMetatile(ACTOR_FIRST_BLOCK + 2);
		scActorRaiseFromMetatile(ACTOR_FIRST_BLOCK + 3);
		scActorRaiseFromMetatile(ACTOR_FIRST_BLOCK + 4);
		scActorRaiseFromMetatile(ACTOR_FIRST_BLOCK + 5);
		scActorRaiseFromMetatile(ACTOR_LAST_BLOCKS);
		scActorRaiseFromMetatile(ACTOR_LAST_BLOCKS + 1);
		scActorRaiseFromMetatile(ACTOR_LAST_BLOCKS + 2);
		scActorRaiseFromMetatile(ACTOR_BEAM_TOP_BLOCK);
		fdActorGet(ACTOR_BEAM_TOP_BLOCK)->gravity = toFIXED(3);
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_BEAM_TOP_BLOCK), 0);
	if(fdScriptGetFlag(FLAG_BRIDGE_BLOCK_RAISED)) {
		fdActorSetPos(ACTOR_BRIDGE_BLOCK, 168, 520);
		scActorRaiseFromMetatile(ACTOR_BRIDGE_BLOCK);
	}
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

void syGameFlagSetByte(Sint32 flg, Sint32 value);

extern Sint32 PaletteCyclePausesInScripts;

#include "shared/palette_cycle_set_pauses_in_scripts.h"

extern Sint32 PaletteCycleTick;

/* Sets the palette cycle's tick counter. */
static inline void scPaletteCycleSetTick(Sint32 value)
{
	PaletteCycleTick = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_metatile.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern Sint32 WhirlwindEndScript[];
/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);

#include "shared/whirlwind_update.h"

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

/* Unused here: nothing in the room sets it as an actor's update. */
#include "shared/actor_emit_smoke.h"

/* Unused here: the aerie's angels and lid are not in this room. */
#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSprsSetPriority(Sint32 value)
{
	LightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(JupiterLighthouseRoomLockedDoors),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_NORTH),
	SCENE_EXIT(3, JupiterLighthouseRoomLockedDoors, 4),
	SCENE_EXIT(4, JupiterLighthouseRoomLockedDoors, 3),
	SCENE_EXIT(5, JupiterLighthouseRoomLockedDoors, 6),
	SCENE_EXIT(6, JupiterLighthouseRoomLockedDoors, 5),
	SCENE_EXIT(7, JupiterLighthouseRoomLockedDoors, 8),
	SCENE_EXIT(8, JupiterLighthouseRoomLockedDoors, 7),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_WEST, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_WEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_EAST, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_EAST),
	SCENE_EXIT(11, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHEAST),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -8, 0, -8, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 520, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 520, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 520, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, BEAM_X, 0, BEAM_Z, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_WEST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_TERRACES_EAST},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST},
	{CUE_EXAMINE, METATILE_BEHAVIOR_RED_DOOR, FLAG_RED_DOOR_OPENED, SCRIPT_REF(scJupiterLighthouseTryLockedDoor)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_BLUE_DOOR, FLAG_BLUE_DOOR_OPENED, SCRIPT_REF(scJupiterLighthouseTryLockedDoor)},
	{USE_ITEM(ITEM_RED_KEY), METATILE_BEHAVIOR_RED_DOOR, FLAG_RED_DOOR_OPENED, SCRIPT_REF(scJupiterLighthouseUseRedKey)},
	{USE_ITEM(ITEM_BLUE_KEY), METATILE_BEHAVIOR_BLUE_DOOR, FLAG_BLUE_DOOR_OPENED, SCRIPT_REF(scJupiterLighthouseUseBlueKey)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRevealThePassage)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseHideThePassage)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_RED_DOOR_TELEPORTER, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLeaveByRedDoorCyclone)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_BLUE_DOOR_TELEPORTER, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLeaveByBlueDoorCyclone)},
	{CUE_TABLE_END},
};

Sint32 PaletteCycleTick = -1;

Sint32 PaletteCyclePausesInScripts = 1;

Sint32 WhirlwindEndScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};
/* clang-format on */

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

#include "../field/field_effect_move.c"

#undef FLAG_RED_DOOR_OPENED
#undef FLAG_BLUE_DOOR_OPENED
#undef FLAG_BRIDGE_BLOCK_RAISED

void scLeaderCenterInMetatile(void);
void scCrackedTileCrack(METATILE_TARGET *tbl, METATILE_TARGET_ENTRY *entry);

extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void syGameFlagSetByte(Sint32 flg, Sint32 value);

/* Watches the leader's steps over the cracked tiles: the first step on one
   cracks it, a second step after leaving it drops the party through, saving the
   metatile in the flag bytes for the floor below. Returns 1 on a fall. */
Bool scCrackedTileStep(void)
{
	ACTOR			*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32			 x = FIELD_METATILE(actor->x.fixed);
	Sint32			 z = FIELD_METATILE(actor->z.fixed);
	METATILE_TARGET *puzzle = &CrackedTiles;
	METATILE_TARGET_ENTRY *tile = puzzle->entries;
	Sint32				   i;

	for(i = 0; i < puzzle->cnt; i++, tile++) {
		if(tile->x == x && tile->z == z) {
			if(!fdScriptGetFlag(tile->flg)) {
				scCrackedTileCrack(puzzle, tile);
				fdScriptSetFlag(tile->flg);
				puzzle->cur = i;
				break;
			}
			if(i == puzzle->cur)
				break;
			puzzle->cur = i;
			scCrackedTileFall(puzzle, tile);
			syGameFlagSetByte(puzzle->flgByte, x);
			syGameFlagSetByte(puzzle->flgByte + 8, z);
			return TRUE;
		}
	}
	return FALSE;
}

/* On landing below a cracked tile, puts the leader on the metatile the flag
   bytes saved, offset by (dx, dz), and starts the screen transition. */
void scCrackedTilePlaceLanding(Sint32 dx, Sint32 dz, Sint32 transition)
{
	ACTOR			*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_STATE		*event = Ram_HeapSlots->fieldState;
	METATILE_TARGET *state = &CrackedTiles;
	Sint32			 x = ((Sint32)scFieldGameFlagGetByte(state->flgByte));
	Sint32			 z = ((Sint32)scFieldGameFlagGetByte(state->flgByte + 8));

	if(x != 0 || z != 0) {
		ACTOR *camera;

		syGameFlagSetByte(state->flgByte, 0);
		syGameFlagSetByte(state->flgByte + 8, 0);
		actor->x.fixed = FIELD_METATILE_CENTER(x + dx);
		actor->z.fixed = FIELD_METATILE_CENTER(z + dz);
		camera = event->camera;
		camera->x.fixed = actor->x.fixed;
		camera->z.fixed = actor->z.fixed;
		fdMapGridCenterOnFollowedPoint();
		scCrackedTileDropIn();
		FieldStateWork->transition = transition;
	}
}
