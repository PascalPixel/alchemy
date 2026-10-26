// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 6 of 169 in tla-en. First difference: this issues `ldr r3, [r1, #12]` earlier
// (row 88) than the native does (row 90). No single empty asm or one-pass block
// reaches 0.
// Near miss, differ 8 of 169 (2026-10-19; 22 on 2026-10-15, written from the
// listing; ☀️ has no twin). Func_080e8c9c restores what this keeps. viewX is
// fixed in r11, which also gives minZ and maxZ the ROM's r10 and r9 (22 ->
// 8). What remains is two reordered places: the ROM builds left + 12 among
// the view centre's copies, and reloads viewZ before it starts top + 12.
// Measured with the pin: the copies before the edges (48), the margins
// folded into the edge locals (48); permute 4 min: 10 at best. Before the
// pin, viewX, minZ and maxZ took r10, r9 and r11. Measured: the two tests
// apart, each with both shakes, as the ROM branches (167 -> 157); the bounds
// copied to locals after the call (100); the pixel edges in locals before the
// tests, as the ROM computes all four first (22). viewX or viewZ as s16 (22);
// reversed declarations, assignments or edges (30-43). Permute 6 min, 2 jobs:
// 22 at best; taking viewX's address puts it on the stack (18 by its own
// count, not kept: no reason in the source).
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"
#include "map.h"
#include "party_state.h"
#include "view_move.h"

// How far the view stays from the edge it is beside, in pixels.
#define VIEW_EDGE_MARGIN 12

// A map's view bounds, in 8-pixel units.
struct MapBounds {
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
};

extern struct PartyState gPartyState;

void Func_08020328(s32 map, struct MapBounds *bounds);
void Map_SetScreenShakeFar(s32 shakeX, s32 shakeY, s32 decay);

// Opens the whole map to the view: keeps the view centre's position and the
// map's view bounds for Func_080e8c9c to restore, notes which edges the view
// is beside, and sets the bounds to the map's own. Where the view may not
// move (the view is held, or the field is in control mode 3), it shakes the
// screen along the asked direction instead.
void Func_080e89e4(s32 direction) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *center = event->viewCenter;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  register s32 viewX asm("r11") = map->viewX >> 16;
  s32 viewZ = map->viewZ >> 16;
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct MapBounds bounds;
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;

  if (*(s32 *)map->unk0fc != 0) {
    if (direction & VIEW_MOVE_X) {
      Map_SetScreenShakeFar(0x30000, 0, 0x10000);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      Map_SetScreenShakeFar(0, 0x30000, 0x10000);
      return;
    }
  }
  if (event->controlMode == 3) {
    if (direction & VIEW_MOVE_X) {
      Map_SetScreenShakeFar(0x30000, 0, 0x10000);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      Map_SetScreenShakeFar(0, 0x30000, 0x10000);
      return;
    }
  }

  Func_08020328(gPartyState.map, &bounds);
  minX = bounds.minX;
  maxX = bounds.maxX;
  minZ = bounds.minZ;
  maxZ = bounds.maxZ;
  left = minX * 8;
  right = maxX * 8;
  top = minZ * 8;
  bottom = maxZ * 8;
  work->x = center->x;
  work->y = center->y;
  work->z = center->z;

  work->edgeX = 0;
  if (viewX < left + VIEW_EDGE_MARGIN)
    work->edgeX = -1;
  if (viewX + 240 >= right - VIEW_EDGE_MARGIN)
    work->edgeX = 1;
  work->edgeZ = 0;
  if (viewZ < top + VIEW_EDGE_MARGIN)
    work->edgeZ = -1;
  if (viewZ + 160 >= bottom - VIEW_EDGE_MARGIN)
    work->edgeZ = 1;

  work->savedMinX = map->minX;
  work->savedMaxX = map->maxX;
  work->savedMinZ = map->minZ;
  work->savedMaxZ = map->maxZ;
  map->minX = minX << 19;
  map->maxX = maxX << 19;
  map->minZ = minZ << 19;
  map->maxZ = maxZ << 19;
  Object_ResetMotion(center);
}
