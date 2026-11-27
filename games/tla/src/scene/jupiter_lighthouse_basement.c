/* @file */
/* The grey caves beneath Jupiter Lighthouse, reached by the teleporter in the
 */
/* sandy dead end of its grounds */
/* (SceneId_JupiterLighthouseBasementTeleporters). The party arrives on the */
/* near teleporter in the west. Cyclone scatters the weeds round each */
/* teleporter and a patch in the lower right corner of the cave, where a Mad */
/* Plant hides (scJupiterLighthouseUncoverMadPlant). Hover on the purple tile */
/* crosses to the far teleporter in the east, whose Cyclone takes the party up
 */
/* to the ground floor inside the main gates */
/* (scJupiterLighthouseTeleportToGroundFloor), and the near one back up to the
 */
/* grounds. Its side room (SceneId_JupiterLighthouseBasementPsynergyStone) */
/* holds a Psynergy Stone, and sixteen sparks rise and sway out of the dark in
 */
/* its middle. */

/* This unit declares its own form of fdMapGridGetCellBehavior, below. */
#define OWN_MapGrid_GetCellBehavior

#include "scene/event_layer.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "scene/lighthouse_palettes.h"

/* The side room's sparks, which take turns at the particle sprite sheet's two
 */
/* swells. */
#define JUPITER_LIGHTHOUSE_BASEMENT_SPARK_COUNT 16

/* The cells Cyclone acts on in the teleporters' cave: the weeds round the */
/* near teleporter, the five of the Mad Plant's patch that hide nothing, the */
/* weeds round the far teleporter, the two teleporters (cells (5, 26) and */
/* (46, 32)) and the Mad Plant's own, cell (26, 30). */
enum {
	CELL_NEAR_WEEDS = 30,
	CELL_MAD_PLANT_WEEDS,
	CELL_FAR_WEEDS,
	CELL_NEAR_TELEPORTER,
	CELL_FAR_TELEPORTER,
	CELL_MAD_PLANT = 38
};

struct EFFECT_OPTIONS;

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

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
/* Sets a map cell's ground height (fdMapGridSetCellHeight, through the */
/* overlay's import). */
void syImportMapGridSetCellHeight(s32 x, s32 z, s32 layer, s32 height);

void fdLeaderCenterInCell(void);

void scJupiterLighthouseScatterMadPlantWeeds(void);

void scJupiterLighthouseScatterFarWeeds(void);

void scJupiterLighthouseTeleportToEntrance(void);

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];
extern u32 FieldEffect_LongScript[];

/* The ROM's callers here are compiled against this form, which returns an s32
 */
/* where the definition returns a u8. */
s32 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer);
const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

extern TRIGGER JupiterLighthouseBasement_TeleportersTriggers[];
extern TRIGGER JupiterLighthouseBasement_PsynergyStoneTriggers[];

/* Cyclone cast in the weeds round the near teleporter in the west of the */
/* cave, the one up to the grounds, scatters those within two cells of Felix. */
void scJupiterLighthouseScatterNearWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(69, 0, 4, 4, 67, 24, 1);
}

/* Cyclone cast in the patch of weeds in the lower right corner of the cave */
/* (cells (25, 28) to (28, 29)) scatters those within two cells of Felix; */
/* only from the patch's middle, cell (26, 30), does it uncover the Mad Plant.
 */
void scJupiterLighthouseScatterMadPlantWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(64, 0, 4, 3, 89, 28, 1);
}

/* Cyclone cast standing on cell (26, 30), in the middle of the weeds in the */
/* cave's lower right corner, scatters them, and until it is beaten a Mad */
/* Plant leaps out at Felix and a battle with it begins, one the party cannot */
/* run from. */
void scJupiterLighthouseUncoverMadPlant(void) {
	syFieldTileCopiesCopyTouchedCells(64, 0, 4, 3, 89, 28, 1);
	if(!fdEventGetFlag(FLAG_MAD_PLANT(3))) {
		syFieldMadPlantLeapAtLeader(SPRITE_MAD_PLANT, 424, 488);
		MainRoutineStateWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(3));
		btSetType(BATTLE_TYPE_NO_ESCAPE);
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_MAD_PLANTS, 3);
	}
}

