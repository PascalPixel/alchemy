// Measured under -Os, 5 November 2026: differ 123 (tla-en); notes below may predate it.
// Draft, differ 178 of 148, skeleton gap 44 (tla-en, 2026-11-01)
// Control flow, switch, fields and arguments are the native's; almost all of
// the gap is allocation, and it has two causes this form cannot reach.
//
// 1. The level load. In the native r2 holds the jump table address, then the
// table entry, then the level, so its load comes after the entry's load and
// before the jump in the RTL, not only after sched2: r2 cannot be reused
// otherwise. With nothing left free at that load, the reload of work takes r3
// and evicts the jump target to ip (the native's mov r12), which is why level
// r2, x r1, y r0, z r4 all fit below the view values. Source cannot put a
// statement there: this form, level read under the range test, comes before
// the table pseudos, conflicts with them, and so takes r1, leaving x r0, y r4,
// z r6, and work r7 pushes map to the stack. No pass was found that moves it:
// gcse inserts at a block's end only on an abnormal edge, and a load read in
// every case stays in the cases (gap 50, 4 more instructions). Finding what
// places that load is the next step; it decides most of the 178.
//
// 2. The native loads map->origin[1] and stores it to sp[0], its whole frame,
// and nothing reads the slot. Plain C drops it: an unread local and a
// one-element array vanish. It looks like a pseudo whose use went away after
// flow, an eighth call-crossing value that lost a register. Measured on a
// scratch copy only, a cancelled term (+ origin - origin in sy) gives the load
// and store, gap 37, differ 191, with tile on the stack instead. That is a
// `dead` device, so it stays out; it also shows the store alone does not settle
// allocation.
//
// Priorities here (refs, live): work 9, 168; viewZ 5, 82; viewX 5, 85; i 7,
// 130; tile 3, 72; map 3, 90. The native's order is sprite, viewZ, viewX, map,
// work, i, tile, so map and work stand differently; the tail's level read goes
// to PRE here (reg 100, copies on two edges, adding to work's refs), where the
// native reads unk0c then level in the tail block. Casts and Data_02004030
// spellings fold back to work, except that spelling the tail or the switch's
// read gives a pool load and more instructions (tail: differ 169 but gap 51).
// Measured without gain: unk0c first in the tail, a real goto loop, tile
// declared after i, a pointer local for the test (gap 55), view fields without
// the view pointer (gap 48), reading level before the switch's range test.
#include "types.h"
#include "call.h"
#include "map.h"
#include "ram_buffer.h"
#include "vram_tab.h"

struct AnkohlRuinsFloatSprite {
  s32 x;
  s32 z;
  s32 y;
  u32 kind;
  u32 oam[3];
};

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
  struct AnkohlRuinsFloatSprite sprites[8];
};

extern struct AnkohlRuinsFloatWork Data_02004030;

s32 GameFlag_Test(s32 flag);
void Func_02002dac(u32 *oam, s32 b);

void Func_020012c4(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  volatile s32 kept = (*(s32 **)map)[1];
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  struct AnkohlRuinsFloatSprite *sprite = work->sprites;
  s32 tile = gResourceTableEntries[work->tile].offset >> 5;
  s32 i;

  if (GameFlag_Test(0x205) == 0 && work->level < 0x200000) {
    for (i = 0; i <= 7; i++) {
      u32 kind = sprite->kind;
      s32 x = sprite->x;
      s32 y = sprite->y;
      s32 z = sprite->z;
      s32 sx, sy;
      s32 level;

      level = work->level;
      switch (kind) {
      case 0:
        x -= level;
        y += level;
        if (level > 0x100000)
          x = -0x100000;
        break;
      case 1:
        x -= level;
        y += level;
        break;
      case 2:
        x += level;
        y += level;
        break;
      case 3:
        x += level;
        y += level;
        if (level > 0x100000)
          x = -0x100000;
        break;
      case 4:
        if (level >= 0x100000) {
          x = -0x100000;
          break;
        }
        y += level;
        break;
      }
      sx = ((x - viewX) >> 16) - 8;
      sy = ((z - viewZ - y) >> 16) - 8;
      if (sx >= -15 && sx <= 319 && sy >= -15 && sy <= 239) {
        sx &= 511;
        sy &= 255;
        sprite->oam[0] = 0;
        sprite->oam[1] = sy | (sx << 16) | 0x40000800;
        sprite->oam[2] = tile | 0x400;
        Func_02002dac(sprite->oam, 255);
      }
      sprite++;
    }
  }
  {
    struct MapLayer *layer = &map->layers[1];

    ((s32 *)layer->unk08)[1] = work->unk0c + work->level;
  }
}
