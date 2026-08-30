#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08002dd8(s32);
s32 Resource_ResetEntry(u16);
s32 ScheduleCallback(s32);
s32 UiWork_FinalizePending();
extern u8 Data_080b00f5;

void Inn_CleanupResources(void) {
    void *temp_r5;

    temp_r5 = *(void **)ADDR_03001F2C;
    ScheduleCallback((s32)&Data_080b00f5);
    UiWork_FinalizePending();
    Resource_ResetEntry(FIELD_AT_OFFSET(temp_r5, u16 *, 0x390));
    Resource_ResetEntry(FIELD_AT_OFFSET(temp_r5, u16 *, 0x392));
    Resource_ResetEntry(FIELD_AT_OFFSET(temp_r5, u16 *, 0x394));
    Resource_ResetEntry(FIELD_AT_OFFSET(temp_r5, u16 *, 0x396));
    Resource_ResetEntry(FIELD_AT_OFFSET(temp_r5, u16 *, 0x398));
    Resource_ResetEntry(FIELD_AT_OFFSET(temp_r5, u16 *, 0x39A));
    Func_08002dd8(0x37);
}
