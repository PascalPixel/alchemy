#include "types.h"
#include "actor.h"
#include "field/event_work.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field_effect.h"
#include "event_inline.h"
#include "frame.h"

extern u32 gData_02003d50[];

s32 Math_Cosine(s32 angle);
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
s32 Func_020005a0(s32 *a, s32 *b);
// XXX: s0c08.c defines this with an s16 value; this call matches only with u16.
// A match of s0c08.c under u16 would settle which the game declares.
void Func_02000c08(u16 index);
void Func_02000d40(void);
void Func_020011b8(void);
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
  step = Math_Cosine(source->angle) << 1;
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
      if (*((struct ObjectAnimationState *)other->animation)->value28 == 0x110)
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
