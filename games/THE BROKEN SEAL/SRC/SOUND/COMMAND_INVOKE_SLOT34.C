#include "TYPES.H"

/* Sound_CommandTable (0x02004000) slot 34 */
extern void (*Data_02004088)(s32);

void AudioCommand_InvokeSlot34(s32 argument)
{
    Data_02004088(argument);
}
