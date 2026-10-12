#include "types.h"

extern const u8 Data_020027f4[];
extern const u8 Data_02002ad0[];
extern const u8 Data_02002e48[];
extern const u8 Data_02003154[];

s32 GameFlag_Test(s32 flag);

const u8 *Func_02000788(void)
{
    if (GameFlag_Test(0x8a7)) {
        if (GameFlag_Test(0x90f) == 0)
            return Data_02003154;
        return Data_02002e48;
    }
    if (GameFlag_Test(0x8ab))
        return Data_02002ad0;
    return Data_020027f4;
}
