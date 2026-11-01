// Draft, differ 221 of 200, skeleton gap 26 (tla-en, 2026-11-01)
// Overlay 6a3's copy of the function drafted in resource_69d/02008e58.c
// (overlay 69d's Func_02000e58). Shape follows the matched sibling in
// games/tla/src/field/map/cell_sprite.c (view, x, sy, masks in place).
// Left, from the dumps (diff --rtl):
// - Registers. Listing: actor r5, oam r6, frame r7, viewZ r8, i r9, viewX r10,
//   depth r11, large ip; tile sp+4, viewY sp+0. Draft ranks i, depth, viewY,
//   viewZ, viewX, tile, so viewX takes the stack slot. The listing needs viewX
//   and viewZ above i (priority 2*5/120 against 3*10/208), which is at least
//   three more refs each at lreg, and final code shows one use each. Not found.
//   viewY set first, or i set twice, only permutes the high registers.
// - gData_02005a82's address: PRE copies it (a pool load available from the
//   top test along every continue path), and the listing reloads it. A
//   memory-clobber asm at the top of the body stops that (0 substs, differ
//   209), and a set-once pointer local is rematerialized but PRE then takes
//   *count; neither reached 0, so both came out.
// - The -1 compare hoists on loop.c's second pass: 15 * 2 * life 4 >= 117 real
//   insns. Four more real insns in the loop, or life 3, keeps it in the body.
// - A goto loop loses -16 and -64: loop.c's single-use constant substitution
//   needs loop notes and a call, else combine folds to -15 and -63 with blt.
// Tried: 4,600 combinations of statement order, sx/sy/x reuse, a large local,
// a timer pointer (the listing's shared adds #100), word and mask order.
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

extern s16 gData_02005a80;
extern s16 gData_02005a82;
extern struct FieldActor *gData_02005a84[];
extern struct LightOam Data_02005ac4[];
extern s16 gData_02005b84;
extern s16 gData_02005b86;

s32 Func_0200499c(struct FieldActor *actor);
void Func_020046ac(struct LightOam *oam, s32 depth);

void Func_0200377c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_02005a80].offset >> 5;
  struct LightOam *oam = Data_02005ac4;
  s32 viewY = map->origin[1];
  s32 i;

  for (i = 0; i < gData_02005a82; i++) {
    struct FieldActor *actor;
    s32 x, y, z, depth, frame, sx, sy, ox, oy, diff, large;
    u32 *word;
    u16 *timer;

    actor = gData_02005a84[i];
    if (actor == 0 || actor->x == 0)
      continue;
    x = actor->x - viewX;
    y = actor->y - viewY + 0x80000;
    z = actor->z - viewZ - viewY;
    diff = z - y;
    depth = ((y + z) >> 16) + 58;
    large = gData_02005b84;
    timer = &actor->timer;
    x >>= 16;
    sy = diff >> 16;
    if (large == 0) {
      frame = (s16)*timer;
      ox = x - 8;
      oy = sy - 16;
      if (ox <= -16 || ox >= 320 || oy <= -16 || oy >= 240)
        continue;
      word = (u32 *)oam;
      ox &= 0x1ff;
      oy &= 0xff;
      *word++ = 0;
      *word++ = oy | (ox << 16) | 0x40002000;
    } else {
      frame = (s16)*timer;
      ox = x - 8;
      oy = sy - 64;
      if (ox <= -32 || ox >= 320 || oy <= -64 || oy >= 176)
        continue;
      word = (u32 *)oam;
      ox &= 0x1ff;
      oy &= 0xff;
      *word++ = 0;
      *word++ = oy | (ox << 16) | 0xc000a000;
    }
    *word = (tile + frame * 8) | 0x800;
    if (gData_02005b86 == -1)
      oam->priority = Func_0200499c(actor);
    else
      oam->priority = gData_02005b86;
    Func_020046ac(oam, depth);
    oam++;
  }
}
