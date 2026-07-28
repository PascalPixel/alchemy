#include "types.h"

extern u8 Data_000000d5[];
void Func_080030f8(u32);
s32 Func_08002f40(s32 index);
void Func_08005340(s32, s32);
void Func_080113e4(void);
s32 Func_0800439c(u32 value);

struct MapInitWork {
    u8 unknown_000[0x100];
    s16 first;
    s16 second;
};

void Func_0801173c(void)
{
    struct MapInitWork *work = *(struct MapInitWork **)0x03001E70;
    *(s32 *)0x03001CFC = 0x0801161D;
    work->first = 0;
    work->second = 0x9f;
    Func_080030f8(1U);
    Func_08005340(Func_08002f40((s32) Data_000000d5), 0x02010000);
    Func_080113e4();
    Func_0800439c(0x0801179D);
    Func_080030f8(1U);
}
