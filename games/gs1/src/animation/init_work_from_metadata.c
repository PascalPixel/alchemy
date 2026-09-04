#include "metadata_lookup.h"
#include "types.h"

#define Animation_InitWorkFromMetadata Func_0800b868

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_0800b798(s32 arg0);

void Animation_InitWorkFromMetadata(void *work)
{
    s32 value;
    s32 z;
    void *info;

    if (work != NULL) {
        info = Func_08185000(FIELD_AT_OFFSET(work, s16, 0));
        if (FIELD_AT_OFFSET(info, u8, 0) != 0) {
            value = FIELD_AT_OFFSET(info, s32, 0x0c);
            if (value == 0) {
                value = Func_0800b798(FIELD_AT_OFFSET(work, s16, 0));
            }
            FIELD_AT_OFFSET(work, u8, 4) = FIELD_AT_OFFSET(info, u8, 4);
            FIELD_AT_OFFSET(work, s32, 0x0c) = FIELD_AT_OFFSET(info, s32, 0x10);
            FIELD_AT_OFFSET(work, s32, 8) = value;
            FIELD_AT_OFFSET(work, u8, 7) = FIELD_AT_OFFSET(info, u8, 0x0a);
            z = 0;
            FIELD_AT_OFFSET(work, u8, 0x16) = 0xff;
            FIELD_AT_OFFSET(work, s32, 0x10) = z;
            FIELD_AT_OFFSET(work, u8, 0x14) = z;
        }
    }
}
