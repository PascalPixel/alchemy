#include "ENTRANCE.H"
#include "CALL.H"
extern u8 MsgMakyuriWhoHonorsHeart[];

void FieldScene_Forward31d4(void)
{
    Makyuri_TickSpawnTimer();
}

void FieldScene_CallHelper2e58(void)
{
    SceneState_ClearCurrentRecordAndReleaseTarget();
}

void FieldScene_RunSingleStep(void)
{
    Makyuri_RunActorMove();
}

void FieldScene_CallHelper2d50(void)
{
    MakyuriIriguchi_RaisePillar();
}

/* Contiguous unnamed leaf-owner run for resource_39b. */
void *SceneData_GetSceneTableA(void) { return MakyuriIriguchi_SceneTableA; }

int SceneData_ReturnZero(void) { return 0; }

void *SceneData_GetSceneTableB(void) { return MakyuriIriguchi_SceneTableB; }

void *SceneData_GetSceneTableC(void) { return MakyuriIriguchi_SceneTableC; }

/* Apply the overlay's common actor-0 presentation preset. */
void FieldScene_RunStepWithValue1632(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgMakyuriWhoHonorsHeart, 1);
    Event_End();
}

void SceneActor_RunActorZeroHandledMotion(s32 a)
{
    u8 *v = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Audio_PlayCue(0xe4);
    F(v, s32, 0x6c) = (s32)MakyuriIriguchi_TrailSparks;
    F(v, s32, 0x30) = 0x3333;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    F(v, s32, 0x6c) = 0;
    Event_Wait(30);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(a);
    Event_End();
}

void MakyuriIriguchi_DropLeaderToColumn(s32 a0)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(228);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    Event_Wait(8);
    Actor_SetPosition(ACTOR_PARTY_LEADER, ((a0 << 19) + 0x80000), 0);
    Event_Wait(30);
}

/* Contiguous unnamed leaf-owner run for resource_39b. */

/* Clear the scene flag and point actor 8 at its first local path. */
void FieldScene_RunIndexedStep17(void)
{
    SceneActor_RunActorZeroHandledMotion(17);
}

void FieldScene_RunIndexedStep18(void)
{
    SceneActor_RunActorZeroHandledMotion(18);
}

void FieldScene_RunIndexedStep19(void)
{
    SceneActor_RunActorZeroHandledMotion(19);
}

void *SceneData_GetSceneTableD(void) { return MakyuriIriguchi_SceneTableD; }

void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 a;
    s32 b;
    s32 zero;
    s32 counter;
    s32 x;
    s32 y;
    s32 t;
    s32 record;
    u8 *slot;
    u8 slot16[40];

    a = *(s32 *)(Value1(Object_GetById, ACTOR_PARTY_LEADER) + 8) / 0x100000;
    b = *(s32 *)(Value1(Object_GetById, ACTOR_PARTY_LEADER) + 16) / 0x100000;
    if (a == 12 && b == 32) {
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(120);
        ColorBuffer_ApplyTarget(0x10005, 1);
        ColorBuffer_Interpolate(60);
        Event_Wait(40);
        counter = 0;
        slot = slot16;
        zero = 0;
        do {
            *(s32 *)(slot) = 1;
            {
                s32 shown = 0x11e;

                *(u16 *)(slot + 24) = shown;
            }
            *(s32 *)(slot + 28) = (s32)MakyuriIriguchi_SparkBurstScript;
            Audio_PlayCue(246);
            x = 208 - ((u32)(Random_Next() << 4) >> 16);
            y = 560 - ((u32)(Random_Next() << 4) >> 16);
            t = ((u32)(Engine_RandomNext() << 2) >> 16);
            record = Math_Divide((((t << 4) - t) << 16) + 0x3c0000, 100);
            Effect_Spawn(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
            Event_Wait(4);
            counter = counter + 1;
        } while ((u32)counter <= 14);
        Audio_PlayCue(220);
        Event_Wait(60);
        GameFlag_Set(0x875);
        Engine_TaskAddCallback((s32)Makyuri_CyclePalette, 0xc80);
        Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
        Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
        ColorBuffer_ApplyTarget(0x10000, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(120);
        Event_End();
    }
}

void FieldScene_RunIndexedStep63(void)
{
    Event_RequestExit(63);
}

void FieldScene_RunActor8StepWithTableA820(void)
{
    GameFlag_Clear(0x205);
    Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path1);
}

void MakyuriIriguchi_SendActor8ByLeaderColumn(void)
{
    s32 record;
    s32 field8;
    s32 quotient;

    record = Object_GetById(ACTOR_PARTY_LEADER);
    field8 = *(s32 *)(record + 8);
    quotient = field8 / 0x100000;
    GameFlag_Set(0x205);
    if (quotient == 7) {
        Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path2);
    } else {
        Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path3);
    }
}
