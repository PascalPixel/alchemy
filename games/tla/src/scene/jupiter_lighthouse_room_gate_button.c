/* @file */
/* The ground floor of Jupiter Lighthouse, inside its main gates. The party */
/* first comes in from below, rising out of the basement's Cyclone teleporter.
 */
/* Two sandstone gates close the passage north and south of a purple button at
 */
/* cell (17, 33). Standing on the button holds them open */
/* (scJupiterLighthouseStepOnButton), and they slide shut as Felix steps off */
/* (scJupiterLighthouseStepOffButton); the stone pillar west of it, pushed or */
/* Moved three cells east onto the button, opens them for good */
/* (scJupiterLighthouseCheckPillarOnButton), so the way in from the */
/* lighthouse's entrance stays open. Cyclone on the teleporter pad at cell (29,
 */
/* 47) takes the party back down (scJupiterLighthouseLeaveByCyclone). */

/* This unit declares its own form of fdMapGridGetCellBehavior, below. */
#define OWN_MapGrid_GetCellBehavior

#include "field/field_sequence.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "field/cell_target.h"

/* Set while Felix stands on the button by the gates, which holds them open. */
#define FLAG_GATES_BUTTON_HELD FLAG_SCENE_TEMP_00

/* The stone pillar Felix pushes onto the button by the gates, after the two */
/* winged statues in ActorPlacements. */
enum { ACTOR_GATES_PILLAR = ACTOR_FIRST_PLACED + 2 };

/* The button's cell, which the pillar is moved onto. */
enum { GATES_BUTTON_CELL_X = 17, GATES_BUTTON_CELL_Z = 33 };

/* The step triggers of the button: its cell, and the eight cells round it */
/* that Felix steps off it onto. */
enum { TRIGGER_GATES_BUTTON = 20, TRIGGER_OFF_GATES_BUTTON = 21 };
/* The teleporter pad's cell behavior, at cell (29, 47). */
enum { CELL_BEHAVIOR_CYCLONE_TELEPORTER = 30 };

struct ACTOR;
struct LIGHT_OAM;
s32 WhirlwindRiding BSS_DATA;
u16 PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours */
/* after them. */
u16 PaletteCycleUnlitPalette[2] BSS_DATA;
u16 PaletteCycleUnlitColors[14] BSS_DATA;
s16 LightSpriteTile BSS_DATA;
s16 LightSpriteCount BSS_DATA;
ACTOR *LightSpriteActors[16] BSS_DATA;
LIGHT_OAM LightSpriteOams[16] BSS_DATA;
s16 LightSpritesLarge BSS_DATA;
s16 LightSpritePriority BSS_DATA;
CELL_TARGET CrackedTiles BSS_DATA;
/* The ROM's callers here are compiled against this form, which returns an s32
 */
/* where the definition returns a u8. */
s32 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer);
void scJupiterLighthouseStepOnButton(void);
void scJupiterLighthouseStepOffButton(void);

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Felix casts Cyclone on the purple teleporter pad, cell (29, 47): the */
/* whirlwind takes the party back down to the basement's teleporters. */
void scJupiterLighthouseLeaveByCyclone(void) {
	syFieldEventSetExitRequest(
		JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS);
}

/* How scJupiterLighthouseOpenGates opens the gates. */
enum { GATES_STAND_OPEN, GATES_SLIDE_OPEN };

/* Opens both sandstone gates: GATES_SLIDE_OPEN has them thud and slide open */
/* through their half-open picture, GATES_STAND_OPEN simply shows them open. */
void scJupiterLighthouseOpenGates(s32 how) {
	if(how == GATES_SLIDE_OPEN) {
		fdEventPlaySnd(SE_IMPACT);
		fdMapGridCopyCellsLayered(81, 20, 2, 3, 79, 29);
		fdMapGridCopyCellsLayered(81, 20, 2, 3, 79, 37);
		syWait(6);
	}
	fdMapGridCopyCellsLayered(84, 20, 2, 3, 79, 29);
	fdMapGridCopyCellsLayered(84, 20, 2, 3, 79, 37);
	fdMapGridCopyCellAttributes(20, 22, 2, 1, 15, 31);
	fdMapGridCopyCellAttributes(20, 22, 2, 1, 15, 39);
}

/* Slides both sandstone gates shut with a heavy thud, through their half-open
 */
