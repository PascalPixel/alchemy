#include "PROBE.H"

void SceneActor_RunActorZeroHandledMotion(s32 a)
{
    u8 *v = Actor_Get(0);
    Event_Begin();
    Audio_PlayCue(0xe4);
    FIELD_AT_OFFSET(v, s32, 0x6c) = (s32)MakyuriHeya_TrailSparks;
    FIELD_AT_OFFSET(v, s32, 0x30) = 0x3333;
    Actor_SetAnimation(0, 2);
    Actor_SetDestinationOffset(0, 0, -6);
    Actor_WaitForMove(0);
    Actor_SetChildValue(0, 15);
    Actor_SetSpriteFlags(Actor_Get(0), 0);
    FIELD_AT_OFFSET(v, s32, 0x6c) = 0;
    Event_Wait(30);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(a);
    Event_End();
}

void MakyuriHeya_DropLeaderToColumn(s32 a0)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(228);
    Actor_SetSpeed(0, 0x6666, 0x3333);
    Actor_SetSpritePriority(0, 2);
    Actor_SetDestinationOffset(0, 0, -8);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Event_Wait(8);
    Actor_SetPosition(0, ((a0 << 19) + 0x80000), 0);
    Event_Wait(30);
}

void SceneState_ApplyWork16cMinus50A(void)
{
    SceneActor_RunActorZeroHandledMotion(*(s16 *)((u8 *)gEventWork + 0x16c) - 50);
}

void SceneState_ApplyWork16cMinus50(void)
{
    SceneActor_RunActorZeroHandledMotion(*(s16 *)((u8 *)gEventWork + 0x16c) - 50);
}

void SceneState_ApplyWork16cMinus50B(void)
{
    SceneActor_RunActorZeroHandledMotion(*(s16 *)((u8 *)gEventWork + 0x16c) - 50);
}

void MakyuriHeya_ExitWhenChannelsOpen(void)
{
    if (GameFlag_IsSet(0x310) != 0
        && GameFlag_IsSet(0x311) != 0
        && GameFlag_IsSet(0x312) != 0) {
        GameFlag_Set(0x876);
        Event_Wait(30);
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        Audio_PlayCue(141);
        Event_Wait(60);
        *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x100;
        Event_CloseScreen();
        Event_WaitForScreen();
        Audio_PlayCue(0x121);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        MapRender_WaitForValues();
        Event_RequestExit(13);
    } else {
        GameFlag_Clear(0x876);
    }
}
