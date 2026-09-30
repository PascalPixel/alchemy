#include "TYPES.H"
#include "CALL.H"
#include "STATUE_HALL.H"

void SetMapCellCollision();

extern u8 MsgSoruSomethingClicked[];
void Engine_EventBegin();
s32 Engine_GameFlagIsSet();
void Engine_ActorWalkToAndWait();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorSetSpritePriority();
void Engine_AudioPlayCue();
void Engine_ActorSetDestination();
void Engine_ActorSetPosition();
void Engine_WorkSetValuesIfNonNegative();
void Engine_MessageShowCentered();
void Engine_MapCopyCellAttributes();
void Engine_EventEnd();

void Engine_ActorSetAnimation();
void Engine_ActorWaitForMove();

void SoruSekizo_CheckTileTrigger0166C(void)
{
    s32 x = *(s32 *)((s32)Engine_ActorGet(0) + 8) >> 20;
    s32 y = *(s32 *)((s32)Engine_ActorGet(0) + 16) >> 20;

    if (y == 7 && (u32)(x - 13) <= 1) {
        Call4(SetMapCellCollision, 2, 0xd00000, 0x700000, 255);
    }
}

void SoruSekizo_CheckTileTrigger016A4(void)
{
    s32 x = *(s32 *)((s32)Engine_ActorGet(0) + 8) >> 20;
    s32 y = *(s32 *)((s32)Engine_ActorGet(0) + 16) >> 20;

    if (y == 7 && (u32)(x - 21) <= 1) {
        Call4(SetMapCellCollision, 2, 0x1600000, 0x700000, 255);
    }
}

void SoruSekizo_RunStatueDropScene(void)
{
    u32 i;
    u8 *rec8;
    s32 record;
    u8 *p5;
    s32 zero;

    rec8 = (u8 *)Engine_ActorGet(17);
    Call4(SetMapCellCollision, 2, 0x1100000, 0x800000, 0);
    Call4(SetMapCellCollision, 2, 0x1200000, 0x800000, 0);
    if ((s32)rec8 == 0) {
    } else {
        p5 = *(s32 *)((s32)rec8 + 16);
        Engine_EventBegin();
        if (((s32)p5 >> 20) != 8) {
        } else {
            if (Engine_GameFlagIsSet(0x207) == 0) {
                record = (u8 *)Engine_ActorGet(0);
                if ((u32)(*(s32 *)(record + 16) >> 19) <= 17) {
                    Call3(Engine_ActorWalkToAndWait, 0, 0x121, 158);
                    record = (u8 *)Engine_ActorGet(0);
                    {
                        s32 shown = 0xc000;
                    
                        *(u16 *)(record + 6) = shown;
                    }
                }
            }
            if (Engine_GameFlagIsSet(0x816) == 0) {
            } else {
                if (Engine_GameFlagIsSet(0x817) == 0) {
                } else {
                    Engine_GameFlagSet(0x818);
                    Call2(Engine_CameraSetSpeed, 0x20000, 0x4000);
                    Call4(Engine_CameraMoveTo, 0x11e0000, -1, 0x920000, 1);
                    Engine_CameraWaitForMove();
                    *(u8 *)((u8 *)Engine_ActorGet(17) + 90) &= 254;
                    Value3(Engine_ActorSetSpeed, 17, 0x30000, 0x10000);
                    zero = 0;
                    rec8[85] = zero;
                    Engine_ActorSetSpritePriority(17, 3);
                    Engine_AudioPlayCue(189);
                    Engine_ActorSetDestination(17, 0x120, 178);
                    ((void (*)())Engine_EventWait)(8);
                    Call3(Engine_ActorSetPosition, 18, 0x1200000, 0xb20000);
                    *(s32 *)((s32)rec8 + 56) = -0x80000000;
                    *(s32 *)((s32)rec8 + 60) = -0x80000000;
                    *(s32 *)((s32)rec8 + 64) = -0x80000000;
                    *(s32 *)((s32)rec8 + 8) = zero;
                    *(s32 *)((s32)rec8 + 12) = zero;
                    *(s32 *)((s32)rec8 + 16) = zero;
                    *(s32 *)((s32)rec8 + 36) = zero;
                    *(s32 *)((s32)rec8 + 40) = zero;
                    *(s32 *)((s32)rec8 + 44) = zero;
                    Engine_ActorSetPosition(17, 0, 0);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
                    Engine_EventWait(10);
                    Engine_AudioPlayCue(141);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
                    Engine_EventWait(10);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
                    Engine_EventWait(35);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
                    Engine_EventWait(20);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
                    Engine_EventWait(30);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
                    Engine_EventWait(40);
                    Engine_AudioPlayCue(0x121);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
                    Engine_EventWait(10);
                    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
                    Engine_EventWait(60);
                    Engine_AudioPlayCue(188);
                    if (Engine_GameFlagIsSet(0x80b) != 0) {
                        if (Engine_GameFlagIsSet(0x80c) != 0) {
                            if (Engine_GameFlagIsSet(0x80d) != 0) {
                                if (Engine_GameFlagIsSet(0x80e) != 0) {
                                    Engine_GameFlagSet(0x80f);
                                }
                            }
                        }
                    }
                    Engine_EventWait(40);
                    Engine_MessageShowCentered((s32)MsgSoruSomethingClicked, 1);
                    Engine_MapCopyCellAttributes(0, 1, 2, 1, 17, 8);
                    Engine_MapCopyCellAttributes(17, 9, 2, 1, 17, 7);
                }
            }
        }
        Engine_EventEnd();
    }
}

