// Draft, differ 18 of 64, skeleton gap 2 (tla-en, 2026-11-01)
// Left: the native builds 0x8000 in r0 and -13 in r0, copying the mask to r2
// (the listing's copy of the object to r0 and of the mask to r2 are the gap).
// Those registers come from loop-depth weights: a real do loop gives them all
// and differ 16, but loop.c then hoists the zero (reg life 41, moved) out of the
// loop, where the native sets r7 in the body, and swaps r2 and r3 in the mask
// step, for skeleton gap 4. So the open question is why the native's zero stays
// in the loop: it is not movable there, or its life is short at the loop pass
// and it joins the later zeros only at cse2. A variable set to 0 twice in the
// body does not change that (differ 14, the zero still hoisted). Tried without
// gain: statement orders of the setup (all 720), one-pass blocks, a chained
// store, narrow zero locals, an index postincrement in the test, for and while
// forms.
#include "types.h"

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "frame.h"
#include "field_sprite.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

struct CellTarget {
  u8 unk00[6];
  s16 x;
  s16 z;
};

s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_0200015c(struct Actor *object);
void Func_020001d8(struct Actor *object);
void Func_020004dc(s32 mode);
struct Actor *Func_02001c04(s32 type, s32 x, s32 y, s32 z);
void Func_02001c34(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct Actor *Func_02001cdc(void);
void Func_02001d3c(s32 id);
void Func_02001d4c(s32 cue);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Func_02000298(void) {
  s32 i;
  s32 next;

  struct Actor *object;
  s16 *action;
  s16 *timer;

  i = 0;
loop:
  object = Func_02001c04(0x2a0, 0, 0, 0);

  object->motionFlags = 0;
  object->priorityFlags = 0;
  object->speedLimit = (i << 12) + 0x8000;
  object->acceleration = (i << 12) + 0x8000;
  object->update = (void (*)(void))Func_020001d8;
  OBJECT_BYTE(object, 99) = i << 3;
  Actor_SetSingleChildFlags(object, 0);
  Actor_SetPartAttribute(object, 2);
  ((struct FieldSprite *)object->animation)->priority = 1;
  Animation_SetStateFlags(object, 1);
  action = &object->action;
  timer = (s16 *)&object->unk66;
  next = i + 1;
  if (i & 1) {
    s32 parameter = 212;
    *action = parameter;
    *timer = 0;
  } else {
    s32 parameter = 116;
    *action = parameter;
    *timer = 0;
  }
  i = next;
  if (i != 16)
    goto loop;
}
