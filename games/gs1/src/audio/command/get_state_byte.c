#include "types.h"

#define AudioCommand_GetStateByte Func_080f954c

u8 AudioCommand_GetStateByte(void)
{
    /* Current sound command/state byte. */
    return *(u8 *)0x02003000;
}
