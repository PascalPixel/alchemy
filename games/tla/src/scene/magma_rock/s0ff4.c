#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

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

void Func_02001738(void);

void Func_02000ff4(struct Actor *object) {
  struct MagmaRockEventWork *work = Ram_HeapSlots->eventWork;

  if (object->timer > 528) {
    if (object->timer & 2)
      Actor_SetSpriteColor(object, 10);
    else
      Actor_SetSpriteColor(object, 0);
  } else if (object->timer > 400) {
    if (object->timer & 16)
      Actor_SetSpriteColor(object, 10);
    else
      Actor_SetSpriteColor(object, 0);
  } else {
    object->update = Func_02001738;
    object->timer = 1;
  }
  if (work->value166 == 0 && work->value15a == 0 && work->value15e == 0 &&
      work->value168 == 0 && work->value1b2 != 0)
    object->timer--;
}

#include "magma_rock.h"

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

s32 Func_02001084(u16 *table) {
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

s32 Func_020010cc(s32 x, s32 z, u16 *table) {
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

extern s16 gData_02003d9e[];
extern s16 gData_02003db2[];
extern s16 gData_02003dc4[];
extern s16 gData_02003dd6[];
extern s16 gData_02003dee[];
extern s16 gData_02003e02[];

void Func_02000fcc(struct Actor *object);

/// Picks the cell table for the room and entrance, finds the target in it and,
/// in modes 2 and 3, sets the target's byte 99 to 4 or 2 and makes it blink
/// with Func_02000fcc.
void Func_02001110(s32 mode) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  s16 *table = gData_02003d9e;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->scene == (s32)&SceneId_Ov691Room2) {
    if (party->entrance <= 3)
      table = gData_02003db2;
    else if (party->entrance <= 8)
      table = gData_02003dc4;
    else if (party->entrance <= 12)
      table = gData_02003dd6;
    else
      table = gData_02003dee;
  } else if (party->scene == (s32)&SceneId_Ov691Room3) {
    table = gData_02003e02;
  }
  if (mode == 2) {
    struct Actor *target;

    Func_02001084(table);
    target = work->target;
    ((u8 *)target)[99] = 4;
    target->update = (void (*)(void))Func_02000fcc;
  }
  if (mode == 3) {
    struct Actor *target;

    Func_02001084(table);
    target = work->target;
    ((u8 *)target)[99] = 2;
    target->update = (void (*)(void))Func_02000fcc;
  }
}

/// Adds the velocity to the position and decays it by one eighth each
/// update.
void Func_020011b8(struct Actor *object) {
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

extern u32 gData_02003d50[];

s32 Math_Cosine(s32 angle);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
s32 Func_020005a0(s32 *a, s32 *b);
// XXX: magma_rock.c defines this with an s16 value; this call matches only with
// u16. A match of magma_rock.c's Func_02000c08 under u16 would settle which the
// game declares.
void Func_02000c08(u16 index);
void Func_02000d40(void);
void Func_02003a54(struct Actor *object, s32 a);
void Func_02003a6c(struct Actor *object);
void Func_02003aec(void);
s32 Func_02003af4(void);
void Func_02003b6c(void);

s32 Func_020011e4(struct Actor *source) {
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
  step = Math_Cosine(source->facing) << 1;
  object->motionFlags = 0;
  Func_02003a54(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Func_02003aec();
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
      if (Func_020005a0(&other->x, position) > 8)
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
  object->update = Func_02000d40;
  FieldActor_SetMoveTarget(object, position[0], position[1], position[2]);
  /* FAKEMATCH(wrapper): the form kept; as FieldCamera_SetSpeed(0x20000,
   * 0x4000), 2 of 260 instructions differ in tla-en. */
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(position[0], position[1], position[2], 1);
  Actor_WaitForMove(object);
  object->update = NULL;
  Func_02003a6c(object);
  for (i = 0; i != 16; i++) {
    parity = gFrameCount & 1;

    if (parity == 0) {
      s32 dx;
      s32 dz;

      options.type = 403;
      options.priority = 1;
      options.update = (void (*)(union FieldObject *))Func_020011b8;
      options.script = (const s32 *)gData_02003d50;
      dx = Random16Far() & 31;
      dz = Random16Far() & 31;
      Func_0200015c(position[0], position[1], position[2], (dx - 16) << 13,
                    (dz - 16) << 13, parity, 0x1320000, &options);
    }
    System_Wait(1);
  }
  System_Wait(24);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003b6c();
  System_Wait(10);
  return Func_02003af4();

hit:
  object->acceleration = 0x20000;
  object->speedLimit = 0x20000;
  object->update = Func_02000d40;
  FieldActor_SetMoveTarget(object, position[0], position[1], position[2]);
  /* FAKEMATCH(wrapper): the form kept; as FieldCamera_SetSpeed(0x20000,
   * 0x4000), 2 of 260 instructions differ in tla-en. */
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(position[0], position[1], position[2], 1);
  Actor_WaitForMove(object);
  object->update = NULL;
  Func_02003a6c(object);
  /* FAKEMATCH(wrapper): the form kept; as Event_SetFlag(0x2f0), 8 of 260
   * instructions differ in tla-en. */
  Event_SetFlag(0x2f0);
  Func_02000c08(i);
  Event_ClearFlag(0x2f0);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003b6c();
  System_Wait(10);
  return Func_02003af4();
}
