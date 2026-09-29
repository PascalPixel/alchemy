/* The link lobby: its scene tables and the serial query with interrupts held. */
#include "LOBBY.H"

u8 *LinkLobby_GetEntrances(void)
{
    return gLinkLobbyEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u32 State_RunQueryWithInterruptMasterSaved(void)
{
    volatile u16 *ime = (volatile u16 *)0x04000208;
    u32 saved = *ime;
    u32 ret;

    *ime = (u16)(u32)ime;
    SerialRuntime_RemoveIrqHandlers();
    ret = SerialRuntime_Initialize();
    *ime = saved;
    return ret;
}

u8 *LinkLobby_GetExits(void) { return gLinkLobbyExits; }
s32 LinkLobby_SelectPlacements(void)
{
    s16 v = gGameState[225];

    if (v == 11 || v == 9) {
        return (s32)gLinkLobbyBattlePlacements;
    }
    return (s32)gLinkLobbyPlacements;
}
