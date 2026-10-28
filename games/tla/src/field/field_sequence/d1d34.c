#include "types.h"
#include "dma.h"

s32 Task_SuspendAll();
void Func_080d1ae4();

/// Suspends the Func_080d1ae4 overlay task.
void Func_080d1d34(void) { Task_SuspendAll(Func_080d1ae4); }

s32 Task_ResumeAll();

/// Resumes the Func_080d1ae4 overlay task.
void Func_080d1d44(void) { Task_ResumeAll(Func_080d1ae4); }

void *HeapSlot_Alloc();
void HeapSlot_Release();
void VramCache_Load();

/// Fills the overlay's 512 bytes of sprite tiles (VRAM cache slot 94) with
/// colour 1 when solid is set, else clears them.
void BattleEffect_FillOverlayTiles(s32 solid) {
  void *buffer = HeapSlot_Alloc(56, 0x400);

  if (solid == 0) {
    Dma_Fill32(0, buffer, 0x200);
  } else {
    Dma_Fill32(0x01010101, buffer, 0x200);
  }
  VramCache_Load(94, 0x200, buffer);
  HeapSlot_Release(56);
}
