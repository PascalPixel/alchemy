#include "types.h"
#include "scene.h"
#include "abi/audio/command/get_state_byte.h"

u8 AudioCommand_GetStateByte(void)
{
    /* Current sound command/state byte. */
    return *(u8 *)0x02003000;
}
