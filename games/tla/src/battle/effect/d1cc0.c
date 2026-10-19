#include "types.h"
#include "dma.h"
#include "party_state.h"

u8 *Runtime_AllocateBlock();
void *Runtime_AllocateHeapBlock();
void Runtime_ReleaseHeapBlock();
s32 VramCache_Load(s32 slot, s32 size, void *data);
void Scheduler_AddOrUpdateCallback();
s32 ObjectTable_Get();
void Func_080d1ae4();

/// Like Func_080d1a18 (the fade overlay) with blank tiles and no blending:
/// loads 0x200 zeroed tiles to VRAM slot 94 and starts Func_080d1ae4 on
/// object (the controlled owner's when 0).
void Func_080d1cc0(s32 object) {
  u8 *work = Runtime_AllocateBlock(144, 28);
  void *buffer = Runtime_AllocateHeapBlock(56, 0x400);

  Dma_Fill32(0, buffer, 0x200);
  VramCache_Load(94, 0x200, buffer);
  Scheduler_AddOrUpdateCallback(Func_080d1ae4, 0x480);
  Runtime_ReleaseHeapBlock(56);
  if (object == 0)
    object = ObjectTable_Get(gPartyState.currentOwner);
  *(s32 *)(work + 24) = object;
}
