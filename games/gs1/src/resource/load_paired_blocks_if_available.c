#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData(s32, s32, u8 *);
extern u8 Data_080aed4c[];
extern u8 Data_080aedcc[];

void Resource_LoadPairedBlocksIfAvailable(void) {
    s32 first_slot;
    s32 second_slot;
    void *active_state;

    active_state = *(void **)ADDR_03001F2C;
    first_slot = Resource_FindFreeSlot();
    FIELD_AT_OFFSET(active_state, s16, 0x392) = (s16) first_slot;
    if (first_slot != -1) {
        Resource_CopyData(first_slot, 0x80, Data_080aed4c);
    }
    second_slot = Resource_FindFreeSlot();
    FIELD_AT_OFFSET(active_state, s16, 0x394) = (s16) second_slot;
    if (second_slot != -1) {
        Resource_CopyData(second_slot, 0x80, Data_080aedcc);
    }
}
