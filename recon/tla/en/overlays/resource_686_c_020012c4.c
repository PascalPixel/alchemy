// Draft, differ 178 of 148, skeleton gap 44 (tla-en, 2026-11-01)
// Control flow, switch, fields and arguments are the native's. What is left
// follows from allocation and one store this form does not make.
// The native loads map->origin[1] and stores it to sp[0], its whole 4-byte
// frame, and nothing reads the slot. Plain C sprites it: an unread local and a
// one-element array both vanish. The likely source is a local used only in a
// comparison that goes away after reload; that, or a volatile or stack-forced
// local, is a dead-code device, so it is left for a round that allows one. A
// volatile local alone brings the skeleton only 1 closer.
// Native registers: viewX r7, viewZ r6, sprite r5, z r4, map r8, work r10, i r9,
// tile r11, x r1, y r0, level r2. Here work outranks viewX, viewZ and map and
// takes r7, map goes to the stack, and with work low no reload evicts the jump
// target to ip (the native's mov r12), level takes r1 and the constants split
// across r2 and r3, so cross-jumping cannot merge the tails of cases 0 and 3
// and 511 is built from 334. Reading the level before the switch under the
// range test gives the native's load before the table jump. Measured without
// gain: the address spelled Data_02004030 in any or all places (worse when the
// loop or the tail reads it), block scope against function scope, tile as u32
// or u16, view values or i as u32, i as s16, a goto into a shared tail of cases
// 0 and 3 (removed 25, added 20), and the limit in a local.
#include "types.h"
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
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  struct AnkohlRuinsFloatSprite *sprite = work->sprites;
  s32 tile = gResourceTableEntries[work->tile].offset >> 5;
  s32 i;
  s32 *layer;

  if (GameFlag_Test(0x205) == 0 && work->level < 0x200000) {
    for (i = 0; i <= 7; i++) {
      u32 kind = sprite->kind;
      s32 x = sprite->x;
      s32 y = sprite->y;
      s32 z = sprite->z;
      s32 sx, sy;
      s32 level;

      if ((u32)kind <= 4) {
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
          if (level < 0x100000) {
            y += level;
            break;
          }
          x = -0x100000;
          break;
        }
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
  layer = (s32 *)&map->layers[1];
  layer[3] = work->level + work->unk0c;
}
