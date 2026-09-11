#include "metadata_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/animation/init_work_from_metadata.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Animation_InitWorkFromMetadata(void *work)
{
    s32 value;
    s32 z;
    void *info;

    if (work != NULL) {
        info = Sys_Run(FIELD_AT_OFFSET(work, s16, 0));
        if (FIELD_AT_OFFSET(info, u8, 0) != 0) {
            value = FIELD_AT_OFFSET(info, s32, 0x0c);
            if (value == 0) {
                value = Sys_Check(FIELD_AT_OFFSET(work, s16, 0));
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
