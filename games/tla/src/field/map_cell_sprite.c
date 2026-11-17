/// @file
/// The hole mask over the cracked tile the leader stands on, shared by the
/// Jupiter and Mars Lighthouse rooms and the Anemos Inner Sanctum, each of
/// which lists the cells its map watches in its own gCrackedTiles. While he
/// drops through the tile (CrackedTile_Fall), the mask is drawn into the OBJ
/// window, where no sprite shows, so that he sinks out of sight through the
/// hole. The including file names its overlay's import of Oam_AddSpriteAtDepth
/// (CELL_PUSH_SLOT_ENTRY).

#include "types.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "field/cell_target.h"

/// The mask, 16 by 32, hangs from HOLE_MASK_TOP pixels down its cell.
#define HOLE_MASK_TOP 14
/// The bits of a sprite's x and y in its OAM attributes.
#define OBJ_X_MASK 0x1ff
#define OBJ_Y_MASK 0xff

extern struct CellTarget gCrackedTiles;

struct Actor *Actor_GetById(u32 objectId);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void CELL_PUSH_SLOT_ENTRY(void *entry, s32 priority);

/// Clears the current entry once the leader has left its cell; while the
/// leader stays on it and the mask is shown, draws the mask over the cell at
/// the height of the ground there, unless it is off screen.
void CrackedTile_Update(void) {
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
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if ((FIELD_CELL(leader->x.fixed)) != entry->x ||
      (FIELD_CELL(leader->z.fixed)) != entry->z)
    table->current = -1;
  {
    s16 current = table->current;

    if (current == -1)
      return;
  }
  if (table->shown == 0)
    return;
  map = Ram_HeapSlots->mapGrid;
  view = &map->viewX;
  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  viewY = map->origin[1];
  tile = gVramCacheEntries[table->tile].offset >> 5;
  x = FIELD_CELL_ORIGIN(entry->x) - viewX;
  height = Map_GetTerrainHeight(0, FIELD_CELL_ORIGIN(entry->x),
                                FIELD_CELL_ORIGIN(entry->z)) -
           viewY;
  z = FIELD_CELL_ORIGIN(entry->z) - viewZ - viewY;
  sy = z - height;
  x = FIXEDtoINT(x);
  height += z;
  height = FIXEDtoINT(height);
  sy = FIXEDtoINT(sy);
  sy += HOLE_MASK_TOP;
  depth = height + 58;
  if ((u32)(x + MAPGRID_CELL_PIXELS - 1) > 334 ||
      sy < -(MAPGRID_CELL_PIXELS - 1) || sy > 239)
    return;
  x &= OBJ_X_MASK;
  sy &= OBJ_Y_MASK;
  table->oam[0] = 0;
  table->oam[1] = sy | OBJ_X(x) | OBJ_SIZE_16X32 | OBJ_MODE(OBJ_MODE_WINDOW);
  table->oam[2] = tile | OBJ_PRIORITY(1);
  CELL_PUSH_SLOT_ENTRY(table->oam, depth);
}

#undef HOLE_MASK_TOP
#undef OBJ_X_MASK
#undef OBJ_Y_MASK
