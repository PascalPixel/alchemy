// Draft, differ 225 of 200, skeleton gap 33/36 (tla-en, 2026-11-01)
// Overlay 6a5's copy of the function in resource_69d/02008e58.c (69d's
// Func_02000e58); 6a1, 6a2 and 6a4 hold it too. It draws each listed actor as
// one light sprite, sized by gData_02006b70, at a depth from y + z.
// Closed: &actor->timer in r3 ahead of the branch (the timer local, one address
// for its life), and x and the screen row carried through to the sprite
// position as one variable each. The listing's r0 and r4 are those variables.
// Left, in order of cause:
// - gcse's PRE on the loop bound. The listing loads &gData_02006a6e after the
//   continue label (all six continues land on that ldr, offset 374), once at
//   entry and once at the bottom. Here .07.gcse inserts the bottom load on edge
//   (15,16), ahead of the label, as a pseudo in lr set twice, because the copied
//   entry test makes it available round the continue paths. Any store or call
//   before a continue would stop that and the listing has none. for, while,
//   guarded do and goto loops all keep it; a bottom-entered loop has no entry
//   test. Same family as 690_1d3c and 671_53bc; classic GCSE would leave this
//   shape, which is a compiler reading (K1 to K3), out of scope here.
//   s16 *count = &gData_02006a6e keeps the load after the label but PRE then
//   carries the value (entry ldrh in lr): 243 and 36/43 on this form.
// - Allocation. The listing has actor r5, oam r6, frame r7, i r9, cameraZ r8,
//   cameraX sl, depth fp, tile sp+4, originY sp+0, gData_02006b70's value in ip.
//   That ranks cameraZ, cameraX, i, depth, originY. In the lreg dump i is
//   10 refs over 208 insns (doubled for its constant) and cameraX 5 over 119, so
//   cameraX needs 8 refs and the listing shows no further use; read from the
//   greg dump, no C here reaches it. Reading originY first spills it as the
//   listing does but moves the entry (194). The value in ip needs a rank below
//   frame's 6 refs over 39 insns; it has 6 over 11.
// Lead, not kept: with s32 sx = x >> 16, sy = height >> 16 ahead of the branch
// and x = sx - 8, height = sy - 16 in the arms, the registers of the arms fall
// in: differ 183, gap 26/29 (differ 174, 25/31 with the count local). That
// reuses x and height for a second value, a temporary device under S2, so it
// waits on a ruling. Without any reuse: differ 267, gap 34/37.
// Tried, no gain: a mode local (s32, s16, u16), frame after the position, height
// before x, depth after the shifts (245), the timer set in the arms, camera
// through MAP_WORD (201), declaration and scope orders of every local,
// s16 frame, u32 tile. const on the count local folds it. No device added.
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

extern s16 gData_02006a6c;
extern s16 gData_02006a6e;
extern struct FieldActor *gData_02006a70[];
extern struct LightOam Data_02006ab0[];
extern s16 gData_02006b70;
extern s16 gData_02006b72;

s32 Func_020059bc(struct FieldActor *actor);
void Func_0200571c(struct LightOam *oam, s32 depth);

void Func_020047d4(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 cameraX = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_02006a6c].offset >> 5;
  struct LightOam *oam = Data_02006ab0;
  s32 originY = map->origin[1];
  s32 i;

  for (i = 0; i < gData_02006a6e; i++) {
    struct FieldActor *actor = gData_02006a70[i];
    s32 x, y, z, depth, frame, screenY;
    u32 *word;
    s16 *timer;

    if (actor == 0 || actor->x == 0)
      continue;
    x = actor->x - cameraX;
    y = actor->y - originY + 0x80000;
    z = actor->z - cameraZ - originY;
    screenY = z - y;
    depth = ((y + z) >> 16) + 58;
    timer = (s16 *)&actor->timer;
    if (gData_02006b70 == 0) {
      frame = *timer;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 16;
      if (x <= -16 || x >= 320 || screenY <= -16 || screenY >= 240)
        continue;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = (screenY & 0xff) | ((x & 0x1ff) << 16) | 0x40002000;
    } else {
      frame = *timer;
      x = (x >> 16);
      screenY = (screenY >> 16);
      x -= 8;
      screenY -= 64;
      if (x <= -32 || x >= 320 || screenY <= -64 || screenY >= 176)
        continue;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = (screenY & 0xff) | ((x & 0x1ff) << 16) | 0xc000a000;
    }
    *word = (tile + frame * 8) | 0x800;
    if (gData_02006b72 == -1)
      oam->priority = Func_020059bc(actor);
    else
      oam->priority = gData_02006b72;
    Func_0200571c(oam, depth);
    oam++;
  }
}
