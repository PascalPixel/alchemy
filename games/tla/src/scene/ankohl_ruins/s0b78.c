/// @file
/// Places count effects in a row, rows times, each row two pixels further down, waiting between rows.
#include "types.h"
#include "event_inline.h"

struct AnkohlRuinsEffectParams {
  u32 unk00;
  s32 unk04;
  s32 unk08;
  s32 unk0c;
  u8 unk10[0x8];
  u16 kind;
  u8 unk1a[0xe];
};

s32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct AnkohlRuinsEffectParams *params);

void Func_02000b78(s32 rows, s32 frames, s32 x, s32 y, s32 startZ, s32 count) {
  struct AnkohlRuinsEffectParams params;
  s32 row;
  s32 left;
  s32 dx;
  s32 dy;
  s32 z;

  params.unk04 = 1;
  params.unk08 = 0xc000;
  params.unk0c = 0xc000;
  row = 0;
  if (row < rows) {
    z = startZ;
  next_row:
    for (left = 0; left < count; left++) {
      {
        s32 r = Random16Far();
        dx = 15;
        dx &= r;
      }
      dy = Random16Far() & 31;
      Func_0200015c(((x + left * 16) << 16) + ((dx - 8) << 16), (y + dy) << 16, z << 16, 0, 0,
                    0x100, 0x80001, &params);
    }
    FieldEvent_Wait(frames);
    z += 2;
    row++;
    if (row < rows)
      goto next_row;
  }
}
