/* Flagged lines. */
#include "TOREBI.H"
extern u8 MsgTorebiGrrrScamWhyWontThey[];
extern u8 MsgTorebiHehHehSheJustHid[];
extern u8 MsgTorebiThingFoundDefinitelySameAs[];

void SceneDialogue_RunActor25FlaggedLine(void)
{
    void Event_Begin(void);

    Event_Begin();
    if (GameFlag_IsSet(0x8BE) == 0) {
        Event_SetMessage((s32)MsgTorebiHehHehSheJustHid);
    } else {
        Event_SetMessage((s32)MsgTorebiThingFoundDefinitelySameAs);
    }
    Event_ShowMessage(25, 0);
    Event_End();
}

void FieldScene_RunScene3b6_02000898(s32 a0)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgTorebiGrrrScamWhyWontThey);
    Actor_ShowEmote(31, 0x103, 40);
    Event_ShowMessage(a0, 0);
    Event_End();
}
