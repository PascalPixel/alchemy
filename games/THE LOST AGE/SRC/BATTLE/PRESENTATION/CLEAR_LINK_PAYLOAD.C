#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SERIAL_RUNTIME.H"

/* Clears the sixteen bytes of payload this console sends over the link. */
void BattlePres_ClearLinkPayload(void)
{
    Iwram_ClearWords(gSerialTransfer.payload, 16);
}
