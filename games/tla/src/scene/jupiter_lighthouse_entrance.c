// The sandy grounds before Jupiter Lighthouse, reached from the world map at their south edge. The
// lighthouse's front wall runs across the north, with a door at each end (metatiles (11, 16) and
// (27, 16)) that leads into the ground floor once its gates are opened from within.
//
// The way round to the west ends in a dead end with a purple teleporter pad among weeds, metatile
// (7, 21): Cyclone on it lifts the party away to the basement beneath the lighthouse
// (scJupiterLighthouseTeleportToBasement). Weeds also grow in three patches along the path under
// the front wall, and Cyclone there uncovers a Mint (scJupiterLighthouseScatterWeedsOverMint).

// This unit declares its own form of fdMapGridGetMetatileBehavior, below.
#define OWN_fdMapGridGetMetatileBehavior

#include "scene/script_layer.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "macros.h"
#include "scene/scene.h"
#include "field/metatile_target.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/script_inline.h"

Sint32 WhirlwindRiding       BSS_DATA;
Uint16                       PaletteCycleColors[16] BSS_DATA;
// Palette 13's first two colours; it is copied whole, with the unlit colours after them.
Uint16                       PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16                       PaletteCycleUnlitColors[14] BSS_DATA;
Sint16 LightSpriteTile       BSS_DATA;
Sint16 LightSpriteCount      BSS_DATA;
Uint32                       LightSpriteActors[16] BSS_DATA;
LIGHT_OAM                    LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge     BSS_DATA;
Sint16 LightSpritePriority   BSS_DATA;
METATILE_TARGET CrackedTiles BSS_DATA;
void                         scFieldEffectSpawn(Sint32                x,
                                                Sint32                y,
                                                Sint32                z,
                                                Sint32                velocityX,
                                                Sint32                velocityY,
                                                Sint32                velocityZ,
                                                Uint32                flags,
                                                const EFFECT_OPTIONS *options);
void                         scJupiterLighthouseScatterWeedsSouthOfTheTeleporter(void);
void                         scJupiterLighthouseScatterWeedsBelowTheStairs(void);
void                         scJupiterLighthouseScatterEastWeeds(void);
void                         scJupiterLighthouseScatterWeedsOverMint(void);
void                         scJupiterLighthouseTeleportToBasement(void);

// The grounds' map metatiles that answer Cyclone: the weeds round the teleporter pad and under the
// front wall, the pad itself, and the weeds over the Mint.
enum
{
	METATILE_WEEDS_NORTH_OF_THE_TELEPORTER = 30,
	METATILE_WEEDS_SOUTH_OF_THE_TELEPORTER,
	METATILE_WEST_WEEDS,
	METATILE_WEEDS_BELOW_THE_STAIRS,
	METATILE_EAST_WEEDS,
	METATILE_TELEPORTER,
	METATILE_WEEDS_OVER_MINT
};

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

// Cyclone cast in the weeds just north of the teleporter pad in the west dead end (metatiles (9,
// 19), (6, 20) and (7, 20)) scatters those within two metatiles of Felix.
void scJupiterLighthouseScatterWeedsNorthOfTheTeleporter(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(56, 0, 4, 2, 70, 19, 1);
}

// Cyclone cast in the weeds just south of the teleporter pad in the west dead end (metatiles (5,
// 22) to (8, 23)) scatters those within two metatiles of Felix.
void scJupiterLighthouseScatterWeedsSouthOfTheTeleporter(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(56, 0, 4, 2, 69, 22, 1);
}

// Cyclone cast in the weeds under the west end of the lighthouse's front wall (metatiles (15, 25)
// to (17, 27)) scatters those within two metatiles of Felix.
void scJupiterLighthouseScatterWestWeeds(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(56, 0, 3, 3, 79, 25, 1);
}

// Cyclone cast in the weeds below the stairs in the middle of the lighthouse's front (metatiles
// (21, 26) to (25, 28)) scatters those within two metatiles of Felix.
void scJupiterLighthouseScatterWeedsBelowTheStairs(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(56, 0, 5, 3, 85, 26, 1);
}

// Cyclone cast in the weeds under the east part of the lighthouse's front (metatiles (30, 26) to
// (34, 28)) scatters those within two metatiles of Felix.
void scJupiterLighthouseScatterEastWeeds(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(56, 0, 5, 3, 94, 26, 1);
}

