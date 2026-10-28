/// @file
/// The sprite over the cell-table entry the leader stands on, shared by the
/// Jupiter and Mars Lighthouse overlays and overlay 6B0, each of which lists
/// the cells its map watches in a table of its own. The including file names
/// the table (CELL_TABLE), the function (CELL_SPRITE_DRAW) and its overlay's
/// import of Oam_AddSpriteAtDepth (CELL_PUSH_SLOT_ENTRY).

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "map.h"
#include "vram_tab.h"

/// One watched cell, in map cells.
struct CellTargetEntry {
  s16 x;
  s16 z;
  s16 flag; ///< set the first time the leader steps on the cell
  s16 unk06;
};

/// A map's watched cells and the sprite drawn over the current one.
struct CellTarget {
  u8 unk00[2];
  /// The first of two flag bytes that keep the leader's cell, x then z (at
  /// flagByte + 8).
  s16 flagByte;
  u8 unk04[2];
  s16 x; ///< column of the copied cell, whose row is z + 1
  s16 z;
  s16 count;   ///< entries in use
  s16 current; ///< the entry the leader stands on, or -1
  s16 shown;   ///< nonzero draws the sprite
  s16 tile;    ///< resource entry of the sprite's tiles
  u16 unk12;
  u32 oam[3]; ///< the sprite's attributes, as Oam_AddSpriteAtDepth takes them
  struct CellTargetEntry entries[1];
};

extern struct CellTarget CELL_TABLE;

struct Actor *Actor_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void CELL_PUSH_SLOT_ENTRY(void *entry, s32 priority);

/// Clears the current entry once the leader has left its cell; while the
/// leader stays on it and the sprite is shown, draws the sprite over the cell
/// at the height of the ground there, unless it is off screen.
void CELL_SPRITE_DRAW(void) {
  struct CellTarget *table = &CELL_TABLE;
  struct CellTargetEntry *entry;
  struct Actor *leader;
  struct MapWork *map;
  s32 *view;
  s32 viewX, viewZ, viewY, tile;
  s32 x, height, z, sy, depth;

  if (table->current == -1)
    return;
  /* FAKEMATCH(one-pass): the entry's address in a one-pass block, so hegSequenceParentWork's address loads before the constant that offsets it; as a
   * plain statement, 2 of 136 instructions differ in tla-en. */
  do {
    entry = &table->entries[table->current];
  } while (0);
  leader = Actor_GetById(gSequenceParentWork.currentOwner);
  if ((leader->x >> 20) != entry->x || (leader->z >> 20) != entry->z)
    table->current = -1;
  {
    s16 current = table->current;

    /* FAKEMATCH(barrier): an asm reading the current entry, which gives its
     * load as many dependents as the -1 it is compared with, so it loads
     * first; without it, 2 of 136 instructions differ in tla-en. */
    asm("" : : "r"(current));
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
