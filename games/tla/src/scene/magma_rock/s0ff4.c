/// @file
/// Magma Rock's statues: the magma they fire, the flame columns that lift the
/// leader, the cracked columns, slides, pillars, vines and the final blast.
#include "../shared/actors.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"
#include "event.h"
#include "event_work.h"
#include "field.h"
#include "flag_call.h"
#include "module/scene/magma_rock.h"

struct MagmaRockEventWork {
  u8 unk000[0x15a];
  s16 value15a;
  u8 unk15c[2];
  s16 value15e;
  u8 unk160[6];
  s16 value166;
  s16 value168;
  u8 unk16a[0x48];
  u16 value1b2;
};

void MagmaRock_WarnStatue(struct Actor *object) {
  struct MagmaRockEventWork *work = Ram_HeapSlots->eventWork;

  if (object->timer > 528) {
    if (object->timer & 2)
      FieldActor_SetSpriteColor(object, 10);
    else
      FieldActor_SetSpriteColor(object, 0);
  } else if (object->timer > 400) {
    if (object->timer & 16)
      FieldActor_SetSpriteColor(object, 10);
    else
      FieldActor_SetSpriteColor(object, 0);
  } else {
    object->update = MagmaRock_BurnStatue;
    object->timer = 1;
  }
  if (work->value166 == 0 && work->value15a == 0 && work->value15e == 0 &&
      work->value168 == 0 && work->value1b2 != 0)
    object->timer--;
}

#include "magma_rock.h"

s32 MagmaRock_FindStatue(u16 *table) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  s32 result = 0;
  s32 code = MapCell_GetAttributes(0, work->x >> 20, work->z >> 20) >> 8;
  s32 id;

  goto check;
next:
  result = *table++;
check:
  if (*table == 0xffff)
    goto done;
  id = *table++;
  if (code != *table++)
    goto next;
  work->target = Actor_Get(id);
  result = *table;
done:
  return result;
}

#include "main_routine_work.h"
#include "scene_ids.h"

s32 MagmaRock_LookUpCellStatue(s32 x, s32 z, u16 *table) {
  s32 result = 0;
  s32 code = MapCell_GetAttributes(0, x >> 20, z >> 20) >> 8;

  while (*table != 0xffff) {
    table++;
    if (code == *table) {
      result = table[1];
      break;
    }
    table++;
    result = *table++;
  }
  return result;
}

/// Picks the cell table for the room and entrance, finds the target in it and,
/// in modes 2 and 3, sets the target's byte 99 to 4 or 2 and makes it blink
/// with MagmaRock_FlashStatue.
void MagmaRock_TargetStatue(s32 mode) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  s16 *table = gMagmaRock_Room1Statues;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->scene == (s32)&SceneId_Ov691Room2) {
    if (party->entrance <= 3)
      table = gMagmaRock_Room2LowerStatues;
    else if (party->entrance <= 8)
      table = gMagmaRock_Room2MiddleStatues;
    else if (party->entrance <= 12)
      table = gMagmaRock_Room2UpperStatues;
    else
      table = gMagmaRock_Room2TopStatues;
  } else if (party->scene == (s32)&SceneId_Ov691Room3) {
    table = gMagmaRock_Room3Statues;
  }
  if (mode == 2) {
    struct Actor *target;

    MagmaRock_FindStatue(table);
    target = work->target;
    ((u8 *)target)[99] = 4;
    target->update = (void (*)(void))MagmaRock_FlashStatue;
  }
  if (mode == 3) {
    struct Actor *target;

    MagmaRock_FindStatue(table);
    target = work->target;
    ((u8 *)target)[99] = 2;
    target->update = (void (*)(void))MagmaRock_FlashStatue;
  }
}

/// Adds the velocity to the position and decays it by one eighth each
/// update.
void MagmaRock_DriftAndSlow(struct Actor *object) {
  object->x += object->bounce;
  object->y += object->gravity;
  object->z += object->unk4c;
  object->bounce -= object->bounce >> 3;
  object->gravity -= object->gravity >> 3;
  object->unk4c -= object->unk4c >> 3;
}

#include "field/event_work.h"
#include "field_effect.h"
#include "frame.h"
#include "field.h"

