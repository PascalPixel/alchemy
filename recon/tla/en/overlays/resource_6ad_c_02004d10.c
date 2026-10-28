// Overlay 6ad's copy of resource_668_c_020044c4.c, with this overlay's symbols:
// differ 337 of 305. That draft's header follows. Draft, differ 337 of 305:
// carries out a push measured by Func_02004240: clears the cells under the
// object's box, walks the leader up to it (mode 8), slides the object to the
// push's end with the leader following (cue 239, Func_020054b8), runs the
// optional callback, restores the leader's speed, plays cues 298 and 213,
// places the object exactly, copies the spare cells into its new place and back
// over its old place (Func_020054e8) and marks layer 0 and 2 there. The same
// listing as overlay 674's Func_02003a5c. Left: registers and stack slots
// throughout (the native keeps the box sizes, the saved speeds and both cell
// vectors on the stack and gMainRoutineWork's owner address in r10); the
// leader's last offset is a guess at the native's shift pair.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/vector3.h"

struct PushTarget {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
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

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

void Func_02004d10(struct PushTarget target, s32 id, void (*callback)(void)) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(id);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  struct Vector3 start, cell;
  s8 *box = gData_02005f64[target.kind];
  s32 width, height, spareX, spareZ;

  start.x = object->x;
  start.z = object->z;
  cell.x = (object->x + (box[0] << 16)) >> 20;
  cell.z = (object->z + (box[1] << 16)) >> 20;
  width = (Abs(box[0]) + Abs(box[2])) >> 4;
  height = (Abs(box[1]) + Abs(box[3])) >> 4;
  Func_02004a44(0, cell.x, cell.z, width, height, 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                            (target.x - start.x) >> 17,
                                            (target.z - start.z) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02004fc0;
  wait(4);
  Func_02005480(object, 2);
  Func_020056c0(239);
  Actor_SetSpeedParameters(id, 0x8000, 0x1999);
  Func_020054b8(object, target.x, target.y, target.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(
      gMainRoutineWork.currentOwner, (s32)(gData_02005f7c[0] << 16) >> 31,
      (s32)(gData_02005f7c[0] << 16) >> 17);
  if (callback != 0)
    callback();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_020054c0(object);
  Func_020056c0(298);
  Func_020056c0(213);
  Actor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_02005480(object, 1);
  target.x = (target.x + (box[0] << 16)) >> 20;
  target.z = (target.z + (box[1] << 16)) >> 20;
  spareX = *(s32 *)(map + 0x148) >> 20;
  spareZ = *(s32 *)(map + 0x14c) >> 20;
  Func_020054e8(target.x, target.z, width, height, spareX + target.x,
                spareZ + target.z);
  Func_02004a44(0, target.x, target.z, width, height, 255);
  Func_02004a44(2, target.x, target.z, width, height, 255);
  cell.x = (start.x + (box[0] << 16)) >> 20;
  cell.z = (start.z + (box[1] << 16)) >> 20;
  Func_020054e8(spareX + cell.x, spareZ + cell.z, width, height, cell.x,
                cell.z);
  Func_02004a44(2, cell.x, cell.z, width, height, 0);
  Func_02005668();
}
