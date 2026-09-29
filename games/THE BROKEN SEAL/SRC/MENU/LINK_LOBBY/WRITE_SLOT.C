#include "TYPES.H"
#include "SERIAL_RUNTIME.H"

extern u8 LinkLobby_SlotColumns[];
extern s32 LinkLobby_SlotValues[];

/* Copies a slot's value into its column of the words the transfer sends. */
void LinkLobby_WriteSlotValue(s32 slot)
{
    s32 *dst = (s32 *)gSerialTransfer.reserved;
    s32 *src = &LinkLobby_SlotValues[slot];
    dst[LinkLobby_SlotColumns[slot]] = *src;
}
