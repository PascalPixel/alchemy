/* The chef's line. */
#include "TOREBI.H"

void SceneDialogue_RunActorLine23a1(s32 no)
{
    Event_Begin();
    Event_SetMessage(MSG_GRRR_CHEF_IN_BAD_MOOD);
    Event_ShowMessage(no, 0);
    Event_End();
}
