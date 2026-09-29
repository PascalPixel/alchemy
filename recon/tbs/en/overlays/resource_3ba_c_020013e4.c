#include "TYPES.H"
extern u8 MsgKorosseoAreaCalledPipeworks[];
extern u8 MsgKorosseoObjectiveMakeGood[];

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3ba:020013e4):
 *   Local_02001b5c = 0x02009b5c (thumb)
 *   Engine_EventBegin = 0x0200bca0 (thumb)
 *   SceneDialogue_RunFlagGatedPromptInteraction = 0x02009d64 (thumb)
 *   Engine_EventSetMessage = 0x0200bd30 (thumb)
 *   Engine_CameraSetSpeed = 0x0200bd70 (thumb)
 *   Engine_CameraMoveTo = 0x0200bd78 (thumb)
 *   Engine_CameraWaitForMove = 0x0200bd80 (thumb)
 *   Engine_EventWait = 0x0200bc98 (thumb)
 *   Engine_EventShowMessage = 0x0200bd40 (thumb)
 *   SceneState_StoreParamsAndInitTable = 0x0200ad28 (thumb)
 *   SceneState_InitTableWordsAndLoad3200 = 0x0200ad8c (thumb)
 *   SceneState_ReleaseTableAndResetC6a6 = 0x0200ade8 (thumb)
 *   Engine_TaskWait = 0x0200bb08 (thumb)
 *   Engine_ActorGet = 0x0200bcb8 (thumb)
 *   Engine_ObjectSetPosition = 0x0200bbd8 (thumb)
 *   Engine_ObjectCommitPosition = 0x0200bbe0 (thumb)
 *   Engine_CameraFollowActor = 0x0200bd68 (thumb)
 *   SceneState_SendIdBySceneId = 0x02009e20 (thumb)
 *   FieldScene_RunMiddleSequence = 0x02009e7c (thumb)
 *   Engine_EventEnd = 0x0200bca8 (thumb)
 */

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

/* NONMATCHING: 504 of 508 bytes, 73 differing halfwords, 32 aligned edits
 * (2026-09-26). Whole owner 020013e4..020015e0; the pool starts at 020015cc
 * and includes the final 0x208f message word. Calls and pool audited against
 * our own ROM. The prompt result lives in sl and speed 0xcccc in r6 in the
 * reference; the retained candidate swaps them. Literal zeros and a word
 * speed temporary had already failed on 2026-09-24.
 *
 * Bounded ownership trial: FIELD_EVENT.H, typed FieldActor pointers/fields
 * and pointer-returning ActorGet produced 504 bytes, 88 differing halfwords,
 * 58 aligned edits. It moved speaker/actor/acceleration allocations as well,
 * without correcting result/speed lifetime. Rejected; best baseline retained.
 * No declaration permutation or register spelling sweep. Reopen only with a
 * new result/speed lifetime hypothesis, not the same typed-field conversion.
 * 2026-09-27 result-view hypothesis: one union keeps the full decision for
 * the branch/tail and a byte view for the four motion stores. Fresh complete
 * normalized diff is unchanged: 504/508 bytes, 73 halfwords / 32 edits.
 * It emits byte-identical assembly to the scalar baseline; the decision
 * remains r6 and the speed sl. Width separation creates no new lifetime.
 * Rejected witness is preserved at 9e5ca58a8; scalar source restored here.
 * No function or alignment credit; stop the result-view axis here.
 */
void Korosseo_RunPromptMotionSequence(s32 a0)
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
            Call1(Engine_EventSetMessage, (s32)MsgKorosseoAreaCalledPipeworks);
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
            Call1(Engine_EventSetMessage, (s32)MsgKorosseoObjectiveMakeGood);
            Engine_EventShowMessage(a0, 0);
        }
        L_020015b0:;
        Value3(FieldScene_RunMiddleSequence, rec4, a0, 2);
        Engine_EventEnd();
    }
}
