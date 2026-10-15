/* Draft: the runner for the ARM kernel at Data_081380ac, the same wrapper as
 * Func_08196958 (project_vertices.c) and UiText_LookupMessage, with two
 * arguments. Nothing in the image calls it. The kernel is 0x3cc bytes; the
 * native loads that size from its literal pool, so it is written as the
 * kernel's size symbol (Data_081380acCodeSize, a ". - Data_081380ac" set
 * after the kernel's last word, as relocated_kernel_11.s defines
 * Data_0813b9a0CodeSize); adopting needs that set in the kernel's listing
 * (recon/tla/raw/081380ae.s). */
#include "types.h"
#include "dma.h"
#include "mem.h"
#include "io_reg.h"

extern const u8 Data_081380ac[];
extern u8 Data_081380acCodeSize[];

// Copies the ARM kernel at Data_081380ac into a heap block (DMA 3, words),
// runs it there with the caller's two arguments and frees the block.
void Func_0819690c(s32 arg0, s32 arg1) {
  void (*routine)(s32, s32);
  u32 size;

  /* FAKEMATCH: the wrapper keeps the size load after the parameter copies, as
   * in UiText_LookupMessage. */
  do {
    size = (u32)Data_081380acCodeSize;
  } while (0);
  routine = (void (*)(s32, s32))MemAlloc(size);
  Dma_Set((const void *)Data_081380ac, routine, 0x84000000 | (size >> 2),
          REG_DMA3);
  routine(arg0, arg1);
  MemFree(routine);
}
