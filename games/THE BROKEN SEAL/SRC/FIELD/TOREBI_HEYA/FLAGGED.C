/* Flagged lines. */
#include "TOREBI.H"

void SceneDialogue_RunActor25FlaggedLine(void)
{
    void Event_Begin(void);

    Event_Begin();
    if (GameFlag_IsSet(0x8BE) == 0) {
        Event_SetMessage(MSG_HEH_HEH_SHE_JUST_HID);
    } else {
        Event_SetMessage(MSG_THING_FOUND_DEFINITELY_SAME_AS);
    }
    Event_ShowMessage(25, 0);
    Event_End();
}

void FieldScene_RunScene3b6_02000898(s32 a0)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_GRRR_SCAM_WHY_WONT_THEY);
    Actor_ShowEmote(31, 0x103, 40);
    Event_ShowMessage(a0, 0);
    Event_End();
}
