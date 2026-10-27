// Draft, differ 2 of 157: rewritten from the listing with the loops as goto
// loops (as for loops loop.c hoists the masks and the zero: 79). Under flag
// 0x200, when the leader stands on a cell of kind 77, it centres the leader
// there, turns it, raises the seven objects of gData_02000eec one after
// another, then moves objects 9 to 14 by the leader's side's shift and lands
// them. The first Func_02000c74 call goes through a forwarding inline (direct,
// its mask takes r4: 10). Left: the known store-then-load order: after the
// raised object's motionFlags store the native builds the first half of 0x8000
// before loading y; a temporary, the sum reversed and empty asms holding y, the
// object or the flags change nothing or add differences.
#include "../../../../games/tla/src/scene/anemos_inner_sanctum_entrance/anemos_inner_sanctum_entrance.h"

extern const s8 gData_02000eec[];

void Func_02000388(void);
void Func_020004b4(void);
void Func_02000c74(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02000c7c(void);
s32 Func_02000c8c(s32 layer, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 a);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
u32 Random16Far(void);

static __inline__ void MoveTo(struct ObjectRuntime *object, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h). */
  Func_02000c74(object, x, y, z);
}

void Func_02000588(void) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 shift = 0x1300000;
  s32 i;

  if (object->z >> 20 <= 9)
    shift = -shift;
  Func_02000d44(0);
  if (!GameFlag_Test(0x200))
    return;
  if (Func_02000c8c(object->terrainId, object->x, object->z) != 77)
    return;
  Object_SetModeById(gPartyState.currentOwner, 1);
  MoveTo(object, (object->x & 0xfff00000) + 0x80000, object->y,
                (object->z & 0xfff00000) + 0x80000);
  {
    s32 id = gPartyState.currentOwner;

    if (shift > 0)
      ObjectMotion_ArmCallback(id, 0xc000, 0);
    else
      ObjectMotion_ArmCallback(id, 0x4000, 0);
  }
  WaitTicks(10);
  i = 0;
next:
  {
    object = Object_GetById(gData_02000eec[i]);
    object->motionFlags = 0;
    object->y += 0x8000;
    object->acceleration = 0x1000;
    object->speedLimit = 0x20000;
    ObjectDispatch_SetSingleChildFlags(object, 1);
    Func_02000c74(object, object->x, 0xfff80000, object->z);
    object->update = Func_020004b4;
    Func_02000d64(152);
    WaitTicks((Random16Far() * 4 >> 16) + 3);
  }
  if (++i <= 6)
    goto next;
  WaitTicks(40);
  Func_02000d64(198);
  i = 9;
lower:
  {
    object = Object_GetById(i);
    {
      s32 z = object->z - shift;

      Func_02000c74(object, object->x, 0xfff80000, z);
    }
  }
  if (++i <= 14)
    goto lower;
  Object_GetById(9);
  Func_02000c7c();
  WaitTicks(40);
  i = 9;
land:
  {
    object = Object_GetById(i);
    object->z = (object->z & 0xfff00000) + 0x80000;
    object->motionFlags = 3;
    object->update = 0;
  }
  if (++i <= 14)
    goto land;
  WaitTicks(3);
  Func_02000d64(188);
  WaitTicks(10);
  if (!GameFlag_Test(0xa9f) && shift > 0)
    Func_02000388();
}
