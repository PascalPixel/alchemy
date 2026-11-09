#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern s8 gData_02004cb0[];
extern s32 gData_02004cc8[];

void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void Func_02004a0c(struct Actor *object, s32 mode);
void Func_02004c0c(void);
void Func_02004774(void);

/// The heap slots read as words: the map slot comes through u32, not its
/// pointer type, so its load stays behind the stores of the block.
struct HeapSlotWords {
  u32 slots[8];
  u32 mapWork;
};

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_020044c4(struct PushTarget target) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(target.id);
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
  cell[0] = (object->x + (gData_02004cb0[footprint * 4] << 16)) >> 20;
  cell[2] = (object->z + (gData_02004cb0[footprint * 4 + 1] << 16)) >> 20;
  width = (ABS(gData_02004cb0[footprint * 4]) +
           ABS(gData_02004cb0[footprint * 4 + 2])) >>
          4;
  height = (ABS(gData_02004cb0[footprint * 4 + 1]) +
            ABS(gData_02004cb0[footprint * 4 + 3])) >>
           4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX,
                                       deltaZ);
  }
  Actor_Get(gMainRoutineWork.currentOwner)->update = Func_02004774;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Func_02004a0c(object, 3);
  else
    Func_02004a0c(object, 2);
  Event_PlaySound(239);
  Actor_SetSpeed(target.id, 0x8000, 0x1999);
  FieldActor_SetMoveTarget(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                                     (gData_02004cc8[direction] << 16) >> 31,
                                     (gData_02004cc8[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Actor_WaitForMove(object);
  Event_PlaySound(298);
  Event_PlaySound(213);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_02004a0c(object, 1);
  target.x = target.x + (gData_02004cb0[footprint * 4] << 16);
  target.z = target.z + (gData_02004cb0[footprint * 4 + 1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gData_02004cb0[footprint * 4] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02004cb0[footprint * 4 + 1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Func_02004c0c();
}
