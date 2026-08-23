#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Resource_ResetEntry(u16);

void Func_080ae8dc(void) {
    void *temp_r5;

    temp_r5 = *(void **)ADDR_03001F2C;
    Resource_ResetEntry(M2C_FIELD(temp_r5, u16 *, 0x392));
    Resource_ResetEntry(M2C_FIELD(temp_r5, u16 *, 0x394));
}
