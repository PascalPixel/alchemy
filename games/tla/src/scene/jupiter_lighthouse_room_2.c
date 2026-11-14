/// @file
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
#include "field/field_sequence_event.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "system/iwram_call.h"
#include "macros.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "call.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/irq.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "system/heap_release.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "objcmd.h"

void Actor_Release(struct Actor *object);

void Actor_RaiseFromCell(s32 id);

void LightSprites_InitB(void);

void Func_020015a0(s32 id, s32 value);

void BattleEffect_FillOverlayTiles(s32 a);

void JupiterLighthouse_LoadPalettes(s32 mode);

extern s32 gWhirlwindRiding;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

s32 Position_FlatDistance(s32 *a, s32 *b);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void Leader_CenterInCell(void);

extern struct CellTarget gCrackedTiles;

void Func_020003f4(void);
void Func_02000410(void);

void Func_02000440(void);

void Func_02000454(void);

void Func_020006dc(void);

void Func_02000704(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void MapGrid_SetAttributeBAt(s32 actor);
extern s32 gData_02002b88[3];
extern const u8 gData_02002b60[];
void EffectSprite_InitFromSlot94Blended(const void *script, s32 a, s32 b,
                                        s32 c);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitColors[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(void) {
  struct Actor *object;

  object = Actor_Get(gMainRoutineWork.currentOwner);
  if (object->terrainId == 1) {
    FieldMapGrid_SetCellBehavior(px(144), px(576), 2, 0xea);
    FieldMapGrid_SetCellBehavior(px(256), px(560), 2, 0xea);
  } else {
    FieldMapGrid_SetCellBehavior(px(144), px(576), 2, 0xeb);
    FieldMapGrid_SetCellBehavior(px(256), px(560), 2, 0xeb);
  }
}

void Func_020000b4(void) {
  BattleEffect_DrawSpriteAtWorldPosition((s32)gData_02002b60, gData_02002b88[0],
                                         gData_02002b88[1], gData_02002b88[2]);
}

void Func_020000d0(s32 kind) {
  s32 owner = gMainRoutineWork.currentOwner;
  struct Actor *leader = Actor_Get(owner);
  struct Actor *effect;
  s32 i;
  s32 *target;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  target = gData_02002b88;
  if (kind == 32) {
    target[0] = 136 << 16;
    target[1] = 144 << 16;
    target[2] = 148 << 18;
  } else {
    target[0] = 248 << 16;
    target[1] = 144 << 16;
    target[2] = 144 << 18;
  }
  {
    s32 priority = 0x480;

    Task_AddFrame(Func_020000b4, priority);
  }
  EventCamera_FollowActor(owner, 1);
  for (i = 35; i >= 0; i--) {
    leader->y += -px(1);
    System_Wait(1);
  }
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  effect = Actor_Spawn(280, leader->x, leader->y, leader->z);
  Actor_SelectAnimation(effect, 2);
  effect->scaleX = Q16_ONE * 3 / 2;
  effect->scaleY = Q16_ONE;
  ((struct FieldSprite *)effect->sprite)->priority = 1;
  while (leader->y > px(32)) {
    leader->y += -px(1);
    leader->facing += 0x2000;
    effect->x = leader->x;
    effect->y = leader->y + -0x60000;
    effect->z = leader->z;
    System_Wait(1);
  }
  Actor_SelectAnimation(effect, 6);
  i = 14;
  do {
    leader->facing += 0x2000;
    System_Wait(1);
    i--;
  } while (i >= 0);
  Actor_Release(effect);
  Actor_SetSpriteFlags(Actor_Get(owner), 1);
  leader->facing = 0x4000;
  leader->terrainId = 0;
  leader->motionFlags = 3;
  FieldTask_Remove(Func_020000b4);
  Func_02000054();
  Event_End();
}

void Func_02000230(s32 kind) {
  s32 owner = gMainRoutineWork.currentOwner;
  struct Actor *leader = Actor_Get(owner);
  struct Actor *effect;
  s32 *target;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  target = gData_02002b88;
  if (kind == 30) {
    target[0] = 136 << 16;
    target[1] = 144 << 16;
    target[2] = 148 << 18;
  } else {
    target[0] = 248 << 16;
    target[1] = 144 << 16;
    target[2] = 144 << 18;
  }
  {
    s32 priority = 0x480;

    Task_AddFrame(Func_020000b4, priority);
  }
  EventCamera_FollowActor(owner, 1);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  effect = Actor_Spawn(280, leader->x, leader->y, leader->z);
  Actor_SelectAnimation(effect, 2);
  effect->scaleX = Q16_ONE * 3 / 2;
  effect->scaleY = Q16_ONE;
  ((struct FieldSprite *)effect->sprite)->priority = 1;
  while (leader->y < px(108)) {
    leader->y += px(1);
    leader->facing += 0x2000;
    effect->x = leader->x;
    effect->y = leader->y + -0x60000;
    effect->z = leader->z;
    System_Wait(1);
  }
  leader->scaleX = 0;
  effect->scaleX = 0;
  while (leader->y < px(144)) {
    leader->y += px(1);
    effect->y += px(1);
    System_Wait(1);
  }
  effect->scaleX = Q16_ONE / 2;
  effect->scaleY = Q16_ONE * 3 / 4;
  for (i = 31; i >= 0; i--) {
    effect->scaleX += 0x800;
    effect->scaleY += 0x200;
    System_Wait(1);
  }
  effect->scaleX = Q16_ONE * 3 / 2;
  effect->scaleY = Q16_ONE;
  Actor_SelectAnimation(effect, 6);
  leader->scaleX = Q16_ONE * 3 / 8;
  leader->scaleY = Q16_ONE * 3 / 8;
  i = 19;
spin:
  leader->facing += 0x2000;
  leader->scaleX += 0x800;
  leader->scaleY = leader->scaleX;
  System_Wait(1);
  i--;
  if (i >= 0)
    goto spin;
  Actor_Release(effect);
  Actor_SetSpriteFlags(Actor_Get(owner), 1);
  leader->facing = 0x4000;
  leader->terrainId = 1;
  leader->motionFlags = 3;
  leader->y = px(144);
  leader->terrainHeight = px(144);
  FieldTask_Remove(Func_020000b4);
  Func_02000054();
  Event_End();
}

#define DEFINE_JUPITER_LIGHTHOUSE_ROOM_2_SET_STATE(name, n1)                   \
  void name(void) {                                                            \
    struct FieldWork *event = Ram_HeapSlots->fieldWork;                        \
    u8 *effect = Ram_HeapSlots->effectWork;                                    \
                                                                               \
    event->raisedTrigger = n1;                                                 \
    effect[52] = 1;                                                            \
  }

DEFINE_JUPITER_LIGHTHOUSE_ROOM_2_SET_STATE(Func_020003f4, 30)

DEFINE_JUPITER_LIGHTHOUSE_ROOM_2_SET_STATE(Func_02000410, 31)

void Func_0200042c(void) { gFieldWork->raisedTrigger = 0x20; }

void Func_02000440(void) { gFieldWork->raisedTrigger = 0x21; }

/// Once the leader stands at height 32 or more, it rises and spins for 110
/// frames.
void Func_02000454(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 i;
  s32 angle;
  s32 y;

  y = object->y;
  if (y >= 0x200000) {
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 2);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_WalkTo(gMainRoutineWork.currentOwner, 216, 680);
    Actor_SetAnimationSpeed(object, 64);
    object->motionFlags = 0;
    for (i = 109; i >= 0; i--) {
      object->y += px(3);
      angle = object->facing;
      angle += 0x1800;
      object->facing = angle;
      System_Wait(2);
    }
    FieldEvent_SetExitRequest(12);
  }
}

void Func_020004d0(s32 a0, s32 a1) {

  Actor_Get(a1)->motionFlags = 0;
  FieldActor_SetSpritePriority(a1, 1);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
}

static inline void CopyCells(s32 srcX, s32 srcY, s32 width, s32 height,
                             s32 destX, s32 destY) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline. */
  MapGrid_CopyCellPictureCellsRect(srcX, srcY, width, height, destX, destY);
}

