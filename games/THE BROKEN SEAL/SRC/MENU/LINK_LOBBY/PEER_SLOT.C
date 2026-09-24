#include "TYPES.H"
#include "SERIAL_RUNTIME.H"

#define LINK_STAT (*(u16 *)0x03001f64)

void Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
s32 Engine_GameFlagIsSet(s32 flag);

extern const s32 LinkLobby_SlotValues[];
extern const u8 LinkLobby_SlotColumns[];

/* One linked player's six compared words. */
struct LinkPeer {
    s32 values[6];
};

extern struct LinkPeer LinkLobby_PeerValues[2];

/* Record whether a link is up (flag 0x303) and whether this player is a child (flag 0x302), then report whether the other player's word for this slot equals the slot's expected value. */
/* Adopt LinkLobby_PeerSlotMatches resource_3cb:0200008c (156 bytes, fake match) */
s32 LinkLobby_PeerSlotMatches(s32 slot)
{
    s32 id = -1;

    if ((LINK_STAT & 3) == 3) {
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
            struct LinkPeer *row = &LinkLobby_PeerValues[Engine_GameFlagIsSet(0x302) ^ 1];

            if (row->values[LinkLobby_SlotColumns[slot]] == *expected)
                return 1;
            break;
        }
    }
    return 0;
}
