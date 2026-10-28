#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetAngleToward(s32 id, s32 other, s32 flags);
void Func_02004464(void);
void Func_0200446c(void);
void Func_0200451c(s32 message);
void Func_02004534(s32 id, s32 flags);
void Func_020045ec(s32 value);

extern u8 MsgProx113[];
extern u8 MsgProx114[];
extern u8 MsgProx115[];
extern u8 MsgProx117[];
extern u8 MsgProx118[];
extern u8 MsgProx119[];

void Func_02000764(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(16);
  s16 angle = object->angle;

  Func_02004464();
  Func_020045ec(0);
  ((u8 *)object)[99] = 1;
  if (*(s16 *)(work + 358) == 0) {
    Actor_SetAngleToward(16, gSequenceParentWork.currentOwner, 0);
    Func_0200451c((s32)MsgProx113);
  } else {
    Func_0200451c((s32)MsgProx117);
  }
  Func_02004534(16, 0);
  if (*(s16 *)(work + 358) == 0)
    object->angle = angle;
  ((u8 *)object)[99] = 0;
  Func_0200446c();
}

void Func_020007f4(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(17);
  s16 angle = object->angle;

  Func_02004464();
  Func_020045ec(0);
  ((u8 *)object)[99] = 1;
  if (*(s16 *)(work + 358) == 0) {
    Actor_SetAngleToward(17, gSequenceParentWork.currentOwner, 0);
    Func_0200451c((s32)MsgProx114);
  } else {
    Func_0200451c((s32)MsgProx118);
  }
  Func_02004534(17, 0);
  if (*(s16 *)(work + 358) == 0)
    object->angle = angle;
  ((u8 *)object)[99] = 0;
  Func_0200446c();
}

void Func_02000884(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(18);
  s16 angle = object->angle;

  Func_02004464();
  Func_020045ec(0);
  ((u8 *)object)[99] = 1;
  if (*(s16 *)(work + 358) == 0) {
    Actor_SetAngleToward(18, gSequenceParentWork.currentOwner, 0);
    Func_0200451c((s32)MsgProx115);
  } else {
    Func_0200451c((s32)MsgProx119);
  }
  Func_02004534(18, 0);
  if (*(s16 *)(work + 358) == 0)
    object->angle = angle;
  ((u8 *)object)[99] = 0;
  Func_0200446c();
}
