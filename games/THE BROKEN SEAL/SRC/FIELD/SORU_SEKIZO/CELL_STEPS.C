#include "STATUE_HALL.H"

void FieldScene_RunScene37bSequenceA(void)
{
    u32 i;
    s32 record;

    record = Value1(Engine_ActorGet, 17);
    if (record != 0) {
        if ((*(s32 *)(record + 16) >> 20) == 8) {
            Event_Begin();
            Audio_PlayCue(185);
            Actor_SetSpeed(17, 0x3333, 0x1999);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
            *(u8 *)((s32)Engine_ActorGet(17) + 90) &= 254;
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
            record = Engine_ActorGet(0);
            Actor_SetDestination(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), 136);
            Actor_SetDestination(17, 0x120, 120);
            Actor_WaitForMove(17);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Event_End();
        }
    }
}

void FieldScene_RunFiveValueStep9(void)
{
    SoruSekizo_RunEventSequence(9, 31, 9, 30, 9);
    SceneData_InitTableA980();
}

void FieldScene_RunFiveValueStep11(void)
{
    SoruSekizo_RunEventSequence(11, 40, 9, 41, 9);
    SceneData_FillTableA980();
}

void FieldScene_ApplyRect13_31_12_30_12(void)
{
    SoruSekizo_RunEventSequence(13, 31, 12, 30, 12);
    SceneData_InitTableA980AndRunB();
}

void FieldScene_RunFiveValueStep15(void)
{
    SoruSekizo_RunEventSequence(15, 40, 12, 41, 12);
    SceneData_BuildTableA980();
}

void FieldScene_ApplyRect10_14_7_13_7(void)
{
    SoruSekizo_RunEventSequence(10, 14, 7, 13, 7);
    Scene_ShineLeftBeam();
}

/*
 * Fetches scene record 10 and, when it exists, hands a coarse coordinate
 * derived from it to a five-argument routine, which receives both the
 * coordinate and the coordinate plus one; the fifth argument travels on the
 * stack. The `>> 20` reduction to a cell index is by analogy with the rest of
 * the tree and is not verified, and the repeated 13 is as written.
 */
void SceneActor_UseActorTenCellAndNext(void)
{
    u8 *record = Engine_ActorGet(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = *(s32 *)(record + 16) >> 20;
    SoruSekizo_RunEventSequence(10, 13, cell + 1, 13, cell);
}

void SceneActor_MoveActor10ByRow(void)
{
    s32 *p = Engine_ActorGet(10);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        SoruSekizo_RunEventSequence(10, 13, v - 1, 13, v);
    }
}

void FieldScene_ApplyRect12_21_7_22_7(void)
{
    SoruSekizo_RunEventSequence(12, 21, 7, 22, 7);
    Scene_ShineRightBeam();
}

void SceneActor_ApplyActorTwelveZCellPair(void)
{
    s32 *p = Engine_ActorGet(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        SoruSekizo_RunEventSequence(12, 22, v + 1, 22, v);
    }
}

void SceneActor_RunSlot12ColumnStep(void)
{
    s32 *p = Engine_ActorGet(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        SoruSekizo_RunEventSequence(12, 22, v - 1, 22, v);
    }
}
