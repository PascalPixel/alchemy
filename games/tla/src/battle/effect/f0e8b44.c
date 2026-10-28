#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "event_work.h"
#include "actor.h"
#include "map.h"
#include "fixed_math.h"
#include "view_move.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x))

/// Swings the view centre away from where Func_080e89e4 found it, along the
/// asked directions, by amplitude times the sine of the phase. The swing goes
/// into the room: away from the edge the view is beside, or forward when it is
/// beside neither. A held view does not move.
void Func_080e8b44(s32 direction, s32 phase, s32 amplitude) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *center = event->viewCenter;
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 x;
  s32 z;
  s32 edge;

  if (*(s32 *)map->unk0fc != 0 || event->controlMode == 3)
    return;

  if (direction & VIEW_MOVE_X) {
    edge = work->edgeX;
    if (edge == -1)
      x = work->x + Iwram_MulQ16(ABS(Trig_Sin(phase << 12)), amplitude);
    else if (edge == 1)
      x = work->x + Iwram_MulQ16(-ABS(Trig_Sin(phase << 12)), amplitude);
    else
      x = work->x + Iwram_MulQ16(ABS(Trig_Sin(phase << 12)), amplitude);
  } else {
    x = work->x;
  }

  if (direction & VIEW_MOVE_Z) {
    edge = work->edgeZ;
    if (edge == -1)
      z = work->z + Iwram_MulQ16(ABS(Trig_Sin(phase << 12)), amplitude);
    else if (edge == 1)
      z = work->z + Iwram_MulQ16(-ABS(Trig_Sin(phase << 12)), amplitude);
    else
      z = work->z + Iwram_MulQ16(ABS(Trig_Sin(phase << 12)), amplitude);
  } else {
    z = work->z;
  }

  center->x = x;
  center->y = work->y;
  center->z = z;
}
