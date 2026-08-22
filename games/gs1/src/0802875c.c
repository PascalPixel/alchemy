#include "types.h"

extern u8 Data_000000f1[];

u32 Func_08004938(s32 size);
s32 Func_08002f40(s32 index);
u32 Func_080053e8(const void *, void *);
void Func_08003fa4(s32, s32, void *);
void Func_08002df0(void *);

void Func_0802875c(s32 arg0, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)Func_08004938(size);
    u16 *base = (u16 *)Func_08002f40(Data_000000f1);

    /* 表内の相対位置から転送元を求める。 */
    Func_080053e8((void *)((u32)base + base[index]), buffer);
    Func_08003fa4(arg0, size, buffer);
    Func_08002df0(buffer);
}
