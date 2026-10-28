// Draft, differ 337 of 305: overlay 674's copy of overlay 668's
// Func_020044c4 (the same listing, calling Func_02003790 where 668 calls
// Map_FillCellRectByte2), whose draft holds its notes.
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "system/vector3.h"

struct PushTarget {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
};

extern s8 gData_0200407c[][4];
extern s32 gData_02004094[];
struct Actor *Actor_GetById(u32 id);
void Func_02003790(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_SetModeById(u32 id, s32 mode);
void WaitFrames(s32 frames);
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

void Func_02003a5c(struct PushTarget target, s32 id, void (*callback)(void)) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct Actor *object = Actor_GetById(id);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  struct Vector3 start, cell;
  s8 *box = gData_0200407c[target.kind];
  s32 width, height, spareX, spareZ;

  start.x = object->x;
  start.z = object->z;
  cell.x = (object->x + (box[0] << 16)) >> 20;
  cell.z = (object->z + (box[1] << 16)) >> 20;
  width = (Abs(box[0]) + Abs(box[2])) >> 4;
  height = (Abs(box[1]) + Abs(box[3])) >> 4;
  Func_02003790(0, cell.x, cell.z, width, height, 0);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gAdventure.currentOwner, 8);
  WaitFrames(15);
  Actor_OffsetPositionAndResetMotion(gAdventure.currentOwner,
                                            (target.x - start.x) >> 17,
                                            (target.z - start.z) >> 17);
  Actor_GetById(gAdventure.currentOwner)->update = Func_02003d0c;
  WaitFrames(4);
  Func_02003de4(object, 2);
  Func_02003ffc(239);
  Actor_SetSpeedParameters(id, 0x8000, 0x1999);
  Func_02003e0c(object, target.x, target.y, target.z);
  Actor_WaitForMoveById(gAdventure.currentOwner);
  Actor_SetModeById(gAdventure.currentOwner, 2);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(
      gAdventure.currentOwner, (s32)(gData_02004094[0] << 16) >> 31,
      (s32)(gData_02004094[0] << 16) >> 17);
  if (callback != 0)
    callback();
  Actor_WaitForMoveById(gAdventure.currentOwner);
  Actor_SetModeById(gAdventure.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02003e14(object);
  Func_02003ffc(298);
  Func_02003ffc(213);
  Actor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_02003de4(object, 1);
  target.x = (target.x + (box[0] << 16)) >> 20;
  target.z = (target.z + (box[1] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  Func_02003e24(target.x, target.z, width, height, spareX + target.x,
                spareZ + target.z);
  Func_02003790(0, target.x, target.z, width, height, 255);
  Func_02003790(2, target.x, target.z, width, height, 255);
  cell.x = (start.x + (box[0] << 16)) >> 20;
  cell.z = (start.z + (box[1] << 16)) >> 20;
  Func_02003e24(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Func_02003790(2, cell.x, cell.z, width, height, 0);
  Func_02003fd4();
}
