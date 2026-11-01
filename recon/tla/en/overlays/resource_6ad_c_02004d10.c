// Draft, differ 327 of 305, skeleton gap 121 (tla-en, 2026-11-01): the shape
// is the native's (one 24-byte PushMove by value, its first four words in
// r0-r3 spilled to the pretend-argument area and number and callback on the
// stack, as overlay 668's caller passes it; the direction in a local whose
// times-four lives in one register throughout; the tables not const; start
// below cell in the frame; the map slot through struct HeapSlotWords, which
// keeps the four argument stores ahead of it). Left: the native's middle.
// With devices the same text reaches differ 88, gap 48, 305 instructions, the
// right register for every long-lived value; they are out because that is not
// differ 0, and this is the recipe, all of it measured:
// - stepIndex is a variable the native never folds because it sits in a
//   one-iteration loop: `for (step = 0; step < 1; step++) { ... stepIndex++; }`
//   from just after Func_020054b8 to just after the mode 1 call, `step` a
//   separate counter so that stepIndex keeps two definitions through GCSE.
//   loop.c deletes the back branch only at n_iterations 1 and at most 50 real
//   insns (unroll.c, "Loop too big": 42 here, a loop from wait(4) has 62, stays).
//   stepIndex++ then dies in flow and r9 stays. Set `stepIndex = 0` before the
//   first ABS: nearer the top gives speed r9 (differ 154), and it is set in
//   native's first block, so the last open point is how it gets there.
// - `spareX = ABS(move.x - leader->x) + ABS(move.z - leader->z);` before wait(4)
//   and `spareZ = ABS(stepIndex) + ABS(stepIndex + 1);` first in the loop give
//   the two CSE boundaries: the conditional also keeps the address from being
//   hoisted whole into one pool word (loop.c, maybe_never). Reads of start here
//   put &start in r7 or r11; on leader they do not. Two ABS are needed, one
//   changes nothing, which looks like CSE's path length running out.
// - A static inline forwarder on Actor_SetSpeedParameters alone rebuilds
//   0x8000 and 0x1999 at each call; on all four Actor calls it costs 41 more.
// - Declaration order map, width, speed, acceleration gives the native's slots.
// What differ 88 still lacks: Func_02004a44's last 0 is `mov r3, r9`, not
// `movs r3, #0` (CSE1 follows the ABS branches and swaps in stepIndex; that ref
// is also what lifts stepIndex over speed, with its live length doubled by the
// constant), and that moves the first block's reloads and the call 2 arguments.
// A native ordering with no i-in-store needs stepIndex above speed some other
// way. No gain from: index types (s16, u8, long same; others far worse), a
// `clear` local, a forwarder on Func_02004a44, inline Abs in four body forms, a
// dead read of stepIndex after the loop to keep its increment (179 to 240), more
// dead sums to stretch speed's live range, and two more ABS before the fill call
// (249: stepIndex's first set is deleted). Sibling drafts 668 Func_020044c4 and
// 674 Func_02003a5c are the same listing, so this recipe should carry over.
//
// Overlay 6ad's copy of resource_668_c_020044c4.c, with this overlay's
// symbols: carries out a push measured as overlay 668's Func_02004240
// measures one (this overlay's Func_02004a8c has the same shape): clears
// the cells under the object's box, walks the leader up to it (mode 8),
// slides the object to the push's end with the leader following (cue 239,
// Func_020054b8), runs the optional callback, restores the leader's speed,
// plays cues 298 and 213, places the object exactly, copies the spare cells
// into its new place and back over its old place (Func_020054e8) and marks
// layer 0 and 2 there. The same listing as overlay 674's Func_02003a5c.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/vector3.h"

struct PushMove {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 direction;
  s32 unk10;
  void (*unk14)(void);
};

struct HeapSlotWords {
  s32 unk00[8];
  s32 map;
};

extern s8 gData_02005f64[][4];
extern s32 gData_02005f7c[];
struct Actor *Actor_GetById(u32 id);
void Func_02004a44(s32 layer, s32 x, s32 z, s32 width, s32 height, s32 value);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_SetModeById(u32 id, s32 mode);
void wait(s32 frames);
void Actor_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(u32 id);
void Func_02004fc0(void);
void Func_02005480(struct Actor *object, s32 mode);
void Func_020056c0(s32 cue);
void Func_020054b8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020054c0(struct Actor *object);
void Func_020054e8(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02005668(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_02004d10(struct PushMove move) {
  u8 *map;
  s32 width;
  s32 speed;
  s32 acceleration;
  struct Actor *leader;
  struct Actor *object;
  s32 height;
  s32 direction;
  s32 stepIndex;
  struct Vector3 cell;
  struct Vector3 start;
  s32 spareX;
  s32 spareZ;

  map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->map;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  object = Actor_GetById(move.unk10);
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  start.x = object->x;
  start.z = object->z;
  direction = move.direction;
  stepIndex = 0;
  cell.x = (object->x + (gData_02005f64[direction][0] << 16)) >> 20;
  cell.z = (object->z + (gData_02005f64[direction][1] << 16)) >> 20;
  width = (ABS(gData_02005f64[direction][0]) +
           ABS(gData_02005f64[direction][2])) >>
          4;
  height = (ABS(gData_02005f64[direction][1]) +
            ABS(gData_02005f64[direction][3])) >>
           4;
  Func_02004a44(0, cell.x, cell.z, width, height, 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (move.x - start.x) >> 17,
                                     (move.z - start.z) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02004fc0;
  wait(4);
  Func_02005480(object, 2);
  Func_020056c0(239);
  Actor_SetSpeedParameters(move.unk10, 0x8000, 0x1999);
  Func_020054b8(object, move.x, move.y, move.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(
      gMainRoutineWork.currentOwner,
      (s32)(gData_02005f7c[stepIndex] << 16) >> 31,
      (s32)(gData_02005f7c[stepIndex] << 16) >> 17);
  if (move.unk14 != 0)
    move.unk14();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_020054c0(object);
  Func_020056c0(298);
  Func_020056c0(213);
  Actor_SetPositionAndResetMotion(object, move.x, object->y, move.z);
  Func_02005480(object, 1);
  move.x = (move.x + (gData_02005f64[direction][0] << 16)) >> 20;
  move.z = (move.z + (gData_02005f64[direction][1] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  Func_020054e8(move.x, move.z, width, height, spareX + move.x,
                spareZ + move.z);
  Func_02004a44(0, move.x, move.z, width, height, 255);
  Func_02004a44(2, move.x, move.z, width, height, 255);
  cell.x = (start.x + (gData_02005f64[direction][0] << 16)) >> 20;
  cell.z = (start.z + (gData_02005f64[direction][1] << 16)) >> 20;
  Func_020054e8(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Func_02004a44(2, cell.x, cell.z, width, height, 0);
  Func_02005668();
}
