#include "EDITION.H"
#include "PROBE.H"
#include "CALL.H"
#include "TYPES.H"
#include "MAKYURI.H"

extern const u16 MakyuriHeya_FloorSwitchCloseCells[];

void MakyuriHeya_WalkLeaderIn(void)
{
    u8 *work;

    work = (u8 *)gEventWork;
    Engine_EventBegin();
    Engine_TaskAddCallback((s32)SceneEffect_SpawnParticleEveryFourthFrame, 0xc80);
    ObjectMotion_SetSpeedParameters(0, 0x28000, 0x14000);
    Object_SetModeById(0, 1);
    Object_GetById(0)->unknown_5a &= 254;
    Audio_PlayCue(228);
    if (*(s16 *)(work + 0x16c) == 2) {
        Engine_ActorSetDestination(0, 232, 616);
    } else if (*(s16 *)(work + 0x16c) == 3) {
        Engine_ActorSetDestination(0, 360, 728);
    } else if (*(s16 *)(work + 0x16c) == 4) {
        Engine_ActorSetDestination(0, 248, 792);
    } else {
#if EDITION_INTERNATIONAL
        Call3(Engine_ActorMoveToAndWait, 0, 696, 592);
#else
        Call3(Engine_ActorSetDestination, 0, 696, 592);
#endif
#if EDITION_INTERNATIONAL
        Engine_ActorSetDestination(0, 696, 600);
        Battle_WaitMode0(30);
#endif
    }
    ObjectMotion_CommitCurrentPositionAndActivate(0);
    SetFlagBits(&Object_GetById(0)->unknown_5a, 1);
    Engine_TaskRemoveCallback((s32)SceneEffect_SpawnParticleEveryFourthFrame);
    Engine_EventEnd();
}

void FieldScene_RunFourCallSequence(void)
{
    Engine_EventBegin();
    StagedActor_AdvancePair();
    Engine_EventEnd();
    FieldScene_RunActorElevenAtTile5And13();
}

void FieldScene_RunActorElevenAtTile5And13(void)
{
    s32 x;
    s32 y;
    u8 *p;

    x = FIELD_AT_OFFSET(Object_GetById(11), s32, 8) / 0x100000;
    y = FIELD_AT_OFFSET(Object_GetById(11), s32, 16) / 0x100000;
    Engine_EventBegin();
    if (x == 5 && y == 13) {
        FIELD_AT_OFFSET(Object_GetById(11), s32, 12) += 0xfffe0000;
        p = Object_GetById(11);
        FIELD_AT_OFFSET(p, s32, 0x3c) = FIELD_AT_OFFSET(Object_GetById(11), s32, 12);
        Engine_MapCopyCellsTo(5, 2, 5, 11, 1, 1);
        Audio_PlayCue(0xd9);
        Engine_MapAnimateCells(MakyuriHeya_GateCells, 9, 7);
        {
            s32 s0 = 9;
            s32 s1 = 10;
            Map_CopyCellAttributeRect(9, 5, 1, 1, s0, s1);
        }
        Engine_GameFlagSet(0x874);
    }
    Engine_EventEnd();
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
            Engine_EventBegin();
            Engine_GameFlagSet(0x256);
            Battle_WaitMode0(5);
            Object_GetById(0)->y.fixed += -0x20000;
            actor = Object_GetById(0);
            FIELD_AT_OFFSET(actor, s32, 0x3c) = Object_GetById(0)->y.fixed;
            Engine_MapCopyCellsTo(5, 2, 5, 11, 1, 1);
            Audio_PlayCue(217);
            Engine_MapAnimateCells((s32)MakyuriHeya_GateCells, 9, 7);
            Engine_EventEnd();
        }
    }
}

