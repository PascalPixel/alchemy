#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08002dd8(s32);
s32 Resource_ResetEntry(u16);
s32 ScheduleCallback(s32);
s32 Func_08015140();
extern u8 Data_080b00f5;

void Func_080b0204(void) {
    void *temp_r5;

    temp_r5 = *(void **)ADDR_03001F2C;
    ScheduleCallback((s32)&Data_080b00f5);
    Func_08015140();
    Resource_ResetEntry(M2C_FIELD(temp_r5, u16 *, 0x390));
    Resource_ResetEntry(M2C_FIELD(temp_r5, u16 *, 0x392));
    Resource_ResetEntry(M2C_FIELD(temp_r5, u16 *, 0x394));
    Resource_ResetEntry(M2C_FIELD(temp_r5, u16 *, 0x396));
    Resource_ResetEntry(M2C_FIELD(temp_r5, u16 *, 0x398));
    Resource_ResetEntry(M2C_FIELD(temp_r5, u16 *, 0x39A));
    Func_08002dd8(0x37);
}
