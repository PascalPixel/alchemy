/* @file */
/* The sandy grounds before Jupiter Lighthouse, reached from the world map at */
/* their south edge. The lighthouse's front wall runs across the north, with a
 */
/* door at each end (cells (11, 16) and (27, 16)) that leads into the ground */
/* floor once its gates are opened from within. */
/* */
/* The way round to the west ends in a dead end with a purple teleporter pad */
/* among weeds, cell (7, 21): Cyclone on it lifts the party away to the */
/* basement beneath the lighthouse (scJupiterLighthouseTeleportToBasement). */
/* Weeds also grow in three patches along the path under the front wall, and */
/* Cyclone there uncovers a Mint (scJupiterLighthouseScatterWeedsOverMint). */

/* This unit declares its own form of fdMapGridGetCellBehavior, below. */
#define OWN_MapGrid_GetCellBehavior

#include "scene/event_layer.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "macros.h"
#include "scene/scene.h"
#include "field/cell_target.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_inline.h"

s32 WhirlwindRiding BSS_DATA;
u16 PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours */
/* after them. */
u16 PaletteCycleUnlitPalette[2] BSS_DATA;
u16 PaletteCycleUnlitColors[14] BSS_DATA;
s16 LightSpriteTile BSS_DATA;
s16 LightSpriteCount BSS_DATA;
u32 LightSpriteActors[16] BSS_DATA;
LIGHT_OAM LightSpriteOams[16] BSS_DATA;
s16 LightSpritesLarge BSS_DATA;
s16 LightSpritePriority BSS_DATA;
CELL_TARGET CrackedTiles BSS_DATA;
void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);
void scJupiterLighthouseScatterWeedsSouthOfTheTeleporter(void);
void scJupiterLighthouseScatterWeedsBelowTheStairs(void);
void scJupiterLighthouseScatterEastWeeds(void);
void scJupiterLighthouseScatterWeedsOverMint(void);
void scJupiterLighthouseTeleportToBasement(void);

/* The grounds' map cells that answer Cyclone: the weeds round the teleporter */
/* pad and under the front wall, the pad itself, and the weeds over the Mint. */
enum {
	CELL_WEEDS_NORTH_OF_THE_TELEPORTER = 30,
	CELL_WEEDS_SOUTH_OF_THE_TELEPORTER,
	CELL_WEST_WEEDS,
	CELL_WEEDS_BELOW_THE_STAIRS,
	CELL_EAST_WEEDS,
	CELL_TELEPORTER,
	CELL_WEEDS_OVER_MINT
};

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Cyclone cast in the weeds just north of the teleporter pad in the west dead
 */
/* end (cells (9, 19), (6, 20) and (7, 20)) scatters those within two cells of
 */
/* Felix. */
void scJupiterLighthouseScatterWeedsNorthOfTheTeleporter(void) {
	syFieldTileCopiesCopyTouchedCells(56, 0, 4, 2, 70, 19, 1);
}

/* Cyclone cast in the weeds just south of the teleporter pad in the west dead
 */
/* end (cells (5, 22) to (8, 23)) scatters those within two cells of Felix. */
void scJupiterLighthouseScatterWeedsSouthOfTheTeleporter(void) {
	syFieldTileCopiesCopyTouchedCells(56, 0, 4, 2, 69, 22, 1);
}

/* Cyclone cast in the weeds under the west end of the lighthouse's front */
/* wall (cells (15, 25) to (17, 27)) scatters those within two cells of Felix.
 */
void scJupiterLighthouseScatterWestWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(56, 0, 3, 3, 79, 25, 1);
}

/* Cyclone cast in the weeds below the stairs in the middle of the */
/* lighthouse's front (cells (21, 26) to (25, 28)) scatters those within two */
/* cells of Felix. */
void scJupiterLighthouseScatterWeedsBelowTheStairs(void) {
	syFieldTileCopiesCopyTouchedCells(56, 0, 5, 3, 85, 26, 1);
}

/* Cyclone cast in the weeds under the east part of the lighthouse's front */
/* (cells (30, 26) to (34, 28)) scatters those within two cells of Felix. */
void scJupiterLighthouseScatterEastWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(56, 0, 5, 3, 94, 26, 1);
}

