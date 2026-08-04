#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_020021de(s32);
extern u8 *Func_0200366c(s32);
extern void Func_02003714(void);

void Func_02001120(void)
{
    u8 *slot;
    s16 *counter;
    s32 reset;

    Func_020021de(2);
    slot = Func_0200366c(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        counter = (s16 *)(Data_03001ebc + 412);
        if (*counter > 12) {
            Func_02003714();
            reset = 0;
            *counter = reset;
        }
    }
}
