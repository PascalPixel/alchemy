#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData(s32, s32, u8 *);
extern u8 Data_080aed4c[];
extern u8 Data_080aedcc[];

void Resource_LoadPairedBlocksIfAvailable(void) {
    s32 temp_r0;
    s32 temp_r0_2;
    void *temp_r5;

    temp_r5 = *(void **)ADDR_03001F2C;
    temp_r0 = Resource_FindFreeSlot();
    FIELD_AT_OFFSET(temp_r5, s16, 0x392) = (s16) temp_r0;
    if (temp_r0 != -1) {
        Resource_CopyData(temp_r0, 0x80, Data_080aed4c);
    }
    temp_r0_2 = Resource_FindFreeSlot();
    FIELD_AT_OFFSET(temp_r5, s16, 0x394) = (s16) temp_r0_2;
    if (temp_r0_2 != -1) {
        Resource_CopyData(temp_r0_2, 0x80, Data_080aedcc);
    }
}
