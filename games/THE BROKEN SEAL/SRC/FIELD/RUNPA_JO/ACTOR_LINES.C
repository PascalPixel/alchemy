/* The Lunpa fortress: three actors' lines. */
#include "FORTRESS.H"
extern u8 MsgRunpaGuysTougherThought[];
extern u8 MsgRunpaKnowWhereDodonpa[];
extern u8 MsgRunpaRightRightGive[];

void ConfigureSceneActor13(void)
{
    Actor_RunRepeatedMotion(13, 2);
    Event_SetMessage((s32)MsgRunpaRightRightGive);
    Event_ShowMessage(13, 0);
}

void ConfigureSceneActor12Variant(void)
{
    Actor_RunRepeatedMotion(12, 2);
    Event_SetMessage((s32)MsgRunpaGuysTougherThought);
    Event_ShowMessage(12, 0);
}

void ConfigureSceneActor18(void)
{
    Event_SetMessage((s32)MsgRunpaKnowWhereDodonpa);
    Event_AskYesNo(18, 0);
}
