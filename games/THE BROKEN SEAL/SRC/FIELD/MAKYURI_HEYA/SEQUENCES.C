#include "PROBE.H"

void MakyuriHeya_WalkLeaderIn(void)
{
    u8 *work;

    work = (u8 *)gEventWork;
    Event_Begin();
    Value2(Engine_TaskAddCallback, (s32)SceneEffect_SpawnParticleEveryFourthFrame, 0xc80);
    Actor_SetSpeed(0, 0x28000, 0x14000);
    Actor_SetAnimation(0, 1);
    Object_GetById(0)->unknown_5a &= 254;
    Audio_PlayCue(228);
    if (*(s16 *)(work + 0x16c) == 2) {
        Actor_SetDestination(0, 232, 616);
    } else if (*(s16 *)(work + 0x16c) == 3) {
        Actor_SetDestination(0, 360, 728);
    } else if (*(s16 *)(work + 0x16c) == 4) {
        Actor_SetDestination(0, 248, 792);
    } else {
        Call3(Engine_ActorMoveToAndWait, 0, 696, 592);
        Actor_SetDestination(0, 696, 600);
        Event_Wait(30);
    }
    Actor_WaitForMove(0);
    SetFlagBits(&Object_GetById(0)->unknown_5a, 1);
    Call1(Engine_TaskRemoveCallback, (s32)SceneEffect_SpawnParticleEveryFourthFrame);
    Event_End();
}

void FieldScene_RunFourCallSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    FieldScene_RunActorElevenAtTile5And13();
}

void FieldScene_RunActorElevenAtTile5And13(void)
{
    s32 x;
    s32 y;
    u8 *p;

    x = FIELD_AT_OFFSET(Actor_Get(11), s32, 8) / 0x100000;
    y = FIELD_AT_OFFSET(Actor_Get(11), s32, 16) / 0x100000;
    Event_Begin();
    if (x == 5 && y == 13) {
        FIELD_AT_OFFSET(Actor_Get(11), s32, 12) += 0xfffe0000;
        p = Actor_Get(11);
        FIELD_AT_OFFSET(p, s32, 0x3c) = FIELD_AT_OFFSET(Actor_Get(11), s32, 12);
        Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
        Audio_PlayCue(0xd9);
        Map_AnimateCells(MakyuriHeya_GateCells, 9, 7);
        {
            s32 s0 = 9;
            s32 s1 = 10;
            Map_CopyCellAttributes(9, 5, 1, 1, s0, s1);
        }
        GameFlag_Set(0x874);
    }
    Event_End();
}

void MakyuriHeya_TriggerSmallFloorSwitch(void)
{
    s32 x;
    s32 z;
    struct FieldActor *actor;

    if (Value1(Engine_GameFlagIsSet, 0x256) == 0) {
        x = Object_GetById(0)->x.part.pixel;
        z = Object_GetById(0)->z.part.pixel;
        if ((u32)(x - 84) <= 7 && z > 211 && z <= 219) {
            Event_Begin();
            Call1(Engine_GameFlagSet, 0x256);
            Event_Wait(5);
            Object_GetById(0)->y.fixed += -0x20000;
            actor = Object_GetById(0);
            FIELD_AT_OFFSET(actor, s32, 0x3c) = Object_GetById(0)->y.fixed;
            Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
            Audio_PlayCue(217);
            Call3(Engine_MapAnimateCells, (s32)MakyuriHeya_GateCells, 9, 7);
            Event_End();
        }
    }
}

void MakyuriHeya_CloseFloorSwitch(void)
{
    struct FieldActor *actor;

    if (GameFlag_IsSet(0x256) != 0) {
        Event_Begin();
        GameFlag_Clear(0x256);
        Actor_Get(0)->y.fixed += 0x20000;
        actor = Actor_Get(0);
        FIELD_AT_OFFSET(actor, s32, 0x3c) = Actor_Get(0)->y.fixed;
        Event_Wait(5);
        Map_CopyCellsTo(7, 2, 5, 11, 1, 1);
        Audio_PlayCue(217);
        Map_AnimateCells(MakyuriHeya_GateCloseCells, 9, 7);
        Event_End();
    }
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 v;

    if (a0 != 0) {
        Audio_PlayCue(219);
    }
    i = (u32)a1;
    if (i < (u32)a2) {
        do {
            Map_CopyCellsTo(45 - (s32)(i << 1), 32, 44 - (s32)(i << 1), 32, (s32)(i + 1), 6);
            Map_CopyCellsTo(45 - (s32)i, 51, 45 - (s32)i, 32, 1, 6);
            v = 109 - (s32)i;
            Map_CopyCellsTo(v, 32, 108 - (s32)i, 32, 1, 4);
            Map_CopyCellsTo(v, 51, v, 32, 1, 4);
            if (a0 != 0) {
                Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
                Work_SetValuesIfNonNegative(-1, -1, 0xe666);
                Event_Wait(a0);
            }
            i = i + 1;
        } while (i < (u32)a2);
    }
    Map_CopyCellAttributes(42, 52, 4, 5, 42, 33);
}

