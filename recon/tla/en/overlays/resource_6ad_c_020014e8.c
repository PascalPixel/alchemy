// Near miss, differ 6 of 83 (tla-en, 2026-10-31): mov r7, sp outside the loop
// and the table word loaded as [offset, table].
// Draft of overlay 6ad's Func_020014e8, the palette cycle of overlay 69f's
// Func_020002e8 with this overlay's symbols. The
// colours' address goes through p, set before the loop, so it takes r7 as the
// native's does (as colors throughout: 14). Left: the native builds that
// address (mov r7, sp) inside the loop, before the __modsi3 call, where gcse's
// PRE hoists it out of the goto loop here; and it loads the table word as
// [table, offset] where this has [offset, table], however the sum is written.
// Measured 2026-10-31: p = colors inside the goto loop is hoisted by PRE just
// the same (14; and set both before and in the loop, 8). A for loop with
// gData_02007068[i + 2] puts the table first but loop.c folds it into one
// table + 4 pointer (12 to 14). The table comes first in this goto loop only
// through a local pointer, which then takes r8 (34): the native's table is a
// constant pool load after the call, base first. Permuter, 4 minutes: none.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_6/jupiter_lighthouse_room_6.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_0200703c;
extern s32 gData_02007040;
extern u16 gData_02007068[];

s32 __modsi3();
s32 Map_GetTerrainHeight();
void Func_02001b34();
void Func_02001be4();
void Func_02001250(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_0200196c(s32 a, s32 b, s32 c);

void Func_020014e8(void) {
  u16 colors[16];
  s32 phase;
  s32 i;
  s32 offset;
  u16 *p;

  phase = (gData_0200703c / 4) % 5;
  if (gData_02007040 != 0) {
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
  colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02007068 + offset);
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_0200703c++;
}