void MakyuriHeya_CloseFloorSwitch(void)
{
    struct FieldActor *actor;

    if (Engine_GameFlagIsSet(0x256) != 0) {
        Engine_EventBegin();
        Engine_GameFlagClear(0x256);
        Object_GetById(0)->y.fixed += 0x20000;
        actor = Object_GetById(0);
        FIELD_AT_OFFSET(actor, s32, 0x3c) = Object_GetById(0)->y.fixed;
        Battle_WaitMode0(5);
        Engine_MapCopyCellsTo(7, 2, 5, 11, 1, 1);
        Audio_PlayCue(217);
        Engine_MapAnimateCells(MakyuriHeya_GateCloseCells, 9, 7);
        Engine_EventEnd();
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
            Engine_MapCopyCellsTo(45 - (s32)(i << 1), 32, 44 - (s32)(i << 1), 32, (s32)(i + 1), 6);
            Engine_MapCopyCellsTo(45 - (s32)i, 51, 45 - (s32)i, 32, 1, 6);
            v = 109 - (s32)i;
            Engine_MapCopyCellsTo(v, 32, 108 - (s32)i, 32, 1, 4);
            Engine_MapCopyCellsTo(v, 51, v, 32, 1, 4);
            if (a0 != 0) {
                Engine_WorkSetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
                Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
                Battle_WaitMode0(a0);
            }
            i = i + 1;
        } while (i < (u32)a2);
    }
    Map_CopyCellAttributeRect(42, 52, 4, 5, 42, 33);
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
        Engine_MapCopyCellsTo(y, 32, x, 32, 3 - (s32)i, six);
        k = 2;
        Engine_MapCopyCellsTo(39, 51, y, 32, 1, six);
        pos = (s32)i + 106;
        four = 4;
        Engine_MapCopyCellsTo(105, 51, pos, 32, k, four);
        if (a0 != 0) {
            Engine_WorkSetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
            Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
            Battle_WaitMode0(a0);
        }
        i = i + 1;
        x = x + 2;
        y = y + 2;
    } while (i <= 2);
    Audio_PlayCue(0x120);
    Map_CopyCellAttributeRect(106, 33, 4, 5, 42, 33);
    Engine_MapRenderWaitForValues();
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

    Engine_MapCopyCellsTo(78, 59, 110, 36, 1, 1);
    Engine_MapCopyCellsTo(76, 59, 109, 36, 1, 1);
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
                va0 = Engine_RandomNext();
                va = (((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) + ((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                neg = -inner;
                vb0 = Engine_RandomNext();
                vb = (((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) + ((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                Effect_Spawn(v8, 0, 0x2480000, va, 0, vb, 0x90000, (s32)rec);
                Battle_WaitMode0(1);
            } else {
                neg = -inner;
            }
            SceneEffect_SpawnRandomEveryFourFrames(((s32)((s32)(neg - shift4) << 16) + 0x2d80000), 0, 0x2480000);
            inner = (inner + 1);
            v8 = (v8 + -0x10000);
        } while ((u32)inner <= 7);
        Engine_MapCopyCellsTo(76, 59, (108 - outer), 36, 2, 1);
        next = outer + 1;
        FieldScene_RunOpeningAuxiliarySequence(a0, outer, next);
        outer = next;
    } while ((u32)next <= 1);
    Battle_WaitMode0(a0);
    FieldScene_RunOpeningAuxiliarySequence(0, next, (next + 1));
    Audio_PlayCue(211);
    Engine_TaskAddCallback((s32)MakyuriHeya_SprayAtFountain, 0xc80);
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

    Engine_MapCopyCellsTo(78, 58, 110, 36, 1, 1);
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
                raw = Engine_RandomNext();
                shown = ((0x248 - (s32)((u32)((raw << 2) + raw) >> 16)) & 0xffff) << 16;
                pos = (base - (outer << 19)) + 0x2d80000;
                Effect_Spawn(pos, 0, shown, -0x4000, 0, 0, 0x90000, (s32)rec);
                Battle_WaitMode0(1);
            }
            inner = inner + 1;
            base = base + -0x20000;
        } while ((u32)inner <= 7);
        Engine_MapCopyCellsTo(111, 35, (109 - outer), 36, 1, 1);
        outer = outer + 1;
    } while ((u32)outer <= 2);
    Engine_TaskRemoveCallback((s32)MakyuriHeya_SprayAtFountain);
}

void MakyuriHeya_RunSequenceE(void)
{
    SceneEvent ev;
    s32 kind;
    s32 shape;
    s32 three;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&ev) != 0) {
        kind = ev.f1;
        if (kind == 8) {
            shape = ev.f2;
            if ((shape >> 20) == 11) {
                SceneActor_MoveAndRedraw(ev);
                Battle_WaitMode0(30);
                Audio_PlayCue(211);
                SceneEffect_SpawnParticleRowsAndDrawTiles();
                three = 3;
                Engine_MapCopyCellsTo(76, 60, 74, 38, three, 1);
                Engine_MapCopyCellsTo(77, 60, 76, 38, 2, 1);
                Engine_MapCopyCellsTo(75, 58, 86, 41, 1, three);
                Engine_MapCopyCellsTo(75, 59, 86, 43, 1, 2);
                Engine_MapCopyCellsTo(76, 59, 80, 49, 2, 1);
                Engine_MapCopyCellsTo(77, 59, 82, 49, 2, 1);
                Engine_GameFlagSet(0x302);
            } else {
                ev.t1 = (s32)MakyuriHeya_OpenThreeStepStair;
                Engine_MapCopyCellsTo(75, 57, 86, 41, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 86, 42, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 86, 43, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 86, 44, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 80, 49, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 81, 49, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 82, 49, 1, 1);
                Engine_MapCopyCellsTo(78, 58, 83, 49, 1, 1);
                SceneActor_MoveAndRedraw(ev);
                Engine_GameFlagClear(0x302);
            }
        } else if (kind == 10) {
            if ((ev.t0 >> 20) == 40) {
                SceneActor_MoveAndRedraw(ev);
                if (Engine_GameFlagIsSet(0x307) == 0) {
                    Engine_CameraSetSpeed(0x18000, 0x3000);
                    Engine_CameraMoveTo(0x2ca0000, -1, 0x2500000, 1);
                    Engine_CameraWaitForMove();
                    Engine_GameFlagSet(0x307);
                    FieldScene_RunSupplementalSequenceOne(5);
                    Battle_WaitMode0(50);
                } else {
                    FieldScene_RunSupplementalSequenceOne(5);
                }
                Engine_GameFlagSet(0x306);
            } else if ((ev.t0 >> 20) == 42) {
                ev.t1 = (s32)MakyuriHeya_RunSequenceF;
                SceneActor_MoveAndRedraw(ev);
                MakyuriHeya_RunSequenceG(5);
                Engine_GameFlagClear(0x306);
            }
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunFourSteps(void)
{
    Engine_EventBegin();
    StagedActor_AdvancePair();
    Engine_EventEnd();
    SceneState_RunWhenActor8AtTile10x23();
}

void SceneState_RunWhenActor8AtTile10x23(void)
{
    s32 x = Object_GetById(8)->x.fixed / 0x100000;
    s32 y = Object_GetById(8)->z.fixed / 0x100000;

    Engine_EventBegin();
    if (x == 10 && y == 23) {
        s32 *p;
        Object_GetById(8)->y.fixed += 0xfffe0000;
        p = (s32 *)Object_GetById(8);
        p[15] = Object_GetById(8)->y.fixed;
        Engine_MapCopyCellsTo(6, 29, 10, 23, 1, 1);
        Audio_PlayCue(0xd9);
        Engine_MapAnimateCells(MakyuriHeya_FloorSwitchCells, 10, 18);
        Map_CopyCellAttributeRect(10, 16, 1, 1, x, 19);
        Engine_GameFlagSet(0x878);
    }
    Engine_EventEnd();
}

/* Once, when the leader stands in the cell block at x 164-171 and z
 * 372-379, lifts him by two pixels, opens the passage and animates it. */
void MakyuriHeya_TriggerFloorSwitch(void)
{
    s32 x;
    s32 z;

    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x256) != 0)
        return;
    x = Object_GetById(0)->x.part.pixel;
    z = Object_GetById(0)->z.part.pixel;
    if (x < 164 || x > 171)
        return;
    if (z < 372)
        return;
    if (z >= 380)
        return;
    Engine_EventBegin();
    Engine_GameFlagSet(0x256);
    Battle_WaitMode0(5);
    Object_GetById(0)->y.fixed -= 0x20000;
    Object_GetById(0)->target_y = Object_GetById(0)->y.fixed;
    Engine_MapCopyCellsTo(6, 29, 10, 23, 1, 1);
    Audio_PlayCue(217);
    Engine_MapAnimateCells(((u16 *)MakyuriHeya_FloorSwitchCells), 10, 18);
    Engine_EventEnd();
}

