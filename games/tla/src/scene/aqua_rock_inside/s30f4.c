/// @file
/// Draws the scene's pillars: a top sprite for each active one and, when it
/// stands above its base, a column sprite as tall as its height.

#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "vram_tab.h"

struct PillarEntry {
  s32 x;
  s32 height;
  s32 z;
  s32 base;
  u16 kind;
  u16 state;
  s32 *position;
  u32 top[3];
  u32 column[3];
};

struct PillarWork {
  u16 vramEntry;
  u16 count;
  struct PillarEntry entries[32];
};

extern struct PillarWork gData_0200536c;

void Func_02003c94(u32 *sprite, s32 depth);

void Func_020030f4(void) {
  s32 *view = &Ram_HeapSlots->mapWork->viewX;
  struct PillarWork *work = &gData_0200536c;
  s32 viewX = view[0];
  struct PillarEntry *entry = work->entries;
  s32 viewZ;
  s32 originY;
  s32 depth;
  s32 i;
  u32 tile;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = Ram_HeapSlots->mapWork->origin[1];
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;

  for (i = 0; i < 32; i++) {
    if (entry->state != 0) {
      s32 *position = entry->position;
      s32 x;
      s32 y;
      s32 z;
      s32 screenY;
      s32 attributes;

      if (position != 0) {
        x = position[0] - viewX;
        y = position[1] + entry->height - originY;
        z = position[2] - viewZ - originY;
        screenY = z - y;
      } else {
        x = entry->x - viewX;
        y = entry->height - originY;
        z = entry->z - viewZ - originY;
        screenY = z - y;
      }
      if (entry->height != entry->base || entry->state == 2)
        attributes = 0x800;
      else
        attributes = 0xc00;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 8;
      depth = ((y + z) >> 16) + 50;
      if ((u32)(x + 15) <= 270) {
        s32 rise;

        if ((u32)(screenY + 47) <= 238) {
          x &= 511;
          screenY &= 255;
          rise = entry->kind * 8;
          entry->top[0] = 0;
          screenY |= x << 16;
          screenY |= 0x40002000;
          entry->top[1] = screenY;
          entry->top[2] = (tile + rise) | attributes;
          Func_02003c94(entry->top, depth);
        }
        screenY = z - y;
        screenY = (screenY >> 16) + 8;
        if ((u32)(screenY + 47) <= 238) {
          rise = (entry->height - entry->base) >> 16;
          if (rise != 0) {
            u32 *column;
            rise--;
            screenY &= 255;
            column = entry->column;
            if ((u32)rise > 7)
              rise = 7;
            rise = rise * 4 + 64;
            column[0] = 0;
            screenY |= x << 16;
            screenY |= 0x6000;
            entry->column[1] = screenY;
            entry->column[2] = attributes | (tile + rise);
            Func_02003c94(column, depth);
          }
        }
      }
    }
    entry++;
  }
}
