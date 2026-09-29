/* The house's counters: a shop or the inn when the leader faces them,
 * otherwise a line chosen by flag 0x96f. */
#include "SUHARA.H"
extern u8 MsgSuharaArentSurprisedFind[];
extern u8 MsgSuharaBusyEverSince[];
extern u8 MsgSuharaGetSickThinkingAboutLalivero[];
extern u8 MsgSuharaOursOnlyStore[];
extern u8 MsgSuharaSighNothinMaybe[];
extern u8 MsgSuharaWonderWhySandstorms[];

void Dialogue_HandleFacingChoice(s32 no)
{
    u16 facing = (Actor_Get(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Engine_ShopOpen(31, no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        s32 msg = (s32)MsgSuharaArentSurprisedFind;
        Event_SetMessage(msg);
        Event_OpenMessage(no, 0);
        if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(no, 0);
    } else {
        Event_SetMessage((s32)MsgSuharaOursOnlyStore);
        Event_ShowMessage(no, 0);
    }
}

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