/* Cyclone cast in the weeds round the far teleporter in the east of the */
/* cave, the one up to the ground floor, scatters those within two cells of */
/* Felix. */
void scJupiterLighthouseScatterFarWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(69, 1, 4, 3, 108, 31, 1);
}

/* Cyclone on the near teleporter, cell (5, 26), lifts the party back up to */
/* the grounds before the lighthouse. */
void scJupiterLighthouseTeleportToEntrance(void) {
	syImportEventSetExitRequest(
		JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ENTRANCE);
}

/* Cyclone on the far teleporter, cell (46, 32), lifts the party up into the */
/* lighthouse's ground floor, inside its gates. */
void scJupiterLighthouseTeleportToGroundFloor(void) {
	syImportEventSetExitRequest(
		JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON);
}

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next ==
	   (s32)&SceneId_JupiterLighthouseBasementPsynergyStone)
		return JupiterLighthouseBasement_PsynergyStoneTriggers;
	return JupiterLighthouseBasement_TeleportersTriggers;
}

/* Starts a spark's life again, 32 pixels below the floor at a random point 8 */
/* to 40 pixels from the middle of the dark (one time in 256 it starts from */
/* where it is), swelling slowly or quickly as its lifetime is long or short. */
void scJupiterLighthouseRestartSpark(ACTOR *actor) {
	if(syRandom() & 0xff) {
		FIXED position[3];
		s32 radius;
		s32 angle;

		SET_VECTOR(position, toFIXED(200), toFIXED(-32), toFIXED(164));
		radius = syRandom();
		angle = syRandom();
		syFieldVectorAddPolarOffsetFX((radius << 5) + toFIXED(8), angle,
									  position);
		SET_ACTOR_POSITION(actor, position[0], position[1], position[2]);
	}
	actor->userValue = actor->tmr;
	if(actor->tmr == PARTICLE_SLOW_LIFETIME)
		fdActorSelectAnimation(actor, ANIM_PARTICLE_SLOW_SWELL);
	else
		fdActorSelectAnimation(actor, ANIM_PARTICLE_QUICK_SWELL);
}

/* A spark's update: it stands still, and the room's tile animations with it, */
/* while Psynergy or a menu is up; otherwise it waits out its starting delay, */
/* then rises, swaying east and west, until its life starts again. */
void scJupiterLighthouseUpdateSpark(ACTOR *actor) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0) {
		fdActorSetAnimationSpeed(actor, 0);
		fdTilesetAnimSuspendAll();
		return;
	}
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL);
	fdTilesetAnimResumeAll();
	if(actor->sceneByte != 0) {
		if((FrameCount & 7) == 0)
			actor->sceneByte--;
		return;
	}
	if(actor->userValue == 0)
		scJupiterLighthouseRestartSpark(actor);
	actor->userValue--;
	actor->x.fixed += syCos((FIXEDtoINT_FOUR_TIMES(actor->z.fixed) +
							 FIXEDtoINT_TWICE(actor->y.fixed)) *
							(TRIG_TURN / 256)) /
					  2;
	actor->y.fixed += actor->speedLimit;
}

