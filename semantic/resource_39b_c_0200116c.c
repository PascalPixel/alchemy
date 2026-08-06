#include "types.h"





/* Select actor 8's path according to actor 0's wrapped X tile. */
extern u8 * Func_0200365e();
extern void Func_02003648();
extern void Func_0200368c();
extern void Func_02003696();
void Func_0200116c(void)
{
    s32 x = *(s32 *)(Func_0200365e(0) + 8);
    s32 tile = ((x < 0 ? x + 0x000fffff : x) >> 20);

    Func_02003648(0x205);
    if (tile == 7)
        Func_0200368c(8, (void *)0x0200a874);
    else
        Func_02003696(8, (void *)0x0200a8c8);
}
