// The runner for the ARM kernel at Data_081380ac (relocated_kernel_0.s), the
// same wrapper as Func_08196958 (project_vertices.c) and UiText_LookupMessage,
// with two arguments. Nothing in the image calls it.
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
