// Draft of overlay 6a5's Func_02003f60, the palette cycle of overlay 69f's
// Func_020002e8 with this overlay's symbols: differ 6 of 83 in tla-en. The
// colours' address goes through p, set before the loop, so it takes r7 as the
// native's does (as colors throughout: 14). Left: the native builds that
// address (mov r7, sp) inside the loop, before the __modsi3 call, where gcse's
// PRE hoists it out of the goto loop here; and it loads the table word as
// [table, offset] where this has [offset, table], however the sum is written.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_6/jupiter_lighthouse_room_6.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020069f4;
extern s32 gData_020069f8;
extern u16 gData_02006a2c[];

s32 __modsi3();
s32 Map_GetTerrainHeight();
void Func_02001b34();
void Func_02001be4();
void Func_02001250(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_0200196c(s32 a, s32 b, s32 c);

void Func_02003f60(void) {
  u16 colors[16];
  s32 phase;
  s32 i;
  s32 offset;
  u16 *p;

  phase = (gData_020069f4 / 4) % 5;
  if (gData_020069f8 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;

    if (*(s16 *)(work + 358) != 0 && (HALF_AT(work, 358) & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  p = colors;
  i = 0;
  offset = 4;
next:
  colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02006a2c + offset);
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_020069f4++;
}