void MakyuriHeya_RunSequenceG(s32 a0)
{
    u32 i;
    s32 x;
    s32 y;
    s32 k;
    s32 six;
    s32 pos;
    s32 four;
    Audio_PlayCue(219);
    six = 6;
    i = 0;
    x = 41;
    y = 40;
    do {
        Map_CopyCellsTo(y, 32, x, 32, 3 - (s32)i, six);
        k = 2;
        Map_CopyCellsTo(39, 51, y, 32, 1, six);
        pos = (s32)i + 106;
        four = 4;
        Engine_MapCopyCellsTo(105, 51, pos, 32, k, four);
        if (a0 != 0) {
            Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(a0);
        }
        i = i + 1;
        x = x + 2;
        y = y + 2;
    } while (i <= 2);
    Audio_PlayCue(0x120);
    Map_CopyCellAttributes(106, 33, 4, 5, 42, 33);
    MapRender_WaitForValues();
}

void MakyuriHeya_SprayAtFountain(void)
{
    SceneEffect_SpawnRandomEveryFourFrames(0x2b20000, 0, 0x2480000);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    s32 *rec;
    s32 outer;
    s32 shift4;
    s32 v8;
    s32 inner;
    s32 neg;
    s32 nsh;
    s32 va0;
    s32 vb0;
    s32 va;
    s32 vb;
    s32 next;
    s32 slot20[10];

    Map_CopyCellsTo(78, 59, 110, 36, 1, 1);
    Map_CopyCellsTo(76, 59, 109, 36, 1, 1);
    rec = slot20;
    rec[1] = 7;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        shift4 = (outer << 4);
        inner = 0;
        nsh = -(outer << 20);
        v8 = (0x2d80000 + nsh);
        do {
            if ((inner & 1) != 0) {
                va0 = Random_Next();
                va = (((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) + ((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                neg = -inner;
                vb0 = Random_Next();
                vb = (((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) + ((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                Effect_Spawn(v8, 0, 0x2480000, va, 0, vb, 0x90000, (s32)rec);
                Event_Wait(1);
            } else {
                neg = -inner;
            }
            SceneEffect_SpawnRandomEveryFourFrames(((s32)((s32)(neg - shift4) << 16) + 0x2d80000), 0, 0x2480000);
            inner = (inner + 1);
            v8 = (v8 + -0x10000);
        } while ((u32)inner <= 7);
        Map_CopyCellsTo(76, 59, (108 - outer), 36, 2, 1);
        next = outer + 1;
        FieldScene_RunOpeningAuxiliarySequence(a0, outer, next);
        outer = next;
    } while ((u32)next <= 1);
    Event_Wait(a0);
    FieldScene_RunOpeningAuxiliarySequence(0, next, (next + 1));
    Audio_PlayCue(211);
    Value2(Engine_TaskAddCallback, (s32)MakyuriHeya_SprayAtFountain, 0xc80);
    Engine_MapRenderWaitForValues();
}

void MakyuriHeya_RunSequenceF(void)
{
    s32 *rec;
    s32 outer;
    s32 inner;
    s32 base;
    s32 raw;
    s32 pos;
    s32 shown;
    s32 arr[10];

    Map_CopyCellsTo(78, 58, 110, 36, 1, 1);
    rec = arr;
    rec[1] = 5;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        base = -0x20000;
        inner = 1;
        do {
            if ((inner & 1) != 0) {
                raw = Value0(Engine_RandomNext);
                shown = ((0x248 - (s32)((u32)((raw << 2) + raw) >> 16)) & 0xffff) << 16;
                pos = (base - (outer << 19)) + 0x2d80000;
                Effect_Spawn(pos, 0, shown, -0x4000, 0, 0, 0x90000, (s32)rec);
                Event_Wait(1);
            }
            inner = inner + 1;
            base = base + -0x20000;
        } while ((u32)inner <= 7);
        Map_CopyCellsTo(111, 35, (109 - outer), 36, 1, 1);
        outer = outer + 1;
    } while ((u32)outer <= 2);
    Call1(Engine_TaskRemoveCallback, (s32)MakyuriHeya_SprayAtFountain);
}

void MakyuriHeya_RunSequenceE(void)
{
    SceneEvent ev;
    s32 kind;
    s32 shape;
    s32 three;

    Event_Begin();
    if (StagedActor_FindClearPosition(&ev) != 0) {
        kind = ev.f1;
        if (kind == 8) {
            shape = ev.f2;
            if ((shape >> 20) == 11) {
                SceneActor_MoveAndRedraw(ev);
                Event_Wait(30);
                Audio_PlayCue(211);
                SceneEffect_SpawnParticleRowsAndDrawTiles();
                three = 3;
                Map_CopyCellsTo(76, 60, 74, 38, three, 1);
                Map_CopyCellsTo(77, 60, 76, 38, 2, 1);
                Map_CopyCellsTo(75, 58, 86, 41, 1, three);
                Map_CopyCellsTo(75, 59, 86, 43, 1, 2);
                Map_CopyCellsTo(76, 59, 80, 49, 2, 1);
                Map_CopyCellsTo(77, 59, 82, 49, 2, 1);
                Engine_GameFlagSet(0x302);
            } else {
                ev.t1 = (s32)MakyuriHeya_OpenThreeStepStair;
                Map_CopyCellsTo(75, 57, 86, 41, 1, 1);
                Map_CopyCellsTo(71, 59, 86, 42, 1, 1);
                Map_CopyCellsTo(71, 59, 86, 43, 1, 1);
                Map_CopyCellsTo(71, 59, 86, 44, 1, 1);
                Map_CopyCellsTo(71, 59, 80, 49, 1, 1);
                Map_CopyCellsTo(71, 59, 81, 49, 1, 1);
                Map_CopyCellsTo(71, 59, 82, 49, 1, 1);
                Map_CopyCellsTo(78, 58, 83, 49, 1, 1);
                SceneActor_MoveAndRedraw(ev);
                GameFlag_Clear(0x302);
            }
        } else if (kind == 10) {
            if ((ev.t0 >> 20) == 40) {
                SceneActor_MoveAndRedraw(ev);
                if (GameFlag_IsSet(0x307) == 0) {
                    Camera_SetSpeed(0x18000, 0x3000);
                    Camera_MoveTo(0x2ca0000, -1, 0x2500000, 1);
                    Camera_WaitForMove();
                    GameFlag_Set(0x307);
                    FieldScene_RunSupplementalSequenceOne(5);
                    Event_Wait(50);
                } else {
                    FieldScene_RunSupplementalSequenceOne(5);
                }
                GameFlag_Set(0x306);
            } else if ((ev.t0 >> 20) == 42) {
                ev.t1 = (s32)MakyuriHeya_RunSequenceF;
                SceneActor_MoveAndRedraw(ev);
                MakyuriHeya_RunSequenceG(5);
                GameFlag_Clear(0x306);
            }
        }
    }
    Event_End();
}

void FieldScene_RunFourSteps(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    SceneState_RunWhenActor8AtTile10x23();
}

void SceneState_RunWhenActor8AtTile10x23(void)
{
    s32 x = Object_GetById(8)->x.fixed / 0x100000;
    s32 y = Object_GetById(8)->z.fixed / 0x100000;

    Event_Begin();
    if (x == 10 && y == 23) {
        s32 *p;
        Object_GetById(8)->y.fixed += 0xfffe0000;
        p = (s32 *)Actor_Get(8);
        p[15] = Object_GetById(8)->y.fixed;
        Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
        Audio_PlayCue(0xd9);
        Map_AnimateCells(MakyuriHeya_FloorSwitchCells, 10, 18);
        Map_CopyCellAttributes(10, 16, 1, 1, x, 19);
        GameFlag_Set(0x878);
    }
    Event_End();
}
