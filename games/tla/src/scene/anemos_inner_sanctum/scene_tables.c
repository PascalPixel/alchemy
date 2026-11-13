/// @file
/// Scene code of overlay 0x6b0, in ROM order.

#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "actor.h"

#include "macros.h"
#include "field.h"
#include "module/scene/anemos_inner_sanctum.h"

void Actor_SelectAnimation(struct Actor *, s32);
void Actor_StartScript(struct Actor *object, const s32 *script);
extern const s32 *gData_02003840[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party = ((struct FieldEffect *)Actor_GetById(0));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect, gData_02003840[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    // Read as its low byte: (u8)options->priority orders the loads apart.
    sprite->priority = *(u8 *)&options->priority;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gData_02003840[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - Q16_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - Q16_ONE) / script[3];
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

s32 Func_02000654(s32 a0) {
  Actor_SetSpriteFlags(a0, 0);
  return 0;
}

s32 Func_02000660(struct Actor *object) {
  s32 radius;

  object->timer += object->unk66;
  SET_POSITION(object, object->bounce, object->gravity, 0x980000);
  FieldVector_AddPolarOffset(object->unk4c, object->timer, &object->x);
  radius = object->unk4c - 0x10000;
  object->unk4c = radius;
  if (radius)
    return 1;
  return 0;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 gData_02003674[];

void Func_020006a4(struct Actor *source) {
  struct Actor *object =
      (FieldActor_Create(672, source->x, source->y, source->z));

  object->motionFlags = 0;
  FieldActor_SetSpriteColor(object, BYTE_AT(source, 98));
  Scene_SetActorSpritePriority(object, 2);
  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gData_02003674);
  Actor_SetBlendMode(object, 1);
}

s32 Func_020006f8(struct Mover *mover) {
  if (mover->radius < mover->step)
    return 0;
  mover->angle += mover->spin >> 1;
  mover->x = mover->originX;
  mover->y = mover->height;
  mover->z = mover->originZ;
  // Read through a pointer of its own type, an access alchemy-gcc keeps apart
  // from the s32 stores before it: as mover->angle, the angle loads before
  // the radius (2 of 73 instructions differ).
  FieldVector_AddPolarOffset(mover->radius, *(s16 *)&mover->angle, &mover->x);
  Func_020006a4(mover);
  mover->radius -= mover->step >> 1;
  mover->angle += mover->spin >> 1;
  mover->x = mover->originX;
  mover->y = mover->height;
  mover->z = mover->originZ;
  FieldVector_AddPolarOffset(mover->radius, mover->angle, &mover->x);
  Func_020006a4(mover);
  mover->radius -= mover->step >> 1;
  mover->height -= mover->fall;
  return 1;
}

s32 Func_02000794(struct Actor *object) {
  struct SpawnParams params;
  s32 x;
  s32 y;

  params.kind = 2;
  params.part = *((u8 *)object + 98);
  FieldEffect_Spawn(object->x, object->y, object->z, 0, 0x10000, 0, 0x30000,
                    &params);
  x = (object->bounce >> 20) + 64;
  y = object->gravity >> 20;
  MapGrid_CopyCellsTo(x, y + 26, x, y, 1, 1);
  return 0;
}

#include "../../field/object/wait_until_idle.c"

// ---- scene/anemos_inner_sanctum/scene_tables.c ----

/// Scene code of overlay 0x6b0, in ROM order.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "module/scene/anemos_inner_sanctum.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "main_routine_work.h"

extern struct ActorPlacement gData_02003b38[];
extern struct ActorPlacement gData_02003b50[];
extern struct ActorPlacement gData_02003b68[];
extern struct ActorPlacement gData_02003bc8[];
extern const u8 gData_02003c88[];
extern struct ActorPlacement gData_02003ce8[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room1)
    return gData_02003b50;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room2)
    return gData_02003b68;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3)
    return gData_02003bc8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
    return gData_02003c88;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room5)
    return gData_02003ce8;
  return gData_02003b38;
}

#include "system/iwram_call.h"
#include "event_inline.h"

struct Work {
  u8 pad0[8];
  struct Vector3 position;
  s32 unk14;
  u8 pad18[0x18];
  s32 unk30;
  s32 unk34;
  u8 pad38[0x0c];
  s32 unk44;
  s32 unk48;
  s32 unk4c;
  u8 pad50[5];
  u8 unk55;
  u8 pad56[0x0c];
  u8 unk62;
  u8 pad63;
  s16 unk64;
  u16 unk66;
};

void Actor_SelectAnimation(struct Actor *, s32);
void Actor_StartScript();
extern u8 gData_02003684[];

void Func_0200088c(struct Vector3 *from, struct Vector3 *to, s16 part) {
  struct Work *actor;
  s32 x, z, distance;

  actor = ((struct Work *)FieldActor_Create(0x2a0, from->x, from->y, from->z));
  x = (from->x - to->x) >> 16;
  z = (from->z - to->z) >> 16;
  actor->unk64 = ArcTan2(from->z - to->z, from->x - to->x);
  actor->unk66 = (((Random16() & 2) - 1) * (Random16() & 15)) << 8;
  actor->unk62 = part;
  actor->unk55 = 0;
  actor->unk44 = to->x;
  actor->unk48 = to->z;
  distance = Iwram_Sqrt(x * x + z * z) << 16;
  actor->unk4c = distance;
  actor->unk30 = distance / 16;
  actor->unk34 = from->y;
  actor->unk14 = (from->y - to->y) / 16;
  Actor_SetBlendMode(actor, 1);
  FieldActor_SetSpriteColor(actor, (u16)part);
  Scene_SetActorSpritePriority(actor, 2);
  Actor_SelectAnimation(actor, 7);
  Actor_SetSpriteFlags(actor, 0);
  FieldVector_AddPolarOffset(actor->unk4c, actor->unk64, &actor->position);
  Actor_StartScript(actor, gData_02003684);
  Func_020006f8(actor);
}

#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_020009b8(s32 x, s32 y, s16 part, s16 a3) {
  s32 i;

  for (i = 0; i != 16;) {
    struct Actor *object = (FieldActor_Create(0x2a0, x, y, 0x980000));
    s32 angle = i << 12;

    FieldVector_AddPolarOffset(0x200000, angle, &object->x);
    object->timer = angle;
    object->unk66 = a3;
    BYTE_AT(object, 98) = i;
    BYTE_AT(object, 99) = part;
    object->bounce = x;
    object->unk4c = 0x200000;
    object->gravity = y;
    object->motionFlags = 0;
    Actor_SetBlendMode(object, 1);
    FieldActor_SetSpriteColor(object, (u16)part);
    Scene_SetActorSpritePriority(object, 2);
    Actor_SelectAnimation(object, 7);
    Actor_SetSpriteFlags(object, 0);
    Actor_StartScript(object, gData_02003584);
    i += 2;
  }
}

