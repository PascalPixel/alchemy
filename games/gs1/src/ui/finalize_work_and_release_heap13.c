#include "types.h"
#include "scene.h"
#include "global_cells.h"

void UiWork_Finalize(struct Work *work, s32 release);
s32 UiWork_IsIdle(void *arg0);
void WaitFrames(s32);
s32 Resource_ResetEntry(u32 index);

void Ui_FinalizeWorkAndReleaseHeap13(void)
{
    u8 *state;
    u8 *unused;
    u16 *p;

    state = *(u8 **)ADDR_03001E9C;
    UiWork_Finalize(FIELD_AT_OFFSET(state, s32 *, 0xFF4), 0);
    while (UiWork_IsIdle(FIELD_AT_OFFSET(state, s32 *, 0xFF4)) == 0) {
        WaitFrames(1);
    }
    if (FIELD_AT_OFFSET(state, u16 *, 0x46) != 0) {
        Resource_ResetEntry(FIELD_AT_OFFSET(state, u16 *, 0x48));
    }
    p = (u16 *)(state + 0x352);
    if (*p != 0) {
        p = (u16 *)((u8 *)p + 2);
        Resource_ResetEntry(*p);
    }
    Ui_Do(0x13);
}
