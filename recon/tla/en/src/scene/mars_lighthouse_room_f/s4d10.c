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

extern s8 gData_02005f64[];
extern s32 gData_02005f7c[];

struct Actor *Actor_GetById(s32 id);
void Func_02004a44(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void wait(s32 frames);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Func_02005480(struct Actor *object, s32 mode);
void Func_020056c0(s32 sound);
void Func_020054b8(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_020054c0(struct Actor *object);
void Func_020054e8(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Func_02005668(void);
void Func_02004fc0(void);

/// The heap slots read as words: the map slot comes through u32, not its
/// pointer type, so its load stays behind the stores of the block.
struct HeapSlotWords {
  u32 slots[8];
  u32 mapWork;
};

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_02004d10(struct PushTarget target) {
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
  cell[0] = (object->x + (gData_02005f64[footprint * 4] << 16)) >> 20;
  cell[2] = (object->z + (gData_02005f64[footprint * 4 + 1] << 16)) >> 20;
  width = (ABS(gData_02005f64[footprint * 4]) +
           ABS(gData_02005f64[footprint * 4 + 2])) >>
          4;
  height = (ABS(gData_02005f64[footprint * 4 + 1]) +
            ABS(gData_02005f64[footprint * 4 + 3])) >>
           4;
  Func_02004a44(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, SPLIT_CONSTANT(0x1900, 153));
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, deltaX,
                                       deltaZ);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02004fc0;
  wait(4);
  if ((u32)(direction - 6) <= 7)
    Func_02005480(object, 3);
  else
    Func_02005480(object, 2);
  Func_020056c0(239);
  Call3((void (*)())Actor_SetSpeedParameters, target.id, 0x8000, 0x1999);
  Func_020054b8(object, target.x, target.y, target.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (gData_02005f7c[direction] << 16) >> 31,
                                     (gData_02005f7c[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
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
  target.x = target.x + (gData_02005f64[footprint * 4] << 16);
  target.z = target.z + (gData_02005f64[footprint * 4 + 1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  Call6((void (*)())Func_020054e8, target.x, target.z, width, height,
        originX + target.x, originZ + target.z);
  Func_02004a44(0, target.x, target.z, width, height, 255);
  Func_02004a44(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gData_02005f64[footprint * 4] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02005f64[footprint * 4 + 1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  Func_020054e8(originX, originZ, width, height, cell[0], cell[2]);
  Func_02004a44(2, cell[0], cell[2], width, height, 0);
  Func_02005668();
}
