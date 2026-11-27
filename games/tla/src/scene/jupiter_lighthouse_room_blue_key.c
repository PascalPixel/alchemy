/* @file */
/* The lower rooms of Jupiter Lighthouse's left tower. Past the entrance with */
/* its stone archer statue pointing east, the easy chest in the back room is a
 */
/* Mimic. Up the right passage, five blue fallen columns, two lying north to */
/* south and three west to east, wall in the Blue Key, which lies on the floor
 */
/* where no one can walk to it: Felix pushes the columns, each sliding as far */
/* as it will go (scJupiterLighthousePushColumn), and gets the key only by */
/* falling onto it from the room above. That room, drawn west of the columns */
/* on the same map, holds the 306 Coins' chest and the Mist Potion's. Its */
/* cracked tiles take one step and give way under the next, dropping the party
 */
/* into the column room below (scJupiterLighthouseStepOnCrackedTile). The lone
 */
/* tile at cell (11, 9), cracked already and over the key, gives way at once, */
/* and can only be reached by dropping in from the cracked floor room further */
/* up. Move pushes its standing pillar one cell west */
/* into the gap in the floor at cell (6, 14), where it sinks level with the */
/* floor for Felix to cross on the way round to the Mist Potion */
/* (scJupiterLighthouseFinishMovingPillar). */

/* This unit declares its own form of fdMapGridGetCellBehavior, below. */
#define OWN_MapGrid_GetCellBehavior

#include "shared/tables.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/call.h"
#include "field/cell_target.h"
#include "macros.h"
#include "field/actor_script.h"

/* Set once Move has dropped the pillar into the gap in the floor; cleared */
/* whenever the party comes into the scene, except back from a battle */
/* (FLAG_KEEP_PARTY_POSITION), so the pillar stands again. */
#define FLAG_PILLAR_IN_GAP 0xa18

/* The rooms' statues, pillar and columns, numbering the rows of */
/* ActorPlacements. */
typedef enum JUPITER_LIGHTHOUSE_LEFT_TOWER_ACTOR_ID {
	ACTOR_ARCHER_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_WINGED_STATUE,
	ACTOR_PILLAR,
	/* The first of the five blue fallen columns round the Blue Key, two lying
	 */
	/* north to south and three west to east. */
	ACTOR_FIRST_COLUMN
} JUPITER_LIGHTHOUSE_LEFT_TOWER_ACTOR_ID;

/* The cell of the gap in the upper room's floor that the pillar drops into. */
enum { PILLAR_GAP_X = 6, PILLAR_GAP_Z = 14 };
/* The room's own cell behaviors. */
typedef enum JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_CELL_BEHAVIOR {
	/* The floor round the blue fallen columns, cells (23, 8) to (37, 15), where
	 */
	/* walking into a column pushes it. */
	CELL_BEHAVIOR_BESIDE_COLUMNS = 30
} JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_CELL_BEHAVIOR;
/* The frames the dropped pillar is given to land. */
#define PILLAR_LANDING_FRAMES 30

struct ACTOR;
s32 scCrackedTileStep(void);
void scCrackedTilePlaceLanding(s32 x, s32 z, s32 effect);
void scJupiterLighthousePushColumn(void);
ACTOR *scPushTargetFindAhead(s32 *direction, s32 *slot, s32 *kind);
extern u16 PushKinds[PUSH_KINDS];
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
/* The cells scCrackedTileStep watches, and the cell it copies over the one the
 */
/* leader steps on. */
CELL_TARGET CrackedTiles BSS_DATA;

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Where a pushed object comes to rest, read by the push that follows. */
typedef struct PUSH_TARGET {
	FIXED x; /* 16.16 fixed-point pixels */
	s32 y;
	s32 z;
	s32 rectangle; /* index into the rectangle table */
	s32 id;
	void *func;
} PUSH_TARGET;

s32 scPushTargetFindStop(PUSH_TARGET *target);
void scPushTargetPush(PUSH_TARGET target);

/* Felix steps on a cracked tile in the room above the columns. The first step
 */
