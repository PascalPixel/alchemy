// Draft, differ 171 of 200, skeleton gap 23 (tla-en, 2026-11-01)
// Overlay 69a's copy of the function drafted in resource_698_c_02000aac.c, whose
// header says what it is. Gap is 20 removed, 23 added; the older form stood at
// 187 and 31/28. Gained here: the shifted screen x and y as their own variables
// (screenX, screenY), which gives the listing's `x + 7` and `x + 23` tests, and
// a pointer to the timer taken before the branch, which gives the listing's add
// above it (a local holding one address, so no S2 device).
// Left: allocation, and the bottom test. The listing has actor r5, oam r6, frame
// r7, the view z r8, view x r10, i r9, depth r11, the mode test's value in ip,
// and spills tile (sp+4) and view height (sp+0). The draft spills view x and
// tile, and holds the count's address in lr across the loop where the listing
// reloads it from the pool: PRE makes the bottom test's load redundant along the
// `continue` paths. Those orders do not follow from this compiler's priorities:
// view x and z have 5 refs over about 116 insns (.086) against i's .144, depth's
// .105 and view height's .096, and mode's .66 beats oam's .355.
// Tried without gain: view height read first (right spills, wrong prologue
// order), goto loop (the address is still kept), a set-once pointer to the count
// (hoists gData_0200239e too), a mode local in s32 and s16, the mode read as a
// const global, the tail written in both arms (no cross-jump, worse).
// Lead, not tested, and a compiler question for Pascal (K3): refs with no loop
// weight would give the listing's order of view x and z over i, and the hoisted
// add and the missing PRE fit too, all as -Os would behave. It does not yet
// explain mode or depth over view height.
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

void Func_02000b3c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_02002298].offset >> 5;
  struct LightOam *oam = gData_020022dc;
  s32 viewY = map->origin[1];
  s32 i;

  for (i = 0; i < gData_0200229a; i++) {
    struct FieldActor *actor = gData_0200229c[i];
    s32 x, y, z, sy, depth, frame, sx, ty;
    u32 *word;
    s16 *timer;

    if (actor == 0 || actor->x == 0)
      continue;
    x = actor->x - viewX;
    y = actor->y - viewY + 0x80000;
    z = actor->z - viewZ - viewY;
    sy = z - y;
    y += z;
    depth = (y >> 16) + 58;
    sx = x >> 16;
    ty = sy >> 16;
    timer = (s16 *)&actor->timer;
    if (gData_0200239c == 0) {
      frame = *timer;
      x = sx - 8;
      sy = ty - 16;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        continue;
      x &= 0x1ff;
      sy &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = sy | (x << 16) | 0x40002000;
    } else {
      frame = *timer;
      x = sx - 8;
      sy = ty - 64;
      if ((u32)(x + 31) > 350 || sy <= -64 || sy > 175)
        continue;
      x &= 0x1ff;
      sy &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = sy | (x << 16) | 0xc000a000;
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
