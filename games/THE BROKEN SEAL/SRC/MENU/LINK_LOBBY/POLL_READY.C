/* The link lobby: poll whether the peers stand ready in the circle. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

struct LobbyPanel {
    u8 unknown_00[24];
};

s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);

/* Frames the peers have been waited for; it follows the overlay's image. */
static s32 sWaitFrames;
extern struct LobbyPanel gLinkPeerSignatures[];

s32 LinkLobby_PollPeerReady(void)
{
    struct EventWork *work;
    s32 result;
    s32 i;

    work = gEventWork;
    result = 1;
    if (Engine_ActorGet(0)->z.fixed > 0xe00000) {
        Engine_GameFlagClear(0x304);
    }
    if (work->raised_trigger != 2) {
        LinkLobby_PeerSlotMatches(0);
        if (!Engine_GameFlagIsSet(0x303)) {
            if (++sWaitFrames > 25) {
                for (i = 0; i < 4; i++) {
                    Iwram_ClearWords(&gLinkPeerSignatures[i], 20);
                }
                sWaitFrames = 0;
                LinkLobby_WriteSlotValue(4);
            }
        } else {
            sWaitFrames = 0;
        }
        if (sWaitFrames == 0) {
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
        && !LinkLobby_PeerSlotMatches(0) && sWaitFrames > 24) {
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
