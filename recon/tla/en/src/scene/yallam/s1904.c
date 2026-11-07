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

struct YallamActor *Actor_GetById(s32 id);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetModeById(u32 id, s32 mode);

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

void Func_02001904(s32 id) {
  struct YallamActor *actor = Actor_GetById(id);

  Actor_SetSpeedParameters_(id, 0x13333, 0x13333);
  Actor_SetModeById(id, 2);
  actor->timer = 0;
  actor->count = 0;
  actor->state = 1;
  actor->kind = 0;
}