void Func_02000a78(s32 a0, s32 a1, s16 a2) {
  struct Actor *t0;

  t0 = FieldActor_Create(0x2a0, (a0 << 16), ((a1 + 3) << 16), 0x980000);
  t0->motionFlags = 4;
  FieldActor_SetSpriteColor(t0, (u16)a2);
  Scene_SetActorSpritePriority(t0, 2);
  Actor_SelectAnimation(t0, 7);
  Actor_SetSpriteFlags(t0, 0);
}

// ---- scene/anemos_inner_sanctum/s0ac8.c ----

/// Scene code of overlay 0x6b0, in ROM order.

#include "../shared/actors.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"
#include "module/scene/anemos_inner_sanctum.h"
#include "text/msg_ids.h"

/* VIEW(s0ac8): tag struct Work as struct Work__s0ac8;
 * scene/anemos_inner_sanctum/scene_tables has another definition; tried with
 * the definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/anemos_inner_sanctum/scene_tables.c:357: structure has no
 * member named `room'. */
struct Work__s0ac8 {
  u8 pad[340];
  u16 room;
};

/* VIEW(s0ac8): routine Func_0200088c as void Func_0200088c(), with no
 * prototype, as the part calls it; scene/anemos_inner_sanctum/scene_tables has
 * void Func_0200088c(struct Vector3 *, struct Vector3 *, s16); tried as the
 * definition's void Func_0200088c(struct Vector3 *from, struct Vector3 *to, s16
 * part), which the object already declares, Func_02000ac8 moves: 7 of 630
 * instructions in tla-ja. */
void Func_0200088c__s0ac8() __asm__("Func_0200088c");
#define Func_0200088c Func_0200088c__s0ac8
/* VIEW(s0ac8): routine Func_020009b8 as void Func_020009b8(), with no
 * prototype, as the part calls it; scene/anemos_inner_sanctum/scene_tables has
 * void Func_020009b8(s32, s32, s16, s16); tried as the definition's void
 * Func_020009b8(s32 x, s32 y, s16 part, s16 a3), which the object already
 * declares, Func_02000ac8 moves: 10 of 630 instructions in tla-ja. */
void Func_020009b8__s0ac8() __asm__("Func_020009b8");
#define Func_020009b8 Func_020009b8__s0ac8
/* VIEW(s0ac8): routine Func_02000a78 as void Func_02000a78(), with no
 * prototype, as the part calls it; scene/anemos_inner_sanctum/scene_tables has
 * void Func_02000a78(s32, s32, s16); tried as the definition's void
 * Func_02000a78(s32 a0, s32 a1, s16 a2), which the object already declares,
 * Func_02000ac8 moves: 1 of 630 instructions in tla-ja. */
void Func_02000a78__s0ac8() __asm__("Func_02000a78");
#define Func_02000a78 Func_02000a78__s0ac8
void Actor_StartScript();
void BlendScript_Resume(void);
extern u8 gData_020035cc[];
extern u8 gData_02003620[];
extern s32 gData_020036fc[][2];

