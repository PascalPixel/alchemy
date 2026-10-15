#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "mem.h"
#include "resource.h"
#include "callback_scheduler.h"

// The shop's work (heap slot 220), as far as these functions use it.
struct ShopRuntime {
    u8 unknown_000[0x4ca];
    s16 party_member_ids[8];
    u8 unknown_4da[0x12];
    u16 cursor_icon;
    u16 previous_page_icon;
    u16 next_page_icon;
    u16 gem_icon;
    u16 stat_up_icon;
    u16 stat_down_icon;
    u8 unknown_4f8[0xc];
    u8 party_member_count;
    u8 mode;
};

extern char ResourceId_UiIconTiles;
extern char ResourceId_ShopGemTiles;
extern char ResourceId_MenuSmallDownArrowTiles;
extern char ResourceId_MenuArrowTiles;
extern char ResourceId_ShopUpArrowTiles;
extern char ResourceId_ShopDownArrowTiles;

void Func_08108948(void *work);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
// The Spanish and Italian far-call tables reach it through the slot they name
// BattleEffect_InitializeSharedSceneFar.
void BattleEffect_InitializeSharedSceneFar(void);
#else
void EventRuntime_ResolveAllPendingActionsFar(void);
#endif
s32 Party_ListActiveOwnersFar(s16 *owners);
void Palette_CopyObjectBankToBackground14Far(void);
void VramBlock_LoadResource(s32 slot, s32 size, s32 resource);
void Func_0810bdf4(void);
void Func_0810be3c(void);
void UiWork_FinalizePendingCoreFar(void);

void Func_08108130(void)
{
    Func_08108948((u8 *)Ram_HeapSlots->menu_runtime + 0x4dc);
}

// Opens the shop's work: lists the party, loads the cursor, page arrow, gem
// and stat arrow tiles, and starts the cursor's callback. The Broken Seal's
// Shop_InitializeCursorWork.
void Func_08108148(void)
{
    struct ShopRuntime *work;
    s32 slot;

    work = Runtime_AllocateHeapBlock(220, 0xcec);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    BattleEffect_InitializeSharedSceneFar();
#else
    EventRuntime_ResolveAllPendingActionsFar();
#endif
    Dma_Fill32(0, work, 0xcec);
    work->mode = 12;
    work->party_member_count = Party_ListActiveOwnersFar(work->party_member_ids);
    Palette_CopyObjectBankToBackground14Far();
    slot = Resource_FindFreeEntry();
    work->cursor_icon = slot;
    VramBlock_LoadResource(slot, 128, (s32)&ResourceId_UiIconTiles);
    slot = Resource_FindFreeEntry();
    work->previous_page_icon = slot;
    VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopUpArrowTiles);
    slot = Resource_FindFreeEntry();
    work->next_page_icon = slot;
    VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopDownArrowTiles);
    slot = Resource_FindFreeEntry();
    work->gem_icon = slot;
    VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopGemTiles);
    slot = Resource_FindFreeEntry();
    work->stat_down_icon = slot;
    VramBlock_LoadResource(slot, 128, (s32)&ResourceId_MenuSmallDownArrowTiles);
    slot = Resource_FindFreeEntry();
    work->stat_up_icon = slot;
    VramBlock_LoadResource(slot, 128, (s32)&ResourceId_MenuArrowTiles);
    Scheduler_AddOrUpdateCallback(Func_08108130, 0x480);
    Func_0810bdf4();
}

// Closes the shop: stops the cursor's callback, frees its tiles and the work.
// The Broken Seal's Inn_Cleanup.
void Func_0810824c(void)
{
    struct ShopRuntime *state;

    state = Ram_HeapSlots->menu_runtime;
    Func_0810be3c();
    Scheduler_RemoveCallback(Func_08108130);
    UiWork_FinalizePendingCoreFar();
    Resource_ResetEntry(state->cursor_icon);
    Resource_ResetEntry(state->previous_page_icon);
    Resource_ResetEntry(state->next_page_icon);
    Resource_ResetEntry(state->gem_icon);
    Resource_ResetEntry(state->stat_up_icon);
    Resource_ResetEntry(state->stat_down_icon);
    Runtime_ReleaseHeapBlock(220);
}
