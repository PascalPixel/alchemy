// Draft, differ 337 of 305: overlay 674's copy of overlay 668's
// Func_020044c4 (the same listing, calling Func_02003790 where 668 calls
// Map_FillCellRectByte2), whose draft holds its notes.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
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
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02003790(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void ObjectMotion_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Object_SetModeById(u32 id, s32 mode);
void WaitFrames(s32 frames);
void ObjectMotion_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 id);
void Func_02003d0c(void);
void Func_02003de4(struct ObjectRuntime *object, s32 mode);
void Func_02003ffc(s32 cue);
void Func_02003e0c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02003e14(struct ObjectRuntime *object);
void Func_02003e24(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02003fd4(void);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

void Func_02003a5c(struct PushTarget target, s32 id, void (*callback)(void)) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *object = Object_GetById(id);
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
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x1999);
  Object_SetModeById(gPartyState.currentOwner, 8);
  WaitFrames(15);
  ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner,
                                            (target.x - start.x) >> 17,
                                            (target.z - start.z) >> 17);
  Object_GetById(gPartyState.currentOwner)->update = Func_02003d0c;
  WaitFrames(4);
  Func_02003de4(object, 2);
  Func_02003ffc(239);
  ObjectMotion_SetSpeedParameters(id, 0x8000, 0x1999);
  Func_02003e0c(object, target.x, target.y, target.z);
  ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
  Object_SetModeById(gPartyState.currentOwner, 2);
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x4ccc, 0x1999);
  ObjectMotion_OffsetPositionAndResetMotion(
      gPartyState.currentOwner, (s32)(gData_02004094[0] << 16) >> 31,
      (s32)(gData_02004094[0] << 16) >> 17);
  if (callback != 0)
    callback();
  ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
  Object_SetModeById(gPartyState.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02003e14(object);
  Func_02003ffc(298);
  Func_02003ffc(213);
  Object_SetPositionAndResetMotion(object, target.x, object->y, target.z);
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
