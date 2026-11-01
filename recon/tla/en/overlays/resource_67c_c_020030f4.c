// Draft, differ 204 of 184, skeleton gap 50 (tla-en, 2026-11-01)
// Draws Aqua Rock's thirty-two water spouts: for each active one a top sprite
// and, when it stands above its base, a column sprite as tall as its height,
// placed from the camera and queued by depth.
// What is left: the listing reads entry->height once before the branch on
// position and again after the join (ldr, cmp ip), and this source cannot get
// that second load: gcse removes it, whether height is a local or read in each
// branch, because the reads are the same memory. Only a volatile height keeps
// it, which is a device and is not used. Reading in each branch, as here, gives
// the lowest differ; a local read beside base and position has the listing's
// load but the same gap and keeps height live across the branches.
// With no reload, base lands in lr and state in ip, where the listing has ip and
// r0; screenY then takes r1, not r4, so y takes r4 with a caller save around
// the first call, not r8, attributes r8, not r9, and depth r9, not its stack
// slot. Getting screenY into r4 should settle the rest: a caller save of y stops
// being an option once r4 is taken. Tried: rise split in two, rise as u32, the
// declaration order, attributes as an initial value (gap rises to 33), a loop
// from labels (flow still weights it, same result).
// The per-branch temporary keeps z - y from being merged across the join, so it
// is recomputed after the call as in the listing; a copy made before the
// subtraction, or no temporary, lets gcse merge it.
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
  u32 tile;
  s32 i;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = Ram_HeapSlots->mapWork->origin[1];
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;

  for (i = 31; i >= 0; i--) {
    s32 state = entry->state;
    if (state != 0) {
      s32 base = entry->base;
      s32 *position = entry->position;
      s32 x;
      s32 y;
      s32 z;
      s32 screenY;
      s32 attributes;
      s32 depth;

      if (position != 0) {
        s32 relative;
        x = position[0] - viewX;
        relative = position[1] + entry->height - originY;
        z = position[2] - viewZ - originY;
        screenY = z - relative;
        y = relative;
      } else {
        s32 relative;
        x = entry->x - viewX;
        relative = entry->height - originY;
        z = entry->z - viewZ - originY;
        screenY = z - relative;
        y = relative;
      }
      if (entry->height != base || state == 2)
        attributes = 0x800;
      else
        attributes = 0xc00;
      depth = ((y + z) >> 16) + 50;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 8;
      if ((u32)(x + 15) <= 270) {
        s32 rise;

        if ((u32)(screenY + 47) <= 238) {
          u32 offset;
          x &= 511;
          screenY &= 255;
          offset = entry->kind << 3;
          entry->top[0] = 0;
          screenY |= x << 16;
          screenY |= 0x40002000;
          entry->top[1] = screenY;
          entry->top[2] = (tile + offset) | attributes;
          Func_02003c94(entry->top, depth);
        }
        screenY = z - y;
        screenY = (screenY >> 16) + 8;
        if ((u32)(screenY + 47) <= 238) {
          rise = entry->height;
          rise -= entry->base;
          rise >>= 16;
          if (rise != 0) {
            u32 *column = entry->column;
            s32 offset;
            rise--;
            screenY &= 255;
            if ((u32)rise > 7)
              rise = 7;
            offset = (rise << 2) + 64;
            column[0] = 0;
            screenY |= x << 16;
            screenY |= 0x6000;
            entry->column[1] = screenY;
            entry->column[2] = attributes | (tile + offset);
            Func_02003c94(column, depth);
          }
        }
      }
    }
    entry++;
  }
}
