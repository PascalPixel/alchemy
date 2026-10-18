// Near miss, differ 6 of 43: the progress bar's serial callback, drawing row
// min(progress, 6) of gUi_ProgressBarRows. Left, instruction order only: the
// native loads the rows' address between the two halves of row * 3 and loads
// the destination and 0xf080 before the last shift (lsls; ldr; adds; ldr;
// ldr; lsls). 2026-10-18: the rows' address held by an empty asm that also
// reads row << 1 orders the first load but swaps the final sum's operands
// and builds the shift before the other two loads (differ 6 to 10).
#include "types.h"
#include "ram_buffer.h"
#include "mem.h"

s32 Func_080cc67c(s32);
s32 Func_080cc7c4(void);
void UiWork_Finalize(void *, s32);
extern const u8 gUi_ProgressBarRows[];

void Func_08042dc4(s32 step) {
  u8 *save = Ram_HeapSlots->serialWork;
  u32 row;
  const u8 *src;
  u16 *dst;
  s32 i;

  /* FAKEMATCH: the native issues the workspace load first (differ 8
   * without the barrier). */
  __asm__("");
  if (*(s16 *)(save + 0x3142) <= 3)
    step <<= 1;
  row = (s16)(*(u16 *)(save + 0x3140) += step);
  if (row > 6)
    row = 6;
  dst = (u16 *)0x06002252;
  src = gUi_ProgressBarRows + row * 12;
  for (i = 11; i >= 0; i--)
    *dst++ = *src++ + 0xf080;
}
