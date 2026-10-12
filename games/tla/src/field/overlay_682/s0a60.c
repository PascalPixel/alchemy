#include "types.h"

extern s32 Data_0200320c[];
extern s32 Data_0200b410[];
s32 GameFlag_Test(s32 flag);

const s32 *Func_02000a60(void)
{
    if (GameFlag_Test(0x97f))
        return Data_0200b410;
    return Data_0200320c;
}
