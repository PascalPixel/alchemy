// Draft, differ 12 of 305, skeleton gap 6 (tla-en, 2026-11-01): overlay 6a1's
// copy of overlay 69c's Func_02001050, which pushes an object to its stopping
// point; its twin is The Broken Seal's Actor_MoveAndRedraw (scene/shared/
// staged_actor.h), and most of this draft is that function's statements.
// It takes the six-word block by value, with id and callback its last two
// words, which gives the sub sp, #16 entry; the speed calls go through the call
// layer (called directly, CSE shares 0x8000 and 0x1999). The footprint table is
// not const: the native rereads it after calls.
// What closed the distance, and applies to 69c's copy (differ 72 with a dead
// device and an uninitialized step, neither needed here):
// - direction is the twin's leader facing, as leader->angle >> 16 (the twin has
//   >> 12). angle is a u16, so only combine sees the zero: it deletes the load,
//   leaves the 0 in r9 for the step table, and folds the twin's test on
//   direction, which until then ends CSE's block. That test is the boundary
//   after the leader's update store, and arm 2 is the one that stays. The
//   shift count is an inference from the bytes, not evidence of the source.
// - The map slot is read as a word, as the twin's *(u32 *)gMapGridWork, so it
//   conflicts with the block's stores and stays behind them. It has to go
//   through a struct field: a cast to a constant address builds adds r3, #32.
// - footprint is read before speed and shifted after from[2], and the deltas
//   are computed before the owner is read; both set reload's rotation.
// - cell and from come first in the frame, speed and acceleration last.
// Left: two scheduling sites with the right instructions in the wrong order.
// 1. First block: the native has mov r9, r1; ldr r3, [r6, #16]; mov r1, sp;
//    adds r1, #44; str r3, [r0, #8]; ldr r5, =table; str r1, [sp, #8]; lsls r7.
//    Here the pool load goes straight after str r3, [r0, #0] and lsls r7 ahead
//    of str r1. sched2 gives the load priority 19, tied with mov r9 and with
//    mov r1, sp, and it wins on dependents. The native needs it at about 17 and
//    lsls r7 at 16. A pool load has no memory dependence (unchanging), so
//    pointer locals for from and cell, *(from + 2), a struct for them and
//    a table local (differ 147) do not move it.
// 2. Second-last table read: the native has adds r1, r1, r3; ldr r4, [sp, #28];
//    two asrs; both stores; mov r9, r2. Here ldr r4 goes first and mov r9, r2
//    before the stores. The stores of target.x and target.z rank 229 through
//    their edge to the map load, against 230 for mov r9, r2, and they need one
//    more. Map reads as an array or a struct do not change it, and an empty
//    asm or do { } while (0) after the stores moves movs r5, #255 and stays at
//    differ 12, so no device is kept.
// Both read as the known load-after-store and store-cost gaps of K2's
// -mthumb-slow-loads approximation: the instructions and their dependences are
// the same, so a change there is a compiler question for Pascal.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern s8 gData_020022dc[];
extern s32 gData_020022f4[];

struct Actor *Actor_GetById(s32 id);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void wait(s32 frames);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Func_02002160(struct Actor *object, s32 mode);
void Func_020022c8(s32 sound);
void Func_02002190(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_02002198(struct Actor *object);
void Func_020021b0(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Func_020022a0(void);
void Func_02001178(void);

/// The heap slots read as words: the map slot comes through u32, not its pointer type, so its load stays behind the stores of the block.
struct HeapSlotWords {
  u32 slots[8];
  u32 mapWork;
};

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_02000ec8(struct PushTarget target) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(target.id);
  s32 cell[3];
  s32 from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  direction = leader->angle >> 16;
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gData_020022dc[footprint * 4] << 16)) >> 20;
  cell[2] = (object->z + (gData_020022dc[footprint * 4 + 1] << 16)) >> 20;
  width =
      (ABS(gData_020022dc[footprint * 4]) + ABS(gData_020022dc[footprint * 4 + 2])) >>
      4;
  height =
      (ABS(gData_020022dc[footprint * 4 + 1]) + ABS(gData_020022dc[footprint * 4 + 3])) >>
      4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, deltaX,
                                       deltaZ);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02001178;
  wait(4);
  if ((u32)(direction - 6) <= 7)
    Func_02002160(object, 3);
  else
    Func_02002160(object, 2);
  Func_020022c8(239);
  Call3((void (*)())Actor_SetSpeedParameters, target.id, 0x8000, 0x1999);
  Func_02002190(object, target.x, target.y, target.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (gData_020022f4[direction] << 16) >> 31,
                                     (gData_020022f4[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02002198(object);
  Func_020022c8(298);
  Func_020022c8(213);
  Actor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_02002160(object, 1);
  target.x = target.x + (gData_020022dc[footprint * 4] << 16);
  target.z = target.z + (gData_020022dc[footprint * 4 + 1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  Call6((void (*)())Func_020021b0, target.x, target.z, width, height,
        originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gData_020022dc[footprint * 4] << 16)) >> 20;
  cell[2] = (from[2] + (gData_020022dc[footprint * 4 + 1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  Call6((void (*)())Func_020021b0, originX, originZ, width, height, cell[0],
        cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Func_020022a0();
}