s32 Trig_Cos(s32 angle);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);
s32 Position_FlatDistance(s32 *a, s32 *b);
// XXX: magma_rock.c defines this with an s16 value; this call matches only with
// u16. A match of magma_rock.c's MagmaRock_PlayBurst under u16 would settle
// which the game declares.
void MagmaRock_PlayBurst(u16 index);
void MagmaRock_TrailEmbers(void);
void Actor_Release(struct Actor *object);
s32 Func_02003af4(void);
void FieldCamera_WaitForArrival(void);

s32 MagmaRock_FireFromStatue(struct Actor *source) {
  s32 position[3];
  struct EffectOptions options;
  struct Actor *object;
  struct Actor *other;
  s32 step;
  s32 parity;
  s32 i;
  s32 attributes;

  {
    s32 x = source->x;
    s32 y = source->y;
    s32 z = source->z;

    object = FieldActor_Create(403, x, y, z);
  }
  step = Trig_Cos(source->facing) << 1;
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  for (;;) {
    position[0] += step;
    attributes = MapCell_GetAttributes(2, position[0] >> 20,
                                       (position[2] - position[1]) >> 20) >>
                 8;
    if (attributes == 212 || attributes == 222)
      goto stop;
    for (i = 8; i != 64; i++) {
      other = ((struct EventWork *)Ram_HeapSlots->eventWork)->objects[i];
      if (other->script == NULL)
        continue;
      if (source->x >> 20 == other->x >> 20 &&
          source->y >> 20 == other->y >> 20 &&
          source->z >> 20 == other->z >> 20)
        continue;
      if (Position_FlatDistance(&other->x, position) > 8)
        continue;
      if (other->y >> 20 != position[1] >> 20)
        continue;
      if (*((struct ObjectAnimationState *)other->sprite)->value28 == 0x110)
        goto stop;
      goto hit;
    }
  }

stop:
  object->acceleration = 0x20000;
  object->speedLimit = 0x20000;
  object->update = MagmaRock_TrailEmbers;
  FieldActor_SetMoveTarget(object, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(object);
  object->update = NULL;
  Actor_Release(object);
  for (i = 0; i != 16; i++) {
    parity = gFrameCount & 1;

    if (parity == 0) {
      s32 dx;
      s32 dz;

      options.type = 403;
      options.priority = 1;
      options.update = (void (*)(union FieldObject *))MagmaRock_DriftAndSlow;
      options.script = (const s32 *)gMagmaRock_EmberScript;
      dx = Random16() & 31;
      dz = Random16() & 31;
      FieldEffect_Spawn(position[0], position[1], position[2], (dx - 16) << 13,
                        (dz - 16) << 13, parity, 0x1320000, &options);
    }
    System_Wait(1);
  }
  System_Wait(24);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  return Func_02003af4();

hit:
  object->acceleration = 0x20000;
  object->speedLimit = 0x20000;
  object->update = MagmaRock_TrailEmbers;
  FieldActor_SetMoveTarget(object, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(object);
  object->update = NULL;
  Actor_Release(object);
  Event_SetFlag(0x2f0);
  MagmaRock_PlayBurst(i);
  Event_ClearFlag(0x2f0);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  return Func_02003af4();
}

#include "macros.h"

void MagmaRock_FireStatue(void) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  object = work->target;
  object->update = NULL;
  FieldActor_SetSpriteColor(object, 7);
  FieldEvent_Wait(3);
  FieldActor_SetSpriteColor(work->target, 0);
  FieldEvent_Wait(1);
  MagmaRock_FireFromStatue(work->target);
  Func_02003af4();
}

void MagmaRock_IgniteStatue(void) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object = work->target;
  object->timer = 0x400;
  object->update = MagmaRock_WarnStatue;
  Func_02003af4();
}

