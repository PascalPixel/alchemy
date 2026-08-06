/* Advance the actor's +100 counter by a bounded random amount. */
#include "types.h"
 
extern u32 Func_0200359e(void);
extern void Func_020036d2(u8 *object, s32 mode);
extern void Func_020036dc(u8 *object, s32 mode);
s32 Func_020009fc(u8 *object)
{
    u16 *counter = (u16 *)(object + 100);
    *counter = (u16)(*counter + ((Func_0200359e() * 100) >> 16));
    if ((s16)*counter > 1000) Func_020036d2(object, 7);
    else Func_020036dc(object, 10);
    if (*(s16 *)counter > 1200) *counter = 0;
    return 1;
}
