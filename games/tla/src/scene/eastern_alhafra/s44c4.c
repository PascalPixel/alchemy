/// @file
/// Scene code of overlay 0x668, in ROM order.

#include "../shared/push.h"
#include "../shared/map_cells.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field.h"
#include "map.h"
#include "module/field/field.h"
#include "module/scene/eastern_alhafra.h"
#include "module/scene/scene.h"

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern s8 gPushFootprints[];
extern s32 gPushDirectionSteps[];

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

  direction = leader->facing >> 16;
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gPushFootprints[footprint * 4] << 16)) >> 20;
  cell[2] = (object->z + (gPushFootprints[footprint * 4 + 1] << 16)) >> 20;
  width = (ABS(gPushFootprints[footprint * 4]) +
           ABS(gPushFootprints[footprint * 4 + 2])) >>
          4;
  height = (ABS(gPushFootprints[footprint * 4 + 1]) +
            ABS(gPushFootprints[footprint * 4 + 3])) >>
           4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX, deltaZ);
  }
  Actor_Get(gMainRoutineWork.currentOwner)->update = Actor_StopAtPushTarget;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Actor_SelectAnimation(object, 3);
  else
    Actor_SelectAnimation(object, 2);
  Event_PlaySound(239);
  Actor_SetSpeed(target.id, 0x8000, 0x1999);
  FieldActor_SetMoveTarget(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                             (gPushDirectionSteps[direction] << 16) >> 31,
                             (gPushDirectionSteps[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(298);
  Event_PlaySound(213);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Actor_SelectAnimation(object, 1);
  target.x = target.x + (gPushFootprints[footprint * 4] << 16);
  target.z = target.z + (gPushFootprints[footprint * 4 + 1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gPushFootprints[footprint * 4] << 16)) >> 20;
  cell[2] = (from[2] + (gPushFootprints[footprint * 4 + 1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}

DEFINE_ACTOR_STOP_AT_PUSH_TARGET(Actor_StopAtPushTarget, Actor_FindAtCell,
                                 gPushKinds, MapGrid_CheckCollision)

void Func_020047fc(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
  s16 mode;
  s16 flag;
  s32 limit;
  s32 first;
  limit = 0xff;
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
next:
  id = spec[0];
  object = Actor_Get(id);
  mode = spec[1];
  flag = spec[2];
  object->collisionFlags &= 0xfe;
  Actor_SetSpriteFlags(object, 0);
  Actor_SetCellOpen(id, (u16)mode, (u16)flag, object->terrainId);
  limit = 0xff;
  spec += 3;
  first = spec[0];
  if (first != ((limit << 8) + 0xff)) {
    goto next;
  }
}

// Finds the current target's table row and shrinks it.
DEFINE_STATUE_SINK(Statue_Sink, Actor_SetCellOpen)

#include "field_event.h"

DEFINE_ACTOR_SET_CELL_OPEN(Actor_SetCellOpen, Actor_GetCellIndex,
                           Actor_SelectAnimation)