void MagmaRock_BurstStatue(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0)
    MagmaRock_StartFallingRocks();
  if (mode == 2) {
    struct Actor *object = work->target;

    MagmaRock_DropRocks(object->x, object->y + 0xf0000, object->z, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    MagmaRock_DropRocks(object->x, object->y + 0xf0000, object->z, 8);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    MagmaRock_EndFallingRocks();
  if (cue == 178)
    Actor_SelectAnimation(work->target, 3);
}

// One of the four parts MagmaRock_BurnStatue builds: rises and falls with its
// owner's action counter.

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// A 16.16 position word, read whole or by halves.
union Position {
  s32 word;
  s16 half[2];
};

#define PART_Y(part) (((union Position *)&(part)->y)->word)

void Actor_Release(struct Actor *part);

/// One of the four parts MagmaRock_BurnStatue builds: rises and falls with its
/// owner's action counter.
void MagmaRock_RaiseFlamePart(struct Actor *part) {
  struct Actor *owner = part->watchedObject;

  if (owner->timer > 35 && owner->timer < BYTE_AT(part, 98) * 2 + 36) {
    PART_Y(part) = owner->y + 0x200000;
    PART_Y(part) += (owner->timer << 19) + (-35 << 19);
  } else if (BYTE_AT(part, 98) * 2 + 36 < owner->timer && owner->timer < 332) {
    PART_Y(part) = owner->y + (BYTE_AT(part, 98) << 20) + 0x200000;
  } else if (owner->timer > 332 && owner->timer < BYTE_AT(part, 98) * 2 + 333) {
    PART_Y(part) = owner->y + (BYTE_AT(part, 98) << 20) + 0x200000;
    PART_Y(part) -= (owner->timer << 19) - (332 << 19);
  } else if (BYTE_AT(part, 98) * 2 + 333 < owner->timer) {
    Actor_Release(part);
  }
}

// The fifth part of MagmaRock_BurnStatue's group: follows its owner like the
// others, throws effect 403 every eighth frame and marks the ground beneath.
#include "system/field_effect.h"
#include "system/main_routine_work.h"

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[4];
  void (*unk24)(void);
};

void MagmaRock_SetCellShape(s32 layer, s32 x, s32 z, s32 height);

#define EFFECT_Y(object) (((union Position *)&(object)->effect.y)->word)

/// The fifth part of MagmaRock_BurnStatue's group: follows its owner like the
/// others, throws effect 403 every eighth frame and marks the ground beneath.
void MagmaRock_RaiseFlameTop(union FieldObject *object) {
  struct Actor *watched = (struct Actor *)object->effect.countdown;
  struct MagmaRockEffectConfig config;
  s32 step;
  s32 mask;

  if (watched->timer >= 35 && watched->timer <= 42) {
    EFFECT_Y(object) = watched->y + 0x180000;
    EFFECT_Y(object) = EFFECT_Y(object) + (watched->timer << 19) + (-34 << 19);
  } else {
    if (watched->timer >= 9 && watched->timer <= 331) {
      EFFECT_Y(object) =
          watched->y + (object->effect.unk5a[8] << 20) + 0x180000;
    } else if (watched->timer > 332 &&
               watched->timer < (object->effect.unk5a[8] << 1) + 333) {
      EFFECT_Y(object) =
          watched->y + (object->effect.unk5a[8] << 20) + 0x180000;
      EFFECT_Y(object) -= (watched->timer << 19) - (332 << 19);
    } else if ((object->effect.unk5a[8] << 1) + 333 < watched->timer) {
      object->effect.y = object->actor.terrainHeight;
      object->effect.update = 0;
    }
  }

  mask = 7;
  step = gFrameCount & mask;
  if (step == 0) {
    config.id = 403;
    config.variant = 10;
    config.kind = 2;
    config.unk24 = MagmaRock_DriftAndSlow;
    config.data = gMagmaRock_EmberScript;
    FieldEffect_Spawn(object->effect.x + 0x10000, object->effect.y - 0x40000,
                      object->effect.z, ((Random16() & mask) - 3) << 14,
                      -0x20000, step, 0x1330000, &config);
  }
  MagmaRock_SetCellShape(watched->terrainId, watched->x >> 20, watched->z >> 20,
                         (object->effect.y >> 19) + 1);
}
#undef BYTE_AT
#undef PART_Y
#undef EFFECT_Y

#include "system/ram_buffer.h"

/// The native reads these event work halfwords signed (ldrsh).

void Event_PlayCueNearFocus(struct Actor *object, s32 a);

struct AnimationEntry;

