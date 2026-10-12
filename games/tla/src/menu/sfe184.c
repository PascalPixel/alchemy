#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "animspr.h"

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
void WaitFrames(s32 frames);
void UiWindow_InitializeWork(s32 unused);
s32 Party_ListActiveOwnersFar(s16 *owners);
void Func_080fee04(s32 a, s32 b, s32 c, s32 d);
void *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 Func_080fe240(void);
void RenderOutput_ClearListFar(void *window);
void ItemMenu_Close(void);

s32 Func_080fe184(void)
{
    struct OwnerMenuState *state = (struct OwnerMenuState *)Runtime_AllocateHeapBlock(220, 0x0cec);
    s32 result;
    s32 index;

    Ram_HeapSlots->object_system->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);

    state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
    Func_080fee04(0, 3, 0, 7);
    state->selector_window = UiWindow_CreateFar(13, 0, 17, 5, 2);
    {
        struct { u16 value; } y;

        y.value = 26;
        /* FAKEMATCH: the native loads the row height before it builds the rows' offset (differ 2 without the barrier). */
        __asm__ volatile("" : "+r"(y.value));
        for (index = 3; index >= 0; index--)
            state->owner_y[index] = y.value;
    }
    state->page = 3;

    result = Func_080fe240();

    RenderOutput_ClearListFar(state->status_window);
    ItemMenu_Close();
    Ram_HeapSlots->object_system->suspended = 0;
    WaitFrames(1);
    Runtime_ReleaseHeapBlock(220);
    return result;
}

struct MenuRuntimeSelect {
    u8 unknown_000[0x168];
    u16 selected_slots[1];
    u8 unknown_16a[0xac];
    u8 selected_ids[1];
};

s32 Func_080fe580(s32 slot);

s32 Func_080fe240(void)
{
    struct MenuRuntimeSelect *work = (struct MenuRuntimeSelect *)Ram_HeapSlots->menu_runtime;
    s32 result = 0;

    /* FAKEMATCH: the do-while keeps the cursor store in source order, as in The Broken Seal's CharacterSelector_Run */
    do {
        work->selected_slots[0] = result;
    } while (0);
    if (Func_080fe580(0) == -1)
        result = -1;
    else
        result = work->selected_ids[0];
    return result;
}