void Func_02000ac8(void) {
  struct Work__s0ac8 *work = ((struct Globals *)Ram_HeapSlots)->work;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  volatile u16 alpha = REG_BLDALPHA;
  s32 x, z;
  s32 targetX = 0;
  s32 targetZ = 0;
  s16 type = work->room + 193;
  s16 room = work->room;
  s32 index = room - 50;
  s32 base;
  u8 count;
  s32 part;
  s32 column, row, t0, t1;
  s32 i;
  u8 last;
  struct Actor *actor;
  u8 found[20];
  u8 order[20];
  struct Vector3 from;
  struct Vector3 to;

  x = ((index & 1) << 8) + 408;
  base = 0;
  count = 0;
  part = 0;
  z = (index & 2) * 3 * 32 + 168;
  switch (room) {
  case 50:
    base = 48;
    targetX = 0x208;
    targetZ = 48;
    if (GameFlag_Get(0xa87))
      goto seen;
    break;
  case 51:
    part = 3;
    base = 68;
    targetX = 0x228;
    targetZ = 48;
    if (GameFlag_Get(0xa88))
      goto seen;
    break;
  case 52:
    part = 10;
    base = 88;
    targetX = 0x1f8;
    targetZ = 40;
    if (GameFlag_Get(0xa89))
      goto seen;
    break;
  case 53:
    part = 2;
    base = 108;
    targetX = 0x238;
    targetZ = 40;
    if (GameFlag_Get(0xa8a)) {
    seen:
      GameFlag_Set(0x200);
      return;
    }
    break;
  }

  i = (u16)base;
  if (i != (u16)base + 18) {
  scan:
    if (GameFlag_Get(i)) {
      found[count] = i - base;
      count++;
    }
    i++;
    if (i != (u16)base + 18)
      goto scan;
  }
  if (count == 0)
    return;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Actor_WalkTo(gMainRoutineWork.currentOwner, x, z);
  FieldCamera_MoveTo(x << 16, -1, z << 16, 1);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  actor =
      (FieldActor_Create((u16)type, leader->x, leader->y + 0x80000, leader->z));
  actor->motionFlags = 0;
  actor->scaleY = 0x1999;
  actor->scaleX = 0x1999;
  Actor_SetSpriteFlags((s32)actor, 0);
  Actor_StartScript(actor, gData_020035cc);
  Actor_SetBlendMode(actor, 1);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(60);
  BlendScript_Suspend();
  alpha = alpha & 255;
  REG_BLDALPHA = 0x1000;
  t0 = x >> 4;
  t1 = z >> 4;
  column = t0;
  row = t1;
  MapGrid_CopyCellsTo(t0 + 63, t1 + 23, t0 + 63, t1 - 3, 3, 5);
  i = 0;
  if (alpha != 0) {
  fadeIn:
    REG_BLDALPHA = i | 0x1000;
    System_Wait(8);
    i++;
    if (i != alpha)
      goto fadeIn;
  }
  BlendScript_Resume();
  Event_PlaySound(246);
  FieldEvent_Wait(48);

  last = count - 1;
  for (i = 0; i != count; i++) {
    s32 pick = Random16();
    pick &= last;

    order[i] = found[pick];
    found[pick] = found[last];
    last--;
  }
  for (i = 0; i != count;) {
    s32 slot;

    from.x = leader->x + ((Random16() & 31) << 16) - 0x100000;
    from.y = leader->y + ((Random16() & 15) << 16) + 0x80000;
    from.z = leader->z + ((Random16() & 15) << 16) - 0x80000;
    slot = order[i];
    to.x = leader->x + (gData_020036fc[slot][0] << 16);
    to.y = leader->y;
    to.z = leader->z + (gData_020036fc[slot][1] << 16);
    Func_0200088c(&from, &to, part);
    if (i & 1)
      Event_PlaySound(195);
    i++;
    FieldEvent_Wait(8);
  }

  if (count != 18) {
    FieldEvent_Wait(96);
    Actor_StartScript(actor, gData_02003620);
    BlendScript_Suspend();
    alpha = REG_BLDALPHA & 255;
    PaletteFade_ApplyTarget(0x10000, 1);
    PaletteFade_Interpolate(60);
    for (i = alpha; i != 0;) {
      REG_BLDALPHA = i | 0x1000;
      i--;
      System_Wait(8);
    }
    for (i = 0; i != count;) {
      s32 slot = order[i];

      i++;
      MapGrid_CopyCellsTo(
          64, 0, ((leader->x + (gData_020036fc[slot][0] << 16)) >> 20) + 64,
          (leader->z + (gData_020036fc[slot][1] << 16)) >> 20, 1, 1);
    }
    MapGrid_CopyCellsTo(64, 0, column + 63, row - 3, 3, 5);
    BlendScript_Resume();
    Event_WaitTransitionFrames();
  } else {
    Event_SetFlag((s16)work->room + 0xa55);
    Actor_StartScript(actor, gData_02003620);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(0x40000, 0x8000);
    FieldCamera_MoveTo(0x2180000, -1, 0x780000, 1);
    FieldCamera_WaitForArrival();
    Event_PlaySound(220);
    {
      s32 a = targetX << 16;
      s32 b = targetZ << 16;

      Func_020009b8(a, b, part, 0x800);
      FieldEvent_Wait(10);
      Func_020009b8(a, b, part, 0x900);
      FieldEvent_Wait(10);
      Func_020009b8(a, b, part, 0xa00);
    }
    FieldEvent_Wait(12);
    System_Wait(16);
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(1);
    System_Wait(1);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(32);
    Event_PlaySound(0x23e);
    Func_02000a78(targetX, targetZ, part);
    FieldEvent_Wait(80);
    if (GameFlag_Get(0xa87) && GameFlag_Get(0xa88) && GameFlag_Get(0xa89) &&
        GameFlag_Get(0xa8a)) {
      Event_SetFlag(0xa8b);
      Event_PlaySound(188);
      MapGrid_CopyCellsTo(52, 7, 33, 7, 1, 2);
      System_Wait(5);
      MapGrid_CopyCellsTo(54, 7, 33, 7, 1, 2);
      MapGrid_SetCellHeight(0x2180000, 0x880000, 0, 0);
      System_Wait(60);
    }
  }

  Actor_Release(actor);
  Event_SetFlag(0x200);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Event_End();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001090(void) { GameFlag_Clear(0x200); }

extern u8 gData_020036e4[];
extern u8 gData_020036ea[];
extern u8 gData_020036f6[];

void Func_0200109c(void) {
  if (gMainRoutineWork.scene == (s32)MsgHat) {
    Actor_LiftTargetBlock((s32)gData_020036e4);
  } else {
    if (gMainRoutineWork.scene == (s32)MsgCrownBoostsLuck) {
      Actor_LiftTargetBlock((s32)gData_020036ea);
      Field_SetCellBehaviorAt(2, 0x480000, 0x1b80000, 0xff);
    } else {
      if (gMainRoutineWork.scene == (s32)MsgHatBoostsAgility) {
        Actor_LiftTargetBlock((s32)gData_020036f6);
        Field_SetCellBehaviorAt(2, 0x1180000, 0x1580000, 0xff);
        Field_SetCellBehaviorAt(2, 0x1180000, 0x1780000, 0xff);
        Field_SetCellBehaviorAt(2, 0x1280000, 0x1680000, 0xff);
        Field_SetCellBehaviorAt(2, 0x1180000, 0x1680000, 0xe8);
        Field_SetCellBehaviorAt(2, 0x2080000, 0x2780000, 0xff);
        Field_SetCellBehaviorAt(2, 0x2080000, 0x2680000, 0xe8);
      }
    }
  }
}

#include "scene_tables.h"

/// An event handler from the sanctum's event table (in s2f48.c),
/// reading the leader's place in 8-pixel cells: sets the cells of rooms 3 and
/// 4, and sets flags 0xa80 to 0xa86 where the leader stands on given cells,
/// running the event at x 67, z 45 of room 3 and moving object 13 to object 12
/// at x 91, z 59 until flag 0xa84 is set.
void Func_02001160(void) {
  struct Actor *leader =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 x;
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  x = leader->x >> 19;
  z = leader->z >> 19;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3) {
    Field_SetCellBehaviorAt(2, 0x480000, 0x1b80000, 232);
    if (x != 8)
      Field_SetCellBehaviorAt(2, 0x480000, 0x1a80000, 232);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4) {
    Field_SetCellBehaviorAt(2, 0x1180000, 0x1580000, 0);
    Field_SetCellBehaviorAt(2, 0x1180000, 0x1780000, 232);
    Field_SetCellBehaviorAt(2, 0x1280000, 0x1680000, 0);
    Field_SetCellBehaviorAt(2, 0x2080000, 0x2780000, 232);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room2) {
    if (x == 37 && z == 31) {
      GameFlag_Set(0xa80);
      Actor_SetSpritePriority(8, 2);
    }
    if (x == 55 && z == 95)
      GameFlag_Set(0xa81);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3) {
    if (x == 7 && z == 53)
      GameFlag_Set(0xa82);
    if (x == 67 && z == 45) {
      MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
      Actor_SetPosition(14, 0, 0);
      Event_PlaySound(0x248);
      FieldEvent_Wait(30);
      Event_PlaySound(188);
      MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
      FieldEvent_Wait(5);
      MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
      FieldEvent_Wait(5);
      MapGrid_SetCellHeight(0x1f00000, 0x1500000, 0, 0);
      GameFlag_Set(0xa83);
    }
    if (x == 93 && z == 57)
      GameFlag_Set(0xa84);
    if (x == 91 && z == 59) {
      if (!Event_GetFlag(0xa84)) {
        s32 fromX = (Actor_Get(12)->x >> 16) << 16;

        Actor_SetPosition(13, fromX, (Actor_Get(12)->z >> 16) << 16);
        Actor_SetPosition(12, PIXELS(728), PIXELS(472));
      }
      GameFlag_Set(0xa85);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4 && x == 33 && z == 45)
    GameFlag_Set(0xa86);
  Event_End();
}

#include "field/event_work.h"

extern const s32 gData_02003574[];

void Actor_SelectAnimation(struct Actor *, s32);

/// An event handler from the sanctum's event table (in s2f48.c):
/// changes the cells at the leader's place, spawns an effect of type 0x15d
/// there and sets the flag the event work's exit selects; once flags 0x200 to
/// 0x206 are all set, it moves object 8 in room 4 (else object 9) to mode 2 and
/// sets flag 0x207.
void Func_02001398(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object;
  s32 all;
  s32 i;
  s32 id;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
    MapGrid_CopyCellsTo(17, 71, leader->x >> 20, (leader->z >> 20) + 64, 1, 1);
  else
    MapGrid_CopyCellsTo(54, 87, leader->x >> 20, (leader->z >> 20) + 64, 1, 1);
  Event_PlaySound(106);
  object = FieldActor_Create(0x15d, ((leader->x >> 20) << 20) + 0x80000, 0,
                             ((leader->z >> 20) << 20) + 0x80000);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  FieldActor_SetSpriteColor(object, 4);
  object->scaleY = 0x16000;
  object->scaleX = 0x16000;
  Scene_SetActorSpritePriority(object, 2);
  Actor_SelectAnimation(object, 3);
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gData_02003574);
  Event_SetFlag(work->exit + 0x1ce);
  all = Event_GetFlag(0x200);
  for (i = 1; i != 7; i++)
    all &= Event_GetFlag(0x200 + i);
  if (all) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_Wait(10);
    FieldCamera_SetSpeed(0xcccc, 0x1999);
    id = 9;
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
      id = 8;
    FieldCamera_MoveToActorById(id, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    Event_PlaySound(0x246);
    Actor_SetAnimation(id, 2);
    Event_SetFlag(0x207);
    Actor_Get(id)->motionFlags = 4;
    FieldEvent_Wait(80);
    Event_End();
  }
}

