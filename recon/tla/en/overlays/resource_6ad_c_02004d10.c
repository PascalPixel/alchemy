// Near miss, differ 327 of 305 (tla-en, 2026-10-31): the shape is the
// native's (one 24-byte PushMove by value, its first four words in r0-r3
// spilled to the pretend-argument area and number and callback on the
// stack, as overlay 668's caller passes it; the direction in a local whose
// times-four lives in one register throughout; the tables not const, so
// their bytes are reread after each call; start below cell in the frame).
// Left: the native's CSE starts afresh between the leader's update hook and
// Actor_WaitForMoveById, with no branch there: it reloads gMainRoutineWork
// and gData_02005f64 into new registers (r5, r10) and keeps stepIndex, 0,
// in r9 as a register index into gData_02005f7c; this compiler carries all
// three through the whole function (stepIndex folds to a constant index),
// which moves the allocation of every callee-saved register. A one-pass
// block at wait(4) splits the first CSE pass (differ 316) but the second
// joins the loads again; reading move.direction at each use instead of the
// local measures 310, without the native's one register. The leader's last
// offset is a guess at the native's shift pair.
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
  struct Actor *leader;
  struct Actor *object;
  s32 speed;
  s32 acceleration;
  struct Vector3 cell;
  struct Vector3 start;
  s32 direction;
  s32 stepIndex;
  s32 width;
  s32 height;
  s32 spareX;
  s32 spareZ;

  map = (u8 *)Ram_HeapSlots->mapWork;
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
