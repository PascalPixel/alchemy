// Near miss, differ 265 of 305 (tla-en, 2026-10-31): the native ends CSE's
// reach between the leader's update hook and wait(4), and this compiler
// does not: after that point the native recomputes
// &gMainRoutineWork.currentOwner (r10, then r5), loads the box table's base
// again (r5, then r10) and recomputes kind * 4 + 1 (r9), and it keeps the
// leader's last offset index, 0, unfolded in r9 from the top; here all of
// them are shared across the whole function and the index folds. No plain
// form found gives that boundary (a one-pass block ends only the first CSE
// pass; the same call in both arms of a test at wait(4) gives 193 but keeps
// its branch). Wrapping only the first speed call gives 258, sharing 0x1999
// between the last two, which the native does not.
// Overlay 674's copy of overlay 668's Func_020044c4 (the same listing,
// calling Func_02003790 where 668 calls Map_FillCellRectByte2), whose draft
// holds its notes. Its one argument is islets.h's struct IsletsTarget, 24
// bytes passed by value: the first 16 in r0 to r3, which the native stores
// in the incoming argument area (the sub sp, #16), the object's number and
// the optional callback on the stack.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/vector3.h"
#include "call.h"

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  void (*unk14)(void);
};

extern s8 gData_0200407c[][4];
extern s32 gData_02004094[];
struct Actor *Actor_GetById(u32 id);
void Func_02003790(s32 layer, s32 x, s32 z, s32 width, s32 height,
                   s32 value);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_SetModeById(u32 id, s32 mode);
void wait(s32 frames);
void Actor_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(u32 id);
void Func_02003d0c(void);
void Func_02003de4(struct Actor *object, s32 mode);
void Func_02003ffc(s32 cue);
void Func_02003e0c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003e14(struct Actor *object);
void Func_02003e24(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02003fd4(void);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

void Func_02003a5c(struct PushState push) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(push.number);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  struct Vector3 cell, start;
  s32 kind = push.kind;
  s32 direction, width, height, spareX, spareZ;

  start.x = object->x;
  start.z = object->z;
  direction = 0;
  cell.x = (object->x + (gData_0200407c[kind][0] << 16)) >> 20;
  cell.z = (object->z + (gData_0200407c[kind][1] << 16)) >> 20;
  width = (Abs(gData_0200407c[kind][0]) +
           Abs(gData_0200407c[kind][2])) >>
          4;
  height = (Abs(gData_0200407c[kind][1]) +
            Abs(gData_0200407c[kind][3])) >>
           4;
  Func_02003790(0, cell.x, cell.z, width, height, 0);
  /* FAKEMATCH(wrapper): the three speed calls go through Call3, so no call
   * shares a constant; as direct calls, 327 of 305 instructions differ in
   * tla-en. */
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (push.x - start.x) >> 17,
                                     (push.z - start.z) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02003d0c;
  wait(4);
  Func_02003de4(object, 2);
  Func_02003ffc(239);
  Call3((void (*)())Actor_SetSpeedParameters, push.number, 0x8000, 0x1999);
  Func_02003e0c(object, push.x, push.y, push.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(
      gMainRoutineWork.currentOwner,
      (s32)(gData_02004094[direction] << 16) >> 31,
      (s32)(gData_02004094[direction] << 16) >> 17);
  if (push.unk14 != 0)
    push.unk14();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02003e14(object);
  Func_02003ffc(298);
  Func_02003ffc(213);
  Actor_SetPositionAndResetMotion(object, push.x, object->y, push.z);
  Func_02003de4(object, 1);
  push.x = (push.x + (gData_0200407c[kind][0] << 16)) >> 20;
  push.z = (push.z + (gData_0200407c[kind][1] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  Func_02003e24(push.x, push.z, width, height, spareX + push.x,
                spareZ + push.z);
  Func_02003790(0, push.x, push.z, width, height, 255);
  Func_02003790(2, push.x, push.z, width, height, 255);
  cell.x = (start.x + (gData_0200407c[kind][0] << 16)) >> 20;
  cell.z = (start.z + (gData_0200407c[kind][1] << 16)) >> 20;
  Func_02003e24(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Func_02003790(2, cell.x, cell.z, width, height, 0);
  Func_02003fd4();
}
