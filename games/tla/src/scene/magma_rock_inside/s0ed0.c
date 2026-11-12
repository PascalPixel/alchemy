#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "field_effect.h"
#include "event_inline.h"
#include "field.h"

void Func_02004c98(struct Actor *object, s32 mode);
s32 Math_Cosine(s32 angle);

extern const s32 gData_0200502c[];

s32 Func_02000658(s32 layer, s32 x, s32 z);
s32 Func_020005b0(s32 *from, s32 *to);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004cb0(struct Actor *object);
void Func_02004e20(void);
void Func_02000d1c(struct Actor *object);
void Func_02000ea4(struct Actor *object);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02001350(void);
void Func_02000ce4(u16 id);

struct ActorTable {
  u8 unk_0[20];
  struct Actor *actors[64];
};

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02004e10_(s32 a, s32 b) { Camera_SetSpeedParameters(a, b); }

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return (struct Actor *)Import_ActorCreate(kind, x, y, z);
}

void Func_02000ed0(struct Actor *source) {
  struct Actor *flame =
      Import_ActorCreate_(403, source->x, source->y, source->z);
  struct Actor *other;
  s32 position[3];
  struct EffectOptions options;
  s32 step = Math_Cosine(source->facing) << 1;
  s32 cell;
  s32 id;
  s32 bit;

  flame->motionFlags = 0;
  Func_02004c98(flame, 1);
  Actor_SetSingleChildFlags(flame, 0);
  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  position[0] = flame->x;
  position[1] = flame->y;
  position[2] = flame->z;
next:
  position[0] += step;
  cell =
      Func_02000658(2, position[0] >> 20, (position[2] - position[1]) >> 20) >>
      8;
  if (cell == 212 || cell == 222)
    goto end;
  id = 8;
inner:
  other = ((struct ActorTable *)Ram_HeapSlots->eventWork)->actors[id];
  if (other->script == 0)
    goto skip;
  if (source->x >> 20 == other->x >> 20 && source->y >> 20 == other->y >> 20 &&
      source->z >> 20 == other->z >> 20)
    goto skip;
  if (Func_020005b0(&other->x, position) > 8)
    goto skip;
  if (other->y >> 20 != position[1] >> 20)
    goto skip;
  if (*(s16 *)(*(u8 **)((u8 *)other->sprite + 40)) == 272)
    goto end;
  goto hit;
skip:
  id++;
  if (id == 64)
    goto next;
  goto inner;
end:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))Func_02000d1c;
  FieldActor_SetMoveTarget(flame, position[0], position[1], position[2]);
  Func_02004e10_(0x20000, 0x4000);
  FieldCamera_MoveTo(position[0], position[1], position[2], 1);
  Actor_WaitForMove(flame);
  flame->update = 0;
  Func_02004cb0(flame);
  /* FAKEMATCH(temporary): id is reused as the spark count; with its own local,
     14 of 270 instructions differ in tla-en. */
  id = 0;
spark:
  bit = gFrameCount;
  bit &= 1;
  if (bit == 0) {
    options.type = 403;
    options.priority = 1;
    options.update = (void (*)(union FieldObject *))Func_02000ea4;
    options.script = gData_0200502c;
    Func_0200015c(position[0], position[1], position[2],
                  ((Random16Far() & 31) - 16) << 13,
                  ((Random16Far() & 31) - 16) << 13, bit, 0x1320000, &options);
  }
  System_Wait(1);
  id++;
  if (id != 16)
    goto spark;
  System_Wait(24);
  if (cell == 222) {
    Func_02001350();
    Func_02004d48();
    return;
  }
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02004e20();
  System_Wait(10);
  Func_02004d48();
  return;
hit:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))Func_02000d1c;
  FieldActor_SetMoveTarget(flame, position[0], position[1], position[2]);
  Func_02004e10_(0x20000, 0x4000);
  FieldCamera_MoveTo(position[0], position[1], position[2], 1);
  Actor_WaitForMove(flame);
  flame->update = 0;
  Func_02004cb0(flame);
  Event_SetFlag(0x2f0);
  Func_02000ce4((u16)id);
  Event_ClearFlag(0x2f0);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02004e20();
  System_Wait(10);
  Func_02004d48();
}
