#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

extern u8 Data_03001e9c[];
s32 UiWork_IsIdle(void *arg0);
s32 Resource_ResetEntry(u32 index);

/* The two resource groups and closing window in heap slot 19. */
struct SelectionWorkspace {
    u8 unknown_000[0x46];
    u16 first_active;
    u16 first_resource;
    u8 unknown_04a[0x352 - 0x4a];
    u16 second_active;
    u16 second_resource;
    u8 unknown_356[0xff4 - 0x356];
    struct UiWindow *window;
    u8 unknown_ff8[12];
};

LAYOUT_SIZE_GUARD(SelectionWorkspace_Size, struct SelectionWorkspace, 0x1004);
LAYOUT_OFFSET_GUARD(SelectionWorkspace_Window, struct SelectionWorkspace, window, 0xff4);

void Menu_AllocateSelectionWorkspace(void)
{
    struct SelectionWorkspace *work;
    u32 zero;

    work = Runtime_AllocateBlock(0x13, 0x1004);
    zero = 0;
    work->first_active = zero;
    work->second_active = zero;
}

void Ui_FinalizeWorkAndReleaseHeap13(void)
{
    struct SelectionWorkspace *state;
    u16 *p;

    state = *(struct SelectionWorkspace **)Data_03001e9c;
    UiWork_Finalize(state->window, 0);
    while (UiWork_IsIdle(state->window) == 0) {
        WaitFrames(1);
    }
    if (state->first_active != 0) {
        Resource_ResetEntry(state->first_resource);
    }
    p = &state->second_active;
    if (*p != 0) {
        p = (u16 *)((u8 *)p + 2);
        Resource_ResetEntry(*p);
    }
    Runtime_ReleaseHeapBlock(0x13);
}

void GraphicsPalette_ReservedNoOpC9BC(void)
{
}

void GraphicsPalette_ReservedNoOpC9C0(void)
{
}

void GraphicsPalette_ReservedNoOpC9C4(void)
{
}
