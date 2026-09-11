#include "types.h"
#include "scene.h"
#include "abi/audio/command/get_secondary_state_byte.h"

u8 AudioCommand_GetSecondaryStateByte(void)
{
    /* Current secondary sound status byte. */
    return *(u8 *)0x0200303c;
}
