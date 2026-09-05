#include "metadata_lookup.h"
#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Animation_SetWorkEntry(void *work, s32 no)
{
    s32 hi;
    void *info;
    s32 value;

    hi = 0x80 & no;
    if (FIELD_AT_OFFSET(work, s32, 0x0c) != 0) {
        info = Func_08185000((s32)FIELD_AT_OFFSET(work, s16, 0));
        if (no < (s32)FIELD_AT_OFFSET(info, u8, 5)) {
            value = *(s32 *)((u8 *)FIELD_AT_OFFSET(work, s32, 0x0c) + (no * 4));
            FIELD_AT_OFFSET(work, u8, 4) = (u8)FIELD_AT_OFFSET(info, u8, 4);
            FIELD_AT_OFFSET(work, s32, 0x10) = value;
            FIELD_AT_OFFSET(work, s8, 0x15) = 0x10;
            if (hi == 0) {
                FIELD_AT_OFFSET(work, s8, 0x14) = hi;
                FIELD_AT_OFFSET(work, s16, 2) = (s16)hi;
            }
        }
    }
}
