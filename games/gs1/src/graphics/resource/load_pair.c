#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002dd8(s32);
u32 Func_080053e8(const void *, void *);
extern s32 Data_08031864[];

void RenderResource_LoadPair(s32 arg0, s32 arg1) {
    register s32 value;
    void *temp_r6;

    temp_r6 = (void *)Runtime_AllocateBlock(14, 0x400);
    if ((value = Data_08031864[arg0], arg1 <= 0x5F)) {
        Func_080053e8((const void *)value, temp_r6);
        Resource_CopyData(arg1, 0x200, temp_r6);
        Func_08002dd8(14);
    }
}