/// Actor id comes down: once it rests on the terrain (or after 30 frames) cue
/// 188 plays, and at cell (20, 39) it sets flag 0xa02 and copies map cells in
/// three steps, each with cue 167.
void Func_02000500(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  struct MainRoutineWork *party;
  s32 i;

  object->motionFlags = 3;
  Actor_Get(id)->priorityFlags |= 1;
  party = &gMainRoutineWork;
  Actor_Get(party->currentOwner)->priorityFlags |= 1;
  if (object->x >> 20 == 20 && object->z >> 20 == 39)
    FieldActor_SetSpritePriority(id, 3);
  if (object->terrainHeight !=
      Map_GetTerrainHeight(object->terrainId, object->x, object->z)) {
    System_Wait(2);
    for (i = 0; i <= 29 &&
                (object->y > object->terrainHeight || object->velocityY != 0);
         i++)
      System_Wait(1);
    Event_PlaySound(SE_THUMP);
  }
  if (object->x >> 20 == 20 && object->z >> 20 == 39) {
    Event_SetFlag(0xa02);
    Actor_SetSpriteFlags(Actor_Get(id), 0);
    FieldActor_SetSpritePriority(id, 3);
    Actor_SetPosition(id, 0, 0);
    MapGrid_CopyCellAttributes(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(92, 39, 1, 1, 84, 39);
    FieldEvent_Wait(30);
    Event_PlaySound(SE_MAP_TRANSITION);
    MapGrid_CopyCellsLayered(92, 40, 1, 1, 84, 40);
    FieldEvent_Wait(20);
    Event_PlaySound(SE_MAP_TRANSITION);
    CopyCells(92, 41, 1, 1, 84, 41);
    FieldEvent_Wait(20);
    Event_PlaySound(SE_MAP_TRANSITION);
    MapGrid_CopyCellsLayered(93, 41, 1, 1, 85, 41);
    FieldMapGrid_SetCellBehavior(168 << 17, 164 << 18, 2, 236);
  }
}

void Func_02000674(s32 a0, s32 a1) { Actor_SetSpritePriority(a1, 2); }

void Func_02000680(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 i;

  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) &&
      object->x >> 20 == 18 && object->z >> 20 == 6) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += px(2);
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(0xa03);
  }
}

