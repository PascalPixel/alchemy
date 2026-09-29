#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgKorosseoAreaCalledPipeworks[];
extern u8 MsgKorosseoObjectiveMakeGood[];

void Korosseo_FinishSoloRound();
s32 SceneDialogue_RunFlagGatedPromptInteraction();
void SceneState_StoreParamsAndInitTable();
void SceneState_InitTableWordsAndLoad3200();
void SceneState_ReleaseTableAndResetC6a6();
void Engine_ObjectSetPosition();
void Script_WaitForEventTimeout();
void SceneState_SendIdBySceneId();
s32 FieldScene_RunMiddleSequence();

/* FAKEMATCH: call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The Pipeworks stage: actor a0 shows the course while actors 13 and 14 are
 * raised and lowered twice, then the stage opens. After a solo round the
 * question whether the party is done cheering comes instead. */
void Korosseo_RunPipeworksIntro(s32 a0)
{
    s32 rec4;
    u8 *rec7;
    u8 *record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec4 = Value2(SceneDialogue_RunFlagGatedPromptInteraction, a0, 2);
        if (rec4 != 0) {
        } else {
            Call1((void (*)())Engine_EventSetMessage, (s32)MsgKorosseoAreaCalledPipeworks);
            Call2((void (*)())Engine_CameraSetSpeed, 0x30000, 0x6000);
            Call4((void (*)())Engine_CameraMoveTo, 0x2500000, -1, 0x780000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(60);
            Call2((void (*)())Engine_CameraSetSpeed, 0x18000, 0x3000);
            Call4((void (*)())Engine_CameraMoveTo, 0x2600000, -1, 0xd80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            SceneState_StoreParamsAndInitTable(56, 64, 0);
            Engine_EventWait(60);
            SceneState_InitTableWordsAndLoad3200(160, 96, 10);
            Engine_EventWait(70);
            Engine_EventShowMessage(a0, 0);
            SceneState_ReleaseTableAndResetC6a6();
            Engine_TaskWait(2);
            record = (u8 *)Value1((s32 (*)())Engine_ActorGet, 13);
            record[85] = rec4;
            *(s32 *)((s32)record + 52) = 0x6666;
            *(s32 *)((s32)record + 48) = 0xcccc;
            Call4(Engine_ObjectSetPosition, (s32)record, *(s32 *)((s32)record + 8), 0x80000, *(s32 *)((s32)record + 16));
            rec7 = (u8 *)Value1((s32 (*)())Engine_ActorGet, 14);
            rec7[85] = rec4;
            *(s32 *)((s32)rec7 + 52) = 0x6666;
            *(s32 *)((s32)rec7 + 48) = 0xcccc;
            Call4(Engine_ObjectSetPosition, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x200000, *(s32 *)((s32)rec7 + 16));
            Script_WaitForEventTimeout((s32)rec7);
            Engine_EventWait(45);
            record = (u8 *)Value1((s32 (*)())Engine_ActorGet, 13);
            record[85] = rec4;
            *(s32 *)((s32)record + 52) = 0x6666;
            *(s32 *)((s32)record + 48) = 0xcccc;
            Call4(Engine_ObjectSetPosition, (s32)record, *(s32 *)((s32)record + 8), 0x180000, *(s32 *)((s32)record + 16));
            rec7 = (u8 *)Value1((s32 (*)())Engine_ActorGet, 14);
            rec7[85] = rec4;
            *(s32 *)((s32)rec7 + 52) = 0x6666;
            *(s32 *)((s32)rec7 + 48) = 0xcccc;
            Engine_ObjectSetPosition((s32)rec7, *(s32 *)((s32)rec7 + 8), 0, *(s32 *)((s32)rec7 + 16));
            Script_WaitForEventTimeout((s32)rec7);
            Engine_EventWait(15);
            Engine_EventShowMessage(a0, 0);
            SceneState_StoreParamsAndInitTable(56, 64, 0);
            Engine_EventWait(30);
            SceneState_InitTableWordsAndLoad3200(160, 96, 10);
            Engine_EventWait(40);
            SceneState_InitTableWordsAndLoad3200(56, 64, 10);
            Engine_EventWait(70);
            Engine_EventShowMessage(a0, 0);
            SceneState_ReleaseTableAndResetC6a6();
            Engine_TaskWait(2);
            Engine_CameraFollowActor(0, 0);
            SceneState_SendIdBySceneId(a0, 2);
            goto done;
        }
        if (rec4 == 1) {
            Call1((void (*)())Engine_EventSetMessage, (s32)MsgKorosseoObjectiveMakeGood);
            Engine_EventShowMessage(a0, 0);
        }
    done:
        Value3(FieldScene_RunMiddleSequence, rec4, a0, 2);
        Engine_EventEnd();
    }
}
