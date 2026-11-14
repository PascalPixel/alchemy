/// @file
/// The sprite over the cell-table entry the leader stands on, shared by the
/// Jupiter and Mars Lighthouse rooms and the Anemos Inner Sanctum, each of
/// which lists the cells its map watches in its own gCrackedTiles. The
/// including file names its overlay's import of Oam_AddSpriteAtDepth
/// (CELL_PUSH_SLOT_ENTRY).

#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "field/cell_target.h"

extern struct CellTarget gCrackedTiles;

struct Actor *Actor_GetById(u32 objectId);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void CELL_PUSH_SLOT_ENTRY(void *entry, s32 priority);

/// Clears the current entry once the leader has left its cell; while the
/// leader stays on it and the sprite is shown, draws the sprite over the cell
/// at the height of the ground there, unless it is off screen.
void CrackedTiles_Update(void) {
  struct CellTarget *table = &gCrackedTiles;
  struct CellTargetEntry *entry;
  struct Actor *leader;
  struct MapWork *map;
  s32 *view;
  s32 viewX, viewZ, viewY, tile;
  s32 x, height, z, sy, depth;

  if (table->current == -1)
    return;
  entry = &table->entries[table->current];
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if ((leader->x >> 20) != entry->x || (leader->z >> 20) != entry->z)
    table->current = -1;
  {
    s16 current = table->current;

    if (current == -1)
      return;
  }
  if (table->shown == 0)
    return;
  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  viewY = map->origin[1];
  tile = gResourceTableEntries[table->tile].offset >> 5;
  x = (entry->x << 20) - viewX;
  height = Map_GetTerrainHeight(0, entry->x << 20, entry->z << 20) - viewY;
  z = (entry->z << 20) - viewZ - viewY;
  sy = z - height;
  x >>= 16;
  height += z;
  height >>= 16;
  sy >>= 16;
  sy += 14;
  depth = height + 58;
  if ((u32)(x + 15) > 334 || sy < -15 || sy > 239)
    return;
  x &= 0x1ff;
  sy &= 0xff;
  table->oam[0] = 0;
  table->oam[1] = sy | (x << 16) | 0x80008800;
  table->oam[2] = tile | 0x400;
  CELL_PUSH_SLOT_ENTRY(table->oam, depth);
}