/* Where the Mint lies under the weeds below the west end of the front wall: */
/* cell (17, 27). */
#define MINT_SPOT_X 280
#define MINT_SPOT_Z 440

/* Cyclone cast standing on cell (17, 27), in the weeds under the west end of */
/* the lighthouse's front wall, scatters them and, until it is found, uncovers
 */
/* a Mint under Felix's feet: he jumps a cell west off it, and its green leaf */
/* lies in the cleared sand. */
void scJupiterLighthouseScatterWeedsOverMint(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	syFieldTileCopiesCopyTouchedCells(56, 0, 3, 3, 79, 25, 1);
	if(!fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_MINT)) {
		fdActorSetPos(FIELD_TREASURE_ACTOR, MINT_SPOT_X, MINT_SPOT_Z);
		if(FIELD_CELL(leader->x.fixed) == FIELD_CELL(toFIXED(MINT_SPOT_X)) &&
		   FIELD_CELL(leader->z.fixed) == FIELD_CELL(toFIXED(MINT_SPOT_Z))) {
			fdEventStart();
			syFieldEventEndLastingPsynergy(0);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
									 ATTACHED_EFFECT_IN_FRONT |
										 ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, -16,
										0);
			leader->velY = toFIXED(6);
			scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdEventEnd();
		}
	}
}

/* Cyclone on the purple teleporter pad in the west dead end, cell (7, 21): */
/* the whirlwind lifts Felix up off the screen, and he drops onto the near */
/* teleporter in the cave beneath the lighthouse. */
void scJupiterLighthouseTeleportToBasement(void) {
	syImportEventSetExitRequest(
		JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS);
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Sets up the grounds as the party enters: the fade in, the teleporter's */
/* ring when the party arrives by it, and Contigo's shops as where a defeat */
/* returns to. Returns 0. */
s32 scInit(void) {
	s32 keepPosition;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	keepPosition = fdEventGetFlag(FLAG_KEEP_PARTY_POSITION);
	if(keepPosition == 0) {
		if(MainRoutineStateWork.entrance ==
		   JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS)
			fdActorArriveInRing(MainRoutineStateWork.leaderActorId);
	}
	MainRoutineStateWork.restartScene = SCENE_CONTIGO_SHOPS;
	MainRoutineStateWork.restartEntrance =
		CONTIGO_SHOPS_FROM_CONTIGO_NORTHEAST_A;
	return 0;
}

/* Sets the scene's arrival flags before the map is built. Returns 0. */
s32 scPrepareMap(void) {
	scSetArrivalFlags(0);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 PaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

void syGameFlagSetByte(s32 flag, s32 value);

extern s32 PaletteCycleFrame;

/* Sets the palette cycle's frame (PaletteCycleFrame). */
static inline void scPaletteCycleSetFrame(s32 value) {
	PaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 WhirlwindEndScript[];

/* The ROM's callers here are compiled against this form, which returns an s32
 */
/* where the definition returns a u8. */
s32 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer);
#include "shared/whirlwind_update_lower.h"

#include "shared/whirlwind_spawn.h"

/* Unused: nothing in the entrance sets fdActorEmitSmoke as an update. */
#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Unused: sets the priority the lit actors' sprites are drawn at; nothing in */
/* the entrance calls it. */
UNUSED void scLightSprsSetPriority(s32 value) { LightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(JupiterLighthouseEntrance),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_JUPITER_LIGHTHOUSE_ENTRANCE),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_ENTRANCE_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_ENTRANCE_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, JupiterLighthouseBasementTeleporters, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_ENTRANCE),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 360, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_WORLD_MAP, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_NORTH_OF_THE_TELEPORTER, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterWeedsNorthOfTheTeleporter)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_SOUTH_OF_THE_TELEPORTER, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterWeedsSouthOfTheTeleporter)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEST_WEEDS, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterWestWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_BELOW_THE_STAIRS, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterWeedsBelowTheStairs)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_EAST_WEEDS, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterEastWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_TELEPORTER, CONDITION_ALWAYS, EVENT(scJupiterLighthouseTeleportToBasement)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_OVER_MINT, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterWeedsOverMint)},
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

void scCrackedTileCrack();
void scCrackedTileFall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
