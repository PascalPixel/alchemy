#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void ScheduleCallback(void *);
void UiWork_Finalize(struct Work *work, s32 release);
s32 Resource_ResetEntry(u32 index);
void Func_08002dd8(s32);
void WaitFrames(u32);
extern u8 Data_08028195;

void Menu_EndResourceSelection(void) {
    struct Work *childWork;
    s32 i;
    u16 *entry;
    void *work;

    work = *(void **)ADDR_03001F38;
    ScheduleCallback(&Data_08028195);
    childWork = FIELD_AT_OFFSET(work, struct Work *, 0x78);
    if (childWork != 0) {
        UiWork_Finalize(childWork, 2);
    }
    i = 0;
    while (i < (s32)FIELD_AT_OFFSET(work, s16, 0x8E)) {
        entry = (u16 *)((u8 *)work + 0x12) + i * 10;
        Resource_ResetEntry(*entry);
        i += 1;
    }
    Func_08002dd8(0x3A);
    WaitFrames(1U);
}
