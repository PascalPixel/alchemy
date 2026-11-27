/* The ground floor of Jupiter Lighthouse, inside its main gates. The party
   first comes in from below, rising out of the basement's Cyclone teleporter.
   Two sandstone gates close the passage north and south of a purple button at
   metatile (17, 33). Standing on the button holds them open
   (scJupiterLighthouseStepOnButton), and they slide shut as Felix steps off
   (scJupiterLighthouseStepOffButton); the stone pillar west of it, pushed or
   Moved three metatiles east onto the button, opens them for good
   (scJupiterLighthouseCheckPillarOnButton), so the way in from the lighthouse's
   entrance stays open. Cyclone on the teleporter pad at metatile (29, 47) takes
   the party back down (scJupiterLighthouseLeaveByCyclone). */

/* This unit declares its own form of fdMapGridGetMetatileBehavior, below. */
#define OWN_fdMapGridGetMetatileBehavior

#include "field/field_mode.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "field/metatile_target.h"

/* Set while Felix stands on the button by the gates, which holds them open. */
#define FLAG_GATES_BUTTON_HELD FLAG_SCENE_TEMP_00

/* The stone pillar Felix pushes onto the button by the gates, after the two
   winged statues in ActorPlacements. */
enum { ACTOR_GATES_PILLAR = ACTOR_FIRST_PLACED + 2 };

/* The button's metatile, which the pillar is moved onto. */
enum { GATES_BUTTON_METATILE_X = 17, GATES_BUTTON_METATILE_Z = 33 };

/* The step cues of the button: its metatile, and the eight metatiles round it
   that Felix steps off it onto. */
enum { CUE_GATES_BUTTON = 20, CUE_OFF_GATES_BUTTON = 21 };

/* The teleporter pad's metatile behavior, at metatile (29, 47). */
enum { METATILE_BEHAVIOR_CYCLONE_TELEPORTER = 30 };

struct ACTOR;
struct LIGHT_OAM;
Sint32 WhirlwindRiding BSS_DATA;
Uint16				   PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours
   after them. */
Uint16						 PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16						 PaletteCycleUnlitColors[14] BSS_DATA;
Sint16 LightSpriteTile		 BSS_DATA;
Sint16 LightSpriteCount		 BSS_DATA;
ACTOR						*LightSpriteActors[16] BSS_DATA;
LIGHT_OAM					 LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge	 BSS_DATA;
Sint16 LightSpritePriority	 BSS_DATA;
METATILE_TARGET CrackedTiles BSS_DATA;
/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);
void   scJupiterLighthouseStepOnButton(void);
void   scJupiterLighthouseStepOffButton(void);

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

/* Felix casts Cyclone on the purple teleporter pad, metatile (29, 47): the
   whirlwind takes the party back down to the basement's teleporters. */
void scJupiterLighthouseLeaveByCyclone(void)
{
	syFieldScriptSetExitRequest(
		JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS);
}

/* How scJupiterLighthouseOpenGates opens the gates. */
enum { GATES_STAND_OPEN, GATES_SLIDE_OPEN };

/* Opens both sandstone gates: GATES_SLIDE_OPEN has them thud and slide open
   through their half-open picture, GATES_STAND_OPEN simply shows them open. */
void scJupiterLighthouseOpenGates(Sint32 how)
{
	if(how == GATES_SLIDE_OPEN) {
		fdScriptPlaySound(SE_IMPACT);
		fdMapGridCopyMetatilesLayered(81, 20, 2, 3, 79, 29);
		fdMapGridCopyMetatilesLayered(81, 20, 2, 3, 79, 37);
		syWait(6);
	}
	fdMapGridCopyMetatilesLayered(84, 20, 2, 3, 79, 29);
	fdMapGridCopyMetatilesLayered(84, 20, 2, 3, 79, 37);
	fdMapGridCopyMetatileAttributes(20, 22, 2, 1, 15, 31);
	fdMapGridCopyMetatileAttributes(20, 22, 2, 1, 15, 39);
}

/* Slides both sandstone gates shut with a heavy thud, through their half-open
   picture. */
