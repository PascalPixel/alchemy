#include "TYPES.H"

void Local_02001b5c();
void Engine_EventBegin();
s32 SceneDialogue_RunFlagGatedPromptInteraction();
void Engine_EventSetMessage();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventWait();
void Engine_EventShowMessage();
void SceneState_StoreParamsAndInitTable();
void SceneState_InitTableWordsAndLoad3200();
void SceneState_ReleaseTableAndResetC6a6();
void Engine_TaskWait();
s32 Engine_ActorGet();
void Engine_ObjectSetPosition();
void Engine_ObjectCommitPosition();
void Engine_CameraFollowActor();
void SceneState_SendIdBySceneId();
s32 FieldScene_RunMiddleSequence();
void Engine_EventEnd();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
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

/* NONMATCHING: 508 of 508 bytes, 32 halfword edits (2026-09-24). The prompt
 * result (zero on this path) lives in sl and the 0xcccc speed constant in r6
 * in the reference; here the two allocations are swapped. Literal zeros and a
 * word temporary for the speed moved nothing. */
void Func_020013e4(s32 a0)
{
    u32 i;
    s32 rec4;
    u8 *rec7;
    u8 *record;

    if (Data_02000240_t[225][0] == 2) {
        Local_02001b5c();
    } else {
        Engine_EventBegin();
        rec4 = Value2(SceneDialogue_RunFlagGatedPromptInteraction, a0, 2);
        if (rec4 != 0) {
        } else {
            Call1(Engine_EventSetMessage, 0x2090);
            Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
            Call4(Engine_CameraMoveTo, 0x2500000, -1, 0x780000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(60);
            Call2(Engine_CameraSetSpeed, 0x18000, 0x3000);
            Call4(Engine_CameraMoveTo, 0x2600000, -1, 0xd80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            SceneState_StoreParamsAndInitTable(56, 64, 0);
            Engine_EventWait(60);
            SceneState_InitTableWordsAndLoad3200(160, 96, 10);
            Engine_EventWait(70);
            Engine_EventShowMessage(a0, 0);
            SceneState_ReleaseTableAndResetC6a6();
            Engine_TaskWait(2);
            record = Value1(Engine_ActorGet, 13);
            record[85] = rec4;
            *(s32 *)((s32)record + 52) = 0x6666;
            *(s32 *)((s32)record + 48) = 0xcccc;
            Call4(Engine_ObjectSetPosition, (s32)record, *(s32 *)((s32)record + 8), 0x80000, *(s32 *)((s32)record + 16));
            rec7 = Value1(Engine_ActorGet, 14);
            rec7[85] = rec4;
            *(s32 *)((s32)rec7 + 52) = 0x6666;
            *(s32 *)((s32)rec7 + 48) = 0xcccc;
            Call4(Engine_ObjectSetPosition, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x200000, *(s32 *)((s32)rec7 + 16));
            Engine_ObjectCommitPosition((s32)rec7);
            Engine_EventWait(45);
            record = Value1(Engine_ActorGet, 13);
            record[85] = rec4;
            *(s32 *)((s32)record + 52) = 0x6666;
            *(s32 *)((s32)record + 48) = 0xcccc;
            Call4(Engine_ObjectSetPosition, (s32)record, *(s32 *)((s32)record + 8), 0x180000, *(s32 *)((s32)record + 16));
            rec7 = Value1(Engine_ActorGet, 14);
            rec7[85] = rec4;
            *(s32 *)((s32)rec7 + 52) = 0x6666;
            *(s32 *)((s32)rec7 + 48) = 0xcccc;
            Engine_ObjectSetPosition((s32)rec7, *(s32 *)((s32)rec7 + 8), 0, *(s32 *)((s32)rec7 + 16));
            Engine_ObjectCommitPosition((s32)rec7);
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
            goto L_020015b0;
        }
        if (rec4 == 1) {
            Call1(Engine_EventSetMessage, 0x208f);
            Engine_EventShowMessage(a0, 0);
        }
        L_020015b0:;
        Value3(FieldScene_RunMiddleSequence, rec4, a0, 2);
        Engine_EventEnd();
    }
}
