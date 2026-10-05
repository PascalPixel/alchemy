#include "TYPES.H"
#include "SERIAL_RUNTIME.H"

extern const s32 LinkLobby_SlotValues[];
extern const u8 LinkLobby_SlotColumns[];

void LinkLobby_WriteSlotValue(s32 slot)
{
    s32 *dst = (s32 *)gSerialTransfer.payload;
    const s32 *src = &LinkLobby_SlotValues[slot];
    dst[LinkLobby_SlotColumns[slot]] = *src;
}