void scJupiterLighthouseCloseGates(void)
{
	fdScriptPlaySound(SE_HEAVY_IMPACT);
	fdMapGridCopyMetatilesLayered(81, 20, 2, 3, 79, 29);
	fdMapGridCopyMetatilesLayered(81, 20, 2, 3, 79, 37);
	syWait(6);
	fdMapGridCopyMetatilesLayered(87, 20, 2, 3, 79, 29);
	fdMapGridCopyMetatilesLayered(87, 20, 2, 3, 79, 37);
	fdMapGridCopyMetatileAttributes(23, 22, 2, 1, 15, 31);
	fdMapGridCopyMetatileAttributes(23, 22, 2, 1, 15, 39);
}

/* Opens the gates for good once the stone pillar stands on the button. */
void scJupiterLighthouseCheckPillarOnButton(void)
{
	ACTOR *actor = fdActorGet(ACTOR_GATES_PILLAR);

	if(!fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED) &&
		FIELD_METATILE(actor->x.fixed) == GATES_BUTTON_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == GATES_BUTTON_METATILE_Z) {
		fdScriptSetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED);
		scJupiterLighthouseOpenGates(GATES_SLIDE_OPEN);
	}
}

/* Felix steps onto the purple button between the gates, metatile (17, 33): both
   gates slide open with a thud, and stay open while he stands on it. */
void scJupiterLighthouseStepOnButton(void)
{
	if(!fdScriptGetFlag(FLAG_GATES_BUTTON_HELD)) {
		fdScriptSetFlag(FLAG_GATES_BUTTON_HELD);
		scJupiterLighthouseOpenGates(GATES_SLIDE_OPEN);
	}
}

/* Felix steps off the purple button onto one of the metatiles round it: the
   gates he held open slide shut with a heavy thud. If the stone pillar has just
   been pushed onto the button, they open for good instead. */
void scJupiterLighthouseStepOffButton(void)
{
	if(fdScriptGetFlag(FLAG_GATES_BUTTON_HELD) == 1) {
		fdScriptClearFlag(FLAG_GATES_BUTTON_HELD);
		scJupiterLighthouseCloseGates();
	}
	scJupiterLighthouseCheckPillarOnButton();
}

#include "shared/jupiter_lighthouse_finish_moving_block.c"

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Sets up the floor: Felix arriving by the teleporter comes up in its ring of
   light, and the gates stand open while the pillar or Felix holds the
   button. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) &&
		MainRoutineStateWork.entrance ==
			JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS)
		fdActorArriveInRing(MainRoutineStateWork.leaderActorId);
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED)) {
		fdActorSetPos(ACTOR_GATES_PILLAR, 280, 536);
		scJupiterLighthouseOpenGates(GATES_STAND_OPEN);
	}
	if(fdScriptGetFlag(FLAG_GATES_BUTTON_HELD))
		scJupiterLighthouseOpenGates(GATES_STAND_OPEN);
	return 0;
}

/* Sets the scene's arrival flags before the map is built. Returns 0. */
Sint32 scPrepareMap(void)
{
	scSetArrivalFlags(0);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

void syGameFlagSetByte(Sint32 flag, Sint32 value);

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

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern Sint32 WhirlwindEndScript[];

Sint32 scPosFlatDistance(FIXED *from, FIXED *to);

#include "shared/whirlwind_update_lower.h"

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velocityX,
	Sint32 velocityY, Sint32 velocityZ, Uint32 flags,
	const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke_b.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSpritesSetPriority(Sint32 value)
{
	LightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(JupiterLighthouseRoomGateButton),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_EAST, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_WEST, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST),
	SCENE_EXIT(4, JupiterLighthouseRoomGateButton, 5),
	SCENE_EXIT(5, JupiterLighthouseRoomGateButton, 4),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, JupiterLighthouseBasementTeleporters, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 744, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 744, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 536, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_EAST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_WEST},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_STEP, CUE_GATES_BUTTON, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, SCRIPT_REF(scJupiterLighthouseStepOnButton)},
	{CUE_STEP, CUE_OFF_GATES_BUTTON, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, SCRIPT_REF(scJupiterLighthouseStepOffButton)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_CYCLONE_TELEPORTER, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLeaveByCyclone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATES_PILLAR, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, SCRIPT_REF(scJupiterLighthouseFinishMovingBlock)},
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

#undef FLAG_GATES_BUTTON_HELD

extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSpriteAtDepthClamped
#include "../field/cracked_tile_mask.c"

/* Unused: the cracked tiles' functions, found, cracked and fallen through; this
   room has no cracked tiles, and nothing here calls them. */
#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

struct METATILE_TARGET_ENTRY;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void scCrackedTileCrack();
void scCrackedTileFall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
