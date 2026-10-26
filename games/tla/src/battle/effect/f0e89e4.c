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
