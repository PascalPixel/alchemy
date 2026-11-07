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
