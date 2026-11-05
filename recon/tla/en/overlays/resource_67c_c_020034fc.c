// Draft, differ 16 of 137 (tla-en, 5 November), from 53: x and y subtract as they read, z in two steps; all allocation agrees.
// Left: reload's rotation at the sprite word: native tile r2, 0x800 r4, then the counter in r2; here 0x800 r2, the counter r4.
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
  struct Splash *splash;
  s32 i;
  u32 tile;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  tile = gResourceTableEntries[splashes->resource].unk2 >> 5;
  splash = splashes->splashes;
  i = 31;

  do {
    if (splash->active != 0) {
      s32 x = splash->x - viewX;
      s32 y = splash->y - originY;
      s32 z = splash->z;
      s32 screenY, rawX, rawY;
      z -= viewZ;
      z -= originY;
      screenY = z - y;
      rawX = (x >> 16) + ((Random16Far() * 10) >> 16);

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
