#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "map.h"
#include "view_move.h"

struct EffectScene {
  u8 unk00[0x18];
  s16 firstObjectId;
};

void Map_SetScreenShakeFar(s32 shakeX, s32 shakeY, s32 decay);
void Camera_FollowActor(s32 id, s32 mode);

// Ends a view move: gives the map back the view bounds Func_080e89e4 kept
// and lets the view follow the scene's first object again. Where the view
// was held, it only stops the screen shake.
void Func_080e8c9c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct EffectScene *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;

  if (*(s32 *)map->unk0fc != 0 || event->controlMode == 3) {
    Map_SetScreenShakeFar(0, 0, 0);
  } else {
    map->minX = work->savedMinX;
    map->maxX = work->savedMaxX;
    map->minZ = work->savedMinZ;
    map->maxZ = work->savedMaxZ;
    Camera_FollowActor(scene->firstObjectId, 1);
  }
}
