#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "callback_scheduler.h"

void Func_08108030(void *panel);

struct ScreenWork
{
    u8 unk000[0x5a4];
    u8 panel[0x84];
};

void Func_08040c60(void)
{
    struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

    Func_08108030(work->panel);
}

void Func_08040c78(void)
{
    void *work = Runtime_AllocateBlock(208, 0x628);

    Dma_Fill32(0, work, 0x628);
    Scheduler_AddOrUpdateCallback(Func_08040c60, 1152);
}

void Func_08040cb0(void)
{
    Scheduler_RemoveCallback(Func_08040c60);
    Runtime_ReleaseHeapBlock(208);
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern u8 MsgFullPpAndMoney[];

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void UiWindow_DrawDividerLine(struct UiWindow *window, s32 x, s32 y, s32 width, s32 row);
void UiText_DrawResource(s32 message, struct UiWindow *window, s32 x, s32 y);
s32  Resource_LoadFixedBlockBIntoFreeSlot(void);
s32  RenderOutput_Create(s32 resource, s32 flags, struct UiWindow *window, s32 x, s32 y);

struct UiWindow *Func_08040cc4(void)
{
    void            *work = Ram_HeapSlots->unkd0[0];
    struct UiWindow *window = UiWindow_Create(6, 2, 18, 16, 2);
    s32              i;

    for (i = 0; i < 4; i++)
        UiWindow_DrawDividerLine(window, 0, i * 3 + 3, 17, i * 3 + 3);
    UiText_DrawResource((s32)MsgFullPpAndMoney, window, 8, 4);
    UiText_DrawResource((s32)MsgFullPpAndMoney + 1, window, 8, 28);
    UiText_DrawResource((s32)MsgFullPpAndMoney + 2, window, 8, 52);
    UiText_DrawResource((s32)MsgFullPpAndMoney + 3, window, 8, 76);
    UiText_DrawResource((s32)MsgFullPpAndMoney + 4, window, 8, 100);
    WORD_AT(work, 1444) =
        RenderOutput_Create(Resource_LoadFixedBlockBIntoFreeSlot(), 0x40000000, window, 0, 0);
    return window;
}
