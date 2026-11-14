/// @file
/// The ground floor of Jupiter Lighthouse, inside its main gates. The party
/// first comes in from below, by the basement's Cyclone teleporter (entrance
/// 6). Standing on the button by the gates holds them open
/// (JupiterLighthouse_StepOnButton, JupiterLighthouse_StepOffButton); the
/// pillar pushed onto it opens them for good
/// (JupiterLighthouse_CheckBlockOnCell), so the way out to the entrance stays
/// open. A Cyclone on the teleporter here leads back down
/// (JupiterLighthouse_LeaveByCyclone).
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_1/, in two parts the shared field effect objects
/// link between.

#include "shared/tables.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "field/event_inline.h"
#include "field_event.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "debug.h"
#include "field/field_sequence_event.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "system/irq.h"
#include "field_sprite.h"
#include "call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "objcmd.h"
#include "field/cell_target.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "actor.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "module/field/field_effect.h"

// Set while the leader stands on the button by the gates, which holds them
// open.
#define FLAG_GATES_BUTTON_HELD 0x200

/// The pillar the party pushes onto the button by the gates, after the two
/// angel statues in gActorPlacements.
enum { ACTOR_GATES_PILLAR = ACTOR_FIRST_PLACED + 2 };

struct Actor;
struct LightOam;
void Actor_ArriveInRing(s32 owner);
extern s32 gWhirlwindRiding;
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;
void JupiterLighthouse_StepOnButton(void);
void JupiterLighthouse_StepOffButton(void);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void JupiterLighthouse_LeaveByCyclone(void) { Import_EventSetExitRequest(6); }

void JupiterLighthouse_OpenDoor(s32 animate) {
  if (animate == 1) {
    Event_PlaySound(SE_IMPACT);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 39);
}

void JupiterLighthouse_CloseDoor(void) {
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
  System_Wait(6);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 39);
}

void JupiterLighthouse_CheckBlockOnCell(void) {
  struct Actor *object = Actor_Get(ACTOR_GATES_PILLAR);

  if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED) &&
      object->x >> 20 == 17 && object->z >> 20 == 33) {
    Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED);
    JupiterLighthouse_OpenDoor(1);
  }
}

void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(FLAG_GATES_BUTTON_HELD)) {
    Event_SetFlag(FLAG_GATES_BUTTON_HELD);
    JupiterLighthouse_OpenDoor(1);
  }
}

void JupiterLighthouse_StepOffButton(void) {
  if (Event_GetFlag(FLAG_GATES_BUTTON_HELD) == 1) {
    Event_ClearFlag(FLAG_GATES_BUTTON_HELD);
    JupiterLighthouse_CloseDoor();
  }
  JupiterLighthouse_CheckBlockOnCell();
}

void JupiterLighthouse_FinishMovingBlock(void) {
  System_Wait(10);
  JupiterLighthouse_CheckBlockOnCell();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  gFieldWork->transition = 0x204;
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.entrance == 6)
    Actor_ArriveInRing(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_GATES_PILLAR, 280, 536);
    JupiterLighthouse_OpenDoor(0);
  }
  if (Event_GetFlag(FLAG_GATES_BUTTON_HELD))
    JupiterLighthouse_OpenDoor(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

extern s32 gPaletteCyclePausesInEvents;

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

#include "shared/palette_cycle_step_at.h"

void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();

void JupiterLighthouse_LoadPalettes(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gPaletteCycleUnlitPalette,
                         (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    Counter_Set(0);
    Task_Add(PaletteCycle_StepAt, 1152);
  } else {
    Dma_Set(gPaletteCycleUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 Position_FlatDistance(s32 *from, s32 *to);
void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks_b.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

extern struct LightOam gLightSpriteOams[];

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

void JupiterLighthouse_LeaveByCyclone(void);
void JupiterLighthouse_FinishMovingBlock(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoom1),
    SCENE_EXIT(1, JupiterLighthouseRoom2, 1),
    SCENE_EXIT(2, JupiterLighthouseEntrance, 2),
    SCENE_EXIT(3, JupiterLighthouseEntrance, 3),
    SCENE_EXIT(4, JupiterLighthouseRoom1, 5),
    SCENE_EXIT(5, JupiterLighthouseRoom1, 4),
    SCENE_EXIT(6, JupiterLighthouseBasement1, 6),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 744, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 744, FACING_SOUTHEAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(312, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 536, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, 21, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATES_PILLAR, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "../field/field_effect/move.c"

#undef FLAG_GATES_BUTTON_HELD
// @file
// Jupiter Lighthouse's ground floor: its copy of the effect spawner, and the
// cracked tiles, found, cracked and fallen through.

extern const s32 *gFieldEffect_DurationScripts[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

#include "shared/cracked_tiles_find_lower.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
// Jupiter Lighthouse's ground floor: the cracked tiles' steps and landings.

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

void JupiterLighthouse_StepOnButton(void);
void JupiterLighthouse_StepOffButton(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

#include "shared/cracked_tile_drop_in.h"

extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"
