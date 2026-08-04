#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_020089c8[];
extern u8 Data_02008890[];

void Func_020009b0(void *);

s32 Func_02000328(void)
{
    if (Data_02000240[225] == 8) {
        return (s32)Data_020089c8;
    }
    Func_020009b0(Data_02008890);
    return (s32)Data_02008890;
}