/// The sprite as Sprite_Create fills it, laid out as vram_block_93.c
/// declares it.
struct ResourceSprite {
  u32 imageAttributes[3];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  u32 shadowAttributes[3];
  struct AnimationEntry *entries[4];
};

void MagmaRock_BurnStatue(struct Actor *object) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  // The parts' actor type, held from the top: global allocation leaves it in
  // no register and reload rebuilds 0x15f in r0 at each call, as the native
  // does; as the literal, its pseudo takes r0 before the first call's
  // arguments are built (differ 155).
  s32 type = 0x15f;

  if ((gFrameCount & 15) == 0)
    Event_PlayCueNearFocus(object, 136);
  FieldActor_SetSpriteColor(object, 10);
  if (object->timer <= 31) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct MagmaRockEffectConfig config;
      s32 dx;
      s32 dz;

      config.id = 403;
      config.variant = 10;
      config.kind = 2;
      config.unk24 = MagmaRock_DriftAndSlow;
      config.data = gMagmaRock_EmberScript;
      dx = Random16() & 15;
      dz = Random16() & 15;
      FieldEffect_Spawn(object->x + 0x10000, object->y + 0x180000, object->z,
                        (dx - 8) << 14, 0, (dz - 8) << 14, 0x1330000, &config);
    }
  } else if (object->timer == 33) {
    struct Actor *parts[4];
    struct Actor *fifthPart;
    s32 id;
    s32 i;

    Event_PlayCueNearFocus(object, 144);
    parts[0] = (FieldActor_Create(type, object->x + 0x10000,
                                  object->y + 0x200000, object->z));
    for (i = 1; i != 4; i++) {
      struct ResourceSprite *first = parts[0]->sprite;
      struct Actor *part = (FieldActor_Create(type, 0, 0, 0));
      struct ResourceSprite *sprite;

      parts[i] = part;
      sprite = part->sprite;
      sprite->slot = first->slot;
      sprite->retainCacheEntry = 1;
      part->x = parts[0]->x;
      part->y = parts[0]->y;
      part->z = parts[0]->z;
      part->motionFlags = 0;
      Actor_SetSpriteFlags(part, 0);
      Actor_SelectAnimation(parts[i], 3);
      /* FAKEMATCH(dead): part set again to parts[i], which it holds; as
       * parts[i]-> for the uses below, 65 of 294 instructions differ in
       * tla-en. */
      part = parts[i];
      // Byte 98 lies in actor.h's callback. Written as a field, the value is
      // computed before its address, as the native does (through a u8 *,
      // differ 34).
      part->unk5c[6] = 3 - i;
      part->watchedObject = object;
      part->update = MagmaRock_RaiseFlamePart;
      FieldActor_SetSpriteColor(part, 3);
    }
    {
      struct Actor *part = parts[0];

      part->motionFlags = 0;
      Actor_SetSpriteFlags(part, 0);
      Actor_SelectAnimation(parts[0], 3);
      Scene_SetActorSpritePriority(parts[0], 3);
      /* FAKEMATCH(dead): part set again to parts[0], which it holds; as
       * parts[0]-> for the uses below, 45 of 294 instructions differ in
       * tla-en. */
      part = parts[0];
      part->unk5c[6] = 3;
      part->watchedObject = object;
      part->update = MagmaRock_RaiseFlamePart;
      FieldActor_SetSpriteColor(part, 3);
    }
    id = 0;
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
      id = MagmaRock_LookUpCellStatue(object->x, object->z,
                                      gMagmaRock_Room1Statues);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
      // The native loads x and z once, ahead of the entrance tests.
      s16 entrance = gMainRoutineWork.entrance;
      s32 x = object->x;
      s32 z = object->z;

      if (entrance <= 3)
        id = MagmaRock_LookUpCellStatue(x, z, gMagmaRock_Room2LowerStatues);
      else if (entrance <= 8)
        id = MagmaRock_LookUpCellStatue(x, z, gMagmaRock_Room2MiddleStatues);
      else if (entrance <= 12)
        id = MagmaRock_LookUpCellStatue(x, z, gMagmaRock_Room2UpperStatues);
      else
        id = MagmaRock_LookUpCellStatue(x, z, gMagmaRock_Room2TopStatues);
    }
    fifthPart = Actor_Get(id);
    fifthPart->unk5c[6] = 4;
    fifthPart->watchedObject = object;
    fifthPart->update = MagmaRock_RaiseFlameTop;
    object->timer++;
  } else if (object->timer > 364) {
    FieldActor_SetSpriteColor(object, 0);
    object->update = NULL;
  }
  // screenStatus read unsigned: as work->screenStatus, 39 of 294 instructions
  // differ in tla-en.
  if (work->unk166 == 0 && *(s16 *)&work->unk15a == 0 &&
      *(s16 *)&work->unk15c[2] == 0 && *(s16 *)work->unk168 == 0 &&
      (u16)work->screenStatus != 0)
    object->timer++;
}

