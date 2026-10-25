#include "types.h"

struct YallamActor {
  u8 unk00[98];
  u8 state;
  u8 kind;
  s16 timer;
  s16 count;
  u8 unk68[4];
  void (*update)(struct YallamActor *actor);
};

struct YallamActor *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Object_SetModeById(u32 id, s32 mode);

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

void Func_02001904(s32 id) {
  struct YallamActor *actor = Object_GetById(id);

  ObjectMotion_SetSpeedParameters_(id, 0x13333, 0x13333);
  Object_SetModeById(id, 2);
  actor->timer = 0;
  actor->count = 0;
  actor->state = 1;
  actor->kind = 0;
}
