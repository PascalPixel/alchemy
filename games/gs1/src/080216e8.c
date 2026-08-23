#include "types.h"
#include "resource.h"

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
u32 Func_080053e8(const void *, void *);
void Func_08009288(void *, s32);
void Resource_CopyData(s32, s32, void *);
void Func_08002dd8(s32);
extern unsigned char Data_000000f1[];

void Func_080216e8(s32 index, s32 value, s32 flag) {
    s32 size = 1024;
    void *buffer = Runtime_AllocateBlock(14, size);
    u16 *base = GetResource((s32)Data_000000f1);

    if (value <= 95) {
        Func_080053e8((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Func_08009288(buffer, 768);
        Resource_CopyData(value, size, buffer);
        Func_08002dd8(14);
    }
}
