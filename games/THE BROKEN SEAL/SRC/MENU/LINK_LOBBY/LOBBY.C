#include "EDITION.H"
/* The link lobby: its scene tables and serial initialization with IME saved and restored. */
#include "LOBBY.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "FIELD_EVENT.H"
#include "SERIAL_RUNTIME.H"
#include "IWRAM_CALL.H"

extern u16 gLinkStatus;
extern const s32 LinkLobby_SlotValues[];
extern const u8 LinkLobby_SlotColumns[];

/* Four-character wire tags identify the edition and each exchange phase. */
#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

const s32 LinkLobby_SlotValues[] = {
#if defined(TBS_EDITION_DE)
    PEER_TAG('C', 'S', 'G', 'G'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'G', 'S', 'C')
#elif defined(TBS_EDITION_ES)
    PEER_TAG('C', 'S', 'S', 'G'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'S', 'S', 'C')
#elif defined(TBS_EDITION_FR)
    PEER_TAG('C', 'S', 'G', 'M'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('C', 'S', 'G', 'M')
#elif defined(TBS_EDITION_IT)
    PEER_TAG('S', 'G', 'I', 'C'), PEER_TAG('0', '1', '2', '3'),
    PEER_TAG('A', 'B', 'C', '1'), PEER_TAG('C', 'D', 'E', '2'),
    PEER_TAG('E', 'F', 'G', '3'), PEER_TAG('C', 'I', 'G', 'S')
#else
#if EDITION_INTERNATIONAL
    PEER_TAG('S', 'G', 'M', 'C'),
#else
    PEER_TAG('C', 'M', 'G', 'S'),
#endif
    PEER_TAG('0', '1', '2', '3'), PEER_TAG('A', 'B', 'C', '1'),
    PEER_TAG('C', 'D', 'E', '2'), PEER_TAG('E', 'F', 'G', '3'),
    PEER_TAG('S', 'G', 'M', 'C')
#endif
};

#undef PEER_TAG

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
void SerialRuntime_RemoveIrqHandlers(void);
void SerialRuntime_Initialize(void);

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

void LinkLobby_InitializeSerial(void)
{
    volatile u16 *ime;
    u16 saved;

    ime = &REG_IME;
    saved = *ime;
    *ime = (u16)(u32)ime;
    SerialRuntime_RemoveIrqHandlers();
    SerialRuntime_Initialize();
    *ime = saved;
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

    if ((gLinkStatus & 3) == 3) {
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
