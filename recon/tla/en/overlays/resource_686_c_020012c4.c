// Differ 16 of 148 (tla-en, 2026-11-08, -mtune=gba).
// Draft, differ 16 of 148 (tla-en, 5 November 2026), under -Os. Left: only the first block's reload
// registers, r0 and r1 swapped for the map and the work (the native has the map's in r1, the work's in r0
// and inherits r0 for the tile's read). The native stores map->origin[1] to its one frame slot and nothing
// reads it: here a volatile local, a device to tag; a cancelled term gives the same code.
// The level is read in each case and hoisted to the jump, case 4's x = -0x100000 comes last so that case 0
// and 3 cross-jump into it, and sy's subtraction sits between x -= viewX and x's shift so that the row test's
// temporary is born after it.
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
  s32 *origin = *(s32 **)map;
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  volatile s32 kept = origin[1];
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

      switch (kind) {
      case 0:
        x -= work->level;
        y += work->level;
        if (work->level > 0x100000)
          x = -0x100000;
        break;
      case 1:
        x -= work->level;
        y += work->level;
        break;
      case 2:
        x += work->level;
        y += work->level;
        break;
      case 3:
        x += work->level;
        y += work->level;
        if (work->level > 0x100000)
          x = -0x100000;
        break;
      case 4:
        if (work->level < 0x100000)
          y += work->level;
        else
          x = -0x100000;
        break;
      }
      x -= viewX;
      sy = z - viewZ - y;
      sx = (x >> 16) - 8;
      sy = (sy >> 16) - 8;
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
