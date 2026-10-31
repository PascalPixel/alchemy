// Near miss, differ 310 of 305 (tla-en, 2026-10-31): register allocation
// throughout, from a CSE boundary the native has before the second
// Actor_WaitForMoveById and no C form tried reproduces: the native
// recomputes gMainRoutineWork's owner address there (r5; r10 before), keeps
// the direction index (0) live in r9 and still shifts it, keeps kind * 4 in
// r7 but recomputes kind * 4 + 1 and gData_02004cb0 afterwards, and with
// those pseudos holding r5 to r11 it builds 0x8000 and 0x1999 again at every
// call and spills width to the stack (frame 56). Here CSE merges all of it,
// folds the index and gives the constants r5 and r6 (frame 52). Hiding the
// second half's owner pointer from CSE with an empty asm (a test only, kind
// as a local) measured differ 267 with the native's frame.
// Carries out a push measured by Func_02004240 (the push state arrives by
// value: four words in r0 to r3, the object's number and the optional
// callback on the stack): clears the cells under the object's box, walks
// the leader up to it (mode 8), slides the object to the push's end with the
// leader following (cue 239, Func_02004a34), runs the optional callback,
// restores the leader's speed, plays cues 298 and 213, places the object
// exactly, copies the spare cells into its new place and back over its old
// place (Func_02004a54) and marks layer 0 and 2 there. The same listing as
// overlay 674's Func_02003a5c.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/vector3.h"

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  void (*unk14)(void);
};

extern s8 gData_02004cb0[][4];
extern s32 gData_02004cc8[];
struct Actor *Actor_GetById(u32 id);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 y, u32 width, u32 height,
                           s32 value);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_SetModeById(u32 id, s32 mode);
void wait(s32 frames);
void Actor_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(u32 id);
void Func_02004774(void);
void Func_02004a0c(struct Actor *object, s32 mode);
void Func_02004c9c(s32 cue);
void Func_02004a34(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02004a3c(struct Actor *object);
void Func_02004a54(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02004c0c(void);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

void Func_020044c4(struct PushState push) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(push.number);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  struct Vector3 cell, start;
  s32 direction, width, height, spareX, spareZ;

  start.x = object->x;
  start.z = object->z;
  direction = 0;
  cell.x = (object->x + (gData_02004cb0[push.kind][0] << 16)) >> 20;
  cell.z = (object->z + (gData_02004cb0[push.kind][1] << 16)) >> 20;
  width = (Abs(gData_02004cb0[push.kind][0]) +
           Abs(gData_02004cb0[push.kind][2])) >>
          4;
  height = (Abs(gData_02004cb0[push.kind][1]) +
            Abs(gData_02004cb0[push.kind][3])) >>
           4;
  Map_FillCellRectByte2(0, cell.x, cell.z, width, height, 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (push.x - start.x) >> 17,
                                     (push.z - start.z) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02004774;
  wait(4);
  Func_02004a0c(object, 2);
  Func_02004c9c(239);
  Actor_SetSpeedParameters(push.number, 0x8000, 0x1999);
  Func_02004a34(object, push.x, push.y, push.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(
      gMainRoutineWork.currentOwner,
      (s32)(gData_02004cc8[direction] << 16) >> 31,
      (s32)(gData_02004cc8[direction] << 16) >> 17);
  if (push.unk14 != 0)
    push.unk14();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02004a3c(object);
  Func_02004c9c(298);
  Func_02004c9c(213);
  Actor_SetPositionAndResetMotion(object, push.x, object->y, push.z);
  Func_02004a0c(object, 1);
  push.x = (push.x + (gData_02004cb0[push.kind][0] << 16)) >> 20;
  push.z = (push.z + (gData_02004cb0[push.kind][1] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  Func_02004a54(push.x, push.z, width, height, spareX + push.x,
                spareZ + push.z);
  Map_FillCellRectByte2(0, push.x, push.z, width, height, 255);
  Map_FillCellRectByte2(2, push.x, push.z, width, height, 255);
  cell.x = (start.x + (gData_02004cb0[push.kind][0] << 16)) >> 20;
  cell.z = (start.z + (gData_02004cb0[push.kind][1] << 16)) >> 20;
  Func_02004a54(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Map_FillCellRectByte2(2, cell.x, cell.z, width, height, 0);
  Func_02004c0c();
}
