/* @file */
/* Rooms of Jupiter Lighthouse just below its aerie, on one map. Their stairs */
/* lead up to the aerie (later to the beacon's floor) and down to the rooms of
 */
/* the blue key and the red key. Standing on the button in the room of cracked
 */
/* tiles holds a stone door open; the pillar beside it, moved onto it */
/* (scJupiterLighthouseCheckPillarOnButton), keeps it open, a shortcut through
 */
/* the tower. The room's cracked tiles give way under a second step */
/* (scJupiterLighthouseStepOnCrackedTile), dropping the party into the hall of
 */
/* the four winged statues below, where a single cracked tile in a ring of */
/* studs (scJupiterLighthouseStepOnSingleCrackedTile) drops it into the room of
 */
/* the blue key. */

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
#include "scene/lighthouse_palettes.h"

/* The ROM's callers here are compiled with no declaration in scope, as against
 */
/* this form: no parameter list and an s32 result, where the definition takes */
/* three s32 values and returns a u8. */
s32 fdMapGridGetCellBehavior();

/* Set while the leader stands on the button by the door, which holds it open.
 */
#define FLAG_DOOR_BUTTON_HELD FLAG_SCENE_TEMP_00

/* The pillar the party pushes onto the button by the door, after the four */
/* winged statues in ActorPlacements. */
enum { ACTOR_SHORTCUT_PILLAR = ACTOR_FIRST_PLACED + 4 };

/* The button's cell, which the pillar is moved onto. */
enum { DOOR_BUTTON_CELL_X = 4, DOOR_BUTTON_CELL_Z = 37 };

/* The step triggers of the button: its cell, and the three cells round it */
/* that the leader steps off it onto. */
enum { TRIGGER_DOOR_BUTTON = 20, TRIGGER_OFF_DOOR_BUTTON = 21 };

/* The cell behavior of the single cracked tile in the hall below, which takes
 */
/* the next one up as it cracks; the room's own are CELL_BEHAVIOR_CRACKED_TILE.
 */
enum { CELL_BEHAVIOR_SINGLE_CRACKED_TILE = 43 };

/* The entrances to the room of cracked tiles: up the stairs from the hall */
/* below, and down the stairs from the room above. */
enum {
	ENTRANCE_CRACKED_TILES_FROM_BELOW = 5,
	ENTRANCE_CRACKED_TILES_FROM_ABOVE
};

/* How scJupiterLighthouseOpenDoor opens the door: at once, as the room is set
 */
/* up, or grinding open with a sound. */
enum { DOOR_OPEN_AT_ONCE, DOOR_GRINDS_OPEN };

void scJupiterLighthouseOpenDoor(s32 how);
void scJupiterLighthouseCheckPillarOnButton(void);
void scCrackedTilePlaceLanding(s32 x, s32 z, s32 effect);
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
void scJupiterLighthouseStepOffButton(void);
void scJupiterLighthouseFinishMovingBlock(void);

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

extern s32 PaletteCyclePausesInEvents;
void scJupiterLighthouseCloseDoor(void);

/* A cracked tile of the room under a grounded leader: as it gives way the */
/* party falls to the hall below. */
void scJupiterLighthouseStepOnCrackedTile(s32 value) {
	if(fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed == 0 &&
	   scCrackedTileStep(value))
		syFieldEventSetExitRequest(THROUGH_CRACKED_TILE);
}

/* The single cracked tile of the hall below under a grounded leader: as it */
/* gives way the party falls into the room of the blue key. */
void scJupiterLighthouseStepOnSingleCrackedTile(s32 value) {
	if(fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed == 0 &&
	   scCrackedTileStep(value))
		syFieldEventSetExitRequest(
			JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_NORTH);
}

/* Opens the door, at once or grinding open through a frame between shut and */
/* open. */
void scJupiterLighthouseOpenDoor(s32 how) {
	if(how == DOOR_GRINDS_OPEN) {
		fdEventPlaySnd(SE_IMPACT);
		fdMapGridCopyCellsLayered(84, 36, 2, 3, 78, 40);
		syWait(6);
	}
	fdMapGridCopyCellsLayered(84, 39, 2, 3, 78, 40);
	fdMapGridCopyCellAttributes(20, 38, 2, 1, 14, 42);
}

/* Closes the door with a thud. */
void scJupiterLighthouseCloseDoor(void) {
	fdEventPlaySnd(SE_HEAVY_IMPACT);
	fdMapGridCopyCellsLayered(84, 36, 2, 3, 78, 40);
	syWait(6);
	fdMapGridCopyCellsLayered(84, 42, 2, 3, 78, 40);
	fdMapGridCopyCellAttributes(20, 44, 2, 1, 14, 42);
}

/* The shortcut pillar moved onto the button holds the door open for good. */
void scJupiterLighthouseCheckPillarOnButton(void) {
	ACTOR *actor = fdActorGet(ACTOR_SHORTCUT_PILLAR);

	if(!fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED) &&
	   FIELD_CELL(actor->x.fixed) == DOOR_BUTTON_CELL_X &&
	   FIELD_CELL(actor->z.fixed) == DOOR_BUTTON_CELL_Z) {
		fdEventSetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED);
		scJupiterLighthouseOpenDoor(DOOR_GRINDS_OPEN);
	}
}

/* Stepping on the button opens the door. */
void scJupiterLighthouseStepOnButton(void) {
	if(!fdEventGetFlag(FLAG_DOOR_BUTTON_HELD)) {
		fdEventSetFlag(FLAG_DOOR_BUTTON_HELD);
		scJupiterLighthouseOpenDoor(DOOR_GRINDS_OPEN);
	}
}

