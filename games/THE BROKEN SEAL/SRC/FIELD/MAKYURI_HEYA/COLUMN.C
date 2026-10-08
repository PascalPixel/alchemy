#include "PROBE.H"
#include "TYPES.H"
#include "MAKYURI.H"

struct BlockProbe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct BlockProbe *probe);
void SceneActor_MoveAndRedraw(struct BlockProbe probe);

void SceneActor_RunActorZeroHandledMotion(s32 a)
{
    u8 *v = Object_GetById(0);
    Engine_EventBegin();
    Engine_AudioPlayCue(0xe4);
    FIELD_AT_OFFSET(v, s32, 0x6c) = (s32)MakyuriHeya_TrailSparks;
    FIELD_AT_OFFSET(v, s32, 0x30) = 0x3333;
    Engine_ActorSetAnimation(0, 2);
    Engine_ActorSetDestinationOffset(0, 0, -6);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    FIELD_AT_OFFSET(v, s32, 0x6c) = 0;
    Engine_EventWait(30);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(a);
    Engine_EventEnd();
}

void MakyuriHeya_DropLeaderToColumn(s32 a0)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_AudioPlayCue(228);
    Actor_SetSpeed(0, 0x6666, 0x3333);
    Engine_ActorSetSpritePriority(0, 2);
    Engine_ActorSetDestinationOffset(0, 0, -8);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_EventWait(8);
    Actor_SetPosition(0, ((a0 << 19) + 0x80000), 0);
    Engine_EventWait(30);
}

void MakyuriHeya_RunExitThreeScene(void)
{
    SceneActor_RunActorZeroHandledMotion(gEventWork->touched_trigger - 50);
}

void MakyuriHeya_RunExitThirteenScene(void)
{
    SceneActor_RunActorZeroHandledMotion(gEventWork->touched_trigger - 50);
}

void MakyuriHeya_RunExitFiveScene(void)
{
    SceneActor_RunActorZeroHandledMotion(gEventWork->touched_trigger - 50);
}

void MakyuriHeya_ExitWhenChannelsOpen(void)
{
    if (GameFlag_IsSet(0x310) != 0
        && GameFlag_IsSet(0x311) != 0
        && GameFlag_IsSet(0x312) != 0) {
        GameFlag_Set(0x876);
        Engine_EventWait(30);
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        Engine_AudioPlayCue(141);
        Engine_EventWait(60);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Engine_AudioPlayCue(0x121);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_MapRenderWaitForValues();
        Engine_EventRequestExit(13);
    } else {
        GameFlag_Clear(0x876);
    }
}

void MakyuriHeya_RunPushedBlockScene(void)
{
    struct BlockProbe probe;
    s32 x;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) == 0)
        goto end;
    switch ((u32)probe.word[1]) {
    case 9:
        if (probe.word[4] >> 20 == 8) {
            SceneActor_MoveAndRedraw(probe);
            Engine_EventWait(20);
            Map_CopyCellsTo(119, 9, 109, 11, 1, 1);
            SceneEffect_SpawnRandomizedBurst(0x2d60000, 0, 0xb40000, 0x8000);
            GameFlag_Set(0x310);
        } else {
            s32 one = 1;

            Map_CopyCellsTo(117, 9, 104, 7, one, one);
            Map_CopyCellsTo(119, 8, 109, 11, one, one);
            Map_CopyCellsTo(118, 8, 104, 13, one, one);
            SceneActor_MoveAndRedraw(probe);
            GameFlag_Clear(0x310);
        }
        break;
    case 10:
        if (probe.word[4] >> 20 == 12) {
            SceneActor_MoveAndRedraw(probe);
            Engine_EventWait(10);
            if (GameFlag_IsSet(0x310)) {
                Map_CopyCellsTo(118, 9, 104, 13, 1, 1);
                SceneEffect_SpawnRandomizedBurst(0x2840000, 0, 0xd20000, 0x4000);
            }
            GameFlag_Set(0x311);
        } else {
            s32 one = 1;

            Map_CopyCellsTo(119, 8, 109, 11, one, one);
            if (GameFlag_IsSet(0x310)) {
                Map_CopyCellsTo(119, 9, 109, 11, one, one);
                Map_CopyCellsTo(118, 8, 104, 13, one, one);
            }
            SceneActor_MoveAndRedraw(probe);
            GameFlag_Clear(0x311);
        }
        break;
    case 11:
        x = probe.word[2];
        if (x >> 20 == 40) {
            SceneActor_MoveAndRedraw(probe);
            GameFlag_Set(0x312);
        } else {
            SceneActor_MoveAndRedraw(probe);
            GameFlag_Clear(0x312);
        }
        break;
    }
    MakyuriHeya_ExitWhenChannelsOpen();
end:
    Engine_EventEnd();
}
