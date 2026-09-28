/* Draft of LinkLobby_StartExchange, resource_3cb at 0x02009050 (was
 * MENU/LINK_LOBBY/START_EXCHANGE.C).
 * Remaining difference: the ROM loads the 1 it passes from its literal pool,
 * as a link-time value.
 * The listing keeps these rows. */
#include "TYPES.H"

extern void SerialRuntime_RemoveIrqHandlers(void);
extern void Sound_LoadPresetParameters(s32);
extern s32 Event_SetPairWork1c0(s32, s32);

s32 LinkLobby_StartExchange(void)
{
    SerialRuntime_RemoveIrqHandlers();
    Sound_LoadPresetParameters(2);
    /* FAKEMATCH: the do/while loads the linked value before the 1. */
    do {
        return Event_SetPairWork1c0(0x1, 1);
    } while (0);
}
