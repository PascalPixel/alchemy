// Draft, differ 273 of 200: overlay 69a's copy of the function drafted in
// resource_69d/02008e58.c (overlay 69d's Func_02000e58), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_02002298;
extern s16 gData_0200229a;
extern struct FieldActor *gData_0200229c[];
extern struct LightOam gData_020022dc[];
extern s16 gData_0200239c;
extern s16 gData_0200239e;

s32 Func_02001bb4(struct FieldActor *actor);
void Func_02001a64(struct LightOam *oam, s32 depth);

#define MAP_WORD(map, offset) (*(s32 *)((u8 *)(map) + (offset)))

void Func_02000b3c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 cameraX = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_02002298].offset >> 5;
  struct LightOam *oam = gData_020022dc;
  s32 originY = map->origin[1];
  s32 i;

  for (i = 0; i < gData_0200229a; i++) {
    struct FieldActor *actor = gData_0200229c[i];
    s32 x, y, z, depth, frame, sx, sy, ox, oy;
    u32 *word;

    if (actor == 0 || actor->x == 0)
      continue;
    x = actor->x - cameraX;
    y = actor->y - originY + 0x80000;
    z = actor->z - cameraZ - originY;
    sy = z - y;
    depth = ((y + z) >> 16) + 58;
    sx = x >> 16;
    sy >>= 16;
    if (gData_0200239c == 0) {
      frame = (s16)actor->timer;
      ox = sx - 8;
      oy = sy - 16;
      if (ox <= -16 || ox >= 320 || oy <= -16 || oy >= 240)
        continue;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = (oy & 0xff) | ((ox & 0x1ff) << 16) | 0x40002000;
    } else {
      frame = (s16)actor->timer;
      ox = sx - 8;
      oy = sy - 64;
      if (ox <= -32 || ox >= 320 || oy <= -64 || oy >= 176)
        continue;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = (oy & 0xff) | ((ox & 0x1ff) << 16) | 0xc000a000;
    }
    *word = (tile + frame * 8) | 0x800;
    if (gData_0200239e == -1)
      oam->priority = Func_02001bb4(actor);
    else
      oam->priority = gData_0200239e;
    Func_02001a64(oam, depth);
    oam++;
  }
}
