/* The chef's line. */
#include "TOREBI.H"
extern u8 MsgTorebiGrrrChefInBadMood[];

void SceneDialogue_RunActorLine23a1(s32 no)
{
    Event_Begin();
    Event_SetMessage((s32)MsgTorebiGrrrChefInBadMood);
    Event_ShowMessage(no, 0);
    Event_End();
}
