/// @file
/// The hall of hover tiles at the top of Jupiter Lighthouse's right tower,
/// below the aerie. Hover here lasts as long as the party stays on the hall's
/// floor (JupiterLighthouse_StartHovering), and stone faces blow whirlwinds
/// across the floor and the upper ledges as the party passes their cells
/// (JupiterLighthouse_RaiseWhirlwind, JupiterLighthouse_RaiseUpperWhirlwind).
/// Cracked tiles give way under a second step
/// (JupiterLighthouse_StepOnLowerCrackedTile,
/// JupiterLighthouse_StepOnUpperCrackedTile); the way on leads out to the
/// aerie.
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_9/, in two parts the shared field effect objects
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
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "field/cell_target.h"
#include "objcmd.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field_event.h"
#include "system/frame.h"
#include "field_sprite.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "module/field/field_effect.h"

/// Where a stone face's whirlwind starts: an unseen actor moved to the face
/// that blows, which the whirlwind takes its place and heading from.
enum { ACTOR_WHIRLWIND_SOURCE = ACTOR_FIRST_PLACED };

struct Actor;
struct LightOam;
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
void JupiterLighthouse_LoadPalettes(s32 mode);
extern s16 gLightSpriteTile;
extern struct Actor *gLightSpriteActors[];
s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void JupiterLighthouse_StopHovering(void);
void Whirlwind_CarryLeader(void);
extern s32 gWhirlwindRiding;
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

s32 CrackedTiles_Step();
struct GustObject *Whirlwind_SpawnUpper();

void JupiterLighthouse_StartHovering(void) {
  if (gMainRoutineWork.renderMode == 10)
    FieldMapGrid_SetCellBehavior(0x2200000, 0x1100000, 0, 255);
}

void JupiterLighthouse_StopHovering(void) {
  MapGrid_SetCellBehavior(0x2200000, 0x1100000, 0, 6);
}

void JupiterLighthouse_RaiseWhirlwind(s32 id) {
  struct Actor *object;

  if (gWhirlwindRiding == 0) {
    object = Actor_Get(ACTOR_WHIRLWIND_SOURCE);
    if (id == 30) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 296, 600);
      object->y = 0;
      object->facing = FACING_WEST;
    } else if (id == 31) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 136, 680);
      object->y = 0;
      object->facing = 0;
    }
    Whirlwind_SpawnUpper(object);
    gWhirlwindRiding = 1;
  }
}

void JupiterLighthouse_RaiseUpperWhirlwind(s32 id) {
  struct Actor *object;
  struct GustObject *motion;

  if (gWhirlwindRiding == 0 &&
      Actor_Get(gMainRoutineWork.currentOwner)->y > 0) {
    object = Actor_Get(ACTOR_WHIRLWIND_SOURCE);
    if (id == 50) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 488, 280);
      object->y = PIXELS(32);
      object->facing = FACING_WEST;
    } else if (id == 51) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 472, 184);
      object->y = PIXELS(32);
      object->facing = FACING_WEST;
    } else if (id == 52) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 616, 280);
      object->y = PIXELS(32);
      object->facing = 0;
    } else if (id == 53) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 632, 184);
      object->y = PIXELS(32);
      object->facing = 0;
    }
    motion = Whirlwind_SpawnUpper(object);
    if (motion != NULL)
      motion->speedLimit = 0x10000;
    gWhirlwindRiding = 1;
  }
}

void JupiterLighthouse_StepOnLowerCrackedTile(s32 arg) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 &&
      CrackedTiles_Step(arg) != 0)
    FieldEvent_SetExitRequest(13);
}

void JupiterLighthouse_StepOnUpperCrackedTile(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 18 && z == 34)
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
    if (x == 8 && z == 39)
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
    if (CrackedTiles_Step(arg) != 0)
      FieldEvent_SetExitRequest(14);
  }
}