void FieldScene_RunScene39cSequenceA(void)
{
    struct FieldActor *actor;

    if (Engine_GameFlagIsSet(0x256) != 0) {
        Engine_EventBegin();
        Engine_GameFlagClear(0x256);
        Object_GetById(0)->y.fixed += 0x20000;
        actor = Object_GetById(0);
        FIELD_AT_OFFSET(actor, s32, 0x3c) = Object_GetById(0)->y.fixed;
        Battle_WaitMode0(5);
        Engine_MapCopyCellsTo(8, 29, 10, 23, 1, 1);
        Audio_PlayCue(217);
        Engine_MapAnimateCells(MakyuriHeya_FloorSwitchCloseCells, 10, 18);
        Engine_EventEnd();
    }
}

/* A random drift of about -0.8 to +0.8 in steps of 0.2. */
static __inline__ s32 Door_DriftStep(void)
{
    u32 drift = (u32)Engine_RandomNext();

    return ((drift << 3) >> 16) * 0x3333;
}

/* Slides one of the three stone doors two cells open, a cell at a time, with
 * dust drifting off its moving edge. */
void MakyuriHeya_OpenStoneDoor(s32 side)
{
    struct EffectOptions params;
    u32 j;
    struct EffectOptions *p;
    u32 i;

    Audio_PlayCue(211);
    if (side == 0) {
        Engine_MapCopyCellsTo(111, 57, 113, 42, 1, 1);
        Engine_MapCopyCellsTo(111, 59, 113, 43, 1, 1);
    } else if (side == 1) {
        Engine_MapCopyCellsTo(113, 58, 112, 46, side, side);
        Engine_MapCopyCellsTo(115, 58, 113, 46, side, side);
    } else {
        Engine_MapCopyCellsTo(115, 57, 116, 44, 1, 1);
        Engine_MapCopyCellsTo(113, 57, 115, 44, 1, 1);
    }
    p = &params;
    p->palette = 7;
    p->start_scale_x = 0x8000;
    p->start_scale_y = 0x8000;
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 7; j++) {
            if (j & 1) {
                if (side == 0) {
                    Effect_Spawn(0x3180000, 0, 0x2c00000 + (i * 16 + j) * 0x10000, Door_DriftStep() + -0xcccc, 0,
                                 Door_DriftStep() + -0xcccc, 0x90000, p);
                } else if (side == 1) {
                    Effect_Spawn(0x3200000 + (i * 16 + j) * 0x10000, 0, 0x2ea0000, Door_DriftStep() + -0xcccc, 0,
                                 Door_DriftStep() + -0xcccc, 0x90000, p);
                } else {
                    Effect_Spawn(0x32c0000 + (i * 16 + j) * -0x10000, 0, 0x2ca0000, Door_DriftStep() + -0xcccc, 0,
                                 Door_DriftStep() + -0xcccc, 0x90000, p);
                }
                Battle_WaitMode0(1);
            }
        }
        if (side == 0) {
            Engine_MapCopyCellsTo(111, 58, 113, i + 43, 1, 1);
            Engine_MapCopyCellsTo(111, 59, 113, i + 44, 1, 1);
        } else if (side == 1) {
            Engine_MapCopyCellsTo(114, 58, i + 113, 46, side, side);
            Engine_MapCopyCellsTo(115, 58, i + 114, 46, side, side);
        } else {
            Engine_MapCopyCellsTo(114, 57, 115 - i, 44, 1, 1);
            Engine_MapCopyCellsTo(113, 57, 114 - i, 44, 1, 1);
        }
    }
}
