// Under -Os, 5 November: differ 221 of 305 (tla-en); older figures below predate it.
// Draft, differ 221 of 305, skeleton gap 101 (tla-en, 2026-11-01)
// Re-measured after the compiler change: the old near miss is gone, and the
// draft stood at differ 314, gap 110, with no device. Everything below is plain
// C and carries no device. The frame is now 56 as in the native.
// Gained here: row, kind * 4 set once below the start stores (r7 in the native,
// which adds 1, 2 and 3 to it), and boxes, the table's second name, set once for
// the tail. Together 314 to 221. A shift in place of * 4 gives the same.
// Left, and why it is not reached by reshaping. The table has one pool load in
// every pass from cse1 to lreg: its pseudo runs from the first half into boxes,
// loses a register and is rematerialized at each use, where the native holds it
// in r5, then reloads into r10. Likewise direction folds to 0 (the native keeps it
// in r9 and shifts it at run time), which moves every later allocation by one
// register: leader r9 for r8, the owner address r8 for r10, object r7 for r6.
// 0x8000 and 0x1999 stay in r5 and r6 across calls where the native rebuilds
// them, and the owner address is never rebuilt after Func_02004a34.
// What that proves: the native has an extended-block end at two points, after
// Func_02004c9c(239) and after Func_02004a34, in cse2 as well as cse1. cse2 flushes
// only at a label or a bare volatile asm. Those devices are outside this
// round's limits, so none is here. Three empty asms once reached differ 10 under
// the old compiler; re-measure before trusting that figure.
// Lead for whoever may add a device, measured in scratch only: a for loop from
// after Func_02004a34 to before Func_02004a3c, on direction, gives the table r5
// and keeps direction in a register (differ 247 on the old source, gap 93, with
// the back branch still there). loop.c's one-iteration path in unroll.c removes
// a back branch only with a known start value, no label between that set and the
// loop, and 50 real insns or fewer. Then cse2 folds direction again, because the
// loop's label went with its branch. A counter i with direction++ at the bottom
// does the same (differ 305). An if on direction inside that loop gives 279 to
// 292. So a one-pass form explains the shape but no form found explains it all.
// Both are S2's one-pass or dead kinds if ever kept.
// Tried with no gain: an if-negate abs (328) and macros.h's ABS (same code as
// Abs), a box local for the first half (309; with row it folds to box + row and
// loses adds rN, row, #1, so 321), one for the whole function (300), the sets of
// row, boxes and direction moved, row declared anywhere (all 221).
// Settled here, keep them: direction set before speed; width declared before
// speed (slots); cell before start; dx, dz and toX, toZ locals (argument order);
// Func_02004a0c returning s32, as resource_668_c_02000200.c has it. Overlay
// 674's Func_02003a5c is the same listing and should take row and boxes.
// The load of the map slot goes through struct HeapSlotWords because the
// native keeps the four argument stores ahead of it: the load is in an alias
// set the push state contains, here s32. As a pointer load the scheduler
// moves the stores past it (a struct MapWork * member in PushState has the
// same effect).
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

struct HeapSlotWords {
  s32 unk00[8];
  s32 map;
};

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  void (*unk14)(void);
};

extern s8 gData_02004cb0[];
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
s32 Func_02004a0c(struct Actor *object, s32 mode);
void Func_02004c9c(s32 cue);
void Func_02004a34(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02004a3c(struct Actor *object);
void Func_02004a54(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02004c0c(void);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

void Func_020044c4(struct PushState push) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->map;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(push.number);
  s32 direction = 0;
  s32 row;
  s32 width;
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  struct Vector3 cell, start;
  s32 height, spareX, spareZ;
  s8 *boxes;

  start.x = object->x;
  start.z = object->z;
  row = push.kind * 4;
  cell.x = (object->x + (gData_02004cb0[row] << 16)) >> 20;
  cell.z = (object->z + (gData_02004cb0[row + 1] << 16)) >> 20;
  width = (Abs(gData_02004cb0[row]) + Abs(gData_02004cb0[row + 2])) >> 4;
  height = (Abs(gData_02004cb0[row + 1]) + Abs(gData_02004cb0[row + 3])) >> 4;
  Map_FillCellRectByte2(0, cell.x, cell.z, width, height, 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  {
    s32 dx = (push.x - start.x) >> 17;
    s32 dz = (push.z - start.z) >> 17;
    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, dx, dz);
  }
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
  boxes = gData_02004cb0;
  push.x = (push.x + (boxes[row] << 16)) >> 20;
  push.z = (push.z + (boxes[row + 1] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  {
    s32 toX = spareX + push.x;
    s32 toZ = spareZ + push.z;
    Func_02004a54(push.x, push.z, width, height, toX, toZ);
  }
  Map_FillCellRectByte2(0, push.x, push.z, width, height, 255);
  Map_FillCellRectByte2(2, push.x, push.z, width, height, 255);
  cell.x = (start.x + (boxes[row] << 16)) >> 20;
  cell.z = (start.z + (boxes[row + 1] << 16)) >> 20;
  Func_02004a54(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Map_FillCellRectByte2(2, cell.x, cell.z, width, height, 0);
  Func_02004c0c();
}
