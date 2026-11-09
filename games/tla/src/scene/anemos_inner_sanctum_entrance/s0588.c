#include "version.h"
#include "anemos_inner_sanctum_entrance.h"

extern const s8 gData_02000eec[];

void Func_02000388(void);
void Func_020004b4(void);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Script_WaitForEventTimeout(void);
s32 GetMapCellCollision(s32 layer, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 a);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
u32 Random16Far(void);

static __inline__ void MoveTo(struct Actor *object, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): MoveTo forwarded inline; as direct calls to
   * Actor_SetMoveTarget, 6 of 157 instructions differ in tla-en. */
  Actor_SetMoveTarget(object, x, y, z);
}

void Func_02000588(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 shift = 0x1300000;
  s32 i;

  if (object->z >> 20 <= 9)
    shift = -shift;
  Event_ResolvePendingActions(0);
  if (!GameFlag_Test(0x200))
    return;
  if (GetMapCellCollision(object->terrainId, object->x, object->z) != 77)
    return;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
#if TLA_PATCH_1
  MoveTo(object, (object->x & 0xfff00000) + 0x80000, object->y,
         (object->z & 0xfff00000) + 0x80000);
#endif
  {
    s32 id = gMainRoutineWork.currentOwner;

    if (shift > 0)
      Actor_ArmCallback(id, 0xc000, 0);
    else
      Actor_ArmCallback(id, 0x4000, 0);
  }
  wait(10);
  i = 0;
next:
  {
    object = Actor_GetById(gData_02000eec[i]);
    object->motionFlags = 0;
    object->y += 0x8000;
    object->acceleration = 0x1000;
    object->speedLimit = 0x20000;
    Actor_SetSingleChildFlags(object, 1);
    Actor_SetMoveTarget(object, object->x, 0xfff80000, object->z);
    object->update = Func_020004b4;
    Sound_Play(152);
    wait((Random16Far() * 4 >> 16) + 3);
  }
  if (++i <= 6)
    goto next;
  wait(40);
  Sound_Play(198);
  i = 9;
lower:
  {
    object = Actor_GetById(i);
    {
      s32 z = object->z - shift;

      Actor_SetMoveTarget(object, object->x, 0xfff80000, z);
    }
  }
  if (++i <= 14)
    goto lower;
  Actor_GetById(9);
  Script_WaitForEventTimeout();
  wait(40);
  i = 9;
land:
  {
    object = Actor_GetById(i);
    object->z = (object->z & 0xfff00000) + 0x80000;
    object->motionFlags = 3;
    object->update = 0;
  }
  if (++i <= 14)
    goto land;
  wait(3);
  Sound_Play(188);
  wait(10);
  if (!GameFlag_Test(0xa9f) && shift > 0)
    Func_02000388();
}

void Func_02000710(void) {}
