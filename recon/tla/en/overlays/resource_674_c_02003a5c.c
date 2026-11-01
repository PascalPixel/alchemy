// Draft, differ 216 of 305, skeleton gap 92 (tla-en, 2026-11-01)
// Left: the native ends CSE at two points, one between the leader update hook
// and the second speed call, one between that call and the next
// Actor_WaitForMoveById. After them it recomputes the owner address (r10, then
// r5) and rebuilds 0x8000 and 0x1999, and past the first it keeps direction,
// 0, in r9 and shifts it at run time; the table base is reloaded late and
// row + 1 recomputed there, while row itself carries across (probably GCSE). Here the
// index folds, so r9 is free, row spills and the table is reloaded per use in
// the first half: most of the gap follows from that. With a volatile asm
// taking an operand at both points (analysis only, not kept) the shape reaches
// differ 185, gap 81, 318 instructions. cse_end_of_basic_block stops the
// second pass only at a CODE_LABEL, so the source must hold a label that lives
// past cse2 and is gone by the end: one-pass loops and for (i = 0; i < 1; i++)
// add instructions (gap 105 on the older shape), and a dead arm or empty test
// is deleted before cse2. GCSE appears to leave the 0 in r9 unfolded (not
// checked in its dump), so it should need only that boundary.
// Tried with no gain: a fill value local for 255 (gap 110), a box row pointer
// or table local (119, 120, 113), struct or array reads of the spare pair
// (no change; the native derives 0x14c from 0x148 with adds #4), a wrapper on
// every call or on Func_02003790 (no change), and declaration orders.
// Leader and object assigned after the declarations give differ 206 but gap
// 94. Overlay 668 Func_020044c4 is the same listing (calling
// Map_FillCellRectByte2 where this calls Func_02003790).
// Its one argument is islets.h struct IsletsTarget, 24 bytes passed by value:
// the first 16 in r0 to r3, which the native stores in the incoming argument
// area (the sub sp, #16), the object number and the optional callback on the
// stack. The map slot goes through struct HeapSlotWords so that the load keeps
// the four argument stores ahead of it (its alias set is in the push state).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/vector3.h"
#include "call.h"

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
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->map;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(push.number);
  s32 speed, acceleration;
  struct Vector3 cell, start;
  s32 kind = push.kind;
  s32 row;
  s32 rowZ;
  s32 direction, width, height, spareX, spareZ;

  speed = leader->speedLimit;
  acceleration = leader->acceleration;
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
   * shares a constant; as direct calls, 292 of 305 instructions differ in
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
  row = kind * 4;
  rowZ = row + 1;
  push.x = (push.x + (((s8 *)gData_0200407c)[row] << 16)) >> 20;
  push.z = (push.z + (((s8 *)gData_0200407c)[rowZ] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  Func_02003e24(push.x, push.z, width, height, spareX + push.x,
                spareZ + push.z);
  Func_02003790(0, push.x, push.z, width, height, 255);
  Func_02003790(2, push.x, push.z, width, height, 255);
  cell.x = (start.x + (((s8 *)gData_0200407c)[row] << 16)) >> 20;
  cell.z = (start.z + (((s8 *)gData_0200407c)[rowZ] << 16)) >> 20;
  Func_02003e24(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Func_02003790(2, cell.x, cell.z, width, height, 0);
  Func_02003fd4();
}
