// Draft of overlay 65f Func_02000390 and Func_020003d8, differ 16 of 34 each,
// skeleton gap 0 (tla-en, 2026-11-01)
// Only allocation is left: dst takes r4 and blue r0 where the native has dst
// r0 and blue r4. Setting dst after blue gives the right registers (differ 2)
// but moves `adds r0, #48` after `movs r4, #4`, a skeleton gap of 1. Splitting
// dst into `dst = buffers->palette; i = 0; dst += 24;` also gives them (differ
// 3), but combine then folds the load into dst and loses the native's
// `adds r0, r3, #0`.
// The loop is written with goto because the native's has no loop optimization:
// the palette address is rebuilt each pass, the table is walked by byte offsets
// and every for or do form is strength-reduced to pointers. green is a
// plain local because table + blue - 2 folds into `adds` and `subs` on the
// address; on adoption it needs checking against S2's temporary.
#include "../../../../games/tla/src/scene/garoh/garoh.h"
#include "palette_buffers.h"
#include "system/io_reg.h"

extern const u16 gData_020054ee[];
extern const u16 gData_020054b8[];

// Build BG palette 1 colors 8-15 from the table.
void Func_02000390(void) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;
  const u8 *table = (const u8 *)gData_020054ee;
  u32 i = 0;
  u16 *dst = &buffers->palette[24];
  s32 blue = 4;
  s32 red = 0;
  volatile u16 *color;
  s32 green;

loop:
  color = BG_PLTT + 24 + i;
  green = blue - 2;
  *color = *(const u16 *)(table + blue) << 10 |
           *(const u16 *)(table + green) << 5 | *(const u16 *)(table + red);
  *dst++ = *color;
  i++;
  blue += 6;
  red += 6;
  if (i < 8)
    goto loop;
}

// Build BG palette 6 colors 1-9 from the table.
void Func_020003d8(void) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;
  const u8 *table = (const u8 *)gData_020054b8;
  u32 i = 0;
  u16 *dst = &buffers->palette[97];
  s32 blue = 4;
  s32 red = 0;
  volatile u16 *color;
  s32 green;

loop:
  color = BG_PLTT + 97 + i;
  green = blue - 2;
  *color = *(const u16 *)(table + blue) << 10 |
           *(const u16 *)(table + green) << 5 | *(const u16 *)(table + red);
  *dst++ = *color;
  i++;
  blue += 6;
  red += 6;
  if (i < 9)
    goto loop;
}
