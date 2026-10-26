// Draft, differ 187 of 200: overlay 6a0's copy of the function drafted in
// resource_698_c_02000aac.c (overlay 698's Func_02000aac, differ 187 there), whose
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

extern s16 gData_020035fc;
extern s16 gData_020035fe;
extern struct FieldActor *gData_02003600[];
extern struct LightOam Data_02003640[];
extern s16 gData_02003700;
extern s16 gData_02003702;

s32 Func_02002c2c(struct FieldActor *actor);
void Func_02002aa4(struct LightOam *oam, s32 depth);

void Func_02001b74(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_020035fc].offset >> 5;
  struct LightOam *oam = Data_02003640;
  s32 viewY = map->origin[1];
  s32 i;

  for (i = 0; i < gData_020035fe; i++) {
    struct FieldActor *actor = gData_02003600[i];
    s32 x, y, z, sy, depth, frame;
    u32 *word;

    if (actor == 0 || actor->x == 0)
      continue;
    x = actor->x - viewX;
    y = actor->y - viewY + 0x80000;
    z = actor->z - viewZ - viewY;
    sy = z - y;
    y += z;
    depth = (y >> 16) + 58;
    x >>= 16;
    sy >>= 16;
    if (gData_02003700 == 0) {
      frame = (s16)actor->timer;
      x -= 8;
      sy -= 16;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        continue;
      x &= 0x1ff;
      sy &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = sy | (x << 16) | 0x40002000;
    } else {
      frame = (s16)actor->timer;
      x -= 8;
      sy -= 64;
      if ((u32)(x + 31) > 350 || sy <= -64 || sy > 175)
        continue;
      x &= 0x1ff;
      sy &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = sy | (x << 16) | 0xc000a000;
    }
    *word = (tile + frame * 8) | 0x800;
    if (gData_02003702 == -1)
      oam->priority = Func_02002c2c(actor);
    else
      oam->priority = gData_02003702;
    Func_02002aa4(oam, depth);
    oam++;
  }
}
