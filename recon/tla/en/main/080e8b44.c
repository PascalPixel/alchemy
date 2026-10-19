// Near miss, differ 4 of 155 (2026-10-18; 8 on 2026-10-15, written from the
// listing; ☀️ has no twin). The sine is taken twice in each arm, as an ABS
// macro that reads its argument twice gives. Each edge is read into a local
// behind an empty volatile asm, so it loads before the -1 (8 -> 4; the local
// alone 8). What remains is one pair: the ROM loads the event work before it
// copies the direction into r11 and starts the centre's offset (adds r7, r1;
// ldr r1, [r2, #108]; mov fp, r0; movs r0, #230), where agscc ranks the copy
// and the phase's copy equal and takes the direction's first. Measured: a
// switch on the edge (29); the direction copied to a local behind a volatile
// asm reading the event work (59); an asm "+r" on the direction reading the
// event work (28; with the direction fixed in r11, 6); an asm "+l" on the
// event work after its load (4, the phase's copy then moves down); on
// 2026-10-19 an asm on the direction as "+h" reading the event work after
// its load (28), and permute 4 min found nothing better.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "event_work.h"
#include "object_runtime.h"
#include "map.h"
#include "fixed_math.h"
#include "view_move.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x))

// Swings the view centre away from where Func_080e89e4 found it, along the
// asked directions, by amplitude times the sine of the phase. The swing goes
// into the room: away from the edge the view is beside, or forward when it is
// beside neither. A held view does not move.
void Func_080e8b44(s32 direction, s32 phase, s32 amplitude) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *center = event->viewCenter;
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 x;
  s32 z;
  s32 edge;

  if (*(s32 *)map->unk0fc != 0 || event->controlMode == 3)
    return;

  if (direction & VIEW_MOVE_X) {
    edge = work->edgeX;
    /* FAKEMATCH: ⚓️ loads the edge before it builds the -1 it compares with;
     * plainly the -1 comes first (differ 2 for each edge). */
    asm volatile("" : : "l"(edge));
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
    /* FAKEMATCH: as for edgeX above. */
    asm volatile("" : : "l"(edge));
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
