#include "types.h"
#include "event_inline.h"

struct YallamActor {
  u8 unk00[98];
  u8 state;
  u8 kind;
  s16 timer;
  s16 count;
  u8 unk68[4];
  void (*update)(struct YallamActor *actor);
};


void Func_02001904(s32 id) {
  struct YallamActor *actor = ((struct YallamActor *)Actor_Get(id));

  Actor_SetSpeed(id, 0x13333, 0x13333);
  Actor_SetAnimation(id, 2);
  actor->timer = 0;
  actor->count = 0;
  actor->state = 1;
  actor->kind = 0;
}