void Func_020006dc(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode == 10)
    FieldMapGrid_SetCellBehavior(px(336), px(592), 0, 255);
}

void Func_02000704(void) {
  MapGrid_SetCellBehavior(0x1500000, 0x2500000, 0, 4);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// A particle's step: it circles at its angle (action) around x 214 and
/// rises with its phase (unk66), drawn in front while the sine is
/// negative and behind otherwise.
void Func_02000720(struct Actor *object) {
  s16 angle = object->timer;
  s16 height = object->unk66;

  SET_POSITION(object,
               Iwram_MulQ16(Trig_Cos(angle), object->speedLimit) + (214 << 16),
               (height * 3 << 16) + 0x80000, 170 << 18);
  // Read signed, as the phase is above; the field is declared unsigned.
  if ((s16)object->unk66 == 0)
    Actor_SelectAnimation(object, 7);
  if (object->y > 170 << 17)
    object->unk66 = 0xffff;
  if (Math_Sin(angle) < 0)
    ((struct FieldSprite *)object->sprite)->priority = 1;
  else
    ((struct FieldSprite *)object->sprite)->priority = 2;
  object->timer += object->acceleration;
  object->unk66++;
}

void Func_020007d0(void) {
  s32 i;
  s32 phase;
  struct Actor *object;

  for (i = 0, phase = 0; i != 22; i++, phase += 5) {
    object = (FieldActor_Create(672, 0, 0, 0));
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16() << 2) + 0x60000;
    object->acceleration = ((Random16() << 11) >> 16) + 0x800;
    object->update = Func_02000720;
    object->timer = Random16();
    object->unk66 = phase;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->sprite)->priority = 1;
  }
}

