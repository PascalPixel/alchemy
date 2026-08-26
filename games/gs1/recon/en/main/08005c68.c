#include "save_state_family.h"

s32 Func_08005c68(void)
{
    struct SaveWorkspace *work;
    u8 *summary;
    u32 zero;
    s32 group;
    s32 count;

    work = SAVE_WORKSPACE;
    summary = work->summary[0];
    count = 0;
    group = 0;
    do {
        u32 index;

        zero = 0;
        START_DMA(&zero, summary, 0x85000010);
        index = Func_08005b24(group);
        if (index <= 15) {
            ReadFlash((u16)index, 0, summary, 64);
            count++;
        }
        index = Func_08005b24(group + 3);
        if (index <= 15)
            ReadFlash((u16)index, 0x110, summary + 56, 4);
        else
            *(u32 *)(summary + 56) = zero;
        group++;
        summary += 64;
    } while (group <= 2);
    return count;
}