/// In room 1 or 2, targets actor 15 or 14, runs MagmaRock_BurstStatue and sets
/// flag 0x200.
#define DEFINE_MAGMA_ROCK_BURST_COLUMN(name, n1, n2, n3)                       \
  void name(s32 mode, s32 cue) {                                               \
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;                    \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
                                                                               \
    {                                                                          \
      s32 scene = party->scene;                                                \
                                                                               \
      if (scene == (s32)&SceneId_Ov691Room1)                                   \
        work->target = Actor_Get(n1);                                          \
      else if (scene == (s32)&SceneId_Ov691Room2)                              \
        work->target = Actor_Get(n2);                                          \
      MagmaRock_BurstStatue(mode, cue);                                        \
    }                                                                          \
    Event_SetFlag(n3);                                                         \
  }

DEFINE_MAGMA_ROCK_BURST_COLUMN(MagmaRock_BurstColumn1, 15, 14, 0x200)

/// In room 1 or 2, targets actor 16 or 10, runs MagmaRock_BurstStatue and sets
/// flag 0x201.
DEFINE_MAGMA_ROCK_BURST_COLUMN(MagmaRock_BurstColumn2, 16, 10, 0x201)

#define DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(name, n1, n2)                    \
  void name(s32 mode, s32 cue) {                                               \
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;                    \
                                                                               \
    work->target = Actor_Get(n1);                                              \
    MagmaRock_BurstStatue(mode, cue);                                          \
    Event_SetFlag(n2);                                                         \
  }

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstColumn3, 17, 0x202)

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstColumn4, 18, 0x203)

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstColumn5, 11, 0x204)

void MagmaRock_ClearSlideFlag(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode != 2)
    Event_ClearFlag(0x2f8);
}

void MagmaRock_SlideDown(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode != 2) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_Get(party->currentOwner)->motionFlags = 0;
    Actor_WalkTo(party->currentOwner, 264, 440);
    FieldEvent_Wait(5);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(2);
    Func_02003af4();
  }
  Event_SetFlag(0x2f8);
}

void MagmaRock_StepOnCrumblingPillar(const u8 *data);

void MagmaRock_ResolveBeforeBurst(void) { Event_ResolvePendingActions(0); }

void MagmaRock_CheckCrumblingPillars(void) {
  MagmaRock_StepOnCrumblingPillar(gMagmaRock_CrumblingPillars);
}

void MagmaRock_SetLedgeBehavior(void) {
  Field_SetCellBehaviorAt(0, 0x1080000, 0x2280000, 30);
  Event_SetFlag(0x212);
}

void MagmaRock_ClearLedgeBehavior(void) {
  Field_SetCellBehaviorAt(0, 0x1080000, 0x2280000, 0);
  Event_ClearFlag(0x212);
}

#include "call.h"

void Actor_SetActionById(s32 id, s32 action);
/* VIEW(s1c00): routine FieldCamera_MoveToActor as void
 * FieldCamera_MoveToActor(s32 a, s32 b); event.h has s32
 * FieldCamera_MoveToActor(s32 actorId, s32 pan); tried as the definition's void
 * FieldCamera_MoveToActor(s32 id, s32 use_setter), it does not compile in
 * tla-ja: games/tla/src/scene/magma_rock/s0ff4.c:672: conflicting types for
 * `FieldCamera_MoveToActor'. */
extern void Func_080d452c__s1c00(s32 a,
                                 s32 b) __asm__("FieldCamera_MoveToActor");
void FieldCamera_WaitForArrival(void);

