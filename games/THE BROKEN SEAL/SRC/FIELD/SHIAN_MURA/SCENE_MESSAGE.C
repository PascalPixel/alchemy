#include "SHIAN.H"

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void SceneEffect_RunActorSceneMessage(void)
{
    Event_Begin();
    Event_SetMessage(MSG_WARRIORS_FROM_SCHOOL_STRONG_WARRIORS);
    Event_AskYesNo(17, 0);
    Event_End();
}
