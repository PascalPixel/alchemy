// Draft, differ 218 of 200, skeleton gap 24 (tla-en, 2026-11-01)
// Overlay 6a3's copy of the function drafted in resource_69d/02008e58.c
// (overlay 69d's Func_02000e58). Shape follows the matched sibling in
// games/tla/src/field/map/cell_sprite.c (view, x, sy, masks in place).
// The gap left is allocation and the moves and reloads it forces; the body's
// shape is closed. Three causes, from the diff --rtl dumps:
// - Registers. Listing: actor r5, oam r6, frame r7, viewZ r8, i r9, viewX r10,
//   depth r11, large ip; tile sp+4, viewY sp+0. r8, r10, r9, r11 is allocation
//   order, so it wants viewZ > viewX > i > depth > viewY > tile. Here refs and
//   live are viewX 5/119, viewZ 5/116, viewY 5/104, depth 4/76, i 10/208,
//   tile 3/107, which ranks i, depth, viewY, viewZ, viewX, tile.
// - gData_02005a82's address: PRE carries the top test's load to the bottom
//   test in one pseudo (reg 166) across both calls. The listing reloads it.
// - The -1 compare hoists on loop.c's second pass into a call-crossing pseudo
//   (reg 129); the listing keeps it in the body. Those two extra pseudos take
//   registers the listing gives to its locals.
// A goto loop keeps -1 in the body but loses -16 and -64 (combine folds to -15
// and -63 with blt): differ 233, gap 34. It does not stop PRE.
// Compiler lead, out of scope here, for Pascal (K3): all three fit
// optimize_size, untested. flow.c counts refs flat there, which gives viewX 4,
// viewZ 4, i 6, viewY 3, depth 2, tile 2: the listing's order but for
// viewY over depth; gcse.c runs classic GCSE and hoisting instead of PRE;
// loop.c skips move_movables, so nothing leaves the body.
// Kept because they closed gaps: y, x, z in that order (gap 26 to 24);
// sx, sy and x reuse, word's position, large as a read in place, a shared
// priority local and the depth shift split all measured same or worse.
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
    s32 x, y, z, depth, sx, sy, ox, oy, diff, large;
    s16 frame;
    u32 *word;
    u16 *timer;

    actor = gData_02005a84[i];
    if (actor == 0 || actor->x == 0)
      continue;
    y = actor->y - viewY + 0x80000;
    x = actor->x - viewX;
    z = actor->z - viewZ - viewY;
    diff = z - y;
    depth = ((y + z) >> 16) + 58;
    large = gData_02005b84;
    timer = &actor->timer;
    x >>= 16;
    sy = diff >> 16;
    if (large == 0) {
      frame = *timer;
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
      frame = *timer;
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
