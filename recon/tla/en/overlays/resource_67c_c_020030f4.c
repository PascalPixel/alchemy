// Measured under -Os, 5 November 2026: differ 99 (tla-en); notes below may predate it.
// Draft, differ 203 of 184, skeleton gap 49 (tla-en, 2026-11-01)
// Draws Aqua Rock's thirty-two water spouts: for each active one a top sprite
// and, when it stands above its base, a column sprite as tall as its height,
// placed from the camera and queued by depth.
// What is left comes from two values the listing computes again after the join
// and this compiler will not. entry->height is loaded above the position branch
// and again before the compare with base (ldr, cmp ip), and z - y is computed in
// each arm and again after the first call. Under -O2 gcse.c runs PRE, which
// deletes both. Lead for Pascal only (K3), compiler questions being out of scope
// here: the listing is what -Os's path in gcse.c would leave. handle_avail_expr
// keeps a redundancy that two computations reach, and hoist_code would lift the
// arms' height reads above the branch, while state, reached by one computation,
// is still removed with the two copies seen here. Not tested, and only one
// function's evidence.
// state is read twice with no local: gcse's copy into a scratch register before
// each test is the listing's adds r3, r0 and is kept.
// relative is a forced temporary inherited from an earlier round, and it costs
// the allocation. It stops gcse merging z - y but adds a pseudo the listing does
// not have: y is r8 itself, its r2 and r1 being reload registers. So base lands
// in lr, state in ip, screenY in r1, y in r4 with a caller save, depth in r9 where
// the listing has ip, r0, r4, r8 and sp+8. Without relative y and screenY do land
// right, but the second z - y goes and differ rises to 219 (gap 61).
// Tried without gain: u16 state (ldrsh); a height local with no reread (217); the
// locals at function scope or in the listing's slot order, no change; loop with
// entries[i] (extra givs, 254); z - y once after the join (225); base read at the
// compare (202 but gap 52); relative shared across the arms (219); column through
// entry; the state test as an early continue; offset read before the masks.
// A volatile height gives the reread and 200, gap 48; it is a device, not kept.
#include "types.h"
#include "call.h"
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

  for (i = 0; i < 32; i++) {
    if (entry->state != 0) {
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
        z = position[2] - viewZ - originY;
        x = position[0] - viewX;
        relative = position[1] + entry->height - originY;
        screenY = z - relative;
        y = relative;
      } else {
        s32 relative;
        relative = entry->height - originY;
        x = entry->x - viewX;
        z = entry->z - viewZ - originY;
        screenY = z - relative;
        y = relative;
      }
      if (entry->height != base || entry->state == 2)
        attributes = 0x800;
      else
        attributes = 0xc00;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 8;
      depth = ((y + z) >> 16) + 50;
      if ((u32)(x + 15) <= 270) {
        s32 rise;

        if ((u32)(screenY + 47) <= 238) {
          u32 offset;
          x &= 511;
          offset = entry->kind << 3;
          screenY &= 255;
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
