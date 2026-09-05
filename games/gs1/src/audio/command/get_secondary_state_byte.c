#include "types.h"

#define AudioCommand_GetSecondaryStateByte Func_080f9594

u8 AudioCommand_GetSecondaryStateByte(void)
{
    /* Current secondary sound status byte. */
    return *(u8 *)0x0200303c;
}
