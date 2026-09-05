#include "types.h"

#define Ui_BuildPatternToSlot Func_0801a2ec

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08002dd8(s32);
s32 Resource_CopyData(s32, s32, s32);
s32 Runtime_AllocateHeapBlock(s32 no, s32 arg1);
void Func_0801a4c0(u32 index);

s32 Ui_BuildPatternToSlot(s32 no, s32 arg1, s32 slot) {
    s32 work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    Func_0801a4c0(no);
    Resource_CopyData(slot, 0x80, work + 0x400);
    Func_08002dd8(0x11);
    return 1;
}
