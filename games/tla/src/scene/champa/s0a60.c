#include "types.h"

extern s32 gData_0200320c[];
extern s32 gData_0200b410[];
s32 GameFlag_Test(s32 flag);

extern u32 gFrameCount;

const s32 *Func_02000a60(void) {
  if (GameFlag_Test(0x97f))
    return gData_0200b410;
  return gData_0200320c;
}

void Func_02000a80(void) {
  if ((gFrameCount & 1) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x0500007c;
    index = 14;
  first:
    palette = (volatile u16 *)0x05000060;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)0x05000062 = saved;
    saved = *(volatile u16 *)0x0500009c;
    index = 14;
  second:
    palette = (volatile u16 *)0x05000080;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)0x05000082 = saved;
    saved = *(volatile u16 *)0x050000bc;
    index = 14;
  third:
    palette = (volatile u16 *)0x050000a0;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto third;
    *(volatile u16 *)0x050000a2 = saved;
  }
}