void MagmaRock_AmplifyBurst(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MagmaRockEffectOptions options;
  s32 position[3];
  struct MagmaRockEffectOptions *opt;
  s32 *offset;
  s32 i;
  u32 phase;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(0x260);
  Event_PlaySound(133);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 6);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 10);
  Actor_SetActionById(gMainRoutineWork.currentOwner, 32);
  opt = &options;
  opt->kind = 1;
  opt->palette = 14;
  opt->scaleX = 0xcccc;
  opt->scaleY = 0xcccc;
  i = 0;
first:
  {
    s32 r = Random16();

    offset = position;
    offset[0] = ((r & 15) << 16) - 0x80000;
  }
  offset[1] = (Random16() & 15) << 16;
  FieldEffect_Spawn(leader->x + offset[0], leader->y + offset[1], leader->z, 0,
                    0x13333, 0, 0xb0000, opt);
  wait(5);
  i++;
  if (i != 8)
    goto first;
  FieldCamera_MoveTo(-1, -1, -1, 0);
  gMainRoutineWork.renderMode = 0;
  leader->motionFlags = 3;
  leader->gravity = 0xcccc;
  leader->bounce = 0;
  Event_Wait(1);
  leader->facing = 0xc000;
  i = 60;
second:
  phase = gFrameCount & 1;
  if (phase == 0) {
    offset[0] = ((Random16() & 15) << 16) - 0x80000;
    offset[1] = (Random16() & 15) << 16;
    FieldEffect_Spawn(leader->x + offset[0], leader->y + offset[1], leader->z,
                      0, 0x13333, phase, 0xb0000, opt);
  }
  wait(1);
  if (leader->velocityY != 0) {
    i--;
    if (i != 0)
      goto second;
  }
  Event_PlaySound(188);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(0x50000, 0x80000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Func_080d452c__s1c00(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 4);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_WaitForAnimationChange(gMainRoutineWork.currentOwner);
  Event_ClearFlag(0x260);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Func_02003af4();
}

#include "fixed_math.h"

void MagmaRock_ClearBlockCells(struct Actor *object);
void MagmaRock_SetCellShape(s32 layer, s32 x, s32 z, s32 height);
void MagmaRock_PushBlockOnto(void);
void Blocks_MarkCells(s16 *ids);

void MagmaRock_PushPillarEast(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 *cell = position;
  struct Actor *object;
  u8 *terrainId;

  cell[0] = leader->x + 0x100000;
  cell[1] = leader->y;
  cell[2] = leader->z;
  object = Actor_FindInCell(cell, leader);
  MagmaRock_ClearBlockCells(object);
  terrainId = &object->terrainId;
  {
    /* FAKEMATCH(temporary): the three values held in locals; as call arguments,
     * 2 of 92 instructions differ in tla-en. */
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    s32 height = object->terrainHeight >> 19;

    MagmaRock_SetCellShape(*terrainId, x, z, height);
  }
  MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
  MagmaRock_PushBlockOnto();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
  MagmaRock_SetCellShape(*terrainId, object->x >> 20, object->z >> 20,
                         (object->terrainHeight >> 19) + 4);
  Blocks_MarkCells(gMagmaRock_Room2UpperPillars);
  cell[0] = object->x >> 20;
  cell[2] = object->z >> 20;
  if (cell[0] == 16 && cell[2] == 36)
    Event_SetFlag(0x8d9);
}

void MagmaRock_PushPillarWest(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  struct Actor *object;
  u8 *terrainId;

  position[0] = leader->x - 0x100000;
  position[1] = leader->y;
  position[2] = leader->z;
  object = Actor_FindInCell(position, leader);
  MagmaRock_ClearBlockCells(object);
  terrainId = &object->terrainId;
  {
    /* FAKEMATCH(temporary): the three values held in locals; as call arguments,
     * 2 of 90 instructions differ in tla-en. */
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    s32 height = object->terrainHeight >> 19;

    MagmaRock_SetCellShape(*terrainId, x, z, height);
  }
  MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
  MagmaRock_PushBlockOnto();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
  MagmaRock_SetCellShape(*terrainId, object->x >> 20, object->z >> 20,
                         (object->terrainHeight >> 19) + 4);
  Blocks_MarkCells(gMagmaRock_Room2TopPillars);
  position[0] = object->x >> 20;
  position[2] = object->z >> 20;
  if (position[0] == 35 && position[2] == 38)
    Event_SetFlag(0x8db);
}

