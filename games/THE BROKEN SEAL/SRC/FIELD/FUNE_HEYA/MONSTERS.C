/* Actor 8 hears of monsters aboard the ship. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgFuneMonsters[];

void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);

void FuneHeya_AskAboutMonsters(void)
{
    Event_Begin();
    FieldScene_RunSceneStep(15, 1, 1);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(10);
    Actor_FaceDirection(8, 0x3000, 20);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgFuneMonsters);
    Event_ShowMessageAndWait(8, 0, 20);
    FieldScene_RunSceneStep(9, 14, 0);
}
