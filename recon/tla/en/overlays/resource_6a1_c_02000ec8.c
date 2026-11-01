// Draft, differ 259 of 305, skeleton gap 119 (tla-en, 2026-11-01): overlay
// 6a1's copy of overlay 69c's Func_02001050 (resource_69c_c_02001050.c), which
// pushes an object to its stopping point. It takes the six-word block by value,
// with id and callback its last two words, which gives the sub sp, #16 entry;
// the speed calls go through the call layer (called directly, CSE shares 0x8000
// and 0x1999). The footprint table is read as base plus (footprint * 4 + k) and
// is not const: the native rereads it after calls, and as const CSE keeps its
// values and their shifts across the whole function.
// Left: a CSE boundary after the leader's update store that survives cse2.
// There the native recomputes &gMainRoutineWork.currentOwner (r10, then r5),
// keeps its zero step index in r9 and shifts it at its use, and has footprint
// + 1 fresh at the end; here all three merge, and with no r9 the allocation
// shifts, so the table base loses its register and reloads in the first block.
// The twin's same store in both arms reaches the boundary (gap 111) but is a
// dead device, so it is not here. A local owner pointer merges away; a table
// local (first part, or set twice) folds to (table + footprint) + k, gap 116 to
// 140 and differ 298 or worse. A fill local set to 255 after the leader's
// update store gives the native's movs r5, #255 there, but gap 121, differ 277.
// Declaration order changes nothing; speed, acceleration and map read later,
// and height before width, do not help.
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

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_02000ec8(struct PushTarget target) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(target.id);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 cell[3];
  s32 from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 step = 0;

  from[0] = object->x;
  from[2] = object->z;
  footprint = target.footprint * 4;
  cell[0] = (object->x + (gData_020022dc[footprint] << 16)) >> 20;
  cell[2] = (object->z + (gData_020022dc[footprint + 1] << 16)) >> 20;
  width =
      (ABS(gData_020022dc[footprint]) + ABS(gData_020022dc[footprint + 2])) >>
      4;
  height =
      (ABS(gData_020022dc[footprint + 1]) + ABS(gData_020022dc[footprint + 3])) >>
      4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (target.x - from[0]) >> 17,
                                     (target.z - from[2]) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02001178;
  wait(4);
  Func_02002160(object, 2);
  Func_020022c8(239);
  Call3((void (*)())Actor_SetSpeedParameters, target.id, 0x8000, 0x1999);
  Func_02002190(object, target.x, target.y, target.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (gData_020022f4[step] << 16) >> 31,
                                     (gData_020022f4[step] << 16) >> 17);
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
  target.x = (target.x + (gData_020022dc[footprint] << 16)) >> 20;
  target.z = (target.z + (gData_020022dc[footprint + 1] << 16)) >> 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  Call6((void (*)())Func_020021b0, target.x, target.z, width, height,
        originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gData_020022dc[footprint] << 16)) >> 20;
  cell[2] = (from[2] + (gData_020022dc[footprint + 1] << 16)) >> 20;
  Call6((void (*)())Func_020021b0, originX + cell[0], originZ + cell[2], width,
        height, cell[0], cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Func_020022a0();
}
