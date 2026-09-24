#include "TYPES.H"
#include "FIELD_EVENT.H"

struct LobbyPanel {
    u8 unknown_00[24];
};

s32 LinkLobby_PeerSlotMatches(s32 slot);
/* The IWRAM panel routine, called through a function pointer. */
typedef void (*PanelFn)(struct LobbyPanel *panel, s32 value);
#define LobbyPanel_Update ((PanelFn)0x03000164)
void LinkLobby_Func02000128(s32 slot);

extern s32 Data_02009f4c;
extern struct LobbyPanel Data_02002024[];

s32 LinkLobby_Func02000148(void)
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
            if (++Data_02009f4c > 25) {
                for (i = 0; i < 4; i++) {
                    LobbyPanel_Update(&Data_02002024[i], 20);
                }
                Data_02009f4c = 0;
                LinkLobby_Func02000128(4);
            }
        } else {
            Data_02009f4c = 0;
        }
        if (Data_02009f4c == 0) {
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
        && !LinkLobby_PeerSlotMatches(0) && Data_02009f4c > 24) {
        work->raised_trigger = 2;
        GameFlag_Set(0x205);
        GameFlag_Clear(0x201);
        GameFlag_Clear(0x202);
        LinkLobby_Func02000128(4);
    }
    if (GameFlag_IsSet(0x205)) {
        work->raised_trigger = 2;
    }
    return result;
}
