/* The Lunpa fortress: two actors' lines. */
#include "FORTRESS.H"
extern u8 MsgRunpaGuysTougherThought[];
extern u8 MsgRunpaKnowWhereDodonpa[];

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
