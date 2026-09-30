#include "TYPES.H"

extern u8 Data_03001e98[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Resource_ClearOwnerListAndCounters(void)
{
    void *state;

    state = *(void **)((u32)&Data_03001e98);
    FIELD_AT_OFFSET(state, s32 *, 0x348) = 0;
    FIELD_AT_OFFSET(state, s16 *, 0x39A) = 0;
    if (0x80 & FIELD_AT_OFFSET(state, u16 *, 0x39E)) {
        FIELD_AT_OFFSET(state, s16 *, 0x39C) = 0;
        FIELD_AT_OFFSET(state, u16 *, 0x39E) = 0U;
    }
    FIELD_AT_OFFSET(state, s16 *, 0x3A0) = 0;
    FIELD_AT_OFFSET(state, s16 *, 0x394) = 0;
}