// Where the Mint lies under the weeds below the west end of the front wall: metatile (17, 27).
#define MINT_SPOT_X 280
#define MINT_SPOT_Z 440

// Cyclone cast standing on metatile (17, 27), in the weeds under the west end of the lighthouse's
// front wall, scatters them and, until it is found, uncovers a Mint under Felix's feet: he jumps a
// metatile west off it, and its green leaf lies in the cleared sand.
void scJupiterLighthouseScatterWeedsOverMint(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	syFieldTileCopiesCopyTouchedMetatiles(56, 0, 3, 3, 79, 25, 1);
	if (!fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_MINT))
	{
		fdActorSetPos(FIELD_TREASURE_ACTOR, MINT_SPOT_X, MINT_SPOT_Z);
		if (FIELD_METATILE(leader->x.fixed) == FIELD_METATILE(toFIXED(MINT_SPOT_X))
		    && FIELD_METATILE(leader->z.fixed) == FIELD_METATILE(toFIXED(MINT_SPOT_Z)))
		{
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
			                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, -16, 0);
			leader->velocityY = toFIXED(6);
			scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdScriptEnd();
		}
	}
}

// Cyclone on the purple teleporter pad in the west dead end, metatile (7, 21): the whirlwind lifts
// Felix up off the screen, and he drops onto the near teleporter in the cave beneath the
// lighthouse.
void scJupiterLighthouseTeleportToBasement(void)
{
	syImportScriptSetExitRequest(
		JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS);
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

// Sets up the grounds as the party enters: the fade in, the teleporter's ring when the party
// arrives by it, and Contigo's shops as where a defeat returns to. Returns 0.
Sint32 scInit(void)
{
	Sint32 keepPos;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	keepPos                    = fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION);
	if (keepPos == 0)
	{
		if (MainRoutineStateWork.entrance
		    == JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS)
			fdActorArriveInRing(MainRoutineStateWork.leaderActorId);
	}
	MainRoutineStateWork.restartScene    = SCENE_CONTIGO_SHOPS;
	MainRoutineStateWork.restartEntrance = CONTIGO_SHOPS_FROM_CONTIGO_NORTHEAST_A;
	return 0;
}

// Sets the scene's arrival flags before the map is built. Returns 0.
Sint32 scPrepareMap(void)
{
	scSetArrivalFlags(0);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern Sint32 PaletteCyclePausesInScripts;

#include "shared/palette_cycle_set_pauses_in_scripts.h"

void               syGameFlagSetByte(Sint32 flag, Sint32 value);

extern Sint32      PaletteCycleTick;

// Sets the palette cycle's tick (PaletteCycleTick).
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

// The ROM's callers here are compiled against this form, which returns an Sint32 where the
// definition returns a Uint8.
Sint32        fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);
#include "shared/whirlwind_update_lower.h"

#include "shared/whirlwind_spawn.h"

// Unused: nothing in the entrance sets scActorEmitSmoke as an update.
#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

// Unused: sets the priority the lit actors' sprites are drawn at; nothing in the entrance calls it.
UNUSED void scLightSpritesSetPriority(Sint32 value)
{
	LightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
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

CUE SceneCues[] = {
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_WORLD_MAP, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_WORLD_MAP},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_NORTH_OF_THE_TELEPORTER, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseScatterWeedsNorthOfTheTeleporter)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_SOUTH_OF_THE_TELEPORTER, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseScatterWeedsSouthOfTheTeleporter)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEST_WEEDS, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseScatterWestWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_BELOW_THE_STAIRS, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseScatterWeedsBelowTheStairs)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_EAST_WEEDS, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseScatterEastWeeds)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_TELEPORTER, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseTeleportToBasement)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_WEEDS_OVER_MINT, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseScatterWeedsOverMint)},
	{CUE_TABLE_END},
};

Sint32 PaletteCycleTick = -1;

Sint32 PaletteCyclePausesInScripts = 1;

Sint32 WhirlwindEndScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};
// clang-format on

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

#include "../field/field_effect_move.c"

extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSpriteAtDepthClamped
#include "../field/cracked_tile_mask.c"

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
