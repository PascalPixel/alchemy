/// @file
/// The grey caves beneath Jupiter Lighthouse, reached by the teleporter in the
/// sandy dead end of its grounds
/// (SceneId_JupiterLighthouseBasementTeleporters). The party arrives on the
/// near teleporter in the west. Cyclone scatters the weeds round each
/// teleporter and a patch in the lower right corner of the cave, where a Mad
/// Plant hides (JupiterLighthouse_UncoverMadPlant). Hover on the purple tile
/// crosses to the far teleporter in the east, whose Cyclone takes the party up
/// to the ground floor inside the main gates
/// (JupiterLighthouse_TeleportToGroundFloor), and the near one back up to the
/// grounds. Its side room (SceneId_JupiterLighthouseBasementPsynergyStone)
/// holds a Psynergy Stone, and sixteen sparks rise and sway out of the dark in
/// its middle.

#include "flags.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "scene/event_layer.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "macros.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "debug/debug.h"
#include "field/cell_target.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "scene/mars_lighthouse_entrance.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "system/irq.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "window/item_icon.h"
#include "field/field_coordinate.h"
#include "system/task.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// The side room's sparks, which take turns at the particle sprite sheet's two
/// swells.
#define JUPITER_LIGHTHOUSE_BASEMENT_SPARK_COUNT 16

/// The cells Cyclone acts on in the teleporters' cave: the weeds round the
/// near teleporter, the five of the Mad Plant's patch that hide nothing, the
/// weeds round the far teleporter, the two teleporters (cells (5, 26) and
/// (46, 32)) and the Mad Plant's own, cell (26, 30).
enum {
  CELL_NEAR_WEEDS = 30,
  CELL_MAD_PLANT_WEEDS,
  CELL_FAR_WEEDS,
  CELL_NEAR_TELEPORTER,
  CELL_FAR_TELEPORTER,
  CELL_MAD_PLANT = 38
};

struct EffectOptions;

void Actor_DepartInRing(s32 id);

u8 MapGrid_SetCellAttributesAt(s32 x, s32 z, s32 layer, s32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;
/// Sets a map cell's ground height (MapGrid_SetCellHeight, through the
/// overlay's import).
void Import_MapGridSetCellHeight(s32 x, s32 z, s32 layer, s32 height);

void Leader_CenterInCell(void);

void JupiterLighthouse_ScatterMadPlantWeeds(void);

void JupiterLighthouse_ScatterFarWeeds(void);

void JupiterLighthouse_TeleportToEntrance(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

s32 Actor_GetSpritePriority(struct Actor *actor);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

extern struct Trigger gJupiterLighthouseBasement_TeleportersTriggers[];
extern struct Trigger gJupiterLighthouseBasement_PsynergyStoneTriggers[];

/// Cyclone cast in the weeds round the near teleporter in the west of the
/// cave, the one up to the grounds, scatters those within two cells of Felix.
void JupiterLighthouse_ScatterNearWeeds(void) {
  FieldTileCopies_CopyTouchedCells(69, 0, 4, 4, 67, 24, 1);
}

/// Cyclone cast in the patch of weeds in the lower right corner of the cave
/// (cells (25, 28) to (28, 29)) scatters those within two cells of Felix;
/// only from the patch's middle, cell (26, 30), does it uncover the Mad Plant.
void JupiterLighthouse_ScatterMadPlantWeeds(void) {
  FieldTileCopies_CopyTouchedCells(64, 0, 4, 3, 89, 28, 1);
}

/// Cyclone cast standing on cell (26, 30), in the middle of the weeds in the
/// cave's lower right corner, scatters them, and until it is beaten a Mad
/// Plant leaps out at Felix and a battle with it begins, one the party cannot
/// run from.
void JupiterLighthouse_UncoverMadPlant(void) {
  FieldTileCopies_CopyTouchedCells(64, 0, 4, 3, 89, 28, 1);
  if (!Event_GetFlag(FLAG_MAD_PLANT(3))) {
    FieldMadPlant_LeapAtLeader(SPRITE_MAD_PLANT, 424, 488);
    gMainRoutineState.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(3));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_MAD_PLANTS, 3);
  }
}

/// Cyclone cast in the weeds round the far teleporter in the east of the
/// cave, the one up to the ground floor, scatters those within two cells of
/// Felix.
void JupiterLighthouse_ScatterFarWeeds(void) {
  FieldTileCopies_CopyTouchedCells(69, 1, 4, 3, 108, 31, 1);
}

/// Cyclone on the near teleporter, cell (5, 26), lifts the party back up to
/// the grounds before the lighthouse.
void JupiterLighthouse_TeleportToEntrance(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ENTRANCE);
}

/// Cyclone on the far teleporter, cell (46, 32), lifts the party up into the
/// lighthouse's ground floor, inside its gates.
void JupiterLighthouse_TeleportToGroundFloor(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next ==
      (s32)&SceneId_JupiterLighthouseBasementPsynergyStone)
    return gJupiterLighthouseBasement_PsynergyStoneTriggers;
  return gJupiterLighthouseBasement_TeleportersTriggers;
}

