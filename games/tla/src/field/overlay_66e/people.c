#include "types.h"

extern const u8 Data_02001370[];
extern const u8 Data_020014c0[];

s32 GameFlag_Test(s32 flag);

const u8 *Func_020000a4(void)
{
    if (GameFlag_Test(0x8ff))
        return Data_020014c0;
    return Data_02001370;
}
