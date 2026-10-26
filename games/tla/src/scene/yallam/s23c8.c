#include "types.h"

/// Yallam's view of the two objects Func_020023c8 restarts.
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
void Func_0200014c(struct YallamActor *actor);

/// Hands objects 9 and 10 to Func_0200014c's update, object 9 with its timer
/// at 1 and object 10 at 0.
void Func_020023c8(void) {
  struct YallamActor *actor = Object_GetById(9);
  struct YallamActor *other;

  actor->timer = 1;
  actor->kind = 0;
  actor->update = Func_0200014c;
  other = Object_GetById(10);
  other->timer = 0;
  other->kind = 0;
  other->update = Func_0200014c;
}
