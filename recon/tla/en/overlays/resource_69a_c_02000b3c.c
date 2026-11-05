// Measured under -Os, 5 November 2026: differ 153 (tla-en); notes below may predate it.
// Draft, differ 171 of 200, skeleton gap 23 (tla-en, 2026-11-01)
// Overlay 69a's copy of the function drafted in resource_698_c_02000aac.c, whose
// header says what it is. Skeleton gap is 20 removed, 23 added. Re-measured on
// the current compiler: unchanged, and the draft has no devices to remove. The
// shifted screen x and y as their own variables give the listing's `x + 7` and
// `x + 23` tests; a pointer to the timer taken before the branch gives the
// listing's add above it (a local holding one address, so no S2 device).
// Left, in order of size:
// 1. The count's address. gcse's PRE treats the pool load of gData_0200229a as a
// memory expression, killed by the stores and calls of the taken path: it
// deletes the bottom test's load, copies it on the edge after the last call, and
// the head's copy lives along the `continue` paths in a pseudo with two sets.
// That pseudo crosses no call, so it takes lr, and everything after rotates. The
// listing reloads at the bottom test and uses no lr. With two sets it has no
// constant equivalence, so losing its register would give a stack slot, not a
// rematerialized load: the listing has no such pseudo at all. Measured with the
// bottom test reading a second, made-up symbol (diagnostic only, never source):
// differ 126, skeleton 16/16, no lr, so this one pseudo is worth 45. Not found:
// a plain C form that stops the PRE. It needs a store or call on every
// `continue` path or after the head's load, and the listing has neither. gcse
// has no size or setjmp exit that applies. A set-once `s16 *count` is worse: the
// PRE moves to the count's value, the same shape (162/25, with or without the
// hoist of gData_0200239e's read, which falls at loop.c's insn-count threshold).
// 2. mode, frame and timer. The listing has actor r5, oam r6, frame r7 and mode
// in ip; here mode r5, actor r6, oam r7 and frame in ip. find_reg tries the
// preferred class first, so ip means LO_REGS were full when that pseudo was
// reached: the listing allocates mode after frame. Priorities say the opposite
// (mode .8, actor .61, oam .36, frame .31), and a weighted refs of 6 cannot go
// below frame short of a live length of 40. Unexplained; a later reload spill
// of mode was considered and does not fit r5 to r7 either. The listing's timer
// add in r3 needs mode's load above it (the pool register dies there), but a
// mode local in any of six orders with the timer and shifts is worse (173 to
// 176; 158 to 160 with item 1 removed).
// 3. The spills. The listing spills tile (sp+4) and view height (sp+0) and keeps
// view x in r10 and z in r8; here view x and tile. View height's 5 refs over 104
// beat view x's 5 over 119. Slots follow declaration order, so tile is declared
// before view height, but only assignment order changes live length.
// Tried without gain: view height assigned first, second or third with its
// declaration kept last (173, 171), view height declared and read first (178),
// the mask as a second statement (179), a goto loop (the address is still kept),
// the mode read as a const global, the tail written in both arms (no cross-jump).
// Compiler question for Pascal only (K3), not tested: a PRE that leaves pool
// loads alone would close item 1 in this whole family of drafts.
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
    y = actor->y - viewY + 0x80000;
    z = actor->z - viewZ - viewY;
    x = actor->x - viewX;
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
      *word++ = sy | (x << 16) | 0x40002000;
      *word++ = 0;
    } else {
      frame = *timer;
      x = sx - 8;
      sy = ty - 64;
      if ((u32)(x + 31) > 350 || sy <= -64 || sy > 175)
        continue;
      sy &= 0xff;
      x &= 0x1ff;
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
