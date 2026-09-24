#include "TYPES.H"

/* Sound_CommandTable (0x02004000) slot 35 */
extern void (*Data_0200408c)(s32);

void AudioCommand_InvokeSlot35(s32 argument)
{
    Data_0200408c(argument);
}