/// Centres the party leader on its cell and raises actor 8 (in Ov6B0Room4) or
/// actor 9 with cue 198, 4 pixels at once and then 128 more, before the screen
/// transition finishes.
void Func_02001518(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4) {
    object = Actor_Get(8);
    FieldEvent_SetExitRequest(13);
  } else {
    object = Actor_Get(9);
    FieldEvent_SetExitRequest(11);
  }
  {
    s32 x = ((leader->x >> 20) << 4) + 8;
    s32 z = ((leader->z >> 20) << 4) + 8;

    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x, z);
  }
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 10);
  FieldEvent_Wait(30);
  object->motionFlags = 0;
  FieldCamera_MoveTo(-1, -1, -1, 0);
  object->speedLimit = 0x20000;
  object->acceleration = 0x800;
  Event_PlaySound(198);
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x40000, object->z);
  FieldActor_WaitForMove(object);
  Scene_SetActorSpritePriority(leader, 1);
  Scene_SetActorSpritePriority(object, 1);
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x800000, object->z);
  FieldEvent_Wait(80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_02001614(void) {
  struct Actor *t0;
  s32 t1;

  t0 = Actor_Get(gMainRoutineWork.currentOwner);
  t1 = FieldMapGrid_GetCellBehaviorAt(2, t0->x, t0->z);
  if (t1 == 0xe8) {
    t0->terrainId = 2;
  }
}

DEFINE_LEADER_SET_LAYER_BYTE(Leader_SetLayerByte, 0)

void Func_02001660(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 536, 360);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
  Actor_SetPosition(14, 0, 0);
  Event_PlaySound(0x248);
  FieldEvent_Wait(30);
  Event_PlaySound(0xbc);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_SetFlag(0x200);
  Event_End();
}

void Func_020016fc(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(14));
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FIELD_AT(t0, s32, 8) = 0x2180000;
  FIELD_AT(t0, s32, 16) = 0x1680000;
  FIELD_AT(t0, s32, 20) = -917504;
  FIELD_AT(t0, s32, 12) = -917504;
  MapGrid_CopyCellsTo(51, 22, 33, 22, 1, 1);
  Event_PlaySound(0x248);
  FieldEvent_Wait(30);
  Event_PlaySound(0xbc);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(52, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_ClearFlag(0x200);
  Event_End();
}

s32 CrackedTiles_Step();

#define DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(name, n1)                          \
  void name(void) {                                                            \
    s32 t0;                                                                    \
                                                                               \
    t0 = CrackedTiles_Step();                                                  \
    if (t0 != 0) {                                                             \
      FieldEvent_SetExitRequest(n1);                                           \
    }                                                                          \
  }

DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(Func_0200178c, 0xb)

DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(Func_020017a0, 0xc)

#include "frame.h"

extern const s32 gData_02003594[];

