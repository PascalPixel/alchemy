#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Event_StartScreenTransition(void);
void Func_02003310(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003318(struct Actor *object);
void Func_02003398(void);
void Func_020033a0(void);
void Func_020033e0(s32 id, s32 x, s32 z);
void Func_02003488(s32 a);

static __inline__ void Func_020033e0_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020033e0(id, x, z);
}

void Func_02001d68(void) {
  struct Actor *object = Actor_GetById(9);

  Func_02003398();
  Func_02003488(0);
  Actor_SetModeById(9, 2);
  object->y = -0x400000;
  Func_020033e0_(9, 0x2d80000, 0x780000);
  Func_020033e0_(gSequenceParentWork.currentOwner, 0x2d80000, 0x780000);
  Actor_GetById(gSequenceParentWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003310(object, object->x, object->y + 0x300000, object->z);
  Func_02003318(object);
  GameFlag_SetBit(0x300);
  Func_020033a0();
}

void Func_02001df4(void) {
  struct Actor *object = Actor_GetById(11);

  Func_02003398();
  Func_02003488(0);
  object->motionFlags = 0;
  object->y = -0x400000;
  Actor_GetById(gSequenceParentWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003310(object, object->x, object->y + 0x300000, object->z);
  Func_02003318(object);
  GameFlag_SetBit(0x304);
  Func_020033a0();
}