/* only cracks it further; on the second it gives way and the party falls */
/* through into the column room below. */
void scJupiterLighthouseStepOnCrackedTile(void) {
	if(scCrackedTileStep())
		syFieldEventSetExitRequest(THROUGH_CRACKED_TILE);
}

/* Felix walks into one of the blue fallen columns round the Blue Key and */
/* pushes it: it rolls until something stops it (scPushTargetFindStop). */
void scJupiterLighthousePushColumn(void) {
	PUSH_TARGET target;

	if(scPushTargetFindStop(&target))
		scPushTargetPush(target);
}

/* Nothing visible happens as Move takes hold of the standing pillar in the */
/* room above the columns; unlike the other rooms with a pillar to Move, this */
/* one needs no cells changed before the push. */
void scJupiterLighthouseStartMovingPillar(void) {}

/* The standing pillar in the room above the columns, Moved one cell west into
 */
/* the gap in the floor at cell (6, 14), drops in with a heavy thud and sinks */
/* level with the floor, so that Felix can hop across the gap on top of it. */
/* Moved anywhere else, it simply stands where it is put. */
void scJupiterLighthouseFinishMovingPillar(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);
	s32 i;

	if(FIELD_CELL(actor->x.fixed) == PILLAR_GAP_X &&
	   FIELD_CELL(actor->z.fixed) == PILLAR_GAP_Z) {
		fdEventSetFlag(FLAG_PILLAR_IN_GAP);
		syWait(10);
		fdEventPlaySnd(SE_HEAVY_IMPACT);
		i = 0;
		if(actor->y.fixed > actor->groundHeight || actor->velY != 0) {
			do {
				syWait(1);
				i++;
				if(i > PILLAR_LANDING_FRAMES - 1)
					break;
			} while(actor->y.fixed > actor->groundHeight || actor->velY != 0);
		}
		actor->motionFlg = 0;
		fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0, 0);
		fdMapGridSetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
									 MAP_CELL_JUMP);
	}
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

s32 scPushTargetFillRectangle(s32 id);

/* Sets up the rooms as the party enters: the archer statue at three quarters */
/* size, the five columns blocking their cells, the cracked tiles, and the */
/* landing when the party has dropped through a crack, into the column room */
/* from the room above it, or into that room from the cracked floor room. */
/* Returns 0. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdActorGet(ACTOR_ARCHER_STATUE)->scaleX = FIXED_ONE * 3 / 4;
	fdActorGet(ACTOR_ARCHER_STATUE)->scaleY = FIXED_ONE * 3 / 4;
	scFieldActorSetSprPriority(ACTOR_FIRST_COLUMN + 1, 3);
	scFieldActorSetSprPriority(ACTOR_FIRST_COLUMN + 3, 3);
	fdActorDisableById(ACTOR_FIRST_COLUMN);
	fdActorDisableById(ACTOR_FIRST_COLUMN + 1);
	fdActorDisableById(ACTOR_FIRST_COLUMN + 2);
	fdActorDisableById(ACTOR_FIRST_COLUMN + 3);
	fdActorDisableById(ACTOR_FIRST_COLUMN + 4);
	scPushTargetFillRectangle(ACTOR_FIRST_COLUMN);
	scPushTargetFillRectangle(ACTOR_FIRST_COLUMN + 1);
	scPushTargetFillRectangle(ACTOR_FIRST_COLUMN + 2);
	scPushTargetFillRectangle(ACTOR_FIRST_COLUMN + 3);
	scPushTargetFillRectangle(ACTOR_FIRST_COLUMN + 4);
	scCrackedTileFind(CELL_BEHAVIOR_CRACKED_TILE, FLAG_SCENE_TEMP_80,
					  FLAG_AREA_TEMP_80, 0, 1);
	if(MainRoutineStateWork.entrance == THROUGH_CRACKED_TILE)
		scCrackedTilePlaceLanding(
			19, 0,
			SCENE_TRANSITION(TRANSITION_WINDOW,
							 WINDOW_TRANSITION_INTERLACED_WIPE));
	else if(
		MainRoutineStateWork.entrance ==
		JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_NORTH)
		scCrackedTilePlaceLanding(
			-20, -22,
			SCENE_TRANSITION(TRANSITION_WINDOW,
							 WINDOW_TRANSITION_INTERLACED_WIPE));
	return 0;
}

/* Sets the scene's arrival flags, and puts the dropped pillar back in its hole
 */
