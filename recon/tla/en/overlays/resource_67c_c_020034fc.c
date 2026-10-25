// Draft, differ 148 of 137: draws Aqua Rock's thirty-two splashes: each active
// one as a sprite jittered a few pixels at random about its spot, with a random
// frame, queued by depth.
#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

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

extern struct Splashes gData_02005970;
extern u8 gResourceTableEntries[];

u32 Random16Far(void);
void Func_02003c94(u32 *sprite, s32 depth);

void Func_020034fc(void) {
  s32 viewX = FIELD_AT(Ram_HeapSlots->mapWork, s32, 228) & 0xffff0000;
  s32 viewZ = FIELD_AT(Ram_HeapSlots->mapWork, s32, 232) & 0xffff0000;
  s32 originY = FIELD_AT(FIELD_AT(Ram_HeapSlots->mapWork, s32 *, 0), s32, 4);
  struct Splash *splash = gData_02005970.splashes;
  u32 tile = FIELD_AT(gResourceTableEntries + (gData_02005970.resource << 2),
                      u16, 2) >>
             5;
  s32 i = 31;

loop:
  if (splash->active != 0) {
    s32 y = splash->y - originY;
    s32 z = splash->z - viewZ - originY;
    s32 screenX = splash->x - viewX;
    s32 screenY = z - y;
    s32 depth;

    screenX = (screenX >> 16) + ((Random16Far() * 10) >> 16);
    screenX -= 10;
    screenY = (screenY >> 16) + ((Random16Far() * 10) >> 16);
    depth = ((y + z) >> 16) + 50;
    screenY -= 10;
    if ((u32)(screenX + 15) <= 270 && (u32)(screenY + 37) <= 238) {
      splash->sprite[0] = 0;
      splash->sprite[1] =
          (screenY & 255) | ((screenX & 511) << 16) | 0x40002000;
      splash->sprite[2] = (tile + (((Random16Far() * 3) >> 16) << 3)) | 0x800;
      Func_02003c94(splash->sprite, depth);
    }
  }
  splash++;
  if (--i >= 0)
    goto loop;
}
