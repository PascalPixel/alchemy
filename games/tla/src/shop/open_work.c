#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "heap_release.h"
#include "resource.h"
#include "task.h"
#include "edition.h"

/// The shop's work (heap slot 220), as far as these functions use it.
struct ShopRuntime {
  u8 unk000[0x4ca];
  s16 partyMemberIds[8];
  u8 unk4da[0x12];
  u16 cursorIcon;
  u16 previousPageIcon;
  u16 nextPageIcon;
  u16 gemIcon;
  u16 statUpIcon;
  u16 statDownIcon;
  u8 unk4f8[0xc];
  u8 partyMemberCount;
  u8 mode;
};

extern char ResourceId_UiIconTiles;
extern char ResourceId_ShopGemTiles;
extern char ResourceId_MenuSmallDownArrowTiles;
extern char ResourceId_MenuArrowTiles;
extern char ResourceId_ShopUpArrowTiles;
extern char ResourceId_ShopDownArrowTiles;

void ShopCursor_Advance(void *work);
void Event_ResolveAllPendingActionsFar(void);
s32 Party_ListActiveOwnersFar(s16 *owners);
void Palette_CopyObjectBankToBackground14Far(void);
void VramBlock_LoadResource(s32 slot, s32 size, s32 resource);
void Func_0810bdf4(void);
void Func_0810be3c(void);
void Message_CloseFinishedWindowsFar(void);

void Func_08108130(void) {
  ShopCursor_Advance((u8 *)Ram_HeapSlots->menuRuntime + 0x4dc);
}

/// Opens the shop's work: lists the party, loads the cursor, page arrow, gem
/// and stat arrow tiles, and starts the cursor's callback.
// The Broken Seal's Shop_InitializeCursorWork.
void Func_08108148(void) {
  struct ShopRuntime *work;
  s32 slot;

  work = HeapSlot_Alloc(220, 0xcec);
  Event_ResolveAllPendingActionsFar();
  Dma_Fill32(0, work, 0xcec);
  work->mode = 12;
  work->partyMemberCount = Party_ListActiveOwnersFar(work->partyMemberIds);
  Palette_CopyObjectBankToBackground14Far();
  slot = Resource_FindFreeEntry();
  work->cursorIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_UiIconTiles);
  slot = Resource_FindFreeEntry();
  work->previousPageIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopUpArrowTiles);
  slot = Resource_FindFreeEntry();
  work->nextPageIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopDownArrowTiles);
  slot = Resource_FindFreeEntry();
  work->gemIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopGemTiles);
  slot = Resource_FindFreeEntry();
  work->statDownIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_MenuSmallDownArrowTiles);
  slot = Resource_FindFreeEntry();
  work->statUpIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_MenuArrowTiles);
  Task_Add(Func_08108130, TASK_PASS_VBLANK);
  Func_0810bdf4();
}

/// Closes the shop: stops the cursor's callback, frees its tiles and the work.
// The Broken Seal's Shop_ReleaseCursorWork.
void Func_0810824c(void) {
  struct ShopRuntime *state;

  state = Ram_HeapSlots->menuRuntime;
  Func_0810be3c();
  Task_Remove(Func_08108130);
  Message_CloseFinishedWindowsFar();
  Resource_ResetEntry(state->cursorIcon);
  Resource_ResetEntry(state->previousPageIcon);
  Resource_ResetEntry(state->nextPageIcon);
  Resource_ResetEntry(state->gemIcon);
  Resource_ResetEntry(state->statUpIcon);
  Resource_ResetEntry(state->statDownIcon);
  HeapSlot_Release(220);
}
