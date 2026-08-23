#include "types.h"
#include "global_cells.h"
#include "resource.h"

extern u8 Data_000000d5[];
void Func_080030f8(u32);
void Func_08005340(s32, s32);
void Func_080113e4(void);
void Func_0801161c(void);
void Func_0801179c(void);
s32 Func_0800439c(u32 value);

struct MapInitWork {
    u8 unknown_000[0x100];
    s16 first;
    s16 second;
};

void Func_0801173c(void)
{
    struct MapInitWork *work = *(struct MapInitWork **)ADDR_03001E70;
    *(s32 *)ADDR_03001CFC = (s32)Func_0801161c;
    work->first = 0;
    work->second = 0x9f;
    Func_080030f8(1U);
    Func_08005340((s32)GetResource((s32)Data_000000d5), 0x02010000);
    Func_080113e4();
    Func_0800439c((u32)Func_0801179c);
    Func_080030f8(1U);
}
