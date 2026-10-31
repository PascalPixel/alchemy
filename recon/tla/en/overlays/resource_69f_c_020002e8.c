// Near miss, differ 6 of 83 (tla-en, 2026-10-31): the native keeps both loop
// invariants (mov r7, sp and the table's address) inside the loop and loads
// the table as [table, offset]; this keeps only the table load in the loop.
// It has the form of overlay 6a5's Func_02003f60, whose header says what still
// differs. The 07.gcse dump shows PRE hoisting the colours' address
// (sfp - 32) out of the goto loop in every form tried. Expansion puts the
// constant table address last in the sum, so only a table held in a pointer
// local gives [table, offset], and gcse then hoists that load too (40). Also
// measured: colours throughout or p set inside the loop 14; for (i < 5) with
// gData_020023e8[i + 2] 14 (loop.c folds the table into its giv); do-while
// with the offset 36 (loop.c hoists the table); k = (phase + i) % 5 first 8.
// A four-minute permuter run (7,337 candidates) beat none of these.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_6/jupiter_lighthouse_room_6.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020023bc;
extern s32 gData_020023c0;
extern u16 gData_020023e8[];

s32 __modsi3();
s32 Map_GetTerrainHeight();
void Func_02001b34();
void Func_02001be4();
void Func_02001250(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_0200196c(s32 a, s32 b, s32 c);

void Func_020002e8(void) {
  u16 colors[16];
  s32 phase;
  s32 i;
  s32 offset;
  u16 *p;

  phase = (gData_020023bc / 4) % 5;
  if (gData_020023c0 != 0) {
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
  colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_020023e8 + offset);
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_020023bc++;
}
