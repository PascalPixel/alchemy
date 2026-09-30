/* The link lobby: its scene tables and the serial query with interrupts held. */
#include "LOBBY.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SERIAL_RUNTIME.H"
#include "IWRAM_CALL.H"

extern u8 gLinkStatus[];
extern const s32 LinkLobby_SlotValues[];
extern const u8 LinkLobby_SlotColumns[];

/* One linked player's six compared words. */
struct LinkPeer {
    s32 values[6];
};

extern struct LinkPeer gLinkPeerSignatures[2];

struct LobbyPanel {
    u8 unknown_00[24];
};

s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);

/* Frames the peers have been waited for; it follows the overlay's image. */
s32 gLinkLobbyWaitFrames;

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
    s16 v = gGameState.entrance;

    if (v == 11 || v == 9) {
        return (s32)gLinkLobbyBattlePlacements;
    }
    return (s32)gLinkLobbyPlacements;
}

/* Record whether a link is up (flag 0x303) and whether this player is a child (flag 0x302), then report whether the other player's word for this slot equals the slot's expected value. */
s32 LinkLobby_PeerSlotMatches(s32 slot)
{
    s32 id = -1;

    if ((*(u16 *)gLinkStatus & 3) == 3) {
        id = (u32)(REG_SIOCNT << 26) >> 30;
        Engine_GameFlagSet(0x303);
    } else {
        Engine_GameFlagClear(0x303);
    }
    /* FAKEMATCH: a loop that runs at most once places the test after the body as the ROM does. */
    while (id >= 0 && Engine_GameFlagIsSet(0x303)) {
        const s32 *expected = &LinkLobby_SlotValues[slot];

        if (id != 0)
            Engine_GameFlagSet(0x302);
        else
            Engine_GameFlagClear(0x302);
        {
            struct LinkPeer *row = &gLinkPeerSignatures[Engine_GameFlagIsSet(0x302) ^ 1];

            if (row->values[LinkLobby_SlotColumns[slot]] == *expected)
                return 1;
            break;
        }
    }
    return 0;
}

/* Copies a slot's value into its column of the words the transfer sends. */
void LinkLobby_WriteSlotValue(s32 slot)
{
    s32 *dst = (s32 *)gSerialTransfer.reserved;
    s32 *src = &LinkLobby_SlotValues[slot];
    dst[LinkLobby_SlotColumns[slot]] = *src;
}

/* The link lobby: poll whether the peers stand ready in the circle. */
s32 LinkLobby_PollPeerReady(void)
{
    struct EventWork *work;
    s32 result;
    s32 i;

    work = gEventWork;
    result = 1;
    if (Object_GetById(0)->z.fixed > 0xe00000) {
        Engine_GameFlagClear(0x304);
    }
    if (work->raised_trigger != 2) {
        LinkLobby_PeerSlotMatches(0);
        if (!Engine_GameFlagIsSet(0x303)) {
            if (++gLinkLobbyWaitFrames > 25) {
                for (i = 0; i < 4; i++) {
                    Iwram_ClearWords(&gLinkPeerSignatures[i], 20);
                }
                gLinkLobbyWaitFrames = 0;
                LinkLobby_WriteSlotValue(4);
            }
        } else {
            gLinkLobbyWaitFrames = 0;
        }
        if (gLinkLobbyWaitFrames == 0) {
            if (LinkLobby_PeerSlotMatches(0)
                && (LinkLobby_PeerSlotMatches(1) || LinkLobby_PeerSlotMatches(2))) {
                Engine_GameFlagSet(0x201);
                if (Engine_GameFlagIsSet(0x202)) {
                    work->raised_trigger = 1;
                }
                result = 1;
            } else {
                Engine_GameFlagClear(0x201);
                result = 0;
            }
        }
        if (Engine_GameFlagIsSet(0x201) && Engine_GameFlagIsSet(0x202) && !Engine_GameFlagIsSet(0x200)) {
            work->raised_trigger = 1;
        }
    }
    if ((GameFlag_IsSet(0x201) || GameFlag_IsSet(0x202)) && !GameFlag_IsSet(0x173)
        && !LinkLobby_PeerSlotMatches(0) && gLinkLobbyWaitFrames > 24) {
        work->raised_trigger = 2;
        GameFlag_Set(0x205);
        GameFlag_Clear(0x201);
        GameFlag_Clear(0x202);
        LinkLobby_WriteSlotValue(4);
    }
    if (GameFlag_IsSet(0x205)) {
        work->raised_trigger = 2;
    }
    return result;
}
