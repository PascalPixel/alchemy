#include "types.h"

extern void Func_02004348();

void Func_0200011c(u8 *object)
{
    u16 *counter = (u16 *)(object + 0x64);

    /* Arm order decides the branch sense: the reference falls through to the
     * increment and branches away to the call, which is `bgt`. */
    if (*(s16 *)counter <= 0) {
        *counter = (u16)(*counter + 1);
    } else {
        Func_02004348(object);
    }
}
