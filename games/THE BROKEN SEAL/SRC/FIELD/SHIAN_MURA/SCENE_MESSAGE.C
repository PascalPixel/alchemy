#include "SHIAN.H"
extern u8 MsgShianWarriorsFromSchoolStrongWarriors[];

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void SceneEffect_RunActorSceneMessage(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgShianWarriorsFromSchoolStrongWarriors);
    Event_AskYesNo(17, 0);
    Event_End();
}