/// The update of an object that moves with the leader: once the leader is past
/// z 248 it settles on its cell and clears flag 0x200; otherwise it repeats the
/// leader's step, raising an effect of type 0x2a0 on each step, and stops where
/// the cell kind under it is 52 or 51, setting the event work's unk16a to 201
/// or 200. unk4c and acceleration hold the leader's last x and z.
void Func_020017b4(struct Actor *self) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 next[2];
  s32 ahead[3];
  struct Actor *dust;
  s32 kind;

  if (Actor_Get(gMainRoutineWork.currentOwner)->z >> 16 > 248) {
    self->update = 0;
    Event_ClearFlag(0x200);
    self->x = (self->x & 0xfff00000) + 0x80000;
    self->z = (self->z & 0xfff00000) + 0x80000;
    return;
  }
  ahead[0] = next[0] = self->x - (leader->x - self->unk4c);
  ahead[1] = leader->y;
  ahead[2] = next[1] = self->z + (leader->z - self->acceleration);
  if (self->unk4c != leader->x || self->acceleration != leader->z) {
    dust = (FieldActor_Create(0x2a0, self->x, self->y, self->z));
    dust->x += ((Random16() & 15) - 8) << 16;
    dust->y += ((Random16() & 31) - 4) << 16;
    dust->motionFlags = 0;
    FieldActor_SetSpriteColor(dust, ((u8 *)self)[0x62]);
    Scene_SetActorSpritePriority(dust, 2);
    Actor_SelectAnimation(dust, 1);
    Actor_SetSpriteFlags(dust, 0);
    Actor_StartScript(dust, gData_02003594);
    self->unk4c = leader->x;
    self->acceleration = leader->z;
    if ((gFrameCount & 3) == 0)
      Event_PlaySound(106);
  }
  if (ahead[2] >> 16 > 248)
    return;
  FieldVector_AddPolarOffset(0x70000, 0x8000 - leader->facing, ahead);
  if (FieldMapGrid_GetCellBehaviorAt(self->terrainId, ahead[0], ahead[2]) ==
      255)
    return;
  {
    s32 x = next[0];

    self->x = x;
    self->z = next[1];
    kind = FieldMapGrid_GetCellBehaviorAt(self->terrainId, x, next[1]);
  }
  if (kind == 52) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 201;
    self->update = 0;
  } else if (kind == 51) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 200;
    self->update = 0;
  }
}

#undef FIELD_AT

#undef Func_0200088c
#undef Func_020009b8
#undef Func_02000a78

// ---- scene/anemos_inner_sanctum/s1938.c ----

/// Scene code of overlay 0x6b0, in ROM order.

#include "flags.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "event_work.h"
#include "module/field/object.h"
#include "module/scene/anemos_inner_sanctum.h"
#include "text/msg_ids.h"
#include "scene_ids.h"

void Actor_PlaceBlocks(void);

void Func_02000178(void);

void Func_02001938(void) {
  struct Actor *object;

  if (gMainRoutineWork.entrance == 1)
    object = Actor_Get(8);
  else if (gMainRoutineWork.entrance == 3)
    object = Actor_Get(9);
  else
    object = Actor_Get(10);
  if (*(s16 *)((u8 *)Actor_Get(gMainRoutineWork.currentOwner) + 18) <= 247) {
    object->motionFlags = 0;
    object->update = Func_020017b4;
    object->unk4c = Actor_Get(gMainRoutineWork.currentOwner)->x;
    object->acceleration = Actor_Get(gMainRoutineWork.currentOwner)->z;
    Event_SetFlag(0x200);
  }
}

#include "field_sprite.h"
#include "io_reg.h"

struct SpriteView {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 unk4 : 4;
  u8 unk06[14];
  s32 unk14;
};

void Func_020019a8(void) {
  struct Actor *object;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 position[3];

  if (gMainRoutineWork.entrance == 1)
    object = Actor_Get(8);
  else if (gMainRoutineWork.entrance == 3)
    object = Actor_Get(9);
  else
    object = Actor_Get(10);
  position[0] = (object->x & 0xfff00000) + 0x80000;
  position[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetMoveTarget(object, position[0], 0, position[2]);
  effect =
      OverlayObject_PrepareObject(object->x, 0, position[2] + 0x3b0000, 286);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x18000;
  sprite = effect->sprite;
  ((struct SpriteView *)sprite)->mode = 2;
  REG_WINOUT = WINOUT_WIN01_ALL | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
               WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_CLR;
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  Event_PlaySound(204);
  object->motionFlags = 3;
  OverlayObject_WaitUntilIdle(object);
  object->x = 0;
  object->y = 0;
  object->z = 0;
  Actor_SetPosition(8, 0, 0);
  Actor_Release(effect);
  Event_End();
}

/// By the entrance taken, sets that entrance's flag, moves its actor to the
/// middle of its cell and copies a cell picture column twice beside it.
void Func_02001a94(void) {
  struct Actor *object;
  s32 column;
  s32 target[3];

  if (gMainRoutineWork.entrance == 1) {
    object = Actor_Get(8);
    Event_SetFlag(0x301);
    column = 11;
  } else if (gMainRoutineWork.entrance == 3) {
    object = Actor_Get(9);
    Event_SetFlag(0x302);
    column = 26;
  } else {
    object = Actor_Get(10);
    Event_SetFlag(0x303);
    column = 53;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  target[0] = (object->x & 0xfff00000) + 0x80000;
  target[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetMoveTarget(object, target[0], 0, target[2]);
  FieldActor_WaitForMove(object);
  Event_PlaySound(188);
  MapGrid_CopyCellsTo(18, 4, column, 4, 1, 2);
  System_Wait(5);
  MapGrid_CopyCellsTo(18, 7, column, 4, 1, 2);
  System_Wait(5);
  MapGrid_SetCellHeight(column << 20, 0x580000, 0, 0);
  Event_End();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001b88(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  FieldMessage_ShowCentered((s32)MsgSummonInheritedEarthDarkness, 0, 6);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  Event_SetFlag(0xa8c);
  Event_End();
}

extern struct Trigger gData_02003dc0[];
extern struct Trigger gData_02003dcc[];
extern struct Trigger gData_02003e38[];
extern struct Trigger gData_02003fac[];
extern struct Trigger gData_02004150[];
extern struct Trigger gData_020042c4[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room1)
    return gData_02003dcc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room2)
    return gData_02003e38;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3)
    return gData_02003fac;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
    return gData_02004150;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room5)
    return gData_020042c4;
  return gData_02003dc0;
}

void Func_02001c60(void) {
  Func_020000f4();
  Teleport_PlaySparks();
  {
    struct MainRoutineWork2c2 *party =
        (struct MainRoutineWork2c2 *)&gMainRoutineWork;

    party->value = 2;
  }
  Event_SetDestination((s32)&SceneId_Ov694Room1, 99);
  FieldEvent_Wait(40);
  Func_02000158();
}

#include "scene_ids.h"

/* VIEW(s1938): tag struct EventWork154 as struct EventWork154__s1938;
 * module/scene/scene.h has another definition; tried with the definition the
 * rest of the object has, its members read value as returnEntrance, it does not
 * compile in tla-ja:
 * games/tla/src/scene/anemos_inner_sanctum/scene_tables.c:1084: structure has
 * no member named `returnEntrance'. */
