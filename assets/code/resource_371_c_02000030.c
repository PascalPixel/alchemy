#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
extern s32 Func_020041c2(s32, s32);
extern s32 Func_020041f4(void);
extern void Func_02004484(s32, s32);

void Func_02000030(void)
{
    u8 *p = Data_03001ebc;
    s16 *table = Data_02000240;
    s32 *target = (s32 *)&table[284];
    s32 *cur = (s32 *)(p + 428);

    if (*target >= Func_020041c2(*cur * 9, 10)) {
        if ((u32)Func_020041f4() < 0x8000) {
            Func_02004484(0x808, 3);
            *(s32 *)(p + 424) = 0;
        } else {
            *target = *cur;
        }
    }
}
