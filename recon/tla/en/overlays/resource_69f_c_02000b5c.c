// Draft, differ 170 of 200, skeleton gap 23/21 (tla-en, 2026-11-01)
// Overlay 69f's copy of the function drafted in resource_698_c_02000aac.c,
// whose header says what it is. Reshaped here, 187 to 170, and a form the copies
// can take: the sprite position is one variable per axis (screenX, screenY) in
// 16.16 then in pixels, because the listing keeps them in r0 and r4 throughout
// (separate locals, 244); the bounds test x + 7 and x + 23 on the unshifted
// pixel, so combine folds them; timer is the address of actor->timer, computed
// before the branch as the listing does.
// Left, all allocation:
// - viewX, viewZ. The native keeps viewZ r8, viewX r10, i r9, depth r11, and
//   spills tile sp+4 and viewY sp+0, so global.c ranks viewZ, viewX, i, depth,
//   viewY. Here they rank i .144, depth .105, viewY .096, viewZ .085, viewX .083:
//   refs of 8 on viewX and viewZ would do it, and the source shows only 5.
//   Spilling tile and viewY first (viewY read first, 174) keeps the wrong high
//   registers. Spilling viewX here is also what lifts sub sp above the masks.
// - Native actor r5, oam r6, frame r7, mode ip: gData_0200252c's value outranks
//   them here (2 * 6 / 15, oam is 4 * 19 / 214), so it takes a low register and
//   frame goes to ip or r2. Reading it into a local earlier in the block: 195.
// - The count's pool load is PRE'd (a pseudo in ip or lr, reloaded after oam++);
//   the native reloads it at the continue label.
// - Timer set after the mode load gives sy r2 and timer r3 as native, but 173.
// Tried without gain: a goto loop (177), s16 types for timer, frame and mode,
// y += z, separate first and second words.
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

extern s16 gData_02002428;
extern s16 gData_0200242a;
extern struct FieldActor *gData_0200242c[];
extern struct LightOam Data_0200246c[];
extern s16 gData_0200252c;
extern s16 gData_0200252e;

s32 Func_02001bd4(struct FieldActor *actor);
void Func_02001a84(struct LightOam *oam, s32 depth);

void Func_02000b5c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_02002428].offset >> 5;
  struct LightOam *oam = Data_0200246c;
  s32 viewY = map->origin[1];
  s32 i;

  for (i = 0; i < gData_0200242a; i++) {
    struct FieldActor *actor = gData_0200242c[i];
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
    if (gData_0200252c == 0) {
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
    if (gData_0200252e == -1)
      oam->priority = Func_02001bd4(actor);
    else
      oam->priority = gData_0200252e;
    Func_02001a84(oam, depth);
    oam++;
  }
}