/* Places the side room's sixteen sparks, each starting 64 frames after the */
/* one before and rising a sixteenth of a pixel a frame faster; every second */
/* one swells slowly. */
void scJupiterLighthouseSpawnSparks(void) {
	s32 i;
	ACTOR *actor;

	for(i = 0; i != JUPITER_LIGHTHOUSE_BASEMENT_SPARK_COUNT; i++) {
		actor = scFieldActorCreate(SPRITE_PARTICLE, 0, 0, 0);
		actor->motionFlg = 0;
		actor->priFlg = 0;
		actor->speedLimit = i * (toFIXED(1) / 16) + toFIXED(1) / 2;
		actor->acceleration = i * (toFIXED(1) / 16) + toFIXED(1) / 2;
		actor->update = (OBJECT_UPDATE)scJupiterLighthouseUpdateSpark;
		actor->sceneByte = i << 3;
		fdActorSetSprFlags(actor, 0);
		scFieldActorSetSprCol(actor, SPRITE_COLOR_RBG);
		((FIELD_SPR *)actor->spr)->pri = 1;
		fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
		if(i & 1) {
			actor->tmr = PARTICLE_SLOW_LIFETIME;
			actor->userValue = 0;
		}
		else {
			actor->tmr = PARTICLE_QUICK_LIFETIME;
			actor->userValue = 0;
		}
	}
}

/* Sets up the basement room being entered: the teleporters' palettes, cycling
 */
/* whether or not the lighthouse is lit, and the ring the party arrives in by */
/* teleporter, or the Psynergy Stone room's blended sparks. Returns 0. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next ==
	   (s32)&SceneId_JupiterLighthouseBasementTeleporters) {
		scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_LIT);
		if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) &&
		   MainRoutineStateWork.entrance ==
			   JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_ENTRANCE)
			fdActorDepartInRing(MainRoutineStateWork.leaderActorId);
		if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) &&
		   MainRoutineStateWork.entrance ==
			   JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON)
			fdActorDepartInRing(MainRoutineStateWork.leaderActorId);
	}
	if(MainRoutineStateWork.next ==
	   (s32)&SceneId_JupiterLighthouseBasementPsynergyStone) {
		REG_BLDALPHA = BLEND_ALPHA(8, 16);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		scJupiterLighthouseSpawnSparks();
	}
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

extern s32 PaletteCycleFrame;

/* Sets the palette cycle's frame (PaletteCycleFrame). */
static inline void scPaletteCycleSetFrame(s32 value) {
	PaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

/* Lifts an actor 32 pixels over its ground as a platform others may stand on,
 */
/* and raises the map cell under it six height steps to match. */
void scActorRaiseFromCell(s32 id) {
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
	syImportMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
								 CELL_HEIGHT_STEPS(height) + 6);
	fdMapGridSetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
								 MAP_CELL_JUMP);
}

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 WhirlwindEndScript[];

#include "shared/whirlwind_update_lower.h"

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Sets the priority the lit actors' sprites are drawn at; -1 takes each */
/* one's own. */
void scLightSprsSetPriority(s32 value) { LightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

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

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(JupiterLighthouseBasementTeleporters),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE, JupiterLighthouseBasementPsynergyStone, JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS),
    SCENE_EXIT(2, JupiterLighthouseBasementTeleporters, 3),
    SCENE_EXIT(3, JupiterLighthouseBasementTeleporters, 2),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ENTRANCE, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS),
    SCENE_EXITS(JupiterLighthouseBasementPsynergyStone),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, JupiterLighthouseBasementTeleporters, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    {SCENE_TABLE_END},
};

TRIGGER JupiterLighthouseBasement_TeleportersTriggers[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ENTRANCE, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ENTRANCE},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_NEAR_WEEDS, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterNearWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_MAD_PLANT_WEEDS, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterMadPlantWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_FAR_WEEDS, CONDITION_ALWAYS, EVENT(scJupiterLighthouseScatterFarWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_NEAR_TELEPORTER, CONDITION_ALWAYS, EVENT(scJupiterLighthouseTeleportToEntrance)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_FAR_TELEPORTER, CONDITION_ALWAYS, EVENT(scJupiterLighthouseTeleportToGroundFloor)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_MAD_PLANT, CONDITION_ALWAYS, EVENT(scJupiterLighthouseUncoverMadPlant)},
    {TRIGGER_TABLE_END},
};

TRIGGER JupiterLighthouseBasement_PsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS},
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
