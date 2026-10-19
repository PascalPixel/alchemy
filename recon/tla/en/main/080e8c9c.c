// Near miss, differ 6 of 45 (2026-10-19; 10 on 2026-10-15). The permuter
// found a one-pass block around the third bound store, which keeps the
// loads after their address steps as the ROM has them (10 -> 6); a second
// block around the second store changes nothing. Rewritten on view_move.h's
// ViewMoveWork and the map and event work structs, from the earlier draft at
// 12. What remains is order only: the ROM sets the call's 1 (movs r1, #1)
// after the second bound's load, where this sets it after the fourth, and
// forms the effect work's address one slot later; the first store's address
// copy also comes after its load. Measured: the four work pointers
// in nine orders (10-22; map, view-move work, scene, event best).
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "map.h"
#include "view_move.h"

struct EffectScene {
  u8 unk00[0x18];
  s16 firstObjectId;
};

void Runtime_SetWorkTripleIfNonNegativeFar(s32 shakeX, s32 shakeY, s32 decay);
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
    Runtime_SetWorkTripleIfNonNegativeFar(0, 0, 0);
  } else {
    map->minX = work->savedMinX;
    map->maxX = work->savedMaxX;
    do {
      map->minZ = work->savedMinZ;
    } while (0);
    map->maxZ = work->savedMaxZ;
    Camera_FollowActor(scene->firstObjectId, 1);
  }
}
