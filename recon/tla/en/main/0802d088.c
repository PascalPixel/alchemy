// Near miss, differ 2 of 210 (tla-en, 2026-10-30): one swapped pair in the
// brighten branch's BLDY push, where the native scales the queue entry's
// offset (lsls r2, r2, #2) before the tint load after the count store
// (movs r3, #0; ldrsb) and the draft loads first. sched2 ranks that
// sign-extending load above the shift (priority 11 against 9); the native
// behaves as if the load waited a cycle after the count store, which
// -mthumb-slow-loads' store rule gives only to loads read whole, never
// extended, so no source form reaches it (scheduler-order). Four minutes of
// permute on this form (6,808 candidates) found nothing below differ 2.
// The tint fields now sit after struct MapWork, embedded whole, so their
// loads may alias the queue's u16 count (MapWork holds u16 fields); that
// dependence fixed the darken branch's push (differ 4 to 2). On adoption,
// region, tint and target belong in map.h's struct MapWork from 0x973.
// Earlier: differ 4 under -mthumb-slow-loads (2026-10-26), where no single
// empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brought it closer (device search).
// Near miss, differ 10 of 210 (2026-10-18): sets the world map's region (map
// work 0x973) and steps its screen tint (0x974) toward the region's target
// (0x975) one level every 16 frames, or at once when gradual is 0, queueing
// BLDCNT (brighten 0x3f8e or darken 0x3fce) and BLDY. Region 9 tints 12,
// region 18 tints -6 until flag 0x8ef; flag 0x106 holds the tint. Six
// minutes of permute: differ 8 at best, by a block around one queue push;
// eight more minutes (2026-10-19) found the same.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "io_write_queue.h"
#include "flag_call.h"
#include "frame.h"

struct WorldTint {
  struct MapWork map;
  u8 unk970[3];
  s8 region;
  s8 tint;
  s8 target;
};

void Func_0802d088(s32 region, s32 gradual) {
  struct WorldTint *work = (struct WorldTint *)Ram_HeapSlots->mapWork;

  if (work->region != region) {
    work->region = region;
    if (region == 9)
      work->target = 12;
    else if (region == 18 && !GameFlag_Test(0x8ef))
      work->target = -6;
    else
      work->target = 0;
  }
  if (GameFlag_Test(0x106))
    return;
  if (gradual == 0) {
    work->tint = work->target;
  } else {
    region = work->target - work->tint;
    if (region == 0)
      return;
    if (gFrameCount & 15)
      return;
    if (region < 0)
      region = -1;
    else
      region = 1;
    work->tint += region;
  }
  if (work->tint > 0) {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3f8e, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, (u16)work->tint, 0x20000);
  } else {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3fce, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, (u16)-work->tint, 0x20000);
  }
}
