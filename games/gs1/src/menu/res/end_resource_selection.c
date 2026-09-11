#include "types.h"
#include "scene.h"
#include "abi/menu/res/end_resource_selection.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void ScheduleCallback(void *);
void UiWork_Finalize(struct Work *work, s32 release);
s32 Resource_ResetEntry(u32 index);

void WaitFrames(u32);
extern u8 gRom;

void Menu_EndResourceSelection(void)
{
    struct Work *child;
    s32 i;
    u16 *entry;
    void *work;

    work = *(void **)ADDR_03001F38;
    ScheduleCallback(&gRom);
    child = FIELD_AT_OFFSET(work, struct Work *, 0x78);
    if (child != 0) {
        UiWork_Finalize(child, 2);
    }
    i = 0;
    while (i < (s32)FIELD_AT_OFFSET(work, s16, 0x8E)) {
        entry = (u16 *)((u8 *)work + 0x12) + i * 10;
        Resource_ResetEntry(*entry);
        i += 1;
    }
    Menu_Do(0x3A);
    WaitFrames(1U);
}
