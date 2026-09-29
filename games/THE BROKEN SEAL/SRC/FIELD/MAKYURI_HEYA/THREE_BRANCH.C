#include "PROBE.H"

void SceneState_ApplyRectWhenActor20AtColumn28(void)
{
    s32 col;

    Event_Begin();
    col = Object_GetById(20)->x.fixed / 0x100000;
    if (col == 28) {
        GameFlag_Set(840);
        {
            s32 a = 31;
            s32 b = 20;

            Map_CopyCellAttributes(29, 20, 1, 1, a, b);
        }
    }
    Event_End();
}

void SceneEffect_RotatePaletteEntries40To47(void)
{
    unsigned int index;
    u16 *dst;
    u16 *src;
    u32 front;

    if ((*(volatile u32 *)&gFrameCount & 7) != 0) {
        return;
    }

    dst = (u16 *)0x05000050;
    front = *dst;
    index = 0;
    *(u16 *)0x0500005e = front;

    src = (u16 *)0x05000052;
    while (index <= 6) {
        *dst++ = *src++;
        index++;
    }
}

void FieldScene_RunActorThreeBranchSequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(3, FX16_0_8, FX16_0_4);
    Actor_SetSpeed(0, FX16_0_8, FX16_0_4);
    Event_SetMessage(MSG_SOMEBODY_HERE);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_WalkToAndWait(3, 0x348, 0x288);
    Actor_ShowEmote(3, 0x100, 60);
    Actor_FaceDirection(3, FX16_0_5, 20);
    Actor_SetAnimation(3, 16);
    record = (s32)Object_GetById(3);
    /* Set the +24 field of actor 3's record to -1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = -FX16_1_0;
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_SetAnimation(3, 1);
    record = (s32)Object_GetById(3);
    /* Set the +24 field of actor 3's record to 1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = FX16_1_0;
    Event_Wait(20);
    Actor_FaceDirection(3, FX16_0_25, 20);
    Event_OpenMessage(3, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Event_ShowMessageAndWait(3, 0, 20);
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
        Event_Wait(20);
        Actor_SetAnimationAndWait(3, 4);
        Event_ShowMessageAndWait(3, 0, 20);
    }
    Event_Wait(20);
    Actor_FaceDirection(3, FX16_0_75, 20);
    Camera_SetSpeed(FX16_0_8, FX16_0_1);
    Camera_MoveTo(0x3480000, -1, 0x2780000, 1);
    Actor_WalkToAndWait(3, 0x348, 0x278);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(10);
    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Call1((void (*)())Engine_GameFlagSet, 0x870);
    Event_End();
}

void SceneDialogue_RunActor3TimedLine(void)
{
    Event_Begin();
    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_SetMessage(MSG_STATUE_BLOCKING_ENTRANCE);
    Event_ShowMessageAndWait(3, 0, 20);
    Event_End();
}
