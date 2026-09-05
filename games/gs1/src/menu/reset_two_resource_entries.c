#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Resource_ResetEntry(u16);

void Menu_ResetTwoResourceEntries(void) {
    void *state;

    state = *(void **)ADDR_03001F2C;
    Resource_ResetEntry(FIELD_AT_OFFSET(state, u16 *, 0x392));
    Resource_ResetEntry(FIELD_AT_OFFSET(state, u16 *, 0x394));
}
