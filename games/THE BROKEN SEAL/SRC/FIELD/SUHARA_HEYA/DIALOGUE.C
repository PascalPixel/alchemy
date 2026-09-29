#include "SUHARA.H"

void Dialogue_HandleFacingBranch(s32 no)
{
    u16 facing = (((u16 *)Engine_ActorGet(ACTOR_PARTY_LEADER))[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Inn_CheckIn(10, no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        Engine_EventSetMessage(0x2620);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage(0x25d1);
        Engine_EventShowMessage(no, 0);
    }
}

void Dialogue_HandleFacingAction(s32 no)
{
    u16 facing = (((u16 *)Engine_ActorGet(ACTOR_PARTY_LEADER))[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Shop_ConfirmAct(no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        Engine_EventSetMessage(0x262c);
        Engine_EventShowMessage(no, 0);
    } else {
        Event_SetMessage(MSG_GET_SICK_THINKING_ABOUT_LALIVERO);
        Engine_EventShowMessage(no, 0);
    }
}