/* picture. */
void scJupiterLighthouseCloseGates(void) {
	fdEventPlaySnd(SE_HEAVY_IMPACT);
	fdMapGridCopyCellsLayered(81, 20, 2, 3, 79, 29);
	fdMapGridCopyCellsLayered(81, 20, 2, 3, 79, 37);
	syWait(6);
	fdMapGridCopyCellsLayered(87, 20, 2, 3, 79, 29);
	fdMapGridCopyCellsLayered(87, 20, 2, 3, 79, 37);
	fdMapGridCopyCellAttributes(23, 22, 2, 1, 15, 31);
	fdMapGridCopyCellAttributes(23, 22, 2, 1, 15, 39);
}

/* Opens the gates for good once the stone pillar stands on the button. */
void scJupiterLighthouseCheckPillarOnButton(void) {
	ACTOR *actor = fdActorGet(ACTOR_GATES_PILLAR);

	if(!fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED) &&
	   FIELD_CELL(actor->x.fixed) == GATES_BUTTON_CELL_X &&
	   FIELD_CELL(actor->z.fixed) == GATES_BUTTON_CELL_Z) {
		fdEventSetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED);
		scJupiterLighthouseOpenGates(GATES_SLIDE_OPEN);
	}
}

/* Felix steps onto the purple button between the gates, cell (17, 33): both */
/* gates slide open with a thud, and stay open while he stands on it. */
void scJupiterLighthouseStepOnButton(void) {
	if(!fdEventGetFlag(FLAG_GATES_BUTTON_HELD)) {
		fdEventSetFlag(FLAG_GATES_BUTTON_HELD);
		scJupiterLighthouseOpenGates(GATES_SLIDE_OPEN);
	}
}

/* Felix steps off the purple button onto one of the cells round it: the gates
 */
/* he held open slide shut with a heavy thud. If the stone pillar has just been
 */
/* pushed onto the button, they open for good instead. */
void scJupiterLighthouseStepOffButton(void) {
	if(fdEventGetFlag(FLAG_GATES_BUTTON_HELD) == 1) {
		fdEventClearFlag(FLAG_GATES_BUTTON_HELD);
		scJupiterLighthouseCloseGates();
	}
	scJupiterLighthouseCheckPillarOnButton();
}

#include "shared/jupiter_lighthouse_finish_moving_block.c"

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Sets up the floor: Felix arriving by the teleporter comes up in its ring of
 */
/* light, and the gates stand open while the pillar or Felix holds the button.
 */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) &&
	   MainRoutineStateWork.entrance ==
		   JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS)
		fdActorArriveInRing(MainRoutineStateWork.leaderActorId);
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED)) {
		fdActorSetPos(ACTOR_GATES_PILLAR, 280, 536);
		scJupiterLighthouseOpenGates(GATES_STAND_OPEN);
	}
	if(fdEventGetFlag(FLAG_GATES_BUTTON_HELD))
		scJupiterLighthouseOpenGates(GATES_STAND_OPEN);
	return 0;
}

/* Sets the scene's arrival flags before the map is built. Returns 0. */
s32 scPrepareMap(void) {
	scSetArrivalFlags(0);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

void syGameFlagSetByte(s32 flag, s32 value);

extern s32 PaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 PaletteCycleFrame;

/* Sets the palette cycle's frame counter. */
static inline void scPaletteCycleSetFrame(s32 value) {
	PaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 WhirlwindEndScript[];

s32 scPosFlatDistance(FIXED *from, FIXED *to);

#include "shared/whirlwind_update_lower.h"

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
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
void scLightSprsSetPriority(s32 value) { LightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
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

TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_EAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_WEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, TRIGGER_GATES_BUTTON, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(scJupiterLighthouseStepOnButton)},
    {TRIGGER_STEP, TRIGGER_OFF_GATES_BUTTON, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(scJupiterLighthouseStepOffButton)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_CYCLONE_TELEPORTER, CONDITION_ALWAYS, EVENT(scJupiterLighthouseLeaveByCyclone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATES_PILLAR, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(scJupiterLighthouseFinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

s32 PaletteCycleFrame = -1;

s32 PaletteCyclePausesInEvents = 1;

s32 WhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
/* clang-format on */

const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

#include "../field/field_effect_move.c"

#undef FLAG_GATES_BUTTON_HELD

extern const s32 *FieldEffect_DurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY syOamAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

/* Unused: the cracked tiles' functions, found, cracked and fallen through; this
 */
/* room has no cracked tiles, and nothing here calls them. */
#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

struct CELL_TARGET_ENTRY;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void scCrackedTileCrack();
void scCrackedTileFall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
