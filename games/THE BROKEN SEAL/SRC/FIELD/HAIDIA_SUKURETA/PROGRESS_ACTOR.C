#include "SUKURETA.H"
extern u8 MsgHaidiaMemoriesOfThisCottage[];

void SceneState_SetWorkAndFlag87d(void)
{
    u8 *work;

    Event_Begin();

    work = *(u8 **)&gEventWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    GameFlag_Set(0x87D);
    BattleFx_SetWeightedResult(12, 0);
    GameFlag_Set(0x900);   /* 144 << 4 */
    Event_End();
}

void SceneState_SetWorkAndFlag87e(void)
{
    u8 *work;

    Event_Begin();

    work = *(u8 **)&gEventWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    GameFlag_Set(0x87E);
    BattleFx_SetWeightedResult(12, 1);
    GameFlag_Set(0x900);   /* 144 << 4 */
    Event_End();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaMemoriesOfThisCottage);
    Event_AskYesNo(16, 0);
    Event_End();
}