void FieldScene_RunScene37bSequenceA(void)
{
    u32 i;
    s32 record;

    record = Engine_ActorGet(17);
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

void SoruSekizo_RunEventSequence(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 p9b;
    s32 record;

    p9 = a0;
    p8 = a1;
    p10 = a2;
    Engine_EventBegin();
    Engine_AudioPlayCue(185);
    Call3(Engine_ActorSetSpeed, p9, 0x3333, 0x1999);
    Engine_ActorSetSpeed(0, 0x3333, 0x1999);
    *(u8 *)((s32)Engine_ActorGet(p9) + 90) &= 254;
    Engine_ActorSetAnimation(0, 8);
    Engine_ActorSetDestination(0, ((a3 << 4) + 8), ((a4 << 4) + 8));
    p8b = ((s32)p8 << 4);
    p10b = ((s32)p10 << 4);
    Engine_ActorSetDestination(p9, (p8b + 8), (p10b + 8));
    Engine_ActorWaitForMove(p9);
    Engine_ActorSetAnimation(0, 1);
    Engine_EventEnd();
    p9b = ((a3 << 4) + 8);
    p10c = ((a4 << 4) + 8);
}

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z)
{
    s32 *p = Engine_ActorGet(no);
    if (p == NULL || (p[2] >> 20) != x) {
        return 0;
    }
    if ((p[4] >> 20) != z) {
        return 0;
    }
    return 1;
}

void SceneData_InitTableA980(void)
{
    s32 *p = (s32 *)&gSealScene;
    p[0] = 0;
    p[1] = 55;
    p[2] = 32;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 2;
    p[7] = 30;
    p[8] = 34;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 28;
    p[14] = 34;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 30;
    p[20] = 16;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80b;
    p[25] = 0x4000;
    p[26] = 500;
    p[27] = 132;
    p[28] = 8;
    p[29] = 55;
    p[30] = 32;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 2;
    p[35] = 30;
    p[36] = 34;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 28;
    p[42] = 16;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 9;
    p[47] = 488;
    p[48] = 152;
    SoruSekizo_OpenSeal();
}

void SceneData_FillTableA980(void)
{
    s32 *p = (s32 *)&gSealScene;
    p[0] = 4;
    p[1] = 55;
    p[2] = 36;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 4;
    p[7] = 30;
    p[8] = 36;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 28;
    p[14] = 36;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 30;
    p[20] = 18;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80c;
    p[25] = 0x4000;
    p[26] = 654;
    p[27] = 132;
    p[28] = 12;
    p[29] = 55;
    p[30] = 36;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 4;
    p[35] = 30;
    p[36] = 36;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 28;
    p[42] = 18;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 11;
    p[47] = 664;
    p[48] = 152;
    SoruSekizo_OpenSeal();
}

void SceneData_InitTableA980AndRunB(void)
{
    s32 *p = (s32 *)&gSealScene;
    p[0] = 0;
    p[1] = 58;
    p[2] = 32;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 2;
    p[7] = 31;
    p[8] = 34;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 29;
    p[14] = 34;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 31;
    p[20] = 16;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80d;
    p[25] = 0xc000;
    p[26] = 500;
    p[27] = 216;
    p[28] = 8;
    p[29] = 58;
    p[30] = 32;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 2;
    p[35] = 31;
    p[36] = 34;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 29;
    p[42] = 16;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 13;
    p[47] = 488;
    p[48] = 200;
    SoruSekizo_OpenSeal();
}

void SceneData_BuildTableA980(void)
{
    s32 *p = (s32 *)&gSealScene;
    p[0] = 4;
    p[1] = 58;
    p[2] = 36;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 4;
    p[7] = 31;
    p[8] = 36;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 29;
    p[14] = 36;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 31;
    p[20] = 18;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80e;
    p[25] = 0xc000;
    p[26] = 0x28e;
    p[27] = 216;
    p[28] = 12;
    p[29] = 58;
    p[30] = 36;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 4;
    p[35] = 31;
    p[36] = 36;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 29;
    p[42] = 18;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 15;
    p[47] = 664;
    p[48] = 200;
    SoruSekizo_OpenSeal();
}
