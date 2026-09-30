#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

extern u8 LinkLobby_SlotColumns[];
extern s32 LinkLobby_SlotValues[];

struct LobbyPanel {
    u8 unknown_00[24];
};

s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);

/* Frames the peers have been waited for; it follows the overlay's image. */
s32 gLinkLobbyWaitFrames;
extern struct LobbyPanel gLinkPeerSignatures[];

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