/* when the party stays in place. Returns 0. */
s32 scPrepareMap(void) {
	scSetArrivalFlags(0);
	if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
		fdEventClearFlag(FLAG_PILLAR_IN_GAP);
	if(fdEventGetFlag(FLAG_PILLAR_IN_GAP)) {
		ACTOR *actor = fdActorGet(ACTOR_PILLAR);

		actor->motionFlg = 0;
		actor->groundHeight = toFIXED(-PILLAR_SINK_DEPTH);
		actor->y.fixed = toFIXED(-PILLAR_SINK_DEPTH);
		fdActorSetPos(ACTOR_PILLAR, 104, 232);
		fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0, 0);
		fdMapGridSetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
									 MAP_CELL_JUMP);
	}
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

void syGameFlagSetByte(s32 flag, s32 value);

extern s32 PaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 PaletteCycleFrame;

/* Sets the palette cycle's frame (PaletteCycleFrame). */
static inline void scPaletteCycleSetFrame(s32 value) {
	PaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 WhirlwindEndScript[];

/* The ROM's callers here are compiled against this form, which returns an s32
 */
/* where the definition returns a u8. */
s32 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer);
#include "shared/whirlwind_update.h"

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

extern s8 PushRectangles[][4];

#include "shared/push_target_fill_rectangle.h"

#include "../field/map_cell_fill.c"

extern s32 PushLeaderSteps[];

static inline s32 scPushTargetCheckCollision(ACTOR *actor, s32 *position) {
	return fdMapGridCheckCollision(actor, position);
}

#include "shared/push_target_find_stop.h"

#include "shared/push_target_find_ahead.h"

void fdActorStopAtPushTarget(ACTOR *actor);

#include "shared/push_target_push.h"

#include "shared/actor_stop_at_push_target.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSprsSetPriority(s32 value) { LightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

/* clang-format off */
#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"
/* clang-format on */

#include "../field/field_effect_move.c"

void fdLeaderCenterInCell(void);
void scCrackedTileCrack(CELL_TARGET *table, CELL_TARGET_ENTRY *entry);

extern const s32 *FieldEffect_DurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY syOamAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];
extern u32 FieldEffect_LongScript[];

/* clang-format off */
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 SceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomBlueKey),
    SCENE_EXIT(1, JupiterLighthouseRoomBlueKey, 2),
    SCENE_EXIT(2, JupiterLighthouseRoomBlueKey, 1),
    SCENE_EXIT(3, JupiterLighthouseRoomBlueKey, 4),
    SCENE_EXIT(4, JupiterLighthouseRoomBlueKey, 3),
    SCENE_EXIT(5, JupiterLighthouseRoomBlueKey, 6),
    SCENE_EXIT(6, JupiterLighthouseRoomBlueKey, 5),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_TERRACES, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH),
    SCENE_EXIT(9, JupiterLighthouseRoomBlueKey, 10),
    SCENE_EXIT(10, JupiterLighthouseRoomBlueKey, 9),
    SCENE_EXIT(11, JupiterLighthouseRoomBlueKey, 12),
    SCENE_EXIT(12, JupiterLighthouseRoomBlueKey, 11),
    SCENE_EXIT(THROUGH_CRACKED_TILE, JupiterLighthouseRoomBlueKey, THROUGH_CRACKED_TILE),
    SCENE_EXITS_END,
};

LATE_DATA ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 203, 0, 904, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 296, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 480, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_TERRACES, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_TERRACES},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BESIDE_COLUMNS, CONDITION_ALWAYS, EVENT(scJupiterLighthousePushColumn)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 1, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 2, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStepOnCrackedTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(scJupiterLighthouseStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(scJupiterLighthouseFinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 PaletteCycleFrame = -1;

LATE_DATA s32 PaletteCyclePausesInEvents = 1;

LATE_DATA s32 WhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
/* clang-format on */

LATE_DATA const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};
