// Draft, differ 222 of 200, skeleton gap 38 (tla-en, 2026-11-01)
// Overlay 6a0's copy of the function drafted in resource_698_c_02000aac.c, whose
// header says what it is. Left: the skeleton's remainder is allocation, not
// shape. The native keeps view x and z in r10 and r8, the index in r9 and the
// depth in r11 and spills view height and tile; this spills view x and tile,
// which moves the sp-relative loads, the hi-register moves and sub sp. The
// native's flag sits in ip and serves as the 0 stored in the first branch;
// here the first branch stores a literal 0. The shape comes from the listing:
// shifted x and y are new variables beside the raw ones (asrs r1, r0), left
// and top are new again (x + 7 and x + 23 fold from left + 15 and left + 31),
// and &actor->timer is taken before the branch. Tried: about 5,700 orders of
// declarations, scopes and statements; declaration order is why the locals
// sit in this order. View x and z masked in a second step, view height read
// through a local, and a separate null test gain nothing.
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
  struct MapWork *map;
  s32 *view;
  s32 i;
  s32 tile;
  s32 viewX;
  struct LightOam *oam;
  s32 viewZ;
  s32 viewY;
  s32 sy;
  s32 depth;
  s32 z;
  s32 flag;
  s32 left;
  s32 frame;
  s32 x;
  s16 *timer;
  s32 px;
  s32 y;
  s32 top;
  s32 py;
  struct FieldActor *actor;
  u32 *word;
  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_020035fc].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02003640;
  for (i = 0; i < gData_020035fe; i++) {
    actor = gData_02003600[i];
    if (actor == 0 || actor->x == 0)
      continue;
    x = actor->x - viewX;
    y = actor->y - viewY + 0x80000;
    z = actor->z - viewZ - viewY;
    sy = z - y;
    depth = ((y + z) >> 16) + 58;
    px = x >> 16;
    flag = gData_02003700;
    timer = &actor->timer;
    py = sy >> 16;
    if (flag == 0) {
      frame = *timer;
      left = px - 8;
      top = py - 16;
      if ((u32)(left + 15) > 334 || top <= -16 || top > 239)
        continue;
      left &= 0x1ff;
      top &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = top | (left << 16) | 0x40002000;
    } else {
      frame = *timer;
      left = px - 8;
      top = py - 64;
      if ((u32)(left + 31) > 350 || top <= -64 || top > 175)
        continue;
      left &= 0x1ff;
      top &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = top | (left << 16) | 0xc000a000;
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
