#include "GLOBAL.H"
#include "TYPES.H"
#include "SERIAL_RUNTIME.H"


void LinkLobby_WriteSlotValue(s32 slot)
{
    s32 *dst = (s32 *)gSerialTransfer.payload;
    const s32 *src = &LinkLobby_SlotValues[slot];
    dst[LinkLobby_SlotColumns[slot]] = *src;
}