void JupiterLighthouse_StepOnCrackedTileOrWhirlwind(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader->y <= 0) {
    JupiterLighthouse_StepOnLowerCrackedTile(arg);
  } else {
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 25) {
      if (z == 17)
        JupiterLighthouse_RaiseUpperWhirlwind(50);
      if (z == 11)
        JupiterLighthouse_RaiseUpperWhirlwind(51);
    }
    if (x == 43) {
      if (z == 17)
        JupiterLighthouse_RaiseUpperWhirlwind(52);
      if (z == 11)
        JupiterLighthouse_RaiseUpperWhirlwind(53);
    }
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// The scene's set-up: screen effect 0x204, actor 64's action variant 3 and
/// actor 8 held still, then CrackedTiles_Find and CrackedTile_PlaceLanding by
/// the entrance.
s32 Scene_Initialize(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  FieldActor_SetSpritePriority(64, 3);
  gWhirlwindRiding = 0;
  Actor_Get(ACTOR_WHIRLWIND_SOURCE)->motionFlags = 0;
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    CrackedTiles_Find(33, 0x280, 0x380, 48, 1);
    CrackedTile_PlaceLanding(0, 0, 0x204);
  } else if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5 ||
             gMainRoutineWork.entrance == 13) {
    CrackedTiles_Find(43, 0x280, 0x380, 49, 1);
    CrackedTile_PlaceLanding(-21, 32, 0x204);
  } else {
    CrackedTiles_Find(43, 0x280, 0x380, 0, 0);
    CrackedTile_PlaceLanding(-2, -27, 0x204);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
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

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn_upper.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

extern s16 gLightSpriteCount;
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

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

extern u32 gFieldEffect_LongScript[];

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
    SCENE_EXITS(JupiterLighthouseRoom9),
    SCENE_EXIT(1, JupiterLighthouseRoom7, 1),
    SCENE_EXIT(2, JupiterLighthouseRoom9, 3),
    SCENE_EXIT(3, JupiterLighthouseRoom9, 2),
    SCENE_EXIT(4, JupiterLighthouseAerie, 4),
    SCENE_EXIT(5, JupiterLighthouseRoom9, 6),
    SCENE_EXIT(6, JupiterLighthouseRoom9, 5),
    SCENE_EXIT(7, JupiterLighthouseRoom9, 8),
    SCENE_EXIT(8, JupiterLighthouseRoom9, 7),
    SCENE_EXIT(13, JupiterLighthouseRoom9, 13),
    SCENE_EXIT(14, JupiterLighthouseRoom9, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {489, CONDITION_ALWAYS, 11, -0x80000, 0, -0x80000, FACING_WEST, 2, 0},
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 424, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 440, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 664, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 680, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 440, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 424, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 440, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 456, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 472, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 488, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 504, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 520, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 536, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 552, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 568, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 584, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 600, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 616, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 632, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 648, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 664, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 680, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(317, CONDITION_ALWAYS, 11, 696, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Whirlwind_CarryLeader)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTileOrWhirlwind)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartHovering)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StopHovering)},
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

// @file
// The hover-tile hall of Jupiter Lighthouse's right tower: its copy of the
// effect spawner, and the cracked tiles, found, cracked and fallen through.

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

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

/* VIEW(s1648): tag struct CellTarget as struct CellTarget__s1648; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1648 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

void CrackedTile_Fall(struct CellTarget__s1648 *table,
                      struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = (leader->x >> 20) | (s16)(table->x & 64);
  s32 z = (leader->z >> 20) | (s16)(table->z & 64);

  Leader_CenterInCell();
  Event_PlaySound(SE_SPLASH);
  MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  leader->facing = 0x4000;
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  table->shown = 1;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  x = 0;
  do {
    if (x == 5)
      Event_PlaySound(SE_DUST_PUFF);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    leader->y += -0x9999;
    System_Wait(1);
    x++;
  } while (x <= 39);
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  table->shown = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
// The hover-tile hall of Jupiter Lighthouse's right tower: the cracked tiles'
// steps and landings.

struct CellTarget;
struct CellTargetEntry;
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);
void JupiterLighthouse_StopHovering(void);
void Whirlwind_CarryLeader(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"
