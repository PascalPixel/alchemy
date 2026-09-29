#include "SUHARA.H"
extern u8 MsgSuharaBusyEverSince[];
extern u8 MsgSuharaGetSickThinkingAboutLalivero[];
extern u8 MsgSuharaSighNothinMaybe[];
extern u8 MsgSuharaWonderWhySandstorms[];

void Dialogue_HandleFacingBranch(s32 no)
{
    u16 facing = (((u16 *)Engine_ActorGet(ACTOR_PARTY_LEADER))[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Inn_CheckIn(10, no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        Engine_EventSetMessage((s32)MsgSuharaBusyEverSince);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgSuharaSighNothinMaybe);
        Engine_EventShowMessage(no, 0);
    }
}

void Dialogue_HandleFacingAction(s32 no)
{
    u16 facing = (((u16 *)Engine_ActorGet(ACTOR_PARTY_LEADER))[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Shop_ConfirmAct(no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        Engine_EventSetMessage((s32)MsgSuharaWonderWhySandstorms);
        Engine_EventShowMessage(no, 0);
    } else {
        Event_SetMessage((s32)MsgSuharaGetSickThinkingAboutLalivero);
        Engine_EventShowMessage(no, 0);
    }
}
