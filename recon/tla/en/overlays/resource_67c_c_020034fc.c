// Measured under -Os, 5 November 2026: differ 82 (tla-en); notes below may predate it.
// Draft, differ 95 of 137, skeleton gap 35 (tla-en, 2026-11-01)
// Draws Aqua Rock's thirty-two splashes: each active one as a sprite jittered a
// few pixels at random about its spot, with a random frame, queued by depth.
// Left: allocation, and everything else in the gap follows from it. x (6 refs,
// weighted 12) crosses 2 calls, so CALLER_SAVE_PROFITABLE (4 * calls < refs)
// puts it in r1 with stack saves; the listing has x in r10, z in r8, splash in
// r9, viewZ in r11, and originY, viewX, tile and i on the stack. That needs x at
// weighted 8 or less, which needs `x & 511` to go to its own pseudo, as the
// listing's `mov r2, r10; ands r2, r3` suggests, but regmove ties the mask's
// result to x because x dies there: a local, a second set of the local (rawY
// reused) and a copy before the mask all end the same. The listing's z looks
// like 6 refs (load, then its final value); z as a separate load gives 7 or a
// short pseudo, and both lose more than they gain. Label loop and do-while
// measure the same. frame stays a local: it gives the listing's address built
// before the third Random16Far and finished after it.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "map.h"

struct Splash {
  s32 x;
  s32 y;
  s32 z;
  u16 active;
  u16 direction;
  u32 sprite[3];
};

struct Splashes {
  u16 resource;
  u16 count;
  u8 unk04[32];
  struct Splash splashes[32];
};

struct ResourceEntry {
  u16 unk0;
  u16 unk2;
};

extern struct Splashes gData_02005970;
extern struct ResourceEntry gResourceTableEntries[];

u32 Random16Far(void);
void Func_02003c94(u32 *sprite, s32 depth);

void Func_020034fc(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  struct Splashes *splashes = &gData_02005970;
  s32 viewX = view[0];
  s32 viewZ;
  s32 originY;
  u32 tile;
  struct Splash *splash;
  s32 i;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  splash = splashes->splashes;
  tile = gResourceTableEntries[splashes->resource].unk2 >> 5;
  i = 31;

  do {
    if (splash->active != 0) {
      s32 x = splash->x - viewX;
      s32 y = splash->y - originY;
      s32 z = splash->z - viewZ - originY;
      s32 screenY = z - y;
      s32 rawX = (x >> 16) + ((Random16Far() * 10) >> 16);
      s32 rawY;

      x = rawX - 10;
      rawY = (screenY >> 16) + ((Random16Far() * 10) >> 16);
      screenY = rawY - 10;
      z = ((y + z) >> 16) + 50;

      if ((u32)(x + 15) <= 270 && (u32)(screenY + 47) <= 238) {
        u32 *frame = &splash->sprite[2];
        s32 column = x & 511;

        screenY &= 255;
        splash->sprite[0] = 0;
        splash->sprite[1] = screenY | (column << 16) | 0x40002000;
        *frame = (tile + (((Random16Far() * 3) >> 16) << 3)) | 0x800;
        Func_02003c94(splash->sprite, z);
      }
    }
    splash++;
  } while (--i >= 0);
}