/// The scene's set-up entry: screen effect 0x204, actors 13 and 14 at action
/// variant 3, the blend registers, the particles, and the map and actors by
/// flags 0xa02, 0xa00 and 0xa03 and the entrance.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = 0x204;
  FieldActor_SetSpritePriority(13, 3);
  FieldActor_SetSpritePriority(14, 3);
  Io_Write16(0x3f06, &REG_BLDCNT);
  Io_Write16(0x1000, &REG_BLDALPHA);
  BattleEffect_FillOverlayTiles(0);
  Call4((void (*)())EffectSprite_InitFromSlot94Blended, (s32)gData_02002b60, 0,
        0, 0x80000000);
  Func_02000054();
  Func_020007d0();
  if (Event_GetFlag(0xa02)) {
    Actor_SetPosition(17, 0, 0);
    MapGrid_CopyCellAttributes(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(92, 39, 2, 3, 84, 39);
    FieldMapGrid_SetCellBehavior(168 << 17, 164 << 18, 2, 236);
  } else {
    LightSprites_InitB();
    Func_020015a0(17, 0);
  }
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 6 ||
      gMainRoutineWork.entrance == 7)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(8);
    Actor_RaiseFromCell(9);
    Actor_RaiseFromCell(10);
    Actor_RaiseFromCell(11);
  }
  if (Event_GetFlag(0xa03)) {
    Actor_SetPosition(12, 296, 104);
    Actor_RaiseFromCell(12);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

void Func_020009dc(s32 a0) { gPaletteCyclePausesInEvents = a0; }

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

#include "shared/palette_cycle_step_at.h"

extern u16 gLighthousePalette13[];

void JupiterLighthouse_LoadPalettes(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gLighthousePalette13, (void *)&BG_PLTT_COLOR(13, 0),
                         32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    Counter_Set(0);
    Task_Add(PaletteCycle_StepAt, 1152);
  } else {
    Dma_Set(gPaletteCycleUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}

void MapGrid_SetCellHeight();
/* VIEW(jupiter_lighthouse_room_2): routine MapGrid_SetAttributeBAt as void
 * MapGrid_SetAttributeBAt(); module/scene/scene.h has void
 * MapGrid_SetAttributeBAt(s32 actor); tried as the definition's u8
 * MapGrid_SetAttributeBAt(s32 x, s32 z, s32 layer, s32 value), which the object
 * already declares, it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_2.c:539: too many arguments to
 * function `MapGrid_SetAttributeBAt'. */
extern void
Func_0200230c__jupiter_lighthouse_room_2() __asm__("MapGrid_SetAttributeBAt");

void Actor_RaiseFromCell(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  object->bounce = 0;
  object->y = height + 0x200000;
  object->priorityFlags |= 128;
  height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
  MapGrid_SetCellHeight(object->x, object->z, 0, (height >> 19) + 6);
  Func_0200230c__jupiter_lighthouse_room_2(object->x, object->z, 0, 128);
}

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

extern s16 gLightSpritesLarge;

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

void Func_020015a0(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object) {
    object->timer = value;
    {
      s16 *count;

      count = &gLightSpriteCount;
      gLightSpriteActors[*count] = object;
      (*count)++;
    }
  }
}

void Func_020015d0(s32 a0) { gLightSpritePriority = a0; }

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#undef DEFINE_JUPITER_LIGHTHOUSE_ROOM_2_SET_STATE
extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SelectAnimation(struct Actor *object, s32 a);
void Actor_SetSpriteFlags(struct Actor *object, u32 value);

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
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"

void Func_0200042c(void);

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
      SCENE_EXITS(JupiterLighthouseRoom2),         SCENE_EXIT(1, JupiterLighthouseRoom1, 1),   SCENE_EXIT(2, JupiterLighthouseRoom2, 3),   
      SCENE_EXIT(3, JupiterLighthouseRoom2, 2),    SCENE_EXIT(4, JupiterLighthouseRoom2, 5),   SCENE_EXIT(5, JupiterLighthouseRoom2, 4),   
      SCENE_EXIT(6, JupiterLighthouseRoom3, 6),   SCENE_EXIT(7, JupiterLighthouseRoom3, 7),  SCENE_EXIT(8, JupiterLighthouseRoom3, 8),   
      SCENE_EXIT(9, JupiterLighthouseRoom3, 9),   SCENE_EXIT(10, JupiterLighthouseRoom3, 10), SCENE_EXIT(11, JupiterLighthouseRoom3, 11), 
      SCENE_EXIT(12, JupiterLighthouseRoom4, 11), SCENE_EXITS_END,                                
  };

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 104, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 104, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 104, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 104, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 600, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 600, FACING_SOUTHEAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 728, FACING_SOUTHWEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 728, FACING_SOUTHEAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_02000454)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(Func_020003f4)},
    {TRIGGER_RAISED, 30, CONDITION_ALWAYS, EVENT(Func_02000230)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(Func_02000410)},
    {TRIGGER_RAISED, 31, CONDITION_ALWAYS, EVENT(Func_02000230)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(Func_0200042c)},
    {TRIGGER_RAISED, 32, CONDITION_ALWAYS, EVENT(Func_020000d0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33, CONDITION_ALWAYS, EVENT(Func_02000440)},
    {TRIGGER_RAISED, 33, CONDITION_ALWAYS, EVENT(Func_020000d0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(Func_020004d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(Func_02000500)},
    {TRIGGER_PUSH, 35, CONDITION_ALWAYS, EVENT(Func_020004d0)},
    {TRIGGER_PUSHED, 35, CONDITION_ALWAYS, EVENT(Func_02000500)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(Func_02000674)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT(Func_02000680)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_020006dc)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(Func_02000704)},
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
