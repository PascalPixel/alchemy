// Draft, differ 337 of 305: carries out a push measured by Func_02004240: clears
// the cells under the object's box, walks the leader up to it (mode 8),
// slides the object to the push's end with the leader following (cue 239,
// Func_02004a34), runs the optional callback, restores the leader's speed,
// plays cues 298 and 213, places the object exactly, copies the spare
// cells into its new place and back over its old place (Func_02004a54) and
// marks layer 0 and 2 there. The same listing as overlay 674's
// Func_02003a5c. Left: registers and stack slots throughout (the native
// keeps the box sizes, the saved speeds and both cell vectors on the stack
// and the adventure's owner address in r10); the leader's last offset is
// a guess at the native's shift pair.
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "system/vector3.h"

struct PushTarget {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
};

extern s8 gData_02004cb0[][4];
extern s32 gData_02004cc8[];
struct ObjectRuntime *Object_GetById(u32 id);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void ObjectMotion_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Object_SetModeById(u32 id, s32 mode);
void WaitFrames(s32 frames);
void ObjectMotion_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 id);
void Func_02004774(void);
void Func_02004a0c(struct ObjectRuntime *object, s32 mode);
void Func_02004c9c(s32 cue);
void Func_02004a34(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02004a3c(struct ObjectRuntime *object);
void Func_02004a54(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02004c0c(void);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

void Func_020044c4(struct PushTarget target, s32 id, void (*callback)(void)) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *object = Object_GetById(id);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  struct Vector3 start, cell;
  s8 *box = gData_02004cb0[target.kind];
  s32 width, height, spareX, spareZ;

  start.x = object->x;
  start.z = object->z;
  cell.x = (object->x + (box[0] << 16)) >> 20;
  cell.z = (object->z + (box[1] << 16)) >> 20;
  width = (Abs(box[0]) + Abs(box[2])) >> 4;
  height = (Abs(box[1]) + Abs(box[3])) >> 4;
  Map_FillCellRectByte2(0, cell.x, cell.z, width, height, 0);
  ObjectMotion_SetSpeedParameters(gAdventure.currentOwner, 0x8000, 0x1999);
  Object_SetModeById(gAdventure.currentOwner, 8);
  WaitFrames(15);
  ObjectMotion_OffsetPositionAndResetMotion(gAdventure.currentOwner,
                                            (target.x - start.x) >> 17,
                                            (target.z - start.z) >> 17);
  Object_GetById(gAdventure.currentOwner)->update = Func_02004774;
  WaitFrames(4);
  Func_02004a0c(object, 2);
  Func_02004c9c(239);
  ObjectMotion_SetSpeedParameters(id, 0x8000, 0x1999);
  Func_02004a34(object, target.x, target.y, target.z);
  ObjectMotion_CommitCurrentPositionAndActivate(gAdventure.currentOwner);
  Object_SetModeById(gAdventure.currentOwner, 2);
  ObjectMotion_SetSpeedParameters(gAdventure.currentOwner, 0x4ccc, 0x1999);
  ObjectMotion_OffsetPositionAndResetMotion(
      gAdventure.currentOwner, (s32)(gData_02004cc8[0] << 16) >> 31,
      (s32)(gData_02004cc8[0] << 16) >> 17);
  if (callback != 0)
    callback();
  ObjectMotion_CommitCurrentPositionAndActivate(gAdventure.currentOwner);
  Object_SetModeById(gAdventure.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02004a3c(object);
  Func_02004c9c(298);
  Func_02004c9c(213);
  Object_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_02004a0c(object, 1);
  target.x = (target.x + (box[0] << 16)) >> 20;
  target.z = (target.z + (box[1] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  Func_02004a54(target.x, target.z, width, height, spareX + target.x,
                spareZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell.x = (start.x + (box[0] << 16)) >> 20;
  cell.z = (start.z + (box[1] << 16)) >> 20;
  Func_02004a54(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Map_FillCellRectByte2(2, cell.x, cell.z, width, height, 0);
  Func_02004c0c();
}