/* Stepping off the button closes the door, unless the shortcut pillar now */
/* holds it. */
void scJupiterLighthouseStepOffButton(void) {
	if(fdEventGetFlag(FLAG_DOOR_BUTTON_HELD) == 1) {
		fdEventClearFlag(FLAG_DOOR_BUTTON_HELD);
		scJupiterLighthouseCloseDoor();
	}
	scJupiterLighthouseCheckPillarOnButton();
}

#include "shared/jupiter_lighthouse_finish_moving_block.c"

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

extern s32 PaletteCycleFrame;

/* Sets the palette cycle's frame (PaletteCycleFrame). */
static inline void scPaletteCycleSetFrame(s32 value) {
	PaletteCycleFrame = value;
}

/* Sets up the rooms as the party enters: the window in, the door as the */
/* pillar or the button leaves it, and the cracked tiles of the room entered: */
/* the room of cracked tiles by its stairs, or else the hall below, with the */
/* landing of a fall from above. Returns 0. */
s32 scInit(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED)) {
		fdActorSetPos(ACTOR_SHORTCUT_PILLAR, 72, 600);
		scJupiterLighthouseOpenDoor(DOOR_OPEN_AT_ONCE);
	}
	if(fdEventGetFlag(FLAG_DOOR_BUTTON_HELD))
		scJupiterLighthouseOpenDoor(DOOR_OPEN_AT_ONCE);
	if(MainRoutineStateWork.entrance >= ENTRANCE_CRACKED_TILES_FROM_BELOW &&
	   MainRoutineStateWork.entrance <= ENTRANCE_CRACKED_TILES_FROM_ABOVE) {
		scCrackedTileFind(CELL_BEHAVIOR_CRACKED_TILE, FLAG_SCENE_TEMP_80,
						  FLAG_AREA_TEMP_80, 20, 31);
	}
	else {
		scCrackedTileFind(CELL_BEHAVIOR_SINGLE_CRACKED_TILE, FLAG_SCENE_TEMP_80,
						  FLAG_AREA_TEMP_80, 21, 31);
		scCrackedTilePlaceLanding(
			21, -3,
			SCENE_TRANSITION(TRANSITION_WINDOW,
							 WINDOW_TRANSITION_INTERLACED_WIPE));
	}
	return 0;
}

/* Sets the scene's arrival flags and loads the lighthouse's palettes; the */
/* pillar goes back unless the party stays in place. Returns 0. */
s32 scPrepareMap(void) {
	scSetArrivalFlags(0);
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION))
		fdEventClearFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

void syGameFlagSetByte(s32 flag, s32 value);

#include "shared/palette_cycle_set_pauses_in_events.h"

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

/* Unused: holds an actor 32 pixels above its ground, without gravity, its cell
 */
/* raised 6 steps over the terrain and made a gap to jump. */
UNUSED void scJupiterLighthouseFloatAboveGround(s32 id) {
	ACTOR *actor;
	s32 height;

	actor = fdActorGet(id);
	actor->motionFlg = ACTOR_FLOATS;
	height = actor->groundHeight;
	actor->bounce = 0;
	actor->y.fixed = height + toFIXED(32);
	actor->priFlg |= ACTOR_PRIORITY_PLATFORM;
	height =
		fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
						   CELL_HEIGHT_STEPS(height) + 6);
	fdMapGridSetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
								 MAP_CELL_JUMP);
}

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 WhirlwindEndScript[];

#include "shared/whirlwind_update.h"

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSprsSetPriority(u16 value) { LightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomCrackedFloor),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_WEST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST),
    SCENE_EXIT(4, JupiterLighthouseRoomCrackedFloor, 5),
    SCENE_EXIT(5, JupiterLighthouseRoomCrackedFloor, 4),
    SCENE_EXIT(6, JupiterLighthouseRoomCrackedFloor, 7),
    SCENE_EXIT(7, JupiterLighthouseRoomCrackedFloor, 6),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST),
    SCENE_EXIT(11, JupiterLighthouseRoomCrackedFloor, 12),
    SCENE_EXIT(12, JupiterLighthouseRoomCrackedFloor, 11),
    SCENE_EXIT(THROUGH_CRACKED_TILE, JupiterLighthouseRoomCrackedFloor, THROUGH_CRACKED_TILE),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_NORTH, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_NORTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_A, JupiterLighthouseAerieBeacon, JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_B, JupiterLighthouseAerieBeacon, JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 456, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 632, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 632, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 264, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 264, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
    /* Once Isaac and Ivan have fallen, the stairs up lead to the beacon's floor. */
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_A},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_B},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, TRIGGER_DOOR_BUTTON, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(scJupiterLighthouseStepOnButton)},
    {TRIGGER_STEP, TRIGGER_OFF_DOOR_BUTTON, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(scJupiterLighthouseStepOffButton)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 1, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 2, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SINGLE_CRACKED_TILE, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnSingleCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SINGLE_CRACKED_TILE + 1, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnSingleCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SINGLE_CRACKED_TILE + 2, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnSingleCrackedTile)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SHORTCUT_PILLAR, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(scJupiterLighthouseFinishMovingBlock)},
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

#undef FLAG_DOOR_BUTTON_HELD

extern const s32 *FieldEffect_DurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY syOamAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

struct CELL_TARGET_ENTRY;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void scCrackedTileCrack(CELL_TARGET *state, CELL_TARGET_ENTRY *entry);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