/// Starts a spark's life again, 32 pixels below the floor at a random point 8
/// to 40 pixels from the middle of the dark (one time in 256 it starts from
/// where it is), swelling slowly or quickly as its lifetime is long or short.
void JupiterLighthouse_RestartSpark(struct Actor *actor) {
  if (Random16() & 0xff) {
    FIXED position[3];
    s32 radius;
    s32 angle;

    SET_VECTOR(position, toFIXED(200), toFIXED(-32), toFIXED(164));
    radius = Random16();
    angle = Random16();
    FieldVector_AddPolarOffsetFX((radius << 5) + toFIXED(8), angle, position);
    SET_ACTOR_POSITION(actor, position[0], position[1], position[2]);
  }
  actor->userValue = actor->timer;
  if (actor->timer == PARTICLE_SLOW_LIFETIME)
    Actor_SelectAnimation(actor, ANIM_PARTICLE_SLOW_SWELL);
  else
    Actor_SelectAnimation(actor, ANIM_PARTICLE_QUICK_SWELL);
}

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void JupiterLighthouse_LoadPalettes(s32 mode);

/// A spark's update: it stands still, and the room's tile animations with it,
/// while Psynergy or a menu is up; otherwise it waits out its starting delay,
/// then rises, swaying east and west, until its life starts again.
void JupiterLighthouse_UpdateSpark(struct Actor *actor) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0) {
    Actor_SetAnimationSpeed(actor, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL);
  TilesetAnim_ResumeAll();
  if (actor->sceneByte != 0) {
    if ((gFrameCount & 7) == 0)
      actor->sceneByte--;
    return;
  }
  if (actor->userValue == 0)
    JupiterLighthouse_RestartSpark(actor);
  actor->userValue--;
  actor->x.fixed += Trig_Cos((FIXEDtoINT_FOUR_TIMES(actor->z.fixed) +
                              FIXEDtoINT_TWICE(actor->y.fixed)) *
                             (TRIG_TURN / 256)) /
                    2;
  actor->y.fixed += actor->speedLimit;
}

/// Places the side room's sixteen sparks, each starting 64 frames after the
/// one before and rising a sixteenth of a pixel a frame faster; every second
/// one swells slowly.
void JupiterLighthouse_SpawnSparks(void) {
  s32 i;
  struct Actor *actor;

  for (i = 0; i != JUPITER_LIGHTHOUSE_BASEMENT_SPARK_COUNT; i++) {
    actor = FieldActor_Create(SPRITE_PARTICLE, 0, 0, 0);
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->speedLimit = i * (toFIXED(1) / 16) + toFIXED(1) / 2;
    actor->acceleration = i * (toFIXED(1) / 16) + toFIXED(1) / 2;
    actor->update = (ObjectUpdate)JupiterLighthouse_UpdateSpark;
    actor->sceneByte = i << 3;
    Actor_SetSpriteFlags(actor, 0);
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
    Actor_SetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
    if (i & 1) {
      actor->timer = PARTICLE_SLOW_LIFETIME;
      actor->userValue = 0;
    } else {
      actor->timer = PARTICLE_QUICK_LIFETIME;
      actor->userValue = 0;
    }
  }
}

/// Sets up the basement room being entered: the teleporters' palettes, cycling
/// whether or not the lighthouse is lit, and the ring the party arrives in by
/// teleporter, or the Psynergy Stone room's blended sparks. Returns 0.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineState.next ==
      (s32)&SceneId_JupiterLighthouseBasementTeleporters) {
    JupiterLighthouse_LoadPalettes(1);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineState.entrance ==
            JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_ENTRANCE)
      Actor_DepartInRing(gMainRoutineState.leaderActorId);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineState.entrance ==
            JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON)
      Actor_DepartInRing(gMainRoutineState.leaderActorId);
  }
  if (gMainRoutineState.next ==
      (s32)&SceneId_JupiterLighthouseBasementPsynergyStone) {
    REG_BLDALPHA = BLEND_ALPHA(8, 16);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    JupiterLighthouse_SpawnSparks();
  }
  return 0;
}

/// Sets the scene's arrival flags before the map is built. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame (gPaletteCycleFrame).
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

/// Lifts an actor 32 pixels over its ground as a platform others may stand on,
/// and raises the map cell under it six height steps to match.
void Actor_RaiseFromCell(s32 id) {
  struct Actor *actor;
  s32 height;

  actor = Actor_Get(id);
  actor->motionFlags = ACTOR_FLOATS;
  height = actor->groundHeight;
  actor->bounce = 0;
  actor->y.fixed = height + toFIXED(32);
  actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  height = Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
  Import_MapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
                              CELL_HEIGHT_STEPS(height) + 6);
  MapGrid_SetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0, MAP_CELL_JUMP);
}

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/// Sets the priority the lit actors' sprites are drawn at; -1 takes each
/// one's own.
void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
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

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gJupiterLighthouseBasement_TeleportersTriggers[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ENTRANCE, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_TO_JUPITER_LIGHTHOUSE_ENTRANCE},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_NEAR_WEEDS, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterNearWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_MAD_PLANT_WEEDS, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterMadPlantWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_FAR_WEEDS, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterFarWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_NEAR_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_TeleportToEntrance)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_FAR_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_TeleportToGroundFloor)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_MAD_PLANT, CONDITION_ALWAYS, EVENT(JupiterLighthouse_UncoverMadPlant)},
    {TRIGGER_TABLE_END},
};

struct Trigger gJupiterLighthouseBasement_PsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
