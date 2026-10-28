#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "map.h"
#include "object_runtime.h"
#include "macros.h"

void WaitFrames(s32);

void Camera_FollowActor(s32 id, s32 flag) {
  struct ObjectRuntime *obj;
  struct ObjectRuntime *target;
  struct EventWork *work;
  struct MapWork *map;

  obj = ObjectTable_Get(id);
  work = Ram_HeapSlots->eventWork;
  target = work->viewCenter;
  map = Ram_HeapSlots->mapWork;
  if (obj != 0) {
    map->origin = &target->x;
    ObjectDispatch_StartCameraFar(target, obj);
    if (flag == 0) {
      SET_POSITION(target, obj->x, obj->y, obj->z);
      WaitFrames(1);
      if (work->controlMode != 3)
        Map_CenterOnFollowedPointFar();
    }
  }
}

void Camera_SetSpeedParameters(s32 first, s32 second) {
  struct EventWork *work = Runtime_AllocateBlock(108, 0xd50);

  work->viewCenter->speedLimit = first;
  work->viewCenter->acceleration = second;
}
