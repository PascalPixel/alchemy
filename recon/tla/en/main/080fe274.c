/* Near miss, differ 4 of 216: the twin of The Broken Seal's ActionMenu_Open.
   Two pairs are in the other order: the native stores the result's -1 to its stack
   slot before it starts the heap slots' address (str r2, [sp, #8]; movs r3, #192),
   and loads the page's zero before it builds the page's offset (ldr r3, [pc];
   movs r2, #135). Tried: a volatile empty asm after the -1 (it also pulls the tiles
   copy forward, differ 16); a halfword struct temporary with a volatile operand
   barrier for the zero (the last menu_busy store then takes movs instead of the
   pool, differ 20); a volatile result (the registers shift, differ 97). */
#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "animspr.h"
#include "window.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "disptran.h"

struct OwnerMenuState {
    u8 unknown_000[0x28];
    void *status_window;
    u8 unknown_02c[0xc4];
    void *selector_window;
    u8 unknown_0f4[0x34];
    u16 owner_y[4];
    u8 unknown_130[0xd4];
    s16 owner_ids[8];
    u8 unknown_214;
    u8 party_count;
    u8 unknown_216[6];
    u16 page;
};

void UiWindow_DrawFrameFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void WaitFrames(s32 frames);
void UiWindow_InitializeWork(s32 unused);
s32 Party_ListActiveOwnersFar(s16 *owners);
void Func_080fee04(s32 a, s32 b, s32 c, s32 d);
void *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 Func_080fe49c(void);
void RenderOutput_ClearListFar(void *window);
void ItemMenu_Close(void);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void MemFree(void *block);
void Scheduler_EnableOverlayCallbacksWithFlags(void);
void Scheduler_DisableOverlayCallbacksWithFlags(void);
void Palette_LightenBankHighlight(s32 bank);
void UiWork_SetAltFlagAndClearTableFar(s32 flag);
s32 Party_SumDjinnCountsFar(s32 owner);
void Func_081051a8(void);
void Func_081053a8(void);
void Link_DrawShiftedTilePairFar(s32 address);
void Menu_CancelSoundReset(void);
void Menu_EnsureCancelSound(void);
void MenuIcon_ReleaseSlots(void);
void MenuIcon_ReleaseObjects(void);
void Func_08038290(void);

static __inline__ void CopyWords_(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does: called directly, CSE keeps 0x05000000 in a saved register across the calls. */
    Iwram_CopyWords(destination, source, size);
}

s32 Func_080fe274(void)
{
    struct OwnerMenuState *state = (struct OwnerMenuState *)Runtime_AllocateHeapBlock(220, 0x0cec);
    void *palette = Runtime_BumpAllocateAlternatePool(64);
    void *tiles = Runtime_BumpAllocateAlternatePool(0x2800);
    s32 result = -1;
    struct HeapSlots *heap;
    struct DisplayTransitionState *transition;
    u32 saved;
    u16 *page;

    heap = Ram_HeapSlots;
    transition = heap->display_transition;
    heap->object_system->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    saved = *(u16 *)&transition->active;
    transition->active = 17;
    Scheduler_EnableOverlayCallbacksWithFlags();
    UiWindow_InitializeWork(0);
    page = &state->page;
    *page = 0;
    state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
    Func_080fee04(0, 3, 0, 7);
    CopyWords_(palette, (void *)0x05000000, 64);
    CopyWords_(tiles, (void *)0x06004000, 0x2800);
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
    UiWork_SetAltFlagAndClearTableFar(1);
    state->selector_window = UiWindow_CreateFar(13, 0, 17, 5, 2);
    if (Party_SumDjinnCountsFar(result) != 0)
        Func_081051a8();
    Link_DrawShiftedTilePairFar(0x06002500);
    Func_081053a8();
    Menu_CancelSoundReset();
    *page = 0;

    result = Func_080fe49c();

    Menu_EnsureCancelSound();
    MenuIcon_ReleaseSlots();
    RenderOutput_ClearListFar(state->status_window);
    MenuIcon_ReleaseObjects();
    Scheduler_DisableOverlayCallbacksWithFlags();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    Func_08038290();
    UiWork_SetAltFlagAndClearTableFar(0);
    CopyWords_((void *)0x05000000, palette, 64);
    CopyWords_((void *)0x06004000, tiles, 0x2800);
    MemFree(tiles);
    MemFree(palette);
    ((struct UiRenderWork *)heap->window_tiles)->menu_busy = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(220);
    heap->object_system->suspended = 0;
    transition->active = saved;
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    ((struct UiRenderWork *)heap->window_tiles)->menu_busy = 0;
    return result;
}