struct EventWork154__s1938 {
  u8 unk000[0x154];
  s16 value;
};

extern const s16 gData_020043e4[];
extern const struct AnemosInnerSanctumPicture gData_02004410[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02003320_(const void *data, s32 a, s32 b) {
  MapGrid_PlayMapEdit(data, a, b);
}

/// The scene opening: clears the actors' motion flags, plays the entrance's
/// map edit and steps the leader in.
void Func_02001c94(void) {
  struct EventWork154__s1938 *work = Ram_HeapSlots->eventWork;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // A goto loop, as in overlay 6a6's Func_02000e54 (its twin): it carries no
  // loop notes.
  i = 8;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  Event_PlaySound(158);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room1) {
    MapGrid_AnimateCells(gData_020043e4, 33, 7);
  } else {
    s32 index = work->value / 2 - 1;

    Func_02003320_(gData_02004410[index].data, gData_02004410[index].a,
                   gData_02004410[index].b);
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(work->value);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

#include "scene_tables.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_020033e0_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_02001d68(void) {
  struct Actor *object = Actor_Get(9);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(9, 2);
  object->y = -0x400000;
  Func_020033e0_(9, PIXELS(728), PIXELS(120));
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(728), PIXELS(120));
  Actor_Get(gMainRoutineWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x300000, object->z);
  FieldActor_WaitForMove(object);
  Event_SetFlag(0x300);
  Event_End();
}

void Func_02001df4(void) {
  struct Actor *object = Actor_Get(11);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->motionFlags = 0;
  object->y = -0x400000;
  Actor_Get(gMainRoutineWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x300000, object->z);
  FieldActor_WaitForMove(object);
  Event_SetFlag(0x304);
  Event_End();
}

#include "call.h"
s32 Func_02000038();
s32 CrackedTiles_Find();
s32 CrackedTile_PlaceLanding();
extern u8 gData_020036e4[];
extern u8 gData_020036ea[];
extern u8 gData_020036f6[];
extern u8 gData_0200384c[];
extern u8 gData_0200385c[];
extern u8 gData_02003888[];
extern u8 gData_020038b4[];

