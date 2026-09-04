#include "types.h"

#define Battle_ClearObjectFlag5bWhenMode3 Func_0808c4c0

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Func_08009238();

void Battle_ClearObjectFlag5bWhenMode3(void) {
    void *work;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (FIELD_AT_OFFSET(work, s16 *, 0x19E) == 3) {
        Func_08009238();
        FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x1E0), s8 *, 0x5B) = 0;
    }
}
