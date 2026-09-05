#include "types.h"

#define Battle_SetObjectFlag5bWhenMode3 Func_0808c44c

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Func_08009230();

void Battle_SetObjectFlag5bWhenMode3(void) {
    void *work;
    void *blk;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (FIELD_AT_OFFSET(work, s16 *, 0x19E) == 3) {
        blk = Runtime_AllocateBlock(0x1F, 0x540);
        if ((blk != NULL) && (FIELD_AT_OFFSET(blk, s8 *, 0x53D) != 0)) {
            FIELD_AT_OFFSET(blk, s8 *, 0x53A) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53B) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53C) = 1;
            FIELD_AT_OFFSET(blk, s8 *, 0x53D) = 0;
        }
        FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x1E0), s8 *, 0x5B) = 1;
        Func_08009230();
    }
}
