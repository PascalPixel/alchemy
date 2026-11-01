// Draft, differ 312 of 305, skeleton gap 108 (tla-en, 2026-11-01)
// Three lines from differ 10. With these the skeleton gap is 5 + 5 and the only
// difference left is order; all three come back out here because they do not
// reach differ 0:
//   __asm__("" : "=r"(direction) : "0"(direction));
//     after start.z = object->z; keeps direction unknown, so it stays in r9.
//   __asm__ __volatile__("" : : "i"(0));
//     after Func_02004c9c(239) and again after Func_02004a34: each flushes CSE.
// What each proves. CSE's table is flushed at both points in the native: the
// owner address, 0x8000 and 0x1999 are rebuilt after them, the box table is
// reloaded late, and only kind << 2 (r7) carries across. Only a bare
// ASM_OPERANDS flushes (cse.c, "A volatile ASM invalidates everything"); a
// one-pass block's LOOP_END stops the first pass only, plain asm("") is
// ASM_INPUT, an output asm is a SET, and (void)&&label; label: changes nothing
// (differ 309). direction is never folded in the native although it is set
// once: gcse cprop adds a REG_EQUAL note to the lsls and cse2 folds it, unless
// the set is unavailable at that use, and no plain form for that was found.
// Flushes alone give differ 164, gap 82; the direction asm alone 309, gap 113.
// Left under the three lines, 5 instructions moved, the same class as the Broken
// Seal twins' loads issued too early. In block 0 the native has str r3, [r0, #8]
// before ldr r5 (the table, priority 19 here, under 18 there) and str r1,
// [sp, #8] before lsls r7; after the push.z sum it has ldr r4, [sp, #28] one
// later and the stores of push.x and push.z before mov r9, r2. The scheduler's
// load cost (-mthumb-slow-loads) may be the cause: that is K2, out of scope.
// Tried on these with no gain: x and z locals for the push sum, a spare pointer
// or struct (78, 10), a map struct, spare read first (153), z first (45), pointer
// locals for start and cell (26), a table local (182), ox and oz locals (28),
// long fields (12), s32 returns on the other prototypes (10 to 16).
// Settled here, keep them: kind as a local (the second half stops reloading it);
// direction set before kind and speed (reload rotation r1, r2, r3, r4, r0 then
// agrees, so its set comes first in RTL); width declared before speed (slots);
// cell before start; dx, dz and toX, toZ locals (argument order); Func_02004a0c
// returning s32, as resource_668_c_02000200.c has it. Overlay 674's
// Func_02003a5c is the same listing and should take all of this.
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
  s32 kind = push.kind;
  s32 width;
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  struct Vector3 cell, start;
  s32 height, spareX, spareZ;

  start.x = object->x;
  start.z = object->z;
  cell.x = (object->x + (gData_02004cb0[kind][0] << 16)) >> 20;
  cell.z = (object->z + (gData_02004cb0[kind][1] << 16)) >> 20;
  width = (Abs(gData_02004cb0[kind][0]) + Abs(gData_02004cb0[kind][2])) >> 4;
  height = (Abs(gData_02004cb0[kind][1]) + Abs(gData_02004cb0[kind][3])) >> 4;
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
  push.x = (push.x + (gData_02004cb0[kind][0] << 16)) >> 20;
  push.z = (push.z + (gData_02004cb0[kind][1] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  {
    s32 toX = spareX + push.x;
    s32 toZ = spareZ + push.z;
    Func_02004a54(push.x, push.z, width, height, toX, toZ);
  }
  Map_FillCellRectByte2(0, push.x, push.z, width, height, 255);
  Map_FillCellRectByte2(2, push.x, push.z, width, height, 255);
  cell.x = (start.x + (gData_02004cb0[kind][0] << 16)) >> 20;
  cell.z = (start.z + (gData_02004cb0[kind][1] << 16)) >> 20;
  Func_02004a54(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Map_FillCellRectByte2(2, cell.x, cell.z, width, height, 0);
  Func_02004c0c();
}
