// Draft, differ 254 of 184: draws Aqua Rock's thirty-two water spouts: for each
// active one a top sprite and, when it stands above its base, a column sprite
// as tall as its height, placed from the camera and queued by depth.
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
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 originY = Ram_HeapSlots->mapWork->origin[1];
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry = work->entries;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  s32 i;

  for (i = 31; i >= 0; i--) {
    if (entry->state != 0) {
      s32 base = entry->base;
      s32 *position = entry->position;
      s32 height = entry->height;
      s32 x;
      s32 y;
      s32 z;
      s32 screenY;
      s32 attributes;
      s32 depth;

      if (position != 0) {
        x = position[0] - viewX;
        y = position[1] + height - originY;
        z = position[2] - viewZ - originY;
        screenY = z - y;
      } else {
        x = entry->x - viewX;
        y = height - originY;
        z = entry->z - viewZ - originY;
        screenY = z - y;
      }
      if (entry->height != base || entry->state == 2)
        attributes = 0x800;
      else
        attributes = 0xc00;
      depth = ((y + z) >> 16) + 50;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 8;
      if ((u32)(x + 15) <= 270) {
        u32 rise;

        if ((u32)(screenY + 47) <= 238) {
          x &= 511;
          entry->top[0] = 0;
          entry->top[1] = (screenY & 255) | (x << 16) | 0x40002000;
          entry->top[2] = (tile + (entry->kind << 3)) | attributes;
          Func_02003c94(entry->top, depth);
        }
        screenY = ((z - y) >> 16) + 8;
        if ((u32)(screenY + 47) <= 238) {
          rise = (entry->height - entry->base) >> 16;
          if (rise != 0) {
            rise--;
            if (rise > 7)
              rise = 7;
            entry->column[0] = 0;
            entry->column[1] = (screenY & 255) | (x << 16) | 0x6000;
            entry->column[2] = attributes | (tile + (rise << 2) + 64);
            Func_02003c94(entry->column, depth);
          }
        }
      }
    }
    entry++;
  }
}
