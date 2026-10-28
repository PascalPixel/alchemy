#include "types.h"
#include "dma.h"
#include "sequence_parent_work.h"

u8 *HeapSlot_AllocEwram();
void *HeapSlot_Alloc();
void HeapSlot_Release();
s32 VramCache_Load(s32 slot, s32 size, void *data);
void Task_Add();
s32 ObjectTable_Get();
void Func_080d1ae4();

/// Like Func_080d1a18 (the fade overlay) with blank tiles and no blending:
/// loads 0x200 zeroed tiles to VRAM slot 94 and starts Func_080d1ae4 on
/// object (the controlled owner's when 0).
void Func_080d1cc0(s32 object) {
  u8 *work = HeapSlot_AllocEwram(144, 28);
  void *buffer = HeapSlot_Alloc(56, 0x400);

  Dma_Fill32(0, buffer, 0x200);
  VramCache_Load(94, 0x200, buffer);
  Task_Add(Func_080d1ae4, 0x480);
  HeapSlot_Release(56);
  if (object == 0)
    object = ObjectTable_Get(gSequenceParentWork.currentOwner);
  *(s32 *)(work + 24) = object;
}
