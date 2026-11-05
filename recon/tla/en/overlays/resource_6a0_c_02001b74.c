// Draft, differ 164 of 200, skeleton gap 44 (tla-en, 2026-11-01)
// Overlay 6a0's copy of the function drafted in resource_698_c_02000aac.c, whose
// header says what it is. Rewritten in overlay 69f's form, 222 to 170: the
// sprite position is one variable per axis (the listing keeps sy and top in r4),
// the bounds test x + 7 and x + 23 on the shifted pixel. Declaring tile first
// gives the listing's slot for it, 170 to 164. No devices. The compiler change
// closed nothing here. About 6,000 orders of declarations, scopes and statements
// were measured on this form; order alone stops at 162 with a wider gap.
// Left, all allocation, skeleton 23 removed, 21 added:
// - Spills. The listing keeps viewZ r8, viewX r10, i r9, depth r11 and spills tile
//   sp+4 and viewY sp+0; here viewX spills, which lifts sub sp above the masks.
//   Refs over live: viewX 5/119, viewY 5/104, i 10/208. Ranking viewX over i
//   needs about 8 weighted refs, and the source shows 5.
// - Count's address. PRE carries the head test's pool load to the bottom test in
//   a pseudo (ip here), as in overlay 69a's header; the listing reloads it there
//   and keeps ip for mode. No plain C form stops it.
// - Mode outranks actor, oam and frame (2 * 6 / 15), so actor r6, oam r7 here
//   against r5, r6. A mode local read between y and z lengthens its life: actor
//   r5, sy r4, x r1 and sy's pixel r2 then match, but mode r6 and frame ip stay
//   swapped, 155 with gap 53, 152 with gap 58. The listing's load sits after
//   depth, so that placement is not kept here; it is the lead if the gap closes.
// Tried without gain: goto loop (180), do/while with head test (170), timer read
// in each arm or dropped (165, 169), frame read once (176), mask as a second
// statement (174), x reused for the left edge (231), s16 frame and timer (same).
// Compiler question, out of scope and untested (K3): flat ref counts, as in
// overlay 6a3's header, would give this ranking and leave pool loads in place.
#include "types.h"
#include "call.h"
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
  s32 tile, viewY, viewX, viewZ;
  struct LightOam *oam;
  s32 i;

  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_020035fc].offset >> 5;
  oam = Data_02003640;
  viewY = map->origin[1];

  for (i = 0; i < gData_020035fe; i++) {
    struct FieldActor *actor = gData_02003600[i];
    s32 screenX, y, z, screenY, x, sy, depth, frame;
    u16 *timer;
    u32 *word;

    if (actor == 0 || actor->x == 0)
      continue;
    screenX = actor->x - viewX;
    y = actor->y - viewY + 0x80000;
    z = actor->z - viewZ - viewY;
    screenY = z - y;
    depth = ((y + z) >> 16) + 58;
    timer = &actor->timer;
    x = screenX >> 16;
    sy = screenY >> 16;
    if (gData_02003700 == 0) {
      frame = (s16)*timer;
      screenX = x - 8;
      screenY = sy - 16;
      if ((u32)(x + 7) > 334 || screenY <= -16 || screenY > 239)
        continue;
      screenX &= 0x1ff;
      screenY &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = screenY | (screenX << 16) | 0x40002000;
    } else {
      frame = (s16)*timer;
      screenX = x - 8;
      screenY = sy - 64;
      if ((u32)(x + 23) > 350 || screenY <= -64 || screenY > 175)
        continue;
      screenX &= 0x1ff;
      screenY &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = screenY | (screenX << 16) | 0xc000a000;
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