s32 Scene_Initialize(void) {
  s32 v_r1, v_r6, v_r7, v_r2, v_r5, v_r3, v_r0, t0, t1, t3;
  s32 t6, t9, t12, t15, t27;
  s32 t30, t35, t36, t38, t42;
  s32 t59;
  s32 t60, t61, t64;
  s32 t78, t80;
  s32 t91, t92, t93, t94, t95, t96, t106;
  s32 t126;
  s32 t132, t136;
  s32 v_two;

  v_r2 = (s32)Ram_HeapSlots;
  v_r7 = 0x204;
  v_r6 = (s32)&gMainRoutineWork;
  FIELD_AT(FIELD_AT(v_r2, s32, 108), s32, 428) = v_r7;
  v_r1 = gMainRoutineWork.scene;
  if (v_r1 == (s32)&SceneId_Ov6B0Room1) {
    Call4((void (*)())Func_02000038, (s32)gData_0200384c, (s32)gData_0200385c,
          (s32)gData_02003888, (s32)gData_020038b4);
    t3 = Event_GetFlag(0xa87);
    if (t3 != 0) {
      Call3((void (*)())Func_02000a78, 0x208, 48, 0);
      MapGrid_CopyCellsTo(85, 34, 85, 8, 9, 8);
    }
    t6 = Event_GetFlag(0xa88);
    if (t6 != 0) {
      Call3((void (*)())Func_02000a78, 0x228, 48, 3);
      MapGrid_CopyCellsTo(101, 34, 101, 8, 9, 8);
    }
    t9 = Event_GetFlag(0xa89);
    if (t9 != 0) {
      Call3((void (*)())Func_02000a78, 0x1f8, 40, 10);
      MapGrid_CopyCellsTo(85, 46, 85, 20, 9, 8);
    }
    t12 = Event_GetFlag(0xa8a);
    if (t12 != 0) {
      Call3((void (*)())Func_02000a78, 0x238, 40, 2);
      MapGrid_CopyCellsTo(101, 46, 101, 20, 9, 8);
    }
    t15 = Event_GetFlag(0xa8b);
    if (t15 == 0)
      goto L_0200a6e6;
    MapGrid_CopyCellsTo(54, 7, 33, 7, 1, 2);
    MapGrid_SetCellHeight(0x2180000, 0x880000, 0, 0);
    goto L_0200a6e6;
  }
  if (v_r1 != (s32)&SceneId_Ov6B0Room2)
    goto L_0200a1b4;
  FieldActor_SetSpritePriority(8, 1);
  if ((Event_GetFlag(0xa80)) != 0) {
    Actor_SetPosition(8, 0x1280000, 0xf80000);
    FieldActor_SetSpritePriority(8, 2);
  }
  if ((Event_GetFlag(0xa81)) != 0) {
    Actor_SetPosition(10, 0x1b80000, 0x2f80000);
  }
  Call1((void (*)())Actor_PlaceBlocks, (s32)gData_020036e4);
  FIELD_AT(((s32)Actor_Get((FIELD_AT(v_r6, s32, 532)))), u8, 35) |= 64;
  FIELD_AT(((s32)Actor_Get(9)), u8, 85) = 0;
  FIELD_AT(((s32)Actor_Get(9)), u8, 35) |= 128;
  if ((FIELD_AT(v_r6, s16, 482)) != 17)
    goto L_0200a03c;
  if ((Event_GetFlag(0x300)) == 0)
    goto L_0200a03c;
  Actor_SetAnimation(9, 2);
  Actor_SetPosition(9, 0x2d80000, 0x780000);
L_0200a03c:
  if (gMainRoutineWork.entrance == 18 && GameFlag_Get(0x300) == 0)
    Func_02001d68();
  {
    struct MainRoutineWork *work = &gMainRoutineWork;
    if (work->entrance != 10)
      goto L_0200a6e6;
    if ((Event_GetFlag(0x300)) != 0) {
      Actor_SetPosition(9, 0, 0);
    }
    if ((Event_GetFlag(0x207)) != 0) {
      t42 = (s32)Actor_Get(9);
      FIELD_AT(t42, u8, 85) = 4;
    }
    if ((Event_GetFlag(0x200)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 44, 86, 1, 1);
    }
    if ((Event_GetFlag(0x201)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 46, 86, 1, 1);
    }
    if ((Event_GetFlag(0x202)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 43, 87, 1, 1);
    }
    if ((Event_GetFlag(0x203)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 45, 87, 1, 1);
    }
    if ((Event_GetFlag(0x204)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 47, 87, 1, 1);
    }
    if ((Event_GetFlag(0x205)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 44, 88, 1, 1);
    }
    if ((Event_GetFlag(0x206)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 46, 88, 1, 1);
    }
    FIELD_AT(((s32)Actor_Get((work->currentOwner))), u8, 35) |= 32;
  }
  goto L_0200a6e6;
L_0200a1b4:
  if (v_r1 != (s32)&SceneId_Ov6B0Room3)
    goto L_0200a2ce;
  t60 = (s32)Actor_Get(14);
  v_two = 2;
  v_r5 = 1;
  FIELD_AT(t60, u8, 85) = v_two;
  FIELD_AT(t60, s32, 20) = 0xfff20000;
  FIELD_AT(t60, s32, 12) = 0xfff20000;
  FIELD_AT(t60, u8, 35) |= 128;
  FIELD_AT(((s32)Actor_Get((FIELD_AT(v_r6, s32, 532)))), u8, 35) |= 64;
  FIELD_AT(((s32)Actor_Get(9)), u8, 98) = v_r5;
  if ((Event_GetFlag(0xa82)) != 0) {
    Actor_SetPosition(8, 0x380000, 0x1a80000);
  }
  if ((Event_GetFlag(0xa83)) != 0) {
    Actor_SetPosition(10, 0x2180000, 0x1680000);
    MapGrid_CopyCellsTo(51, 20, 31, 20, v_r5, v_two);
    MapGrid_SetCellHeight(0x1f00000, 0x1500000, 0, 0);
  }
  if ((Event_GetFlag(0xa84)) != 0) {
    Actor_SetPosition(13, 0x2e80000, 0x1c80000);
  }
  if ((Event_GetFlag(0xa85)) == 0)
    goto L_0200a2c6;
  if ((Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) != 0)
    goto L_0200a2b8;
  if ((Event_GetFlag(0xa84)) != 0)
    goto L_0200a2b8;
  t78 = FIELD_AT(((s32)Actor_Get(12)), s16, 10);
  t80 = FIELD_AT(((s32)Actor_Get(12)), s16, 18);
  v_r5 = (t78 << 16);
  Actor_SetPosition(13, v_r5, (t80 << 16));
L_0200a2b8:
  Actor_SetPosition(12, 0x2d80000, 0x1d80000);
L_0200a2c6:
  Call1((void (*)())Actor_PlaceBlocks, (s32)gData_020036ea);
  goto L_0200a6e6;
L_0200a2ce:
  if (v_r1 != (s32)&SceneId_Ov6B0Room4)
    goto L_0200a52c;
  if ((Event_GetFlag(0xa86)) != 0) {
    Actor_SetPosition(9, 0x1080000, 0x1680000);
  }
  Call1((void (*)())Actor_PlaceBlocks, (s32)gData_020036f6);
  v_r3 = (FIELD_AT(v_r6, u16, 482));
  v_r2 = (FIELD_AT(v_r6, s16, 482));
  if (v_r2 != 1)
    goto L_0200a4a2;
  CrackedTiles_Find(33, 0x280, 0x380, 18, 68);
  v_r5 = (Event_GetFlag(0x304));
  if (v_r5 == 0) {
    t91 = FIELD_AT(v_r6, s32, 532);
    t92 = (s32)Actor_Get(t91);
    FIELD_AT(t92, u8, 35) |= 64;
    t94 = (s32)Actor_Get(8);
    FIELD_AT(t94, u8, 85) = v_r5;
    t95 = (s32)Actor_Get(8);
    FIELD_AT(t95, u8, 35) |= 128;
    goto L_0200a3a8;
  }
  Actor_SetPosition(8, 0, 0);
  Event_SetFlag(0x200);
  Event_SetFlag(0x201);
  Event_SetFlag(0x202);
  Event_SetFlag(0x203);
  Event_SetFlag(v_r7);
  Event_SetFlag(0x205);
  Event_SetFlag(0x206);
L_0200a3a8:
  if ((Event_GetFlag(0x207)) != 0) {
    t106 = (s32)Actor_Get(8);
    FIELD_AT(t106, u8, 85) = 4;
  }
  if ((Event_GetFlag(0x200)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 8, 69, 1, 1);
  }
  if ((Event_GetFlag(0x205)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 6, 71, 1, 1);
  }
  if ((Event_GetFlag(0x201)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 5, 75, 1, 1);
  }
  if ((Event_GetFlag(0x202)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 8, 75, 1, 1);
  }
  if ((Event_GetFlag(0x203)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 11, 75, 1, 1);
  }
  if ((Event_GetFlag(0x204)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 8, 73, 1, 1);
  }
  if ((Event_GetFlag(0x206)) == 0)
    goto L_0200a6e6;
  MapGrid_CopyCellsTo(17, 71, 10, 71, 1, 1);
  goto L_0200a6e6;
L_0200a4a2:
  if (v_r2 == 11) {
    CrackedTiles_Find(33, 0x280, 0x380, 18, 68);
    CrackedTile_PlaceLanding(22, 0, v_r7);
    goto L_0200a6e6;
  }
  if (v_r2 != 12)
    goto L_0200a4f4;
  CrackedTiles_Find(35, 0x280, 0x380, 18, 68);
  if ((Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) != 0)
    goto L_0200a6e6;
  CrackedTile_PlaceLanding(36, -0x24, v_r7);
  goto L_0200a6e6;
L_0200a4f4:
  if ((u32)((v_r3 - 4) << 16) > (u32)0x10000)
    goto L_0200a6e6;
  CrackedTiles_Find(35, 0x280, 0x380, 18, 68);
  goto L_0200a6e6;
L_0200a52c:
  if (v_r1 != (s32)&SceneId_Ov6B0Room5)
    goto L_0200a6e6;
  t126 = FIELD_AT(v_r2, s32, 32);
  v_r5 = t126;
  v_r3 = ((FIELD_AT(v_r6, s16, 482)) - 1);
  if ((u32)v_r3 > (u32)11)
    goto L_0200a6e6;
  switch (v_r3) {
  case 0:
    goto L_0200a606;
  case 1:
    goto L_0200a606;
  case 2:
    goto L_0200a63a;
  case 3:
    goto L_0200a63a;
  case 4:
    goto L_0200a686;
  case 5:
    goto L_0200a686;
  case 6:
    goto L_0200a580;
  case 7:
    goto L_0200a6e6;
  case 8:
    goto L_0200a6e6;
  case 9:
    goto L_0200a6e6;
  case 10:
    goto L_0200a6e6;
  case 11:
    goto L_0200a596;
  }
L_0200a580:
  if ((Event_GetFlag(0x304)) != 0)
    goto L_0200a596;
  Actor_SetPosition(11, 0, 0);
L_0200a596:
  FIELD_AT(((s32)Actor_Get((gMainRoutineWork.currentOwner))), u8, 35) |= 64;
  Actor_SetAnimation(11, 2);
  FIELD_AT(((s32)Actor_Get(11)), u8, 85) = 0;
  FIELD_AT(((s32)Actor_Get(11)), u8, 35) |= 128;
  Actor_SetAnimation(11, 2);
  /* FAKEMATCH(dead): v_r6 set again to the address it already holds; without
   * it, 4 of 913 instructions differ in tla-en. */
  v_r6 = (s32)&gMainRoutineWork;
  if ((Event_GetFlag(0x304)) != 0)
    goto L_0200a5fa;
  if ((FIELD_AT(v_r6, s16, 482)) != 12)
    goto L_0200a5fa;
  Func_02001df4();
  goto L_0200a6e6;
L_0200a5fa:
  FIELD_AT(((s32)Actor_Get(11)), s32, 12) = 0xfff00000;
  goto L_0200a6e6;
L_0200a606:
  if ((Event_GetFlag(0x301)) != 0) {
    v_r2 = 240;
    v_r0 = 8;
    v_r1 = 0x780000;
    goto L_0200a650;
  }
  MapGrid_CopyCellsTo(18, 1, 11, 4, 1, 2);
  v_r1 = 176;
  v_r0 = 0xb80000;
  goto L_0200a67a;
L_0200a63a:
  if ((Event_GetFlag(0x302)) == 0)
    goto L_0200a660;
  v_r2 = 240;
  v_r0 = 9;
  v_r1 = 0x2280000;
L_0200a650:
  Actor_SetPosition(v_r0, v_r1, (v_r2 << 15));
  Event_SetFlag(0x200);
  goto L_0200a6e6;
L_0200a660:
  MapGrid_CopyCellsTo(18, 1, 26, 4, 1, 2);
  v_r1 = 176;
  v_r0 = 0x1a80000;
L_0200a67a:
  MapGrid_SetCellHeight(v_r0, (v_r1 << 15), 0, 2);
  goto L_0200a6e6;
L_0200a686:
  if ((Event_GetFlag(0x303)) != 0) {
    Actor_SetPosition(10, 0x3180000, 0x780000);
    Event_SetFlag(0x200);
    goto L_0200a6d0;
  }
  MapGrid_CopyCellsTo(18, 1, 53, 4, 1, 2);
  MapGrid_SetCellHeight(0x3580000, 0x580000, 0, 2);
L_0200a6d0:
  FIELD_AT(v_r5, s32, 236) = ((FIELD_AT(v_r5, s32, 236)) + 0x80000);
  FIELD_AT(v_r5, s32, 244) = ((FIELD_AT(v_r5, s32, 244)) + 0x80000);
L_0200a6e6:
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "../../field/object/set_sprite_priority_b.c"

// ---- scene/anemos_inner_sanctum/move.c ----

/// Scene code of overlay 0x6b0, in ROM order.

#include "../shared/cracked_tile.h"
#include "types.h"
#include "field_effect.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"

void Effect_MoveLocal(union FieldObject *object) {
  s32 x;
  s32 velocityX;
  struct FieldSprite *sprite;

  x = object->effect.x;
  velocityX = object->effect.velocity.x;
  sprite = object->effect.sprite;

  object->effect.x = x + velocityX;
  object->effect.y += object->effect.velocity.y;
  object->effect.z += object->effect.velocity.z;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  sprite->rotation += object->effect.spin;
}

#include "main_routine_work.h"
#include "field_sprite.h"
#include "event_inline.h"

void Actor_SelectAnimation(struct Actor *, s32);
extern const s32 *gData_02004428[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void Actor_SetSpritePriorityB(struct Actor *object, s32 priority);
void FieldEffect_SpawnLocal(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                            s32 velocityZ, u32 flags,
                            const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect, gData_02004428[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = Effect_MoveLocal;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Actor_SetSpritePriorityB(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Actor_SetSpritePriorityB(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gData_02004428[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
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

#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepth
#include "../../field/map/cell_sprite.c"

#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "cell_target.h"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gCrackedTiles, CrackedTiles_Update)

void Leader_CenterInCell(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x;
  s32 z = object->z;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x40000, 0x20000);

  x >>= 20;
  z >>= 20;
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, (x << 4) + 8,
                          (z << 4) + 8);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y,
                                       (z << 20) + 0x80000);
  FieldEvent_Wait(4);
  FieldCamera_GetActor()->y -= 0x80000;
}

/// Map_CopyCellsRect through this overlay's import.

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

// ---- scene/anemos_inner_sanctum/s2db0.c ----

/// Scene code of overlay 0x6b0, in ROM order.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "dma.h"
#include "event_inline.h"
#include "io_reg.h"
#include "event_work.h"
#include "field.h"
#include "module/scene/anemos_inner_sanctum.h"
#include "module/scene/scene.h"

void CrackedTile_Fall(struct PushEvent *event) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x;
  s32 z;
  s32 mask;

  mask = 64;
  x = object->x >> 20;
  do {
  } while (0);
  x |= (s16)(event->x & mask);
  z = object->z >> 20;
  z |= (s16)(event->z & mask);
  Leader_CenterInCell();
  Sound_Play(229);
  Map_CopyCellsRect(event->x, event->z + 2, 1, 1, x, z);
  Event_Wait(12);
  {
    u32 zero = 0;

    object->facing = 0x4000;
    object->motionFlags = zero;
  }
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  {
    u32 winout = 0xf1f;

    REG_WINOUT = winout;
  }
  do {
  } while (0);
  REG_DISPCNT |= 0x8000;
  event->busy = 1;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  Event_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Sound_Play(204);
    object->scaleX += -0x400;
    object->scaleY += -0x300;
    object->y += -0x9999;
    wait(1);
  }
  do {
    Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  } while (0);
  event->busy = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
