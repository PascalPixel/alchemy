/// @file
/// The runner for the ARM kernel at Data_081380ac (relocated_kernel_0.s), the
/// same wrapper as Render_TransformVertices (project_vertices.c) and Message_Find,
/// with two arguments. Nothing in the image calls it.
#include "types.h"
#include "dma.h"
#include "heap_release.h"
#include "io_reg.h"

extern const u8 Data_081380ac[];
extern u8 Data_081380acCodeSize[];

/// Copies the ARM kernel at Data_081380ac into a heap block (DMA 3, words),
/// runs it there with the caller's two arguments and frees the block.
void RamCode_RunKernel0(s32 arg0, s32 arg1) {
  void (*routine)(s32, s32);
  u32 size;

  /* FAKEMATCH: the wrapper keeps the size load after the parameter copies, as
   * in Message_Find. */
  do {
    size = (u32)Data_081380acCodeSize;
  } while (0);
  routine = (void (*)(s32, s32))Heap_Alloc(size);
  Dma_Set((const void *)Data_081380ac, routine, DMA_COPY_WORDS(size >> 2),
          REG_DMA3);
  routine(arg0, arg1);
  Heap_Release(routine);
}
