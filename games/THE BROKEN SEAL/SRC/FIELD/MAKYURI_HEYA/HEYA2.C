#include "PROBE.H"
#include "TYPES.H"
#include "MAKYURI.H"

struct BlockProbe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct BlockProbe *probe);
void SceneActor_MoveAndRedraw(struct BlockProbe probe);
void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w);
void MakyuriHeya_ExitWhenChannelsOpen(void);

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

/* Mercury Lighthouse: after a block is pushed, open or close the water
 * cells it controls. Actor 9 in row 8 opens the first channel (flag 0x310),
 * actor 10 in row 12 the second (0x311, with a burst when the first is
 * open), and actor 11 in column 40 sets 0x312. */
void MakyuriHeya_RunPushedBlockScene(void)
{
    struct BlockProbe probe;
    s32 x;

    Event_Begin();
    if (StagedActor_FindClearPosition(&probe) == 0)
        goto end;
    switch ((u32)probe.word[1]) {
    case 9:
        if (probe.word[4] >> 20 == 8) {
            SceneActor_MoveAndRedraw(probe);
            Event_Wait(20);
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
            Event_Wait(10);
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
    Event_End();
}