void MagmaRock_GrowVine1(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
  Event_SetFlag(0x250);
}

void MagmaRock_GrowVine2(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
  Event_SetFlag(0x251);
}

void MagmaRock_GrowVine3(void) {
  MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
  Event_SetFlag(0x252);
}

void MagmaRock_ScatterEmbers(s32 x, s32 z) {
  struct MagmaRockEffectConfig config;
  s32 position[3];
  s32 i;
  s32 kind;

  config.unk08 = Q16_ONE * 2;
  config.unk0c = Q16_ONE * 2;
  config.id = 403;
  kind = 1; // Holds one constant: the native loads 1 before the data address.
  config.data = gMagmaRock_EmberScript;
  config.kind = kind;
  i = 0;
loop:
  position[0] = x + ((Random16() & 63) << 16) - Q16_ONE * 32;
  position[1] = (Random16() & 15) << 12;
  position[2] = z + ((Random16() & 63) << 16) - Q16_ONE * 32;
  FieldEffect_Spawn(position[0], 0, position[2], 0, position[1], 0, 0x3a0001,
                    &config);
  i++;
  if (i != 4)
    goto loop;
  Event_SetFlag(0x8dc);
}

DEFINE_ACTOR_FLASH_COLOR(MagmaRock_FlashTarget, 7)

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// MagmaRock_BurstStatue's counterpart: on cue 0 it hands the target to
/// MagmaRock_FlashTarget; modes 2 and 3 run MagmaRock_ScatterEmbers at the
/// target's cell, and mode 3 also clears its position and copies the cells at
/// columns 32 and 96.
void MagmaRock_BlastOpenWall(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0) {
    struct Actor *object;

    MagmaRock_StartFallingRocks();
    object = work->target;
    BYTE_AT(object, 99) = 4;
    object->update = (void *)MagmaRock_FlashTarget;
  }
  if (mode == 2) {
    MagmaRock_ScatterEmbers(work->target->x, work->target->z);
    MagmaRock_DropRocks(work->target->x, work->target->y + 0xf0000,
                        work->target->z, 1);
  }
  if (mode == 3) {
    struct Actor *target;
    s32 last = 8;

    MagmaRock_ScatterEmbers(work->target->x, work->target->z);
    MagmaRock_ScatterEmbers(work->target->x, work->target->z);
    MagmaRock_ScatterEmbers(work->target->x, work->target->z - 0x200000);
    MagmaRock_DropRocks(work->target->x, work->target->y + 0xf0000,
                        work->target->z, 16);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
    MapGrid_CopyCellAttributes(0, 32, 5, 8, 42, 17);
    MapGrid_CopyCellAttributes(0, 96, 5, 8, 42, 81);
    MapGrid_CopyCellsTo(0, 32, 42, 17, 5, last);
    MapGrid_CopyCellsTo(0, 96, 42, 81, 5, last);
  }
  if (cue == 0x3e7)
    MagmaRock_EndFallingRocks();
}

extern const u8 gMagmaRock_Triggers[];
extern const u8 gMagmaRock_Room2LowerTriggers[];
extern const u8 gMagmaRock_Room2MiddleTriggers[];
extern const u8 gMagmaRock_Room2UpperTriggers[];
extern const u8 gMagmaRock_Room2TopTriggers[];
extern const u8 gMagmaRock_Room3Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3)
      return (const struct Trigger *)gMagmaRock_Room2LowerTriggers;
    if (gMainRoutineWork.entrance <= 8)
      return (const struct Trigger *)gMagmaRock_Room2MiddleTriggers;
    if (gMainRoutineWork.entrance <= 12)
      return (const struct Trigger *)gMagmaRock_Room2UpperTriggers;
    return (const struct Trigger *)gMagmaRock_Room2TopTriggers;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room3)
    return (const struct Trigger *)gMagmaRock_Room3Triggers;
  return (const struct Trigger *)gMagmaRock_Triggers;
}
#undef BYTE_AT
