/// @file
#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
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
#include "event_work.h"
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
#include "shared/cracked_tile.h"
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
    FieldMapGrid_SetCellBehavior(0x900000, 0x2400000, 2, 0xea);
    FieldMapGrid_SetCellBehavior(0x1000000, 0x2300000, 2, 0xea);
  } else {
    FieldMapGrid_SetCellBehavior(0x900000, 0x2400000, 2, 0xeb);
    FieldMapGrid_SetCellBehavior(0x1000000, 0x2300000, 2, 0xeb);
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
    leader->y += -0x10000;
    System_Wait(1);
  }
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  effect = Actor_Spawn(280, leader->x, leader->y, leader->z);
  Actor_SelectAnimation(effect, 2);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  ((struct FieldSprite *)effect->sprite)->priority = 1;
  while (leader->y > 0x200000) {
    leader->y += -0x10000;
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
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  effect = Actor_Spawn(280, leader->x, leader->y, leader->z);
  Actor_SelectAnimation(effect, 2);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  ((struct FieldSprite *)effect->sprite)->priority = 1;
  while (leader->y < 0x6c0000) {
    leader->y += 0x10000;
    leader->facing += 0x2000;
    effect->x = leader->x;
    effect->y = leader->y + -0x60000;
    effect->z = leader->z;
    System_Wait(1);
  }
  leader->scaleX = 0;
  effect->scaleX = 0;
  while (leader->y < 0x900000) {
    leader->y += 0x10000;
    effect->y += 0x10000;
    System_Wait(1);
  }
  effect->scaleX = 0x8000;
  effect->scaleY = 0xc000;
  for (i = 31; i >= 0; i--) {
    effect->scaleX += 0x800;
    effect->scaleY += 0x200;
    System_Wait(1);
  }
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  Actor_SelectAnimation(effect, 6);
  leader->scaleX = 0x6000;
  leader->scaleY = 0x6000;
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
  leader->y = 0x900000;
  leader->terrainHeight = 0x900000;
  FieldTask_Remove(Func_020000b4);
  Func_02000054();
  Event_End();
}

#define DEFINE_JUPITER_LIGHTHOUSE_ROOM_2_SET_STATE(name, n1)                   \
  void name(void) {                                                            \
    struct EventWork *event = Ram_HeapSlots->eventWork;                        \
    u8 *effect = Ram_HeapSlots->effectWork;                                    \
                                                                               \
    event->unk16a = n1;                                                        \
    effect[52] = 1;                                                            \
  }

DEFINE_JUPITER_LIGHTHOUSE_ROOM_2_SET_STATE(Func_020003f4, 30)

DEFINE_JUPITER_LIGHTHOUSE_ROOM_2_SET_STATE(Func_02000410, 31)

void Func_0200042c(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 0x20;
}

void Func_02000440(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 0x21;
}

/// Once the leader stands at height 32 or more, it rises and spins for 110
/// frames.
void Func_02000454(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 i;
  s32 angle;
  s32 y;

  y = object->y;
  if (y >= 0x200000) {
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_WalkTo(gMainRoutineWork.currentOwner, 216, 680);
    Actor_SetAnimationSpeed(object, 64);
    object->motionFlags = 0;
    for (i = 109; i >= 0; i--) {
      object->y += PIXELS(3);
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
  /* FAKEMATCH(wrapper): the call forwarded through this inline, as in ☀️'s
   * scene layer, so each stack argument keeps its own register; as a direct
   * call, 10 of 157 instructions of Func_02000500 differ in tla-en. */
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
    Event_PlaySound(188);
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
    Event_PlaySound(167);
    MapGrid_CopyCellsLayered(92, 40, 1, 1, 84, 40);
    FieldEvent_Wait(20);
    Event_PlaySound(167);
    CopyCells(92, 41, 1, 1, 84, 41);
    FieldEvent_Wait(20);
    Event_PlaySound(167);
    MapGrid_CopyCellsLayered(93, 41, 1, 1, 85, 41);
    FieldMapGrid_SetCellBehavior(168 << 17, 164 << 18, 2, 236);
  }
}

void Func_02000674(s32 a0, s32 a1) { Actor_SetSpritePriority(a1, 2); }

void Func_02000680(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 i;

  if (Event_GetFlag(0xa00) && object->x >> 20 == 18 && object->z >> 20 == 6) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(0xa03);
  }
}

void Func_020006dc(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode == 10)
    FieldMapGrid_SetCellBehavior(0x1500000, 0x2500000, 0, 255);
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
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
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
  if (Event_GetFlag(0xa00)) {
    Actor_RaiseFromCell(8);
    Actor_RaiseFromCell(9);
    Actor_RaiseFromCell(10);
    Actor_RaiseFromCell(11);
  }
  if (Event_GetFlag(0xa03)) {
    Actor_SetPosition(12, 148 << 17, 208 << 15);
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

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gPaletteCycleFrame,
                             gPaletteCyclePausesInEvents, gPaletteCycleColors,
                             0x05000184)

extern u16 gLighthousePalette13[];

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gLighthousePalette13,
                                        PaletteCycle_StepAt,
                                        gPaletteCycleUnlitColors)

void MapGrid_SetCellHeight();
/* VIEW(jupiter_lighthouse_room_2): routine MapGrid_SetAttributeBAt as void
 * MapGrid_SetAttributeBAt(); module/scene/scene.h has void
 * MapGrid_SetAttributeBAt(s32 actor); not tried: MapGrid_SetAttributeBAt's
 * bound declaration was not found. */
extern void
Func_0200230c__jupiter_lighthouse_room_2() __asm__("MapGrid_SetAttributeBAt");

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             Func_0200230c__jupiter_lighthouse_room_2)

DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(Whirlwind_CarryLeaderLower)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Whirlwind_UpdateLower, MapGrid_GetCellBehavior,
                              Position_FlatDistance, gWhirlwindEndScript,
                              gWhirlwindRiding)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Actor_SelectAnimation,
                       Whirlwind_UpdateLower, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

extern s16 gLightSpritesLarge;

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

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
// @file
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

DEFINE_CRACKED_TILES_FIND_LOWER(CrackedTiles_FindLower, gCrackedTiles,
                                CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

struct CellTargetEntry;

/* VIEW(s1c8c): tag struct CellTarget as struct CellTarget__s1c8c; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1c8c {
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

DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s1c8c,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef Actor_StartScript
// @file
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void CrackedTile_Crack();
void CrackedTile_Fall();

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)

void Func_0200042c(void);

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_3(255, 254, 256, 6, 7, 256, 9, 10, 11, 257)

struct ActorPlacement gActorPlacements[] = {
    {316, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(104), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(104), FACING_EAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(600), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(600), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(728), FACING_SOUTHWEST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(728), FACING_SOUTHEAST, 2, 1},
    {428, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(648), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {33, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
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
    {8, 35, CONDITION_ALWAYS, EVENT(Func_020004d0)},
    {9, 35, CONDITION_ALWAYS, EVENT(Func_02000500)},
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
