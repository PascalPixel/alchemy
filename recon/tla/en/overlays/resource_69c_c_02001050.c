// Draft, differ 72 of 305, skeleton gap 34 (tla-en, 2026-11-01): pushes an
// object to the stopping point Func_02000dcc found: clears byte 2 of the cells
// under its footprint, steps the leader back and lets it push (mode 8, sound
// 239), slides the object over (Func_02002310) while the leader follows, runs
// the optional callback, then sets the object down (sounds 298 and 213), copies
// the cells to its new place and fills byte 2 there. Shared with overlay 6a1
// and others. It takes the six-word block by value, as room 3's local.h
// declares it (id and callback are its last two words, so r0 to r3 are stored
// above the frame with the stack words), and its speed calls go through the
// call layer (called directly, CSE shares 0x8000 and 0x1999).
// Left: step reads uninitialized here, but the native sets it to 0 in the first
// block (movs r1, #0; mov r9, r1) and shifts it at its use. It must get that
// zero before adoption. A plain step = 0 is folded: gcse's cprop cannot put 0
// in the shift, so it adds a REG_EQUAL 0 note that cse2 folds (gap 89, differ
// 175), under any spelling of the index (shift, * 4, a pointer sum) or a set
// at the start of the code. Only a zero that combine alone can see stays: step
// = (target.footprint & 3) >> 2 after from[2] gave gap 36, differ 68. That
// is a steering form, so it is not here. A set that
// differs per arm of the device is deleted by CSE as a no-op; a counted
// for (; step < 1; step++) stays in the output as a loop.
// Also left: the first block's order (the native stores r0 to r2 first, loads
// speed and acceleration in r3 and r4, then &from in r0 and &cell in r1), the
// dead mov r2, r9 the device's test leaves before the update store, and
// r4 for r0 at &currentOwner after the slide.
// The footprint table and the step table are not const: the native rereads
// them after calls. The table is read as base plus (footprint * 4 + k), and
// row is footprint's copy for the code after the boundary, which stops PRE
// sharing footprint + 1 with the first block; the two meet in r7 again. The
// native recomputes &gMainRoutineWork.currentOwner after the slide (r10, then
// r5) and reloads the footprint table at the end: a CSE boundary after the
// leader's update store that survives cse2. Only the both-arms store below
// reaches it; an unused label, a goto, do { } while (0) and for (;;) with a
// break (gap 105 at best, with the store plain), one-pass loops and a
// conditional call do not. Its test is on step because that lifts step into
// r9 over speed. speed and acceleration are declared last so that they go to
// the stack, and cell before from gives the native's frame slots.
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

extern s8 gData_02002484[];
extern s32 gData_0200249c[];

struct Actor *Actor_GetById(s32 id);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void wait(s32 frames);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Func_020022e0(struct Actor *object, s32 mode);
void Func_02002470(s32 sound);
void Func_02002310(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_02002318(struct Actor *object);
void Func_02002330(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Func_02002430(void);
void Func_02001300(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_02001050(struct PushTarget target) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(target.id);
  s32 cell[3];
  s32 from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 step;
  s32 row;
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;

  from[0] = object->x;
  from[2] = object->z;
  footprint = target.footprint * 4;
  row = footprint;
  cell[0] = (object->x + (gData_02002484[footprint] << 16)) >> 20;
  cell[2] = (object->z + (gData_02002484[footprint + 1] << 16)) >> 20;
  width =
      (ABS(gData_02002484[footprint]) + ABS(gData_02002484[footprint + 2])) >>
      4;
  height =
      (ABS(gData_02002484[footprint + 1]) + ABS(gData_02002484[footprint + 3])) >>
      4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (target.x - from[0]) >> 17,
                                     (target.z - from[2]) >> 17);
  {
    struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);

    /* FAKEMATCH(dead): the same store in both arms of a test on step, which
     * ends CSE's block there; as one store, 287 of 305 instructions differ in
     * tla-en. */
    if (step)
      actor->update = Func_02001300;
    else
      actor->update = Func_02001300;
  }
  wait(4);
  Func_020022e0(object, 2);
  Func_02002470(239);
  Call3((void (*)())Actor_SetSpeedParameters, target.id, 0x8000, 0x1999);
  Func_02002310(object, target.x, target.y, target.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (gData_0200249c[step] << 16) >> 31,
                                     (gData_0200249c[step] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02002318(object);
  Func_02002470(298);
  Func_02002470(213);
  Actor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_020022e0(object, 1);
  target.x = (target.x + (gData_02002484[row] << 16)) >> 20;
  target.z = (target.z + (gData_02002484[row + 1] << 16)) >> 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  Call6((void (*)())Func_02002330, target.x, target.z, width, height,
        originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gData_02002484[row] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02002484[row + 1] << 16)) >> 20;
  Call6((void (*)())Func_02002330, originX + cell[0], originZ + cell[2], width,
        height, cell[0], cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Func_02002430();
}
