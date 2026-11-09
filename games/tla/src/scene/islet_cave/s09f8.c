#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct FieldActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 speed;
  s32 acceleration;
};

void Func_02001254(void);
void Func_02001214(struct FieldActor *actor, s32 mode);
void Func_0200125c(void);

void Func_020009f8(s32 actorId, s32 dx, s32 dz) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 x;
  s32 z;

  leader = ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner));
  actor = ((struct FieldActor *)Actor_GetById(actorId));
  Func_02001254();
  Event_ResolvePendingActions(0);
  x = ((leader->x + (dx << 16)) & 0xfff00000) + 0x80000;
  z = ((leader->z + (dz << 16)) & 0xfff00000) + 0x80000;
  leader->speed = 0x10000;
  leader->acceleration = 0x8000;
  Actor_SetMoveTarget(leader, x, leader->y, z);
  Func_02001214(leader, 27);
  x = ((actor->x + (dx << 16)) & 0xfff00000) + 0x80000;
  z = ((actor->z + (dz << 16)) & 0xfff00000) + 0x80000;
  actor->speed = 0x10000;
  actor->acceleration = 0x8000;
  Actor_SetMoveTarget(actor, x, actor->y, z);
  if (dx < 0 || dz < 0) {
    Func_02001214(actor, 4);
  } else {
    Func_02001214(actor, 3);
  }
  Sound_Play(226);
  Script_WaitForEventTimeout(leader);
  Func_02001214(actor, 2);
  Sound_Play(298);
  Func_0200125c();
}
