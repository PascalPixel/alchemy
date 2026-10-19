// Draft, differ 4 of 126 (2026-10-18): the block is heap slot 72, and the
// cursors of rows 7 and 8 are cleared inline. Left: in the bit-field updates
// at the end the native loads byte 5 before it builds -13, and byte 7 before
// it ORs in 32 (the lane's load-earlier near misses).
#include "resource.h"
#include "mem.h"
#include "types.h"

struct GlyphCursor {
  u16 unused;
  u16 cursor;
};

struct GlyphRow {
  u8 unk00[8];
  struct GlyphCursor index;
  u8 unk0c[6];
  u16 state;
  u8 unk14[32];
};

struct GlyphVisual {
  u8 unk00[5];
  u8 flags05_0 : 2;
  u8 flags05_2 : 2;
  u8 flags05_4 : 1;
  u8 flags05_5 : 1;
  u8 flags05_6 : 2;
  u8 unk06;
  u8 flags07_0 : 1;
  u8 flags07_1 : 5;
  u8 flags07_6 : 2;
  u8 unk08;
  u8 flags09_0 : 2;
  u8 flags09_2 : 2;
  u8 flags09_4 : 4;
  u8 unk0a[2];
};

struct GlyphWork {
  struct GlyphRow rows[14];
  u8 unk2d8[10];
  u16 field2e2;
  u16 slot;
  u16 tile;
  u8 unk2e8[18];
  u16 field2fa;
  u8 unk2fc[4];
  struct GlyphVisual visual;
  u8 unk30c[10];
  u16 field316;
  u8 unk318[48];
  s32 field348;
  s32 field34c;
  s32 field350;
  u8 unk354[64];
  u16 field394;
  u8 unk396[4];
  u16 field39a;
  u16 field39c;
  u16 field39e;
  u16 field3a0;
  u8 unk3a2[22];
  u16 field3b8;
  u8 unk3ba[42];
};

extern const u8 gData_080346f8[];

void UiGlyph_ResetWorkState(void) {
  struct GlyphWork *work;
  struct GlyphVisual *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  work = Runtime_AllocateBlock(72, 996);
  work->field348 = clear;
  work->field34c = clear;
  work->field350 = clear;
  work->field39a = clear;
  work->field39c = clear;
  work->field39e = 128;
  work->field3a0 = 32;
  work->field394 = clear;
  work->field3b8 = 999;
  i = 0;
  do {
    work->rows[i + 2].index.cursor = 0;
    work->rows[i + 9].index.cursor = 0;
    i++;
  } while (i != 5);
  {
    struct GlyphCursor *cursor = &work->rows[7].index;

    cursor->cursor = 0;
    cursor = &work->rows[8].index;
    cursor->cursor = 0;
  }
  work->rows[0].index.cursor = 0;
  work->rows[1].index.cursor = 0;
  work->rows[0].state = 0;
  work->rows[1].state = 0;
  table = gData_080346f8;
  work->slot = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->slot, 256, table);
  work->field2e2 = 0;
  work->field2fa = 0;
  work->field316 = 0;
  visual = &work->visual;
  visual->flags05_2 = 0;
  visual->flags05_4 = 0;
  visual->flags05_5 = 1;
  visual->flags05_0 = 0;
  visual->flags07_1 = 0;
  visual->flags07_6 = 1;
  visual->flags05_6 = 0;
  visual->flags09_2 = 0;
}
