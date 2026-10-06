#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SERIAL_RUNTIME.H"

void BattlePres_ClearLinkPayload(void)
{
    Iwram_ClearWords(gSerialTransfer.payload, 16);
}
